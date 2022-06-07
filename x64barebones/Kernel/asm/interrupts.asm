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
GLOBAL _exitHandler

GLOBAL _schedulerHandler
GLOBAL _registersHandler

GLOBAL _getRTCHandler

GLOBAL _div0Handler
GLOBAL _invalidOpcodeHandler

EXTERN irqDispatcher
EXTERN syscallDispatcher
EXTERN schedulerDispatcher
EXTERN exceptionDispatcher

EXTERN setRegisters
EXTERN getRegisters
EXTERN getProcesses
EXTERN getReg

EXTERN tick_check

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
	mov r9, %1
	call syscallDispatcher

	popState
	iretq

%endmacro



%macro exceptionHandler 1
	;pushState
	mov [regdata], rax
	mov [regdata+8], rbx
	mov [regdata+16], rcx
	mov [regdata+24], rdx
	mov [regdata+32], rsi
	mov [regdata+40], rdi
	mov [regdata+48], rbp
	mov [regdata+64], r8
	mov [regdata+72], r9
	mov [regdata+80], r10
	mov [regdata+88], r11
	mov [regdata+96], r12
	mov [regdata+104], r13
	mov [regdata+112], r14
	mov [regdata+120], r15

	mov rax, [rsp+16] ;flags
	mov [regdata+136], rax

	mov rax, [rsp + 24] ; rsp
	mov [regdata+56],rax

	mov rax, [rsp] ;rip
	mov [regdata+128], rax

	mov rsi, regdata
	mov rdi, %1 ; pasaje de parametro
	call exceptionDispatcher

	;popState
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
    popState

	call getProcesses
	cmp rax,0
	jz fin

	cmp BYTE [firstTime],0
	jz get

	push rbx
	;1 por el push
	mov rbx, [rsp+1*8]
    mov [regdata], rbx ;rip

	mov rbx, [rsp+4*8]
    mov [regdata+8*8],rbx ;rsp

	mov rbx,[rsp+3*8]
	mov [regdata+17*8],rbx ;flags

	pop rbx

    mov [regdata+1*8], rax
    mov [regdata+2*8], rbx
    mov [regdata+3*8], rcx
    mov [regdata+4*8], rdx
    mov [regdata+5*8], rsi
    mov [regdata+6*8], rdi
    mov [regdata+7*8], rbp
    mov [regdata+9*8], r8
    mov [regdata+10*8], r9
    mov [regdata+11*8], r10
    mov [regdata+12*8], r11
    mov [regdata+13*8], r12
    mov [regdata+14*8], r13
    mov [regdata+15*8], r14
    mov [regdata+16*8], r15

	; ; cargo regdata en rdi para pasar parametro
    mov rdi, regdata
	mov rsi, 1
    call setRegisters

	get:
	; llamo para recibir en rax vector con registros
	mov BYTE [firstTime],1
    call getRegisters

	mov rbx, [rax]
    mov [rsp], rbx ;pongo el rip

	mov rbx, [rax+8*8]
    mov [rsp+3*8], rbx ;pongo el rsp

	mov rbx, [rax+17*8]
    mov [rsp+2*8], rbx  ;pongo los flags


    mov rbx, [rax+2*8]
    mov rcx, [rax+3*8]
    mov  rdx,[rax+4*8]
    mov  rsi,[rax+5*8]
    mov  rdi,[rax+6*8]
    mov  rbp,[rax+7*8]

    mov   r8,[rax+9*8]
    mov   r9,[rax+10*8]
    mov  r10,[rax+11*8]
    mov  r11,[rax+12*8]
    mov  r12,[rax+13*8]
    mov  r13,[rax+14*8]
    mov  r14,[rax+15*8]
    mov  r15,[rax+16*8]

	mov rax, [rax+8] ;restauro el rax


	fin:
	; signal pic EOI (End of Interrupt)
	push rax
    mov al, 20h
    out 20h, al
	pop rax

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

_exitHandler:
    syscallHandlerMaster 4

_schedulerHandler:
    pushState

	;este movimiento es porque los parametros no estan en orden
	mov rcx, [rsp+17*8]
	call schedulerDispatcher

	popState
	iretq

_registersHandler:
        syscallHandlerMaster 6

_getRTCHandler:
	syscallHandlerMaster 7

;Zero Division Exception
_div0Handler:
    exceptionHandler 0

_invalidOpcodeHandler:
	exceptionHandler 1

haltcpu:
    cli
    hlt
    ret



SECTION .bss
    regdata    resq 19 ;registros y flag
    aux resq 1

SECTION .data
	firstTime db 0

