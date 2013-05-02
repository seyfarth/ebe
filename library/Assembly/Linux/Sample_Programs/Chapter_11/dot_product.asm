        segment .data
x       dd      0.0, 1.0, 2.0
y       dd      1.0, 3.0, 5.0

        segment .text
        global  main, dot_product
main:
        push    rbp
        mov     rbp, rsp
        lea     rdi, [x]
        lea     rsi, [y]
        call    dot_product
        xor     eax, eax
        leave
        ret

dot_product:
        movss   xmm0, [rdi]
        mulss   xmm0, [rsi]
        movss   xmm1, [rdi+4]
        mulss   xmm1, [rsi+4]
        addss   xmm0, xmm1
        movss   xmm2, [rdi+8]
        mulss   xmm2, [rsi+8]
        addss   xmm0, xmm2
        ret
