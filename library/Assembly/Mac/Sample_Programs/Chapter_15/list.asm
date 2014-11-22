        segment .data
        struc   node
n_value resq    1
n_next  resq    1
        align   8
        endstruc
        segment .text
        global  main, newlist, insert, print
        extern  malloc, printf, scanf

;       list = newlist();
newlist:
        xor     eax, eax
        ret

;       list = insert ( list, k );
insert:
.list   equ     local1
.k      equ     local1
        push    rbp
        mov     rbp, rsp
        frame   2, 2, 1
        sub     rsp, frame_size
        mov     [rbp+.list], rdi  ; save list pointer
        mov     [rbp+.k], rsi     ; and k on stack
        mov     edi, node_size
        call    malloc            ; rax will be node pointer
        mov     r8, [rbp+.list]   ; get list pointer
        mov     [rax+n_next], r8  ; save pointer in node
        mov     r9, [rbp+.k]      ; get k
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
.rbx    equ     local1
        push    rbp
        mov     rbp, rsp
        frame   1, 1, 2
        sub     rsp, frame_size   ; subtract multiples of 16
        mov     [rbp+.rbx], rbx   ; save old value of rbx
        cmp     rdi, 0
        je      .done
        mov     rbx, rdi
.more   lea     rdi, [.print_fmt]
        mov     rsi, [rbx+n_value]
        xor     eax, eax
        call    printf
        mov     rbx, [rbx+n_next]
        cmp     rbx, 0
        jne     .more
.done   lea     rdi, [.newline]
        xor     eax, eax
        call    printf
        mov     rbx, [rbp+.rbx]   ; restore rbx
        leave
        ret

main:
.list   equ     local1
.k      equ     local2
        segment .data
.scanf_fmt:
        db      "%ld",0
        segment .text
        push    rbp
        mov     rbp, rsp
        frame   2, 2, 2
        sub     rsp, frame_size
        call    newlist
        mov     [rbp+.list], rax
.more   lea     rdi, [.scanf_fmt]
        lea     rsi, [rbp+.k]
        xor     eax, eax
        call    scanf
        cmp     rax, 1
        jne     .done
        mov     rdi, [rbp+.list]
        mov     rsi, [rbp+.k]
        call    insert
        mov     [rbp+.list], rax
        mov     rdi, rax
        call    print
        jmp     .more
.done   leave
        ret
