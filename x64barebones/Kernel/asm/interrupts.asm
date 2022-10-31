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

GLOBAL _psHandler

GLOBAL _schedulerHandler
GLOBAL _killHandler
GLOBAL _niceHandler
GLOBAL _blockHandler
GLOBAL _registersHandler
GLOBAL _exec

GLOBAL _getRTCHandler

GLOBAL _getMemHandler
GLOBAL _div0Handler
GLOBAL _invalidOpcodeHandler

GLOBAL _mallocHandler
GLOBAL _freeHandler
GLOBAL _mmStatusHandler

EXTERN irqDispatcher
EXTERN syscallDispatcher
EXTERN schedulerDispatcher
EXTERN exceptionDispatcher
EXTERN getKey
EXTERN schedulerExit
EXTERN exitCurrent

EXTERN registerManager
EXTERN getProcesses

EXTERN registersForInforeg
EXTERN quantum_check
EXTERN killProcess
EXTERN blockProcess


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

%macro saveRegs 1
	mov [%1+1*8], rax
    mov [%1+2*8], rbx
    mov [%1+3*8], rcx
    mov [%1+4*8], rdx
    mov [%1+5*8], rsi
    mov [%1+6*8], rdi
    mov [%1+7*8], rbp
    mov [%1+9*8], r8
    mov [%1+10*8], r9
    mov [%1+11*8], r10
    mov [%1+12*8], r11
    mov [%1+13*8], r12
    mov [%1+14*8], r13
    mov [%1+15*8], r14
    mov [%1+16*8], r15
%endmacro

%macro findRegs 1
	mov  rax,[%1+8]
	mov rbx, [%1+2*8]
    mov rcx, [%1+3*8]
    mov  rdx,[%1+4*8]
    mov  rsi,[%1+5*8]
    mov  rdi,[%1+6*8]
    mov  rbp,[%1+7*8]
    mov   r8,[%1+9*8]
    mov   r9,[%1+10*8]
    mov  r10,[%1+11*8]
    mov  r11,[%1+12*8]
    mov  r12,[%1+13*8]
    mov  r13,[%1+14*8]
    mov  r14,[%1+15*8]
    mov  r15,[%1+16*8]
%endmacro

%macro _timerHandlerMacro 0
	saveRegs regdata
	mov rdi, 0 ; pasaje de parametro
    call irqDispatcher



	;si no hay procesos no quiero que haya context switching
	call getProcesses
	cmp rax,0
	jz .fin

	;guardo registros en regdata
	mov rbx, [rsp]
    mov [regdata], rbx ;rip

	mov rbx, [rsp+3*8]
    mov [regdata+8*8],rbx ;rsp

	mov rbx,[rsp+2*8]
	mov [regdata+17*8],rbx ;flags

	; cargo regdata en rdi para pasar parametro
	;guardo contexto de programas, recibo contexto del siguiente programa a correr
    mov rdi, regdata
	mov rsi, [firstTime]
    call registerManager

	;la primera vez no debo settear el contexto, por eso el flag "firstTime"
	mov BYTE [firstTime],1

	mov rbx, [rax]
    mov [rsp], rbx ;pongo el rip

	mov rbx, [rax+8*8]
    mov [rsp+3*8], rbx ;pongo el rsp

	mov rbx, [rax+17*8]
    mov [rsp+2*8], rbx  ;pongo los flags

	;setteo el resto de los registros
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
	jmp .endtimer


	.fin:
	findRegs regdata
	; signal pic EOI (End of Interrupt)
	.endtimer:
	push rax
    mov al, 20h
    out 20h, al
	pop rax
    iretq
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

	mov r8, %1
	call syscallDispatcher

	popState
	iretq

%endmacro



%macro exceptionHandler 1

	;copiamos parametros a regdata
	saveRegs regdata

	mov rax, [rsp + 24] ; rsp
	mov [regdata+64],rax

	mov rax, [rsp] ;rip
	mov [regdata], rax

	;pasamos regdata y el codigo de excepcion
	mov rsi, regdata
	mov rdi, %1 ; pasaje de parametro
	call exceptionDispatcher

	findRegs regdata
	_timerHandlerMacro
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


_timerHandler:
	_timerHandlerMacro


;Keyboard
_keyboardHandler:
	saveRegs regsave

	mov rbx, [rsp]
    mov [regsave], rbx ;rip

	mov rbx, [rsp+3*8]
    mov [regsave+8*8],rbx ;rsp

	mov rbx,[rsp+2*8]
	mov [regsave+17*8],rbx ;flags

	call getKey
	cmp rax, 0
	je endcase

	cmp rax, 2
	je inforegcase

	;else, la funcion retorno 1, debe hacer switching
	findRegs regsave
	_timerHandlerMacro

	inforegcase:
	mov rdi, regsave
	call registersForInforeg

	endcase:
	mov al, 20h
	out 20h, al

	findRegs regsave
	iretq


_writeHandler:
    syscallHandlerMaster 1



_readHandler:
    syscallHandlerMaster 2


_clearHandler:
    syscallHandlerMaster 3

_exitHandler:
	pushState

	call exitCurrent

	popState

	_timerHandlerMacro

_schedulerHandler:
    pushState

	mov r9, [rsp+17*8]
	call schedulerDispatcher

	popState
	iretq

_registersHandler:
    syscallHandlerMaster 6

_getRTCHandler:
	syscallHandlerMaster 7

_getMemHandler:
	syscallHandlerMaster 8

;Zero Division Exception
_div0Handler:
    exceptionHandler 0

_invalidOpcodeHandler:
	exceptionHandler 1

_exec:
	_timerHandlerMacro

_mallocHandler:
	syscallHandlerMaster 9

_freeHandler:
	syscallHandlerMaster 10

_mmStatusHandler:
	syscallHandlerMaster 11

_psHandler:
    syscallHandlerMaster 12

_killHandler:
	pushState

	call killProcess

	popState

	_timerHandlerMacro
_niceHandler:
	syscallHandlerMaster 13

_blockHandler:
	pushState

	call blockProcess

	popState

	_timerHandlerMacro


haltcpu:
    cli
    hlt
    ret



SECTION .bss
    regdata    resq 19 ;registros y flag
	regsave resq 19
    aux resq 1

SECTION .data
	firstTime db 0

