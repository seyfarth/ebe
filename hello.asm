       segment .data
first_name_prompt:
       db      "your first name: ", 0
last_name_prompt:
       db      "your last name: ", 0
height_prompt:
       db      "your height feet: ", 0
string_format:
       db      "%79s", 0
integer_format:
       db      "%ld", 0
weight dq      180
feet   dq      6
inches dq      0
age    dq      61
iq     dq      75

      
       
       segment .bss
first_name:
       resb    80
last_name:
       resb    80
height:
       resq    1
       segment .text
       global  main                    ; Tell linker about main
       extern  printf, scanf
main:
       push    rbp
       mov     rbp, rsp
       frame   2, 3, 7
       sub     rsp, frame_size
   ; Up to 6 integer register parameters
   ; in rdi, rsi, rdx, rcx, r8, r9
   ; Up to 8 float register parameters
   ; in xmm0-xmm7

       lea      rdi, [first_name_prompt]
       lea      rsi, [first_name]
       call     read_string
       lea      rdi, [last_name_prompt]
       lea      rsi, [last_name]
       call     read_string
       lea      rdi, [height_prompt]
       lea      rsi, [feet]
       call     read_integer
;   void print_info ( char *first,
;       char *last, long age, long weight,
;       long feet, long inches, long iq), 
       lea      rdi, [first_name]
       lea      rsi, [last_name]
       mov      rdx, [age]
       mov      rcx, [weight]
       mov      r8, [feet]
       mov      r9, [inches]
       mov      r11, [iq]
       mov      [rsp+newPar7], r11 
       call     print_info     
       leave
       ret
            
       
read_string:
        push    rbp
        mov     rbp, rsp
        frame   2, 1, 2
        sub     rsp, frame_size
        
        mov     [rbp+local1], rsi
        xor     eax, eax
        call    printf
        lea     rdi, [string_format]
        mov     rsi, [rbp+local1]
        xor     eax, eax
        call    scanf
        leave
        ret
read_integer:
        push    rbp
        mov     rbp, rsp
        frame   2, 1, 2
        sub     rsp, frame_size
        
        mov     [rbp+local1], rsi
        xor     eax, eax
        call    printf
        lea     rdi, [integer_format]
        mov     rsi, [rbp+local1]
        xor     eax, eax
        call    scanf
        leave
        ret
;
;   void print_info ( char *first,
;       char *last, long age, long weight,
;       long feet, long inches, long iq), 
print_info:
        push    rbp
        mov     rbp, rsp
        frame   7, 0, 8
        sub     rsp, frame_size
;   {
;       printf("%s %s %ld %ld %ld'%ld\" %ld\n");
        segment .data  
info_format:
        db  "%s %s %ld %ld %ld'%ld",34," %ld",0x0a,0
        segment .text
        mov     r11, [rbp+currPar7]
        mov     [rsp+newPar8], r11
        mov     [rsp+newPar7], r9
        mov     r9, r8
        mov     r8, rcx
        mov     rcx, rdx
        mov     rdx, rsi
        mov     rsi, rdi
        lea     rdi, [info_format]
        xor     eax, eax
        call    printf
;   }
        leave
        ret








