.global memcmp
.type memcmp, @function
memcmp:
    testl %edx, %edx
    je 1f
    movl %edx, %ecx
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
