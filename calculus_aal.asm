%include "aal.inc"
        segment .text
        global  main
        extern  malloc, printf, scanf



main:
.n      equ     local1           ; degree of the polynomial
.a      equ     local2
.b      equ     local3
.p      equ     local4          ; polynomial
.pp     equ     local5          ; p' = derivative
.P      equ     local6          ; Integral
.i      equ     local7          ; loop counter
.Pa     equ     local8
.Pb     equ     local9
        push    rfp
        mov     rfp, rsp
        reserve 9, 3

;       prompt for and read n
        segment .data
.n_prompt:
        db      "Enter the degree of the polynomial: ",0

        segment .text
        lea     par1, [.n_prompt]
        xor     retd, retd
        call    printf

        segment .data
.n_scanf:
        db      "%ld",0

        segment .text
        lea     par1, [.n_scanf]
        lea     par2, [rfp+.n]
        xor     retd, retd
        call    scanf

;       allocate pp, p and P
        mov     par1d, [rfp+.n]
        call    create
        mov     [rfp+.pp], retq
        mov     par1d, [rfp+.n]
        inc     par1
        call    create
        mov     [rfp+.p], retq
        mov     par1d, [rfp+.n]
        add     par1, 2
        call    create
        mov     [rfp+.P], retq

;       prompt for and read p
        segment .data
.p_prompt:
        db      "Enter %ld numbers for p: ",0

        segment .text
        lea     par1, [.p_prompt]
        mov     par2d, [rfp+.n]
        inc     par2
        xor     retd, retd
        call    printf
;       for ( i = 0; i &lt;= n; i++ ) {
        xor     scr1, scr1                ; i = 0
        mov     [rfp+.i], scr1
.for    mov     scr2d, [rfp+.n]
        cmp     scr1, scr2                ; i &lt;= n ?
        jg      .end_for
;           scanf ( "%lf", p+i );

            segment .data
.p_scanf:   db      "%lf",0

            segment .text
            lea     par1, [.p_scanf]
            mov     par2, [rfp+.p]
            lea     par2, [par2+8*scr1]
            xor     retd, retd
            call    scanf
            mov     scr1, [rfp+.i]
            inc     scr1
            mov     [rfp+.i], scr1
            jmp     .for
;       }
.end_for:

;       prompt for and read a and b
        segment .data
.ab_prompt:
        db      "Enter a and b: ",0

        segment .text
        lea     par1, [.ab_prompt]
        xor     retd, retd
        call    printf

        segment .data
.ab_read:
        db      "%lf %lf",0

        segment .text
        lea     par1, [.ab_read]
        lea     par2, [rfp+.a]
        lea     par3, [rfp+.b]
        xor     retd, retd
        call    scanf

;       compute and print p(a) and p(b)
        mov     par1, [rfp+.p]
        mov     par2, [rfp+.n]
        movsd   xmm0, [rfp+.a]
        call    horner

        segment .data
.a_print:
        db      "p(a) = %lf",0x0a,0

        segment .text
        lea     par1, [.a_print]
%ifdef WINDOWS
        movsd   xmm1, xmm0
        movq    par2, xmm1
%else
        mov     retd, 1
%endif
        call    printf

        mov     par1, [rfp+.p]
        mov     par2d, [rfp+.n]
        movsd   xmm0, [rfp+.b]
        call    horner

        segment .data
.b_print:
        db      "p(b) = %lf",0x0a,0

        segment .text
        lea     par1, [.b_print]
%ifdef WINDOWS
        movsd   xmm1, xmm0
        movq    par2, xmm1
%else
        mov     retd, 1
%endif
        call    printf

;       compute p prime from p
        mov     par1, [rfp+.p]
        mov     par2, [rfp+.pp]
        mov     par3d, [rfp+.n]
        call    differentiate

;       compute and print pp(a) and pp(b)
        mov     par1, [rfp+.pp]
        mov     par2d, [rfp+.n]
        dec     par2
        movsd   xmm0, [rfp+.a]
        call    horner

        segment .data
.ppa_print:
        db      "pp(a) = %lf",0x0a,0

        segment .text
        lea     par1, [.ppa_print]
%ifdef WINDOWS
        movsd   xmm1, xmm0
        movq    par2, xmm1
%else
        mov     retd, 1
%endif
        call    printf

        mov     par1, [rfp+.pp]
        mov     par2d, [rfp+.n]
        dec     par2
        movsd   xmm0, [rfp+.b]
        call    horner

        segment .data
.ppb_print:
        db      "pp(b) = %lf",0x0a,0

        segment .text
        lea     par1, [.ppb_print]
%ifdef WINDOWS
        movsd   xmm1, xmm0
        movq    par2, xmm1
%else
        mov     retd, 1
%endif
        call    printf

;       compute P from p
        mov     par1, [rfp+.p]
        mov     par2, [rfp+.P]
        mov     par3d, [rfp+.n]
        call    integrate

        mov     par1, [rfp+.P]
        mov     par2d, [rfp+.n]
        inc     par2
        movsd   xmm0, [rfp+.a]
        call    horner
        movsd   [rfp+.Pa], xmm0

        segment .data
.Pa_print:
        db      "P(a) = %lf",0x0a,0

        segment .text
        lea     par1, [.Pa_print]
%ifdef WINDOWS
        movsd   xmm1, xmm0
        movq    par2, xmm1
%else
        mov     retd, 1
%endif
        call    printf

        mov     par1, [rfp+.P]
        mov     par2d, [rfp+.n]
        inc     par2
        movsd   xmm0, [rfp+.b]
        call    horner
        movsd   [rfp+.Pb], xmm0

        segment .data
.Pb_print:
        db      "P(b) = %lf",0x0a,0

        segment .text
        lea     par1, [.Pb_print]
%ifdef WINDOWS
        movsd   xmm1, xmm0
        movq    par2, xmm1
%else
        mov     retd, 1
%endif
        call    printf


;       compute and print P(b)-P(a)
        movsd   xmm0, [rfp+.Pb]
        subsd   xmm0, [rfp+.Pa]

        segment .data
.Pba_print:
        db      "P(b)-P(a) = %lf",0x0a,0

        segment .text
        lea     par1, [.Pba_print]
%ifdef WINDOWS
        movsd   xmm1, xmm0
        movq    par2, xmm1
%else
        mov     retd, 1
%endif
        call    printf

        leave
        ret

create:
        push    rfp
        mov     rfp, rsp
        reserve 0
        shl     par1, 3
        call    malloc
        leave
        ret

horner:
        movsd   xmm1, xmm0              ; use xmm0 as b
                                ; and xmm1 as x
                                ; rsi is i
;       b = p[n];
        movsd   xmm0, [par1+par2*8]

;       for ( i = n-1; i >= 0; i-- ) {
        dec     par2
.for:   cmp     par2, 0
        jl      .done
;               b = b * x + p[i]
                mulsd   xmm0, xmm1      ; b = b * x
                addsd   xmm0, [par1+par2*8] ;   + p[i];
                dec     par2
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
        xor     scr1d, scr1d                ; ecx is i
.for    cmp     scr1, par3
        jge     .done
;               pp[i] = (i+1)*p[i+1];
            movsd       xmm0, [par1+scr1*8+8] ; xmm0 = p[i+1]
            mulsd       xmm0, xmm1              ;            * (i+1)
            movsd       [par2+scr1*8], xmm0   ; p[i] = ...
            inc         scr1
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
        movsd   [par2], xmm0

;       for ( i = 0; i <= n; i++ ) {
        xor     scr1d, scr1d                ; ecx is i
.for    cmp     scr1, par3
        jg      .done
;               P[i+1] = p[i]/(i+1);
            movsd       xmm0, [par1+scr1*8]
            divsd       xmm0, xmm1
            movsd       [par2+scr1*8+8], xmm0
            inc scr1
            addsd       xmm1, xmm2
                jmp     .for
;       }
.done:
        ret
