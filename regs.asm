        segment .data
        cname   ebe_has_avx
        cname   ebe_rax, ebe_rbx, ebe_rcx, ebe_rdx
        cname   ebe_rdi, ebe_rsi, ebe_rsp, ebe_rbp
        cname   ebe_r8,  ebe_r9,  ebe_r10, ebe_r11
        cname   ebe_r12, ebe_r13, ebe_r14, ebe_r15
        cname   ebe_ymm0,  ebe_ymm1,  ebe_ymm2,  ebe_ymm3
        cname   ebe_ymm4,  ebe_ymm5,  ebe_ymm6,  ebe_ymm7
        cname   ebe_ymm8,  ebe_ymm9,  ebe_ymm10, ebe_ymm11
        cname   ebe_ymm12, ebe_ymm13, ebe_ymm14, ebe_ymm15

        align   8
ebe_rax dq      0
ebe_rbx dq      0
ebe_rcx dq      0
ebe_rdx dq      0
ebe_rsi dq      0
ebe_rdi dq      0
ebe_rsp dq      0
ebe_rbp dq      0
ebe_r8  dq      0
ebe_r9  dq      0
ebe_r10 dq      0
ebe_r11 dq      0
ebe_r12 dq      0
ebe_r13 dq      0
ebe_r14 dq      0
ebe_r15 dq      0

            align   16
ebe_ymm0    dq  0, 0, 0, 0
ebe_ymm1    dq  0, 0, 0, 0
ebe_ymm2    dq  0, 0, 0, 0
ebe_ymm3    dq  0, 0, 0, 0
ebe_ymm4    dq  0, 0, 0, 0
ebe_ymm5    dq  0, 0, 0, 0
ebe_ymm6    dq  0, 0, 0, 0
ebe_ymm7    dq  0, 0, 0, 0
ebe_ymm8    dq  0, 0, 0, 0
ebe_ymm9    dq  0, 0, 0, 0
ebe_ymm10   dq  0, 0, 0, 0
ebe_ymm11   dq  0, 0, 0, 0
ebe_ymm12   dq  0, 0, 0, 0
ebe_ymm13   dq  0, 0, 0, 0
ebe_ymm14   dq  0, 0, 0, 0
ebe_ymm15   dq  0, 0, 0, 0

ebe_has_avx:
        dd      0

        segment .text
        cname   test_avx, get_regs
        global  test_avx, get_regs
        extern  exit

test_avx:
        mov     eax, 7
        xor     ecx, ecx
        cpuid
        and     ebx, 0x20
        cmp     ebx, 0
        jz      no_avx
        mov     ebx, 1
        mov     [ebe_has_avx], ebx
no_avx:
        mov     eax, [ebe_has_avx]
        ret

get_regs:
        mov     [ebe_rax], rax
        mov     [ebe_rbx], rbx
        mov     [ebe_rcx], rcx
        mov     [ebe_rdx], rdx
        mov     [ebe_rsi], rsi
        mov     [ebe_rdi], rdi
        mov     [ebe_rsp], rsp
        mov     [ebe_rbp], rbp
        mov     [ebe_r8],  r8
        mov     [ebe_r9],  r9
        mov     [ebe_r10], r10
        mov     [ebe_r11], r11
        mov     [ebe_r12], r12
        mov     [ebe_r13], r13
        mov     [ebe_r14], r14
        mov     [ebe_r15], r15
        
        push    rax
        mov     eax, [ebe_has_avx]
        cmp     eax, 1
        jne     do_xmms

        vmovapd  [ebe_ymm0],  ymm0
        vmovapd  [ebe_ymm1],  ymm1
        vmovapd  [ebe_ymm2],  ymm2
        vmovapd  [ebe_ymm3],  ymm3
        vmovapd  [ebe_ymm4],  ymm4
        vmovapd  [ebe_ymm5],  ymm5
        vmovapd  [ebe_ymm6],  ymm6
        vmovapd  [ebe_ymm7],  ymm7
        vmovapd  [ebe_ymm8],  ymm8
        vmovapd  [ebe_ymm9],  ymm9
        vmovapd  [ebe_ymm10], ymm10
        vmovapd  [ebe_ymm11], ymm11
        vmovapd  [ebe_ymm12], ymm12
        vmovapd  [ebe_ymm13], ymm13
        vmovapd  [ebe_ymm14], ymm14
        vmovapd  [ebe_ymm15], ymm15

        jmp     merge

do_xmms:

        movapd  [ebe_ymm0],  xmm0
        movapd  [ebe_ymm1],  xmm1
        movapd  [ebe_ymm2],  xmm2
        movapd  [ebe_ymm3],  xmm3
        movapd  [ebe_ymm4],  xmm4
        movapd  [ebe_ymm5],  xmm5
        movapd  [ebe_ymm6],  xmm6
        movapd  [ebe_ymm7],  xmm7
        movapd  [ebe_ymm8],  xmm8
        movapd  [ebe_ymm9],  xmm9
        movapd  [ebe_ymm10], xmm10
        movapd  [ebe_ymm11], xmm11
        movapd  [ebe_ymm12], xmm12
        movapd  [ebe_ymm13], xmm13
        movapd  [ebe_ymm14], xmm14
        movapd  [ebe_ymm15], xmm15

merge:
        pop     rax
        ret
