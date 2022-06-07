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

	mov [regdump+8], rax
	mov [regdump+16], rbx
	mov [regdump+24], rcx
	mov [regdump+32], rdx
	mov [regdump+40], rsi
	mov [regdump+48], rdi
	mov [regdump+56], rbp
	mov [regdump+72], r8
	mov [regdump+80], r9
	mov [regdump+88], r10
	mov [regdump+96], r11
	mov [regdump+104], r12
	mov [regdump+112], r13
	mov [regdump+120], r14
	mov [regdump+128], r15

    mov rax, rsp
    add rax, 160
    mov [regdump+64], rax ;RSP

    mov rax, [rsp+15*8]
    mov [regdump], rax ;RIP

    mov BYTE [hasRegDump], 1

    popState

    leave
    ret

section .bss
regdump resq 17

section .data
hasRegDump db 0
