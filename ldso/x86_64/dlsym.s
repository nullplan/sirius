.global dlsym
.type dlsym, @function
dlsym:
    movq (%rsp), %rdx
.hidden __dlsym
    jmp __dlsym
.size dlsym, . - dlsym
