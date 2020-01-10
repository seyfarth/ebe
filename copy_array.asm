        segment .data
a:      dd      1, 2, 3, 4, 5
        segment .bss
b:      resd    10
        segment .text
        global  main, copy_array
main:
        push    rbp
        mov     rbp, rsp
        frame   2, 0, 3
        sub     rsp, frame_size
        lea     rcx, [b]   ; destination
        lea     rdx, [a]   ; source
        mov     r8d, 5     ; count
        call    copy_array
        xor     eax, eax   ; return 0
        leave
        ret
copy_array:
        push    rbp
        mov     rbp, rsp
        frame   3, 0, 0
        sub     rsp, frame_size
        xor     r9d, r9d         ; start with 0 for index
more:   mov     eax, [rdx+4*r9]  ; load dword
        mov     [rcx+4*r9], eax  ; store qword
        add     r9d, 1           ; increment index register
        cmp     r9, r8           ; index vs count register
        jne     more             ; if not equal, more to do
        leave
        ret
