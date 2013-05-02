        segment .data
x       dd      0.0, 1.0, 2.0
y       dd      1.0, 3.0, 5.0

        segment .text
        global  main, distance3d
main:
        push    rbp
        mov     rbp, rsp
        lea     rdi, [x]
        lea     rsi, [y]
        call    distance3d
        xor     eax, eax
        leave
        ret

distance3d:
        movss   xmm0, [rdi]     ; x from first point
        subss   xmm0, [rsi]     ; subtract x from second point
        mulss   xmm0, xmm0      ; (x1-x2)^2
        movss   xmm1, [rdi+4]   ; y from first point
        subss   xmm1, [rsi+4]   ; subtract y from second point
        mulss   xmm1, xmm1      ; (y1-y2)^2
        movss   xmm2, [rdi+8]   ; z from first point
        subss   xmm2, [rsi+8]   ; subtract z from second point
        mulss   xmm2, xmm2      ; (z1-z2)^2
        addss   xmm0, xmm1      ; add x and y parts
        addss   xmm0, xmm2      ; add z part
        sqrtss  xmm0, xmm0
        ret
