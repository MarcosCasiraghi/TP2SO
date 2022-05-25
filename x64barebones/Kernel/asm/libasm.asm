GLOBAL cpuVendor
GLOBAL kbFlag
GLOBAL pushRegisters1
GLOBAL popRegisters2
GLOBAL pushRegisters2
GLOBAL popRegisters1
section .text
	
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

pushRegisters1:
    enter 0,0
    leave
    ret

pushRegisters2:
    enter 0,0
    leave
    ret

popRegisters2:
    enter 0,0
    leave
    ret

popRegisters1:
    enter 0,0
    leave
    ret