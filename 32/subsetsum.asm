#
#       Program to determine if a number can be formed as 
#       the sum of a subset of numbers.  The first number 
#       on the command line is the target number and the
#       rest of the numbers are candidates for forming the
#       sum.

        .section .text
        .global  main
main:
        push    %ebp
        mov     %esp, %ebp
        frame   2, 0
        sub     $frame_size, %esp
        
        mov     currPar1(%ebp), %eax
        sub     $2, %eax
        mov     %eax, n  
        
        mov     currPar2(%ebp), %edx
        add     $4, %edx
        mov     %edx, argv
        push    (%edx)
        add     $4, %edx
        mov     %edx, argv
        
        call    atoi
        add     $4, %esp
        mov     %eax, target
        
        call    build_arrays
        
        push    used
        push    values
        push    n 
        push    target        
        call    subsetsum
        add     $16, %esp
        
        cmp     $1, %al 
        jne     .no_solution
        call    print_solution
        jmp     .done
.no_solution:
        push    $fail
        call    printf
.done:  leave
        ret 

        .section .data
n:      .int    0
argv:   .int    0
target: .int    0
values: .int    0       # pointer to int array
used:   .int    0       # pointer to int array
fmt1:   .asciz  "%d = "
fmt2:   .asciz  "%d "
fmt3:   .asciz  "\n"
fail:   .asciz  "No solution\n"

        .section .text
print_solution:
        push    %ebp
        mov     %esp, %ebp
        frame   0, 0
     
        push    %ebx
        push    %edi
        push    %esi

        push    target 
        push    $fmt1
        call    printf
        add     $8, %esp
        
        xor     %ebx, %ebx
        mov     used, %edi
        mov     values, %esi
.print_top:
        mov     (%edi,%ebx,4), %eax
        cmp     $0, %eax
        jz      .skipit
        mov     (%esi,%ebx,4), %eax
        push    %eax
        push    $fmt2
        call    printf
        add     $8, %esp
.skipit:  inc     %ebx
        cmp     n, %ebx
        jl      .print_top
             
        push    $fmt3
        call    printf
        add     $4, %esp
        pop     %esi
        pop     %edi
        pop     %ebx
        
        leave
        ret
        
subsetsum:
#   bool subsetsum ( int target, int n,
#                    int values[], int used[] )
        push    %ebp
        mov     %esp, %ebp
        frame   4, 0
        sub     $frame_size, %esp
        
        mov     currPar2(%ebp), %eax
        cmp     $0, %eax
        jg      try_next
        mov     currPar1(%ebp), %ecx
        cmp     $0, %ecx
        jne     .done_sss
        mov     $1, %eax
        jmp     .done_sss
        
try_next:
        mov     $1, %eax
        mov     currPar4(%ebp), %edx
        mov     %eax, (%edx)
        add     $4, %edx
        push    %edx
        mov     currPar3(%ebp), %edx
        mov     (%edx), %ecx
        add     $4, %edx
        push    %edx
        mov     currPar2(%ebp), %eax
        dec     %eax
        push    %eax
        mov     currPar1(%ebp), %eax
        sub     %ecx, %eax
        push    %eax
        call    subsetsum
        add     $16, %esp
        cmp     $1, %al 
        je      .done_sss
        xor     %eax, %eax         
        mov     currPar4(%ebp), %edx
        mov     %eax, (%edx)
        add     $4, %edx
        push    %edx
        mov     currPar3(%ebp), %edx
        add     $4, %edx
        push    %edx
        mov     currPar2(%ebp), %eax
        dec     %eax
        push    %eax
        mov     currPar1(%ebp), %eax
        push    %eax
        call    subsetsum
        add     $16, %esp
                  
.done_sss:  
        leave
        ret
        
        
build_arrays:
        push    %ebp
        mov     %esp, %ebp
        frame   0, 1
        sub     $frame_size, %esp
        
        mov     n, %eax
        shl     $2, %eax
        push    %eax
        call    malloc
        add     $4, %esp
        mov     %eax, values
        
        mov     n, %eax
        shl     $2, %eax
        push    %eax
        call    malloc
        add     $4, %esp
        mov     %eax, used
        
        xor     %ecx, %ecx
        mov     %ecx, local1(%ebp)
.top:
        mov     argv, %edx
        mov     (%edx,%ecx,4), %eax
        mov     %ecx, local1(%ebp)
        push    %eax
        call    atoi
        add     $4, %esp
        mov     local1(%ebp), %ecx
        mov     values, %edx
        mov     %eax, (%edx,%ecx,4)
        inc     %ecx
        cmp     n, %ecx
        jl      .top
       
        leave
        ret

