h02215s 00002/00000/00071d D 2.19 90/12/07 20:35:40 sybalsky 29 28c Add optional stack check on each return.es 00002/00002/00069d D 2.18 90/09/18 23:51:35 sybalsky 28 27c Retrofit changes from AIX, DEC3100, PS/2es 00004/00003/00067d D 2.17 90/08/30 10:34:21 sybalsky 27 26c AIX changeses 00015/00001/00055d D 2.16 90/06/04 14:48:17 sybalsky 26 25c reformat, copyrightes 00001/00001/00055d D 2.15 89/03/24 13:25:44 sybalsky 25 24c Changed one word-ptr ref to a GETWORD.es 00001/00001/00055d D 2.14 88/12/01 13:49:07 krivacic 24 23c Add SUN3_OS4_IL & SUN3_OS3_OR_OS4_IL Flags, and move asm labels to inlines.es 00024/00024/00032d D 2.13 88/11/30 19:06:03 shih 23 22c restored sccs stamp, minor cleanupes 00002/00002/00054d D 2.12 88/11/22 13:18:11 shimizu 22 21c AR10951:slowreturn can notice stackoverflowes 00005/00005/00051d D 2.11 88/10/13 19:30:16 krivacic 21 20c ifdef cleanuupes 00003/00003/00053d D 2.10 88/10/12 17:09:28 krivacic 20 19c put SaveD6 in ifdefes 00012/00003/00044d D 2.9 88/10/05 17:03:28 krivacic 19 18c sun 3 / 4 mergees 00009/00001/00038d D 2.8 88/09/21 11:43:19 krivacic 18 17c remove error prone asm code for slow returnes 00005/00004/00034d D 2.7 88/09/16 14:39:37 bane 17 16c Adjusted PC changeses 00017/00003/00021d D 2.6 88/08/15 14:26:58 krivacic 16 15c asm return call & speedup of c codees 00001/00001/00023d D 2.5 88/07/19 16:43:00 krivacic 15 14c fix native returnes 00002/00002/00022d D 2.4 88/06/03 12:22:25 masinter 14 13c reduce diffs between stable and working; put back STK_END_COMPUTEes 00004/00031/00020d D 2.3 88/05/31 20:37:09 masinter 13 12c eliminate StkLimO, NO_C_HARDRET case, tune return somees 00007/00017/00044d D 2.2 88/05/27 11:26:14 krivacic 12 11c check return to native in all return caseses 00000/00000/00061d D 2.1 88/05/17 10:07:35 hayata 11 10c Version up to 2.1es 00030/00002/00031d D 1.10 88/05/11 17:55:01 shimizu 10 9c Add slowreturnes 00006/00001/00027d D 1.9 88/05/10 19:41:41 krivacic 9 8c irq checkses 00001/00001/00027d D 1.8 88/04/22 19:21:55 krivacic 8 7c inlinedefs re-arrangement & ni removales 00006/00000/00022d D 1.7 88/04/11 09:54:15 krivacic 7 6c Ret to Native checkes 00001/00000/00021d D 1.6 88/03/13 14:20:07 hayata 6 5c Change SCCS key ides 00000/00000/00021d D 1.5 88/03/13 13:28:48 hayata 5 4c Add SCCS key id (%Z%)es 00010/00008/00011d D 1.4 88/03/09 15:44:42 krivacic 4 3c compress fn return codees 00006/00007/00013d D 1.3 88/03/08 18:39:08 krivacic 3 2c Register, re-ordiering, swap, Optimizationses 00013/00014/00007d D 1.2 88/03/03 11:43:39 krivacic 2 1c Removed global CurrentFX to use CURRENTFX = PVAR - FRAMESIZEes 00021/00000/00000d D 1.1 88/02/23 18:37:28 hayata 1 0c date and time created 88/02/23 18:37:28 by hayataeuUtTI 6D 17/* This is G-file %Z% %M% Version %I% (%G%). copyright Xerox & Fuji Xerox  */E 17I 17D 23/* This is G-file @(#) tosretmacro.h Version 2.6 (8/15/88). copyright Xerox & Fuji Xerox  */E 23I 23D 26/* %Z% %M% Version %I% (%G%). copyright envos & Fuji Xerox  */E 26I 26/* %Z% %M% Version %I% (%G%). copyright Venue & Fuji Xerox  *//************************************************************************//*									*//*	Copyright 1989, 1990 Venue, Fuji Xerox Co., Ltd, Xerox Corp.	*//*									*//*	This file is work-product resulting from the Xerox/Venue	*//*	Agreement dated 18-August-1989 for support of Medley.		*//*									*//************************************************************************/E 26E 23E 17E 6I 1D 2/* This is G-file %M% Version %I% (%G%). copyright Xerox & Fuji Xerox  */E 2I 19D 21#ifdef NOASM#define RETD6#define RET_CHECK_NATIVE(x)#elseE 21I 21#ifdef NATIVETRANE 21#define RETD6 SaveD6 = 0x100#define RET_CHECK_NATIVE(x) if(x ->native) { RET_TO_NATIVE; }I 21#else#define RETD6#define RET_CHECK_NATIVE(x)E 21#endifE 19I 16D 17#if (defined(sun) && defined(mc68020) && ~(defined(NOASM)) && ~(defined(NOASMFNCALL)) )E 17I 17D 21#if (defined(sun) && defined(mc68020) && !(defined(NOASM)) && !(defined(NOASMFNCALL)) ) E 21I 21D 24#if (defined(SUN3_OS3_IL) &&  !(defined(NOASMFNCALL)) )E 24I 24#if (defined(SUN3_OS3_OR_OS4_IL) &&  !(defined(NOASMFNCALL)) )E 24E 21E 17D 18#define OPRETURN	{ opreturn(); }E 18I 18#define OPRETURN							\D 23{ 	opreturn();							\D 22 	EXT; slowreturn(); RET;  					\E 22I 22 	EXT; if(slowreturn()) goto stackoverflow_help; RET;  		\E 22 	Irq_Stk_Check = STK_END_COMPUTE(EndSTKP,FuncObj);		\	if (((int)(CSTKPTR) > Irq_Stk_Check) || (Irq_Stk_End <= 0)) 	\D 20			{ SaveD6 = 0x100; goto check_interrupt;	}	\E 20I 20			{ RETD6; goto check_interrupt;	}	\E 20 	Irq_Stk_End = (int) EndSTKP;					\E 23I 23{	opreturn();							\	EXT; if(slowreturn()) goto stackoverflow_help; RET;		\	Irq_Stk_Check = STK_END_COMPUTE(EndSTKP,FuncObj);		\	if (((int)(CSTKPTR) > Irq_Stk_Check) || (Irq_Stk_End <= 0))	\			{ RETD6; goto check_interrupt;	}		\	Irq_Stk_End = (int) EndSTKP;					\E 23D 19 	if (BCE_CURRENTFX->native) { RET_TO_NATIVE; }			\E 19I 19	RET_CHECK_NATIVE(BCE_CURRENTFX);				\E 19 }E 18#elseE 16D 10E 10I 10D 13#ifdef NO_C_HARDRET							E 13E 10D 2#define OPRETURN	{\ register struct frameex1 *returnFX ; \ returnFX = CURRENTFX; \ if( returnFX->alink & 1) { goto ni; }; \  /* Returnee's Stack(PV area) exists just before Retuner's BF */ \ CSTKPTR = IVAR  ; \ /* Get PVAR from ALINK */ \ PVAR= Addr68k_from_LADDR((STK_OFFSET | returnFX->alink)); \ /* Move CURRENTFX ptr */ \ CURRENTFX = returnFX = (struct  frameex1 *)(PVAR - FRAMESIZE) ; \ IVAR = Addr68k_from_LADDR(STK_OFFSET | *((DLword *)returnFX -1)) ; \E 2I 2#define OPRETURN	{						\D 3 register struct frameex1 *returnFX ; 					\ returnFX = CURRENTFX; 							\E 3I 3D 23 register struct frameex2 *returnFX ; 					\D 4 returnFX =  BCE_CURRENTFX; 						\E 3 if( returnFX->alink & 1) { goto ni; }; 				\  /* Returnee's Stack(PV area) exists just before Retuner's BF */ 	\E 4I 4 register int alink; 							\E 23I 23 register struct frameex2 *returnFX ;					\ register int alink;							\E 23 alink = ((struct frameex2 *) BCE_CURRENTFX)->alink;			\I 29 FNTPRINT(("RETURN.\n"));						\E 29D 8 if (alink & 1) { goto ni; }; 						\E 8I 8D 9 if (alink & 1) { EXT; OP_return(); NRET; }; 				\E 9I 9D 12 if (alink & 1) { EXT; OP_return(); RET; goto retxit; }; 		\E 12I 12D 13 if (alink & 1) { EXT; OP_return(); RET; goto retxit; };		\E 13I 13D 16 if (alink & 1) { EXT; slowreturn(); RET;  goto retxit; }; 		\E 16I 16D 22 if (alink & 1) { EXT; slowreturn(); RET;  				\E 22I 22D 23 if (alink & 1) { EXT; if(slowreturn()) goto stackoverflow_help; RET; 	\E 22 	Irq_Stk_Check = STK_END_COMPUTE(EndSTKP,FuncObj);		\	if (((int)(CSTKPTR) > Irq_Stk_Check) || (Irq_Stk_End <= 0)) 	\D 20			{ SaveD6 = 0x100; goto check_interrupt;	}	\E 20I 20			{ RETD6; goto check_interrupt;	}	\E 20 	Irq_Stk_End = (int) EndSTKP;					\D 19 	if (BCE_CURRENTFX->native) { RET_TO_NATIVE; }			\E 19I 19 	RET_CHECK_NATIVE(BCE_CURRENTFX);			\E 19	goto retxit; 							\	}; 								\E 16E 13E 12E 9E 8E 4 CSTKPTR = (LispPTR *) IVAR; 						\E 23I 23 if (alink & 1) { EXT; if(slowreturn()) goto stackoverflow_help; RET;	\	Irq_Stk_Check = STK_END_COMPUTE(EndSTKP,FuncObj);		\	if (((int)(CSTKPTR) > Irq_Stk_Check) || (Irq_Stk_End <= 0))	\			{ RETD6; goto check_interrupt;	}		\	Irq_Stk_End = (int) EndSTKP;					\	RET_CHECK_NATIVE(BCE_CURRENTFX);				\	goto retxit;							\	};								\D 27 CSTKPTR = (LispPTR *) IVAR;						\E 27I 27 CSTKPTRL = (LispPTR *) IVAR;						\E 27E 23D 4 /* Get PVAR from ALINK */ 						\D 3 PVAR = (LispPTR *) Addr68k_from_LADDR((STK_OFFSET | returnFX->alink)); 		\ /* Move CURRENTFX ptr */ 						\ returnFX = (struct  frameex1 *)((DLword *) PVAR - FRAMESIZE) ; 	\ IVAR = (LispPTR *) Addr68k_from_LADDR(STK_OFFSET | *((DLword *)returnFX -1));\E 2 FuncObj = (struct fnhead *)Addr68k_from_LADDR((returnFX->hi2fnheader <<16) | returnFX->lofnheader) ; \E 3I 3 PVAR = (LispPTR *) Addr68k_from_StkOffset(returnFX->alink); 		\ returnFX = (struct  frameex2 *)((DLword *) PVAR - FRAMESIZE) ; 	\E 4I 4 returnFX = (struct  frameex2 *)					\D 23	((DLword *) 							\E 23I 23	((DLword *)							\E 23D 27	    (PVAR = (LispPTR *) Addr68k_from_StkOffset(alink))		\E 27I 27D 28	    (PVARL = (LispPTR *) Addr68k_from_StkOffset(alink))		\E 28I 28	    (PVARL = (DLword *) Addr68k_from_StkOffset(alink))		\E 28E 27D 23	    - FRAMESIZE); 						\E 23I 23	    - FRAMESIZE);						\E 23E 4D 25 IVAR = (LispPTR *) Addr68k_from_StkOffset(*((DLword *)returnFX -1));	\E 25I 25D 27 IVAR = (LispPTR *) Addr68k_from_StkOffset(GETWORD((DLword *)returnFX -1));	\E 27I 27D 28 IVARL = (LispPTR *)							\E 28I 28 IVARL = (DLword *)							\E 28	    Addr68k_from_StkOffset(GETWORD((DLword *)returnFX -1));	\E 27E 25D 4 FuncObj = (struct fnhead *)Addr68k_from_LADDR(SWAP_WORDS(returnFX->fnheader) & 0x00FFFFFF) ; \E 4E 3D 2	/* Get PC from Retunee's pc slot in FX */ \ PCMAC = (ByteCode *)FuncObj + returnFX->pc ; \E 2I 2D 23	/* Get PC from Retunee's pc slot in FX */ 			\E 23I 23	/* Get PC from Retunee's pc slot in FX */			\E 23I 7D 12 if (returnFX->native) {						\	PC = (ByteCode *) returnFX->pc;					\	FuncObj = (struct fnhead *)					\	Addr68k_from_LADDR(SWAP_WORDS(returnFX->fnheader) & 0x0ffffff);	\	goto call_native;						\	}								\E 12E 7D 4 PCMAC = (ByteCode *)FuncObj + returnFX->pc ; 				\E 4I 4D 17 PCMAC = returnFX->pc  + (ByteCode *)					\E 17I 17 PCMACL = returnFX->pc  + (ByteCode *)					\E 17D 23 	(FuncObj = (struct fnhead *)					\E 23I 23	(FuncObj = (struct fnhead *)					\E 23D 17	Addr68k_from_LADDR(SWAP_WORDS(returnFX->fnheader) & 0x0ffffff));\E 17I 17	Addr68k_from_LADDR(SWAP_WORDS(returnFX->fnheader) & 0x0ffffff)) + 1;\E 17I 9D 16retxit:									\E 16D 13 Irq_Stk_Check = STK_END_COMPUTE(StkLimO, FuncObj);			\D 12 if (((int)CSTKPTR > Irq_Stk_Check) || (Irq_Stk_End == 0)) 		\		goto check_interrupt;					\E 12I 12 if (((int)(CSTKPTR+1) > Irq_Stk_Check) || (Irq_Stk_End == 0)) 		\E 13I 13D 14 Irq_Stk_Check = (int)EndSTKP - STK_MIN(FuncObj);			\ if (((int)(CSTKPTR+1) > Irq_Stk_Check) || (Irq_Stk_End <= 0)) 		\E 14I 14 Irq_Stk_Check = STK_END_COMPUTE(EndSTKP,FuncObj);			\I 29  FNCHECKER(if (quick_stack_check()) printf("In RETURN.\n"));	\E 29D 23 if (((int)(CSTKPTR) > Irq_Stk_Check) || (Irq_Stk_End <= 0)) 		\E 23I 23 if (((int)(CSTKPTR) > Irq_Stk_Check) || (Irq_Stk_End <= 0))		\E 23E 14E 13D 20		{ SaveD6 = 0x100; goto check_interrupt;	}		\E 20I 20		{ RETD6; goto check_interrupt;	}		\E 20E 12D 13 Irq_Stk_End = (int) StkLimO;						\E 13I 13 Irq_Stk_End = (int) EndSTKP;						\E 13I 12D 15 if (BCE_CURRENTFX->native) { goto ret_native; }			\E 15I 15D 16 if (BCE_CURRENTFX->native) { RET_TO_NATIVE; }				\E 16I 16D 19 if (returnFX->native) { RET_TO_NATIVE; }				\E 19I 19 RET_CHECK_NATIVE(returnFX);						\E 19retxit:	 {}								\E 16E 15E 12E 9E 4E 2D 23} /* OPRETURN end */ E 23I 23} /* OPRETURN end */E 23I 16#endifE 16D 13I 10#elseE 10D 10E 10I 10#define OPRETURN	{						\ register struct frameex2 *returnFX ; 					\ register int alink; 							\ alink = ((struct frameex2 *) BCE_CURRENTFX)->alink;			\ if (alink & 1) { EXT; slowreturn(); RET; goto retxit; }; 		\ CSTKPTR = (LispPTR *) IVAR; 						\ returnFX = (struct  frameex2 *)					\	((DLword *) 							\	    (PVAR = (LispPTR *) Addr68k_from_StkOffset(alink))		\	    - FRAMESIZE); 						\ IVAR = (LispPTR *) Addr68k_from_StkOffset(*((DLword *)returnFX -1));	\	/* Get PC from Retunee's pc slot in FX */ 			\D 12 if (returnFX->native) {						\	PC = (ByteCode *) returnFX->pc;					\	FuncObj = (struct fnhead *)					\	Addr68k_from_LADDR(SWAP_WORDS(returnFX->fnheader) & 0x0ffffff);	\	goto call_native;						\	}								\E 12 PCMAC = returnFX->pc  + (ByteCode *)					\ 	(FuncObj = (struct fnhead *)					\	Addr68k_from_LADDR(SWAP_WORDS(returnFX->fnheader) & 0x0ffffff));\retxit:									\ Irq_Stk_Check = STK_END_COMPUTE(StkLimO, FuncObj);			\D 12 if (((int)CSTKPTR > Irq_Stk_Check) || (Irq_Stk_End == 0)) 		\		goto check_interrupt;					\E 12I 12 if (((int)(CSTKPTR+1) > Irq_Stk_Check) || (Irq_Stk_End < 0)) 		\		{ SaveD6 = 0x100; goto check_interrupt;	}		\E 12 Irq_Stk_End = (int) StkLimO;						\I 12 if (BCE_CURRENTFX->native) { goto ret_native; }			\E 12} /* OPRETURN end */ #endif		E 13E 10E 1