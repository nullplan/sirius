.global memset
.type memset, @function
memset:
    pushl %esi
    pushl %edi
    movl 12(%esp), %edi
    movl 16(%esp), %eax
    movl 20(%esp), %ecx

    andl $255, %eax
    imull $0x01010101, %eax
    leal (%edi, %ecx), %esi
    cmpl $126, %ecx
    ja 2f
    testl %ecx, %ecx
    jz 99f
    movb %al, (%edi)
    movb %al, -1(%esi)
    cmpb $2, %cl
    jna 99f
    movw %ax, 1(%edi)
    movw %ax, -3(%esi)
    cmpb $6, %cl
    jna 99f
    movl %eax, 3(%edi)
    movl %eax, -7(%esi)
    cmpb $14, %cl
    jna 99f
    movl %eax, 7(%edi)
    movl %eax, 11(%edi)
    movl %eax, -15(%esi)
    movl %eax, -11(%esi)
    cmpb $30, %cl
    jna 99f
    movl %eax, 15(%edi)
    movl %eax, 19(%edi)
    movl %eax, 23(%edi)
    movl %eax, 27(%edi)
    movl %eax, -31(%esi)
    movl %eax, -27(%esi)
    movl %eax, -23(%esi)
    movl %eax, -19(%esi)
    cmpb $62, %cl
    jna 99f
    movl %eax, 31(%edi)
    movl %eax, 35(%edi)
    movl %eax, 39(%edi)
    movl %eax, 43(%edi)
    movl %eax, 47(%edi)
    movl %eax, 51(%edi)
    movl %eax, 55(%edi)
    movl %eax, 59(%edi)
    movl %eax, -63(%esi)
    movl %eax, -59(%esi)
    movl %eax, -55(%esi)
    movl %eax, -51(%esi)
    movl %eax, -47(%esi)
    movl %eax, -43(%esi)
    movl %eax, -39(%esi)
    movl %eax, -35(%esi)
    jmp 99f

2:
# large subcase
    movl %eax, (%edi)
    movl %eax, 4(%edi)
    movl %eax, 8(%edi)
    movl %eax, 12(%edi)
    movl %eax, 16(%edi)
    movl %eax, 20(%edi)
    movl %eax, 24(%edi)
    movl %eax, 28(%edi)
    movl %eax, -32(%esi)
    movl %eax, -28(%esi)
    movl %eax, -24(%esi)
    movl %eax, -20(%esi)
    movl %eax, -16(%esi)
    movl %eax, -12(%esi)
    movl %eax, -8(%esi)
    movl %eax, -4(%esi)
    addl $32, %edi
    andl $-32, %esi
    andl $-32, %edi
1:
    movl %eax, (%edi)
    movl %eax, 4(%edi)
    movl %eax, 8(%edi)
    movl %eax, 12(%edi)
    movl %eax, 16(%edi)
    movl %eax, 20(%edi)
    movl %eax, 24(%edi)
    movl %eax, 28(%edi)
    addl $32, %edi
    cmpl %edi, %esi
    jnz 1b

99:
    movl 12(%esp), %eax
    popl %edi
    popl %esi
    retl
.size memset, . - memset
