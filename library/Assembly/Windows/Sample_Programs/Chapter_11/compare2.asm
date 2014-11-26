        segment .text
        global  main
        extern  printf, scanf
main:
a       equ     local1
b       equ     local2
        push    rbp
        mov     rbp, rsp
        frame   2, 2, 3
        sub     rsp, frame_size
        segment .data
.scanf  db      "%lf %lf",0
.prompt db      "Enter 2 floating point numbers: ",0
        segment .text
.loop   lea     rcx, [.prompt]
        call    printf
        lea     rcx, [.scanf]
        lea     rdx, [rsp+a]
        lea     r8, [rsp+b]
        call    scanf
        cmp     ax, 2
        jne     .done
        movsd   xmm0, [rbp+a]
        movsd   xmm1, [rbp+b]
        ucomisd xmm0, xmm1
        jb      .b
.bret   movsd   xmm0, [rsp+a]
        ucomisd xmm0, xmm1
        jbe     .be
.beret  movsd   xmm0, [rsp+a]
        movsd   xmm1, [rsp+b]
        ucomisd xmm0, xmm1
        jg      .a
.aret   movsd   xmm0, [rsp+a]
        movsd   xmm1, [rsp+b]
        ucomisd xmm0, xmm1
        jae     .ae
.aeret  movsd   xmm0, [rsp+a]
        movsd   xmm1, [rsp+b]
        ucomisd xmm0, xmm1
        je      .e
.eret   movsd   xmm0, [rsp+a]
        movsd   xmm1, [rsp+b]
        ucomisd xmm0, xmm1
        jne     .ne
.neret  jmp     .loop

        segment .data
.printb db      "%f < %f",0x0a,0
        segment .text
.b:
        lea     rcx, [.printb]
        movsd   xmm2, xmm1
        movq    r8, xmm2
        movsd   xmm1, xmm0
        movq    rdx, xmm1
        call    printf
        jmp     .bret

        segment .data
.printbe db      "%f <= %f",0x0a,0
        segment .text
.be:
        lea     rcx, [.printbe]
        movsd   xmm2, xmm1
        movq    r8, xmm2
        movsd   xmm1, xmm0
        movq    rdx, xmm1
        call    printf
        jmp     .beret

        segment .data
.printa db      "%f > %f",0x0a,0
        segment .text
.a:
        lea     rcx, [.printa]
        movsd   xmm2, xmm1
        movq    r8, xmm2
        movsd   xmm1, xmm0
        movq    rdx, xmm1
        call    printf
        jmp     .aret

        segment .data
.printae db      "%f >= %f",0x0a,0
        segment .text
.ae:
        lea     rcx, [.printae]
        movsd   xmm2, xmm1
        movq    r8, xmm2
        movsd   xmm1, xmm0
        movq    rdx, xmm1
        call    printf
        jmp     .aeret

        segment .data
.printe db      "%f == %f",0x0a,0
        segment .text
.e:
        lea     rcx, [.printe]
        movsd   xmm2, xmm1
        movq    r8, xmm2
        movsd   xmm1, xmm0
        movq    rdx, xmm1
        call    printf
        jmp     .eret

        segment .data
.printne db      "%f != %f",0x0a,0
        segment .text
.ne:
        lea     rcx, [.printne]
        movsd   xmm2, xmm1
        movq    r8, xmm2
        movsd   xmm1, xmm0
        movq    rdx, xmm1
        call    printf
        jmp     .neret

.done   mov     eax, 0
        leave
        ret
