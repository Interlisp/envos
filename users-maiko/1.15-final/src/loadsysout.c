/* @(#) loadsysout.c Version 2.21 (3/30/89). copyright Envos & Fuji Xerox  */static char *id = "@(#) loadsysout.c	2.21 3/30/89";/* *	loadsysout.c */#include <stdio.h>#include <sys/file.h>#include <sys/types.h>#include <sys/stat.h>#include <errno.h>#include "address68k.h"#include "lispemul.h"#include "lisptypes.h"#include "lispmap.h"#include "lispglobal.h"#include "ifpage.h"#include "dbprint.h"#define	IFPAGE_ADDRESS	512#define MBYTE	0x100000 /* 1 Mbyte */extern int	errno;char *valloc();sysout_loader(sysout_file_name, sys_size)char	*sysout_file_name;int	sys_size;   /* sysout size in megabytes */{    int             sysout;	/* SysoutFile descriptor */    IFPAGE         ifpage;	/* IFPAGE */    char           *fptovp_scratch;	/* scratch area for FPTOVP */    DLword         fptovp[0x10000];	/* FPTOVP */    long           fptovp_offset;	/* FPTOVP start offset */    char           *lispworld_scratch;	/* scratch area for lispworld */    long            lispworld_offset;	/* lispworld offset */    unsigned        sysout_size;/* sysout size in page */    struct stat     stat_buf;	/* file stat buf */    int             i, vp;    int             machinetype;    char            errmsg [ 255 ];    machinetype = 0;    /* allocate Virtual Memory Space */    lispworld_scratch = valloc(sys_size * MBYTE);   if (lispworld_scratch == NULL) {	fprintf(stderr,"sysout_loader: can't allocate Lisp %dMBytes VM \n",								   sys_size);	exit(-1);   }    /* now you can access lispworld */    Lisp_world = (DLword *) lispworld_scratch;    DBPRINT(("VM allocated = 0x%x at 0x%x\n", sys_size * MBYTE, Lisp_world));    DBPRINT(("Native Load Address = 0x%x\n", native_load_address));    /*     * first read the IFPAGE(InterfacePage)     */    /* open SysoutFile */    sysout = open(sysout_file_name, O_RDONLY, NULL);    if (sysout == -1) {	sprintf( errmsg,                 "sysout_loader: can't open sysout file: %s",		 sysout_file_name);	perror( errmsg );	exit(-1);    }    /* seek to IFPAGE */    if (lseek(sysout, IFPAGE_ADDRESS, 0) == -1) {	perror("sysout_loader: can't seek to IFPAGE");	exit(-1);    }    /* reads IFPAGE into scratch_page */    if (read(sysout, &ifpage, sizeof(IFPAGE)) == -1) {	perror("sysout_loader: can't read IFPAGE");	exit(-1);    }#ifdef BYTESWAP    word_swap_page(&ifpage, (3+sizeof(IFPAGE))/4);#endif    /*     * get FPTOVP location and SysoutFile size     */    /* get FPTOVP location from IFPAGE */    fptovp_offset = ifpage.fptovpstart;    DBPRINT(("FPTOVP Location %d \n", fptovp_offset));    /* get sysout file size in halfpage(256) */    if (fstat(sysout, &stat_buf) == -1) {	perror("sysout_loader: can't get sysout file size");	exit(-1);    }    sysout_size = stat_buf.st_size / BYTESPER_PAGE * 2;    DBPRINT(("sysout size / 2 = 0x%x\n", sysout_size / 2));    DBPRINT(("vmem size = 0x%x\n", ifpage.nactivepages));    /* do some simple checks to see if this is really a sysout */    if(ifpage.key != IFPAGE_KEYVAL) {	printf("sysout_loader: %s isn't a sysout:\nkey is %x, should be %x\n",			sysout_file_name, ifpage.key, IFPAGE_KEYVAL);	exit(1);    }    machinetype = ifpage.machinetype;    if(( stat_buf.st_size & 0x1ff) != 0)	printf("CAUTION::sysout & 0x1ff = 0x%x\n",	stat_buf.st_size & BYTESPER_PAGE);    if(ifpage.nactivepages != ( sysout_size /2) ) {	printf("sysout_loader:IFPAGE says sysout size is %d\n",               ifpage.nactivepages);	printf("But, sysout size from UNIX is %d\n",sysout_size /2);	exit(-1);    }    /*     * Now get FPTOVP     */    /* seek to FPTOVP */    fptovp_offset = (fptovp_offset - 1) * BYTESPER_PAGE + 2;    if (lseek(sysout, fptovp_offset, 0) == -1) {	perror("sysout_loader: can't seek to FPTOVP");	exit(-1);    }    /* read FPTOVP */    if (read(sysout, fptovp, sysout_size) == -1) {	perror("sysout_loader: can't read FPTOVP");	exit(-1);    }#ifdef BYTESWAP    word_swap_page(fptovp, (sysout_size/4)+1);#endif    /*     * Initialize the display (note now passing 68k address!!!)     */    init_display2(Addr68k_from_LADDR(DISPLAY_OFFSET), 65536 * 16 * 2);    /* read sysout file to lispworld */    for (i = 0; i < (sysout_size / 2); i++)      {	if (GETBASEWORD(fptovp,i) != 0177777)	  {	    if (lseek(sysout, i * BYTESPER_PAGE, 0) == -1)	      {		perror("sysout_loader: can't seek sysout file");		exit(-1);	      };	    lispworld_offset = GETBASEWORD(fptovp,i) * BYTESPER_PAGE;	    if (read(sysout, lispworld_scratch +			     lispworld_offset , BYTESPER_PAGE) == -1)	      {		printf("sysout_loader: can't read sysout file at %d\n", i);		printf("               offset was 0x%x (0x%x pages).\n",		       lispworld_offset, GETBASEWORD(fptovp,i));		perror("read() error was");		exit(-1);	      };#ifdef BYTESWAP            word_swap_page(lispworld_scratch+lispworld_offset, 128);#endif	  };      }    DBPRINT(("sysout file is read completely.\n"));#ifdef DISPLAYBUFFER    flush_display_buffer();#endif#ifdef XWINDOW    flush_display_buffer();#endif XWINDOW    close(sysout);}