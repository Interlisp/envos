;;;-*-Mode:LISP; Package:(PCL LISP 1000); Base:10; Syntax:Common-lisp -*-;;;;;; *************************************************************************;;; Copyright (c) 1985, 1986, 1987, 1988 Xerox Corporation.;;; All rights reserved.;;;;;; Use and copying of this software and preparation of derivative works;;; based upon this software are permitted.  Any distribution of this;;; software or derivative works must comply with all applicable United;;; States export control laws.;;; ;;; This software is made available AS IS, and Xerox Corporation makes no;;; warranty about the software, its performance or its conformity to any;;; specification.;;; ;;; Any person obtaining a copy of this software is requested to send their;;; name and post office or electronic mail address to:;;;   CommonLoops Coordinator;;;   Xerox PARC;;;   3333 Coyote Hill Rd.;;;   Palo Alto, CA 94304;;; (or send Arpanet mail to CommonLoops-Coordinator.pa@Xerox.arpa);;;;;; Suggestions, comments and requests for improvements are also welcome.;;; *************************************************************************;;;;;; This file contains the higher level support for funcallable instances.;;; (in-package 'pcl)(eval-when (compile load eval)  (mapcar #'eval *fsc-defclass-forms*));;;;;; PCL does us a favor here.  In order for us to implement optimized slot;;; access to funcallable instances, all we have to do is implement these;;; two simple defmethods.  This is an internal and undocumented part of;;; the protocol PCL uses to optimized standard-instance-access.;;; (defmethod wrapper-fetcher ((class funcallable-standard-class))  'funcallable-instance-wrapper)(defmethod slots-fetcher ((class funcallable-standard-class))  'funcallable-instance-static-slots)(defmethod raw-instance-allocator ((class funcallable-standard-class))  'allocate-funcallable-instance-1)(defmethod optimize-standard-instance-access-internal	   ((class funcallable-standard-class))  'fsc-instance-access-pv);;;;;;;;; (defmethod slot-value-using-class	   ((class funcallable-standard-class) object slot-name)  (bind-wrapper-and-static-slots--fsc object    (slot-value-using-class-1)))(defmethod (setf slot-value-using-class)	   (nv (class funcallable-standard-class) object slot-name)  (bind-wrapper-and-static-slots--fsc object    (setf-slot-value-using-class-1)))(defmethod slot-boundp-using-class	   ((class funcallable-standard-class) object slot-name)  (bind-wrapper-and-static-slots--fsc object    (slot-boundp-using-class-1)))(defmethod slot-makunbound-using-class	   ((class funcallable-standard-class) object slot-name)  (bind-wrapper-and-static-slots--fsc object    (slot-makunbound-using-class-1)));;;;;;;;;(defmethod check-super-metaclass-compatibility	   ((fsc funcallable-standard-class)	    (class standard-class))  (null (wrapper-instance-slots-layout (class-wrapper class))))(defmethod allocate-instance	   ((class funcallable-standard-class) &rest initargs)  (declare (ignore initargs))  (let ((class-wrapper (class-wrapper class)))    (allocate-funcallable-instance class-wrapper				   (class-no-of-instance-slots class))))(defmethod make-reader-method-function ((class funcallable-standard-class)					slotd)  (let ((slot-name (slotd-name slotd)))    (funcall (get-templated-function-constructor 'reader-function--fsc)	     slot-name)))(defmethod make-writer-method-function ((class funcallable-standard-class)					slotd)  (let ((slot-name (slotd-name slotd)))    (funcall (get-templated-function-constructor 'writer-function--fsc)	     slot-name)));;;;;; See the comment about reader-function--std and writer-function--sdt.;;;(define-function-template reader-function--fsc () '(slot-name)  `(function     (lambda (instance)       (slot-value-using-class (wrapper-class (funcallable-instance-wrapper instance))			       instance			       slot-name))))(define-function-template writer-function--fsc () '(slot-name)  `(function     (lambda (nv instance)       (setf (slot-value-using-class (wrapper-class (funcallable-instance-wrapper instance))				     instance				     slot-name)	     nv))))(eval-when (load)  (pre-make-templated-function-constructor reader-function--fsc)  (pre-make-templated-function-constructor writer-function--fsc));(defmethod make-type-predicate ((class funcallable-standard-class));  (funcall (get-templated-function-constructor 'type-predicate--fsc) class));;(define-function-template type-predicate--fsc () '(class);  '(function;     (lambda (x);       (not;	 (null;	   (and (funcallable-instance-p x);		(memq class;		      (class-precedence-list;			(funcallable-instance-class x)))))))));;(eval-when (load);  (pre-make-templated-function-constructor type-predicate--fsc));(defmethod all-slots-using-class ((class funcallable-standard-class) object);  (append (gathering ((pairs (collecting)));            (dolist (slotd (class-instance-slots class));              (gather (slotd-name slotd) pairs);              (gather (slot-value object (slotd-name slotd)) pairs)));          (funcallable-instance-dynamic-slots object)))()