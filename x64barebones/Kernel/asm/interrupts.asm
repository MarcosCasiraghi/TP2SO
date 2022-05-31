GLOBAL _cli
GLOBAL _sti
GLOBAL picMasterMask
GLOBAL picSlaveMask
GLOBAL haltcpu
GLOBAL _hlt

GLOBAL _timerHandler
GLOBAL _keyboardHandler
GLOBAL _readHandler
GLOBAL _writeHandler
GLOBAL _clearHandler
GLOBAL _writeDecimalHandler

GLOBAL _schedulerHandler

GLOBAL _exception0Handler

EXTERN irqDispatcher
EXTERN syscallDispatcher
EXTERN exceptionDispatcher

EXTERN setRegisters
EXTERN getRegisters

SECTION .text

%macro pushState 0
	push rax
	push rbx
	push rcx
	push rdx
	push rbp
	push rdi
	push rsi
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
%endmacro

%macro popState 0
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rsi
	pop rdi
	pop rbp
	pop rdx
	pop rcx
	pop rbx
	pop rax
%endmacro

%macro pushState2 0
	push rbx
	push rcx
	push rdx
	push rbp
	push rdi
	push rsi
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
%endmacro

%macro popState2 0
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rsi
	pop rdi
	pop rbp
	pop rdx
	pop rcx
	pop rbx
%endmacro

%macro interruptHandlerMaster 1
	pushState

	mov rdi, %1 ; pasaje de parametro
	call irqDispatcher

	; signal pic EOI (End of Interrupt)
	mov al, 20h
	out 20h, al

	popState
	iretq
%endmacro


%macro syscallHandlerMaster 1
	pushState

	;este movimiento es porque los parametros no estan en orden
	mov r8, %1
	call syscallDispatcher

	popState
	iretq
%endmacro



%macro exceptionHandler 1
	pushState

	mov rdi, %1 ; pasaje de parametro
	call exceptionDispatcher

	popState
	iretq
%endmacro


_hlt:
	sti
	hlt
	ret

_cli:
	cli
	ret


_sti:
	sti
	ret

picMasterMask:
	push rbp
    mov rbp, rsp
    mov ax, di
    out	21h,al
    pop rbp
    retn

picSlaveMask:
	push    rbp
    mov     rbp, rsp
    mov     ax, di  ; ax = mascara de 16 bits
    out	0A1h,al
    pop     rbp
    retn


;8254 Timer (Timer Tick)

;hardcodeado para poder usar el scheduler
_timerHandler:
    pushState

    mov rdi, 0 ; pasaje de parametro
    call irqDispatcher

    ; signal pic EOI (End of Interrupt)
    mov al, 20h
    out 20h, al

    popState

	;rip
	push rbx
	mov rbx, [rsp+8]
    mov [regdata], rbx
	pop rbx

    mov [regdata+1*8], rax
    mov [regdata+2*8], rbx
    mov [regdata+3*8], rcx
    mov [regdata+4*8], rdx
    mov [regdata+5*8], rsi
    mov [regdata+6*8], rdi
    mov [regdata+7*8], rbp

	;rsp
	push rbx
	mov rbx, [rsp+32]
    mov [regdata+8*8],rbx
	pop rbx

    mov [regdata+9*8], r8
    mov [regdata+10*8], r9
    mov [regdata+11*8], r10
    mov [regdata+12*8], r11
    mov [regdata+13*8], r12
    mov [regdata+14*8], r13
    mov [regdata+15*8], r14
    mov [regdata+16*8], r15

	;rflags
	push rbx
	mov rbx,[rsp+24]
	mov [regdata+17*8],rbx
	pop rbx


    mov rax, regdata
    call setRegisters

    call getRegisters

    mov rbx, [rax+2*8]
    mov rcx, [rax+3*8]
    mov  rdx,[rax+4*8]
    mov  rsi,[rax+5*8]
    mov  rdi,[rax+6*8]
    mov  rbp,[rax+7*8]
    mov  rsp,[rax+8*8]
    mov   r8,[rax+9*8]
    mov   r9,[rax+10*8]
    mov  r10,[rax+11*8]
    mov  r11,[rax+12*8]
    mov  r12,[rax+13*8]
    mov  r13,[rax+14*8]
    mov  r14,[rax+15*8]
    mov  r15,[rax+16*8]
	;flags?



    ;mov rax, [rax]

	;piso iretq
	push rbx
	mov rbx, [rax]
    mov [rsp+8], rbx ;pongo el rip

	mov rbx, [rax+8*8]
    mov [rsp+4*8], rbx ;pongo el rsp

	mov rbx, [rax+17*8]
    mov [rsp+3*8], rbx  ;pongo los flags

	pop rbx

    mov rax, [rax+8] ;restauro el rax

    iretq

;Keyboard
_keyboardHandler:
    interruptHandlerMaster 1


_writeHandler:
    syscallHandlerMaster 1


_readHandler:
    syscallHandlerMaster 2


_clearHandler:
    syscallHandlerMaster 3

_writeDecimalHandler:
    syscallHandlerMaster 4

_schedulerHandler:
    syscallHandlerMaster 5


;Zero Division Exception
_exception0Handler:
    exceptionHandler 0

haltcpu:
    cli
    hlt
    ret



SECTION .bss
    regdata    resq 20 ;registros
    regdump resq 17    ;menos porque no imprime rip ni rflags
    aux resq 1

