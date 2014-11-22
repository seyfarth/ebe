        segment .text
        global  main
        extern  printf

; void print_max ( long a, long b )
; {
a       equ     local1
b       equ     local2
print_max:
        push    rbp;         ; normal stack frame
        mov     rbp, rsp
        frame   2, 3, 4
        sub  rsp, frame_size
;       leave space for a, b and max
;       int max;
max     equ     local3
        mov     [rbp+a], rdi ; save a
        mov     [rbp+b], rsi ; save b
;       max = a;
        mov     [rbp+max], rdi
;       if ( b > max ) max = b;
        cmp     rsi, rdi
        jng     skip
        mov     [rbp+max], rsi
skip:
;       printf ( "max(%ld,%ld) = %ld\n",
;              a, b, max );
        segment .data
fmt     db      'max(%ld,%ld) = %ld',0xa,0
        segment .text
        lea     rdi, [fmt]
        mov     rsi, [rbp+a]
        mov     rdx, [rbp+b]
        mov     rcx, [rbp+max]
        xor     eax, eax
        call    printf
; }
        leave
        ret

main:
        push    rbp
        mov     rbp, rsp
;       print_max ( 100, 200 );
        mov     rdi, 100    ; first parameter
        mov     rsi, 200    ; second parameter
        call print_max
        xor     eax, eax    ; to return 0
        leave
        ret
