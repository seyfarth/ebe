        segment .data
format  db      "%s",0x0a,0
        segment .text
        global  main             ; let the linker know about main
        extern  printf           ; resolve printf from libc
main:
        push    rbp              ; prepare stack frame for main
        mov     rbp, rsp
        frame   2, 1, 3
        sub     rsp, frame_size
        mov     r8, rdx          ; move argv to r8
        mov     rdx, [r8]        ; get first argv string
start_loop:
        lea     rcx, [format]
        mov     [rbp+local1], r8 ; save argv
        call    printf
        mov     r8, [rbp+local1] ; restore argv
        add     r8, 8            ; advance to the next pointer in argv
        mov     rdx, [r8]        ; get next argv string
        cmp     rdx, 0           ; it's sad that mov doesn't also test
        jnz     start_loop                                                              ; end with NULL pointer
end_loop:
        xor     eax, eax
        leave
        ret
