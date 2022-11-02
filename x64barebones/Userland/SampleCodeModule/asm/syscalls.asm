section .text

GLOBAL sys_write
GLOBAL sys_clear
GLOBAL sys_read
GLOBAL sys_exit
GLOBAL sys_exec
GLOBAL sys_registers
GLOBAL sys_scheduler
GLOBAL sys_kill
GLOBAL sys_nice
GLOBAL sys_block
GLOBAL sys_time
GLOBAL sys_getMem
GLOBAL sys_malloc
GLOBAL sys_free
GLOBAL sys_memStatus
GLOBAL sys_ps
GLOBAL sys_getPID
GLOBAL sys_semOpen
GLOBAL sys_semClose
GLOBAL sys_semWait
GLOBAL sys_semPost
GLOBAL sys_semStatus
GLOBAL sys_pipeStatus
GLOBAL sys_pipeOpen
GLOBAL sys_pipeClose
GLOBAL sys_pipeWrite
GLOBAL sys_pipeRead
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

sys_getMem:
    int 87h
    ret

sys_exec:
    int 88h
    ret

sys_malloc:
    int 89h
    ret

sys_free:
    int 90h
    ret

sys_memStatus:
    int 91h
    ret

sys_ps:
    int 92h
    ret
sys_kill:
    int 93h
    ret

sys_nice:
    int 94h
    ret

sys_block:
    int 95h
    ret

sys_getPID:
    int 96h
    ret

sys_semOpen:
    int 97h
    ret

sys_semClose:
    int 98h
    ret
sys_semWait:
    int 99h
    ret
sys_semPost:
    int 100h
    ret
sys_semStatus:
    int 101h
    ret

sys_pipeStatus:
    int 102h
    ret

sys_pipeClose:
    int 103h
    ret

sys_pipeOpen:
    int 104h
    ret

sys_pipeWrite:
    int 105h
    ret

sys_pipeRead:
    int 106h
    ret

