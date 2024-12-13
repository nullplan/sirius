.global __set_thread_area
.hidden __set_thread_area
.type __set_thread_area, @function
__set_thread_area:
    mr %r13, %r3
    li %r3, 0
    blr
.size __set_thread_area, . - __set_thread_area
