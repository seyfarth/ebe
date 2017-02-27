
        segment .data
message db      "Hello world!", 0x0a, 0

        segment .text
        global  main
        extern  printf
main:
        push    rbp
        mov     rbp, rsp
        frame   9, 6, 9
        sub     rsp, frame_size
        mov     [rbp+local1], rsp
        mov     [rbp+local2], rax
        lea     rcx, [message]
        xor     eax, eax          ; Linux: number of floating params
        call    printf
        xor     eax, eax
        leave

        ret
