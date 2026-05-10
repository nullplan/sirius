.global dlsym
.type dlsym, @function
dlsym:
    ; i386 is just lovely! You cannot tail-call functions with more arguments. So here I actually have to do a subroutine call.
    pushl (%esp)
    pushl 12(%esp)
    pushl 12(%esp)
.hidden __dlsym
    call __dlsym
    addl $12, %esp
    retl
.size dlsym, . - dlsym
