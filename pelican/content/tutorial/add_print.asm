        segment .data
i       dd      9
j       dd      8
        segment .text
        global  main
        extern  printf
main:
        push    rbp
        mov     rbp, rsp
        mov     esi, [i]
        add     esi, [j]
        segment .data
format: db      'the sum of i and j = %d',0x0a,0
        segment .text
        lea     rdi,[format]
        xor     eax, eax
        call    printf
        xor     eax, eax
        leave
        ret
