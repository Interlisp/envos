%   File   : fbprolog%   Author : Peter Schachte%   Updated: 12/30/87%   Purpose: Prolog compile & consult from lisp file browser%   Copyright (C) 1987, Quintus Computer Systems, Inc.  All rights reserved.						/*  The Lisp library includes a very useful utility called the file    browser.  This presents a scrolling display of files matching a certain    pattern, with a menu allowing various operations on selected files.    These operations include loading or compiling lisp files, but nothing    for Prolog code.  This file adds a Prolog menu item to the file    browser, allowing users to compile Prolog files by selecting them in    the file broswer display, and then selecting the Prolog item in the    file browser menu.  A submenu under the Prolog item offers the choice    of consulting the Prolog file.    No Prolog code is necessary for this.  All the code lives in the file    FBPROLOG, which is loaded below.  The reason for this file is to make    the Prolog library more uniform, and to allow users to load this    package by just loading this Prolog file.*/:- load_foreign_files([library('FBPROLOG')], []).