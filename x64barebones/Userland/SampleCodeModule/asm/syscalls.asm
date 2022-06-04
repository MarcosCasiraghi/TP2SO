section .text

GLOBAL sys_write
GLOBAL sys_clear
GLOBAL sys_read
GLOBAL sys_exit
GLOBAL sys_registers
GLOBAL sys_scheduler
GLOBAL sys_time

;----------------------------------------
; rdi=string, rsi=fontColor, rdx=backColor
;----------------------------------------
sys_write:
    int 80h
    ret

sys_read:
    int 81h
    ret

sys_clear:
    int 82h
    ret

sys_exit:
    int 83h
    ret

sys_scheduler:
    int 84h
    ret

sys_registers:
    int 85h
    ret

sys_time:
    int 86h
    ret
