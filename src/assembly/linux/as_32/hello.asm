        .section .data
hello:  .asciz   "Hello world!\n" # String ending in a 0 byte - C String
        .section .text
        .globl  main
        .extern printf
main:
        push    %ebp
        mov     %esp, %ebp # setup stack frame
        frame   2, 0       # stack frame with 2 parameters to main
                           # and 0 local variables
        sub     $frame_size, %esp
        push    $hello
        call    printf
        sub     $4, %esp
        xor     %eax, %eax
        leave
        ret
