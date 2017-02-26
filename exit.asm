;   Program: exit
;
;   Executes the exit system call
;
;   No input
;
;   Output: only the exit status ($? in the shell)
;
    segment .text
    global  main
    extern  exit
main:
    push rbp
    mov  rbp, rsp
    sub  rsp, 32
    mov  ecx, 5
    call exit
