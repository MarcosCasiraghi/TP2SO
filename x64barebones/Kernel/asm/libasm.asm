GLOBAL cpuVendor
GLOBAL kbFlag
GLOBAL getRTC
GLOBAL loadRegisters
;GLOBAL getReg
GLOBAL hasRegDump
GLOBAL regdump
section .text

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

cpuVendor:
	push rbp
	mov rbp, rsp

	push rbx

	mov rax, 0
	cpuid


	mov [rdi], ebx
	mov [rdi + 4], edx
	mov [rdi + 8], ecx

	mov byte [rdi+13], 0

	mov rax, rdi

	pop rbx

	mov rsp, rbp
	pop rbp
	ret

kbFlag:
    push rbp
    mov rbp, rsp
    ;limpio rax
    mov rax,0

_loop:
    ;muevo a al el contenido del puerto 64
    in al,0x64
    ; dejo solo el ultimo bit
    and al,0x01
    cmp al,0
    je _loop
    in al,0x60

    mov rsp, rbp
    pop rbp
    ret
getRTC:
    push rbp
	mov rbp, rsp

	mov al, dil     ;pasando parametro
	out 70h, al
	in al, 71h

	mov rsp, rbp
	pop rbp
	ret

loadRegisters:
    enter 0,0

    pushState

            ; mov [regdump+2*8], rbx
            ; mov [regdump+3*8], rcx
            ; mov [regdump+4*8], rdx
            ; mov [regdump+5*8], rsi
            ; mov [regdump+6*8], rdi
            ; mov [regdump+7*8], rbp
            ; mov [regdump+9*8], r8
            ; mov [regdump+10*8], r9
            ; mov [regdump+11*8], r10
            ; mov [regdump+12*8], r11
            ; mov [regdump+13*8], r12
            ; mov [regdump+14*8], r13
            ; mov [regdump+15*8], r14
            ; mov [regdump+16*8], r15

	mov [regdump], rax
	mov [regdump+8], rbx
	mov [regdump+16], rcx
	mov [regdump+24], rdx
	mov [regdump+32], rsi
	mov [regdump+40], rdi
	mov [regdump+48], rbp
	mov [regdump+64], r8
	mov [regdump+72], r9
	mov [regdump+80], r10
	mov [regdump+88], r11
	mov [regdump+96], r12
	mov [regdump+104], r13
	mov [regdump+112], r14
	mov [regdump+120], r15

    mov rax, rsp
    add rax, 160
    mov [regdump+56], rax ;RSP

    mov rax, [rsp+15*8]
    mov [regdump+128], rax ;RIP

    mov BYTE [hasRegDump], 1

    popState

    leave
    ret

section .bss
regdump resq 17

section .data
hasRegDump db 0
