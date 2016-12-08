        segment .text
        cname  corr
        global corr

; rdi, rsi, rdx, rcx, r8, r9
;
;       rdi:  x array
;       rsi:  y array
;       rcx:  loop counter
;       rdx:  n
;       xmm0: 2 parts of sum_x
;       xmm1: 2 parts of sum_y
;       xmm2: 2 parts of sum_xx
;       xmm3: 2 parts of sum_yy
;       xmm4: 2 parts of sum_xy
;       xmm5: 2 x values - later squared
;       xmm6: 2 y values - later squared
;       xmm7: 2 xy values
corr:
        xor     r8, r8
        mov     rcx, rdx
        subpd   xmm0, xmm0
        movapd   xmm1, xmm0
        movapd   xmm2, xmm0
        movapd   xmm3, xmm0
        movapd   xmm4, xmm0
        movapd   xmm8, xmm0
        movapd   xmm9, xmm0
        movapd   xmm10, xmm0
        movapd   xmm11, xmm0
        movapd   xmm12, xmm0
.more:   
        movapd  xmm5, [rdi+r8]  ; mov x
        movapd  xmm6, [rsi+r8]  ; mov y
        movapd  xmm7, xmm5      ; mov x
        mulpd   xmm7, xmm6      ; xy
        addpd   xmm0, xmm5      ; sum_x
        addpd   xmm1, xmm6      ; sum_y
        mulpd   xmm5, xmm5      ; xx
        mulpd   xmm6, xmm6      ; yy
        addpd   xmm2, xmm5      ; sum_xx
        addpd   xmm3, xmm6      ; sum_yy
        addpd   xmm4, xmm7      ; sum_xy
        movapd  xmm13, [rdi+r8+16]  ; mov x
        movapd  xmm14, [rsi+r8+16]  ; mov y
        movapd  xmm15, xmm13    ; mov x
        mulpd   xmm15, xmm14    ; xy
        addpd   xmm8, xmm13     ; sum_x
        addpd   xmm9, xmm14     ; sum_y
        mulpd   xmm13, xmm13    ; xx
        mulpd   xmm14, xmm14    ; yy
        addpd   xmm10, xmm13    ; sum_xx
        addpd   xmm11, xmm14    ; sum_yy
        addpd   xmm12, xmm15    ; sum_xy
        add     r8, 32
        sub     rcx, 4
        jnz     .more
        addpd   xmm0, xmm8
        addpd   xmm1, xmm9
        addpd   xmm2, xmm10
        addpd   xmm3, xmm11
        addpd   xmm4, xmm12
        haddpd  xmm0, xmm0      ; sum_x
        haddpd  xmm1, xmm1      ; sum_y
        haddpd  xmm2, xmm2      ; sum_xx
        haddpd  xmm3, xmm3      ; sum_yy
        haddpd  xmm4, xmm4      ; sum_xy
        movsd   xmm6, xmm0      ; sum_x
        movsd   xmm7, xmm1      ; sum_y
        cvtsi2sd xmm8, rdx      ; n
        mulsd   xmm6, xmm6      ; sum_x*sum_x
        mulsd   xmm7, xmm7      ; sum_y*sum_y
        mulsd   xmm2, xmm8      ; n*sum_xx
        mulsd   xmm3, xmm8      ; n*sum_yy
        subsd   xmm2, xmm6      ; n*sum_xx-sum_x*sum_x
        subsd   xmm3, xmm7      ; n*sum_yy-sum_y*sum_y
        mulsd   xmm2, xmm3      ; denom*denom
        sqrtsd  xmm2, xmm2      ; denom
        mulsd   xmm4, xmm8      ; n*sum_xy
        mulsd   xmm0, xmm1      ; sum_x*sum_y
        subsd   xmm4, xmm0      ; n*sum_xy-sum_x*sum_y
        divsd   xmm4, xmm2      ; correlation
        movsd   xmm0, xmm4      ; need in xmm0
        ret
