h19350s 00000/00000/00034d D 1.2 90/04/19 21:50:40 sybalsky 2 1c AIX/Posix:  Name shortened to Xdisplayes 00034/00000/00000d D 1.1 90/03/01 15:27:46 osamu 1 0c date and time created 90/03/01 15:27:46 by osamueuUf e 0tTI 1/* %Z% %M% Version %I% (%G%). copyright Venue & Fuji Xerox  *//**** Copyright (C) 1988 by Fuji Xerox co.,Ltd. All rights reserved.**		Author: Mitsunori Matsuda*		Date  : July 26,1988*/typedef struct {	char *name;		/* name of this window */	Window win;          	/* window id */	int    x,y;          	/* x and y cordinates */	int    width,height; 	/* window size */	int    border;       	/* border width */	GC     *gc; 	unsigned logn event_mask;	Cursor *cursor;      	/* current cursor */	int    (*func)();    	/* Event Function */} MyWindow;typedef struct {	int type;     /* Event type */	int (*func)();/* Event function */} MyEvent;typedef struct {	int left_x;  /* x cordinate of upperleft corner */	int top_y;   /* y cordinate of upperlert corner */	int right_x; /* x cordinate of lowerright corner */	int bottom_y;/* y cordinate of lowerright corner */} DisplayArea;E 1