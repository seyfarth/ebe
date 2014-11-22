;   This program tests the numbers a, b and c to
;   see if they can be the legs and hypotenuse of
;   a right triangle: a^2 + b^2 = c^2

        segment .data
a       dq      246         ; one leg of a triangle
b       dq      328         ; another leg
c       dq      410         ; hypotenuse ?
        segment .text
        global  main
main:
        mov     rax, [a]    ; move a into rax
        imul    rax, rax    ; a squared
        mov     rdx, [b]    ; move b into rdx
        imul    rdx, rdx    ; b squared
        mov     rcx, [c]    ; move c into rcx
        imul    rcx, rcx    ; c squared
        add     rax, rdx    ; rax has a^2+b^2
        sub     rax, rcx    ; is rax 0?
        xor     rax, rax
        ret
