        ;   Program not yet converted!!!!


        segment .data
        struc   node
n_value resq    1
n_left  resq    1
n_right resq    1
        align   8
        endstruc
        struc   tree
t_count resq    1
t_root  resq    1
        align   8
        endstruc

        segment .text
        global  main, new_tree, insert, print, rec_print, find
        extern  malloc, scanf, printf

new_tree:
        push    rbp
        mov     rbp, rsp
        mov     rdi, tree_size
        call    malloc
        xor     edi, edi
        mov     [rax+t_root], rdi
        mov     [rax+t_count], rdi
        leave
        ret

;       insert ( t, n );
insert:
.n      equ     0
.t      equ     8
        push    rbp
        mov     rbp, rsp
        sub     rsp, 16
        mov     [rsp+.t], rdi
        mov     [rsp+.n], rsi
        call    find
        cmp     rax, 0
        jne     .done 
        mov     rdi, node_size
        call    malloc
        mov     rsi, [rsp+.n]
        mov     [rax+n_value], rsi
        xor     edi, edi
        mov     [rax+n_left], rdi
        mov     [rax+n_right], rdi
        mov     rdx, [rsp+.t]
        mov     rdi, [rdx+t_count]
        cmp     rdi, 0
        jne     .findparent
        inc     qword [rdx+t_count]
        mov     [rdx+t_root], rax
        jmp     .done
.findparent:
        inc     qword [rdx+t_count]
        mov     rdx, [rdx+t_root]
.repeatfind:
        cmp     rsi, [rdx+n_value]
        jl      .goleft
        mov     r8, rdx
        mov     rdx, [r8+n_right]
        cmp     rdx, 0
        jne     .repeatfind
        mov     [r8+n_right], rax
        jmp     .done
.goleft:
        mov     r8, rdx
        mov     rdx, [r8+n_left]
        cmp     rdx, 0
        jne     .repeatfind
        mov     [r8+n_left], rax
.done   leave
        ret

;       p = find ( t, n );
;       p = 0 if not found
find:
        push    rbp
        mov     rbp, rsp
        mov     rdi, [rdi+t_root]
        xor     eax, eax
.more   cmp     rdi, 0
        je      .done
        cmp     rsi, [rdi+n_value]
        jl      .goleft
        jg      .goright
        mov     rax, rsi
        jmp     .done
.goleft:
        mov     rdi, [rdi+n_left]
        jmp     .more
.goright:
        mov     rdi, [rdi+n_right]
        jmp     .more
.done   leave
        ret

rec_print:
.t      equ     0
        push    rbp
        mov     rbp, rsp
        sub     rsp, 16
        cmp     rdi, 0
        je      .done
        mov     [rsp+.t], rdi
        mov     rdi, [rdi+n_left]
        call    rec_print
        mov     rdi, [rsp+.t]
        mov     rsi, [rdi+n_value]
        segment .data
.print  db      "%ld ",0
        segment .text
        lea     rdi, [.print]
        xor     eax, eax
        call    printf
        mov     rdi, [rsp+.t]
        mov     rdi, [rdi+n_right]
        call    rec_print
.done   leave
        ret

;       print(t);
print:
        push    rbp
        mov     rbp, rsp
        mov     rdi, [rdi+t_root]
        call    rec_print
        segment .data
.print  db      0x0a, 0
        segment .text
        lea     rdi, [.print]
        xor     eax, eax
        call    printf
        leave
        ret

main:
.k      equ     0
.t      equ     8
        segment .data
.scanf_fmt:
        db      "%ld",0
        segment .text
        push    rbp
        mov     rbp, rsp
        sub     rsp, 16
        call    new_tree
        mov     [rsp+.t], rax
.more   lea     rdi, [.scanf_fmt]
        lea     rsi, [rsp+.k]
        xor     eax, eax
        call    scanf
        cmp     rax, 1
        jne     .done
        mov     rdi, [rsp+.t]
        mov     rsi, [rsp+.k]
        call    insert
        mov     rdi, [rsp+.t]
        call    print
        jmp     .more
.done   leave
        ret
