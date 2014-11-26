        segment .data
a:      dd      1, 2, 3, 4, 5
        segment .bss
b:      resd    10
        segment .text
        global  main
main:
        push    rbp
        mov     rbp, rsp
        sub     rsp, 32
        lea     rcx, [b]
        lea     rdx, [a]
        mov     r8d, 5
        call    copy_array
        xor     eax, eax
        leave
        ret
copy_array:
        xor     r9d, r9d
more:   mov     eax, [rdx+4*r9]
        mov     [rcx+4*r9], eax
        add     r9, 1
        cmp     r9, r8
        jne     more
        xor     eax, eax
        ret
