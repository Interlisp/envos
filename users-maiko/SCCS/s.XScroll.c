h56913s 00000/00000/00219d D 1.7 93/02/08 14:43:16 sybalsky 7 6c Big VM (and new CDR coding) changes for 3.0 \nes 00002/00000/00217d D 1.6 92/05/27 19:05:47 sybalsky 6 5c 386 Unix x-signal locking changeses 00018/00010/00199d D 1.5 92/04/27 18:42:15 nilsson 5 4c Fix of Xdisplay deps.es 00001/00001/00208d D 1.4 92/04/21 16:59:48 sybalsky 4 3c shortening file names for DOS \nes 00048/00008/00161d D 1.3 90/09/18 23:28:59 sybalsky 3 2c Retrofit changes from AIX, DEC3100, PS/2es 00013/00002/00156d D 1.2 90/04/20 01:15:32 sybalsky 2 1c e names, bulk change.es 00158/00000/00000d D 1.1 90/04/19 23:18:45 sybalsky 1 0c date and time created 90/04/19 23:18:45 by sybalskyeuUf e 0tTI 1/* %Z% %M% Version %I% (%G%). copyright venue & Fuji Xerox  */static char *id = "%Z% %M%	%I% %G%	(venue & Fuji Xerox)";D 2/* %Z% %M% Version %I% (%G%). copyright venue & Fuji Xerox  */static char *id = "%Z% %M%	%I% %G%	(venue & Fuji Xerox)";E 2I 2/************************************************************************//*									*//*	Copyright 1989, 1990 Venue, Fuji Xerox Co., Ltd, Xerox Corp.	*//*									*//*	This file is work-product resulting from the Xerox/Venue	*//*	Agreement dated 18-August-1989 for support of Medley.		*//*									*//************************************************************************/E 2/**** Copyright (C) 1988 by Fuji Xerox co.,Ltd. All rights reserved.**		Author: Mitsunori Matsuda*		Date  : September 5, 1988*/#include <stdio.h>#include <X11/Xlib.h>#include <X11/Xutil.h>#include "XVersion.h"#include "MyWindow.h"D 4#include "XCursorDef.h"E 4I 4#include "Xcursdef.h"E 4#include "XCursors.h"Cursor HorizScrollCursor     , HorizThumbCursor     , ScrollDownCursor     , ScrollLeftCursor     , ScrollRightCursor     , ScrollUpCursor     , VertScrollCursor     , VertThumbCursor;extern Display *Xdisplay;extern MyWindow Vert_Scroll	      , Horiz_Scroll;extern int Scroll_Width	 , Scroll_Border;I 3/************************************************************************//*									*//*									*//*									*//*									*//*									*//************************************************************************/E 3Before_CreateScroll( parent, child )MyWindow *parent       , *child;{#ifdef TRACE	printf( "TRACE: Before_CreateScroll()\n" );#endif	/* Scroll Window Geometory */	child->border = parent->border;	if( child == &Vert_Scroll ) {		child->y = 0;		child->width = Scroll_Width;		child->x = (parent->width - 1)				- (child->width + child->border);		child->height = parent->height				- (Scroll_Width + 3*Scroll_Border);	} 	else if( child == &Horiz_Scroll ) { 		child->x = 0;		child->height = Scroll_Width;		child->y = (parent->height - 1)				- (child->height + child->border);		child->width = parent->width				- (Scroll_Width + 3*Scroll_Border);	} /* end if(child) */} /* end Before_CreateScroll */I 3/************************************************************************//*									*//*									*//*									*//*									*//*									*//************************************************************************/E 3After_CreateScroll( parent, child )MyWindow *parent       , *child;{#ifdef TRACE	printf( "TRACE: After_CreateScroll()\n" );#endifI 6	XLOCK;E 6	XMapWindow( Xdisplay, child->win );	Add_WindowList( child );	/* Cursor */	if( child == &Vert_Scroll ) {D 5		set_Xcursor( (char*)scrolldown_cursor.cuimageE 5I 5		set_Xcursor( Xdisplay			    , (char*)scrolldown_cursor.cuimageE 5				, (int) scrolldown_cursor.cuhotspotx				, (int) (15 - scrolldown_cursor.cuhotspoty)D 3				, &ScrollDownCursor );E 3I 3				, &ScrollDownCursor, 0 );E 3D 5		set_Xcursor( (char*)scrollleft_cursor.cuimageE 5I 5		set_Xcursor( Xdisplay			    , (char*)scrollleft_cursor.cuimageE 5				, (int) scrollleft_cursor.cuhotspotx				, (int) (15 - scrollleft_cursor.cuhotspoty)D 3				, &ScrollLeftCursor );E 3I 3				, &ScrollLeftCursor , 0);E 3D 5		set_Xcursor( (char*)vertscroll_cursor.cuimageE 5I 5		set_Xcursor( Xdisplay			    , (char*)vertscroll_cursor.cuimageE 5				, (int) vertscroll_cursor.cuhotspotx				, (int) (15 - vertscroll_cursor.cuhotspoty)D 3				, &VertScrollCursor );E 3I 3				, &VertScrollCursor, 0 );E 3D 5		set_Xcursor( (char*)vertthumb_cursor.cuimageE 5I 5		set_Xcursor( Xdisplay			    , (char*)vertthumb_cursor.cuimageE 5				, (int) vertthumb_cursor.cuhotspotx				, (int) (15 - vertthumb_cursor.cuhotspoty)D 3				, &VertThumbCursor );E 3I 3				, &VertThumbCursor, 0 );E 3D 5		DefineCursor( child, &VertScrollCursor );E 5I 5		DefineCursor( Xdisplay, child, &VertScrollCursor );E 5	}	else if( child == &Horiz_Scroll ) {D 5		set_Xcursor( (char*)horizscroll_cursor.cuimageE 5I 5		set_Xcursor( Xdisplay			    , (char*)horizscroll_cursor.cuimageE 5				, (int) horizscroll_cursor.cuhotspotx				, (int) (15 - horizscroll_cursor.cuhotspoty)D 3				, &HorizScrollCursor );E 3I 3				, &HorizScrollCursor, 0 );E 3D 5		set_Xcursor( (char*)horizthumb_cursor.cuimageE 5I 5		set_Xcursor( Xdisplay			    , (char*)horizthumb_cursor.cuimageE 5				, (int) horizthumb_cursor.cuhotspotx				, (int) (15 - horizthumb_cursor.cuhotspoty)D 3				, &HorizThumbCursor );E 3I 3				, &HorizThumbCursor, 0 );E 3D 5		set_Xcursor( (char*)scrollright_cursor.cuimageE 5I 5		set_Xcursor( Xdisplay			    , (char*)scrollright_cursor.cuimageE 5				, (int) scrollright_cursor.cuhotspotx				, (int) (15 - scrollright_cursor.cuhotspoty)D 3				, &ScrollRightCursor );E 3I 3				, &ScrollRightCursor, 0 );E 3D 5		set_Xcursor( (char*)scrollup_cursor.cuimageE 5I 5		set_Xcursor( Xdisplay			    , (char*)scrollup_cursor.cuimageE 5				, (int) scrollup_cursor.cuhotspotx				, (int) (15 - scrollup_cursor.cuhotspoty)D 3				, &ScrollUpCursor );E 3I 3				, &ScrollUpCursor, 0 );E 3D 5		DefineCursor( child, &HorizScrollCursor );E 5I 5		DefineCursor( Xdisplay, child, &HorizScrollCursor );E 5	}I 6    XUNLOCK;E 6} /* end After_CreateScroll */I 3/************************************************************************//*									*//*									*//*									*//*									*//*									*//************************************************************************/E 3Before_ResizeScroll( window )MyWindow *window;{#ifdef TRACE	printf( "TRACE: Before_ResizeScroll()\n" );#endif		if( window == &Vert_Scroll ) {		window->x = (window->parent->width - 1)				- (window->width + window->border);		window->height = window->parent->height				- (Scroll_Width + 3*Scroll_Border);	} 	else if( window == &Horiz_Scroll ) { 		window->y = (window->parent->height - 1)				- (window->height + window->border);		window->width = window->parent->width				- (Scroll_Width + 3*Scroll_Border);	} /* end if(window) */} /* Before_ResizeScroll */I 3/************************************************************************//*									*//*									*//*									*//*									*//*									*//************************************************************************/E 3After_ResizeScroll( window )MyWindow *window; {#ifdef TRACE	printf( "TRACE: After_ResizeScroll()\n" );#endif		if( window == &Vert_Scroll ) {	} 	else { 	}} /* After_ResizeScroll */E 1