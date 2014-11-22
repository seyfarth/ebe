        segment .data
switch: dq      main.case0
        dq      main.case1
        dq      main.case2
        dq      main.case3

i:      dq      2

        segment .text
        global  main
        extern  scanf, scanf
        extern  printf
main:
        push    rbp
        mov     rbp, rsp
        sub     rsp, 16
        mov     rax, [i]
        lea     rdx, [switch]
        jmp     [rdx+rax*8]
.case0:
        mov     rbx, 100
        jmp     .end
.case1:
        mov     rbx, 101
        jmp     .end
.case2:
        mov     rbx, 102
        jmp     .end
.case3:
        mov     rbx, 103
        jmp     .end
.end:
        xor     eax, eax
        leave
        ret
