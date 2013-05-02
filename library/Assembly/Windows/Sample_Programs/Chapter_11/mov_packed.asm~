        segment .data
x       dd      1.75, 3.25, 4.765, 6.3
y       dq      3.14159, 5.29287

        segment .bss
a       resq    2

        segment .text
        global  main
main:
        push    rbp
        mov     rbp, rsp
        movupd  xmm15, [y]
        movups  xmm0, [x]
        movupd  [a], xmm15
        leave
        ret
