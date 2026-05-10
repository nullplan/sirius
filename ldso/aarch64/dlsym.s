.global dlsym
.type dlsym, %function
dlsym:
    mov x2, lr
.hidden __dlsym
    b __dlsym
.size dlsym, . - dlsym
