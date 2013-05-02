        ;   Program not yet converted!!!!


        segment .data
        struc   node
n_value resq    1
n_next  resq    1
n_prev  resq    1
        align   8
        endstruc
        segment .text
        global  main, newlist, insert, print
        extern  malloc, printf, scanf

;       list = newlist();
newlist:
        push    rbp
        mov     rbp, rsp
        mov     edi, node_size
        call    malloc
        mov     [rax+n_next], rax
        mov     [rax+n_prev], rax
        leave
        ret

;       insert ( list, k );
insert:
.list   equ     0
.k      equ     8
        push    rbp
        mov     rbp, rsp
        sub     rsp, 16
        mov     [rsp+.list], rdi  ; save list pointer
        mov     [rsp+.k], rsi     ; and k on stack
        mov     edi, node_size
        call    malloc            ; rax will be node pointer
        mov     r8, [rsp+.list]   ; get list pointer
        mov     r9, [r8+n_next]   ; get head's next
        mov     [rax+n_next], r9  ; set new node's next
        mov     [rax+n_prev], r8  ; set new node's prev
        mov     [r8+n_next], rax  ; set head's next
        mov     [r9+n_prev], rax  ; set new node's next's prev
        mov     r9, [rsp+.k]      ; get k
        mov     [rax+n_value], r9 ; save k in node
        leave
        ret

;       print ( list );
print:
        segment .data
.print_fmt:
        db      "%ld ",0
.newline:
        db      0x0a,0
        segment .text
.list   equ     0
.rbx    equ     8
        push    rbp
        mov     rbp, rsp
        sub     rsp, 16
        mov     [rsp+.rbx], rbx
        mov     [rsp+.list], rdi
        mov     rbx, [rdi+n_next]
        cmp     rbx, [rsp+.list]
        je      .done
.more   lea     rdi, [.print_fmt]
        mov     rsi, [rbx+n_value]
        xor     eax, eax
        call    printf
        mov     rbx, [rbx+n_next]
        cmp     rbx, [rsp+.list]
        jne     .more
.done   lea     rdi, [.newline]
        xor     eax, eax
        call    printf
        mov     rbx, [rsp+.rbx]
        leave
        ret

main:
.list   equ     0
.k      equ     8
        segment .data
.scanf_fmt:
        db      "%ld",0
        segment .text
        push    rbp
        mov     rbp, rsp
        sub     rsp, 16
        call    newlist
        mov     [rsp+.list], rax
.more   lea     rdi, [.scanf_fmt]
        lea     rsi, [rsp+.k]
        xor     eax, eax
        call    scanf
        cmp     rax, 1
        jne     .done
        mov     rdi, [rsp+.list]
        mov     rsi, [rsp+.k]
        call    insert
        mov     rdi, [rsp+.list]
        call    print
        jmp     .more
.done   leave
        ret
