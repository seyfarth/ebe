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
        push    rbp         ; set up a strack frame for main
        mov     rbp, rsp    ; let rbp point to the previous ssp
        sub     rsp, 16     ; leave some room for local variables
                            ; and align stack to 16 byte boundary
        xor     eax, eax    ; set rax to 0 for return value
        leave               ; undo the stack manipulations
        ret
