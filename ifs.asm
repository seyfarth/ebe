        segment .data
save_rcx dq     0
save_rdi dq     0
save_rsi dq     0

        segment .text
        global  main     ; Tell linker about main
        extern  printf
main:
        push    rbp
        mov     rbp, rsp ; setup stack frame

; Up to 6 integer parameters in rdi, rsi, rdx, rcx, r8, r9
; Up to 8 float parameters in xmm0-xmm7


    

;    }
end_if:                     ; name the label what you want

        xor     ecx, ecx
;    while ( ecx < edi ) {
     cmp    ecx, edi     ; subtract rax-10 without saving
     jnl    end_while   ; jump out of the while if not less than 10

while_top:
        segment .data
format  db      'argv[%d] = %s', 0x0a, 0
        segment .text
        
        mov     [save_rcx], rcx
        mov     [save_rdi], rdi
        mov     [save_rsi], rsi
        lea     rdi, [format]
        mov     rdx, [rsi+8*rcx]
        mov     esi, ecx
        xor     eax, eax
        call    printf
        
        mov     rcx, [save_rcx]
        mov     rdi, [save_rdi]
        mov     rsi, [save_rsi]
        inc     ecx

     cmp    ecx, edi    ; subtract rax-10 without saving
     jl     while_top   ; repeat while rax < 10
;    }
end_while:


;   do {
top_of_do:

;       do whatever you want

        dec     rax         ; rax needs to change somehow in the loop
        jne     top_of_do   ; repeat if i is not 0
;   } while ( rax != 0 );


;    It is slightly more efficient to test at the bottom.
;    Jumping back to the original test is fine for most uses.



        leave
        ret

