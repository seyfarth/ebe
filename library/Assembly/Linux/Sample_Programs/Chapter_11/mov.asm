        segment .data
y       dq      3.25
z       dq      10.53
x       dd      1.75

        segment .text
        global  main
main:
        push    rbp
        mov     rbp, rsp
        movsd   xmm1, [z]   ; move double from z into xmm1
        movss   xmm0, [x]   ; move value at x into xmm0
        movsd   [y], xmm1   ; move double from xmm1 to y
        movss   xmm2, xmm0  ; move from xmm0 to xmm2
        leave
        ret
