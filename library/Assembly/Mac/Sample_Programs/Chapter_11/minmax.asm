        segment .data
x       dd      3.25
y       dd      10.53
        align   16
a       dq      1.75, 3.14159
b       dq      3.14159, 2.5

        segment .text
        global  main
main:
        push    rbp
        mov     rbp, rsp
        movss   xmm0, [x]   ; load x
        maxss   xmm0, [y]   ; xmm0 has max(x,y)
        movapd  xmm0, [a]   ; move a[0] and a[1] into xmm0
        minpd   xmm0, [b]   ; xmm0[0] has min(a[0],b[0])
                            ; xmm0[1] has min(a[1],b[1])
        leave
        ret
