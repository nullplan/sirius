#if __ARM_ARCH < 5 && defined __thumb__
.thumb
.section ".text.__thumb_bx_r0","ax",%progbits
.global __thumb_bx_r0
.type __thumb_bx_r0, %function
__thumb_bx_r0: bx r0

.section ".text.__thumb_bx_r1","ax",%progbits
.global __thumb_bx_r1
.type __thumb_bx_r1, %function
__thumb_bx_r1: bx r1

.section ".text.__thumb_bx_r2","ax",%progbits
.global __thumb_bx_r2
.type __thumb_bx_r2, %function
__thumb_bx_r2: bx r2

.section ".text.__thumb_bx_r3","ax",%progbits
.global __thumb_bx_r3
.type __thumb_bx_r3, %function
__thumb_bx_r3: bx r3
#endif
