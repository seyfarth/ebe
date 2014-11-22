        segment .text
        cname   popcnt_array
        global  popcnt_array           ; let the linker know about popcnt_array

popcnt_array:
        push    r12
        push    r13
        push    r14
        push    r15
        xor     eax, eax
        xor     r8d, r8d
        xor     ecx, ecx
        xor     r14d, r14d
        xor     r15d, r15d
.count_more:
        popcnt  rdx, [rdi+rcx*8]
        add     rax, rdx
        popcnt  r9, [rdi+rcx*8+8]
        add     r8, r9
        popcnt  r12, [rdi+rcx*8+16]
        add     r14, r12
        popcnt  r13, [rdi+rcx*8+24]
        add     r15, r13
        add     rcx, 4
        cmp     rcx, rsi
        jl      .count_more
        add     rax, r8
        add     rax, r14
        add     rax, r15
        pop     r15
        pop     r14
        pop     r13
        pop     r12
        ret
