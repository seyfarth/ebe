        segment .data
a       dd      3.14159
        align   8
b       dq      3.14159265358979323846
c       dd      0.0
two_pi  dq      6.28318530717958647692
        segment .text
        global  main
main:
        push        rbp
        mov         rbp, rsp
        cvtss2sd    xmm0, [a]  ; get a into xmm0 as a double
        addsd       xmm0, [b]  ; add a double to a
        cvtsd2ss    xmm0, xmm0 ; convert to float
        movss       [c], xmm0
        leave
        ret
