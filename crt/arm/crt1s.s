.syntax unified
.global _start
.type _start, %function
_start:
    mov r0, sp
    and r1, r0, -8
    mov sp, r1
    mov fp, 0
.hidden _start_c
    bl _start_c
