section .text

GLOBAL sys_print
GLOBAL sys_clear

sys_print:
    ;deberia tener en el rdi mi string
    int 80h
    ret

sys_clear:
    int 82h
    ret
