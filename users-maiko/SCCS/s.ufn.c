h13223s 00006/00001/00221d D 2.8 93/02/08 15:00:04 sybalsky 15 14c Big VM (and new CDR coding) changes for 3.0 \nes 00002/00002/00220d D 2.7 92/06/26 13:50:56 sybalsky 14 13c retrofit of 386 unix changes \nes 00005/00005/00217d D 2.6 92/04/21 17:27:22 sybalsky 13 12c shortening file names for DOS \nes 00012/00001/00210d D 2.5 90/06/04 18:10:02 sybalsky 12 11c reformat, copyrightes 00000/00000/00211d D 2.4 90/04/20 02:00:14 sybalsky 11 10c AIX:  shortening file names, bulk change.es 00007/00007/00204d D 2.3 89/03/27 22:35:34 sybalsky 10 9c word-pointers to use GETWORD.es 00004/00097/00207d D 2.2 88/06/03 09:54:23 masinter 9 8c Put back ufn (used by C_ONLY)es 00000/00000/00304d D 2.1 88/05/17 09:27:20 hayata 8 7c Version up to 2.1es 00003/00000/00301d D 1.7 88/04/22 10:39:33 krivacic 7 6c add clfmemb & classoces 00002/00000/00299d D 1.6 88/04/13 12:33:06 shimizu 6 5c Add TRADSTACKOVERes 00003/00003/00296d D 1.5 88/04/07 15:57:06 masinter 5 4c Make Ni always call UFN rather than signal errores 00001/00002/00298d D 1.4 88/03/31 10:59:49 shimizu 4 3c replace FnStkOvr with DOSTACKOVERFLOWes 00002/00002/00298d D 1.3 88/03/13 15:08:55 hayata 3 2c Add SCCS key id (%Z%)es 00014/00011/00286d D 1.2 88/03/03 11:39:24 krivacic 2 1c Removed global CurrentFX to use CURRENTFX = PVAR - FRAMESIZEes 00297/00000/00000d D 1.1 88/02/24 16:58:51 hayata 1 0c date and time created 88/02/24 16:58:51 by hayataeuUtTI 1D 3/* This is G-file %M% Version %I% (%G%). copyright Xerox & Fuji Xerox  */static char *id = "%M%	%I% %G%";E 3I 3D 12/* This is G-file %Z% %M% Version %I% (%G%). copyright Xerox & Fuji Xerox  */E 12I 12/* This is G-file %Z% %M% Version %I% (%G%). copyright Venue & Fuji Xerox  */E 12static char *id = "%Z% %M%	%I% %G%";E 3/* *	Copyright (C) 1987 by Fuji Xerox Co., Ltd. All rights reserved. *D 9 *	Auther :  Takeshi ShimizuE 9I 9 *	Author :  Takeshi ShimizuE 9D 9 MERGED AT AISE 9 * *//******************************************************************//*		File Name  :	ufn.c(for use with SYSOUT)		Including  :	ufnD 9				init_ufn		dealing OP : 	checkapplyE 9		Created    :	jun 8, 1987 by T.Shimizu*//******************************************************************/I 12/************************************************************************//*									*//*	Copyright 1989, 1990 Venue, Fuji Xerox Co., Ltd, Xerox Corp.	*//*									*//*	This file is work-product resulting from the Xerox/Venue	*//*	Agreement dated 18-August-1989 for support of Medley.		*//*									*//************************************************************************/E 12#include "lispemul.h"#include "address.h"D 13#include "address68k.h"#include "lisptypes.h"E 13I 13#include "adr68k.h"#include "lsptypes.h"E 13#include "lispmap.h"#include "stack.h"D 13#include "emulglobal.h"#include "lispglobal.h"#include "initatoms.h"E 13I 13#include "emlglob.h"#include "lspglob.h"#include "initatms.h"E 13#include "cell.h"I 9#ifdef C_ONLYE 9#define GetUFNEntry(num)	(((UFN *)UFNTable) + (num))/******************************************************************//*		Func Name  :	ufn		Desc  :		call Lisp UFN func				TopOfStack;				PC ;		Created    :	May 29, 1987 by T.Shimizu		Changed	   :	Jun 17 1987 take				Aug 25 1987 take				Sep  4  1987 NMitani				Oct 07 1987 take				Nov 13 1987 take +8 & Simple STKCHK				Nov 25 1987 take(tmp_fn)*//******************************************************************/ufn(bytecode)DLword bytecode ;{  register DefCell *defcell68k ;  /* Definition Cell PTR */  register int pv_num ; /* scratch for pv */  register UFN *entry68k;  register struct fnhead *tmp_fn;  register unsigned int arg_num ;    /* Num of args */  register int rest ;  /* use for arignments */#ifdef TRACE	printPC();	print_atomname("TRACE : ufn() 0%o ",bytecode);#endif  PushCStack;  entry68k = (UFN *)GetUFNEntry(bytecode); /*changing I/F 17-jun*/#ifdef TRACE	if(entry68k->atom_name)	  printf("Atom number: %d\n", 0xffff && entry68k->atom_name);	else	  error("UNF not specified");#endif  switch(entry68k->byte_num)   {	case 0 :	break;	case 1 :	/*PushStack(SPOS_HI  | Get_BYTE(PC+1));*/			CurrentStackPTR +=2;D 10			*CurrentStackPTR=SPOS_HI;			*(CurrentStackPTR+1)=(DLword)Get_BYTE(PC+1);E 10I 10			GETWORD(CurrentStackPTR)=SPOS_HI;D 14			GETWORD(CurrentStackPTR+1)=(DLword)Get_BYTE(PC+1);E 14I 14			GETWORD(CurrentStackPTR+1)=(DLword)Get_code_BYTE(PC+1);E 14E 10	/* I think we don't have to shift alpha byte eight bit before save it. */#ifdef DEBUG		printf("***ufn: case 1\n");#endif		break;	case 2 :	/*PushStack(S_POSITIVE |Get_DLword(PC+1));*/			CurrentStackPTR +=2;D 10			*(CurrentStackPTR) = SPOS_HI ;			*(CurrentStackPTR+1) = (DLword)((Get_BYTE(PC+1) << 8) | Get_BYTE(PC+2));E 10I 10			GETWORD(CurrentStackPTR) = SPOS_HI ;D 14			GETWORD(CurrentStackPTR+1) = (DLword)((Get_BYTE(PC+1) << 8) | Get_BYTE(PC+2));E 14I 14			GETWORD(CurrentStackPTR+1) = (DLword)((Get_code_BYTE(PC+1) << 8) | Get_code_BYTE(PC+2));E 14E 10#ifdef DEBUG		printf("***ufn: case 2\n");#endif		break;	default : error("ufn : Bad UFN MP 9351 ");		break;   }  /* Get Next Block offset form OPCODE byte */D 2  CurrentFX->nextblock = (LADDR_from_68k(CurrentStackPTR) & 0x0ffff)E 2I 2  CURRENTFX->nextblock = (LADDR_from_68k(CurrentStackPTR) & 0x0ffff)E 2					- (entry68k->arg_num<<1) +2 /** +1 **/ ;  /* Setup IVar */D 2  IVar = Addr68k_from_LADDR((((LispPTR)(CurrentFX->nextblock)) | STK_OFFSET)) ;E 2I 2  IVar = Addr68k_from_LADDR((((LispPTR)(CURRENTFX->nextblock)) | STK_OFFSET)) ;E 2#ifdef LISPTRACE	print(entry68k->atom_name);printf(": ");	{int cnt;		for(cnt=0;cnt<arg_num;cnt++){			printf(" IVAR%d :",cnt);			print(*((LispPTR *)(IVar + (cnt*2))));		}	}	printf("\n");#endif  /* Set PC to the Next Instruction and save into FX */D 2  CurrentFX->pc = ((unsigned int)PC - (unsigned int)FuncObj) + entry68k->byte_num +1;  E 2I 2  CURRENTFX->pc = ((unsigned int)PC - (unsigned int)FuncObj) + entry68k->byte_num +1;  E 2  defcell68k =(DefCell *) GetDEFCELL68k(entry68k->atom_name) ;  if( defcell68k->ccodep == 0 )   {	   /* This LispFunc is NOT compiled object . We must use Interpreter*/	printf("UFN: UFN func isn't compiled OBJ \n");	defcell68k = (DefCell *)GetDEFCELL68k(ATOM_INTERPRETER);	PushStack(TopOfStack);  /* Move AtomIndex to CSTK */   }   /* Nov 25 87 take */  tmp_fn = (struct fnhead *)Addr68k_from_LADDR( defcell68k->defpointer ) ;/* stack overflow check STK_SAFE is redundant?? */ if ( (unsigned int)(CurrentStackPTR + tmp_fn->stkmin +STK_SAFE) >=  (unsigned int)StkLimO )  {	/**printf("#### STKOVER in UFN case\n");**/I 6D 9#ifdef TRADSTACKOVERE 6	TopOfStack = entry68k->atom_name;I 6#endifE 9I 9E 9E 6D 4	FnStkOvr(entry68k->arg_num,entry68k->byte_num); /* After STKOVR, retry current OPCODE */	return;E 4I 4	DOSTACKOVERFLOW(entry68k->arg_num,entry68k->byte_num); /* After STKOVR, retry current OPCODE */E 4 }  FuncObj =tmp_fn ; if( FuncObj->na >= 0 )  {	/* This Function is Spread Type */	/* Arguments on Stack Adjustment  */	rest = entry68k->arg_num - FuncObj->na ;		while(rest <0) { 	  PushStack(NIL_PTR) ;	  rest++;	}	CurrentStackPTR -= (rest<<1) ;  } /* if end */ /* Set up BF */ CurrentStackPTR +=2;D 10 *(CurrentStackPTR) = BF_MARK ;D 2 *(CurrentStackPTR+1) = CurrentFX->nextblock ;E 2I 2 *(CurrentStackPTR+1) = CURRENTFX->nextblock ;E 10I 10 GETWORD(CurrentStackPTR) = BF_MARK ; GETWORD(CurrentStackPTR+1) = CURRENTFX->nextblock ;E 10E 2 CurrentStackPTR +=2;D 2/*  *(++CurrentStackPTR) = CurrentFX->nextblock ; */E 2I 2/*  *(++CurrentStackPTR) = CURRENTFX->nextblock ; */E 2 /* Set up FX */D 10 *(CurrentStackPTR) = FX_MARK;E 10I 10 GETWORD(CurrentStackPTR) = FX_MARK;E 10D 2 CurrentFX = (struct frameex1 *)CurrentStackPTR ; CurrentFX->alink = LADDR_from_68k(PVar); CurrentFX->lofnheader = (defcell68k->defpointer) & 0x0ffff ; CurrentFX->hi2fnheader = ((defcell68k->defpointer) & 0x0ff0000) >> 16 ;E 2D 2 /*  Set PVar pointer */ PVar = CurrentStackPTR = (DLword *)CurrentFX + FRAMESIZE ;E 2I 2 ((struct frameex1 *)CurrentStackPTR)->alink = LADDR_from_68k(PVar); PVar = (DLword *)CurrentStackPTR + FRAMESIZE ;I 15#ifdef BIGVM ((struct frameex1 *)CurrentStackPTR)->fnheader = 	(defcell68k->defpointer) ;#elseE 15 ((struct frameex1 *)CurrentStackPTR)->lofnheader = 	(defcell68k->defpointer) & 0x0ffff ; ((struct frameex1 *)CurrentStackPTR)->hi2fnheader = D 15	((defcell68k->defpointer) & 0x0ff0000) >> 16 ;E 15I 15	((defcell68k->defpointer) & SEGMASK) >> 16 ;#endif /* BIGVM */E 15 CurrentStackPTR = PVar ;E 2 /* Set up PVar area */ pv_num = FuncObj->pv +1 ;  while(pv_num > 0) {	*((LispPTR *)CurrentStackPTR) = 0x0ffff0000 ;	CurrentStackPTR += DLWORDSPER_CELL ;	*((LispPTR *)CurrentStackPTR) = 0x0ffff0000 ;	CurrentStackPTR += DLWORDSPER_CELL ; 	pv_num--;  }/*  CurrentStackPTR ++ ;*/ /* Set PC points New Function's first OPCODE */ PC =(ByteCode *) ((unsigned int)FuncObj + FuncObj->startpc) ;/** TopOfStack = (FuncObj->startpc -1) >> 1;**/} /* end ufn */D 9OP_term(){ error(" OP:TERM , Bad!!");}E 9I 9#endifE 9D 9Ni(){ /* for vacant entries */ extern ByteCode *PC ;D 5  printf("Invalid OPCODE >> 0x%x \n",Get_BYTE(PC));  error("I give up . bye!! ");E 5I 5  ufn(Get_BYTE(PC));  /* printf("Invalid OPCODE >> 0x%x \n",Get_BYTE(PC));  error("I give up . bye!! "); */E 5 }/**********************************************************//*	  The Following OPCODEs are executed by UFN    */  I 7OP_clfmemb() {  ufn(033); }OP_classoc() {  ufn(035); }E 7OP_restlist(){		/*Hayata: Nov 30 1987 */		ufn(043);}OP_ubfloat3(){		ufn(062);}OP_equal(){		ufn(0364);}OP_misc3(){		ufn(0372);}OP_nop(){ ++PC;}OP_aref1(){		ufn(0266);}OP_aset1(){		ufn(0267);}OP_fplus(){		ufn(0350);}OP_fdifference(){		ufn(0351);}OP_ftimes(){		ufn(0352);}OP_fquotient(){		ufn(0353);}OP_ubfloat2(){		ufn(0354);}OP_ubfloat1(){		ufn(0355);}OP_aref2(){		ufn(0356);}OP_aset2(){		ufn(0357);}OP_fgreaterp(){		ufn(0362);}OP_misc4(){		ufn(0373);}OP_eqq(){		ufn(0377);}OP_dbx(){ TopOfStack = NIL_PTR; ++PC;}OP_drawline(){	ufn(073);}E 9E 1