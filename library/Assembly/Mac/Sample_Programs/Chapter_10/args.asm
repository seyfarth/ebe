        segment .data
format  db      "%s",0x0a,0
        segment .text
        global  main            ; let the linker know about main
        extern  printf          ; resolve printf from libc
main:
        push    rbp             ; prepare stack frame for main
        mov     rbp, rsp
        sub     rsp, 16
        mov     rcx, rsi        ; move argv to rcx
        mov     rsi, [rcx]      ; get first argv string
start_loop:
        lea     rdi, [format]
        mov     [rsp], rcx      ; save argv
        xor     eax, eax
        call    printf
        mov     rcx, [rsp]      ; restore rsi
        add     rcx, 8          ; advance to the next pointer in argv
        mov     rsi, [rcx]      ; get next argv string
        cmp     rsi, 0          ; it's sad that mov doesn't also test
        jnz     start_loop      ; end with NULL pointer
end_loop:
        xor     eax, eax
        leave
        ret
