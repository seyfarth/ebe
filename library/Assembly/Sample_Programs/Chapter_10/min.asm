        segment .text
        extern  printf, random, malloc, atoi
        global  main, create, fill, min

;       array = create ( size );
create:
        push    rbp
        mov     rbp, rsp
        imul    rdi, 4
        call    malloc
        leave
        ret

;       fill ( array, size );
fill:
.array  equ     0
.size   equ     8
.i      equ     16
        push    rbp
        mov     rbp, rsp
        sub     rsp, 32
        mov     [rsp+.array], rdi
        mov     [rsp+.size], rsi
        xor     ecx, ecx
.more   mov     [rsp+.i], rcx
        call    random
        mov     rcx, [rsp+.i]
        mov     rdi, [rsp+.array]
        mov     [rdi+rcx*4], eax
        inc     rcx
        cmp     rcx, [rsp+.size]
        jl      .more
        leave
        ret

;       print ( array, size );
print:
.array  equ     0
.size   equ     8
.i      equ     16
        push    rbp
        mov     rbp, rsp
        sub     rsp, 32
        mov     [rsp+.array], rdi
        mov     [rsp+.size], rsi
        xor     ecx, ecx
        mov     [rsp+.i], rcx
        segment .data
.format:
        db      "%10d",0x0a,0
        segment .text
.more   lea     rdi, [.format]
        mov     rdx, [rsp+.array]
        mov     rcx, [rsp+.i]
        mov     esi, [rdx+rcx*4]
        mov     [rsp+.i], rcx
        xor     eax, eax
        call    printf
        mov     rcx, [rsp+.i]
        inc     rcx
        mov     [rsp+.i], rcx
        cmp     rcx, [rsp+.size]
        jl      .more
        leave
        ret

;       x = min ( array, size );
min:
        mov     eax, [rdi]
        mov     rcx, 1
.more   mov     r8d, [rdi+rcx*4]
        cmp     r8d, eax
        cmovl   eax, r8d
        inc     rcx
        cmp     rcx, rsi
        jl      .more
        ret

main:
.array  equ     0
.size   equ     8
        push    rbp
        mov     rbp, rsp
        sub     rsp, 16

;       set default size
        mov     ecx, 10
        mov     [rsp+.size], rcx

;       check for argv[1] providing a size
        cmp     edi, 2
        jl      .nosize
        mov     rdi, [rsi+8]
        call    atoi
        mov     [rsp+.size], rax
.nosize:

;       create the array
        mov     rdi, [rsp+.size]
        call    create
        mov     [rsp+.array], rax

;       fill the array with random numbers
        mov     rdi, rax
        mov     rsi, [rsp+.size]
        call    fill

;       if size <= 20 print the array
        mov     rsi, [rsp+.size]
        cmp     rsi, 20
        jg      .toobig
        mov     rdi, [rsp+.array]
        call    print
.toobig:

;       print the minimum
        segment .data
.format:
        db      "min: %ld",0xa,0
        segment .text
        mov     rdi, [rsp+.array]
        mov     rsi, [rsp+.size]
        call    min
        lea     rdi, [.format]
        mov     rsi, rax
        xor     eax, eax
        call    printf

        leave
        ret
