        segment .text
        cname   popcnt_array
        global  popcnt_array           ; let the linker know about popcnt_array

popcnt_array:
        push    rbx
        push    rbp
        push    r12
        push    r13
        push    r14
        push    r15
        xor     eax, eax
        xor     ebx, ebx
        xor     ecx, ecx
        xor     edx, edx
        xor     r12d, r12d
        xor     r13d, r13d
        xor     r14d, r14d
        xor     r15d, r15d
.count_words:
        mov     r8, [rdi]
        mov     r9, r8
        mov     r10, r8
        mov     r11, r9
        and     r8, 0xffff
        shr     r9, 16
        and     r9, 0xffff
        shr     r10, 32
        and     r10, 0xffff
        shr     r11, 48
        and     r11, 0xffff

        mov     r12w, r8w
        and     r12w, 1
        add     rax, r12
        mov     r13w, r9w
        and     r13w, 1
        add     rbx, r13
        mov     r14w, r10w
        and     r14w, 1
        add     rcx, r14
        mov     r15w, r11w
        and     r15w, 1
        add     rdx, r15

%rep 15
        shr     r8w, 1
        mov     r12w, r8w
        and     r12w, 1
        add     rax, r12
        shr     r9w, 1
        mov     r13w, r9w
        and     r13w, 1
        add     rbx, r13
        shr     r10w, 1
        mov     r14w, r10w
        and     r14w, 1
        add     rcx, r14
        shr     r11w, 1
        mov     r15w, r11w
        and     r15w, 1
        add     rdx, r15
%endrep
        add     rdi, 8
        dec     rsi
        jg      .count_words
        add     rax, rbx
        add     rax, rcx
        add     rax, rdx
        pop     r15
        pop     r14
        pop     r13
        pop     r12
        pop     rbp
        pop     rbx
        ret
