GLOBAL cpuVendor
GLOBAL kbFlag
GLOBAL getRTC
GLOBAL loadRegisters
;GLOBAL hasRegDump
;GLOBAL regdump
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

;utilizado para apretado de tecla
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

	mov al, dil  ;pasando parametro
	out 70h, al
	in al, 71h

	mov rsp, rbp
	pop rbp
	ret

