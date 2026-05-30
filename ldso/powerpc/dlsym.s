.global dlsym
.type dlsym, @function
dlsym:
    mflr %r5
.hidden __dlsym
    b __dlsym
.size dlsym, . - dlsym
