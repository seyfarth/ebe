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
.n      equ     local1
.h      equ     local2
        push    rbp
        mov     rbp, rsp
        frame   1, 2, 1
        sub     rsp, frame_size
        mov     [rbp+.n], rdi
        call    find
        cmp     rax, 0
        jne     .found
        mov     rdi, [rbp+.n]
        call    hash
        mov     [rbp+.h], rax
        mov     rdi, node_size
        call    malloc
        mov     r9, [rbp+.h]
        lea     rsi, [table]
        mov     r8, [rsi+r9*8]
        mov     [rax+n_next], r8
        mov     r8, [rbp+.n]
        mov     [rax+n_value], r8
        mov     [rsi+r9*8], rax
.found  leave
        ret

;       p = find ( n );
;       p = 0 if not found
find:
.n      equ     local1
        push    rbp
        mov     rbp, rsp
        frame   1, 1, 1
        sub     rsp, frame_size
        mov     [rbp+.n], rdi
        call    hash
        lea     rsi, [table]
        mov     rax, [rsi+rax*8]
        mov     rdi, [rbp+.n]
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
.r12    equ     local1
.r13    equ     local2
        push    rbp
        mov     rbp, rsp
        frame   0, 2, 2
        ; preserve r12 and r13 which will survive function calls
        sub     rsp, frame_size
        mov     [rbp+.r12], r12 ; r12 is i: integer counter for table
        mov     [rbp+.r13], r13 ; r13 is p: pointer for list at table[i]
        xor     r12, r12
.more_table:
        lea     rsi, [table]
        mov     r13, [rsi+r12*8]
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
        mov     r12, [rbp+.r12]
        mov     r13, [rbp+.r13]
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
.more   lea     rdi, [.scanf_fmt]
        lea     rsi, [rbp+.k]
        xor     eax, eax
        call    scanf
        cmp     rax, 1
        jne     .done
        mov     rdi, [rbp+.k]
        call    insert
        call    print
        jmp     .more
.done   leave
        ret
