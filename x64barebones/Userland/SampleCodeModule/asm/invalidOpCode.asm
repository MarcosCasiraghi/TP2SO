GLOBAL invalidOpCode

;UD2 representa un invalid Opcode
invalidOpCode:
        push rbp
        mov rbp, rsp
        mov rbx, 11     ;para verificar que funciona correctamente

        UD2

        mov rsp, rbp    ;;no es necesario
        pop rbp

        ret