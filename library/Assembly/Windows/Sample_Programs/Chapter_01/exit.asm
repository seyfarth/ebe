;   Program: exit
;
;   Calls the exit function
;
;   No input
;
;   Output: only the exit status 
;           %errorlevel%    
;           $? in the the Cygwin shell
;
    segment .text
    global  main
    extern  exit

main:
    push    rbp
    mov     rbp, rsp
    sub     rsp, 32 ; shadow parameter space
    mov     ecx,5   ; parameter for exit
    call    exit
