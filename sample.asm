%include "aal.inc"
        segment .data
message db      "Hello world!",0x0a,0
        segment .text
        global  main
        extern  printf

main:
        push    rfp
        mov     rfp, rsp
        reserve 0
        lea     par1, [message]
%if REG_PARS = 6
        xor     retd, retd
%endif
        call    printf
        leave
        ret
