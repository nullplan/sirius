#ifndef __SYS_SYSMACROS_H
#define __SYS_SYSMACROS_H

/* Linux dev_t definition (makes it a 32-bit dev_t).
 *
 * Each letter stands for 4 bits (1 hex digits). M = major, m = minor:
 * mmmMMMmm
 *
 * glibc naturally extens this to 64 bits:
 *
 * MMMMMmmmmmmMMMmm
 *
 * Which allows for a 32-bit major and a 32-bit minor and is backwards compatible to everything older.
 */

#define major(d)    ((((d) >> 8) & 0xfff) | (((d) >> 32) & 0xfffff000))
#define minor(d)    (((d) & 0xff) | (((d) >> 12) & 0xffffff00))
#define makedev(maj,min)    (((min) & 0xff) | ((maj) & 0xfff) << 8 | ((min) & 0xffffff00ull) << 12 | ((maj) & 0xfffff000ull) << 32)
#endif
