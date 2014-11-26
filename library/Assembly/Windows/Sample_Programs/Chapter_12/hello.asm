       segment .data
msg:   db      'Hello World!',0x0a     ; String to print
len:   equ     $-msg                   ; String length

       segment .text
       global  main                    ; Tell linker about main
       extern  write, exit
main:
       push    rbp
       mov     rbp, rsp
       sub     rsp, 32                 ; Must leave room for 4 register params
                                       ; Up to 4 register parameters
                                       ; in rcx, rdx, r8, r9
       mov     r8, len                 ; Parameter 3 for write
       lea     rdx, [msg]              ; Parameter 2 for write
       mov     ecx, 1                  ; Parameter 1 (fd)
       call    write
       xor     ecx, ecx                ; 0 return = success
       call    exit
