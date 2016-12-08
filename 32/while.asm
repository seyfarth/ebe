        .section .data
format:
        .string "%s\n"
        .section .text
        .global main
main:
        push    %ebp
        mov     %esp, %ebp
        frame   2, 3
        sub     $frame_size, %esp

        xor     %eax, %eax          # loop counter
        mov     %eax, local1(%ebp)
while_top:
        mov     local1(%ebp), %eax
        mov     currPar2(%ebp), %ecx
        mov     (%ecx,%eax,4), %edx
        inc     %eax
        mov     %eax, local1(%ebp)
        push    %edx
        push    $format
        call    printf
        add     $8, %esp
        mov     local1(%ebp), %eax
        cmp     currPar1(%ebp), %eax
        jl      while_top
        leave
        ret
        
