        ;   Program not yet converted!!!!


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
hash    mov     rax, rdi
        and     rax, 0xff
        ret

;       insert ( n );
insert:
.n      equ     0
.h      equ     8
        push    rbp
        mov     rbp, rsp
        sub     rsp, 16
        mov     [rsp+.n], rdi
        call    find
        cmp     rax, 0
        jne     .found
        mov     rdi, [rsp+.n]
        call    hash
        mov     [rsp+.h], rax
        mov     rdi, node_size
        call    malloc
        mov     r9, [rsp+.h]
        mov     r8, [table+r9*8]
        mov     [rax+n_next], r8
        mov     r8, [rsp+.n]
        mov     [rax+n_value], r8
        mov     [table+r9*8], rax
.found  leave
        ret

;       p = find ( n );
;       p = 0 if not found
find:
.n      equ     0
        push    rbp
        mov     rbp, rsp
        sub     rsp, 16
        mov     [rsp+.n], rdi
        call    hash
        mov     rax, [table+rax*8]
        mov     rdi, [rsp+.n]
        cmp     rax, 0
        je      .done
.more   cmp     rdi, [rax+n_value]
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
        sub     rsp, 16
        push    r12         ; i: integer counter for table
        push    r13         ; p: pointer for list at table[i]
        xor     r12, r12
.more_table:
        mov     r13, [table+r12*8]
        cmp     r13, 0
        je      .empty
        segment .data
.print1 db      "list %3d: ",0
        segment .text
        lea     rdi, [.print1]
        mov     rsi, r12
        xor     eax, eax
        call    printf
.more_list:
        segment .data
.print2 db      "%ld ",0
        segment .text
        lea     rdi, [.print2]
        mov     rsi, [r13+n_value]
        xor     eax, eax
        call    printf
        mov     r13, [r13+n_next]
        cmp     r13, 0
        jne     .more_list
        segment .data
.print3 db      0x0a,0
        segment .text
        lea     rdi, [.print3]
        xor     eax, eax
        call    printf
.empty  inc     r12
        cmp     r12, 256
        jl      .more_table
        pop     r13
        pop     r12
        leave
        ret

main:
.k      equ     0
        segment .data
.scanf_fmt:
        db      "%ld",0
        segment .text
        push    rbp
        mov     rbp, rsp
        sub     rsp, 16
.more   lea     rdi, [.scanf_fmt]
        lea     rsi, [rsp+.k]
        xor     eax, eax
        call    scanf
        cmp     rax, 1
        jne     .done
        mov     rdi, [rsp+.k]
        call    insert
        call    print
        jmp     .more
.done   leave
        ret
