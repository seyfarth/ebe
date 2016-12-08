#
#       Program to test bit field extraction and insertion
#
        .section .text
        .global  main
main:
        push    %ebp
        mov     %esp, %ebp
        frame   2, 2
        sub     $frame_size, %esp

        push    $prompt
        call    printf
        add     $4, %esp

        push    $count
        push    $to
        push    $from
        push    $read_fmt
        call    scanf
        add     $16, %esp

        push    count
        push    from
        push    $array
        call    extract
        add     $12, %esp
        
        push    count
        push    to
        push    $array
        push    %eax
        call    insert
        add     $12, %esp
        
        mov     %eax, field
        leave
        ret
    
extract:
        push    %ebp
        mov     %esp, %ebp
        frame   3, 2
        sub     $frame_size, %esp
        
        push    %ebx
        push    %esi
        push    %edi
        mov     currPar1(%ebp), %edx
        mov     currPar2(%ebp), %ecx
        
#       Determine first word for extraction
#       and fetch it in eax

        mov     %ecx, %ebx
        shr     $5, %ebx
        mov     %ebx, local1(%ebp)
        mov     (%edx,%ebx,4), %eax

#       Get number of bits to extract in edi
        
        mov     currPar3(%ebp), %edi      

#       Determine first bit of eax to fetch
#       and shift right to flush other bits

        and     $0x1f, %ecx
        shr     %cl, %eax
        
#       Determine the number of bits to use
#       from the first word

        sub     $32, %ecx
        neg     %ecx
        mov     %ecx, local2(%ebp)
        
#       Determine bit number of last bit to extract

        mov     currPar2(%ebp), %ecx
        add     %ecx, %edi
        mov     %edi, %esi
        dec     %esi
        
#       Determine dword index of last bit
#       and complete the extraction if it
#       the same as the first dword index

        shr     $5, %esi
        cmp     local1(%ebp),%esi
        je      one_dword

#       Determine how many bits are not needed
#       in the second dword and flush
        
        and     $0x1f, %edi
        mov     $32, %ecx
        sub     %edi, %ecx
        mov     4(%edx,%ebx,4), %ebx
        shl     %cl, %ebx

#       Subtract the first bit number and
#       shift right to align for or of the parts

        sub     local2(%ebp), %ecx
        shr     %cl, %ebx
        or      %ebx, %eax
        jmp     end
         
        
one_dword:

#       Shift left and right to clear out unneeded
#       high order bits

        mov     $32, %ecx
        sub     currPar3(%ebp), %ecx
        shl     %cl, %eax
        shr     %cl, %eax

end:
        pop     %edi
        pop     %esi
        pop     %ebx
        leave
        ret
        
insert:
        .equ    .count, currPar4
        .equ    .to,    currPar3
        .equ    .array, currPar2
        .equ    .field, currPar1
        .equ    .dword, local1
        .equ    .bits_dword1, local2
        push    %ebp
        mov     %esp, %ebp
        frame   4, 2
        sub     $frame_size, %esp

#       eax = field to insert
#       edx = array address
#       ebx = starting bit
#       edi = count

        push    %ebx
        push    %esi
        push    %edi
        
        mov     .array(%ebp), %edx
        mov     .to(%ebp), %ebx
        mov     .count(%ebp), %edi
        
#       Work on first word
#       
        mov     %ebx, %esi
        shr     $5, %ebx
        mov     %ebx, .dword(%ebp)
        mov     (%edx,%ebx,4), %ebx

#       Now ebx has the first dword
       
#       Need to determine number of high order
#       bits of ebx to clear out

        and     $0x1f, %esi     # first bit
        mov     $32, %ecx
        sub     %esi, %ecx      # number of bits
        mov     %ecx, .bits_dword1(%ebp)  
#       Zero out %ecx bits from %esi to 31
    
        mov     $-1, %eax
        shr     %cl, %eax
        and     %eax, %ebx
        
#       Zero out upper bits of field and or with %ebx
        
        mov     .field(%ebp), %esi
        mov     $-1, %eax
        mov     $32, %edi
        sub     %ecx, %edi
        mov     %edi, %ecx
        shr     %cl, %eax
        and     %eax, %esi
        shl     %cl, %esi     
        or      %esi, %ebx
        
#       Complete first word

        mov     .dword(%ebp), %eax
        mov     %ebx, (%edx,%eax,4)

#       Determine if there are more bits to insert

        mov     .to(%ebp), %ebx
        mov     %ebx, %ecx
        mov     .count(%ebp), %edi
        add     %edi, %ebx
        dec     %ebx
        
#       ebx is the last bit number to store

        shr     $5, %ebx
        cmp     %eax, %ebx
        je      .end

#       need to fetch next dword and 
#       place 0's in first ecx low order bits

        mov     (%edx,%ebx,4), %eax    # next dword
        mov     $-1, %edi
        shl     %cl, %edi       
        and     %edi, %eax
        
#       need to or together the highest ecx bits
#       of the field with %eax 
         
        mov     .field(%ebp), %esi
        mov     .bits_dword1(%ebp), %ecx
        shr     %cl, %esi
        or      %esi, %eax
        mov     %eax, (%edx,%ebx,4)

.end:
        pop     %edi
        pop     %esi
        pop     %ebx
        leave
        ret

        .section .data
from:   .int    0
to:     .int    0
count:  .int    0
array:  .int    0x12345678, 0x54321789, 0x321fedcb, 0xdeadbeef
field:  .int    0
prompt: .asciz  "Enter from, to and count: "
read_fmt:
        .asciz  "%d %d %d"
