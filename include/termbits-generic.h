typedef unsigned tcflag_t;
#define NCCS    19

struct termios {
    tcflag_t    c_iflag;
    tcflag_t    c_oflag;
    tcflag_t    c_cflag;
    tcflag_t    c_lflag;
    cc_t        c_line;
    cc_t        c_cc[NCCS];
    speed_t     c_ispeed;
    speed_t     c_ospeed;
};

/* c_cc indices */
#define VINTR       0
#define VQUIT       1
#define VERASE      2
#define VKILL       3
#define VEOF        4
#define VTIME       5
#define VMIN        6
#define VSWTC       7
#define VSTART      8
#define VSTOP       9
#define VSUSP       10
#define VEOL        11
#define VREPRINT    12
#define VDISCARD    13
#define VWERASE     14
#define VLNEXT      15
#define VEOL2       16

#define IUCLC       0x0200
#define IXON        0x0400
#define IXOFF       0x0800
#define IMAXBEL     0x1000
#define IUTF8       0x2000

#define OLCUC       0x0002
#define ONLCR       0x0004
#define NLDLY       0x0100
#define   NL0       0x0000
#define   NL1       0x0100
#define CRDLY       0x0600
#define   CR0       0x0000
#define   CR1       0x0200
#define   CR2       0x0400
#define   CR3       0x0600
#define TABDLY      0x1800
#define   TAB0      0x0000
#define   TAB1      0x0800
#define   TAB2      0x1000
#define   TAB3      0x1800
#define XTABS TAB3
#define BSDLY       0x2000
#define   BS0       0x0000
#define   BS1       0x2000
#define VTDLY       0x4000
#define   VT0       0x0000
#define   VT1       0x4000
#define FFDLY       0x8000
#define   FF0       0x0000
#define   FF1       0x8000

#define CBAUD       0x0000100f
#define CBAUDEX     0x00001000
#define   BOTHER    0x00001000
#define   B57600    0x00001001
#define  B115200    0x00001002
#define  B230400    0x00001003
#define  B460800    0x00001004
#define  B500000    0x00001005
#define  B576000    0x00001006
#define  B921600    0x00001007
#define B1000000    0x00001008
#define B1152000    0x00001009
#define B1500000    0x0000100a
#define B2000000    0x0000100b
#define B2500000    0x0000100c
#define B3000000    0x0000100d
#define B3500000    0x0000100e
#define B4000000    0x0000100f

#define CSIZE   0x30
#define   CS5   0x00
#define   CS6   0x10
#define   CS7   0x20
#define   CS8   0x30
#define CSTOPB  0x40
#define CREAD   0x80
#define PARENB  0x1000
#define PARODD  0x2000
#define HUPCL   0x4000
#define CLOCAL  0x8000
#define CIBAUD  0x100f0000

#define ECHOKE  0x00000001
#define ECHOE   0x00000002
#define ECHOK   0x00000004
#define ECHO    0x00000008
#define ECHONL  0x00000010
#define ECHOPRT 0x00000020
#define ECHOCTL 0x00000040
#define ISIG    0x00000080
#define ICANON  0x00000100
#define IEXTEN  0x00000400
#define XCASE   0x00004000
#define TOSTOP  0x00400000
#define FLUSHO  0x00800000
#define EXTPROC 0x10000000
#define PENDIN  0x20000000
#define NOFLSH  0x80000000

#define TCSANOW 0
#define TCSADRAIN 1
#define TCSAFLUSH 2
