       segment .data
msg:   db      'Hello World!',0x0a     ; String to print
len:   equ     $-msg                   ; String length
msg2   db      'Hello again!',0x0a, 0
       align   2
nums   dw      1, 2, 3, 5, 6, 0x10ff
ints   dd      1, 2, 0x01020304
z      dq      1, 2, 0x1234567890abcdef
z2     dq      1.1
       segment .bss
x:     resb    10

       segment .text
       global  main                    ; Tell linker about main
       extern  printf
main:
       push    rbp
       mov     rbp, rsp
                                       ; Up to 6 integer register parameters
                                       ; in rdi, rsi, rdx, rcx, r8, r9
                                       ; Up to 8 float register parameters
                                       ; in xmm0-xmm7
            
       lea     rdi, [msg2]             
       mov     eax, 0                 
       call    printf

       xor     eax, eax
       leave
       ret
