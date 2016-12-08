        segment .data
x       dq      1.5, 1.6, 1.7, 1.3, 1.4
        dq      2.1, 1.8, 1.6, 1.25, 1.3
y       dq      123456789
z       dq      123456777123456789
fmt     dq      "val %20.18g  ", 0x0d,0x0a, 0        
        segment .text
        global  main     ; Tell linker about main
        extern  printf
main:
        push    rbp
        mov     rbp, rsp ; setup stack frame
        frame   2, 2, 3
        sub     rsp, frame_size
        
        vzeroall
        cvtsi2ss xmm0, dword [y]
        cvtss2sd xmm0, xmm0
        lea      rdi, [fmt]
        mov      eax, 1 
        call     printf
        cvtsi2sd xmm0, qword [z]
        lea      rdi, [fmt]
        mov      eax, 1 
        call     printf
        vmovupd  ymm1, [x]
        vmovupd  ymm2, [x+32]
        vaddpd   ymm0, ymm1, ymm2
; Up to 6 integer parameters in rdi, rsi, rdx, rcx, r8, r9
; Up to 8 float parameters in xmm0-xmm7

        lea     rdi, [x]
        mov     esi, 0
        call    average
        movsd   [rbp+local1], xmm0
        lea     rdi, [x]
        mov     esi, 10
        movsd   xmm0, [rbp+local1]
        call    stddev
        leave
        ret
stddev:
        push    rbp
        mov     rbp, rsp
        frame   3, 0, 0
        sub     rsp, frame_size
        subsd   xmm1, xmm1
        xor     edx, edx
.top:   movsd   xmm2, [rdi+8*rdx]
        subsd   xmm2, xmm0
        mulsd   xmm2, xmm2
        addsd   xmm1, xmm2
        inc     edx
        cmp     edx, esi
        jl      .top
        dec     esi
        cvtsi2sd xmm2, rsi
        divsd   xmm1, xmm2
        vsqrtsd xmm0, xmm1, xmm1
        leave
        ret
        
average:
        push    rbp
        mov     rbp, rsp
        frame   2, 0, 0
        sub     rsp, frame_size
        subsd   xmm0, xmm0
        xor     edx, edx
.top:   addsd   xmm0, [rdi+8*rdx]
        inc     edx
        cmp     edx, esi
        jl      .top
        cvtsi2sd xmm1, rsi
        divsd   xmm0, xmm1
        leave
        ret
