/* @(#) scrollupcursor.h Version 1.2 (4/19/90). copyright Venue & Fuji Xerox  *//**** Copyright (C) 1988 by Fuji Xerox co.,Ltd. All rights reserved.**		Author: Mitsunori Matsuda*		Date  : August 30, 1988*/char scrollupcursor_bitmap[]={		  0x01, 0x00		, 0x03, 0x80		, 0x03, 0x80		, 0x07, 0xC0			, 0x07, 0xC0			, 0x0F, 0xE0		, 0x0F, 0xE0		, 0x1F, 0xF0		, 0x1F, 0xF0		, 0x03, 0x80		, 0x03, 0x80		, 0x03, 0x80		, 0x03, 0x80		, 0x03, 0x80		, 0x03, 0x80		, 0x03, 0x80};LISP_CURSOR scrollup_cursor = {		  1		, scrollupcursor_bitmap		, scrollupcursor_bitmap		, 7		, 15		, NULL};	