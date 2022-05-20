section .text

GLOBAL sys_print

sys_print:
    int 80h
    ret
