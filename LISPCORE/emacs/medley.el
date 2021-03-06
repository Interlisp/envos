;; Connect to socket from Medley so EMACS users won't have to learn anything
;; new.
;;
;;
;; This code adapted from FSF lisp/shell.el and FranzInc fi/sublisp.el,
;; so its availability is the same as those works:
;;
;; Copyright (C) 1990, Venue, Oakland, CA
;; copyright (C) 1987, 1988 Franz Inc, Berkeley, Ca.
;; Copyright (C) 1985, 1986, 1987 Free Software Foundation, Inc.
;;
;; This file is derived from part of GNU Emacs.
;;
;; GNU Emacs is distributed in the hope that it will be useful,
;; but WITHOUT ANY WARRANTY.  No author or distributor
;; accepts responsibility to anyone for the consequences of using it
;; or for whether it serves any particular purpose or works at all,
;; unless he says so in writing.  Refer to the GNU Emacs General Public
;; License for full details.
;;
;; Everyone is granted permission to copy, modify and redistribute
;; GNU Emacs, but only under the conditions described in the
;; GNU Emacs General Public License.   A copy of this license is
;; supposed to have been given to you along with GNU Emacs so you
;; can know your rights and responsibilities.  It should be in a
;; file named COPYING.  Among other things, the copyright notice
;; and this notice must be preserved on all copies.
;;

(require 'shell)

(defvar medley-socket-name
  (concat "/tmp/MS-" (getenv "USER"))
  "Name used by connect-to-medley for finding its Unix socket.")

(defvar medley-buffer "*medley*"
  "Buffer name used by connect-to-medley")

(defvar medley-lisp-load-command
      "(cl:load \"%s\" :verbose nil :package (find-package \"%s\"))"
      "A format string accepting two arguments: FILENAME and PACKAGE-NAME")

(defvar medley-send-defun-package "XCL-USER"
  "The package medley-send-defun uses to load forms;
someday this will be smarter about checking mode lines, etc.")

(defvar inferior-medley-mode-map nil)
(if inferior-medley-mode-map
    nil
  (setq inferior-medley-mode-map (copy-alist inferior-lisp-mode-map))
  (define-key inferior-medley-mode-map "\C-c\C-d" 'medley-exit)
  (define-key inferior-medley-mode-map "\C-c\C-\\" 'medley-exit)
  (define-key inferior-medley-mode-map "\C-c\C-z" 'medley-interrupt)
  (define-key inferior-medley-mode-map "\C-c\C-c" 'medley-interrupt)
  (define-key inferior-medley-mode-map "\C-c\C-t" 'medley-control-t)
  (define-key inferior-medley-mode-map "\C-c\C-e" 'medley-control-e)
; (define-key inferior-medley-mode-map "\e\C-x" 'medley-send-defun)
  )

;;
;; Minor modifications to inferior-lisp-mode behavior
;;

(setq inferior-lisp-prompt "^.*/[0-9]*_ ")

;;
;; Various key-bound medley functions
;;

(defun medley-exit ()
  "Closes medley chat connection by interrupting Medley and telling it to
kill its chat process."
  (interactive)
  (medley-interrupt)
  (let ((cp (get-buffer-process (current-buffer))))
    (accept-process-output cp)
    (process-send-string
     cp "(IL:PROGN (IL:PRIN1 \"Connection closed\")(IL:RETFROM (IL:QUOTE IL:EMACSCHATSERVEROPENFN)))")
    (while (progn
	     (accept-process-output cp)
	     (goto-char (point-max))
	     (beginning-of-line)
	     (not (looking-at "Connection closed"))))
    (delete-process cp)
    )
  )

(defun medley-interrupt ()
  "Sends Medley chat process a Control-B"
  (interactive)
  (process-send-string (get-buffer-process (current-buffer)) "")
  )

(defun medley-control-t ()
  "Sends Medley chat process a Control-T"
  (interactive)
  (process-send-string (get-buffer-process (current-buffer)) "")
  )

(defun medley-control-e ()
  "Sends Medley chat process a Control-E"
  (interactive)
  (process-send-string (get-buffer-process (current-buffer)) "")
  )

;;
;; Bind this to ESC-C-x in modes where you want to send defun text to Medley.
;; Someday this will be smarter regarding packages, compiling, etc...
;;

(defun medley-send-defun (display-flag)
  "Send the current defun to the Lisp process made by M-x connect-to-medley.
With argument, force redisplay and scrolling of the *medley* buffer.
Variable `inferior-lisp-load-command' controls formatting of
the `load' form that is set to the Lisp process."
  (interactive "P")
  (let* ((medley-process
	  (or (get-buffer-process (get-buffer medley-buffer))
	      (error "Medley buffer has no process")))
	 (filename (concat medley-socket-name "-load"))
	 (bufname (file-name-nondirectory filename))
	 (temp-buffer (or (get-buffer bufname)
			  (create-file-buffer filename)))
	 start
	 )
    (save-window-excursion
      (beginning-of-defun)
      (setq start (point))
      (end-of-defun)
      (copy-region-as-kill start (point))
      (pop-to-buffer temp-buffer)
      (erase-buffer)
      (insert "; This line tells Medley this is a Common Lisp source file\n")
      (yank)
      (write-file filename)
      (process-send-string medley-process
			   (format medley-lisp-load-command filename
				   medley-send-defun-package))
      (erase-buffer)
      (set-buffer-modified-p nil)
      (bury-buffer)
      )
    (if display-flag
	(let* ((buffer (process-buffer medley-process))
	       (w (or (get-buffer-window buffer) (display-buffer buffer)))
	       (height (window-height w))
	       (end))
	  (save-excursion
	    (set-buffer buffer)
	    (setq end (point-max))
	    (while (progn
		     (accept-process-output medley-process)
		     (goto-char (point-max))
		     (beginning-of-line)
		     (or (= (point-max) end)
			 (not (looking-at inferior-lisp-prompt)))))
	    (setq end (point-max))
	    (vertical-motion (- 4 height))
	    (set-window-start w (point)))
	  (set-window-point w end)))))

(defun make-medley-buffer ()
  "Connects Medley chat process to the *medley* buffer."
  (let ((buffer (get-buffer-create medley-buffer))
	proc status)
    (setq proc (get-buffer-process buffer))
    (if proc (setq status (process-status proc)))
    (save-excursion
      (set-buffer buffer)
      (if (memq status '(run stop))
	  nil
	(if proc (delete-process proc))
	(setq proc (open-network-stream	(buffer-name buffer) buffer
		                        (expand-file-name medley-socket-name)
					0))
	(goto-char (point-max))
	(set-marker (process-mark proc) (point))
	(shell-mode))
      buffer)))

(defun connect-to-medley ()
  "Connect to a running Medley, input and output via buffer *medley*."
  (interactive)
  (switch-to-buffer (make-medley-buffer))
  (inferior-lisp-mode)
  (use-local-map inferior-medley-mode-map)
  )

        TIMESROMAN           �      � z�