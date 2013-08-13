        struc   Cell
c_right resq    1       ; pointer for horizontal circular lists
c_down  resq    1       ; pointer for vertical circular lists
c_ziggy resq    1       ; pointer for circular lists in 3x3 box
c_value resd    1       ; 0 if unknown, 1-9 if known
c_set   resd    1       ; bits 0-9 represent possible values
        align   8       ; make sure the size is a multiple of 8
        endstruc

        segment .bss
cells:  resq    1       ; pointer to allocated array of Cells
ifp:    resq    1       ; input file pointer, file named on command line
file:   resq    1       ; file name pointer
known:  resd    1       ; how many Cells have known values
value:  resd    1       ; value for a cell: 0-9
bitcount: resd  512     ; precomputed bit count for each index
  

        segment .text

        global  main
        extern  fscanf, printf, malloc, fopen, fclose
main:
        push    rbp
        mov     rbp, rsp


        cmp     rdi, 2
        je      have_name

        segment .data
usage:  db      "Usage: sudoku filename", 0x0a
        db      0x0a
        db      "File contains 9 x 9 matrix with '-' for empty cells",0x0a
        db      "and digits 1-9 for known values at the start",0x0a,0

        segment .text

        lea     rdi, [usage]
        xor     eax, eax
        call    printf
        leave
        ret

have_name:

        segment .data
readonly: db    "r",0
        
        segment .text
        mov     rdi, [rsi+8]
        mov     [file], rdi
        mov     rsi, readonly
        call    fopen
        cmp     rax, 0
        jne     opened

        segment .data
not_open:  db      "Could not open %s", 0x0a

        segment .text

        lea     rdi, [not_open]
        mov     rsi, [file]
        xor     eax, eax
        call    printf
        leave
        ret

opened: mov     [ifp], rax

        call    count_bits
        
        call    create_cells

        call    read_data

        mov     rdi, [ifp]
        call    fclose
        
        call    print_sets

        leave
        ret

create_cells:
        push    rbp
        mov     rbp, rsp

;       cells = malloc ( 81*Cell_size );
        mov     rdi, 81*Cell_size
        call    malloc
        mov     [cells], rax

;       for ( i = 0; i < 81; i++ ) {
        xor     rdi, rdi            ; i = 0
.for:
        cmp     rdi, 81
        jge     .endfor
                
;           c = cells + i;
            mov     rcx, Cell_size
            imul    rcx, rdi
            add     rcx, rax
            
;           c->c_value = 0;
            mov     dword [rcx+c_value],0
;           c->c_set = 0x1ff;
            mov     dword [rcx+c_set], 0x1ff
        inc     rdi
        jmp     .for
;       }
.endfor

;
;       Build horizontal circular lists
;
;       for ( row = 0; row < 9; row++ ) {
        xor     eax, eax        ; rax is row
.for1:
          
;           c = cells + row * 9 * Cell_size;
            mov     rcx, rax    ; rcx is c
            imul    rcx, 9
            imul    rcx, Cell_size
            add     rcx, [cells]
            
;           for ( col = 0; col < 8; col++ ) {
            xor     ebx, ebx    ; rbx is col
.for2:
;               c->c_right = c + Cell_size;
                mov     rdx, rcx
                add     rdx, Cell_size
                mov     [rcx+c_right], rdx
                
;               c = c + Cell_size;
                mov     rcx, rdx
                
                inc     rbx
;           }
            cmp     rbx, 8
            jl      .for2
            
;           c->c_right = c - 8 * Cell_size;
            mov     rbx, 8
            imul    rbx, Cell_size
            mov     rdx, rcx
            sub     rdx, rbx
            mov     [rcx+c_right], rdx
            
            inc     rax
;       }
        cmp     rax, 9
        jl      .for1

;
;       Build vertical circular lists
;
;       for ( col = 0; col < 9; col++ ) {
        xor     ebx, ebx        ; again rbx is col
.for3:
;           c = cells + col * Cell_size;
            mov     rcx, rbx
            imul    rcx, Cell_size
            add     rcx, [cells]
            
;           for ( row = 0; row < 8; row++ ) {
            xor     eax, eax    ; again rax is row
.for4:
;               c->c_down = c + 9 * Cell_size;
                mov     rdx, 9
                imul    rdx, Cell_size
                add     rdx, rcx
                mov     [rcx+c_down], rdx
                
;               c = c->down
                mov     rcx, rdx
                inc     rax
;           }
            cmp     rax, 8
            jl      .for4
            
;           c->c_down = cells + col * Cell_size;
            mov     rdx, rbx
            imul    rdx, Cell_size
            add     rdx, [cells]
            mov     [rcx+c_down], rdx
            inc     rbx
;       }
        cmp     rbx, 9
        jl      .for3
           
;
;       Build lists for 3x3 boxes
;
;       for ( row = 0; row < 9; row += 3 ) {
        xor     eax, eax        ; rax is row
.for5
;           for ( col = 0; col < 9; col += 3 ) {
            xor     ebx, ebx    ; rbx is col
.for6
            
;               d = cells + row*9*Cell_size + col*Cell_size;
                mov     rdi, rax
                imul    rdi, 9
                imul    rdi, Cell_size
                mov     rdx, rbx
                imul    rdx, Cell_size
                add     rdx, rdi
                add     rdx, [cells]    ; rdx is d
                
;               c = d;
                mov     rcx, rdx        ; rcx is c
                
;               c->c_ziggy = c + Cell_size;
                mov     rdi, rcx
                add     rdi, Cell_size
                mov     [rcx+c_ziggy], rdi
                
;               c = c->c_ziggy;
                mov     rcx, rdi
                
;               c->c_ziggy = c + Cell_size;
                add     rdi, Cell_size
                mov     [rcx+c_ziggy], rdi

;               c = c->c_ziggy;
                mov     rcx, rdi
                
;               c->c_ziggy = c + 9*Cell_size;
                mov     rdi, 9
                imul    rdi, Cell_size
                add     rdi, rcx
                mov     [rcx+c_ziggy], rdi
                
;               c = c->c_ziggy;
                mov     rcx, rdi
                
;               c->c_ziggy = c - Cell_size;
                sub     rdi, Cell_size
                mov     [rcx+c_ziggy], rdi
                
;               c = c->c_ziggy;
                mov     rcx, rdi

;               c->c_ziggy = c - Cell_size;
                sub     rdi, Cell_size
                mov     [rcx+c_ziggy], rdi
                
;               c = c->c_ziggy;
                mov     rcx, rdi
                
;               c->c_ziggy = c + 9*Cell_size;
                mov     rdi, 9
                imul    rdi, Cell_size
                add     rdi, rcx
                mov     [rcx+c_ziggy], rdi

;               c = c->c_ziggy;
                mov     rcx, rdi

;               c->c_ziggy = c + Cell_size;
                add     rdi, Cell_size
                mov     [rcx+c_ziggy], rdi
                
;               c = c->c_ziggy;
                mov     rcx, rdi

;               c->c_ziggy = c + Cell_size;
                add     rdi, Cell_size
                mov     [rcx+c_ziggy], rdi

;               c = c->c_ziggy;
                mov     rcx, rdi

;               c->ziggy = d;
                mov     [rcx+c_ziggy], rdx
                add     rbx, 3
    
;           }
            cmp     rbx, 9
            jl      .for6
            
            add     rax, 3
;       }
        cmp     rax, 9
        jl      .for5
                
        leave
        ret
;       rdi,  rsi, rdx, rcx, r8, r9
;       xmm0-7


read_data:
        push    rbp
        mov     rbp, rsp

;       known = 0
        xor     r8, r8          ; r8 will hold known
           
;       for ( i = 0; i < 81; i++ ) {
        xor     r9, r9        ; i = 0
.for:   cmp     r9, 81
        jge     .endfor
        
;           fscanf(ifp,"%d",&value) ) {
            mov     rdi, [ifp]
            segment .data
.format:    db     "%d",0
            segment .text
            lea     rsi, [.format]
            lea     rdx, [value]
            xor     rax, rax
            push    r8
            push    r9
            call    fscanf
            pop     r9
            pop     r8
            
;           if ( value > 0 ) {
            mov     edx, [value]
            cmp     edx, 0
            jz      .value_0
;               c = cells[i];
                mov     rcx, r9
                imul    rcx, Cell_size
                add     rcx, [cells]
                
;               known++;
                inc     r8
               
;               c->c_value = value;
                mov dword [rcx+c_value], edx
                
;               c=>c_set = 0;
                mov word [rcx+c_set], 0
;               clear_list ( c, value, c_right );
                push    r8
                push    r9
                push    rdx
                push    rcx
                mov     rdi, rcx
                mov     rsi, rdx
                mov     rdx, c_right
                call    clear_list
                
;               clear_list ( c, value, c_down );
                mov     rdx, c_down
                call    clear_list

;               clear_list ( c, value, c_ziggy );
                mov     rdx, c_ziggy
                call    clear_list
                pop     rcx
                pop     rdx
                pop     r9
                pop     r8
.value_0:
                
;           }
            inc     r9
            jmp     .for
;       }
.endfor

        mov     [known], r8
        leave
        ret

clear_list:
        push    rbp
        mov     rbp, rsp
        
;       mask = bitclear ( -1, (value-1));
        mov     rcx, rsi
        dec     rcx
        mov     r8, -1          ; r8 is mask
        btc     r8, rcx

;       c = parameter1;
        mov     rcx, rdi
        
;       for ( i = 0; i < 9; i++ ) {
        xor     eax, eax
.for:

;           c->set &= mask;
            and     [rcx+c_set], r8
            
;           c = c->which
            mov     rcx, [rcx+rdx]
            
            inc     eax
;       }
        cmp     rax, 9
        jl      .for
        
        leave
        ret

print_sets:
        push    rbp
        mov     rbp, rsp

;       c = cells;
        mov     rcx, [cells]
;       for ( i = 0; i < 9; i++ ) {
        xor     ebx, ebx
.for1:   
;           for ( j = 0; j < 9; j++ ) {
            xor     r8d, r8d
.for2:
;               printf("%d %03x  ",c->c_value, c->c_set);
                segment .data
.format         db      "%d %03x  ",0
                segment .text
                lea     rdi, [.format]
                mov     esi, [rcx+c_value]
                mov     edx, [rcx+c_set]
                xor     eax, eax
                push    rbx 
                push    rcx
                push    r8
                push    r8
                call    printf
                pop     r8
                pop     r8
                pop     rcx
                pop     rbx
            
;               c += Cell_size;
                add     rcx, Cell_size
                
                inc     r8
;           }
            cmp     r8, 9
            jl      .for2

;           printf("\n");
            segment .data
.newline:   db      0x0a,0 
            segment .text
            lea     rdi, [.newline]
            xor     eax, eax
            push    rbx 
            push    rcx
            call    printf
            pop     rcx
            pop     rbx
            
            inc     rbx
;       }
        cmp     rbx, 9
        jl      .for1
         
        leave
        ret   
        
;       void count_bits()
;       {
count_bits:
        push    rbp
        mov     rbp, rsp
            
            lea     rdi, [bitcount]
            xor     ebx, ebx
;           for ( int i = 0; i < 512; i++ ) {
            xor     eax, eax
.for1:
                
;               sum = 0;
                xor     edx, edx
                
;               for ( bit = 0; bit < 9; bit++ ) {
                xor     rcx, rcx
.for2:              
;                   sum += btest(i,bit);
                    bt      rax, rcx
                    setc    bl
                    add     rdx, rbx
                    
                    inc     rcx
;               }
                cmp     rcx, 9
                jl      .for2
                
                mov     dword [rdi+4*rax], edx
            inc     rax
;           }
            cmp     rax, 512
            jl  .for1
            
;          
        leave
        ret
