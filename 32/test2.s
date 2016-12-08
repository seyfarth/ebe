
.section .data
message:
        .string "Please enter a message: "
        .equ message_size, .-message
response:
        .string "Your message is: "
        .equ response_size, .-response

        .section .bss
        .lcomm inbuf, 256

        .section .text
        .globl _start
_start:
        movl %esp, %ebp
        pushl $123
        cmpl $75, (%esp)
        movl $4, %eax
        movl $1, %ebx
        movl $message, %ecx
        movl $message_size, %edx
        int $0x80

        movl $3, %eax
        movl $0, %ebx
        movl $inbuf, %ecx
        movl $256, %edx
        int $0x80
        movl %eax, %edi

        movl $0, %eax
        incl %eax
        decl %eax

        movl $4, %eax
        movl $1, %ebx
        movl $response, %ecx
        movl $response_size, %edx
        int $0x80

        movl $4, %eax
        movl $1, %ebx
        movl $inbuf, %ecx
        movl %edi, %edx
        int $0x80

        movl $1, %eax
        movl $0, %ebx
        int $0x80
