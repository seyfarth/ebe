        segment .data
a       dd      3.14159
x       dd      125
        segment .text
        global  main
main:
        push        rbp
        mov         rbp, rsp
        mov         rax, 0x1a1a1a1a1a1a1a1a
        movss       xmm0, [a]
        cvtss2si    eax, xmm0  ; get to dword integer
        cvtsi2sd    xmm0, rax  ; convert qword to double
        cvtsi2sd    xmm0, dword [x] ; convert dword integer
        leave
        ret
