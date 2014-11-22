        segment .data
;             0000 0001 0010 0011 0100 0101 0110 0111 1000 1001 1010 1011 1100 1101 1110 1111
count   db       0,   1,   1,   2,   1,   2,   2,   3,   1,   2,   2,   3,   2,   3,   3,   4  ; 0000
        db       1,   2,   2,   3,   2,   3,   3,   4,   2,   3,   3,   4,   3,   4,   4,   5  ; 0001
        db       1,   2,   2,   3,   2,   3,   3,   4,   2,   3,   3,   4,   3,   4,   4,   5  ; 0010
        db       2,   3,   3,   4,   3,   4,   4,   5,   3,   4,   4,   5,   4,   5,   5,   6  ; 0011
        db       1,   2,   2,   3,   2,   3,   3,   4,   2,   3,   3,   4,   3,   4,   4,   5  ; 0100
        db       2,   3,   3,   4,   3,   4,   4,   5,   3,   4,   4,   5,   4,   5,   5,   6  ; 0101
        db       2,   3,   3,   4,   3,   4,   4,   5,   3,   4,   4,   5,   4,   5,   5,   6  ; 0110
        db       3,   4,   4,   5,   4,   5,   5,   6,   4,   5,   5,   6,   5,   6,   6,   7  ; 0111
        db       1,   2,   2,   3,   2,   3,   3,   4,   2,   3,   3,   4,   3,   4,   4,   5  ; 1000
        db       2,   3,   3,   4,   3,   4,   4,   5,   3,   4,   4,   5,   4,   5,   5,   6  ; 1001
        db       2,   3,   3,   4,   3,   4,   4,   5,   3,   4,   4,   5,   4,   5,   5,   6  ; 1010
        db       3,   4,   4,   5,   4,   5,   5,   6,   4,   5,   5,   6,   5,   6,   6,   7  ; 1011
        db       2,   3,   3,   4,   3,   4,   4,   5,   3,   4,   4,   5,   4,   5,   5,   6  ; 1100
        db       3,   4,   4,   5,   4,   5,   5,   6,   4,   5,   5,   6,   5,   6,   6,   7  ; 1101
        db       3,   4,   4,   5,   4,   5,   5,   6,   4,   5,   5,   6,   5,   6,   6,   7  ; 1110
        db       4,   5,   5,   6,   5,   6,   6,   7,   5,   6,   6,   7,   6,   7,   7,   8  ; 1111
        segment .text
        cname popcnt_array
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
        lea     r11, [count]
.count_more:
        mov     rbp, [rdi]
        mov     r12b, bpl
        mov     r12b, byte [r11+r12]
        add     rax, r12
        shr     rbp, 8
        mov     r13b, bpl
        mov     r13b, byte [r11+r13]
        add     rbx, r13
        shr     rbp, 8
        mov     r14b, bpl
        mov     r14b, byte [r11+r14]
        add     rcx, r14
        shr     rbp, 8
        mov     r15b, bpl
        mov     r15b, byte [r11+r15]
        add     rdx, r15
        shr     rbp, 8
        mov     r12b, bpl
        mov     r12b, byte [r11+r12]
        add     rax, r12
        shr     rbp, 8
        mov     r13b, bpl
        mov     r13b, byte [r11+r13]
        add     rbx, r13
        shr     rbp, 8
        mov     r14b, bpl
        mov     r14b, byte [r11+r14]
        add     rcx, r14
        shr     rbp, 8
        mov     r15b, bpl
        mov     r15b, byte [r11+r15]
        add     rdx, r15
        add     rdi, 8
        dec     rsi
        jg      .count_more
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
