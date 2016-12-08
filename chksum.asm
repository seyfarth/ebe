
        segment .data                               
fd      dq      0

        segment .bss
stuff   resq    1
data    resb    10000000000

        segment .text
        global  main
        extern  open, read
chksum:
        push    rbp
        mov     rbp, rsp
        sub     rsp, 32
        
        mov     r8, rsi
        add     rsi, 7
        shr     rsi, 3
        shl     rsi, 3
        xor     eax, eax
.loop:
        cmp     r8, rsi
        jge     .loop_end
        mov     [rdi+r8], al
        inc     r8
        jmp     .loop
.loop_end:
        shr     rsi, 3
        ;dec     rax
        xor     r8d, r8d
.chksum_loop:
        cmp     r8, rsi
        jge     .chksum_end
        ror     rax, 1
        add     rax, [rdi+r8]
        add     r8, 8
        jmp     .chksum_loop
        
.chksum_end:
        
        leave
        ret
main:       
        push    rbp
        mov     rbp, rsp
        
;       if ( argc > 1 ) {   // open a file named in argv[1]
        cmp     edi, 1
        je      no_file_name
            mov     rdi, [rsi+8] ; get argv[1]
            xor     esi, esi     ; O_RDONLY
            call    open
            mov     [fd], eax
;       }              
no_file_name:
        mov     edi, [fd]
        lea     rsi, [data]
        mov     rdx, 1000000000
        call    read
    
        lea     rdi, [data]
        mov     rsi, rax
        call    chksum
        leave
        ret
