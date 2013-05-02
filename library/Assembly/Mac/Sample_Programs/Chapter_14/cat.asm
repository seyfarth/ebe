        segment .data
infp    dq      0
argv    dq      0
read_mode:
        db      "r",0

        segment .bss
data    resb    160
data_size equ   $-data

        segment .text
        global  main
        extern  fopen, fgets, fputs, fclose, stdout

main:
        push    rbp
        mov     rbp, rsp

;       Step past argv[0]
        add     rsi, 8
        mov     [argv], rsi

;       Process each file
.more_files:
        mov     r8, [argv]
        mov     rdi, [r8]     ; get the next filename into rdi
        cmp     rdi, 0
        je      .done
        add     r8, 8         ; advance argv
        mov     [argv], r8
        lea     rsi, [read_mode]
        call    fopen
        cmp     rax, 0
        je      .more_files
        mov     [infp], rax    ; save the file pointer
.more_lines:
        lea     rdi, [data]    ; first parameter of fgets
        mov     rsi, data_size ; second parameter to fgets
        mov     rdx, [infp]    ; third parameter
        call    fgets
        cmp     rax, 0
        jne     .put
        mov     rdi, [infp]
        call    fclose         ; close the file
        jmp     .more_files
.put    lea     rdi, [data]    ; first parameter of fputs
        mov     rsi, [stdout]  ; second parameter
        call    fputs
        jmp     .more_lines
.done   xor     eax, eax
        leave
        ret
