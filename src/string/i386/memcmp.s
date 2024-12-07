.global memcmp
.type memcmp, @function
memcmp:
    pushl %esi
    pushl %edi
    movl 12(%esp), %edi
    movl 16(%esp), %esi
    movl 20(%esp), %ecx
    testl %ecx, %ecx
    je 1f
    repe
    cmpsb
    setab %al
    setbb %cl
    subb %cl, %al
    movsbl %al, %eax
2:
    popl %esi
    popl %edi
    retl

1:
    xorl %eax, %eax
    jmp 2b
.size memcmp, . - memcmp
