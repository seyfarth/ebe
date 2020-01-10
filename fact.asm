        segment .data
x       dq      0
scanf_format   db    "%ld",0
printf_format  db    "fact(%ld) = %ld",0x0a,0

        segment .text
        global  main                ; let the linker know about main
        global  fact                ; tell the world about fact
        extern  scanf               ; resolve write and exit from libc
        extern  printf
main:
        push    rbp
        mov     rbp, rsp
        frame   2, 0, 3
        sub     rsp, frame_size
        lea     rcx, [scanf_format] ; set arg 1 for scanf
        lea     rdx, [x]            ; set arg 2 for scanf
        xor     eax, eax            ; set rax to 0 (2 byte instruction)
        call    scanf
        mov     rcx, [x]            ; move the value of x to rdi for fact call
        ;call    fact
        push    next
        jmp     fact
next:        lea     rcx, [printf_format]    ; set arg 1 for printf
        mov     rdx, [x]            ; set arg 2 for printf
        mov     r8, rax             ; set arg 3 to be x!
        xor     eax, eax            ; set rax to 0
        call    printf
        xor     eax, eax            ; set rax to 0 for return value
        leave
        ret
fact:                               ; recursive factorial function
n       equ     local1
        push    rbp
        mov     rbp, rsp
        frame   1, 1, 1
        sub     rsp, frame_size
        cmp     rcx, 1              ; compare argument with 1
        jg      greater             ; if n <= 1, return 1
        mov     eax, 1              ; set return value to 1
        leave
        ret
greater:
        mov     [rbp+n], rcx        ; save n
        dec     rcx                 ; call fact with n-1
        call    fact
        mov     rcx, [rbp+n]        ; restore original n
        imul    rax, rcx           ; multiply fact(n-1)*n
        leave
        ret

