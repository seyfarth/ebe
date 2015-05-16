       segment .text
       global  main                    ; Tell linker about main
       extern  exit
main:
       push    rbp
       mov     rbp, rsp
       sub     rsp, 32

       mov     rax, 5
       call    exit
