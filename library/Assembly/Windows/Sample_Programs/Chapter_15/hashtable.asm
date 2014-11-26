        segment .data
table   times 256 dq    0
        struc  node
n_value resq    1
n_next  resq    1
        align   8
        endstruc

        segment .text
        global  main, insert, print, find, hash
        extern  malloc, scanf, printf

;       i = hash ( n );
hash    mov     rax, rcx
        and     rax, 0xff
        ret

;       insert ( n );
insert:
.n      equ     local1
.h      equ     local2
        push    rbp
        mov     rbp, rsp
        frame   1, 2, 1
        sub     rsp, frame_size
        mov     [rbp+.n], rcx
        call    find
        cmp     rax, 0
        jne     .found
        mov     rcx, [rbp+.n]
        call    hash
        mov     [rbp+.h], rax
        mov     rcx, node_size
        call    malloc
        mov     r9, [rbp+.h]
        mov     r8, [table+r9*8]
        mov     [rax+n_next], r8
        mov     r8, [rbp+.n]
        mov     [rax+n_value], r8
        mov     [table+r9*8], rax
.found  leave
        ret

;       p = find ( n );
;       p = 0 if not found
find:
.n      equ     local1
        push    rbp
        mov     rbp, rsp
        frame   1, 0, 1
        sub     rsp, frame_size
        mov     [rbp+.n], rcx
        call    hash
        mov     rax, [table+rax*8]
        mov     rcx, [rbp+.n]
        cmp     rax, 0
        je      .done
.more   cmp     rcx, [rax+n_value]
        je      .done
        mov     rax, [rax+n_next]
        cmp     rax, 0
        jne     .more
.done   leave
        ret

;       print();
print:
        push    rbp
        mov     rbp, rsp
        frame   0, 2, 2
        sub     rsp, frame_size
        mov     [rbp+local1], r12
        mov     [rbp+local2], r13
        xor     r12d, r12d
.more_table:
        mov     r13, [table+r12*8]
        cmp     r13, 0
        je      .empty
        segment .data
.print1 db      "list %3d: ",0
        segment .text
        lea     rcx, [.print1]
        mov     rdx, r12
        call    printf
.more_list:
        segment .data
.print2 db      "%ld ",0
        segment .text
        lea     rcx, [.print2]
        mov     rdx, [r13+n_value]
        call    printf
        mov     r13, [r13+n_next]
        cmp     r13, 0
        jne     .more_list
        segment .data
.print3 db      0x0a,0
        segment .text
        lea     rcx, [.print3]
        call    printf
.empty  inc     r12
        cmp     r12, 256
        jl      .more_table
        mov     r12, [rbp+local1]
        mov     r13, [rbp+local2]
        pop     r12
        leave
        ret

main:
.k      equ     local1
        segment .data
.scanf_fmt:
        db      "%ld",0
        segment .text
        push    rbp
        mov     rbp, rsp
        frame   2, 1, 2
        sub     rsp, frame_size
.more   lea     rcx, [.scanf_fmt]
        lea     rdx, [rbp+.k]
        call    scanf
        cmp     rax, 1
        jne     .done
        mov     rcx, [rbp+.k]
        call    insert
        call    print
        jmp     .more
.done   leave
        ret
