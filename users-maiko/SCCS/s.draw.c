h07949s 00002/00002/00385d D 2.25 93/02/08 14:50:43 sybalsky 29 28c Big VM (and new CDR coding) changes for 3.0 \nes 00034/00017/00353d D 2.24 92/11/25 21:02:40 sybalsky 28 27c Fixing endif's with extra text => comment. \nes 00152/00146/00218d D 2.23 92/05/26 13:42:05 sybalsky 27 26c Make N_OP_drawline return a non-negative value in non-error case, to prevent UFNing with undefined final argument.es 00010/00005/00354d D 2.22 92/04/22 12:46:36 sybalsky 26 25c SGI portes 00004/00004/00355d D 2.21 92/04/21 17:09:16 sybalsky 25 24c shortening file names for DOS \nes 00005/00001/00354d D 2.20 91/02/04 12:51:33 sybalsky 24 23c Fix errors in teh Savoir retrofit (placement of ifdefs &c)es 00002/00002/00353d D 2.19 90/10/09 10:44:10 fuji 23 22c es 00040/00039/00315d D 2.18 90/09/18 23:42:57 sybalsky 22 21c Retrofit changes from AIX, DEC3100, PS/2es 00017/00005/00337d D 2.17 90/08/29 12:54:37 gadener 21 20c Fix for AR#11251 by Mitsonori Matsuda @ Fuji Xeroxes 00014/00004/00328d D 2.16 90/07/25 23:35:36 mitani 20 19c Chaged the timing of HideCursor and ShowCursor at DISPLAYBUFFER and NEW_CURSOR.es 00000/00000/00332d D 2.15 90/04/20 01:27:17 sybalsky 19 18c tening file names, bulk change.es 00002/00002/00330d D 2.14 90/03/28 17:32:55 takeshi 18 17c BYTESWAP option es 00029/00012/00303d D 2.13 90/03/01 14:41:50 sybalsky 17 16c byte-swap changes.es 00020/00006/00295d D 2.12 90/03/01 12:56:58 osamu 16 15c Mitsunori Matsuda adds XWindow functionality.es 00073/00016/00228d D 2.11 90/02/07 14:56:10 mitani 15 14c In CG6 and CG3, modified region is transported to the frame buffer at once.es 00002/00002/00242d D 2.10 89/12/13 10:54:22 sybalsky 14 13c color ss1es 00028/00001/00216d D 2.9 89/10/19 17:58:41 sybalsky 13 12c display bufferinges 00003/00003/00214d D 2.8 89/04/07 13:34:02 shih 12 11c lost sccs ides 00003/00002/00214d D 2.7 89/01/04 16:56:04 shimizu 11 10c For C-Cursor trackinges 00002/00090/00214d D 2.6 88/10/12 14:41:45 krivacic 10 9c new out-of-line interfacees 00012/00004/00292d D 2.5 88/09/23 13:52:29 charnley 9 8c changed version of hidecursor used.es 00003/00020/00293d D 2.4 88/06/30 11:01:39 charnley 8 7c removed TRACE printf.es 00002/00000/00311d D 2.3 88/06/30 10:46:57 greep 7 6c put printf inside #ifdef DEBUGes 00006/00000/00305d D 2.2 88/05/31 16:26:47 masinter 6 5c put old-style OP_draw under ifdef C_ONLYes 00000/00000/00305d D 2.1 88/05/17 09:23:23 hayata 5 4c Version up to 2.1es 00207/00203/00098d D 1.4 88/05/05 20:06:57 shih 4 3c cb'ed it, restored sccs id stringes 00052/00042/00249d D 1.3 88/04/29 17:38:42 shih 3 2c Check cursor before drawing, optimize register decl., otherses 00006/00002/00285d D 1.2 88/03/23 09:48:13 charnley 2 1c added TRACE printoutes 00287/00000/00000d D 1.1 88/03/21 16:30:13 charnley 1 0c date and time created 88/03/21 16:30:13 by charnleyeuUtTI 1D 2/* This is G-file %Z% %M% Version %I% (%G%). copyright Xerox & Fuji Xerox  */static char *id = "%Z% %M%	%I% %G%";E 2I 2D 4/* This is G-file @(#) draw.c Version 1.1 (3/21/88). copyright Xerox & Fuji Xerox  */static char *id = "@(#) draw.c	1.1 3/21/88";E 4I 4D 8/* This is G-file %Z% %M% Version %I% (%G%). copyright Xerox & Fuji Xerox  */static char *id = "%Z% %M%	%I% %G%";E 8I 8D 12/* This is G-file @(#) draw.c Version 2.2 (5/31/88). copyright Xerox & Fuji Xerox  */static char *id = "@(#) draw.c	2.2 5/31/88";E 12I 12D 14/* %Z% %M% Version %I% (%G%). copyright envos & Fuji Xerox  */static char *id = "%Z% %M%	%I% %G%		(envos & Fuji Xerox)";E 14I 14D 16/* @(#) draw.c Version 2.8 (4/7/89). copyright envos & Fuji Xerox  */static char *id = "@(#) draw.c	2.8 4/7/89		(envos & Fuji Xerox)";E 16I 16D 21/* %Z% %M% Version %I% (%G%). copyright envos & Fuji Xerox  */static char *id = "%Z% %M%	%I% %G%		(envos & Fuji Xerox)";E 21I 21D 22/* @(#) draw.c Version 2.16 (7/25/90). copyright envos & Fuji Xerox  */static char *id = "@(#) draw.c	2.16 7/25/90		(envos & Fuji Xerox)";E 22I 22D 23/* %Z% %M% Version %I% (%G%). copyright venue & Fuji Xerox  */static char *id = "%Z% %M%	%I% %G%	(venue & Fuji Xerox)";E 23I 23D 27/* @(#) draw.c Version 2.18 (9/18/90). copyright venue & Fuji Xerox  */static char *id = "@(#) draw.c	2.18 9/18/90	(venue & Fuji Xerox)";E 27I 27/* %Z% %M% Version %I% (%G%). copyright venue & Fuji Xerox  */static char *id = "%Z% %M%	%I% %G%	(venue & Fuji Xerox)";E 27E 23E 22E 21E 16E 14E 12E 8E 4E 2D 17/*	draw.c */E 17I 17/************************************************************************//*									*//*				D R A W . C				*//*									*/D 27/*	Line-drawing code.						*/E 27I 27/*	Line-drawing code, N_OP_drawline.				*/E 27/*									*//************************************************************************/I 26E 26/************************************************************************//*									*/D 26/*	Copyright 1989, 1990 Venue, Fuji Xerox Co., Ltd, Xerox Corp.	*/E 26I 26/*	(C) Copyright 1989-92 Venue. All Rights Reserved.		*//*	Manufactured in the United States of America.			*/E 26/*									*/D 26/*	This file is work-product resulting from the Xerox/Venue	*//*	Agreement dated 18-August-1989 for support of Medley.		*/E 26I 26/*	The contents of this file are proprietary information 		*//*	belonging to Venue, and are provided to you under license.	*//*	They may not be further distributed or disclosed to third	*//*	parties without the specific permission of Venue.		*/E 26/*									*//************************************************************************/E 17#include <stdio.h>I 9#include <sys/ioctl.h>I 13#ifndef NOPIXRECTE 13#include <sunwindow/window_hs.h>#include <sunwindow/win_ioctl.h>I 13#endifE 13E 9#include "lispemul.h"D 25#include "lispglobal.h"E 25I 25#include "lspglob.h"E 25D 9#include "address68k.h"E 9#include "lispmap.h"D 25#include "lisptypes.h"#include "emulglobal.h"I 9#include "address68k.h"E 25I 25#include "lsptypes.h"#include "emlglob.h"#include "adr68k.h"E 25#include "bitblt.h"I 21#include "display.h"E 21E 9#include "my.h"I 15#ifdef DISPLAYBUFFERextern struct pixrect *ColorDisplayPixrect, *DisplayRegionPixrect;I 16#endifE 16extern int DisplayWidth, DisplayHeight;extern int DisplayRasterWidth;D 21extern short *DisplayRegion68k;E 21D 22extern short *DisplayRegion68k_end_addr;E 22I 22extern DLword *DisplayRegion68k_end_addr;E 22D 16#endifE 16E 15I 9extern int LispWindowFd;D 11E 11I 11extern int ScreenLocked;I 15I 21#ifdef COLORextern int MonoOrColor;D 28#endif COLORE 28I 28#endif /* COLOR */E 28I 28E 28E 21#ifdef DISPLAYBUFFER n_new_cursorin_CG6( dx, dy, w, h )D 27register int dx		   , dy		   , w		   , h;E 27I 27register int dx, dy, w, h;E 27D 17{E 17I 17  {E 17    if((dx < MOUSEXR)&&(dx + w > MOUSEXL)&&(dy < MOUSEYH)&&(dy + h > MOUSEYL))D 17		return(T);E 17I 17	return(T);E 17    else D 17		return(NIL);}E 17I 17	return(NIL);  }E 17#endifE 15E 11E 9/************************************************************  #      name        len-1    stk level effect   UFN table entry  73     DRAWLINE     0       -8                 \DRAWLINE.UFND 27takes 8 (!) args from top of stack, does line draw inner loopE 27I 27takes 9 (!) args from top of stack, does line draw inner loopE 27   Entry format:     TOS  --> Maximum number of Y steps allowed {uYCount}     SP-0 --> Maximum number of X steps allowed {uXCount}     SP-2 --> Initial Delta {Delta}     SP-4 --> Function number: {L2}     		0: Set pixel		1: Reset Pixel		2: Invert Pixel     SP-6 --> Relative Y coordinate of end of line (for the bucket) {uYT}     SP-8 --> Width of bitmap (in words), signed to indicate Y direction {uYIncHi , uYIncLo}     SP-10 -> Relative X coordinate of end of line (for the bucket) {uXT}     SP-12 -> bit index [0..15] of bit to start at {CurBit}     SP-14 -> Pointer to first word in bitmap {uPAdrHi , uPAdrLo}D 12	E 12I 12E 12***********************************************************//*  curbit = bit position for next bit  *//*  delta = current value of delta  *//*  op = draw operation:  0/paint, 1/erase, 2/invert  *//*****plot:	if x = xmax or y = ymax then exit	data = data op curbit;	if (ymax > xmax) then {movex(-), if delta neg, then movey(+), goto plot}	else {movey(-), if delta neg, then movex(+), goto plot}	movex = {	  xmax--	  delta = delta +/- ysizeD 3	  curbit = curbit >> 1E 3I 3	  curbit >>= 1E 3	  if curbit = 0 then {curbit = 0x8000, incx}	}	movey = {	  ymax--	  delta = delta +/- xsize	  incy	}*****/I 13/*	ptr	pointer to "first word of bitmap"	curbit	bit index to start with, and tracks therefrom.	xsize	Rel X coord	width	width of bitmap, in words	ysize	Rel Y coord of end of line	op	operation	delta	initial delta	numx	step count for X steps	numy	step count for Y steps*/I 16#ifdef XWINDOW#define DISPLAYBUFFERD 28#endif XWINDOWE 28I 28#endif /* XWINDOW */E 28I 28E 28E 16E 13I 6D 10#ifdef C_ONLYE 6/************************************************************	OP_drawline  -- op 073***********************************************************/D 8/*  global counters  */static int cnt1 = 0;static int lim1 = 1;E 8OP_mydrawline(){D 4int numx;int numy;register DLword *dataptr;register int curbit;register int delta;register int ysize;register int xsize;int width;int xfirst;int op;int temp;E 4I 4	int numx;	int numy;	register DLword *dataptr;	register int curbit;	register int delta;	register int ysize;	register int xsize;	int width;	int xfirst;	int op;	int temp;E 4D 4    if ((++cnt1) == lim1) {      printf("draw, C = %d, drawline\n", lim1);      lim1 *= 2;      cnt1 = 0;    }/* end if cnt1 */  delta = 0xFFFF & *((int *)(CurrentStackPTR - 2));  op = 3 & *((int *)(CurrentStackPTR - 4));  width = *((int *)(CurrentStackPTR - 8));    if ((0xFFFF0000 & width) == S_POSITIVE) width &= 0xFFFF;    else if ((0xFFFF0000 & width) == S_NEGATIVE) width |= 0xFFFF0000;    else gotoufn;  curbit = 0xFFFF & *((LispPTR *)(CurrentStackPTR - 12));    curbit = 0x8000 >> curbit;  temp = *((int *)(CurrentStackPTR - 14));    dataptr = Addr68k_from_LADDR(temp);  numy = 0xFFFF & TopOfStack;  numx = 0xFFFF & *((int *)(CurrentStackPTR));  ysize = 0xFFFF & *((int *)(CurrentStackPTR - 6));  xsize = 0xFFFF & *((int *)(CurrentStackPTR - 10));    if (xfirst = (xsize > ysize)) delta = xsize - delta - 1;    else delta = ysize - delta - 1;E 4I 4D 8	if ((++cnt1) == lim1) {I 7#ifdef  DEBUGE 7		printf("draw, C = %d, drawline\n", lim1);I 7#endifE 7		lim1 *= 2;		cnt1 = 0;	}/* end if cnt1 */E 8	delta = 0xFFFF & *((int *)(CurrentStackPTR - 2));	op = 3 & *((int *)(CurrentStackPTR - 4));	width = *((int *)(CurrentStackPTR - 8));	if ((0xFFFF0000 & width) == S_POSITIVE) width &= 0xFFFF;	else if ((0xFFFF0000 & width) == S_NEGATIVE) width |= 0xFFFF0000;	else gotoufn;	curbit = 0xFFFF & *((LispPTR *)(CurrentStackPTR - 12));	curbit = 0x8000 >> curbit;	temp = *((int *)(CurrentStackPTR - 14));	dataptr = Addr68k_from_LADDR(temp);	numy = 0xFFFF & TopOfStack;	numx = 0xFFFF & *((int *)(CurrentStackPTR));	ysize = 0xFFFF & *((int *)(CurrentStackPTR - 6));	xsize = 0xFFFF & *((int *)(CurrentStackPTR - 10));	if (xfirst = (xsize > ysize)) delta = xsize - delta - 1;	else delta = ysize - delta - 1;E 4D 4        while ((numx) && (numy)) {/*     printf("del = %d,op = %d, width = %d, curbit = %x,\n  numx = %d, numy = %d, ysize = %d, xsize = %d,\n  xf = %d\n", delta, op, width, curbit, numx, numy, ysize, xsize, xfirst);  */        switch (op) {	  case 0:  *((DLword *)dataptr) |= curbit;	    break;	  case 1:  *((DLword *)dataptr) &= ~curbit;	    break;	  case 2:  *((DLword *)dataptr) ^= curbit;	    break;        } /* end switch */        if (xfirst) {	  numx--;D 3	  delta = delta - ysize;	  curbit = curbit >> 1;E 3I 3	  delta -= ysize;	  curbit >>= 1;E 3	  if (!curbit) {	    curbit = 0x8000;	    dataptr++;	  } /* end if curbit */	  if (delta < 0) {	    numy--;D 3	    delta = delta + xsize;E 3I 3	    delta += xsize;E 3	    dataptr += width;	  } /* end if delta */	} /* end if xfirst */	else { /* yfirst */	  numy--;D 3	  delta = delta - xsize;E 3I 3	  delta -= xsize;E 3	  dataptr += width;	  if (delta < 0) {	    numx--;D 3	    delta = delta + ysize;	    curbit = curbit >> 1;E 3I 3	    delta += ysize;	    curbit >>= 1;E 3	    if (!curbit) {	      curbit = 0x8000;	      dataptr++;	    } /* end if curbit*/	  } /* end if delta */	} /* end else */      } /* end while numx */        CurrentStackPTR -= 16;      PC++;E 4I 4	while ((numx) && (numy)) {D 8		/*     printf("del = %d,op = %d, width = %d, curbit = %x,\n  numx = %d, numy = %d, ysize = %d, xsize = %d,\n  xf = %d\n", delta, op, width, curbit, numx, numy, ysize, xsize, xfirst);  */E 8		switch (op) {		case 0:  			*((DLword *)dataptr) |= curbit;			break;		case 1:  			*((DLword *)dataptr) &= ~curbit;			break;		case 2:  			*((DLword *)dataptr) ^= curbit;			break;		} /* end switch */		if (xfirst) {			numx--;			delta -= ysize;			curbit >>= 1;			if (!curbit) {				curbit = 0x8000;				dataptr++;			} /* end if curbit */			if (delta < 0) {				numy--;				delta += xsize;				dataptr += width;			} /* end if delta */		} /* end if xfirst */		else { /* yfirst */			numy--;			delta -= xsize;			dataptr += width;			if (delta < 0) {				numx--;				delta += ysize;				curbit >>= 1;				if (!curbit) {					curbit = 0x8000;					dataptr++;				} /* end if curbit*/			} /* end if delta */		} /* end else */	} /* end while numx */	CurrentStackPTR -= 16;	PC++;E 4}  /* end OP_drawline  */I 6#elseE 6N_OP_drawline(ptr, curbit, xsize, width, ysize, op, delta, numx, numy, error_addr)E 10I 10D 27N_OP_drawline(ptr, curbit, xsize, width, ysize, op, delta, numx, numy)E 10D 3int curbit, xsize, width, ysize, op, delta, numx, numy, error_addr;E 3I 3register int curbit, xsize;	 int width;register int ysize;	 int op;register int delta, numx, numy;E 27I 27int N_OP_drawline(ptr, curbit, xsize, width, ysize, op, delta, numx, numy)  register int curbit, xsize;	   int width;  register int ysize;	   int op;  register int delta, numx, numy;  LispPTR ptr;  {    register DLword *dataptr;    ScreenLocked=T;E 27D 8	 int error_addr;E 8I 8D 10	 int *error_addr;E 10E 8E 3D 27LispPTR ptr;{D 4register DLword *dataptr;D 3int xfirst;E 3I 3int displayflg;E 4I 4D 13	register DLword *dataptr;E 13I 13D 15	register DLword *dataptr, *initptr;	int initcurbit, xlim, ylim;E 15I 15	register DLword *dataptr;E 15E 13D 9	int displayflg;E 9E 4E 3D 8I 2#ifdef	TRACE	printPC();	printf("TRACE: N_OP_drawline()\n");#endifE 8E 2D 3  delta = 0xFFFF & delta;  op = 3 & op;  if ((0xFFFF0000 & width) == S_POSITIVE) width &= 0xFFFF;E 3I 3D 11E 11I 11	ScreenLocked=T;I 15D 21	E 21I 21E 27#ifdef COLORD 27	if( MonoOrColor == MONO_SCREEN )E 27I 27    if( MonoOrColor == MONO_SCREEN )E 27D 24#endif COLOR	E 24I 24D 28#endif COLORE 28I 28#endif /* COLOR */E 28#ifdef SUNDISPLAYE 24E 21#ifndef DISPLAYBUFFERE 15E 11D 4  hidecursor();		/** Figure out how to be smart later **/E 4I 4D 9	hidecursor();		/** Figure out how to be smart later **/E 9I 9D 27	HideCursor;		/** Figure out how to be smart later **/E 9E 4D 15E 15I 15D 20#endifE 20I 20#elseE 27I 27    HideCursor;		/** Figure out how to be smart later **/#else /* DISPLAYBUFFER */E 27#ifndef OLD_CURSORD 27	HideCursor;E 27I 27    HideCursor;E 27D 28#endif OLD_CURSOR#endif DISPLAYBUFFERI 24#endif SUNDISPLAYE 28I 28#endif /* OLD_CURSOR */#endif /* DISPLAYBUFFER */#endif /* SUNDISPLAY */E 28E 24E 20	E 15D 4  delta &= 0xFFFF;  op &= 3;  if      ((0xFFFF0000 & width) == S_POSITIVE) width &= 0xFFFF;E 3  else if ((0xFFFF0000 & width) == S_NEGATIVE) width |= 0xFFFF0000;  else ERROR_EXIT(error_addr);  curbit = 0x8000 >> (curbit & 0xFFFF);  dataptr = Addr68k_from_LADDR(ptr);D 3  numy = 0xFFFF & numy;  numx = 0xFFFF & numx;  ysize = 0xFFFF & ysize;  xsize = 0xFFFF & xsize;  if (xfirst = (xsize > ysize)) {E 3I 3  numy  &= 0xFFFF;  numx  &= 0xFFFF;  ysize &= 0xFFFF;  xsize &= 0xFFFF;  if (xsize > ysize) {E 3    delta = xsize - delta - 1;    switch (op) {      case 0:D 3        while ((numx) && (numy)) {E 3I 3        while (numx && numy) {E 3	  if (!curbit) {	    curbit = 0x8000;	    dataptr++;	  } /* end if curbit */          *((DLword *)dataptr) |= curbit;	  numx--;D 3	  delta = delta - ysize;	  curbit = curbit >> 1;E 3I 3	  delta -= ysize;	  curbit >>= 1;E 3	  if (delta < 0) {	    numy--;D 3	    delta = delta + xsize;E 3I 3	    delta += xsize;E 3	    dataptr += width;	  } /* end if delta */        }/* end while */	break;      case 1:D 3        while ((numx) && (numy)) {E 3I 3        while (numx && numy) {E 3	  if (!curbit) {	    curbit = 0x8000;	    dataptr++;	  } /* end if curbit */          *((DLword *)dataptr) &= ~curbit;	  numx--;D 3	  delta = delta - ysize;	  curbit = curbit >> 1;E 3I 3	  delta -= ysize;	  curbit >>= 1;E 3	  if (delta < 0) {	    numy--;D 3	    delta = delta + xsize;E 3I 3	    delta   += xsize;E 3	    dataptr += width;	  } /* end if delta */        }/* end while */	break;      case 2:D 3        while ((numx) && (numy)) {E 3I 3        while (numx && numy) {E 3	  if (!curbit) {	    curbit = 0x8000;	    dataptr++;	  } /* end if curbit */          *((DLword *)dataptr) ^= curbit;	  numx--;D 3	  delta = delta - ysize;	  curbit = curbit >> 1;E 3I 3	  delta -= ysize;	  curbit >>= 1;E 3	  if (delta < 0) {	    numy--;D 3	    delta = delta + xsize;E 3I 3	    delta   += xsize;E 3	    dataptr += width;	  } /* end if delta */        }/* end while */	break;    } /* end switch */D 3  }/* end if xfirst */E 3I 3  }/* end if */E 3  else { /* yfirst */    delta = ysize - delta - 1;    switch (op) {      case 0:D 3        while ((numx) && (numy)) {E 3I 3        while (numx && numy) {E 3          *((DLword *)dataptr) |= curbit;	  numy--;D 3	  delta = delta - xsize;E 3I 3	  delta   -= xsize;E 3	  dataptr += width;	  if (delta < 0) {	    numx--;D 3	    delta = delta + ysize;	    curbit = curbit >> 1;E 3I 3	    delta += ysize;	    curbit >>= 1;E 3	    if (!curbit) {	      curbit = 0x8000;	      dataptr++;	    } /* end if curbit */	  } /* end if delta */        }/* end while */	break;      case 1:D 3        while ((numx) && (numy)) {E 3I 3        while (numx && numy) {E 3          *((DLword *)dataptr) &= ~curbit;	  numy--;D 3	  delta = delta - xsize;E 3I 3	  delta   -= xsize;E 3	  dataptr += width;	  if (delta < 0) {	    numx--;D 3	    delta = delta + ysize;	    curbit = curbit >> 1;E 3I 3	    delta += ysize;	    curbit >>= 1;E 3	    if (!curbit) {	      curbit = 0x8000;	      dataptr++;	    } /* end if curbit */	  } /* end if delta */        }/* end while */	break;      case 2:D 3        while ((numx) && (numy)) {E 3I 3        while (numx && numy) {E 3          *((DLword *)dataptr) ^= curbit;	  numy--;D 3	  delta = delta - xsize;E 3I 3	  delta   -= xsize;E 3	  dataptr += width;	  if (delta < 0) {	    numx--;D 3	    delta = delta + ysize;	    curbit = curbit >> 1;E 3I 3	    delta += ysize;	    curbit >>= 1;E 3	    if (!curbit) {	      curbit = 0x8000;	      dataptr++;	    } /* end if curbit */	  } /* end if delta */        }/* end while */	break;    } /* end switch */  } /* end else */I 3  showcursor();		/** figure how to be smart later **/E 4I 4D 27	delta &= 0xFFFF;	op &= 3;	if      ((0xFFFF0000 & width) == S_POSITIVE) width &= 0xFFFF;	else if ((0xFFFF0000 & width) == S_NEGATIVE) width |= 0xFFFF0000;D 10	else ERROR_EXIT(error_addr);E 10I 10	else ERROR_EXIT(numy);I 13D 15	initcurbit = curbit & 0xFFFF;E 15E 13E 10	curbit = 0x8000 >> (curbit & 0xFFFF);	dataptr = Addr68k_from_LADDR(ptr);I 13D 15	initptr = dataptr;E 15E 13	numy  &= 0xFFFF;	numx  &= 0xFFFF;I 13D 15	xlim = numx; ylim = numy;E 15E 13	ysize &= 0xFFFF;	xsize &= 0xFFFF;	if (xsize > ysize) {		delta = xsize - delta - 1;		switch (op) {		case 0:			while (numx && numy) {				if (!curbit) {E 27I 27    delta &= 0xFFFF;    op &= 3;D 29    if      ((0xFFFF0000 & width) == S_POSITIVE) width &= 0xFFFF;      else if ((0xFFFF0000 & width) == S_NEGATIVE) width |= 0xFFFF0000;E 29I 29    if      ((SEGMASK & width) == S_POSITIVE) width &= 0xFFFF;      else if ((SEGMASK & width) == S_NEGATIVE) width |= 0xFFFF0000;E 29        else ERROR_EXIT(numy);     curbit = 0x8000 >> (curbit & 0xFFFF);     dataptr = Addr68k_from_LADDR(ptr);     numy  &= 0xFFFF;     numx  &= 0xFFFF;     ysize &= 0xFFFF;     xsize &= 0xFFFF;     if (xsize > ysize)      {	delta = xsize - delta - 1;	switch (op)	  {	    case 0:		while (numx && numy)		  {		     if (!curbit)		      {			curbit = 0x8000;			dataptr++;		      } /* end if curbit */		     GETWORD(dataptr) |= curbit;		     numx--;		     delta -= ysize;		     curbit >>= 1;		     if (delta < 0)		      {			numy--;			delta += xsize;			dataptr += width;		      } /* end if delta */		  }/* end while */		break;	    case 1:	      while (numx && numy)		{		  if (!curbit) {E 27					curbit = 0x8000;					dataptr++;				} /* end if curbit */D 17				*((DLword *)dataptr) |= curbit;E 17I 17D 27				GETWORD(dataptr) |= curbit;E 17				numx--;				delta -= ysize;				curbit >>= 1;				if (delta < 0) {					numy--;					delta += xsize;					dataptr += width;				} /* end if delta */			}/* end while */			break;		case 1:			while (numx && numy) {				if (!curbit) {					curbit = 0x8000;					dataptr++;				} /* end if curbit */D 17				*((DLword *)dataptr) &= ~curbit;E 17I 17				GETWORD(dataptr) &= ~curbit;E 17				numx--;				delta -= ysize;				curbit >>= 1;				if (delta < 0) {					numy--;					delta   += xsize;					dataptr += width;				} /* end if delta */			}/* end while */			break;		case 2:			while (numx && numy) {				if (!curbit) {					curbit = 0x8000;					dataptr++;				} /* end if curbit */D 17				*((DLword *)dataptr) ^= curbit;E 17I 17				GETWORD(dataptr) ^= curbit;E 17				numx--;				delta -= ysize;				curbit >>= 1;				if (delta < 0) {					numy--;					delta   += xsize;					dataptr += width;				} /* end if delta */			}/* end while */			break;		} /* end switch */	}/* end if */	else { /* yfirst */		delta = ysize - delta - 1;		switch (op) {		case 0:			while (numx && numy) {D 17				*((DLword *)dataptr) |= curbit;E 17I 17				GETWORD(dataptr) |= curbit;E 17				numy--;				delta   -= xsize;				dataptr += width;				if (delta < 0) {					numx--;					delta += ysize;					curbit >>= 1;					if (!curbit) {						curbit = 0x8000;						dataptr++;					} /* end if curbit */				} /* end if delta */			}/* end while */			break;		case 1:			while (numx && numy) {D 17				*((DLword *)dataptr) &= ~curbit;E 17I 17				GETWORD(dataptr) &= ~curbit;E 17				numy--;				delta   -= xsize;				dataptr += width;				if (delta < 0) {					numx--;					delta += ysize;					curbit >>= 1;					if (!curbit) {						curbit = 0x8000;						dataptr++;					} /* end if curbit */				} /* end if delta */			}/* end while */			break;		case 2:			while (numx && numy) {D 17				*((DLword *)dataptr) ^= curbit;E 17I 17				GETWORD(dataptr) ^= curbit;E 17				numy--;				delta   -= xsize;				dataptr += width;				if (delta < 0) {					numx--;					delta += ysize;					curbit >>= 1;					if (!curbit) {						curbit = 0x8000;						dataptr++;					} /* end if curbit */				} /* end if delta */			}/* end while */			break;		} /* end switch */	} /* end else */E 27I 27		  GETWORD(dataptr) &= ~curbit;		  numx--;		  delta -= ysize;		  curbit >>= 1;		  if (delta < 0)		    {		      numy--;		      delta   += xsize;		      dataptr += width;		    } /* end if delta */		}/* end while */	      break;	    case 2:	      while (numx && numy)		{		  if (!curbit) { curbit = 0x8000; dataptr++; }		  GETWORD(dataptr) ^= curbit;		  numx--;		  delta -= ysize;		  curbit >>= 1;		  if (delta < 0)		    {		      numy--;		      delta   += xsize;		      dataptr += width;		    } /* end if delta */		}/* end while */	      break;	  } /* end switch */      }/* end if */    else      { /* yfirst */	delta = ysize - delta - 1;	switch (op)	  {	    case 0:	      while (numx && numy)		{		  GETWORD(dataptr) |= curbit;		  numy--;		  delta   -= xsize;		  dataptr += width;		  if (delta < 0)		    {		      numx--;		      delta += ysize;		      curbit >>= 1;		      if (!curbit) { curbit = 0x8000; dataptr++; }		    } /* end if delta */		}/* end while */	      break;	    case 1:	      while (numx && numy)		{		  GETWORD(dataptr) &= ~curbit;		  numy--;		  delta   -= xsize;		  dataptr += width;		  if (delta < 0)		    {		      numx--;		      delta += ysize;		      curbit >>= 1;		      if (!curbit) { curbit = 0x8000; dataptr++; }		    } /* end if delta */		  }/* end while */	      break;	    case 2:	      while (numx && numy)		{		  GETWORD(dataptr) ^= curbit;		  numy--;		  delta   -= xsize;		  dataptr += width;		  if (delta < 0)		    {		      numx--;		      delta += ysize;		      curbit >>= 1;		      if (!curbit) { curbit = 0x8000; dataptr++; }		    } /* end if delta */		}/* end while */	      break;	  } /* end switch */      } /* end else */E 27I 13D 21E 21I 21#ifdef COLORD 27	if( MonoOrColor == MONO_SCREEN )E 27I 27    if( MonoOrColor == MONO_SCREEN )E 27D 28#endif COLORE 28I 28#endif /* COLOR */E 28I 24#ifdef SUNDISPLAYE 24E 21D 15#ifdef DISPLAYBUFFER	if (in_display_segment(initptr))	  {	    flush_display_ptrregion((width < 0)? initptr+(width*ylim): initptr, 				    initcurbit, xlim, ylim);	  }E 15I 15#ifndef DISPLAYBUFFERD 27	ShowCursor;		/** figure how to be smart later **/E 27I 27    ShowCursor;		/** figure how to be smart later **/E 27E 15D 20#endifE 20I 20#else #ifndef OLD_CURSORD 27	ShowCursor;E 27I 27    ShowCursor;E 27D 28#endif OLD_CURSOR#endif DISPLAYBUFFERI 24#endif SUNDISPLAYE 28I 28#endif /* OLD_CURSOR */E 28E 24E 20I 28#endif /* DISPLAYBUFFER */#endif /* SUNDISPLAY */E 28E 13D 9	showcursor();		/** figure how to be smart later **/E 9I 9D 15	ShowCursor;		/** figure how to be smart later **/I 11	ScreenLocked=NIL;E 11E 9E 4E 3}  /* end N_OP_drawline()  */E 15I 15#ifdef DISPLAYBUFFERI 21#ifdef COLORD 22	if( MonoOrColor == MONO_SCREEN )E 22I 22    if( MonoOrColor == MONO_SCREEN )E 22D 28#endif COLORE 28I 28#endif /* COLOR */E 28E 21D 22	{		short	  *start_addr		  	, *temp_s		    	, *temp_e;E 22I 22      {	DLword	  *start_addr	  	, *temp_s	    	, *temp_e;E 22E 15I 6I 15D 22		start_addr = (short*)Addr68k_from_LADDR(ptr);E 22I 22	start_addr = (DLword*)Addr68k_from_LADDR(ptr);E 22E 15D 10#endif /* not C_ONLY */E 10E 6D 4	/* end module */E 4I 4D 15/* end module */E 15I 15D 22		if( ((temp_s = (short*)(start_addr - DisplayRegion68k)) >= 0 )		   && (start_addr < DisplayRegion68k_end_addr)		   && ((temp_e = (short*)((short*)dataptr - DisplayRegion68k)) >= 0 )		   && ((short*)dataptr< DisplayRegion68k_end_addr) ) {E 22I 22D 26	if( ((temp_s = (DLword*)(start_addr - DisplayRegion68k)) >= 0 )E 26I 26	if( ((int)(temp_s = (DLword*)(start_addr - DisplayRegion68k)) >= 0 )E 26	   && (start_addr < DisplayRegion68k_end_addr)D 26	   && ((temp_e = (DLword*)(dataptr - DisplayRegion68k)) >= 0 )E 26I 26	   && ((int)(temp_e = (DLword*)(dataptr - DisplayRegion68k)) >= 0 )E 26	   && ((DLword*)dataptr < DisplayRegion68k_end_addr) )	  {E 22E 15E 4I 15D 22			int	  start_x			  	, start_y			    	, end_x			      	, end_y				, w				, h				, displayflg;E 22I 22	    int	  start_x	  	, start_y	    	, end_x	      	, end_y		, w		, h		, displayflg;E 22E 15I 15D 22			start_y = (int)temp_s / DisplayRasterWidth;			start_x = ((int)temp_s % DisplayRasterWidth) * BITSPER_DLWORD;E 22I 22	    start_y = (int)temp_s / DisplayRasterWidth;	    start_x = ((int)temp_s % DisplayRasterWidth) * BITSPER_DLWORD;E 22E 15I 15D 22			end_y = (int)temp_e	/ DisplayRasterWidth ;			end_x = ((int)temp_e % DisplayRasterWidth) * BITSPER_DLWORD E 22I 22	    end_y = (int)temp_e	/ DisplayRasterWidth ;	    end_x = ((int)temp_e % DisplayRasterWidth) * BITSPER_DLWORD E 22			  + (BITSPER_DLWORD - 1);E 15I 15D 22			w = abs(start_x - end_x) + 1;			h = abs(start_y - end_y) + 1;E 22I 22	    w = abs(start_x - end_x) + 1;	    h = abs(start_y - end_y) + 1;E 22E 15I 15D 22			if(start_x > end_x) start_x = end_x;			if(start_y > end_y) start_y = end_y;E 22I 22	    if(start_x > end_x) start_x = end_x;	    if(start_y > end_y) start_y = end_y;E 22E 15I 16D 18#ifdef SUNDISPLAYE 18I 18#if defined(SUNDISPLAY) && !defined(BYTESWAP)I 20#ifdef OLD_CURSORE 20E 18E 16I 15D 22			if( (displayflg = n_new_cursorin_CG6( start_x, start_yD 16							     , w, h )) )											E 16I 16							     , w, h )) )											E 16			  HideCursor;E 22I 22	    if( (displayflg = n_new_cursorin_CG6( start_x, start_y, w, h )) )	      HideCursor;E 22E 15D 20E 20I 20D 28#endif OLD_CURSORE 28I 28#endif /* OLD_CURSOR */E 28E 20I 15D 22			pr_rop(ColorDisplayPixrect, start_x, start_y, w, h,			       PIX_SRC,DisplayRegionPixrect, start_x, start_y );E 22I 22	    pr_rop(ColorDisplayPixrect, start_x, start_y, w, h,	           PIX_SRC,DisplayRegionPixrect, start_x, start_y );E 22E 15D 20E 20I 20#ifdef OLD_CURSORE 20I 15D 22			if( displayflg ) 			  ShowCursor;E 22I 22	    if( displayflg ) ShowCursor;E 22I 20D 28#endif OLD_CURSORE 20I 16#endif SUNDISPLAYE 28I 28#endif /* OLD_CURSOR */E 28E 16E 15I 28#endif /* SUNDISPLAY */E 28I 16D 18#ifdef XWINDOWE 18I 18#if defined(XWINDOW) || defined(BYTESWAP)E 18D 22			flush_display_region( start_x, start_y, w, h);E 22I 22	    flush_display_region( start_x, start_y, w, h);E 22D 28#endif XWINDOWE 28I 28#endif /* XWINDOW */E 28I 28E 28E 16I 15D 22		}E 22I 22	   }E 22	}D 16#endifE 16I 16D 28#endif DISPLAYBUFFERE 28I 28#endif /* DISPLAYBUFFER */E 28E 16I 28E 28D 22	ScreenLocked=NIL;}  /* end N_OP_drawline()  */E 22I 22    ScreenLocked=NIL;I 27    return(0);	/* return a value for the error test to check. */E 27  }  /* end N_OP_drawline()  */E 22I 16#ifdef XWINDOW#undef DISPLAYBUFFERD 28#endif XWINDOWE 28I 28#endif /* XWINDOW */E 28E 16/* end module */E 15E 1