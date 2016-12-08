        segment .data
data    dq      0
n       dq      10
        segment .text
        global  main
        extern  malloc
main:
        push    rbp
        mov     rbp, rsp
        mov     rdi, [n]    ; number of array elements
        shl     rdi, 3      ; multiply by sizeof(qword)
        call    malloc
        mov     [data], rax
        mov     rcx, [n]
more    mov     [rax+rcx*8-8], rcx
        dec     rcx
        jge     more
        leave
        ret
