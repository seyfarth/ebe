        segment .text
        global  main
        extern  rand, srand
main:
        push    rbp
        mov     rbp, rsp
        mov     edi, 1
        call    srand
top     call    rand
        jmp     top
        leave
        ret

