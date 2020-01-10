    segment .text
    global  main
    extern  printf

; void print_max ( long a, long b )
; {
a   equ  local1
b   equ  local2
print_max:
    push rbp;         ; normal stack frame
    mov  rbp, rsp
    frame 2, 3, 4
    sub  rsp, frame_size
;   int max;
max equ  local3
    mov  [rbp+a], rcx ; save a
    mov  [rbp+b], rdx ; save b
;   max = a;
    mov  [rbp+max], rcx
;   if ( b > max ) max = b;
    cmp  rdx, rcx
    jng  skip
    mov  [rbp+max], rdx
skip:
;   printf ( "max(%ld,%ld) = %ld\n",
;            a, b, max );
    segment .data
fmt db   'max(%ld,%ld) = %ld',0xa,0
    segment .text
    lea  rcx, [fmt]
    mov  rdx, [rbp+a]
    mov  r8, [rbp+b]
    mov  r9, [rbp+max]
    xor  eax, eax
    call printf
; }
    leave
    ret

main:
    push rbp
    mov  rbp, rsp
    frame 2, 0, 2
;   print_max ( 100, 200 );
    mov  rcx, 100    ; first parameter
    mov  rdx, 200    ; second parameter
    call print_max
    xor  eax, eax    ; to return 0
    leave
    ret
