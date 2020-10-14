h28050s 00000/00000/00115d D 1.6 93/02/08 14:40:49 sybalsky 6 5c Big VM (and new CDR coding) changes for 3.0 \nes 00000/00000/00115d D 1.5 92/04/21 16:55:22 sybalsky 5 4c shortening file names for DOS \nes 00001/00001/00114d D 1.4 92/03/20 17:22:31 sybalsky 4 3c Remove RCLK call in kbd ring-buffer-fill code, since the time value is never used.es 00074/00096/00041d D 1.3 90/09/18 23:26:26 sybalsky 3 2c Retrofit changes from AIX, DEC3100, PS/2es 00000/00000/00137d D 1.2 90/04/20 01:06:25 sybalsky 2 1c AIX:  shortening file names, bulk change.es 00137/00000/00000d D 1.1 90/03/01 15:30:08 osamu 1 0c date and time created 90/03/01 15:30:08 by osamueuUf e 0tTI 1/* %Z% %M% Version %I% (%G%). copyright venue & Fuji Xerox  */static char *id = "%Z% %M%	%I% %G%	(venue & Fuji Xerox)";/**** Copyright (C) 1988 by Fuji Xerox co.,Ltd. All rights reserved.**		Author: Mitsunori Matsuda*		Date  : September 19, 1988*/I 3/************************************************************************//*									*//*	Copyright 1989, 1990 Venue, Fuji Xerox Co., Ltd, Xerox Corp.	*//*									*//*	This file is work-product resulting from the Xerox/Venue	*//*	Agreement dated 18-August-1989 for support of Medley.		*//*									*//************************************************************************/E 3#include <stdio.h>#include <sys/time.h>#include "lispemul.h"I 3#include "keyboard.h"#include "dbprint.h"E 3D 3#define MAXKEYEVENT    4589#define MINKEYEVENT       2#define KEYEVENTSIZE     12#define KB_ALLUP     0xFFFFE 3D 3#define RING_READ(head68k)  (((RING*)(head68k))->read)#define RING_WRITE(head68k) (((RING*)(head68k))->write)#define RCLK(place) { struct timeval time; \			gettimeofday(&time,NULL); \			(place)=(time.tv_sec * 1000000)+time.tv_usec;}typedef struct {	DLword read;	DLword write;} RING;typedef struct {	DLword   W0;	DLword   W1;	DLword   W2;	DLword   W3;	DLword   WU;	DLword   W4;	DLword   W5;	int      time;	unsigned mousestate :3;	unsigned shift1     :1;	unsigned shift2     :1;	unsigned lock       :1;	unsigned ctrl       :1;	unsigned meta       :1;	unsigned font       :1;	unsigned usermode1  :1;	unsigned usermode2  :1;	unsigned usermode3  :1;	unsigned nil        :4;	DLword   mousex;	DLword   mousey;} KBEVENT;E 3extern DLword *EmKbdAd068K	    , *EmKbdAd168K	    , *EmKbdAd268K	    , *EmKbdAd368K	    , *EmKbdAd468K	    , *EmKbdAd568K	    , *EmRealUtilin68K;extern DLword *CTopKeyevent;extern LispPTR *KEYBUFFERING68k;extern int URaid_req	 , KBDEventFlg;I 3/************************************************************************//*									*//*				D o R i n g				*//*									*//*	Take keyboard events and turn them into Lisp event info		*//*	(when running under X)						*//*									*//************************************************************************/E 3DoRing()D 3{#ifdef TRACE	printf( "TRACE: DoRing()\n" );#endif	DLword w	     , r;	KBEVENT *kbevent;E 3I 3  {    DLword w         , r;    KBEVENT *kbevent;    TPRINT(( "TRACE: DoRing()\n" ));E 3do_ring:D 3	if(((*EmKbdAd268K) & 2113) == 0)	 {/*Ctrl-shift-NEXT*/		error("******  EMERGENCY Interrupt ******");		*EmKbdAd268K=KB_ALLUP; /*reset*/		((RING*)CTopKeyevent)->read=0;  /* reset queue */		((RING*)CTopKeyevent)->write=MINKEYEVENT; 		/*return(0);*/	 } 	else if(((*EmKbdAd268K) & 2114) == 0)	     { /* Ctrl-Shift-DEL */		*EmKbdAd268K=KB_ALLUP; /*reset*/		URaid_req=T;		((RING*)CTopKeyevent)->read=0;  /* reset queue */		((RING*)CTopKeyevent)->write=MINKEYEVENT; 		/*return(0);*/	     }E 3I 3    if(((*EmKbdAd268K) & 2113) == 0)      { /*Ctrl-shift-NEXT*/	error("******  EMERGENCY Interrupt ******");	*EmKbdAd268K=KB_ALLUP; /*reset*/	((RING*)CTopKeyevent)->read=0;  /* reset queue */	((RING*)CTopKeyevent)->write=MINKEYEVENT; 	/*return(0);*/      }     else if(((*EmKbdAd268K) & 2114) == 0)      { /* Ctrl-Shift-DEL */	*EmKbdAd268K=KB_ALLUP; /*reset*/	URaid_req=T;	((RING*)CTopKeyevent)->read=0;  /* reset queue */	((RING*)CTopKeyevent)->write=MINKEYEVENT; 	/*return(0);*/      }E 3	     #ifdef OS4_TYPE4BUGD 3	else if(((*EmKbdAd268K) & 2120) == 0)	     { /* Ctrl-Shift-Return */		*EmKbdAd268K=KB_ALLUP; /*reset*/		URaid_req=T;		((RING*)CTopKeyevent)->read=0;  /* reset queue */		((RING*)CTopKeyevent)->write=MINKEYEVENT; 		     }E 3I 3    else if(((*EmKbdAd268K) & 2120) == 0)      { /* Ctrl-Shift-Return */	*EmKbdAd268K=KB_ALLUP; /*reset*/	URaid_req=T;	((RING*)CTopKeyevent)->read=0;  /* reset queue */	((RING*)CTopKeyevent)->write=MINKEYEVENT;       }E 3#endifD 3	r=RING_READ(CTopKeyevent);	w=RING_WRITE(CTopKeyevent);E 3I 3    r=RING_READ(CTopKeyevent);    w=RING_WRITE(CTopKeyevent);E 3D 3	if(r==w) /* event queqe FULL */		goto KBnext;	kbevent=(KBEVENT*)(CTopKeyevent+ w);#ifndef BYTESWAP	RCLK(kbevent->time);#else		RCLK(kbevent->time_hi, kbevent->time_lo);#endif BYTESWAP	kbevent->W0= *EmKbdAd068K;	kbevent->W1= *EmKbdAd168K;	kbevent->W2= *EmKbdAd268K;	kbevent->W3= *EmKbdAd368K;	kbevent->W4= *EmKbdAd468K;	kbevent->W5= *EmKbdAd568K;	kbevent->WU= *EmRealUtilin68K;E 3I 3    if(r==w) /* event queqe FULL */      goto KBnext;E 3D 3	if(r==0) /* Queue was empty */		((RING*)CTopKeyevent)->read=w;	if(w >= MAXKEYEVENT)		((RING*)CTopKeyevent)->write = MINKEYEVENT;	else		((RING*)CTopKeyevent)->write = w + KEYEVENTSIZE;E 3I 3    kbevent=(KBEVENT*)(CTopKeyevent+ w);D 4    RCLK(kbevent->time);E 4I 4/*    RCLK(kbevent->time); */E 4    kbevent->W0= *EmKbdAd068K;    kbevent->W1= *EmKbdAd168K;    kbevent->W2= *EmKbdAd268K;    kbevent->W3= *EmKbdAd368K;    kbevent->W4= *EmKbdAd468K;    kbevent->W5= *EmKbdAd568K;    kbevent->WU= *EmRealUtilin68K;E 3I 3    if(r==0) /* Queue was empty */	((RING*)CTopKeyevent)->read=w;    if(w >= MAXKEYEVENT) ((RING*)CTopKeyevent)->write = MINKEYEVENT;    else ((RING*)CTopKeyevent)->write = w + KEYEVENTSIZE;E 3  KBnext:D 3	if(*KEYBUFFERING68k ==NIL)		*KEYBUFFERING68k=ATOM_T;E 3I 3    if(*KEYBUFFERING68k ==NIL) *KEYBUFFERING68k=ATOM_T;E 3D 3} E 3I 3  } E 3E 1