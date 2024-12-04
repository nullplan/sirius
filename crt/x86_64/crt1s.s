.global _start
.type _start, @function
_start:
  xorl %ebp, %ebp
  movq %rsp, %rdi
  andq $-16, %rsp
.hidden _start_c
  callq _start_c
  ud2
.size _start, . - _start
