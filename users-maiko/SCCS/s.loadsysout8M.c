h15038s 00002/00001/00182d D 1.3 88/03/17 18:10:01 krivacic 3 2c Hi Res Display Hacks es 00002/00002/00181d D 1.2 88/03/13 14:54:09 hayata 2 1c Add SCCS key id (%Z%)es 00183/00000/00000d D 1.1 88/02/24 16:57:57 hayata 1 0c date and time created 88/02/24 16:57:57 by hayataeuUtTI 1D 2/* This is G-file %M% Version %I% (%G%). copyright Xerox & Fuji Xerox  */static char *id = "%M%	%I% %G%";E 2I 2/* This is G-file %Z% %M% Version %I% (%G%). copyright Xerox & Fuji Xerox  */static char *id = "%Z% %M%	%I% %G%";E 2/* *	Copyright (C) 1987 by Fuji Xerox Co., Ltd. All rights reserved. * *	Auther *//* *	sysoutloader8M.c */#include <stdio.h>#include <sys/file.h>#include <sys/types.h>#include <sys/stat.h>#include <errno.h>#include "lispemul.h"#include "lispmap.h"#include "lispglobal.h"#include "ifpage.h"#define	IFPAGE_ADDRESS	512#define	LISP_SIZE4M	4194304	  /*  4 Mb */#define LISP_SIZE8M	0x0800000 /* 8 M */#define LISP_SIZE16M	0x1000000 /* 16 M */#define LISP_SIZE24M	0x1800000 /* 24 Mb */#define LISP_SIZE32M	0x2000000 /* 32 Mb */extern int	errno;sysout_loader(sysout_file_name)char	*sysout_file_name;{    int             sysout;	/* SysoutFile descriptor */    char           *malloc();	/* malloc() */    char           *valloc();	/* valloc() */    char           *ifpage_scratch;	/* scratch area for IFPAGE */    IFPAGE         *ifpage;	/* IFAPGE */    char           *fptovp_scratch;	/* scratch area for FPTOVP */    DLword         *fptovp;	/* FPTOVP */    long            fptovp_offset;	/* FPTOVP start offset */    char           *lispworld_scratch;	/* scratch area for lispworld */    long            lispworld_offset;	/* lispworld offset */    unsigned        sysout_size;/* sysout size in page */    struct stat     stat_buf;	/* file stat buf */    int             i, vp;    /*     * first read the IFPAGE(InterfacePage)      */    /* open SysoutFile */    sysout = open(sysout_file_name, O_RDONLY, NULL);    if (sysout == -1) {	printf("sysout_loader: can't open SysoutFile");	exit(-1);    }    /* seek to IFPAGE */    if (lseek(sysout, IFPAGE_ADDRESS, 0) == -1) {	printf("sysout_loader: can't seek to IFPAGE");	exit(-1);    }    /* allocate ScratchPage for IFPAGE */    ifpage_scratch = malloc(BYTESPER_PAGE);    if (ifpage_scratch == NULL) {	printf("sysout_loader: can't allocate scratch page");	exit(-1);    }    /* reads IFPAGE into scratch_page */    if (read(sysout, ifpage_scratch, BYTESPER_PAGE) == -1) {	printf("sysout_loader: can't read IFPAGE");	exit(-1);    }    /* now you can access IFPAGE */    ifpage = (IFPAGE *) ifpage_scratch;    /*     * get FPTOVP location and SysoutFile size      */    /* get FPTOVP location from IFPAGE */    fptovp_offset = ifpage->fptovpstart;    printf("FPTOVP Location %d \n", fptovp_offset);    /* free ifpage scratch */    if (free(ifpage_scratch) == -1) {	printf("sysout_loader: can't free scratch_page");	exit(-1);    }    /* get sysout file size in halfpage(256) */    if (fstat(sysout, &stat_buf) == -1) {	printf("sysout_loader: can't get SysoutFile size");	exit(-1);    }    sysout_size = stat_buf.st_size / BYTESPER_PAGE * 2;    printf("sysout size = %d\n", sysout_size / 2);	/*****  take ***/	if(( stat_buf.st_size & 0x1ff) != 0)		printf("CAUTION::sysout & 0x1ff = 0x%x \n", stat_buf.st_size & BYTESPER_PAGE);	if(ifpage->nactivepages !=( sysout_size /2) ) {		printf("sysout_loader:IFPAGE says sysout size is %d \n",ifpage->nactivepages);		printf("But,sysout size from UNIX is %d \n",sysout_size /2);		 error("BYE"); 	 }    /*     * Now get FPTOVP      */    /* allocate fptovp area */    fptovp_scratch = malloc(sysout_size);    if (fptovp_scratch == NULL) {	printf("sysout_loader: can't allocate FPTOVP");	exit(-1);    }    /* seek to FPTOVP */    fptovp_offset = (fptovp_offset - 1) * BYTESPER_PAGE + 2;    if (lseek(sysout, fptovp_offset, 0) == -1) {	printf("sysout_loader: can't seek to FPTOVP");	exit(-1);    }    /* read FPTOVP */    if (read(sysout, fptovp_scratch, sysout_size) == -1) {	printf("sysout_loader: can't read FPTOVP");	printf("%d\n", errno);	exit(-1);    }    /* now you can access FPTOVP */    fptovp = (DLword *) fptovp_scratch;    /*     * Last Build the Lisp VM      */    /* allocate 8MBytes Virtual Memory Space (take)*/    lispworld_scratch = valloc(LISP_SIZE8M);    if (lispworld_scratch == NULL) {	printf("sysout_loader: can't allocate Lisp 8MBytes VM");	exit(-1);    }    /* now you can access lispworld */    Lisp_world = (DLword *) lispworld_scratch;    D 3    init_display();			/* modified by osamu Sep. 3 */E 3I 3    init_display(DISPALY_OFFSET, 65566 * 16);	E 3I 3    E 3    /* read SysoutFile to 8MB lispworld */    for (i = 0; i < (sysout_size / 2); i++) { /*	printf("%4d %6o", i, *fptovp); */	if (*fptovp != 0177777) {	    if (lseek(sysout, i * BYTESPER_PAGE, 0) == -1) {		printf("sysout_loader: can't seek SysoutFile");		exit(-1);	    };	    lispworld_offset = *fptovp * BYTESPER_PAGE;	    if (read(sysout, lispworld_scratch + lispworld_offset, BYTESPER_PAGE) == -1) {		printf("sysout_loader: can't read SysoutFile at %d", i);		exit(-1);	    };	};/*	printf("\b\b\b\b\b\b\b\b\b\b\b"); */	fptovp++;    }    /* free FPTOVP and Scratch Area */    if (free(fptovp_scratch) == -1) {	printf("sysout_loader: can't free fptovp");	exit(-1);    }    printf("SysoutFile is read completely.\n");    close(sysout);}E 1