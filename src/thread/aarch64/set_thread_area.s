.global __set_thread_area
.hidden __set_thread_area
.type __set_thread_area, @function
__set_thread_area:
    msr tpidr_el0, x0
    mov x0, 0
    ret
.size __set_thread_area, . - __set_thread_area
