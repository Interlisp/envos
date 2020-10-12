;;;-*-Mode:LISP; Package: PCL; Base:10; Syntax:Common-lisp -*-;;;;;; *************************************************************************;;; Copyright (c) 1985, 1986, 1987, 1988 Xerox Corporation.;;; All rights reserved.;;;;;; Use and copying of this software and preparation of derivative works;;; based upon this software are permitted.  Any distribution of this;;; software or derivative works must comply with all applicable United;;; States export control laws.;;; ;;; This software is made available AS IS, and Xerox Corporation makes no;;; warranty about the software, its performance or its conformity to any;;; specification.;;; ;;; Any person obtaining a copy of this software is requested to send their;;; name and post office or electronic mail address to:;;;   CommonLoops Coordinator;;;   Xerox PARC;;;   3333 Coyote Hill Rd.;;;   Palo Alto, CA 94304;;; (or send Arpanet mail to CommonLoops-Coordinator.pa@Xerox.arpa);;;;;; Suggestions, comments and requests for improvements are also welcome.;;; *************************************************************************;;;(in-package 'pcl);;;; ;;;;;;(defmacro DEFCLASS (name includes slots &rest options)  (declare (indentation 2 4 3 1))  (let ((metaclass 'standard-class))    ;; Now go see if there is a :metaclass option.  We need that before we    ;; can do anything else.  If there is a :metaclass option, we remove it    ;; from the options -- it isn't needed anymore since the class-protype    ;; communicates the same information.    (dolist (option options)      (if (not (listp option))          (error "~S is not a legal defclass option." option)          (when (eq (car option) ':metaclass)            (unless (legal-class-name-p (cadr option))              (error "The value of the :metaclass option (~S) is not a~%~                       legal class name."                     (cadr option)))            (unless (find-class (cadr option) nil)              (error "The value of the :metaclass option (~S) does not name~%~                      a currently defined class.  The metaclass must be~%~                      defined at the time the defclass form is compiled or~%~                      evaluated."                     (cadr option)))            (setq metaclass (cadr option)                  options (remove option options)))))    (let ((prototype-class (class-prototype (find-class metaclass))))      (make-top-level-form `(defclass ,name)			   '(compile load eval)	(expand-defclass prototype-class name includes slots options)))));;;;;;;;;(defmethod EXPAND-DEFCLASS ((proto standard-class) name includes slots options)  (let ((*initfunctions* ()))    (declare (special *initfunctions*))    (let ((canonical-slots	    (mapcar #'(lambda (spec)			(canonicalize-slot-specification name spec))		    slots))	  (canonical-options	    (mapcar #'(lambda (option)			(canonicalize-defclass-option name option))		    options)))      `(let ,(mapcar #'cdr *initfunctions*)	 (load-defclass ',(class-name (class-of proto))			',name			',includes			(list ,@canonical-slots)			(list ,@canonical-options))))))(defun make-initfunction (initform)  (declare (special *initfunctions*))  (cond ((or (eq initform 't)	     (equal initform ''t))	 '(function true))	((or (eq initform 'nil)	     (equal initform ''nil))	 '(function false))	((or (eql initform '0)	     (equal initform ''0))	 '(function zero))	(t	 (let ((entry (assoc initform *initfunctions* :test #'equal)))	   (unless entry	     (setq entry (list initform			       (gensym)			       `(function (lambda () ,initform))))	     (push entry *initfunctions*))	   (cadr entry)))))(defun canonicalize-slot-specification (class-name spec)  (cond ((and (symbolp spec)	      (not (keywordp spec))	      (not (memq spec '(t nil))))		   	 `'(:name ,spec))	((not (consp spec))	 (error "~S is not a legal slot specification." spec))	((null (cdr spec))	 `'(:name ,(car spec)))	((null (cddr spec))	 (warn "In DEFCLASS ~S, the slot specification ~S is obsolete.~%~                Convert it to ~S"	       class-name spec (list (car spec) :initform (cadr spec)))	 `(list :name ',(car spec)		:initform ',(cadr spec)		:initfunction ,(make-initfunction (cadr spec))))	(t	 (let* ((name (pop spec))		(unsupplied (list nil))		(initform (getf spec :initform unsupplied)))	   (if (eq initform unsupplied)	       `(list* :name ',name ',spec)	       `(list* :name ',name		       :initfunction ,(make-initfunction initform)		       ',spec))))))						(defun canonicalize-defclass-option (class-name option)  (declare (ignore class-name))  (case (car option)    (:default-initargs      (let ((canonical ()))	(let (key val (tail (cdr option)))	  (loop (when (null tail) (return nil))		(setq key (pop tail)		      val (pop tail))		(push ``(,',key ,,(make-initfunction val) ,',val) canonical))	  ``(:default-initargs ,,@(nreverse canonical)))))    (otherwise      ``,',option)))(defun load-defclass (metaclass-name class-name superclasses slots options)  (record-definition 'class class-name)  (let ((proto (class-prototype (find-class metaclass-name))))    (setq superclasses (copy-list superclasses)	;I know that add-named-class	  slots (copy-tree slots)		;and company are NOT allowed	  options (copy-tree options))		;to mutate these.  But since						;CL is allowed to share lists						;in compiled files, why ask						;for hard to find bugs?    (add-named-class proto class-name superclasses slots options)));;;;;; ADD-NAMED-CLASS is the programmatic interface to defining named classes.;;; Think of it as a programmatic interface to defclass.  The arguments are;;; very slightly different, in particular the metaclass is communicated in;;; a different way, but other than that it works just like defclass.  After;;; all, defclass basically expands into a simple call to add-named-class.;;; ;;; ;;; ADD-NAMED-CLASS  proto-class name superclasses slots options;;;;;;  proto-class   is a prototype instance of the metaclass of;;;                the new class;;;;;;  name          the name for the new class;;;;;;  superclasses  a list of the superclasses of the new class.;;;                These can be symbols (class names) or actual;;;                class objects.;;;;;;  slots         descriptions of the slots for this class. This;;;                list has the same syntax and interpretation as;;;                the third argument to defclass.;;;;;;  options       the options for this class.  This list has the;;;                same syntax as the &rest options argument to;;;                defclass, except that the :metaclass option is;;;                not allowed.  The metaclass option would have;;;                no meaning since it is communicated in the;;;                proto-class argument.;;; ;;;;;; add-named-class creates or updates the definition of a named class.  If;;; there is already a class with the given name, class-for-redefinition is;;; called to get the class object to use for the new definition.;;;;;; Once it has a class object this calls add-class to cause the class lattice;;; to be updated to take the new definition into account.  Then stores this;;; class in the class name table.;;;(defmethod ADD-NAMED-CLASS	   ((proto standard-class) name superclasses slots options)  (unless (legal-class-name-p name)    (error "~S is not a legal class name (a non-keyword, non-nil symbol.)"           name))  (let* ((existing (find-class name nil))         (class (if existing		    (class-for-redefinition		      existing proto name superclasses slots options)		    (make-instance (class-of proto))))	 (slotds ()))    ;;    ;; Error check the options and then parse the slots.  We error check    ;; the options first because there is no use doing a bunch of work    ;; parsing the slots if we are just going to signal an error about the    ;; options.    ;;     (dolist (option options)      (unless (legal-class-option-p class (car option))	(error "~S is not a legal class-option." (car option))))    (setq slotds (mapcar #'(lambda (x) (parse-class-slot class x)) slots))    ;;    ;; Set the class name before calling update-class.  This allows    ;; methods on update-class (or other code they call) to get at    ;; the class's name if they want.    ;;    (setf (class-name class) name)    (update-class class :direct-superclasses superclasses			:direct-slots slotds			:options options)    (setf (find-class name) class)    (inform-type-system-about-class class name)	;NOT part of standard protocol						;see comment at the defmethod						;for this.    class));;;;;; This is the programmatic (only) interface to updating the definition of;;; an anonymous class.;;;(defmethod UPDATE-CLASS ((class standard-class)			 &rest key-arguments			 &key (direct-superclasses () new-supers-p)			      (direct-slots () new-slots-p)			      (options () new-options-p)			 &allow-other-keys)  (flet ((translate-&key-arg (name new-name new-value)	   (let ((entry (memq name key-arguments)))	     (if entry		 (setf (car entry) new-name		       (cadr entry) new-value)		 (error "Can't find ~S in key-arguments?" name)))))        (let ((old-direct-superclasses (class-local-supers class))	  (old-forward-referenced-supers	    (class-forward-referenced-supers class))	  (new-forward-referenced-supers ())	  (*umi-gfs* ())	  (*umi-complete-classes* ())	  (*umi-reorder* ()))            (if* (null new-supers-p)	   (setq direct-superclasses (class-local-supers class))	   ;; The supers appear to have changed (new ones were specified).	   ;; Go through and convert names to actual class objects.  If	   ;; there is a name for a class that doesn't exist yet cons up	   ;; a forward-referenced class for that name.	   (setq direct-superclasses	     (or (mapcar		   #'(lambda (s)		       (cond ((classp s) s)			     ((not (legal-class-name-p s))			      (error "~S is not a class or a legal class name."				     s))			     (t			      (or (find-class s nil)				  (setf (find-class s)					(make-instance					  'forward-referenced-class					  :name s))))))		   direct-superclasses)		 (list (find-class 'object))))	   (setf (class-local-supers class) direct-superclasses)	   (dolist (nds direct-superclasses)	     (unless (memq nds old-direct-superclasses)	       (check-super-metaclass-compatibility class nds)	       (push class (class-direct-subclasses nds))))	   (dolist (ods old-direct-superclasses)	     (unless (memq ods direct-superclasses)	       (setf (class-direct-subclasses ods)		     (delq class (class-direct-subclasses ods)))))	   (translate-&key-arg :direct-superclasses			       :its-direct-superclasses			       direct-superclasses))      (setq new-forward-referenced-supers	    (gathering1 (collecting)	      (dolist (super direct-superclasses)		(when (forward-referenced-class-p super)		  (gather1 super)))))      (setf (class-forward-referenced-supers class)	    new-forward-referenced-supers)      (when new-options-p	;; The options appear to have changed (new ones were specified).	;; Use legal-class-option-p to be sure all the new options are	;; legal.	(setf (class-options class) options)	(translate-&key-arg :options :its-options options))            (when new-slots-p	;; The slots appear to have changed (new ones were specified).	;; Use legal-slot-option-p to be sure all the slot options are	;; legal.  Note that we don't check anything else about the	;; slot-descriptions, parse-class slot will have to signal an	;; error about that.	(setf (class-local-slots class) direct-slots)	(translate-&key-arg :direct-slots :its-direct-slots direct-slots))                  (apply #'propagate-class-update	     class	     (null new-forward-referenced-supers)	     (and (null old-forward-referenced-supers)		  old-direct-superclasses) class key-arguments)            )));;; ***                                                                 ***;;; *** This is a piece of shit.  I had to do it quickly to get around  ***;;; *** the defaulting of direct supers above.  Note that the new       ***;;; *** metaobject protocol solves this problem.                        ***;;; ***                                                                 ***(defmethod delete-class ((class standard-class))  (dolist (super (class-local-supers class))    (setf (class-direct-subclasses super)	  (delete class (class-direct-subclasses class)))))(defmethod class-default-initargs ((class standard-class))  (cdr (assq :default-initargs (class-options class))));;;;;;;;;(defvar *slotd-unsupplied* (list '*slotd-unsupplied*))(defmethod PARSE-CLASS-SLOT ((class standard-class) slot)  (let ((name nil)	(initfunction #'(lambda () *slot-unbound*))        (initform *slotd-unsupplied*);	(initvalue ())	(initargs ())        (readers ())	(writers ())        (allocation ':instance)        (type *slotd-unsupplied*))    (let ((plist slot)	  (key nil)	  (val nil))      (loop (when (null plist) (return t))	    (setq key (pop plist)		  val (pop plist))	    (unless (legal-slot-option-p class key)	      (error "In the slot specification ~S,~%~                      the option ~S is not legal."		     slot key))	    (case key	      (:name         (setq name val))	      (:initfunction (setq initfunction val))	      (:initform     (setq initform val));	      (:initvalue    (setq initvalue val))	      (:initarg      (push val initargs))	      (:accessor     (push val readers)			     (push `(setf ,val) writers))	      (:reader       (push val readers))	      (:writer       (push val writers))	      (:allocation   (setq allocation val))	      (:type         (setq type val))))    (make-slotd class		:name name		:keyword (make-keyword name)		:initform initform		:initfunction initfunction		:initargs initargs		:allocation (if (eq allocation :class) class allocation)		:type type		:readers readers		:writers writers))))(defmethod MAKE-SLOTD ((class standard-class) &rest keywords-and-options)  (apply #'make-instance 'standard-slot-description keywords-and-options))(defun merge-accessor/reader-prefixes (slotds class-options)                   (dolist (class-option class-options)    (flet ((do-prefix (slot-name)	     (if (cadr class-option)		 (symbol-append (cadr class-option) slot-name)		 slot-name)))      (when (listp class-option)	(case (car class-option)	  (:accessor-prefix	    (dolist (slotd slotds)	      (let ((name (do-prefix (slotd-name slotd))))		(pushnew name (slotd-readers slotd))		(pushnew `(setf ,name) (slotd-writers slotd)			 :test #'equal))))	  (:reader-prefix	    	    (dolist (slotd slotds)	      (let ((name (do-prefix (slotd-name slotd))))		(pushnew name (slotd-readers slotd))))))))));;;;;; The legal-class-option-p and legal-slot-option-p generic functions;;; use  OR method combination.  The method for a particular metaclass;;; should method should return T if and only if it recognizes the option;;; as one which it will accept responsibility for.;;;;;; Note that during bootstrapping, there is no OR combination so there;;; can't really be more than one of these methods.;;; (defmethod LEGAL-CLASS-OPTION-P ((class standard-class) option)  (memq option	'(:reader-prefix :accessor-prefix :documentation :default-initargs)))(defmethod LEGAL-SLOT-OPTION-P ((class standard-class) option)  (memq option '(:name :reader :writer :accessor :allocation		       :initarg :initform :initfunction :type :documentation)))#-(or KCL IBCL GCLisp Xerox)(defun class-constant-form (class)  (list 'quote class))#+(or KCL IBCL GCLisp Xerox)(let ((counter 0))(defun class-constant-form (class)  (let ((symbol (intern (format nil "class constant form ~A" (incf counter))			(find-package 'pcl))))    (set symbol class)    `(locally (declare (special ,symbol)) ,symbol))))  ;;;;;; NOTE: For bootstrapping considerations, these can't use make-instance;;;       to make the method object.  They have to use make-a-method which;;;       is a specially bootstrapped mechanism for making standard methods.;;;(defmethod ADD-READER-METHOD ((class standard-class) slotd generic-function)  (let* ((name (class-name class))	 (method (make-a-method 'standard-reader-method				()				(list (or name 'object))				(list class)				(make-reader-method-function class slotd)				"automatically generated reader method"				(slotd-name slotd))))    (add-method generic-function method)))(defmethod ADD-WRITER-METHOD ((class standard-class) slotd generic-function)  (let* ((name (class-name class))	 (method (make-a-method 'standard-writer-method				()				(list 'new-value (or name 'object))				(list *the-class-t* class)				(make-writer-method-function class slotd)				"automatically generated writer method"				(slotd-name slotd))))    (add-method generic-function method)))(defmethod REMOVE-READER-METHOD ((class standard-class) slotd generic-function)  (declare (ignore slotd))  (let ((method (get-method generic-function () (list class) nil)))    (when method (remove-method generic-function method))))(defmethod REMOVE-WRITER-METHOD ((class standard-class) slotd generic-function)  (declare (ignore slotd))  (let ((method	  (get-method generic-function () (list *the-class-t* class) nil)))    (when method (remove-method generic-function method))));;;;;; make-reader-method-function and make-write-method function are NOT part of;;; the standard protocol.  They are however useful, PCL makes uses makes use;;; of them internally and documents them for PCL users.;;;;;; NOTE: bootstrap-accessor-definitions calls the templated function;;;       constructors directly.;;;;;; *** This needs work to make type testing by the writer functions which;;; *** do type testing faster.  The idea would be to have one constructor;;; *** for each possible type test.  In order to do this it would be nice;;; *** to have help from inform-type-system-about-class and friends.;;;;;; *** There is a subtle bug here which is going to have to be fixed.;;; *** Namely, the simplistic use of the template has to be fixed.  We;;; *** have to give the optimize-slot-value method the user might have;;; *** defined for this metclass a chance to run.;;;(defmethod make-reader-method-function ((class standard-class) slotd)  (let ((slot-name (slotd-name slotd)))    (funcall (get-templated-function-constructor 'reader-function--std)	     slot-name)))(defmethod make-writer-method-function ((class standard-class) slotd)  (let ((slot-name (slotd-name slotd)))    (funcall (get-templated-function-constructor 'writer-function--std)	     slot-name)));;;;;; These old definitions don't work because they go around slot-value-using-class;;; even when that is not appropriate.  There isn't really a performance hit from;;; using the proper definitions since the method functions are never really called.;;;;(define-function-template reader-function--std () '(slot-name sxhash);  `(function;     (lambda (instance--std);       (let ((wrapper (iwmc-class-class-wrapper instance--std));             (slots (iwmc-class-static-slots instance--std)));         (get-slot-value-1 instance--std wrapper slots sxhash slot-name)))));;(define-function-template writer-function--std () '(slot-name sxhash);  `(function;     (lambda (nv instance--std);       (let ((wrapper (iwmc-class-class-wrapper instance--std));             (slots (iwmc-class-static-slots instance--std)));         (set-slot-value-1 nv instance--std wrapper slots sxhash slot-name))))) (define-function-template reader-function--std () '(slot-name)  `(function     (lambda (instance)       (slot-value-using-class (wrapper-class (iwmc-class-class-wrapper instance))			       instance			       slot-name))))(define-function-template writer-function--std () '(slot-name)  `(function     (lambda (nv instance)       (setf (slot-value-using-class (wrapper-class (iwmc-class-class-wrapper instance))				     instance				     slot-name)	     nv))))(eval-when (load)  (pre-make-templated-function-constructor reader-function--std)  (pre-make-templated-function-constructor writer-function--std))  ;;;; inform-type-system-about-class;;;; make-type-predicate;;;;;; These are NOT part of the standard protocol.  They are internal mechanism;;; which PCL uses to *try* and tell the type system about class definitions.;;; In a more fully integrated implementation of CLOS, the type system would;;; know aout class objects and class names in a more fundamental way and;;; the mechanism used to inform the type system about new classes would be;;; different.;;;;;; This is a generic function because built-in-classes need to be able to;;; turn this off.;;;(defmethod inform-type-system-about-class ((class standard-class) name)  (let ((predicate-name (make-type-predicate-name name)))    (setf (symbol-function predicate-name) (make-type-predicate class))    (do-satisfies-deftype name predicate-name)))(defmethod make-type-predicate ((class standard-class))  (funcall (get-templated-function-constructor 'type-predicate--std) class))(define-function-template type-predicate--std () '(class)  '(function     (lambda (x)       (not	 (null	   (cond ((iwmc-class-p x)		  (memq class (class-precedence-list				(class-of--class x))))		 ((funcallable-instance-p x)		  (memq class (class-precedence-list				(funcallable-instance-class x))))))))))						(eval-when (load)  (pre-make-templated-function-constructor type-predicate--std)) 