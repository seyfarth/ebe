       segment .data
msg:   db      'Hello World!',0x0a     ; String to print
len:   equ     $-msg                   ; String length

       segment .text
       global  main                    ; Tell linker about main
       extern  write, exit

main:
       push    rbp
       mov     rbp, rsp
       frame   2, 2, 1
       sub     rsp, frame_size
       mov     rax, 7
       mov     [rbp+local1], rax
       inc     eax
       mov     [rbp+local2], rax
       mov     edx, len                ; Parameter 3 for write
       lea     rsi, [msg]              ; Parameter 2 for write
       mov     edi, 1                  ; Parameter 1 (fd)
       call    write
;       rdi,  rsi, rdx, rcx, r8, r9
;       xmm0-7

       xor     edi, edi                ; 0 return = success
       call    exit
