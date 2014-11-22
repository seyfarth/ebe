        segment .text
        global  main
        extern  printf, scanf
a       equ     local1
b       equ     local2
main:
        push    rbp
        mov     rbp, rsp
        frame   2, 2, 3
        sub     rsp, frame_size
        segment .data
.scanf  db      "%lf %lf",0
.prompt db      "Enter 2 floating point numbers: ",0
        segment .text
.loop   lea     rdi, [.prompt]
        xor     eax, eax
        call    printf
        lea     rdi, [.scanf]
        lea     rsi, [rbp+a]
        lea     rdx, [rbp+b]
        xor     eax, eax
        call    scanf
        cmp     ax, 2
        jne     .done
        movsd   xmm0, [rbp+a]
        movsd   xmm1, [rbp+b]
        ucomisd xmm0, xmm1
        jb      .b
.bret   movsd   xmm0, [rbp+a]
        ucomisd xmm0, xmm1
        jbe     .be
.beret  movsd   xmm0, [rbp+a]
        movsd   xmm1, [rbp+b]
        ucomisd xmm0, xmm1
        jg      .a
.aret   movsd   xmm0, [rbp+a]
        movsd   xmm1, [rbp+b]
        ucomisd xmm0, xmm1
        jae     .ae
.aeret  movsd   xmm0, [rbp+a]
        movsd   xmm1, [rbp+b]
        ucomisd xmm0, xmm1
        je      .e
.eret   movsd   xmm0, [rbp+a]
        movsd   xmm1, [rbp+b]
        ucomisd xmm0, xmm1
        jne     .ne
.neret  jmp     .loop

        segment .data
.printb db      "%f < %f",0x0a,0
        segment .text
.b:
        lea     rdi, [.printb]
        mov     eax, 2
        call    printf
        jmp     .bret

        segment .data
.printbe db      "%f <= %f",0x0a,0
        segment .text
.be:
        lea     rdi, [.printbe]
        mov     eax, 2
        call    printf
        jmp     .beret

        segment .data
.printa db      "%f > %f",0x0a,0
        segment .text
.a:
        lea     rdi, [.printa]
        mov     eax, 2
        call    printf
        jmp     .aret

        segment .data
.printae db      "%f >= %f",0x0a,0
        segment .text
.ae:
        lea     rdi, [.printae]
        mov     eax, 2
        call    printf
        jmp     .aeret

        segment .data
.printe db      "%f == %f",0x0a,0
        segment .text
.e:
        lea     rdi, [.printe]
        mov     eax, 2
        call    printf
        jmp     .eret

        segment .data
.printne db      "%f != %f",0x0a,0
        segment .text
.ne:
        lea     rdi, [.printne]
        mov     eax, 2
        call    printf
        jmp     .neret

.done   mov     eax, 0
        leave
        ret
