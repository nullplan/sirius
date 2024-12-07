.global _start
.type _start, @function
_start:
  mov x0, sp
  and sp, x0, -16
  mov fp, 0
.hidden _start_c
  b _start_c
.size _start, . - _start
