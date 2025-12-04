/* none of this is needed for armv7 */
#if __ARM_ARCH < 7
/* without Thumb2, everything following must be assembled in ARM mode */
/* otherwise I leave it up to settings */
#ifndef __thumb2__
.arm
#endif
@ set compiler to newest machine, but mark file as supporting the oldest
.arch armv7-a
.eabi_attribute Tag_CPU_arch, 2
.eabi_attribute Tag_FP_arch, 0 @ GCC otherwise puts a VFPv4 requirement into this file. I don't know why.
.syntax unified

.section ".data","aw",%progbits
.balign 4
.global __a_barrier_ptr
.hidden __a_barrier_ptr
.type __a_cas_ptr, %object
__a_barrier_ptr: .long barrier_dummy
.size __a_barrier_ptr, . - __a_barrier_ptr
.global __a_cas_ptr
.hidden __a_cas_ptr
.type __a_cas_ptr, %object
__a_cas_ptr: .long cas_dummy
.size __a_cas_ptr, . -__a_cas_ptr
.global __a_gettp_ptr
.hidden __a_gettp_ptr
.type __a_gettp_ptr, %object
__a_gettp_ptr: .long 0
.size __a_gettp_ptr, . - __a_gettp_ptr

.section ".text","ax",%progbits
.type barrier_dummy, %function
barrier_dummy:
    bx lr

@ barrier instruction for old kuser versions that don't have it yet in the helpers
@ must not modify any regs
.global __a_barrier_old
.hidden __a_barrier_old
.type __a_barrier_old, %function
__a_barrier_old:
    push {r0, r1, r2, r3, ip, lr}
    mov r0, #0
    mov r1, #0
    mov r2, sp
    ldr ip, =0xffff0fc0
#if __ARM_ARCH < 5
    mov lr,pc
    bx ip
#else
    blx ip
#endif
    pop {r0, r1, r2, r3, ip, lr}
    bx lr
.size __a_barrier_old, . - __a_barrier_old

.global __a_barrier_v6
.hidden __a_barrier_v6
.type __a_barrier_v6, %function
__a_barrier_v6:
    mcr p15, 0, r0, c7, c10, 5
    bx lr
.size __a_barrier_v6, . - __a_barrier_v6

.global __a_barrier_v7
.hidden __a_barrier_v7
.type __a_barrier_v7, %function
__a_barrier_v7:
    dmb ish
    bx lr
.size __a_barrier_v7, . - __a_barrier_v7


@ all CAS implementations must follow the kuser calling convention:
@ input: r0 = oldval, r1 = newval, r2 = ptr
@ output: r0 = 0 iff successful
@ clobbered: r3, ip, CC
cas_dummy:
.type cas_dummy, %function
    ldr r3, [r2]
    subs r0, r3
    streq r1, [r2]
    bx lr

.global __a_cas_v6
.hidden __a_cas_v6
.type __a_cas_v6, %function
__a_cas_v6:
    mcr p15, 0, r0, c7, c10, 5
    mov ip, r0
1:
    ldrex r3, [r2]
    subs r0, r3
    strexeq r0, r1, [r2]
    teqeq r0, 1
    beq 1b

    mcr p15, 0, r0, c7, c10, 5
    bx lr
.size __a_cas_v6, . - __a_cas_v6

.global __a_cas_v7
.hidden __a_cas_v7
.type __a_cas_v7, %function
__a_cas_v7:
    dmb ish
    mov ip, r0
1:
    ldrex r3, [r2]
    subs r0, r3
    strexeq r0, r1, [r2]
    teqeq r0, 1
    beq 1b

    dmb ish
    bx lr
.size __a_cas_v7, . - __a_cas_v7

.global __a_gettp_v6
.hidden __a_gettp_v6
.type __a_gettp_v6, %function
__a_gettp_v6:
    mrc p15, 0, r0, c13, c0, 3
    bx lr
.size __a_gettp_v6, . - __a_gettp_v6
#endif
