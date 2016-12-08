        ;default rel
        segment .data
a       times 10 dw 0
        segment .bss
b       resw    10

        segment .text
        global  main     ; Tell linker about main
        extern  random, malloc  
main:
array   equ     local1
        push    rbp
        mov     rbp, rsp ; setup stack frame
        frame   2, 1, 2
        sub     rsp, frame_size
        
; Up to 6 integer parameters in rdi, rsi, rdx, rcx, r8, r9
; Up to 8 float parameters in xmm0-xmm7


        lea     rdi, [a]
        mov     esi, 10
        call    fill
        lea     rdi, [a]
        mov     esi, 10
        call    sort
        lea     rdi, [b]
        mov     esi, 10
        call    fill
        lea     rdi, [b]
        mov     esi, 10
        call    sort 
        
        mov     edi, 2000
        ;call    [malloc wrt ..got]
        call    malloc
        mov     [rbp+array], rax
        
        mov     rdi, [rbp+array]
        mov     esi, 1000
        call    fill
        mov     rdi, [rbp+array]
        mov     esi, 1000
        call    sort
        
        leave
        ret

fill:
;array    equ     local1
n       equ     local2
i       equ     local3
        push    rbp
        mov     rbp, rsp        
        frame   2, 3, 1
        sub     rsp, frame_size
        
        mov     [rbp+array], rdi
        mov     [rbp+n], rsi
        xor     ecx, ecx
.more:        
        mov     [rbp+i], rcx
        call    [random wrt ..got]
        and     rax, 0x3ff
        mov     rcx, [rbp+i]
        mov     rdi, [rbp+array]
        mov     [rdi+2*rcx], ax
        inc     ecx
        cmp     rcx, [rbp+n]
        jne     .more
        
        leave
        ret
        
; void sort ( short *array, int n )
sort:
; {
        push    rbp 
        mov     rbp, rsp
        frame   2 
        sub     rsp, frame_size
        
;       int i;          rcx
;       bool sorted;    rdx
;
;       do {
top:
;           sorted = true;
            mov     edx, 1 
;           for ( i = 0; i < n-1; i++ ) {
            xor     ecx, ecx
            dec     esi
for_top:
                cmp     ecx, esi
                jnl     for_end
;               if ( array[i] > array[i+1] ( {
                    mov     r8w, [rdi+2*rcx]
                    mov     r9w, [rdi+2*rcx+2]
                    cmp     r8w, r9w
                    jng     no_swap
                       mov  [rdi+2*rcx], r9w
                       mov  [rdi+2*rcx+2], r8w
                       xor  edx, edx
no_swap:                            
;                   swap array[i] & array[i+1]
;                   sorted = false;
;               }
                inc     ecx
                jmp     for_top
for_end:
;           }

;       } while ( ! sorted );
        cmp     edx, 1 
        jne     top
        leave
        ret
