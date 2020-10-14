/* This is G-file @(#) new-smallfs.c Version 1.2 (4/4/88). copyright Xerox & Fuji Xerox  */static char *id = "@(#) new-smallfs.c	1.2 4/4/88";/* *	Copyright (C) 1987 by Fuji Xerox Co., Ltd. All rights reserved. * *	Auther	:	Hiroshi Hayata	 *//*** ADOPTED BOB'S VERSION ***/#include	<sys/file.h>#include	<sys/types.h>#include	<sys/stat.h>#include	<sys/param.h>#include	<errno.h>#include	"lispemul.h"#include	"lispmap.h"#include	"address68k.h"#include	"lisptypes.h"#include	"arith.h"#include	"stream.h"#include	"filesystem.h"#define	PAGESIZE	512	/* 1 page == 512 byte */#define	CELLPERBYTE	4#define CellFromByte(xbyte)	((xbyte/CELLPERBYTE)+(xbyte%CELLPERBYTE ? 1 : 0))#define	RECOG_OLD		S_POSITIVE | 0#define	RECOG_OLDEST		S_POSITIVE | 1#define	RECOG_NEW		S_POSITIVE | 2#define	RECOG_NEW_OLD		S_POSITIVE | 3#define	ACCESS_INPUT		S_POSITIVE | 0#define	ACCESS_OUTPUT		S_POSITIVE | 1#define	ACCESS_BOTH		S_POSITIVE | 2#define	ACCESS_APPEND		S_POSITIVE | 3extern	int	errno;extern	DLword	*Lisp_world;	/* To access LispSysout area */extern	DLword	*MDStypetbl;	char	*setpathname();char	*malloc();DSK_readpage(args)LispPTR	*args;/*	args[0]	:	Indirect pointer to FileID	args[1] :	Indirect pointer to PageOffset	args[2] :	pointer to buffer		*/{int	id;int	pageoffset;char	*bufp;int	count;char	*bp;#ifdef TRACE	printf("TRACE:DSK_readpage(%x) \n", args);#endif#ifndef	TEST	if( (GetTypeEntry(args[0]) | TT_FIXP) == 0 ){		printf("Error: DSK_readpage[Bad pointer to FileID: %x]\n", args[0]);		return(NIL);	}#endif	if( (args[0] & 0xFF0000) == S_POSITIVE )		id = args[0] & 0xFFFF;	else		id = *((int *)(Addr68k_from_LADDR(args[0])));	pageoffset = GetSmalldata(args[1]);	bufp = (char*)(Addr68k_from_LADDR(args[2]));	/* The filepointer is set to (pageoffset*PAGESIZE) bytes. */	if(lseek(id, (pageoffset*PAGESIZE), L_SET) == -1){		printf("Error: DSK_readpage[lseek errno %d]\n", errno);		return(NIL);	}	/* Read BUFFSIZE bytes file contents from filepointer. */	count = read(id, bufp, PAGESIZE);	if( count == -1){		printf("Error: DSK_readpage[read errno %d]\n", errno);		return(NIL);	}	/* Fill the unread part of buffer with 0. */	for(bp = &bufp[count]; count < PAGESIZE; ++count, ++bp){		*bp = 0;	}	return(ATOM_T);}DSK_writepage(args)LispPTR	*args;/*	args[0]	:	Indirect pointer to FileID	args[1] :	Indirect pointer to PageOffset	args[2] :	pointer to buffer		args[3] :	Indirect pointer to size		*/{int	id,i;int	pageoffset;char	*bufp;int	count, size;char	*bp;	printf("TRACE:DSK_writepage(%x) %x %x %x %x \n", args,args[0],args[1],args[2],args[3]);#ifdef TRACE	printf("TRACE:DSK_writepage(%x) %x %x %x %x \n", args,args[0],args[1],args[2],args[3]);#endif#ifndef	TEST	if( (GetTypeEntry(args[0]) | TT_FIXP) == 0 ){		printf("Error: DSK_writepage[Bad pointer to FileID: %x]\n", args[0]);		return(NIL);	}#endif	if( (args[0] & 0xFF0000) == S_POSITIVE )		id = args[0] & 0xFFFF;	else		id = *((int *)(Addr68k_from_LADDR(args[0])));	pageoffset = GetSmalldata(args[1]);	size = GetSmalldata(args[3]);	bufp = (char*)(Addr68k_from_LADDR(args[2]));	/* The filepointer is set to (pageoffset*PAGESIZE) bytes. */	if(lseek(id, (pageoffset*PAGESIZE), L_SET) == -1){		printf("Error: DSK_writepage[lseek errno %d]\n", errno);		return(NIL);	}/* *****	printf("Writing %d bytes at %d\n",size,(pageoffset*PAGESIZE));	for(i=0, bp=bufp; i<PAGESIZE; ++i, ++bp)		{printf("%c ",*bp);}	printf("\n");	for(i=0, bp=bufp; i<size; ++i, ++bp)		{printf("%c ",*bp);}	printf("\n");****** */	/* Read PAGESIZE bytes file contents from filepointer. *//* **** ***** Temproarily changed write size to PAGESIZE instead of size	***** Due to error in LISP sysout code for pagewrite****** *//** Removed 3/11 by Hiroshi. Load DSKWRITEPATCH.LCOM.	size = PAGESIZE;	while ((bufp[size-1] == 0) && (size > 2)) size -= 1;**/	count = write(id, bufp, size);	if( count == -1){		printf("Error: DSK_writepage[write errno %d]\n", errno);		return(NIL);	}/** Removed 3/11 by Hiroshi	if( count != PAGESIZE ){		bufp[count] = 0xFF;	}**/	return(ATOM_T);}DSK_getsize(args)LispPTR	*args;/*	args[0]:	Indirect pointer file ID to be closed.	(ex)	args[0]: {S_POSHI, xxx}	-> FileID: xxx		args[0]: {hi, low}	-> FileID: @{hi, low}	args[1]:	Pointer to FIXP buffer.	*/{int	id;	/* FileID */int	*bufp;	/* pointer to FIXP buffer */struct	stat	sbuf;	/* Strucure for FileInfo. */#ifdef TRACE	printf("TRACE:DSK_getsize(%x) \n", args);#endif	if( (args[0] & 0xFF0000) == S_POSITIVE )		id = args[0] & 0xFFFF;	else		id = *((int *)(Addr68k_from_LADDR(args[0])));	if(fstat(id, &sbuf) != 0){		printf("Error: stat [errno %d]\n", errno);		return(NIL);	}	bufp = (int *)(Addr68k_from_LADDR(args[1]));	*bufp = sbuf.st_size;#ifdef	DEBUG	printf("DEBUG:DSK_getsize[size -- %d]\n", *bufp);#endif	return(ATOM_T);}DSK_deletefile(args)LispPTR	*args;/*	args[0]:	Indirect pointer to buffer	*//*	args[1]:	Length of the file   -- No need	*/{char	buf1[MAXPATHLEN];char	*filep;#ifdef TRACE	printf("TRACE:DSK_deletefile(%x) \n", args);#endif	GetName(args[0], buf1, filep);	catpathname(buf1, filep);	if(unlink(buf1) == -1){		printf("Error: unlink[errno %d]\n", errno);		return(NIL);	}	return(ATOM_T);}DSK_renamefile(args)LispPTR	*args;/*	args[0]:	Indirect pointer to buffer (old file name)	*//*	args[1]:	Indirect pointer to buffer (new file name)	*/{char	*filep0, *filep1;char	buf0[MAXPATHLEN], buf1[MAXPATHLEN];#ifdef TRACE	printf("TRACE:DSK_renamefile(%x) \n", args);#endif	GetName(args[0], buf0, filep0);	GetName(args[1], buf1, filep1);	catpathname(buf0, filep0);	catpathname(buf1, filep1);	if(rename(buf0, buf1) == -1){		printf("Error: rename[errno %d]\n", errno);		return(NIL);	}	return(ATOM_T);}DSK_closefile(args)LispPTR	*args;/*	args[0]:	Indirect pointer file ID to be closed.	(ex)	args[0]: {S_POSHI, xxx}	-> FileID: xxx		args[0]: {hi, low}	-> FileID: @{hi, low}	*/{int	id;	/* FileID */#ifdef TRACE	printf("TRACE:DSK_closefile(%x : %x) \n", args, *args);#endif	if( (args[0] & 0xFF0000) == S_POSITIVE )		id = args[0] & 0xFFFF;	else		id = *((int *)(Addr68k_from_LADDR(args[0])));	if( close(id) == -1){		printf("Error: close [errno %d]\n", errno);		return(NIL);	}	return(ATOM_T);}DSK_getfilename(args)LispPTR	*args;/*	args[0]:	Lisp pointer to file name to get.	args[1]:	Length of file name.   -- NO need	args[2]:	Indirect pointer to buffer area to save the got file name. 	args[3]:	OLDFILE(== S_POSITIVE|0) or NEWFILE(== S_POSITIVE|1)	*/{LispPTR	*p1;		/* Address of pointer */char	dirp[MAXPATHLEN], *filep;char	*buf2;char	*buf3;		/* 68K pointer to buffer area */int	i,len,len1;#ifdef TRACE	printf("TRACE:DSK_getfilename(%x) \n", args);#endif	GetName(args[0], dirp, filep);	if((i = dirSize(dirp)) == -1){		printf("Error: DSK_getfilename[Bad directory name %s]\n", dirp);		return(NIL);	}	if((buf2 = malloc(i)) == 0){		printf("Error: DSK_getfilename[Can allocate buffer %s]\n", dirp);		return(NIL);	}	if(readDir(dirp, buf2) == -1){		printf("Error: DSK_getfilename[Bad directory %s]\n", dirp);		free(buf2);		return(NIL);	}	i = searchfile(filep, buf2);	switch(args[3]){	case RECOG_OLD:	case RECOG_OLDEST:		if(i == -1){	/* The file does not exist. */			free(buf2);			return(NIL);		}		break;	case RECOG_NEW_OLD:	case RECOG_NEW:		if(i != -1){	/* The file already exist. */			if(handle_oldfile(dirp, filep) == NIL){				free(buf2);				return(NIL);			}		}		break;	default:		free(buf2);		return(NIL);	}	p1 = (LispPTR *)(Addr68k_from_LADDR(args[2]));	buf3 = (char *)(Addr68k_from_LADDR(0xFFFFFF & (*p1)));	*buf3++ = '{';	*buf3++ = 'D';	*buf3++ = 'S';	*buf3++ = 'K';	*buf3++ = '}';	*buf3++ = '<';	len = strlen(dirp);	for(i=0;i<len;++i,++buf3)		*buf3 = dirp[i];	*buf3++ = '>';	len1 = strlen(filep);	for(i=0;i<len1;++i,++buf3,++filep)		*buf3 = *filep;	free(buf2);	return(GetSmallp(len+len1+7));}searchfile(filep, buffer)char	*filep;char	*buffer;	/* Assume the contents is file names.				(ex) ".\..\abc.c\def.c\dir1\"	*/{char	*cp;#ifdef	TRACE	printf("searchfile(%s %s)\n", filep, buffer);#endif	for(;;){		if(*buffer == '\0')		/* Empty buffer */			return(-1);		for(cp=buffer; *cp!='\\'; ++cp);		*cp = '\0';		if(strcmp(filep, buffer) == 0)	/* Found file name */			return(0);		else				if(*(++cp) == '\0')	/* Serched all. Not found */				return(-1);			else				buffer = cp;	/* search next */	}}char *setpathname(sbuf, dbuf, length)char	*sbuf;	/* (Ex) Contents of buffer :			 "{DSK}abc.c" or "{DSK}</usr/fx/hayata/>abc.c" */char	*dbuf;	/* At exit: (Ex) "/usr/fx/hayata/\0abc.c\0" */			/* Return value points the first of file name.			   (Ex) Return value points 'a' */int	length;{char	*filep;	/* Points the top of file name. */int	i, dirlength;#ifdef	TRACE	printf("TRACE:setpathname[sbuf: %c %c %c %c %c %c]\n[length %d]\n", *sbuf, *(sbuf+1), *(sbuf+2), *(sbuf+3), *(sbuf+4), *(sbuf+5), length);#endif	SkipHostName(sbuf);	switch(*sbuf){	case '<': /* sbuf contents is a path name with <..> */		++sbuf;		/* skip '<' */		for(i=0; i<(length-6); ++i, ++sbuf, ++dbuf)			*dbuf = *sbuf;			/* 6 -- char size of "{DSK}<" */		*dbuf = '\0';			/* set directory and file name to buf1 */		for(; *dbuf!='>'; --dbuf);#ifdef	DEBUG	printf("DEBUG:setpathname[i after for: %d]\n", i);#endif			/* set directory and file name to buf1 */		*dbuf='\0';			/* separete directory and file name */		filep = dbuf+1;		break;	case '/': /* sbuf contents is a path name without <..> */		for(i=0; i<(length-5); ++i, ++sbuf, ++dbuf)			*dbuf = *sbuf;			/* 5 -- char size of "{DSK}" */		*dbuf = '\0';		for(--dbuf;*dbuf!='/';--dbuf);		*dbuf = '\0';		filep = dbuf+1;		break;	default: /* sbuf contents is a file name */	  		dirlength = getWorkingDirectory(dbuf, MAXPATHLEN);			/* set directory name to dbuf */#ifdef	DEBUG	printf("DEBUG:setpathname[dirlength %d]\n", dirlength);#endif		dbuf += dirlength;		*dbuf = '\0';		++dbuf;		filep = dbuf;		for(i=0; i<(length-5); ++i, ++sbuf, ++dbuf)			*dbuf = *sbuf;			/* set file name to dbuf */			/* 5 -- char size of "{DSK}"	*/		*dbuf = '\0';		break;	}#ifdef	DEBUG	printf("DEBUG:setpathname[file %s]\n", filep);#endif	return(filep);}catpathname(dirp, filep)char	*dirp;char	*filep;	/* pointer to file name */{char	*cp;#ifdef	TRACE	printf("TRACE:catpathname[dir %s\t file %s]\n", dirp, filep);#endif	if(*(filep-2) == '/'){		/* Last char of directory name is '/'.		   Move file name. */		for(cp=filep-1; *filep != '\0'; ++cp, ++filep)			*cp = *filep;		*cp = '\0';	}else{		/* Not move file name. */		*(filep-1) = '/';	}#ifdef	DEBUG	printf("DEBUG:catpathname[pathname %s]\n", dirp);#endif}handle_oldfile(dirp, filep)char	*dirp;char	*filep;	/* pointer to file name */{char	*cp0, *cp1;char	pathname[MAXPATHLEN];char	savename[MAXPATHLEN];	for(cp0=dirp, cp1=pathname;*cp0!='\0';++cp0, ++cp1)		*cp1 = *cp0;	if(*(cp0-1) != '/'){		*cp1++ = '/';	}	/* Last char of directory name is '/'. */	for(cp0=filep; *cp0 != '\0'; ++cp0, ++cp1)		*cp1 = *cp0;	*cp1 = '\0';	for(cp0=pathname, cp1=savename;*cp0!='\0';++cp0, ++cp1)		*cp1 = *cp0;	*cp1++ = '%';	*cp1 = '\0'; 	if(rename(pathname, savename) == -1){		printf("Error: rename[errno %d]\n", errno);		return(NIL);	}	return(ATOM_T);}DSK_openfile(args)LispPTR	*args;/*	args[0]	:	Indirect pointer to FileName	args[1]	:	Pointer to RECOG code	args[2]	:	Pointer to ACCESS code	args[3]	:	Length of FileName  -- no need*/{char	dirp[MAXPATHLEN], *filep;char	*buf2;int	id;	/* return value  of open system call. */int	flags;	/* open system call's argument */int	mode;		/* open system call's argument */int	i;#ifdef TRACE	printf("TRACE:DSK_openfile(%x: %x %x %x %x) \n", args, args[0], args[1], args[2], args[3]);#endif	GetName(args[0], dirp, filep);	if((i = dirSize(dirp)) == -1){		printf("Error: DSK_openfile[Bad directory name %s]\n", dirp);		return(NIL);	}	if((buf2 = malloc(i)) == 0){		printf("Error: DSK_openfile[Bad directory name %s]\n", dirp);		return(NIL);	}	if(readDir(dirp, buf2) == -1){		printf("Error: DSK_openfile[Bad directory name %s]\n", dirp);		free(buf2);		return(NIL);	}	switch(args[1]){	case RECOG_OLD:	case RECOG_OLDEST:			if(searchfile(filep, buf2) == -1){				printf("Error: DSK_openfile[Cannot find file name %s]\n", filep);				free(buf2);				return(NIL);			}			flags = 0;			break;	case RECOG_NEW_OLD:			if(searchfile(filep, buf2) == -1)				flags = O_CREAT;			else				flags = 0;			break;	case RECOG_NEW:			flags = O_CREAT;			break;	default:			free(buf2);			return(NIL);	}	switch(args[2]){	case	ACCESS_INPUT:			flags |= O_RDONLY;			/* mode = 0440; */			mode = 0666;			break;	case	ACCESS_OUTPUT:			flags |= (O_WRONLY | O_CREAT);			/* mode = 0220; */			mode = 0666;			break;	case	ACCESS_BOTH:			flags |= (O_RDWR | O_CREAT);			mode = 0666;			break;	case	ACCESS_APPEND:			flags |= O_APPEND;			mode = 0666;			break;	default:			free(buf2);			return(NIL);	}	catpathname(dirp, filep);	id = open(dirp, flags, mode);#ifdef	DEBUG	printf("Opend FILEID is %d\n", id);#endif	if(id == -1){			printf("Error: DSK_openfile[open system call: can not open %s]\n", dirp);			free(buf2);			return(NIL);	}	free(buf2);	return(GetSmallp(id));}