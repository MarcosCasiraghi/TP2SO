section .text

GLOBAL sys_write
GLOBAL sys_clear
GLOBAL sys_read

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
