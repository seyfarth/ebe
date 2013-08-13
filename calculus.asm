        segment .text
        global  main
        extern  malloc, printf, scanf



   main:
.n      equ     0           ; degree of the polynomial
.a      equ     8
.b      equ     16
.p      equ     24          ; polynomial
.pp     equ     32          ; p' = derivative
.P      equ     40          ; Integral
.i      equ     48          ; loop counter
.Pa     equ     56
.Pb     equ     64
        push    rbp
        mov     rbp, rsp
        sub     rsp, 80

;       prompt for and read n
        segment .data
.n_prompt:
        db      "Enter the degree of the polynomial: ",0
        segment .text
        lea     rdi, [.n_prompt]
        xor     eax, eax
        call    printf
        segment .data
.n_scanf:
        db      "%ld",0
        segment .text
        lea     rdi, [.n_scanf]
        lea     rsi, [rsp+.n]
        xor     eax, eax
        call    scanf

;       allocate pp, p and P
        mov     rdi, [rsp+.n]
        call    create
        mov     [rsp+.pp], rax
        mov     rdi, [rsp+.n]
        inc     rdi
        call    create
        mov     [rsp+.p], rax
        mov     rdi, [rsp+.n]
        add     rdi, 2
        call    create
        mov     [rsp+.P], rax

;       prompt for and read p
        segment .data
.p_prompt:
        db      "Enter %ld numbers for p: ",0
        segment .text
        lea     rdi, [.p_prompt]
        mov     rsi, [rsp+.n]
        inc     rsi
        xor     eax, eax
        call    printf
;       for ( i = 0; i &lt;= n; i++ ) {
        xor     ecx, ecx                ; i = 0
        mov     [rsp+.i], rcx
.for    mov     rdx, [rsp+.n]
        cmp     rcx, rdx                ; i &lt;= n ?
        jg      .end_for
;           scanf ( "%lf", p+i );
            segment .data
.p_scanf:   db      "%lf",0
            segment .text
            lea     rdi, [.p_scanf]
            mov     rsi, [rsp+.p]
            lea     rsi, [rsi+8*rcx]
            xor     eax, eax
            call    scanf
            mov     rcx, [rsp+.i]
            inc     rcx
            mov     [rsp+.i], rcx
            jmp     .for
;       }
.end_for:
;       prompt for and read a and b
        segment .data
.ab_prompt:
        db      "Enter a and b: ",0
        segment .text
        lea     rdi, [.ab_prompt]
        xor     eax, eax
        call    printf
        segment .data
.ab_read:
        db      "%lf %lf",0
        segment .text
        lea     rdi, [.ab_read]
        lea     rsi, [rsp+.a]
        lea     rdx, [rsp+.b]
        xor     eax, eax
        call    scanf

;       compute and print p(a) and p(b)
        mov     rdi, [rsp+.p]
        mov     rsi, [rsp+.n]
        movsd   xmm0, [rsp+.a]
        call    horner
        segment .data
.a_print:
        db      "p(a) = %lf",0x0a,0
        segment .text
        lea     rdi, [.a_print]
        mov     rax, 1
        call    printf

        mov     rdi, [rsp+.p]
        mov     rsi, [rsp+.n]
        movsd   xmm0, [rsp+.b]
        call    horner
        segment .data
.b_print:
        db      "p(b) = %lf",0x0a,0
        segment .text
        lea     rdi, [.b_print]
        mov     rax, 1
        call    printf

;       compute p prime from p
        mov     rdi, [rsp+.p]
        mov     rsi, [rsp+.pp]
        mov     rdx, [rsp+.n]
        call    differentiate

;       compute and print pp(a) and pp(b)
        mov     rdi, [rsp+.pp]
        mov     rsi, [rsp+.n]
        dec     rsi
        movsd   xmm0, [rsp+.a]
        call    horner
        segment .data
.ppa_print:
        db      "pp(a) = %lf",0x0a,0
        segment .text
        lea     rdi, [.ppa_print]
        mov     rax, 1
        call    printf

        mov     rdi, [rsp+.pp]
        mov     rsi, [rsp+.n]
        dec     rsi
        movsd   xmm0, [rsp+.b]
        call    horner
        segment .data
.ppb_print:
        db      "pp(b) = %lf",0x0a,0
        segment .text
        lea     rdi, [.ppb_print]
        mov     rax, 1
        call    printf

;       compute P from p
        mov     rdi, [rsp+.p]
        mov     rsi, [rsp+.P]
        mov     rdx, [rsp+.n]
        call    integrate

        mov     rdi, [rsp+.P]

        mov     rsi, [rsp+.n]
        inc     rsi
        movsd   xmm0, [rsp+.a]
        call    horner
        movsd   [rsp+.Pa], xmm0
        segment .data
.Pa_print:
        db      "P(a) = %lf",0x0a,0
        segment .text
        lea     rdi, [.Pa_print]
        mov     rax, 1
        call    printf

        mov     rdi, [rsp+.P]
        mov     rsi, [rsp+.n]
        inc     rsi
        movsd   xmm0, [rsp+.b]
        call    horner
        movsd   [rsp+.Pb], xmm0
        segment .data
.Pb_print:
        db      "P(b) = %lf",0x0a,0
        segment .text
        lea     rdi, [.Pb_print]
        mov     rax, 1
        call    printf


;       compute and print P(b)-P(a)
        movsd   xmm0, [rsp+.Pb]
        subsd   xmm0, [rsp+.Pa]
        segment .data
.Pba_print:
        db      "P(b)-P(a) = %lf",0x0a,0
        segment .text
        lea     rdi, [.Pba_print]
        mov     rax, 1
        call    printf

        leave
        ret

create:
        push    rbp
        mov     rbp, rsp
        shl     rdi, 3
        call    malloc
        leave
        ret

horner:
        movsd   xmm1, xmm0              ; use xmm0 as b
                                ; and xmm1 as x
                                ; rsi is i
;       b = p[n];
        movsd   xmm0, [rdi+rsi*8]

;       for ( i = n-1; i >= 0; i-- ) {
        dec     rsi
.for:   cmp     rsi, 0
        jl      .done
;               b = b * x + p[i]
                mulsd   xmm0, xmm1      ; b = b * x
                addsd   xmm0, [rdi+rsi*8] ;   + p[i];
                dec     rsi
        jmp     .for
;       }
.done:
;       return b;
        ret

differentiate:
        segment .data
.one    dq      1.0
        segment .text
        movsd   xmm1, [.one]            ; xmm1 is i+1 as a double
        movsd   xmm2, xmm1
;       for ( i = 0; i < n; i++ ) {
        xor     ecx, ecx                ; ecx is i
.for    cmp     rcx, rdx
        jge     .done
;               pp[i] = (i+1)*p[i+1];
            movsd       xmm0, [rdi+rcx*8+8] ; xmm0 = p[i+1]
            mulsd       xmm0, xmm1              ;            * (i+1)
            movsd       [rsi+rcx*8], xmm0   ; p[i] = ...
            inc rcx
            addsd       xmm1, xmm2              ; increment the i+1
                jmp     .for

;       }
.done:
        ret

integrate:
        movsd   xmm1, [differentiate.one]               ; xmm1 is i+1 as a double
        movsd   xmm2, xmm1
        segment .data
.zero   dq      0.0
        segment .text

;       P[0] = 0.0;
        movsd   xmm0, [.zero]
        movsd   [rsi], xmm0

;       for ( i = 0; i <= n; i++ ) {
        xor     ecx, ecx                ; ecx is i
.for    cmp     rcx, rdx
        jg      .done
;               P[i+1] = p[i]/(i+1);
            movsd       xmm0, [rdi+rcx*8]
            divsd       xmm0, xmm1
            movsd       [rsi+rcx*8+8], xmm0
            inc rcx
            addsd       xmm1, xmm2
                jmp     .for
;       }
.done:
        ret
