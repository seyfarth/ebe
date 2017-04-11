    .section .data
message:
    .string "Hello, world!\n"
    .equ message_size, .-message    
    .section .text
    .globl _start
_start:
    movl $4, %eax
    movl $1, %ebx
    movl $message, %ecx
    movl $message_size, %edx
    int $0x80
    
    movl $1, %eax
    movl $0, %ebx
    int $0x80
