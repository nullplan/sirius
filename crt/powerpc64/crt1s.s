.global _start
.type _start, @function
_start:
  addis %r2, %r12, .TOC.-_start@ha
  addi %r2, %r2, .TOC.-_start@l
  li %r0, 0
  mr %r3, %r1
  clrrdi %r1, %r1, 4
  stdu %r0, -32(%r1)
  mtlr %r0
.hidden _start_c
  b _start_c
.size _start, . - _start
