h53578s 00000/00000/00060d D 1.2 90/04/19 21:50:13 sybalsky 2 1c AIX/Posix:  Name shortened to Xbitbltes 00060/00000/00000d D 1.1 90/03/01 15:26:45 osamu 1 0c date and time created 90/03/01 15:26:45 by osamueuUf e 0tTI 1/* %Z% %M% Version %I% (%G%). copyright Venue & Fuji Xerox  *//**** Copyright (C) 1988 by Fuji Xerox co.,Ltd. All rights reserved.**		Author: Mitsunori Matsuda*		Date  : August 11, 1988**/#define Xbitblt(x,y,w,h) { \		XPutImage( Xdisplay, Display_Window.win \			, *(Display_Window.gc) \			, &XScreenBitmap \			, x \			, y \			, x - ScreenRegion.left_x \			, y - ScreenRegion.top_y \			, w \			, h ); \	/*	XFlush( Xdisplay ); */	 \} #define clipping_Xbitblt(x,y,w,h) { \		int temp_x \		  , temp_y; \		temp_x = x + w - 1; \		temp_y = y + h - 1; \		if( ( temp_x < ScreenRegion.left_x ) \		 || ( x > ScreenRegion.right_x ) \		 || ( temp_y < ScreenRegion.top_y ) \		 || ( y > ScreenRegion.bottom_y ) ) { \			return; \		} \		if( ( x >= ScreenRegion.left_x ) \		 && ( temp_x <= ScreenRegion.right_x ) \		 && ( y >= ScreenRegion.top_y ) \		 && ( temp_y <= ScreenRegion.bottom_y ) ) { \			Xbitblt( x, y, w, h ); \			return; \		} \		if( x < ScreenRegion.left_x ) { \			w -= ScreenRegion.left_x - x; \			x = ScreenRegion.left_x; \		} \		if( temp_x > ScreenRegion.right_x ) { \			w -= temp_x - ScreenRegion.right_x; \		} \		if( y < ScreenRegion.top_y ) { \			h -= ScreenRegion.top_y - y; \			y = ScreenRegion.top_y; \		} \		if( temp_y > ScreenRegion.bottom_y ) { \			h -= temp_y - ScreenRegion.bottom_y; \		} \		if( ( w>0 ) && ( h>0 ) ) { \			Xbitblt( x, y, w, h ); \		} \}E 1