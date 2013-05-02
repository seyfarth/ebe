        segment .data
        align   16
a       dq      3.25, 1.1
b       dq      10.53, 2.2
c       dq      0.0, 0.0

        segment .text
        global  main
main:
        push    rbp
        mov     rbp, rsp
        movapd  xmm0, [a]   ; load 2 doubles from a
        addpd   xmm0, [b]   ; add a[0]+b[0] and a[1]+b[1]
        movapd  [c], xmm0   ; store 2 sums in c
        leave
        ret
