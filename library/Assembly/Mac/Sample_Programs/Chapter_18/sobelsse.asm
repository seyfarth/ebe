
;       xmm0:  input data

;       move xmm0 to xmm1
;       shift left
;       move xmm0 to xmm2
;       shift right
;
;       use punpcldw to convert low bytes to words
;       use pslldq to shift left 2 bytes
;       use psrldq to shift right 2 bytes
;       use pmullw, paddw
;       use cvtpi2ps to convert dwords to float
;       use punpcklwd to convert low words to dwords
;       use punpckhwd to convert high words to dwords

;rdi, rsi, rdx, rcx, r8, r9
;xmm0-7

;caller cleans up stack
;stack aligned on 16 bytes so that local data for functions can be properly
;aligned for SSE, ...

;callee must preserve rbx, rbp, r12-r15

%macro  multipush 1-*
    %rep  %0
        push    %1
        %rotate 1
    %endrep
%endmacro

%macro  multipop 1-*
    %rep %0
        %rotate -1
        pop     %1
    %endrep
%endmacro

;       sobel ( input, output, rows, cols );
;       char input[rows][cols]
;       float output[rows][cols]
;       boundary of the output array will be unfilled
;
        segment .text
        cname   sobel
        global  sobel, main
sobel:
.cols   equ     0
.rows   equ     8
.output equ     16
.input  equ     24
.bpir   equ     32
.bpor   equ     40
        multipush   rbx, rbp, r12, r13, r14, r15
        sub     rsp, 48
        cmp     rdx, 3
        jl      .noworktodo
        cmp     rcx, 3
        jl      .noworktodo
        mov     [rsp+.input], rdi
        mov     [rsp+.output], rsi
        mov     [rsp+.rows], rdx
        mov     [rsp+.cols], rcx
        mov     [rsp+.bpir], rcx
        imul     rcx, 4
        mov     [rsp+.bpor], rcx

        mov     rax, [rsp+.rows]; count of rows to process
        mov     rdx, [rsp+.cols]
        sub     rax, 2
        mov     r8, [rsp+.input]
        add     r8, rdx
        mov     r9, r8          ; address of row
        mov     r10, r8
        sub     r8, rdx         ; address of first byte of row-1
        add     r10, rdx        ; address of first byte of row+1
        add     rsi, [rsp+.bpor] ; address of first row of output
        pxor    xmm13, xmm13
        pxor    xmm14, xmm14
        pxor    xmm15, xmm15
.more_rows:
        mov     rbx, 1          ; first column to process
.more_cols:
        ;       8 low data              row-1
        ;       8 low data shifted left
        ;       8 low data shifted right
        ;       8 low data              row
        ;       8 low data shifted left
        ;       8 low data shifted right
        ;       8 low data              row+1
        ;       8 low data shifted left
        ;       8 low data shifted right
        
        movdqu  xmm0, [r8+rbx-1]        ; data for 1st row of 3
        movdqu  xmm1, xmm0
        movdqu  xmm2, xmm0
        pxor    xmm9, xmm9
        pxor    xmm10, xmm10
        pxor    xmm11, xmm11
        pxor    xmm12, xmm12
        psrldq  xmm1, 1
        psrldq  xmm2, 2
        movdqa  xmm3, xmm0
        movdqa  xmm4, xmm1
        movdqa  xmm5, xmm2
        punpcklbw   xmm3, xmm13
        punpcklbw   xmm4, xmm14
        punpcklbw   xmm5, xmm15         ; 8 values for 1st row
        psubw   xmm11, xmm3
        psubw   xmm9, xmm3
        paddw   xmm11, xmm5
        psubw   xmm9, xmm4
        psubw   xmm9, xmm4
        psubw    xmm9, xmm5      ; finished tally for 1st row, 1st 8
        punpckhbw  xmm0, xmm13
        punpckhbw  xmm1, xmm14
        punpckhbw  xmm2, xmm15
        psubw   xmm12, xmm0
        psubw   xmm10, xmm0
        paddw   xmm12, xmm2
        psubw   xmm10, xmm1
        psubw   xmm10, xmm1
        psubw   xmm10, xmm2     ; finished tally for 1st row, last 6

        movdqu  xmm0, [r9+rbx-1]        ; data for 1st row of 3
        ;movdqu  xmm1, xmm0
        movdqu  xmm2, xmm0
        ;psrldq  xmm1, 1
        psrldq  xmm2, 2
        movdqa  xmm3, xmm0
        ;movdqa  xmm4, xmm1
        movdqa  xmm5, xmm2
        punpcklbw   xmm3, xmm13
        ;punpcklbw   xmm4, xmm14
        punpcklbw   xmm5, xmm15         ; 8 values for 1st row
        psubw   xmm11, xmm3
        psubw   xmm11, xmm3
        paddw   xmm11, xmm5
        paddw   xmm11, xmm5
        punpckhbw  xmm0, xmm13
        ;punpckhbw  xmm1, xmm14
        punpckhbw  xmm2, xmm15
        psubw   xmm12, xmm0
        psubw   xmm12, xmm0
        paddw   xmm12, xmm2
        paddw   xmm12, xmm2     ; finished tally for 2nd row, last 6

        movdqu  xmm0, [r10+rbx-1]        ; data for 3rd row of 3
        movdqu  xmm1, xmm0
        movdqu  xmm2, xmm0
        psrldq  xmm1, 1
        psrldq  xmm2, 2
        movdqa  xmm3, xmm0
        movdqa  xmm4, xmm1
        movdqa  xmm5, xmm2
        punpcklbw   xmm3, xmm13
        punpcklbw   xmm4, xmm14
        punpcklbw   xmm5, xmm15         ; 8 values for 3rd row
        psubw   xmm11, xmm3
        paddw   xmm9, xmm3
        paddw   xmm11, xmm5
        paddw   xmm9, xmm4
        paddw   xmm9, xmm4
        paddw   xmm9, xmm5      ; finished tally for 3rd row, 1st 8
        punpckhbw  xmm0, xmm13
        punpckhbw  xmm1, xmm14
        punpckhbw  xmm2, xmm15
        psubw   xmm12, xmm0
        paddw   xmm10, xmm0
        paddw   xmm12, xmm2
        paddw   xmm10, xmm1
        paddw   xmm10, xmm1
        paddw   xmm10, xmm2     ; finished tally for 3rd row, last 6

        pmullw  xmm9, xmm9      ; square Gx and Gy values
        pmullw  xmm10, xmm10
        pmullw  xmm11, xmm11
        pmullw  xmm12, xmm12
        paddw   xmm9, xmm11     ; sum of squares
        paddw   xmm10, xmm12
        movdqa  xmm1, xmm9
        movdqa  xmm3, xmm10
        punpcklwd xmm9, xmm13   ; Convert low 4 words to double words
        punpckhwd xmm1, xmm13   ; Convert high 4 words to double words
        punpcklwd xmm10, xmm13  ; Convert low 4 words to double words
        punpckhwd xmm3, xmm13   ; Convert high 4 words to double words
        cvtdq2ps  xmm0, xmm9    ; Convert to floating point
        cvtdq2ps  xmm1, xmm1    ; Convert to floating point
        cvtdq2ps  xmm2, xmm10   ; Convert to floating point
        cvtdq2ps  xmm3, xmm3    ; Convert to floating point
        sqrtps    xmm0, xmm0    ; Take sqrt to get magnitude
        sqrtps    xmm1, xmm1    ; Take sqrt to get magnitude
        sqrtps    xmm2, xmm2    ; Take sqrt to get magnitude
        sqrtps    xmm3, xmm3    ; Take sqrt to get magnitude
        movups    [rsi+rbx*4], xmm0
        movups    [rsi+rbx*4+16], xmm1
        movups    [rsi+rbx*4+32], xmm2
        movlps    [rsi+rbx*4+48], xmm3

        add     rbx, 14         ; process 14 Sobel values
        cmp     rbx, rdx
        jl      .more_cols
        
        add     r8, rdx
        add     r9, rdx
        add     r10, rdx
        add     rsi, [rsp+.bpor]
        sub     rax, 1          ; 1 fewer row to process
        cmp     rax, 0
        jg      .more_rows
.noworktodo:
        add     rsp, 48
        multipop    rbx, rbp, r12, r13, r14, r15
        ret
