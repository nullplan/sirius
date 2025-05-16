.global _start
.type _start, @function
_start:
  li %r0, 0
  mr %r3, %r1
  clrrwi %r1, %r1, 4
  stwu %r0, -16(%r1)
  mtlr %r0
.hidden _start_c
  b _start_c
.size _start, . - _start
