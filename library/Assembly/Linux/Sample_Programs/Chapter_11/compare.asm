        segment .data
a       dd      1.5
b       dd      2.5
c       dd      7.5
        segment .text
        global  main
main:
        push    rbp
        mov     rbp, rsp
        movss   xmm0, [a]
        mulss   xmm0, [b]
        ucomiss xmm0, [c]
        jbe     less_eq
        mov     eax, 0
        jmp     skip
less_eq mov     eax, 1
skip    leave
        ret
