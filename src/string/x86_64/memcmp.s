.global memcmp
.type memcmp, @function
memcmp:
    testq %rdx, %rdx
    je 1f
    movq %rdx, %rcx
    repe
    cmpsb
    setab %al
    setbb %cl
    subb %cl, %al
    movsbl %al, %eax
    retq

1:
    xorl %eax, %eax
    retq
.size memcmp, . - memcmp
