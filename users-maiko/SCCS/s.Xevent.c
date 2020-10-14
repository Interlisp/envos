h16841s 00000/00000/00256d D 1.4 93/02/08 14:44:18 sybalsky 4 3c Big VM (and new CDR coding) changes for 3.0 \nes 00011/00012/00245d D 1.3 92/04/27 18:42:59 nilsson 3 2c Fix of Xdisplay deps.es 00000/00006/00257d D 1.2 92/04/23 16:30:16 nilsson 2 1c New short name regimen. Improved X commandline handling.es 00263/00000/00000d D 1.1 92/04/21 15:27:36 sybalsky 1 0c date and time created 92/04/21 15:27:36 by sybalskyeuUf e 0tTI 1/* %Z% %M% Version %I% (%G%). copyright venue & Fuji Xerox  */static char *id = "%Z% %M%	%I% %G%	(venue & Fuji Xerox)";D 2/* @(#) Xevent.c Version 1.2 (4/16/91). copyright venue & Fuji Xerox  */static char *id = "@(#) Xevent.c	1.2 4/16/91	(venue & Fuji Xerox)";E 2/************************************************************************//*									*//*	(C) Copyright 1989, 1990, 1991 Venue. All Rights Reserved.	*//*	Manufactured in the United States of America.			*//*									*//*	The contents of this file are proprietary information 		*//*	belonging to Venue, and are provided to you under license.	*//*	They may not be further distributed or disclosed to third	*//*	parties without the specific permission of Venue.		*//*									*//************************************************************************/#include <stdio.h>#include <X11/Xlib.h>#include <X11/Xutil.h>#include "XVersion.h"#include "MyWindow.h"#include "lispXbitblt.h"#define FALSE 0#define TRUE  !FALSEint Mouse_Included = FALSE;D 3extern Display *Xdisplay;E 3extern XImage XScreenBitmap;extern XSetWindowAttributes Lisp_SetWinAttributes;extern DisplayArea ScreenRegion;extern MyWindow Display_Window	      , Lisp_Window	      , Grav_Window[];extern int Bit_Gravity;D 2extern int Auto_Repeat;extern int Def_Auto_Repeat;E 2extern int Backing_Store;/************************************************************************//*									*//*			l i s p _ X e x p o s e				*//*									*//*	Handle an "Expose" event from X.				*//*									*//************************************************************************/lisp_Xexpose( event, window )  XExposeEvent *event;  MyWindow *window;  {#ifdef TRACE    printf( "TRACE:	lisp_Xexpose()\n" );    printf( "lisp_Xexpose: Exposed region x=%d, y=%d, w=%d, h=%d\n"    	, event->x, event->y, event->width, event->height );#endif    /* Repair Display Window */    Xbitblt ( event->x+ScreenRegion.left_x	    , event->y+ScreenRegion.top_y	    , event->width	    , event->height );  } /* end lisp_Xexpose *//************************************************************************//*									*//*									*//*									*//*									*//*									*//************************************************************************/Raise_LispWindow( event, window )  XCrossingEvent *event;  MyWindow *window;  {#ifdef TRACE    printf( "TRACE:Rase_LispWindow()\n" );#endif    if( !Backing_Store )      {D 3	XRaiseWindow( Xdisplay, Lisp_Window.win );	XFlush( Xdisplay );E 3I 3	XRaiseWindow( event->display, Lisp_Window.win );	XFlush( event->display );E 3      }  } /* end Raise_LispWindow *//************************************************************************//*									*//*									*//*									*//*									*//*									*//************************************************************************/#define North_West 0#define North_East 1#define South_West 2#define South_East 3extern Pixmap GravityOn_Pixmap            , GravityOff_Pixmap;Set_BitGravity( event, window )  XButtonEvent *event;  MyWindow  *window;  {    int old_gravity, win;#ifdef TRACE    printf( "TRACE: Set_BitGravity()\" );#endif    old_gravity = Bit_Gravity;    if( (event->type == ButtonPress) 	 && ((event->button & 0xFF) == Button1) ) {		if( window->win == Grav_Window[0].win )      			Bit_Gravity = NorthWestGravity;		else if( window->win == Grav_Window[1].win ) 			Bit_Gravity = NorthEastGravity;		else if( window->win == Grav_Window[2].win ) 			Bit_Gravity = SouthWestGravity;		else if( window->win == Grav_Window[3].win ) 			Bit_Gravity = SouthEastGravity;	} /* end if(button) */    if( Bit_Gravity != old_gravity )      { /* gravity changed */	if( old_gravity == NorthWestGravity ) win = North_West;	if( old_gravity == NorthEastGravity ) win = North_East;	if( old_gravity == SouthWestGravity ) win = South_West;	if( old_gravity == SouthEastGravity ) win = South_East;	/* Change Background Pixmap of Gravity Window */D 3	XSetWindowBackgroundPixmap( XdisplayE 3I 3	XSetWindowBackgroundPixmap( event->displayE 3				, Grav_Window[win].win				, GravityOff_Pixmap );D 3	XClearWindow( Xdisplay, Grav_Window[win].win );E 3I 3	XClearWindow( event->display, Grav_Window[win].win );E 3D 3	XSetWindowBackgroundPixmap( XdisplayE 3I 3	XSetWindowBackgroundPixmap( event->displayE 3				, window->win 				, GravityOn_Pixmap );D 3	XClearWindow( Xdisplay, window->win );E 3I 3	XClearWindow( event->display, window->win );E 3	/* Display BitGravity */	Lisp_SetWinAttributes.bit_gravity = Bit_Gravity;D 3	XChangeWindowAttributes( XdisplayE 3I 3	XChangeWindowAttributes( event->displayE 3				, Display_Window.win				, CWBitGravity				, &Lisp_SetWinAttributes );	/* Lisp_Window Window Gravity */	/*	Lisp_SetWinAttributes.win_gravity = Bit_Gravity;D 3	XChangeWindowAttributes( XdisplayE 3I 3	XChangeWindowAttributes( event->displayE 3				, Lisp_Window.win				, CWWinGravity				, &Lisp_SetWinAttributes );	*/D 3	XFlush( Xdisplay );E 3I 3	XFlush( event->display );E 3      } /* end if */  } /* end Set_BitGravity *//************************************************************************//*									*//*									*//*									*//*									*//*									*//************************************************************************/extern int LispWindow_MaxWidth	    , LispWindow_MaxHeight;lisp_Xconfigure( event, window )  XConfigureEvent *event;  MyWindow *window;  {    int flag;#ifdef TRACE    printf( "TRACE: lisp_Xconfigure()\n" );#endif    window->x = event->x;    window->y = event->y;    if( (event->width  != window->width)     || (event->height != window->height) )      { /* Lisp Window Resized */	window->width  = event->width;	window->height = event->height;		flag = FALSE;	if( window->width > LispWindow_MaxWidth )	  {	    window->width  = LispWindow_MaxWidth;	    flag = TRUE;	  }	if( window->height > LispWindow_MaxHeight )	  {	    window->height = LispWindow_MaxHeight;	    flag = TRUE;	  }D 3	if( flag ) XResizeWindow( Xdisplay, window->winE 3I 3	if( flag ) XResizeWindow( event->display, window->winE 3				  , window->width, window->height );D 3	Resize_Subwindows();E 3I 3	Resize_Subwindows(event->display);E 3      } /* end if */  } /* end lisp_Xconfigure *//************************************************************************//*									*//*									*//*									*//*									*//*									*//************************************************************************/mouse_Xcrossed( event, window )  XCrossingEvent *event;  MyWindow *window;  {#ifdef TRACE    printf( "TRACE: mouse_Xcrossed()\n" );#endif    if( event->type == EnterNotify )      {	Mouse_Included = TRUE;D 2	if(Def_Auto_Repeat && !Auto_Repeat ) XAutoRepeatOff( Xdisplay );E 2      }    if( event->type == LeaveNotify )      {	Mouse_Included = FALSE;D 2	if(Def_Auto_Repeat && !Auto_Repeat ) XAutoRepeatOn( Xdisplay );E 2      }  } /* end mouse_Xcrossed */E 1