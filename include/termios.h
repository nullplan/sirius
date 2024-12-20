#ifndef __TERMIOS_H
#define __TERMIOS_H

#define __NEED_pid_t
#include <alltypes.h>
#include <features.h>
#ifdef __cplusplus
extern "C" {
#endif

struct winsize {
    unsigned short ws_row;
    unsigned short ws_col;
    unsigned short ws_xpixel;
    unsigned short ws_ypixel;
};

typedef unsigned char cc_t;
typedef unsigned speed_t;

/* c_iflag bits */
#define IGNBRK      0x001
#define BRKINT      0x002
#define IGNPAR      0x004
#define PARMRK      0x008
#define INPCK       0x010
#define ISTRIP      0x020
#define INLCR       0x040
#define IGNCR       0x080
#define ICRNL       0x100
#define IXANY       0x200

/* c_oflag bits */
#define OPOST       0x01
#define OCRNL       0x08
#define ONOCR       0x10
#define ONLRET      0x20
#define OFILL       0x40
#define OFDEL       0x80

/* common CBAUD rates */
#define B0          0x00000000
#define B50         0x00000001
#define B75         0x00000002
#define B110        0x00000003
#define B134        0x00000004
#define B150        0x00000005
#define B200        0x00000006
#define B300        0x00000007
#define B600        0x00000008
#define B1200       0x00000009
#define B1800       0x0000000a
#define B2400       0x0000000b
#define B4800       0x0000000c
#define B9600       0x0000000d
#define B19200      0x0000000e
#define B38400      0x0000000f
#define EXTA        B19200
#define EXTB        B38400

#define ADDRB       0x20000000
#define CMSPAR      0x40000000
#define CRTSCTS     0x80000000

#define IBSHIFT     16

/* arguments for TCXONC */
#define TCOOFF      0
#define TCOON       1
#define TCIOFF      2
#define TCION       3

/* arguments for TCFLSH */
#define TCIFLUSH    0
#define TCOFLUSH    1
#define TCIOFLUSH   2

#include <bits/termbits.h>

speed_t cfgetispeed(const struct termios *);
speed_t cfgetospeed(const struct termios *);
int cfsetispeed(struct termios *, speed_t);
int cfsetospeed(struct termios *, speed_t);
int tcdrain(int);
int tcflow(int, int);
int tcflush(int, int);
pid_t tcgetsid(int);
int tcsendbreak(int, int);
int tcgetattr(int, struct termios *);
int tcsetattr(int, int, struct termios *);
int tcgetwinsize(int, struct winsize *);
int tcsetwinsize(int, const struct winsize *);
#ifdef __cplusplus
}
#endif
#endif
