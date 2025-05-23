#ifndef __SYS_IOCTL_H
#define __SYS_IOCTL_H

#include <features.h>
#ifdef __cplusplus
extern "C" {
#endif

#include <bits/ioctl.h>
#include <bits/ioctls.h>
#include <termios.h>

struct serial_iso7816 {
    unsigned    flags;                  /* ISO7816 feature flags */
#define SER_ISO7816_ENABLED             1
#define SER_ISO7816_T_PARAM             0xf0
#define SER_ISO7816_T(t)                (((t) & 0x0f) << 4)
    unsigned    tg;
    unsigned    sc_fi;
    unsigned    sc_di;
    unsigned    clk;
    unsigned    reserved[5];
};

#define SIOCINQ         FIONREAD
#define SIOCOUTQ        TIOCOUTQ        /* output queue size (not sent + not acked) */
#define SOCK_IOC_TYPE   0x89
#define SIOCGSTAMP_NEW   _IOR(SOCK_IOC_TYPE, 0x06, long long[2])
#define SIOCGSTAMPNS_NEW _IOR(SOCK_IOC_TYPE, 0x07, long long[2])

#ifdef __SIXTY_FOUR
#define SIOCGSTAMP      SIOCGSTAMP_OLD
#define SIOCGSTAMPNS    SIOCGSTAMPNS_OLD
#else
#define SIOCGSTAMP      SIOCGSTAMP_NEW
#define SIOCGSTAMPNS    SIOCGSTAMPNS_NEW
#endif

#define SIOCADDRT       0x890B
#define SIOCDELRT       0x890C
#define SIOCRTMSG       0x890D
#define SIOCGIFNAME     0x8910
#define SIOCSIFLINK     0x8911
#define SIOCGIFCONF     0x8912
#define SIOCGIFFLAGS    0x8913
#define SIOCSIFFLAGS    0x8914
#define SIOCGIFADDR     0x8915
#define SIOCSIFADDR     0x8916
#define SIOCGIFDSTADDR  0x8917
#define SIOCSIFDSTADDR  0x8918
#define SIOCGIFBRDADDR  0x8919
#define SIOCSIFBRDADDR  0x891a
#define SIOCGIFNETMASK  0x891b
#define SIOCSIFNETMASK  0x891c
#define SIOCGIFMETRIC   0x891d
#define SIOCSIFMETRIC   0x891e
#define SIOCGIFMEM      0x891f
#define SIOCSIFMEM      0x8920
#define SIOCGIFMTU      0x8921
#define SIOCSIFMTU      0x8922
#define SIOCSIFNAME     0x8923
#define SIOCSIFHWADDR   0x8924
#define SIOCGIFENCAP    0x8925
#define SIOCSIFENCAP    0x8926
#define SIOCGIFHWADDR   0x8927
#define SIOCGIFSLAVE    0x8929
#define SIOCSIFSLAVE    0x8930
#define SIOCADDMULTI    0x8931
#define SIOCDELMULTI    0x8932
#define SIOCGIFINDEX    0x8933
#define SIOGIFINDEX     SIOCGIFINDEX
#define SIOCSIFPFLAGS   0x8934
#define SIOCGIFPFLAGS   0x8935
#define SIOCDIFADDR     0x8936
#define SIOCSIFHWBROADCAST      0x8937
#define SIOCGIFCOUNT    0x8938
#define SIOCGIFBR       0x8940
#define SIOCSIFBR       0x8941
#define SIOCGIFTXQLEN   0x8942
#define SIOCSIFTXQLEN   0x8943
#define SIOCETHTOOL     0x8946
#define SIOCGMIIPHY     0x8947
#define SIOCGMIIREG     0x8948
#define SIOCSMIIREG     0x8949
#define SIOCWANDEV      0x894A
#define SIOCOUTQNSD     0x894B
#define SIOCGSKNS       0x894C
#define SIOCDARP        0x8953
#define SIOCGARP        0x8954
#define SIOCSARP        0x8955
#define SIOCDRARP       0x8960
#define SIOCGRARP       0x8961
#define SIOCSRARP       0x8962
#define SIOCGIFMAP      0x8970
#define SIOCSIFMAP      0x8971
#define SIOCADDDLCI     0x8980
#define SIOCDELDLCI     0x8981
#define SIOCGIFVLAN     0x8982
#define SIOCSIFVLAN     0x8983
#define SIOCBONDENSLAVE 0x8990
#define SIOCBONDRELEASE 0x8991
#define SIOCBONDSETHWADDR      0x8992
#define SIOCBONDSLAVEINFOQUERY 0x8993
#define SIOCBONDINFOQUERY      0x8994
#define SIOCBONDCHANGEACTIVE   0x8995
#define SIOCBRADDBR     0x89a0
#define SIOCBRDELBR     0x89a1
#define SIOCBRADDIF     0x89a2
#define SIOCBRDELIF     0x89a3
#define SIOCSHWTSTAMP   0x89b0
#define SIOCGHWTSTAMP   0x89b1

int ioctl(int, int, ...);

#ifdef __cplusplus
}
#endif
#endif
