h06408s 00022/00006/00004d D 1.2 90/06/04 14:45:40 sybalsky 2 1c reformat, copyrightes 00010/00000/00000d D 1.1 88/07/26 16:00:01 hayata 1 0c date and time created 88/07/26 16:00:01 by hayataeuUtTI 1D 2/* This is G-file %Z% %M% Version %I% (%G%). copyright Xerox & Fuji Xerox  */E 2I 2/* This is G-file %Z% %M% Version %I% (%G%). copyright Venue & Fuji Xerox  */E 2/*************************************************D 2	This is OSNESSAGE stuf.E 2I 2	This is OSMESSAGE stuf.	Print a console message.E 2*************************************************/D 2#define	OSMESSAGE_PRINT(print_exp){	\		 flush_pty();		\		 print_exp;		\		}E 2I 2/************************************************************************//*									*//*	Copyright 1989, 1990 Venue, Fuji Xerox Co., Ltd, Xerox Corp.	*//*									*//*	This file is work-product resulting from the Xerox/Venue	*//*	Agreement dated 18-August-1989 for support of Medley.		*//*									*//************************************************************************/#define	OSMESSAGE_PRINT(print_exp)		\  {						\    flush_pty();				\    print_exp;					\  }E 2E 1