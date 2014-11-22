;   Program: exit
;
;   Executes the exit system call
;
;   No input
;
;   Output: only the exit status ($? in the shell)
;
    segment .text
    global  start

start:
    mov  eax,0x2000001  ; 0x2000001 is the exit syscall number
                        ; 60 for Linux
    mov  edi,5          ; the status value to return
    syscall             ; execute a system call
