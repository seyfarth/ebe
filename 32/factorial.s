    ## Simple factorial example
    .section .data
format:      .asciz  "Factorial(%d) = %d\n"
n:  .int     5
 
    .section .bss
    .lcomm   fact, 4

    .section .text
    .globl   main
    .extern  printf
main:
    pushl   %ebp
    movl    %esp, %ebp
    frame   2, 0, 3
    subl    $frame_size, %esp

    movl    n, %eax
    movl    %eax, newPar1(%esp)
    call    factorial
    movl    %eax, fact
    
    lea     format, %eax
    movl    %eax, newPar1(%esp)
    movl    n, %eax
    movl    %eax, newPar2(%esp)
    movl    fact, %eax
    movl    %eax, newPar3(%esp)
    call    printf

    leave
    ret

    ## Factorial function
    ## Returns 1 if the argument is <= 0
    #.type factorial, @function
factorial:
    pushl   %ebp
    movl    %esp, %ebp
    frame   1, 2, 2
    subl    $frame_size, %esp

    movl    currPar1(%ebp), %eax
    .equ    num, currPar1
    movl    num(%ebp), %eax           ## alternative after .equ
    cmpl    $1,%eax
    jle     fac1
    movl    %eax, %ecx
    decl    %ecx
fac_loop:   
    imull   %ecx, %eax
    decl    %ecx
    jnz     fac_loop
    leave
    ret
fac1:
    movl $1,%eax
    leave
    ret
