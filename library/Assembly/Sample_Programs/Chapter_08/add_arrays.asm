        segment .data
n       dq      5
a       dq      1, 2, 3, 4, 5
b       dq      10, 20, 30, 40, 50
c       dq      0, 0, 0, 0, 0
        segment .text
        global  main
main:
        push    rbp
        mov     rbp, rsp
        mov     rdx, [n]
        xor     ecx, ecx
for:    cmp     rcx, rdx
        je      end_for
        mov     rax, [a+rcx*8]
        add     rax, [b+rcx*8]
        mov     [c+rcx*8], rax
        inc     rcx
        jmp for
end_for:
        xor eax, eax
        leave
        ret
