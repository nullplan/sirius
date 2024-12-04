.global _start
.type _start, @function
_start:
  xorl %ebp, %ebp
  movl %esp, %eax
  andl $-16, %esp
  addl $-12, %esp
  pushl %eax
.hidden _start_c
  calll _start_c
  ud2
.size _start, . - _start
