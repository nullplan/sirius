.global memset
.type memset, @function
memset:
    movq %edi, %eax
    andl $0xff, %esi
    movabsq $0x0101010101010101, %rcx
    imulq %rsi, %rcx
    leaq (%rdi, %rdx), %rsi

    cmpq $254, %rdx
    ja 2f

    testq %rdx, %rdx
    jz 99f
    movb %cl, (%rdi)
    movb %cl, -1(%rsi)

    cmpq $2, %rdx
    jbe 99f
    movw %cx, 1(%rdi)
    movw %cx, -3(%rsi)
    cmpq $6, %rdx
    jbe 99f

    movl %ecx, 3(%rdi)
    movl %ecx, -7(%rsi)
    cmpq $14, %rdx
    jbe 99f

    movq %rcx, 7(%rdi)
    movq %rcx, -15(%rsi)
    cmpq $30, %rdx
    jbe 99f

    movq %rcx, 15(%rdi)
    movq %rcx, 23(%rdi)
    movq %rcx, -31(%rsi)
    movq %rcx, -23(%rsi)
    cmpq $62, %rdx
    jbe 99f

    movq %rcx, 31(%rdi)
    movq %rcx, 39(%rdi)
    movq %rcx, 47(%rdi)
    movq %rcx, 55(%rdi)
    movq %rcx, -63(%rsi)
    movq %rcx, -55(%rsi)
    movq %rcx, -47(%rsi)
    movq %rcx, -39(%rsi)

    cmpq $120, %rdx
    jbe 99f

    movq %rcx, 63(%rdi)
    movq %rcx, 71(%rdi)
    movq %rcx, 79(%rdi)
    movq %rcx, 87(%rdi)
    movq %rcx, 95(%rdi)
    movq %rcx, 103(%rdi)
    movq %rcx, 111(%rdi)
    movq %rcx, 119(%rdi)
    movq %rcx, -127(%rsi)
    movq %rcx, -119(%rsi)
    movq %rcx, -111(%rsi)
    movq %rcx, -103(%rsi)
    movq %rcx, -95(%rsi)
    movq %rcx, -87(%rsi)
    movq %rcx, -79(%rsi)
    movq %rcx, -71(%rsi)
99:
    retq

2:
# large case
    movq %rcx, (%rdi)
    movq %rcx, 8(%rdi)
    movq %rcx, 16(%rdi)
    movq %rcx, 24(%rdi)
    movq %rcx, 32(%rdi)
    movq %rcx, 40(%rdi)
    movq %rcx, 48(%rdi)
    movq %rcx, 56(%rdi)
    movq %rcx, -64(%rsi)
    movq %rcx, -56(%rsi)
    movq %rcx, -48(%rsi)
    movq %rcx, -40(%rsi)
    movq %rcx, -32(%rsi)
    movq %rcx, -24(%rsi)
    movq %rcx, -16(%rsi)
    movq %rcx, -8(%rsi)
    addq $64, %rdi
    andq $-64, %rsi
    andq $-64, %rdi
1:
    movq %rcx, (%rdi)
    movq %rcx, 8(%rdi)
    movq %rcx, 16(%rdi)
    movq %rcx, 24(%rdi)
    movq %rcx, 32(%rdi)
    movq %rcx, 40(%rdi)
    movq %rcx, 48(%rdi)
    movq %rcx, 56(%rdi)
    addq $64, %rdi
    cmpq %rdi, %rsi
    jnz 1b

    retq
.size memset, . - memset
