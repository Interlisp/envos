;;;-*-Mode:LISP; Package: PCL; Base:10; Syntax:Common-lisp -*-;;;;;; *************************************************************************;;; Copyright (c) 1985, 1986, 1987, 1988 Xerox Corporation.;;; All rights reserved.;;;;;; Use and copying of this software and preparation of derivative works;;; based upon this software are permitted.  Any distribution of this;;; software or derivative works must comply with all applicable United;;; States export control laws.;;; ;;; This software is made available AS IS, and Xerox Corporation makes no;;; warranty about the software, its performance or its conformity to any;;; specification.;;; ;;; Any person obtaining a copy of this software is requested to send their;;; name and post office or electronic mail address to:;;;   CommonLoops Coordinator;;;   Xerox PARC;;;   3333 Coyote Hill Rd.;;;   Palo Alto, CA 94304;;; (or send Arpanet mail to CommonLoops-Coordinator.pa@Xerox.arpa);;;;;; Suggestions, comments and requests for improvements are also welcome.;;; *************************************************************************;;;(in-package 'pcl)(defun get-class-slot-value-1 (object wrapper slot-name)  (let ((entry (assq slot-name (wrapper-class-slots wrapper))))    (if (null entry)	(slot-missing (wrapper-class wrapper) object slot-name 'slot-value)	(if (eq (cdr entry) *slot-unbound*)	    (slot-unbound (wrapper-class wrapper) object slot-name)	    (cdr entry)))))(defun set-class-slot-value-1 (new-value object wrapper slot-name)  (let ((entry (assq slot-name (wrapper-class-slots wrapper))))    (if (null entry)	(slot-missing (wrapper-class wrapper)		      object		      slot-name		      'setf		      new-value)	(setf (cdr entry) new-value))))(defmethod class-slot-value ((class standard-class) slot-name)  (let ((wrapper (class-wrapper class))	(prototype (class-prototype class)))    (get-class-slot-value-1 prototype wrapper slot-name)))(defmethod (setf class-slot-value) (nv (class standard-class) slot-name)  (let ((wrapper (class-wrapper class))	(prototype (class-prototype class)))    (set-class-slot-value-1 nv prototype wrapper slot-name)));;;;;; The slot-value--std and slot-value--fsc cache.;;;;;; First off, don't confuse this with either of the other two mechanisms;;; used by PCL for slot access caching.  This mechanism is used only by;;; internal parts of PCL which explictly call one of these two special;;; versions of slot-value.  Both of these versions require their slot-name;;; argument to be a quoted symbol.  In addition, they require the caller;;; to know the metaclass of the instance argument.;;;;;; PCL uses these for itself in certain critical code segments where it;;; needs to read the value of a slot and one or more of the following is;;; true:;;;   - it isn't a context where PV optimization could have been done;;;     and performance is critical;;;     ;;;   - metacircularity considerations prevent doing method lookup;;;;;; These caches only provide high performance and prevent method lookup;;; when the slot is a bound, instance slot.  All other cases trap through;;; the documented generic functions.;;; (defconstant slot-value-cache-size 1024)(defconstant slot-value-cache-mask (make-cache-mask slot-value-cache-size 3))(defvar *slot-value-cache* (make-memory-block slot-value-cache-size))(defun reduce-slot-value--mumble-arg (slot-name macro)  (if (constantp slot-name)      (eval slot-name)      (error "Using ~S incorrectly.~@              Slot-name argument must be a constant."	     macro)));;;;;; These next four are the interface between this slot-value cache and any;;; part of PCL which wants to use it for its call to slot value.  In summary;;; the entry points are:;;;;;;   slot-value--std         (<std instance> <quoted slot name>);;;   |SETF SLOT-VALUE--STD|  (<nv> ...);;;   slot-value--fsc         (<funcallable std instance> <quoted slot name>);;;   |SETF SLOT-VALUE--FSC|  (<nv> ...);;;(defmacro slot-value--std (instance slot-name)  (let ((symbol (reduce-slot-value--mumble-arg slot-name 'slot-value--std)))    `(let* ((.instance. ,instance)	    (.wrapper. (iwmc-class-class-wrapper .instance.))	    (.slots. (iwmc-class-static-slots .instance.)))       (get-slot-value-1	 .instance. .wrapper. .slots. ,(sxhash symbol) ,slot-name))))(defmacro slot-value--fsc (instance slot-name)  (let ((symbol (reduce-slot-value--mumble-arg slot-name 'slot-value--fsc)))    `(let* ((.instance. ,instance)	    (.wrapper. (funcallable-instance-wrapper .instance.))	    (.slots. (funcallable-instance-static-slots .instance.)))       (get-slot-value-1	 .instance. .wrapper. .slots. ,(sxhash symbol) ,slot-name))));;;;;; We don't bother to optimize the SETFs of these.  It doesn't seem worth;;; it, and whats more I can't figure out how to make it work given that the;;; slot-name argument must be a quoted symbol, and setf doesn't let me get;;; at that.  Presumably this could be done with define-modify-macro, but I;;; will leave that as an exercise for the reader.;;; (defmacro SETF\ SLOT-VALUE--STD (nv instance slot-name)  `(setf (slot-value ,instance ,slot-name) ,nv))(defmacro SETF\ SLOT-VALUE--FSC (nv instance slot-name)  `(setf (slot-value ,instance ,slot-name) ,nv));;;;;; At the next level, get-slot-value-1 and set-slot-value-1 are used by a;;; few other parts of PCL which want a different interface to this cache.;;; The setf function stuff breaks down here because setf is such a pain in;;; the ass to use.;;; (defmacro get-slot-value-1 (instance wrapper static-slots sxhash slot-name)  `(let ((offset (%logand slot-value-cache-mask			  (%logxor (validate-wrapper ,instance ,wrapper)				   ,sxhash)))	 (cache *slot-value-cache*)	 (pos 0))     (declare (type fixnum offset pos))     (without-interrupts       (if (and (eq (%svref cache offset) ,wrapper)		(eq (%svref cache (%+ offset 1)) ,slot-name))	   (progn	     (setq pos (%svref cache (%+ offset 2)))	     (interrupts-on)	     (get-slot-value-2	       ,instance ,wrapper ,slot-name ,static-slots pos))	   (progn	     (interrupts-on)	     (get-slot-value-cache-miss	       ,instance ,wrapper ,static-slots ,slot-name offset))))))(defmacro set-slot-value-1 (nv instance wrapper static-slots sxhash slot-name)  `(let ((offset (%logand slot-value-cache-mask			  (%logxor (validate-wrapper ,instance ,wrapper)				   ,sxhash)))	 (cache *slot-value-cache*)	 (pos 0))     (declare (type fixnum offset pos))     (without-interrupts       (if (and (eq (%svref cache offset) ,wrapper)		(eq (%svref cache (%+ offset 1)) ,slot-name))	   (progn	     (setq pos (%svref cache (%+ offset 2)))	     (interrupts-on)	     (set-slot-value-2	       ,nv ,instance ,wrapper ,slot-name ,static-slots pos))	   (progn	     (interrupts-on)	     (set-slot-value-cache-miss	       ,nv ,instance ,wrapper ,static-slots ,slot-name offset))))))(defmacro get-slot-value-2 (instance wrapper slot-name static-slots position)  `(let ((val (%svref ,static-slots ,position)))     (if (eq val ',*slot-unbound*)	 (slot-unbound (wrapper-class ,wrapper) ,instance ,slot-name)	 val)))(defmacro set-slot-value-2 (nv instance wrapper slot-name static-slots position)  (declare (ignore instance wrapper slot-name))  `(setf (%svref ,static-slots ,position) ,nv));;;;;; These are the miss handlers for this slot value cache.;;; (defun get-slot-value-cache-miss       (instance wrapper static-slots slot-name offset)  (let ((pos (posq slot-name (wrapper-instance-slots-layout wrapper))))    (if pos	(let ((cache *slot-value-cache*))	  (without-interrupts	    (setf (%svref cache offset) wrapper)	    (setf (%svref cache (%+ offset 1)) slot-name)	    (setf (%svref cache (%+ offset 2)) pos))	  (get-slot-value-2 instance wrapper slot-name static-slots pos))	(slot-value-using-class (class-of instance) instance slot-name))))(defun set-slot-value-cache-miss       (nv instance wrapper static-slots slot-name offset)  (let ((pos (posq slot-name (wrapper-instance-slots-layout wrapper))))    (if pos	(let ((cache *slot-value-cache*))	  (without-interrupts	    (setf (%svref cache offset) wrapper)	    (setf (%svref cache (%+ offset 1)) slot-name)	    (setf (%svref cache (%+ offset 2)) pos))	  (set-slot-value-2 nv instance wrapper slot-name static-slots pos))	(setf (slot-value-using-class (class-of instance) instance slot-name)	      nv))));;;;;; Slotd-position is used to find the position of a slot with a particular;;; name in a list of slotds.  Specifically it is used in the case of a;;; slot-value cache miss to find this slot index.  That means it is used in;;; about 2% of the total slot accesses so it should be fast.;;; (defmacro slotd-position (slotd-name slotds)  `(let ((slotd-name ,slotd-name))     (do ((pos 0 (+ pos 1))	  (slotds ,slotds (cdr slotds)))	 ((null slotds) nil)       (declare (type fixnum pos) (type list slotds))       (and (eq slotd-name (slotd-name (car slotds)))	    (return pos)))))(defmacro find-slotd (slotd-name slotds)  (once-only (slotd-name)    (let ((slotd-var (gensym)))      `(dolist (,slotd-var ,slotds)	 (when (eq (slotd-name ,slotd-var) ,slotd-name)	   (return ,slotd-var))))))  (defmethod copy-slotd ((slotd standard-slot-description))  (make-instance 'standard-slot-description    :name (slotd-name slotd)    :keyword (slotd-keyword slotd)    :initform (slotd-initform slotd)    :readers (slotd-readers slotd)    :writers (slotd-writers slotd)    :allocation (slotd-allocation slotd)    :type (slotd-type slotd)))	;;; At last the meta-braid is up.  The method class-instance-slots exists and;;; there is peace in the land.  Now we can finish slot-value, and friends.(defun slot-value (object slot-name)  (slot-value-using-class (class-of object) object slot-name))(defun set-slot-value (object slot-name new-value)  (setf (slot-value-using-class (class-of object) object slot-name) new-value))(defun slot-boundp (object slot-name)  (slot-boundp-using-class (class-of object) object slot-name))(defun slot-makunbound (object slot-name)  (slot-makunbound-using-class (class-of object) object slot-name))(defun slot-exists-p (object slot-name)  (slot-exists-p-using-class (class-of object) object slot-name))(defmacro bind-wrapper-and-static-slots--std (object &body body)  (once-only (object)    `(let ((wrapper (iwmc-class-class-wrapper ,object))	   (static-slots (iwmc-class-static-slots ,object)))       .,body)))(defmacro bind-wrapper-and-static-slots--fsc (object &body body)  (once-only (object)    `(let ((wrapper (funcallable-instance-wrapper ,object))	   (static-slots (funcallable-instance-static-slots ,object)))       .,body)));;;;;; These 4 macros implement the body of the behavior of the four generic;;; functions which are their namesakes.  Each macro assumes that it will;;; be used in a lexical context in which these variables are bound:;;;   CLASS;;;   OBJECT;;;   SLOT-NAME;;;   WRAPPER;;;   STATIC-SLOTS;;;;;;   The setf one also assumes that NV will be bound to the new value.;;;;;; Given those bindings, the macro will implement the appropriate behavior.;;;;;; This modularity, combined with the bind-wrapper-and-static-slots--xxx;;; macros above makes for a nice, clean separation between the methods for;;; standard-class and funcallable-standard-class.;;; (defmacro slot-value-using-class-1 ()  `(let ((pos (instance-slot-position wrapper slot-name)))     (if pos	 (get-slot-value-2 object wrapper slot-name static-slots pos)	 (get-class-slot-value-1 object wrapper slot-name))))(defmacro setf-slot-value-using-class-1 ()  `(let ((pos (instance-slot-position wrapper slot-name)))     (if pos	 (set-slot-value-2 nv object wrapper slot-name static-slots pos)	 (set-class-slot-value-1 nv object wrapper slot-name))))(defmacro slot-boundp-using-class-1 ()  `(let ((pos (instance-slot-position wrapper slot-name)))     (if pos	 (neq (%svref static-slots pos) ',*slot-unbound*)	 (let ((entry (assq slot-name (wrapper-class-slots wrapper))))	   (if (null entry)	       (slot-missing class object slot-name 'slot-boundp)	       (neq (cdr entry) ',*slot-unbound*))))))(defmacro slot-makunbound-using-class-1 ()  `(let ((pos (instance-slot-position wrapper slot-name)))     (if pos	 (progn	   (setf (%svref static-slots pos) ',*slot-unbound*)	   object)	 (let ((entry (assq slot-name (wrapper-class-slots wrapper))))	   (if (null entry)	       (slot-missing class object slot-name 'slot-makunbound)	       (progn		 (setf (cdr entry) ',*slot-unbound*)		 object))))));;;;;; These are the actual method definitions, which combine the appropriate;;; binding of wrapper and static-slots with the appropriate body code.;;; (defmethod slot-value-using-class	   ((class standard-class) object slot-name)  (bind-wrapper-and-static-slots--std object    (slot-value-using-class-1)))(defmethod (setf slot-value-using-class)	   (nv (class standard-class) object slot-name)  (bind-wrapper-and-static-slots--std object    (setf-slot-value-using-class-1)))(defmethod slot-boundp-using-class	   ((class standard-class) object slot-name)  (bind-wrapper-and-static-slots--std object    (slot-boundp-using-class-1)))(defmethod slot-makunbound-using-class	   ((class standard-class) object slot-name)  (bind-wrapper-and-static-slots--std object    (slot-makunbound-using-class-1)))(defmethod slot-exists-p-using-class ((class standard-class) object slot-name)  (declare (ignore object))  (some #'(lambda (s) (eq (slotd-name s) slot-name))	(class-slots class)))(defmethod slot-missing	   ((class t) instance slot-name operation &optional new-value)  (error "When attempting to ~A,~%the slot ~S is missing from the object ~S."	 (ecase operation	   (slot-value "read the slot's value (slot-value)")	   (setf (format nil			 "set the slot's value to ~S (setf of slot-value)"			 new-value))	   (slot-boundp "test to see if slot is bound (slot-boundp)")	   (slot-makunbound "make the slot unbound (slot-makunbound)"))	 slot-name	 instance))(defmethod slot-unbound ((class t) instance slot-name)  (error "The slot ~S is unbound in the object ~S." slot-name instance))(defun slot-value-or-default (object slot-name &optional (default "unbound"))  (if (slot-boundp object slot-name)      (slot-value object slot-name)      default))(defmacro typep--class (iwmc-class type)  `(not (null (memq (find-class ,type t)                    (class-class-precedence-list                       (wrapper-class			(iwmc-class-class-wrapper ,iwmc-class)))))))(defun subclassp (class1 class2)  (or (classp class1) (setq class1 (find-class class1)))  (or (classp class2) (setq class2 (find-class class2)))  (memq class2 (class-class-precedence-list class1)))(defun sub-class-p (x class)  (if (symbolp class) (setq class (find-class class)))  (not (null (memq class (class-precedence-list (class-of x))))))(defmethod allocate-instance ((class standard-class) &rest initargs)  (declare (ignore initargs))  (let* ((class-wrapper (class-wrapper class))	 (instance (%allocate-instance--class		     (class-no-of-instance-slots class))))    (setf (iwmc-class-class-wrapper instance) class-wrapper)    instance));(defun make-instance (class &rest init-plist);  (when (symbolp class) (setq class (find-class class)));  (let ((object (allocate-instance class)));    (initialize object init-plist);    object));;(defmethod initialize ((object object) init-plist);  (initialize-from-defaults object);  (initialize-from-init-plist object init-plist));;(defmethod initialize-from-defaults ((self object));  (dolist (slotd (class-slots (class-of self)));    (let* ((name (slotd-name slotd));	   (allocation (slotd-allocation slotd));	   (initfunction (slotd-initfunction slotd));	   (value (and (eq allocation :instance);		       initfunction;		       (funcall initfunction))));      (and (eq allocation :instance);	   (setf (slot-value self name) value)))));;(defmethod initialize-from-init-plist ((self object) init-plist);  (when init-plist;    (let* ((class (class-of self));	   (slotds (class-slots class)));      (flet ((get-slotd (keyword);	       (dolist (slotd slotds);		 (when (eq (slotd-keyword slotd) keyword);		   (return slotd)))));	(do* ((keyword-loc init-plist (cdr value-loc));	      (value-loc (cdr keyword-loc) (cdr keyword-loc));	      (slotd () ());	      (allow-other-keys-p () allow-other-keys-p));	     (());	  (flet ((allow-other-keywords-p ();		   (when (null allow-other-keys-p);		     (setq allow-other-keys-p;			   (do ((loc keyword-loc (cddr loc)));			       ((null loc) 0);			     (when (eq (car loc) ':allow-other-keys);			       (return (if (cadr loc) 1 0))))));		   (if (= allow-other-keys-p 1) t nil)));	    (cond ((null keyword-loc) (return nil));		  ((eq (car keyword-loc) :allow-other-keys);		   (setq allow-other-keys-p;			 (if (cadr keyword-loc) 1 0)));		  ((null value-loc);		   (error "No value supplied for the init-keyword ~S.";			  (car keyword-loc)));		  ((null (setq slotd (get-slotd (car keyword-loc))));		   (unless (allow-other-keywords-p);		     (error "~S is not a valid keyword in the init-plist.";			    (car keyword-loc))));		  (t;		   (setf (slot-value self (slotd-name slotd));			 (car value-loc))))))))));;;(defmethod class-default-includes ((class standard-class));  (declare (ignore class));  (list (find-class 'object)))