h54772s 00001/00000/00270d D 1.3 88/03/24 15:13:41 hayata 3 2c For Testes 00002/00002/00268d D 1.2 88/03/13 14:46:41 hayata 2 1c Add SCCS key id (%Z%)es 00270/00000/00000d D 1.1 88/02/24 16:56:44 hayata 1 0c date and time created 88/02/24 16:56:44 by hayataeuUtTI 1D 2/* This is G-file %M% Version %I% (%G%). copyright Xerox & Fuji Xerox  */static char *id = "%M%	%I% %G%";E 2I 2/* This is G-file %Z% %M% Version %I% (%G%). copyright Xerox & Fuji Xerox  */static char *id = "%Z% %M%	%I% %G%";I 3/* For Test */E 3E 2/* *	Copyright (C) 1987 by Fuji Xerox Co., Ltd. All rights reserved. * *	Auther	:	Hiroshi Hayata	 */#include	<sys/param.h>#include	<sys/file.h>#include	<sys/types.h>#include	<sys/stat.h>#include	<sys/time.h>#include	<errno.h>#include	"lispemul.h"#include	"lispmap.h"#include	"address68k.h"#include	"lisptypes.h"#include	"arith.h"#include	"filesystem.h"#define	SIZE	S_POSITIVE | 1	/* For getfileinfo */#define	WDATE	S_POSITIVE | 2#define	RDATE	S_POSITIVE | 3#define	AUTHOR	S_POSITIVE | 5#define	ToLispTime(x)	((int)x + 29969152 - 61200)			/* For fetfileinfo. For WDATE&RDATE */			/* 29969152 == (timer.c)LISP_UNIX_TIME_DIFF */			/* 61200 == 17hours */extern	int	errno;extern	DLword	*Lisp_world;extern	DLword	*MDStypetbl;	extern	char	*setpathname();UFS_directorynamep(args)LispPTR	*args;/*	args[0]:	Indirect pointer to file name to check. 			Assume contents is a path name (ex {UFS}</usr/fx/hayata/>) 	args[1]:	Length of file name.    -- No need*/{char	buf1[MAXPATHLEN];char	*filep;#ifdef TRACE	printf("TRACE:UFS_directorynamep(%x) \n", args[0]);#endif	GetName(args[0], buf1, filep);	if(dirSize(buf1) == -1){		return(NIL);	}	return(ATOM_T);}UFS_getfileinfo(args)LispPTR	*args;/*	args[0]:	Indirect pointer to file name.	args[1]:	Length of file name	args[2]:	Attiribute code	args[3]:	Indirect pointer to buffer to store the string data			or pointer to buffer to store the FXIP data		-- No need*/{LispPTR	*p0;char	*filep;char	buf0[MAXPATHLEN];struct stat sbuf;char	*bufstr;int	*buffixp;int	i;#ifdef	TRACE	printf("TRACE:UFS_getfileinfo(%x)\n", args[0]);#endif	GetName(args[0], buf0, filep);	catpathname(buf0, filep);#ifdef	DEBUG	printf("DEBUG:UFS_getfileinfo(%s)\n", buf0);#endif		if(stat(buf0, &sbuf) != 0){#ifdef	DEBUG		printf("Error: stat [errno %d]\n", errno);#endif		return(NIL);	}	switch(args[2]){	case SIZE:		buffixp = (int *)(Addr68k_from_LADDR(args[3]));		*buffixp = sbuf.st_size;		return(ATOM_T);	case WDATE:		buffixp = (int *)(Addr68k_from_LADDR(args[3]));		*buffixp = ToLispTime(sbuf.st_mtime);		return(ATOM_T);	case RDATE:		buffixp = (int *)(Addr68k_from_LADDR(args[3]));		*buffixp= ToLispTime(sbuf.st_atime);		return(ATOM_T);	case AUTHOR:		p0 = (LispPTR *)(Addr68k_from_LADDR(args[3]));		bufstr = (char *)(Addr68k_from_LADDR(*p0));		getpw(sbuf.st_uid, bufstr);		for(i=1;(*bufstr)!=':';++i, ++bufstr);		return(GetSmallp(i-1));	default:		return(NIL);	}}	UFS_dirsize(args)LispPTR	*args;/*	args[0]:	Lisp pointer to directory.			(ex)	/usr/aig/lispg/hayata/	args[1]:	Length of directory name.	-- No need	args[2]:	Lisp pointer to filter.			(ex)	abc*d/test*.c	args[3]:	Length of filter name.	-- No need*/{char	*cp;char	bufdir[MAXPATHLEN], buffil[MAXPATHLEN];int	size;int	i;#ifdef	TRACE	printf("TRACE:UFS_dirsize(%x)\n", args[0]);#endif/*	GetNameSize(args[0], size, cp); */	{		int	scratch;		LispPTR	*naddress;		scratch = GetTypeNumber(args[0]);		switch(scratch){					case TYPE_STRINGP:						naddress = (LispPTR *)(Addr68k_from_LADDR(args[0]));			cp = (char *)(Addr68k_from_LADDR(((STRINGP *)naddress)->base));			size = ((STRINGP *)naddress)->length;			break;						case TYPE_ONED_ARRAY:					case TYPE_GENERAL_ARRAY:					naddress = (LispPTR *)(Addr68k_from_LADDR(args[0]));			cp = (char *)(Addr68k_from_LADDR(((ONEDARRAAY *)naddress)->base));			size = ((ONEDARRAAY *)naddress)->totalsize;				break;						case TYPE_LITATOM:						naddress = (LispPTR *)(Addr68k_from_LADDR((PNP_OFFSET + ((0xFFFF & args[0]) << 1))));			cp = (char *)(Addr68k_from_LADDR(0xFFFFFF & (*naddress)));			size = (int)*cp;					++cp;							break;						default:							printf("Bad Type Number %d\n", scratch);			return(NIL);					} } 	SkipHostName(cp);	switch(*cp){	case '<':	/* buf0 contents is a path name */		++cp;		/* skip '<' */		for(i=0; i<size;++i, ++cp)			bufdir[i] = *cp;		bufdir[i] = '\0';		break;	case '/':		for(i=0; i<size;++i, ++cp)			bufdir[i] = *cp;		bufdir[i] = '\0';		break;	default:	/* contents of cp is "{DSK}" */		i = getWorkingDirectory(bufdir, MAXPATHLEN);			/* set directory name to dbuf */		bufdir[i] = '\0';		break;	}#ifdef DEBUG	printf("dirSize: %s\n", bufdir);#endif	GetNameSize(args[2], size, cp);	for(i=0;i<size;++i, ++cp)		buffil[i] = *cp;	buffil[i] = '\0';#ifdef DEBUG	printf("dirSize: %s\n", buffil);#endif	size = alldirSize(bufdir, "", 0, buffil);	if(size == -1)		return(NIL);	else		return(GetSmallp(size));}UFS_readdir(args)LispPTR	*args;/*	args[0]:	Lisp pointer to directory.			(ex)	/usr/aig/lispg/hayata/	args[1]:	Length of directory.			-- No need	args[2]:	Lisp pointer to filter.			(ex)	abc*d/test*.c	args[3]:	Length of filter name.			-- No need	args[4]:	Indirect pointer to buffer.			Buffer size == UFS_dirsize() + 1 */{LispPTR	*p0;char	*cp;char	buf_dir[MAXPATHLEN], buf_fil[MAXPATHLEN];char	*buf_names;int	size;int	i;short	type;#ifdef	TRACE	printf("TRACE:UFS_readdir(%x)\n", args[0]);#endif	GetNameSize(args[0], size, cp);	SkipHostName(cp);	switch(*cp){	case '<':	/* buf0 contents is a path name with <..>.. */		++cp;		/* skip '<' */		for(i=0; i<size;++i, ++cp)			buf_dir[i] = *cp;		buf_dir[i] = '\0';		break;	case '/':	/* buf0 contents is a path name without <..>.. */		for(i=0; i<size;++i, ++cp)			buf_dir[i] = *cp;		buf_dir[i] = '\0';		break;	default:	/* contents of cp is "{DSK}" */		i = getWorkingDirectory(buf_dir, MAXPATHLEN);			/* set directory name to dbuf */		buf_dir[i] = '\0';		break;	}#ifdef	DEBUG	printf("UFS_readdir: buf_dir %s\n",buf_dir);#endif	GetNameSize(args[2], size, cp);	for(i=0;i<size;++i, ++cp)		buf_fil[i] = *cp;	buf_fil[i] = '\0';	p0 = (LispPTR *)(Addr68k_from_LADDR(args[4]));	buf_names = (char *)(Addr68k_from_LADDR(*p0));	size = readAllDir(buf_dir, "", 0, buf_fil, buf_names);	if(size == -1)		return(NIL);	else		return(GetSmallp(size));}UFS_currentdir(args)LispPTR	*args;/*	args[0]	:	Indirect pointer to buffer */{LispPTR	*p0;char	*buf;int	size;	p0 = (LispPTR *)(Addr68k_from_LADDR(args[0]));	buf = (char *)(Addr68k_from_LADDR(((STRINGP *)p0)->base));	size = getWorkingDirectory(buf, ((STRINGP *)p0)->length);/*	buf[size] = '\0';*/	return(GetSmallp(size));}E 1