        SECTION .data
data    db      "hello world", 0
length  equ     $ - data - 1
loc     dq      0
n       dq      length
needle  db      'o'

        SECTION .text
        global  main
main:
        push    rbp
        mov     rbp, rsp
        mov     al, [needle]
        mov     ecx, [n]
more:   cmp    [data+rcx-1],al
        je     found
        loop   more
found:  sub    ecx, 1
        mov    [loc], ecx
        xor     eax, eax
        leave
        ret
