typedef unsigned tcflag_t;
#define NCCS    19

struct termios {
    tcflag_t    c_iflag;
    tcflag_t    c_oflag;
    tcflag_t    c_cflag;
    tcflag_t    c_lflag;
    cc_t        c_cc[NCCS];
    cc_t        c_line;
    speed_t     c_ispeed;
    speed_t     c_ospeed;
};

/* c_cc indices */
#define VINTR       0
#define VQUIT       1
#define VERASE      2
#define VKILL       3
#define VEOF        4
#define VMIN        5
#define VEOL        6
#define VTIME       7
#define VEOL2       8
#define VSWTC       9
#define VWERASE     10
#define VREPRINT    11
#define VSUSP       12
#define VSTART      13
#define VSTOP       14
#define VLNEXT      15
#define VDISCARD    16

#define IXON        0x0200
#define IXOFF       0x0400
#define IUCLC       0x0800
#define IMAXBEL     0x1000
#define IUTF8       0x2000

#define ONLCR       0x0002
#define OLCUC       0x0004
#define NLDLY       0x0300
#define   NL0       0x0000
#define   NL1       0x0100
#define   NL2       0x0200
#define   NL3       0x0300
#define TABDLY      0x0c00
#define   TAB0      0x0000
#define   TAB1      0x0400
#define   TAB2      0x0800
#define   TAB3      0x0c00
#define XTABS TAB3
#define CRDLY       0x3000
#define   CR0       0x0000
#define   CR1       0x1000
#define   CR2       0x2000
#define   CR3       0x3000
#define FFDLY       0x4000
#define   FF0       0x0000
#define   FF1       0x4000
#define BSDLY       0x8000
#define   BS0       0x0000
#define   BS1       0x8000
#define VTDLY       0x10000
#define   VT0       0x00000
#define   VT1       0x10000

#define CBAUD       0x000000ff
#define CBAUDEX     0x00000000
#define   B57600    0x00000010
#define  B115200    0x00000011
#define  B230400    0x00000012
#define  B460800    0x00000013
#define  B500000    0x00000014
#define  B576000    0x00000015
#define  B921600    0x00000016
#define B1000000    0x00000017
#define B1152000    0x00000018
#define B1500000    0x00000019
#define B2000000    0x0000001a
#define B2500000    0x0000001b
#define B3000000    0x0000001c
#define B3500000    0x0000001d
#define B4000000    0x0000001e
#define BOTHER      0x0000001f


#define CSIZE   0x300
#define   CS5   0x000
#define   CS6   0x100
#define   CS7   0x200
#define   CS8   0x300
#define CSTOPB  0x400
#define CREAD   0x800
#define PARENB  0x100
#define PARODD  0x200
#define HUPCL   0x400
#define CLOCAL  0x800
#define CIBAUD  0x00ff0000

#define ISIG    0x00000001
#define ICANON  0x00000002
#define XCASE   0x00000004
#define ECHO    0x00000008
#define ECHOE   0x00000010
#define ECHOK   0x00000020
#define ECHONL  0x00000040
#define NOFLSH  0x00000080
#define TOSTOP  0x00000100
#define ECHOCTL 0x00000200
#define ECHOPRT 0x00000400
#define ECHOKE  0x00000800
#define FLUSHO  0x00001000
#define PENDIN  0x00004000
#define IEXTEN  0x00008000
#define EXTPROC 0x00010000

#define TCSANOW 0
#define TCSADRAIN 1
#define TCSAFLUSH 2
