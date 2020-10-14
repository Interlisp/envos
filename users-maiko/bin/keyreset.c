#include <stdio.h>#include <sys/types.h>#include <fcntl.h>#include <sundev/kbd.h>#include <sundev/kbio.h>#include <sys/ioctl.h>/* Unshifted keyboard table for Sun-3 keyboard */#define     c(char)   (char&0x1F)static u_char SUNNormalKeyMap[128] = {     /*  0 */    HOLE,     BUCKYBITS+SYSTEMBIT,                           HOLE,     LF(2),    HOLE,     TF(1),TF(2),HOLE,     /*  8 */    TF(3),    HOLE,     TF(4),    HOLE,     TF(5),HOLE,TF(6),HOLE,     /* 16 */    TF(7),    TF(8),    TF(9),    ALT, HOLE,     RF(1),RF(2),RF(3),     /* 24 */    HOLE,     LF(3),    LF(4),    HOLE,     HOLE,c('['),'1','2',     /* 32 */    '3', '4', '5', '6', '7', '8', '9', '0',     /* 40 */    '-', '=', '`', '\b',     HOLE,     RF(4),    RF(5),RF(6),     /* 48 */    HOLE,     LF(5),    HOLE,     LF(6),    HOLE,'\t','q','w',     /* 56 */    'e', 'r', 't', 'y', 'u', 'i', 'o', 'p',     /* 64 */    '[', ']', 0x7F,     HOLE,     RF(7),    STRING+UPARROW,                                               RF(9),    HOLE,     /* 72 */    LF(7),    LF(8),    LF(14),   HOLE,     SHIFTKEYS+LEFTCTRL,                                          'a',      's', 'd',     /* 80 */    'f', 'g', 'h', 'j', 'k', 'l', ';', '\'',     /* 88 */    '\\',     '\r',     HOLE,     STRING+LEFTARROW,                 		RF(11),   STRING+RIGHTARROW, HOLE,     LF(9),     /* 96 */    LF(15),   LF(10),   HOLE,     SHIFTKEYS+LEFTSHIFT,                                     'z', 'x', 'c', 'v',     /*104 */    'b', 'n', 'm', ',', '.', '/', SHIFTKEYS+RIGHTSHIFT,                                                    '\n',     /*112 */    RF(13),   STRING+DOWNARROW, RF(15),   HOLE,     HOLE,      			    HOLE,  HOLE,  SHIFTKEYS+CAPSLOCK,     /*120 */    BUCKYBITS+METABIT, ' ', BUCKYBITS+METABIT,                                HOLE,     HOLE,     HOLE,     ERROR,IDLE,     };/* Shifted keyboard table for Sun-3 keyboard */static u_char SUNShiftKeyMap[] = {     /*  0 */    HOLE,     BUCKYBITS+SYSTEMBIT,                           HOLE,     LF(2),    HOLE,     TF(1),TF(2),HOLE,     /*  8 */    TF(3),    HOLE,     TF(4),    HOLE,     TF(5),HOLE,TF(6),HOLE,     /* 16 */    TF(7),    TF(8),    TF(9),    ALT, HOLE,     RF(1),RF(2),RF(3),     /* 24 */    HOLE,     LF(3),    LF(4),    HOLE,     HOLE,c('['),'!',    '@',     /* 32 */    '#', '$', '%', '^', '&', '*', '(', ')',     /* 40 */    '_', '+', '~', '\b',     HOLE,     RF(4),    RF(5),RF(6),     /* 48 */    HOLE,     LF(5),    HOLE,     LF(6),    HOLE,'\t','Q','W',     /* 56 */    'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P',     /* 64 */    '{', '}', 0x7F,     HOLE,     RF(7),    STRING+UPARROW,                                               RF(9),    HOLE,     /* 72 */    LF(7),    LF(8),    HOLE,     HOLE,     SHIFTKEYS+LEFTCTRL,                                          'A',      'S', 'D',     /* 80 */    'F', 'G', 'H', 'J', 'K', 'L', ':', '"',     /* 88 */    '|', '\r',     HOLE,     STRING+LEFTARROW,                                     RF(11),   STRING+RIGHTARROW,                                               HOLE,     LF(9),     /* 96 */    LF(15),   LF(10),   HOLE,     SHIFTKEYS+LEFTSHIFT,                                     'Z', 'X', 'C', 'V',     /*104 */    'B', 'N', 'M', '<', '>', '?', SHIFTKEYS+RIGHTSHIFT, '\n',     /*112 */    RF(13),   STRING+DOWNARROW,    RF(15),   HOLE,     HOLE,      			    HOLE,HOLE,SHIFTKEYS+CAPSLOCK,     /*120 */    BUCKYBITS+METABIT,  ' ', BUCKYBITS+METABIT,                                HOLE,     HOLE,     HOLE,     ERROR,IDLE,     };/* Caps Locked keyboard table for Sun-3 keyboard */static u_char SUNCapsKeyMap[] = {     /*  0 */    HOLE,     BUCKYBITS+SYSTEMBIT,                           HOLE,     LF(2),    HOLE,     TF(1),TF(2),HOLE,     /*  8 */    TF(3),    HOLE,     TF(4),    HOLE,     TF(5),HOLE,TF(6),HOLE,     /* 16 */    TF(7),    TF(8),    TF(9),    ALT, HOLE,     RF(1),RF(2),RF(3),     /* 24 */    HOLE,     LF(3),    LF(4),    HOLE,     HOLE,c('['),'1','2',     /* 32 */    '3', '4', '5', '6', '7', '8', '9', '0',     /* 40 */    '-', '=', '`', '\b',     HOLE,     RF(4),    RF(5),RF(6),     /* 48 */    HOLE,     LF(5),    HOLE,     LF(6),    HOLE,'\t','Q','W',     /* 56 */    'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P',     /* 64 */    '[', ']', 0x7F,     HOLE,     RF(7),    STRING+UPARROW,                                               RF(9),    HOLE,     /* 72 */    LF(7),    LF(8),    HOLE,     HOLE,     SHIFTKEYS+LEFTCTRL,                                          'A',      'S', 'D',     /* 80 */    'F', 'G', 'H', 'J', 'K', 'L', ';', '\'',     /* 88 */    '\\',     '\r',     HOLE,     STRING+LEFTARROW,    RF(11),     		   STRING+RIGHTARROW,  HOLE,     LF(9),     /* 96 */    LF(15),   LF(10),   HOLE,     SHIFTKEYS+LEFTSHIFT,                                     'Z', 'X', 'C', 'V',     /*104 */    'B', 'N', 'M', ',', '.', '/', SHIFTKEYS+RIGHTSHIFT,  '\n',     /*112 */    RF(13),   STRING+DOWNARROW,                           RF(15),   HOLE,     HOLE,     HOLE,HOLE,SHIFTKEYS+CAPSLOCK,     /*120 */    BUCKYBITS+METABIT, ' ', BUCKYBITS+METABIT,                                HOLE,     HOLE,     HOLE,     ERROR,IDLE,     };/* Controlled keyboard table for Sun-3 keyboard */static u_char SUNCtrlKeyMap[] = {     /*  0 */    HOLE,     BUCKYBITS+SYSTEMBIT,                           HOLE,     LF(2),    HOLE,     TF(1),TF(2),HOLE,     /*  8 */    TF(3),    HOLE,     TF(4),    HOLE,     TF(5),HOLE,TF(6),HOLE,     /* 16 */    TF(7),    TF(8),    TF(9),    ALT, HOLE,     RF(1),RF(2),RF(3),     /* 24 */    HOLE,     LF(3),    LF(4),    HOLE,     HOLE,c('['),'1',c('@'),     /* 32 */    '3', '4', '5', c('^'),   '7', '8', '9', '0',     /* 40 */    c('_'),   '=', c('^'),   '\b',     HOLE,     RF(4),RF(5),RF(6),     /* 48 */    HOLE,     LF(5),    HOLE,     LF(6),    HOLE,'\t',   c('q'),c('w'),     /* 56 */    c('e'),   c('r'),   c('t'),   c('y'),   c('u'),c('i'),c('o'),c('p'),     /* 64 */    c('['),   c(']'),   0x7F,     HOLE,     RF(7),STRING+UPARROW,                                               RF(9),    HOLE,     /* 72 */    LF(7),    LF(8),    HOLE,     HOLE,     SHIFTKEYS+LEFTCTRL,                                          c('a'),   c('s'),   c('d'),     /* 80 */    c('f'),   c('g'),   c('h'),   c('j'),   c('k'),c('l'),';','\'',     /* 88 */    c('\\'), '\r',     HOLE,     STRING+LEFTARROW,                   RF(11),   STRING+RIGHTARROW,   HOLE,     LF(9),     /* 96 */    LF(15),   LF(10),   HOLE,     SHIFTKEYS+LEFTSHIFT,                                     c('z'),   c('x'),   c('c'),c('v'),     /*104 */    c('b'),   c('n'),   c('m'),   ',', '.', c('_'),SHIFTKEYS+RIGHTSHIFT,                                                    '\n',     /*112 */    RF(13),   STRING+DOWNARROW, RF(15),   HOLE,     HOLE,       		   HOLE,HOLE,SHIFTKEYS+CAPSLOCK,     /*120 */    BUCKYBITS+METABIT,   c(' '),   BUCKYBITS+METABIT,                                HOLE,     HOLE,     HOLE,     ERROR,IDLE,     };/* "Key Up" keyboard table for Sun-3 keyboard */static SUNUpKeyMap[] = {     /*  0 */    HOLE,     BUCKYBITS+SYSTEMBIT,                           HOLE,     OOPS,     HOLE,     OOPS,OOPS,HOLE,     /*  8 */    OOPS,     HOLE,     OOPS,     HOLE,     OOPS,HOLE,OOPS,HOLE,     /* 16 */    OOPS,     OOPS,     OOPS,     OOPS,     HOLE,OOPS,OOPS,NOP,     /* 24 */    HOLE,     OOPS,     OOPS,     HOLE,     HOLE,NOP,NOP,NOP,     /* 32 */    NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP,     /* 40 */    NOP, NOP, NOP, NOP, HOLE,     OOPS,     OOPS,NOP,     /* 48 */    HOLE,     OOPS,     HOLE,     OOPS,     HOLE,NOP,NOP,NOP,     /* 56 */    NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP,     /* 64 */    NOP, NOP, NOP, HOLE,     OOPS,     OOPS,     NOP,HOLE,     /* 72 */    OOPS,     OOPS,     HOLE,     HOLE,     SHIFTKEYS+LEFTCTRL,                                          NOP,      NOP, NOP,     /* 80 */    NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP,     /* 88 */    NOP, NOP, HOLE,     OOPS,     OOPS,     NOP, HOLE,OOPS,     /* 96 */    OOPS,     OOPS,     HOLE,     SHIFTKEYS+LEFTSHIFT,                                     NOP, NOP, NOP, NOP,     /*104 */    NOP, NOP, NOP, NOP, NOP, NOP, SHIFTKEYS+RIGHTSHIFT,                                                    NOP,     /*112 */    OOPS,     OOPS,     NOP, HOLE,     HOLE,     HOLE,HOLE,NOP,     /*120 */    BUCKYBITS+METABIT, NOP, BUCKYBITS+METABIT,                                HOLE,     HOLE,     HOLE,     HOLE,RESET,     };#define SUN3KEYENTRIES	128extern int errno;main(){	set_sun3_keymap();}set_sun3_keymap(){	int kbfd;	/* Keyboard file descriptor */	kbfd = open( "/dev/kbd", O_RDONLY, 0600 );	if( kbfd == NULL ){		fprintf( stderr,"can't open /dev/kbd \n" );		exit( -1 );	} 	set_keytable( kbfd, 0, SUNNormalKeyMap ); 	set_keytable( kbfd, CAPSMASK, SUNCapsKeyMap ); 	set_keytable( kbfd, SHIFTMASK, SUNShiftKeyMap ); 	set_keytable( kbfd, CTRLMASK, SUNCtrlKeyMap );		close(kbfd);}set_keytable( kbfd, mode, keytable )int kbfd;		/* keyboard file discriptor	*/int mode;		/* one of 0, CAPSMASK, SHIFTMASK, CTRLMASK	*/u_char keytable[];	/* keytable */{	int loc;	for( loc = 0; loc < SUN3KEYENTRIES; loc++ )		set_keyaction( kbfd, mode, loc, keytable[loc] );}set_keyaction( kbfd, mode, keyloc, keycode )int kbfd;		/* keyboard file discriptor	*/int mode;		/* one of 0, CAPSMASK, SHIFTMASK, CTRLMASK	*/int keyloc;		/* physical key location	*/u_char keycode;{	struct kiockey keyinfo;	keyinfo.kio_tablemask = mode;	keyinfo.kio_station = keyloc;	keyinfo.kio_entry = keycode;	if( ioctl( kbfd, KIOCSETKEY, (char *)&keyinfo ) == -1){		fprintf( stderr, "set_keyaction: errno = %d\n", errno );		exit( -1 );	}}