        segment .data
a       dq      1.5
b       dq      10.43
s       db      'sdfsdfasdfasdfasdfasdfsadfasdf'

        segment .bss
c       resb    3
        alignb  16
d       resq    8

        segment .text
        global  main
main:
        push    rbp
        mov     rbp, rsp
        frame   8, 2, 8
        sub     rsp, frame_size
        mov     rax, 1
        
        roundsd xmm0, [a], 0
        roundsd xmm1, [b], 0
        movsd   xmm0, [a]        ; load a
        movsd   xmm1, [b]        ; load b
        vaddsd  xmm2, xmm0, xmm1 ; xmm2 = xmm1 + xmm0
        leave
        ret
