h30456s 00060/00001/00174d D 2.8 93/02/08 15:03:36 sybalsky 11 10c Big VM (and new CDR coding) changes for 3.0 \nes 00011/00031/00164d D 2.7 92/11/25 20:17:00 sybalsky 10 9c Fixing extra text in endifs => comment.\nes 00002/00002/00193d D 2.6 92/04/21 16:21:54 sybalsky 9 8c shortening file names for DOSes 00012/00001/00183d D 2.5 90/06/04 10:03:00 sybalsky 8 7c reformat, copyrightes 00000/00001/00184d D 2.4 89/03/30 13:02:50 sybalsky 7 6c Fixed abdum defn to be 1 word in both versions, and made right changes elsewhere for it to scan right.es 00015/00004/00170d D 2.3 89/03/27 22:50:37 sybalsky 6 5c Added byte-swqpped defn for struct 'abdum'.es 00047/00000/00127d D 2.2 89/03/16 12:02:52 sybalsky 5 4c Added byte-swapped definitions.es 00000/00000/00127d D 2.1 88/05/17 09:17:03 hayata 4 3c Version up to 2.1es 00020/00003/00107d D 1.3 88/04/29 01:03:23 masinter 3 2c merge tomtom's changes; add arrayheader structurees 00001/00001/00109d D 1.2 88/03/13 13:16:39 hayata 2 1c Add SCCS key id (%Z%)es 00110/00000/00000d D 1.1 88/02/23 18:35:19 hayata 1 0c date and time created 88/02/23 18:35:19 by hayataeuUtTI 1D 2/* This is G-file %M% Version %I% (%G%). copyright Xerox & Fuji Xerox  */E 2I 2D 6/* This is G-file %Z% %M% Version %I% (%G%). copyright Xerox & Fuji Xerox  */E 6I 6D 8/* This is G-file @(#) array.h Version 2.2 (3/16/89). copyright Xerox & Fuji Xerox  */E 8I 8D 10/* This is G-file @(#) array.h Version 2.2 (3/16/89). copyright Venue & Fuji Xerox  */E 8E 6E 2D 3E 3/****************************************************************************//****************************************************************************//*	Copyright (C) 1987 by Fuji Xerox Co., Ltd. All rights reserved.	    *//****************************************************************************//*                                                                          *//*                    File name : array.h                                   *//*                                                                          *//****************************************************************************//*                                                                          */D 3/*                    Creation Date : Oct-5-1987                            */E 3I 3/*                    Creation Date : Oct-5,1987                            */E 3/*                    Written by Tomoru Teruuchi                            */D 3/*                                                                          */E 3I 3/*                            Modified : April-22,1988                      */E 3/****************************************************************************//*                                                                          *//*         Herein, Entries : sequencedescriptor                             *//*                           arrayp                                         *//*                           arrayblock                                     */I 3/*                           arrayheader (added structure)                  */E 3/*                                                                          *//*                                                                          *//*                                                                          *//*                                                                          *//****************************************************************************//*                                                                    Tomtom*//****************************************************************************/E 10I 10/* %Z% %M% Version %I% (%G%). copyright Venue  */E 10I 8I 10E 10/************************************************************************//*									*/D 10/*	Copyright 1989, 1990 Venue, Fuji Xerox Co., Ltd, Xerox Corp.	*/E 10I 10/*	(C) Copyright 1989-92 Venue. All Rights Reserved.		*//*	Manufactured in the United States of America.			*/E 10/*									*/D 10/*	This file is work-product resulting from the Xerox/Venue	*//*	Agreement dated 18-August-1989 for support of Medley.		*/E 10I 10/*	The contents of this file are proprietary information 		*//*	belonging to Venue, and are provided to you under license.	*//*	They may not be further distributed or disclosed to third	*//*	parties without the specific permission of Venue.		*/E 10/*									*//************************************************************************/E 8D 10E 10I 5#ifndef BYTESWAP	/********************************/	/*  Normal byte-order version   */	/********************************/I 11#ifdef BIGVME 11E 5typedef struct sequencedescriptor {	unsigned	orig		:1;	unsigned	nil		:1;	unsigned	readonly	:1;	unsigned 	nil2		:1;I 11	unsigned	base		:28;E 11	unsigned	typ		:4;I 11	unsigned		length: 28;	LispPTR		offst;} Arrayp;struct arrayheader {        unsigned        nil             :4;	unsigned        base            :28;	unsigned        readonlyp       :1;	unsigned        indirectp       :1;	unsigned        bitp            :1;	unsigned        stringp         :1;	unsigned        adjustablep     :1;	unsigned        displacedp      :1;	unsigned        fillpointerp    :1;	unsigned        extendablep     :1;	unsigned        typenumber      :8;	DLword          offset;	LispPTR          fillpointer;	LispPTR          totalsize;      };#elsetypedef struct sequencedescriptor {	unsigned	orig		:1;	unsigned	nil		:1;	unsigned	readonly	:1;	unsigned 	nil2		:1;	unsigned	typ		:4;E 11	unsigned	base		:24;	DLword		length;	DLword		offst;} Arrayp;I 3struct arrayheader {        unsigned        nil             :8;	unsigned        base            :24;	unsigned        readonlyp       :1;	unsigned        indirectp       :1;	unsigned        bitp            :1;	unsigned        stringp         :1;	unsigned        adjustablep     :1;	unsigned        displacedp      :1;	unsigned        fillpointerp    :1;	unsigned        extendablep     :1;	unsigned        typenumber      :8;	DLword          offset;	DLword          fillpointer;	DLword          totalsize;      };I 11#endif /* BIGVM */E 11E 3struct arrayblock {	unsigned	password	:13;	unsigned	gctype		:2;	unsigned	inuse		:1;	DLword		arlen;	LispPTR		fwd;	LispPTR		bkwd;};I 6struct abdum {	DLword		abflags;};E 6I 5#else	/********************************/	/*  Byte-swapped version, for   */	/*  e.g., 80386's		*/	/********************************/I 11#ifdef BIGVME 11typedef struct sequencedescriptor {I 11	unsigned	base		:28;	unsigned 	nil2		:1;	unsigned	readonly	:1;	unsigned	nil		:1;	unsigned	orig		:1;	unsigned		length: 28;	unsigned	typ		:4;	LispPTR		offst;} Arrayp;struct arrayheader {	unsigned        base            :28;        unsigned        nil             :4;	DLword          offset;	unsigned        typenumber      :8;	unsigned        extendablep     :1;	unsigned        fillpointerp    :1;	unsigned        displacedp      :1;	unsigned        adjustablep     :1;	unsigned        stringp         :1;	unsigned        bitp            :1;	unsigned        indirectp       :1;	unsigned        readonlyp       :1;	LispPTR          totalsize;	LispPTR          fillpointer;      };#elsetypedef struct sequencedescriptor {E 11	unsigned	base		:24;	unsigned	typ		:4;	unsigned 	nil2		:1;	unsigned	readonly	:1;	unsigned	nil		:1;	unsigned	orig		:1;	DLword		offst;	DLword		length;} Arrayp;struct arrayheader {	unsigned        base            :24;        unsigned        nil             :8;	DLword          offset;	unsigned        typenumber      :8;	unsigned        extendablep     :1;	unsigned        fillpointerp    :1;	unsigned        displacedp      :1;	unsigned        adjustablep     :1;	unsigned        stringp         :1;	unsigned        bitp            :1;	unsigned        indirectp       :1;	unsigned        readonlyp       :1;	DLword          totalsize;	DLword          fillpointer;      };D 11E 11I 11#endif /* BIGVM */E 11struct arrayblock {	DLword		arlen;	unsigned	inuse		:1;	unsigned	gctype		:2;	unsigned	password	:13;	LispPTR		fwd;	LispPTR		bkwd;};D 6#endif BYTESWAPE 5struct abdum {E 6I 6struct abdum  {D 7	DLword		nil;E 7E 6	DLword		abflags;};I 6D 10#endif BYTESWAPE 10I 10#endif /* BYTESWAP */E 10E 6/****************************************************************************//*                                                                          *//*        The following data are the constant values for array itself.      *//*                                                                          *//****************************************************************************/D 9/* #define ST_BYTE			0 in lisptypes.h *//* #define ST_POS16			1 in lisptypes.h */E 9I 9/* #define ST_BYTE			0 in lsptypes.h *//* #define ST_POS16			1 in lsptypes.h */E 9#define ST_INT32		2/*#define ST_CODE			4 in load.h      */#define ST_PTR			6#define ST_FLOAT		7#define ST_BIT			8#define ST_PTR2			11/****************************************************************************//*                                                                          *//*        The following data are the constant values for array reclaimer.   *//*                                                                          *//****************************************************************************/#define MAXARRAYBLOCKSIZE	65535#define MAXARRAYLEN		65535#define ARRAYBLOCKHEADERCELLS	1#define ARRAYBLOCKTRAILERCELLS	1#define ARRAYBLOCKOVERHEADCELLS	(ARRAYBLOCKHEADERCELLS+ARRAYBLOCKTRAILERCELLS)#define MAXARRAYNCELLS		(MAXARRAYBLOCKSIZE-ARRAYBLOCKOVERHEADCELLS)#define ARRAYBLOCKHEADERWORDS	2#define ARRAYBLOCKTRAILERWORDS	2#define ARRAYBLOCKOVERHEADWORDS	(ARRAYBLOCKHEADERWORDS+ARRAYBLOCKTRAILERWORDS)#define ARRAYBLOCKLINKINGCELLS	2#define MINARRAYBLOCKSIZE	(ARRAYBLOCKOVERHEADCELLS+ARRAYBLOCKLINKINGCELLS)#define MAXBUCKETINDEX		30#define UNBOXEDBLOCK_GCT	0#define PTRBLOCK_GCT		1#define CODEBLOCK_GCT		2#define ABPASSWORDSHIFT		3#define ARRAYBLOCKPASSWORD	(43690 >> ABPASSWORDSHIFT)				/* = 1010101010101010 >> 3 = 5461 */#define FREEARRAYFLAGWORD	((ARRAYBLOCKPASSWORD << ABPASSWORDSHIFT) | (UNBOXEDBLOCK_GCT << 1))				/* = 43688 */#define USEDARRAYFLAGWORD	((ARRAYBLOCKPASSWORD << ABPASSWORDSHIFT) | 1)#define CODEARRAYFLAGWORD	((ARRAYBLOCKPASSWORD << ABPASSWORDSHIFT) | ((CODEBLOCK_GCT << 1) | 1))#define FIRSTARRAYSEGMENT	19#define MAXCELLSPERHUNK		64/****************** The following are for codereclaimer *********************/#define BITSPERBITE		8/********************* End of codereclaimer *********************************//****************************************************************************//*                                                                          *//*                  End of Definitions                                      *//*                                                                          *//****************************************************************************/E 1