h04870s 00000/00000/00029d D 1.3 92/04/21 17:14:51 sybalsky 3 2c shortening file names for DOS \nes 00027/00000/00002d D 1.2 92/03/04 19:47:32 nilsson 2 1c The modus operanid of this file is not fully understood.es 00002/00000/00000d D 1.1 92/03/04 19:44:50 nilsson 1 0c date and time created 92/03/04 19:44:50 by nilssoneuUf e 0tTI 1/* %Z% %M% Version %I% (%G%). copyright venue & Fuji Xerox  */static char *id = "%Z% %M%	%I% %G%	(venue & Fuji Xerox)";I 2/* These functions are created so that you can split a float into *//* four integers. The general idea behind these functions is to */ /* act as a caster between different entitys on the stack */int pickapart1(i1, i2, i3, i4)     int i1, i2, i3, i4;{  return(i1);}int pickapart2(i1, i2, i3, i4)     int i1, i2, i3, i4;{  return(i2);}int pickapart3(i1, i2, i3, i4)     int i1, i2, i3, i4;{  return(i3);}int pickapart4(i1, i2, i3, i4)     int i1, i2, i3, i4;{  return(i4);}E 2E 1