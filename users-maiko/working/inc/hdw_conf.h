/* @(#) hdw_conf.h Version 1.2 (2/8/93). copyright Venue  *//************************************************************************//*									*//*	(C) Copyright 1989-92 Venue. All Rights Reserved.		*//*	Manufactured in the United States of America.			*//*									*//*	The contents of this file are proprietary information 		*//*	belonging to Venue, and are provided to you under license.	*//*	They may not be further distributed or disclosed to third	*//*	parties without the specific permission of Venue.		*//*									*//************************************************************************//**************************************************************//*	File name :	machineconfig.h	Created	:	Aug-19-87 take	Desc. :		Define for Machine dependent param*//**************************************************************//* Machine Type */#define KATANA		3/* Lastvmempage(32Mbyte) */#ifdef BIGVM#define LASTVMEMPAGE 0x1FFFF#else#define LASTVMEMPAGE	(65535)#endif