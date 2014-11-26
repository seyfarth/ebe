        segment .data
a       dd      3.25
b       dd      10.53
c       dd      0.0

        segment .text
        global  main
main:
        movss   xmm0, [a]   ; load a
        addss   xmm0, [b]   ; add b to a
        movss   [c], xmm0   ; store sum in c
        ret
