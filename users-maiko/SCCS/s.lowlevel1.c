h12608s 00000/00000/00159d D 2.9 90/04/20 01:49:31 sybalsky 14 13c AIX:  shortening file names, bulk change.es 00004/00003/00155d D 2.8 89/03/27 22:36:24 sybalsky 13 12c changed word-ptr refs to use GETWORDes 00015/00015/00143d D 2.7 89/01/09 17:18:27 shih 12 11c spellinges 00005/00008/00153d D 2.6 88/10/12 14:44:30 krivacic 11 10c new out-of-line interfacees 00016/00019/00145d D 2.5 88/10/10 14:09:29 krivacic 10 9c sun3/4 mergees 00001/00001/00163d D 2.4 88/09/27 16:46:20 shih 9 8c sp. correction nes 00000/00070/00164d D 2.3 88/09/27 15:26:34 shih 8 7c removed old C-only version of putbitsnfd()es 00046/00287/00188d D 2.2 88/05/25 17:48:41 krivacic 7 6c removed obsolete unreferenced fnses 00000/00000/00475d D 2.1 88/05/17 09:25:58 hayata 6 5c Version up to 2.1es 00001/00002/00474d D 1.5 88/04/18 12:38:42 krivacic 5 4c remove error address parm from N_OP_getbitsnfdes 00058/00000/00418d D 1.4 88/04/12 11:50:27 krivacic 4 3c putbasen & putbaseptrn for Native codees 00092/00000/00326d D 1.3 88/03/31 10:07:43 krivacic 3 2c N interfaceses 00002/00002/00324d D 1.2 88/03/13 14:54:25 hayata 2 1c Add SCCS key id (%Z%)es 00326/00000/00000d D 1.1 88/02/24 16:58:00 hayata 1 0c date and time created 88/02/24 16:58:00 by hayataeuUtTI 1D 2/* This is G-file %M% Version %I% (%G%). copyright Xerox & Fuji Xerox  */static char *id = "%M%	%I% %G%";E 2I 2D 12/* This is G-file %Z% %M% Version %I% (%G%). copyright Xerox & Fuji Xerox  */E 12I 12/* %Z% %M% Version %I% (%G%). copyright envos & Fuji Xerox  */E 12static char *id = "%Z% %M%	%I% %G%";E 2/* *	Copyright (C) 1987 by Fuji Xerox Co., Ltd. All rights reserved. *D 9 *	Auther	:	Hiroshi Hayata	E 9I 9D 10 *	Author	:	Hiroshi Hayata	E 10I 10D 12 *	Auther	:	Hiroshi Hayata	E 12I 12 *	Author	:	Hiroshi HayataE 12E 10E 9 */#include <stdio.h>#include "lispemul.h"#include "lispglobal.h"#include "address68k.h"#include "lispmap.h"I 13#include "lisptypes.h"E 13#include "emulglobal.h"I 3D 12int mask_array[16] = {	1, 3, 7, 0xf, E 12I 12int mask_array[16] = {	1, 3, 7, 0xf,E 12			0x1f, 0x3f, 0x7f, 0xff,			0x1ff, 0x3ff, 0x7ff, 0xfff,			0x1fff, 0x3fff, 0x7fff, 0xffff};E 3D 7/************************************************************ OP_hiloc	entry		HILOC		OPCODE[0322]E 7D 7	1. set high word of TopOfStack to Low word of TopOfStack.	2. set SMALLPL to high word of TopOfStack.	3. increment PC by 1.E 7I 7D 8#ifdef C_ONLYE 7E 8D 7***********************************************************/OP_hiloc(){#ifdef	TRACE	printPC();	printf("TRACE: OP_hiloc()\n");#endif	TopOfStack = (0xFFFF & (TopOfStack >> 16)) | S_POSITIVE;		++PC;}E 7/************************************************************D 7 OP_loloc	entry		LOLOC		OPCODE[0323]	1. set SMALLPL to high word of TopOfStack.	2. increment PC by 1.***********************************************************/OP_loloc(){#ifdef	TRACE	printPC();	printf("TRACE: OP_loloc()\n");#endif	TopOfStack = (0xFFFF & TopOfStack) | S_POSITIVE;	++PC;}/************************************************************ OP_getbasen	entry		GETBASE.N		OPCODE[0310]	1.	<<Enter>>		TopOfStack: base address.		alpha: word offset.	2.	fetch 16 bits word at (base address + word offset).	3.	increment PC by 2.	4.	<<Exit>>		TopOfStack: 	High Word - SMALLPL				Low Word  - fetched data***********************************************************/OP_getbasen(){DLword	alpha;	/* 1st argument of opcode, word offset */register DLword	*pslot;	/* address to fetch, 68 address */ #ifdef	TRACE	printPC();	printf("TRACE: OP_getbasen()\n");#endif/***	pslot = Addr68k_from_LADDR( ((0xFFFFFF & TopOfStack) + (0xFF & *(PC+1))) );	TopOfStack = S_POSITIVE | (0xFFFF & (*pslot));	PC += 2; ***/	alpha = 0xFF & *(++PC);	pslot = Addr68k_from_LADDR( ((0xFFFFFF & TopOfStack) + alpha) );	TopOfStack = S_POSITIVE | (0xFFFF & (*pslot));	++PC;}/************************************************************ OP_getbaseptrn	entry		GETBASEPTR.N		OPCODE[0311]	1.	<<Enter>>		TopOfStack: base address.		alpha: word offset.	2.	fetch 32 bits word at (base address + word offset).	3.	replace TopOfStack with the fetched data.	4.	increment PC by 2.	5.	<<Exit>>		TopOfStack: 	High Word	- fetched data					Low Word - fetched data***********************************************************/OP_getbaseptrn(){register LispPTR	*pslot;	/* address to fetch, 68 address */#ifdef	TRACE	printPC();	printf("TRACE: OP_getbaseptrn()\n");#endif	pslot = (LispPTR *)Addr68k_from_LADDR((0xFFFFFF & TopOfStack) + (0xFF & *(PC+1)));	TopOfStack = 0xFFFFFF & (*pslot); /* no effect to most hi-byte */	PC += 2;}/************************************************************ OP_getbitsnfd	entry		GETBITS.N.FD		OPCODE[0312]	1.	<<Enter>>		TopOfStack: base address.		alpha: 	word offset.		beta:	High nibble -- number of the first bit of the field			Low nibble  -- (number of the size of the fiel) - 1	2.	Data is @[TopOfStack + alpha]	3.	Shift and mask the data.	4.	<<Exit>>		TopOfStack: 	hi - S_POSITIVE				lo - mask & shifted data***********************************************************/OP_getbitsnfd(){register DLword	*pword;register short first;register short size;register short data;register short mask;#ifdef	TRACE	printPC();	printf("TRACE: OP_getbitsnfd()\n");#endif	pword = Addr68k_from_LADDR( (0xFFFFFF & (TopOfStack + ((*(PC+1)) & 0xFF))) );	size = 0xFF & *(PC+2);	first = 0xF & (size >> 4);	size &= 0xF;#ifdef	DEBUG	if(first+size > 15)		printf("DEBUG: OP_getbitsnfd[Bad beta! data-%x, first-%x, size-%x]\n", *pword, first, size);#endif	data= ( (*pword) >> (16 - (first + size + 1)) );		/* (16 - (first + size + 1)) is shift size. */		/* 16 is size of DLword. */	switch(size){		case 0:		mask = 1; break;		case 1:		mask = 3; break;		case 2:		mask = 7; break;		case 3:		mask = 0xF; break;		case 4:		mask = 0x1F; break;		case 5:		mask = 0x3F; break;		case 6:		mask = 0x7F; break;		case 7:		mask = 0xFF; break;		case 8:		mask = 0x1FF; break;		case 9:		mask = 0x3FF; break;		case 10:	mask = 0x7FF; break;		case 11:	mask = 0xFFF; break;		case 12:	mask = 0x1FFF; break;		case 13:	mask = 0x3FFF; break;		case 14:	mask = 0x7FFF; break;		case 15:	mask = 0xFFFF; break;	}	TopOfStack = S_POSITIVE | ( data & mask );	PC += 3;}/************************************************************ OP_putbasen	entry		PUTBASE.N		OPCODE[0315]	1.	<<Enter>>		TopOfStack: 	Low Word - replace data		*(CurrentStackPTR): base address.		alpha: offset.	2.	if high word of TopOFStack is not SMALLPL,		then call ufn2incS,		else replace (base address + offset) with data.	3.	increment PC by 2.	4.	<<Exit>>		TopOfStack:	base address.		 ***********************************************************/OP_putbasen(){register int	base;	/* base address. Lisp address */register int	data;	/* data to replace */register DLword	*pslot;	/* address to replace, 68 address */ #ifdef	TRACE	printPC();	printf("TRACE: OP_putbasen()\n");#endif	base = 0xFFFFFF & *((LispPTR *)(CurrentStackPTR));	data = GetLoWord( TopOfStack );	if( (0xFFFF0000 & TopOfStack ) != S_POSITIVE ){		error("OP_putbasen: Bad TopOfStack\n");	}else{		pslot = Addr68k_from_LADDR( ( base + (0xFF & (*(PC+1)))) );		*pslot = data;		TopOfStack = base;	}	CurrentStackPTR -= 2;	PC+=2;}/************************************************************ OP_putbaseptrn	entry		PUTBASEPTR.N		OPCODE[0316]	1.	<<Enter>>		TopOfStack: 	replace data (2 words)		*(CurrentStackPTR): base address.		alpha: offset.	2.	replace (base address + offset) with data.	3.	save base address to TopOfStack	3.	increment PC by 2.	4.	<<Exit>>		TopOfStack:	base address.		 ***********************************************************/OP_putbaseptrn(){register LispPTR	base;	/* base address. Lisp address */register DLword	*pslot;	/* address to replace, 68 address */ #ifdef	TRACE	printPC();	printf("TRACE: OP_putbaseptrn()\n");#endif	base = 0xFFFFFF & *((LispPTR *)(CurrentStackPTR));	pslot = Addr68k_from_LADDR( ( base + (0xFF & (*(PC+1)))) );	/* I think that ( base + alpha) is always even. */	*((LispPTR *)pslot) =(0xFFFFFF & TopOfStack); /* *((LispPTR *)pslot) = (0xFF000000 & *(LispPTR *)pslot) | (0xFFFFFF & TopOfStack); */	TopOfStack = base;	CurrentStackPTR -= 2;	PC+=2;}/************************************************************E 7D 8 OP_putbitsnfd	entry		PUTBITS.N.FD		OPCODE[0317]	1.	<<Enter>>		TopOfStack: NewValue		*((int *)(CurrentStackPTR)):	 base address.		alpha: 	word offset.		beta:		High nibble -- number of the first bit of the field				Low nibble  -- (number of the size of the fiel) - 1	2.	Data is @[TopOfStack + alpha]	3.	Shift and mask the data.	4.	Set the data at [TopOfStack + alpha].	5.	<<Exit>>		TopOfStack: 	??***********************************************************/OP_putbitsnfd(){int	base;DLword	*pword;register short first;register short size;register short fmask, fsmask;#ifdef	TRACE	printPC();	printf("TRACE: putbitsnfd()\n");#endif	base = 0xFFFFFF & *((LispPTR *)(CurrentStackPTR));	if( (0xFFFF0000 & TopOfStack ) != S_POSITIVE ){		error("OP_putbitsnfd\n");		/* goto UFN. */	}else{		pword = Addr68k_from_LADDR( base + (0xFF & (*(PC+1))) );				/* (*(++PC)) is alpha */		size = 0xFF & (*(PC+2));		first = 0xF & (size >> 4);	/* 0 <= fisrt <= 15 */		size &= 0xF;			/* 0 <= size <= 15 */#ifdef	DEBUG	if(first+size > 15)		printf("DEBUG: OP_getbitsnfd[Bad beta! data-%x, first-%x, size-%x]\n", *pword, first, size);#endif		fsmask = 0x8000;		fsmask = fsmask >> (first + size);		if(first == 0)			fmask = 0;		else{			fmask = 0x8000;			fmask = fmask >> (first - 1);		}		fmask ^= fsmask;		TopOfStack <<= (15 - first - size);		*pword = (TopOfStack & fmask) | ((*pword) & (~fmask));			/* Low word of TopOfStack -- new data (needs mask) */			/* (*pword) -- old data. (Other of replace field is avilable.) */	}	TopOfStack = base;	CurrentStackPTR -= 2;	PC+=3;I 3}D 7/************************************************************ N_OP_getbitsnfd	entry		GETBITS.N.FD		OPCODE[0312]E 7I 7#elseE 7D 7	1.	<<Enter>>		TopOfStack: base address.		alpha: 	word offset.		beta:	High nibble -- number of the first bit of the field			Low nibble  -- (number of the size of the fiel) - 1	2.	Data is @[TopOfStack + alpha]	3.	Shift and mask the data.	4.	<<Exit>>		return: 	hi - S_POSITIVE				lo - mask & shifted data***********************************************************/D 5LispPTR N_OP_getbitsnfd(base_addr, word_offset, beta, error_addr)E 5I 5LispPTR N_OP_getbitsnfd(base_addr, word_offset, beta)E 5	int base_addr;	register int word_offset;	int beta;D 5	int *error_addr;E 5{register DLword	*pword;register short first;register short size;#ifdef	TRACE	printf("TRACE: N_OP_getbitsnfd()\n");#endif	pword = Addr68k_from_LADDR( 0xFFFFFF & (base_addr + word_offset) );	size = 0xF & beta;	first = 0xF & (beta >> 4);#ifdef	DEBUG	if(first+size > 15)		printf("DEBUG: N_OP_getbitsnfd[Bad beta! data-%x, first-%x, size-%x]\n", *pword, first, size);#endif	return(S_POSITIVE | 		( ( (*pword) >> (16 - (first + size + 1)) ) 			& mask_array[size] ) ) ;}E 7/************************************************************E 8 N_OP_putbitsnfd	entry		PUTBITS.N.FD		OPCODE[0317]	1.	<<Enter>>		TopOfStack: NewValue		*((int *)(CurrentStackPTR)):	 base address.D 12		alpha: 	word offset.E 12I 12		alpha:	word offset.E 12		beta:		High nibble -- number of the first bit of the field				Low nibble  -- (number of the size of the fiel) - 1	2.	Data is @[TopOfStack + alpha]	3.	Shift and mask the data.	4.	Set the data at [TopOfStack + alpha].	5.	<<Exit>>D 12		return: 	??E 12I 12		return:	??E 12***********************************************************/D 10LispPTR N_OP_putbitsnfd(base_addr, data, word_offset, beta, error_addr)	int base_addr;	int word_offset;	register int beta;	int *error_addr;E 10I 10D 11LispPTR N_OP_putbitsnfd(base, data, word_offset, beta, error_addr)E 11I 11LispPTR N_OP_putbitsnfd(base, data, word_offset, beta)E 11register LispPTR base, data;	 int word_offset;register int beta;D 11	 int *error_addr;E 11E 10{D 10register int	base;E 10register DLword	*pword;register int shift_size, field_size, fmask;D 10#ifdef	TRACE	printf("TRACE: N_putbitsnfd()\n");E 10I 10#ifdef CHECK	if (base > 0xFFFFFF) {error("getbits: base out of range");}	if (beta > 0xFF) {error("bad beta argument to PUTBITS");}E 10#endifD 11	if( (0xFFFF0000 & data ) != S_POSITIVE ) { ERROR_EXIT(error_addr); };E 11I 11	if( (0xFFFF0000 & data ) != S_POSITIVE ) { ERROR_EXIT(data); };E 11D 12	E 12I 12E 12D 10	base = 0xFFFFFF & base_addr;E 10	pword = Addr68k_from_LADDR( base + word_offset );	field_size = 0xF & beta;D 10	shift_size = 15 - (0xF & (beta >> 4)) - field_size;E 10I 10	shift_size = 15 - (beta >> 4) - field_size;E 10	fmask = mask_array[field_size] << shift_size;D 13	*pword = ( (data << shift_size) & fmask)  |  ((*pword) & (~fmask));E 13I 13	GETWORD(pword) = ( (data << shift_size) & fmask)  |  (GETWORD(pword) & (~fmask));E 13D 12	E 12I 12E 12	return(base);I 7}D 8#endifE 8/************************************************************ N_OP_getbitsnfd	entry		GETBITS.N.FD		OPCODE[0312]	1.	<<Enter>>		TopOfStack: base address.D 12		alpha: 	word offset.E 12I 12		alpha:	word offset.E 12		beta:	High nibble -- number of the first bit of the field			Low nibble  -- (number of the size of the fiel) - 1	2.	Data is @[TopOfStack + alpha]	3.	Shift and mask the data.	4.	<<Exit>>D 12		return: 	hi - S_POSITIVEE 12I 12		return:		hi - S_POSITIVEE 12				lo - mask & shifted data***********************************************************/LispPTR N_OP_getbitsnfd(base_addr, word_offset, beta)	int base_addr;	register int word_offset;	int beta;{register DLword	*pword;register short first;register short size;D 10#ifdef	TRACE	printf("TRACE: N_OP_getbitsnfd()\n");#endifE 10D 10	pword = Addr68k_from_LADDR( 0xFFFFFF & (base_addr + word_offset) );E 10I 10D 12	E 12I 12E 12	pword = Addr68k_from_LADDR( base_addr + word_offset );E 10	size = 0xF & beta;D 10	first = 0xF & (beta >> 4);E 10I 10	first = beta >> 4;E 10D 10#ifdef	DEBUG	if(first+size > 15)		printf("DEBUG: N_OP_getbitsnfd[Bad beta! data-%x, first-%x, size-%x]\n", *pword, first, size);E 10I 10#ifdef CHECK	if (base_addr > 0xFFFFFF) {error("getbits: base out of range");}	if (first+size > 15) {error("getbits beta too big");}E 10#endifD 12	return(S_POSITIVE | 		( ( (*pword) >> (16 - (first + size + 1)) ) E 12I 12	return(S_POSITIVE |D 13		( ( (*pword) >> (16 - (first + size + 1)) )E 13I 13		( ( GETWORD(pword) >> (16 - (first + size + 1)) )E 13E 12			& mask_array[size] ) ) ;E 7E 3}I 4/************************************************************ N_OP_putbasen	entry		PUTBASE.N		OPCODE[0315]	1.	<<Enter>>D 12		TopOfStack: 	Low Word - replace dataE 12I 12		TopOfStack:	Low Word - replace dataE 12		*(CurrentStackPTR): base address.		alpha: offset.	2.	if high word of TopOFStack is not SMALLPL,		then call ufn2incS,		else replace (base address + offset) with data.	3.	increment PC by 2.	4.	<<Exit>>		return:	base address.		(Called only by Native code)***********************************************************/D 11LispPTR N_OP_putbasen(base, tos, n, error_addr)E 11I 11LispPTR N_OP_putbasen(base, tos, n)E 11	register LispPTR base, tos;	int n;D 11	int *error_addr;E 11{	base = 0xFFFFFF & base;D 11	if( (0xFFFF0000 & tos ) != S_POSITIVE ) {ERROR_EXIT(error_addr);}E 11I 11	if( (0xFFFF0000 & tos ) != S_POSITIVE ) {ERROR_EXIT(tos);}E 11	else{D 13		*((DLword *) Addr68k_from_LADDR( base + n )) = GetLoWord(tos);E 13I 13		GETWORD((DLword *) Addr68k_from_LADDR( base + n )) = GetLoWord(tos);E 13		return(base);	}}/************************************************************ N_OP_putbaseptrn	entry		PUTBASEPTR.N		OPCODE[0316]	1.	<<Enter>>D 12		TopOfStack: 	replace data (2 words)E 12I 12		TopOfStack:	replace data (2 words)E 12		*(CurrentStackPTR): base address.		alpha: offset.	2.	replace (base address + offset) with data.	3.	save base address to TopOfStack	3.	increment PC by 2.	4.	<<Exit>>		return:	base address.D 12		(Called only by Native code) E 12I 12		(Called only by Native code)E 12***********************************************************/D 11LispPTR N_OP_putbaseptrn(base, tos, n, error_addr)E 11I 11LispPTR N_OP_putbaseptrn(base, tos, n)E 11	register LispPTR base, tos;	int n;D 11	int *error_addr;E 11{	base = 0xFFFFFF & base;	*((LispPTR *) Addr68k_from_LADDR( base + n )) = tos & 0xffffff;	return(base);}E 4E 1