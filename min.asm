        segment .text
        extern  printf, rand, malloc, atoi
        global  main, create, fill, min

;       array = create ( size );
create:
        push    rbp
        mov     rbp, rsp
        frame   1, 0, 1
        sub     rsp, frame_size
        sal     rcx, 2            ; multiply size by 4
        call    malloc
        leave
        ret

;       fill ( array, size );
fill:
 .array equ     local1  ; using .array instead of local1
 .size  equ     local2  ; using .size instead of local2
 .i     equ     local3  ; using .i instead of local3
        push    rbp
        mov     rbp, rsp
        frame   2, 3, 0
        sub     rsp, frame_size 
        mov     [rbp+.array], rcx  ; save array on stack
        mov     [rbp+.size], rdx   ; save size on stack
        xor     ecx, ecx           ; zero index register
 .more  mov     [rbp+.i], rcx      ; save index register
        call    rand
        mov     rcx, [rbp+.i]      ; load index register
        mov     rdx, [rbp+.array]  ; load array address
        mov     [rdx+rcx*4], eax   ; store random value
        inc     rcx                ; increment rcx
        cmp     rcx, [rbp+.size]   ; compare rcx & size
        jl      .more              ; more if rcx is less
        leave
        ret

;       print ( array, size );
print:
 .array equ     local1
 .size  equ     local2
 .i     equ     local3
        push    rbp
        mov     rbp, rsp
        frame   2, 3, 2
        sub     rsp, frame_size
        mov     [rbp+.array], rcx ; save array
        mov     [rbp+.size], rdx  ; save size
        xor     r8d, r8d          ; zero index register
        mov     [rbp+.i], r8      ; 
        segment .data
 .format:
        db      "%10d",0x0a,0
        segment .text
 .more  lea     rcx, [.format]    ; first parameter
        mov     rdx, [rbp+.array] ; get array address
        mov     r8, [rbp+.i]      ; get index register
        mov     edx, [rdx+r8*4]   ; get array[i]
        call    printf
        mov     rcx, [rbp+.i]     ; get index register
        inc     rcx               ; increment index
        mov     [rbp+.i], rcx     ; save index register
        cmp     rcx, [rbp+.size]  ; compare index & size
        jl      .more             ; more if index is <
        leave
        ret

;       x = min ( array, size );
min:
        push    rbp
        mov     rbp, rsp
        frame   2, 0, 0
        sub     rsp, frame_size
        mov     eax, [rcx]      ; get array[0]
        mov     r8d, 1           ; set index register = 1
 .more  mov     r9d, [rcx+r8*4] ; get array[r8]
        cmp     r9d, eax        ; is array[r8] < eax
        cmovl   eax, r9d        ; if so, move to eax
        inc     r8              ; increment index
        cmp     r8, rdx         ; compare r8 vs size
        jl      .more           ; more if r8 < size
        leave
        ret

main:
.array equ     local1
.size  equ     local2
       push    rbp
       mov     rbp, rsp
       frame   2, 2, 2
       sub     rsp, frame_size
       mov     r8d, 10          ; set default size
       mov     [rbp+.size], r8

;      check for argv[1] providing a size
       cmp     ecx, 2
       jl      .nosize
       mov     rcx, [rdx+8]
       call    atoi
       mov     [rbp+.size], rax

.nosize: 
;      create the array
       mov     rcx, [rbp+.size]
       call    create

       mov     [rbp+.array], rax
;      fill the array with random numbers
       mov     rcx, rax
       mov     rdx, [rbp+.size]
       call    fill
       mov     rcx, [rbp+.array]
       mov     rdx, [rbp+.size]
       call    fill
        

;      if size <= 20 print the array
       mov     rdx, [rbp+.size]
       cmp     rcx, 20
       jg      .toobig
       mov     rcx, [rbp+.array]
       call    print

.toobig:
;      print the minimum
       segment .data 
.format:
       db      "min: %ld",0xa,0
       segment .text
       mov     rcx, [rbp+.array]
       mov     rdx, [rbp+.size]
       call    min
       lea     rcx, [.format]
       mov     rdx, rax
       call    printf 

       leave
       ret
