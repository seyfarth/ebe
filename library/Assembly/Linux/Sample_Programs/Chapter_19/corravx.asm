        segment .text
        cname  corr
        global corr

; rdi, rsi, rdx, rcx, r8, r9
;
;       rdi:  x array
;       rsi:  y array
;       rcx:  loop counter
;       rdx:  n
;       ymm0: 4 parts of sum_x
;       ymm1: 4 parts of sum_y
;       ymm2: 4 parts of sum_xx
;       ymm3: 4 parts of sum_yy
;       ymm4: 4 parts of sum_xy
;       ymm5: 4 x values - later squared
;       ymm6: 4 y values - later squared
;       ymm7: 4 xy values
corr:
        xor      r8, r8
        mov      rcx, rdx
        vzeroall
.more:   
        vmovupd  ymm5, [rdi+r8]  ; mov x
        vmovupd  ymm6, [rsi+r8]  ; mov y
        vmulpd   ymm7, ymm5, ymm6      ; xy
        vaddpd   ymm0, ymm0, ymm5      ; sum_x
        vaddpd   ymm1, ymm1, ymm6      ; sum_y
        vmulpd   ymm5, ymm5, ymm5      ; xx
        vmulpd   ymm6, ymm6, ymm6      ; yy
        vaddpd   ymm2, ymm2, ymm5      ; sum_xx
        vaddpd   ymm3, ymm3, ymm6      ; sum_yy
        vaddpd   ymm4, ymm4, ymm7      ; sum_xy
        vmovupd  ymm13, [rdi+r8+32]  ; mov x
        vmovupd  ymm14, [rsi+r8+32]  ; mov y
        vmulpd   ymm15, ymm13, ymm14    ; xy
        vaddpd   ymm8, ymm8, ymm13     ; sum_x
        vaddpd   ymm9, ymm9, ymm14     ; sum_y
        vmulpd   ymm13, ymm13, ymm13    ; xx
        vmulpd   ymm14, ymm14, ymm14    ; yy
        vaddpd   ymm10, ymm10, ymm13    ; sum_xx
        vaddpd   ymm11, ymm11, ymm14    ; sum_yy
        vaddpd   ymm12, ymm12, ymm15    ; sum_xy
        add     r8, 64
        sub     rcx, 8
        jnz     .more
        vaddpd   ymm0, ymm0, ymm8
        vaddpd   ymm1, ymm1, ymm9
        vaddpd   ymm2, ymm2, ymm10
        vaddpd   ymm3, ymm3, ymm11
        vaddpd   ymm4, ymm4, ymm12
        vhaddpd  ymm0, ymm0, ymm0      ; sum_x
        vhaddpd  ymm1, ymm1, ymm1      ; sum_y
        vhaddpd  ymm2, ymm2, ymm2      ; sum_xx
        vhaddpd  ymm3, ymm3, ymm3      ; sum_yy
        vhaddpd  ymm4, ymm4, ymm4      ; sum_xy
        vextractf128 xmm5, ymm0, 1
        vaddsd   xmm0, xmm0, xmm5
        vextractf128 xmm6, ymm1, 1
        vaddsd   xmm1, xmm1, xmm6
        vmulsd   xmm6, xmm0, xmm0      ; sum_x*sum_x
        vmulsd   xmm7, xmm1, xmm1      ; sum_y*sum_y
        vextractf128  xmm8, ymm2, 1
        vaddsd   xmm2, xmm2, xmm8
        vextractf128  xmm9, ymm3, 1
        vaddsd   xmm3, xmm3, xmm9
        cvtsi2sd xmm8, rdx      ; n
        vmulsd   xmm2, xmm2, xmm8      ; n*sum_xx
        vmulsd   xmm3, xmm3, xmm8      ; n*sum_yy
        vsubsd   xmm2, xmm2, xmm6      ; n*sum_xx-sum_x*sum_x
        vsubsd   xmm3, xmm3, xmm7      ; n*sum_yy-sum_y*sum_y
        vmulsd   xmm2, xmm2, xmm3      ; denom*denom
        vsqrtsd  xmm2, xmm2, xmm2      ; denom
        vextractf128  xmm6, ymm4, 1
        vaddsd   xmm4, xmm4, xmm6
        vmulsd   xmm4, xmm4, xmm8      ; n*sum_xy
        vmulsd   xmm0, xmm0, xmm1      ; sum_x*sum_y
        vsubsd   xmm4, xmm4, xmm0      ; n*sum_xy-sum_x*sum_y
        vdivsd   xmm0, xmm4, xmm2      ; correlation
        ret
