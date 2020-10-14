/* This is G-file @(#) draw.c Version 2.2 (5/31/88). copyright Xerox & Fuji Xerox  */static char *id = "@(#) draw.c	2.2 5/31/88";/*	draw.c */#include <stdio.h>#include "lispemul.h"#include "lispglobal.h"#include "address68k.h"#include "lispmap.h"#include "lisptypes.h"#include "emulglobal.h"#include "my.h"/************************************************************  #      name        len-1    stk level effect   UFN table entry  73     DRAWLINE     0       -8                 \DRAWLINE.UFNtakes 8 (!) args from top of stack, does line draw inner loop   Entry format:     TOS  --> Maximum number of Y steps allowed {uYCount}     SP-0 --> Maximum number of X steps allowed {uXCount}     SP-2 --> Initial Delta {Delta}     SP-4 --> Function number: {L2}     		0: Set pixel		1: Reset Pixel		2: Invert Pixel     SP-6 --> Relative Y coordinate of end of line (for the bucket) {uYT}     SP-8 --> Width of bitmap (in words), signed to indicate Y direction {uYIncHi , uYIncLo}     SP-10 -> Relative X coordinate of end of line (for the bucket) {uXT}     SP-12 -> bit index [0..15] of bit to start at {CurBit}     SP-14 -> Pointer to first word in bitmap {uPAdrHi , uPAdrLo}	***********************************************************//*  curbit = bit position for next bit  *//*  delta = current value of delta  *//*  op = draw operation:  0/paint, 1/erase, 2/invert  *//*****plot:	if x = xmax or y = ymax then exit	data = data op curbit;	if (ymax > xmax) then {movex(-), if delta neg, then movey(+), goto plot}	else {movey(-), if delta neg, then movex(+), goto plot}	movex = {	  xmax--	  delta = delta +/- ysize	  curbit >>= 1	  if curbit = 0 then {curbit = 0x8000, incx}	}	movey = {	  ymax--	  delta = delta +/- xsize	  incy	}*****/#ifdef C_ONLY/************************************************************	OP_drawline  -- op 073***********************************************************/OP_mydrawline(){	int numx;	int numy;	register DLword *dataptr;	register int curbit;	register int delta;	register int ysize;	register int xsize;	int width;	int xfirst;	int op;	int temp;	delta = 0xFFFF & *((int *)(CurrentStackPTR - 2));	op = 3 & *((int *)(CurrentStackPTR - 4));	width = *((int *)(CurrentStackPTR - 8));	if ((0xFFFF0000 & width) == S_POSITIVE) width &= 0xFFFF;	else if ((0xFFFF0000 & width) == S_NEGATIVE) width |= 0xFFFF0000;	else gotoufn;	curbit = 0xFFFF & *((LispPTR *)(CurrentStackPTR - 12));	curbit = 0x8000 >> curbit;	temp = *((int *)(CurrentStackPTR - 14));	dataptr = Addr68k_from_LADDR(temp);	numy = 0xFFFF & TopOfStack;	numx = 0xFFFF & *((int *)(CurrentStackPTR));	ysize = 0xFFFF & *((int *)(CurrentStackPTR - 6));	xsize = 0xFFFF & *((int *)(CurrentStackPTR - 10));	if (xfirst = (xsize > ysize)) delta = xsize - delta - 1;	else delta = ysize - delta - 1;	while ((numx) && (numy)) {		switch (op) {		case 0:  			*((DLword *)dataptr) |= curbit;			break;		case 1:  			*((DLword *)dataptr) &= ~curbit;			break;		case 2:  			*((DLword *)dataptr) ^= curbit;			break;		} /* end switch */		if (xfirst) {			numx--;			delta -= ysize;			curbit >>= 1;			if (!curbit) {				curbit = 0x8000;				dataptr++;			} /* end if curbit */			if (delta < 0) {				numy--;				delta += xsize;				dataptr += width;			} /* end if delta */		} /* end if xfirst */		else { /* yfirst */			numy--;			delta -= xsize;			dataptr += width;			if (delta < 0) {				numx--;				delta += ysize;				curbit >>= 1;				if (!curbit) {					curbit = 0x8000;					dataptr++;				} /* end if curbit*/			} /* end if delta */		} /* end else */	} /* end while numx */	CurrentStackPTR -= 16;	PC++;}  /* end OP_drawline  */#elseN_OP_drawline(ptr, curbit, xsize, width, ysize, op, delta, numx, numy, error_addr)register int curbit, xsize;	 int width;register int ysize;	 int op;register int delta, numx, numy;	 int *error_addr;LispPTR ptr;{	register DLword *dataptr;	int displayflg;	hidecursor();		/** Figure out how to be smart later **/	delta &= 0xFFFF;	op &= 3;	if      ((0xFFFF0000 & width) == S_POSITIVE) width &= 0xFFFF;	else if ((0xFFFF0000 & width) == S_NEGATIVE) width |= 0xFFFF0000;	else ERROR_EXIT(error_addr);	curbit = 0x8000 >> (curbit & 0xFFFF);	dataptr = Addr68k_from_LADDR(ptr);	numy  &= 0xFFFF;	numx  &= 0xFFFF;	ysize &= 0xFFFF;	xsize &= 0xFFFF;	if (xsize > ysize) {		delta = xsize - delta - 1;		switch (op) {		case 0:			while (numx && numy) {				if (!curbit) {					curbit = 0x8000;					dataptr++;				} /* end if curbit */				*((DLword *)dataptr) |= curbit;				numx--;				delta -= ysize;				curbit >>= 1;				if (delta < 0) {					numy--;					delta += xsize;					dataptr += width;				} /* end if delta */			}/* end while */			break;		case 1:			while (numx && numy) {				if (!curbit) {					curbit = 0x8000;					dataptr++;				} /* end if curbit */				*((DLword *)dataptr) &= ~curbit;				numx--;				delta -= ysize;				curbit >>= 1;				if (delta < 0) {					numy--;					delta   += xsize;					dataptr += width;				} /* end if delta */			}/* end while */			break;		case 2:			while (numx && numy) {				if (!curbit) {					curbit = 0x8000;					dataptr++;				} /* end if curbit */				*((DLword *)dataptr) ^= curbit;				numx--;				delta -= ysize;				curbit >>= 1;				if (delta < 0) {					numy--;					delta   += xsize;					dataptr += width;				} /* end if delta */			}/* end while */			break;		} /* end switch */	}/* end if */	else { /* yfirst */		delta = ysize - delta - 1;		switch (op) {		case 0:			while (numx && numy) {				*((DLword *)dataptr) |= curbit;				numy--;				delta   -= xsize;				dataptr += width;				if (delta < 0) {					numx--;					delta += ysize;					curbit >>= 1;					if (!curbit) {						curbit = 0x8000;						dataptr++;					} /* end if curbit */				} /* end if delta */			}/* end while */			break;		case 1:			while (numx && numy) {				*((DLword *)dataptr) &= ~curbit;				numy--;				delta   -= xsize;				dataptr += width;				if (delta < 0) {					numx--;					delta += ysize;					curbit >>= 1;					if (!curbit) {						curbit = 0x8000;						dataptr++;					} /* end if curbit */				} /* end if delta */			}/* end while */			break;		case 2:			while (numx && numy) {				*((DLword *)dataptr) ^= curbit;				numy--;				delta   -= xsize;				dataptr += width;				if (delta < 0) {					numx--;					delta += ysize;					curbit >>= 1;					if (!curbit) {						curbit = 0x8000;						dataptr++;					} /* end if curbit */				} /* end if delta */			}/* end while */			break;		} /* end switch */	} /* end else */	showcursor();		/** figure how to be smart later **/}  /* end N_OP_drawline()  */#endif /* not C_ONLY *//* end module */