        segment .data
coeff   dq      1.0, 2.0, 3.0, 4.0
x       dq      2.0

        segment .text
        global  main, horner
main:
        push    rbp
        mov     rbp, rsp
        lea     rdi, [coeff]
        movsd   xmm0, [x]
        mov     esi, 3
        call    horner
        xor     eax, eax
        leave
        ret

horner: movsd   xmm1, xmm0          ; use xmm1 as x
        movsd   xmm0, [rdi+rsi*8]   ; accumulator for b_k
        cmp     esi, 0              ; is the degree 0?
        jz      done
        more:   sub     esi, 1
        mulsd   xmm0, xmm1          ; b_k * x
        addsd   xmm0, [rdi+rsi*8]   ; add p_k
        jnz     more
done:   ret

