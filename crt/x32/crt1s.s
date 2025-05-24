.global _start
.type _start, @function
_start:
  xorl %ebp, %ebp
  movl %esp, %edi
  andl $-16, %esp
.hidden _start_c
  callq _start_c
  ud2
.size _start, . - _start
