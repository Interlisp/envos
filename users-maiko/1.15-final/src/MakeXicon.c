/* @(#) MakeXicon.c Version 1.1 (3/1/90). copyright venue & Fuji Xerox  */static char *id = "@(#) MakeXicon.c	1.1 3/1/90	(venue & Fuji Xerox)";/**** Copyright (C) 1988 by Fuji Xerox co.,Ltd. All rights reserved.**		Author: Mitsunori Matsuda*		Date  : August 18, 1988*/#include <stdio.h>#include <X11/Xlib.h>#include <X11/Xutil.h>#include "XVersion.h"#include "MyWindow.h"#include "lispXicon.h"extern Display *Xdisplay;extern MyWindow Lisp_Window;extern int Bitmap_Pad		 , Default_Depth;Pixmap IconPixmap;XImage IconImage;make_Xicon(){#ifdef TRACE	printf( "TRACE: make_Xicon()\" );#endif        IconImage.width = Lisp_icon_width;        IconImage.height = Lisp_icon_height;        IconImage.xoffset = 0;        IconImage.format  = XYBitmap;        IconImage.data    = (char*) Lisp_icon;#ifdef XV11R1        IconImage.byte_order  = LSBFirst;#else XV11R1        IconImage.byte_order  = MSBFirst;#endif XV11R1        IconImage.bitmap_unit = 8;        IconImage.bitmap_pad  = Bitmap_Pad;        IconImage.depth       = 1;        IconImage.bytes_per_line = Lisp_icon_width/8;        IconImage.bitmap_bit_order = MSBFirst;        IconPixmap = XCreatePixmap( Xdisplay, Lisp_Window.win                                , Lisp_icon_width , Lisp_icon_height								, Default_Depth );        XPutImage( Xdisplay, IconPixmap, *(Lisp_Window.gc), &IconImage			, 0, 0, 0, 0                        , Lisp_icon_width, Lisp_icon_height );} /* end make_Xicon */