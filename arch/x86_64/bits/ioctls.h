#define TCGETA		0x5405
#define TCSETA		0x5406
#define TCSETAW		0x5407
#define TCSETAF		0x5408
#define TCSBRK		0x5409
#define TCXONC		0x540A
#define TCFLSH		0x540B
#define TIOCEXCL	0x540C
#define TIOCNXCL	0x540D
#define TIOCSCTTY	0x540E
#define TIOCGPGRP	0x540F
#define TIOCSPGRP	0x5410
#define TIOCOUTQ	0x5411
#define TIOCSTI		0x5412
#define TIOCGWINSZ	0x5413
#define TIOCSWINSZ	0x5414
#define TIOCMGET	0x5415
#define TIOCMBIS	0x5416
#define TIOCMBIC	0x5417
#define TIOCMSET	0x5418
#define TIOCGSOFTCAR	0x5419
#define TIOCSSOFTCAR	0x541A
#define FIONREAD	0x541B
#define TIOCINQ		FIONREAD
#define TIOCLINUX	0x541C
#define TIOCCONS	0x541D
#define TIOCGSERIAL	0x541E
#define TIOCSSERIAL	0x541F
#define TIOCPKT		0x5420
#define FIONBIO		0x5421
#define TIOCNOTTY	0x5422
#define TIOCSETD	0x5423
#define TIOCGETD	0x5424
#define TCSBRKP		0x5425	/* Needed for POSIX tcsendbreak() */
#define TIOCSBRK	0x5427  /* BSD compatibility */
#define TIOCCBRK	0x5428  /* BSD compatibility */
#define TIOCGSID	0x5429  /* Return the session ID of FD */
#define TCGETS		_IOR('T', 0x2A, struct termios)
#define TCSETS		_IOW('T', 0x2B, struct termios)
#define TCSETSW		_IOW('T', 0x2C, struct termios)
#define TCSETSF		_IOW('T', 0x2D, struct termios)
#define TIOCGRS485	0x542E
#define TIOCSRS485	0x542F
#define TIOCGPTN	_IOR('T', 0x30, unsigned int) /* Get Pty Number (of pty-mux device) */
#define TIOCSPTLCK	_IOW('T', 0x31, int)  /* Lock/unlock Pty */
#define TIOCGDEV	_IOR('T', 0x32, unsigned int) /* Get primary device node of /dev/console */
#define TCGETX		0x5432 /* SYS5 TCGETX compatibility */
#define TCSETX		0x5433
#define TCSETXF		0x5434
#define TCSETXW		0x5435
#define TIOCSIG		_IOW('T', 0x36, int)  /* pty: generate signal */
#define TIOCVHANGUP	0x5437
#define TIOCGPKT	_IOR('T', 0x38, int) /* Get packet mode state */
#define TIOCGPTLCK	_IOR('T', 0x39, int) /* Get Pty lock state */
#define TIOCGEXCL	_IOR('T', 0x40, int) /* Get exclusive mode state */
#define TIOCGPTPEER	_IO('T', 0x41) /* Safely open the slave */
#define TIOCGISO7816	_IOR('T', 0x42, struct serial_iso7816)
#define TIOCSISO7816	_IOWR('T', 0x43, struct serial_iso7816)
#define FIONCLEX	0x5450
#define FIOCLEX		0x5451

#define TIOCM_LE	0x001
#define TIOCM_DTR	0x002
#define TIOCM_RTS	0x004
#define TIOCM_ST	0x008
#define TIOCM_SR	0x010
#define TIOCM_CTS	0x020
#define TIOCM_CAR	0x040
#define TIOCM_RNG	0x080
#define TIOCM_DSR	0x100
#define TIOCM_CD	TIOCM_CAR
#define TIOCM_RI	TIOCM_RNG
#define TIOCM_OUT1	0x2000
#define TIOCM_OUT2	0x4000
#define TIOCM_LOOP	0x8000


#define FIOSETOWN	0x8901
#define SIOCSPGRP	0x8902
#define FIOGETOWN	0x8903
#define SIOCGPGRP	0x8904
#define SIOCATMARK	0x8905
#define SIOCGSTAMP_OLD	0x8906		/* Get stamp (timeval) */
#define SIOCGSTAMPNS_OLD 0x8907		/* Get stamp (timespec) */
