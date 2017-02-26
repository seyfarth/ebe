        segment .data
a       dd      4
b       dd      4.4
c       times   10 dd 0
d       dw      1, 2
e       db      0xfb
f       db      "hello world", 0

        segment .bss
g       resd    1
h       resd    10
i       resb    100

        section .text
        global  main        ; let the linker know about main
main:
        push    rbp
        mov     rbp, rsp
        sub     rsp, 24     ; leave room for shadow parameteres
        xor     eax, eax    ; set rax to 0 for return value
        leave               ; undo the stack manipulations
        ret
