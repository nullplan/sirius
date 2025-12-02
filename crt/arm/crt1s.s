.syntax unified
.global _start
.type _start, %function
_start:
    mov r0, sp
    and sp, r0, -8
    mov fp, 0
.hidden _start_c
    bl _start_c
