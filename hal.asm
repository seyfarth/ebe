%ifidni __OUTPUT_FORMAT__, win64
%assign REG_PARS 4
%assign SCRATCH_REGS 2
%assign SAVE_REGS 7
%define WINDOWS
%elifidni __OUTPUT_FORMAT__, elf64
%assign REG_PARS 6
%assign SCRATCH_REGS 2
%assign SAVE_REGS 5
%define LINUX
%elifidni __OUTPUT_FORMAT__, macho64
%define OSX
%assign REG_PARS 6
%assign SCRATCH_REGS 2
%assign SAVE_REGS 5
%endif

%define raxq rax
%define raxd eax
%define raxw ax
%define raxb al
%define raxh ah

%define rbxq rbx
%define rbxd ebx
%define rbxw bx
%define rbxb bl
%define rbxh bh

%define rcxq rcx
%define rcxd ecx
%define rcxw cx
%define rcxb cl
%define rcxh ch

%define rdxq rdx
%define rdxd edx
%define rdxw dx
%define rdxb dl
%define rdxh dh

%define rdiq rdi
%define rdid edi
%define rdiw di
%define rdib dil

%define rsiq rsi
%define rsid six
%define rsiw si
%define rsib sil

%define rbpq rbp
%define rbpd ebp
%define rbpw bp

;                           ; Windows and Linux
%define rfp   rbp    
%define acc   rax
%define accq  rax
%define accd  eax
%define accw  ax
%define accb  al
%define acch  ah

;                           ; Windows and Linux
%define sav1  r12
%define sav1q r12
%define sav1d r12d
%define sav1w r12w
%define sav1b r12b

;                           ; Windows and Linux
%define sav2  r13
%define sav2q r13
%define sav2d r13d
%define sav2w r13w
%define sav2b r13b

;                           ; Windows and Linux
%define sav3  r14
%define sav3q r14
%define sav3d r14d
%define sav3w r14w
%define sav3b r14b

;                           ; Windows and Linux
%define sav4  r15
%define sav4q r15
%define sav4d r15d
%define sav4w r15w
%define sav4b r15b
;                           ; Windows and Linux
%define sav5  rbx
%define sav5q rbx
%define sav5d ebx
%define sav5w bx
%define sav5b bl
%define sav5h bh
;                           ; Windows and Linux
%define scr1  r10
%define scr1q r10
%define scr1d r10d
%define scr1w r10w
%define scr1g r10b
;                           ; Windows and Linux
%define scr2  r11
%define scr2q r11
%define scr2d r11d
%define scr2w r11w
%define scr2b r11b


%ifdef WINDOWS
;                           Windows
%define par1  rcx
%define par1q rcx
%define par1d ecx
%define par1w cx 
%define par1b cl 
%define par1h ch 
;                           Windows
%define par2  rdx
%define par2q rdx
%define par2d edx
%define par2w dx 
%define par2b dl 
%define par2h dh 
;                           Windows
%define par3  r8
%define par3q r8
%define par3d r8d
%define par3w r8w
%define par3b r8b
;                           Windows
%define par4  r9
%define par4q r9
%define par4d r9d
%define par4w r9w
%define par4b r9b
;                           Windows
%define sav6  rdi
%define sav6q rdi
%define sav6d edi
%define sav6w di
%define sav6b dil
;                           Windows
%define sav7  rsi
%define sav7q rsi
%define sav7d esi
%define sav7w si
%define sav7b sil

;   Windows
;   reserve locals [parameters]
;   parameters > 4 on stack
;
%macro reserve 1-2 0   ; %2 defaults to 0 
%if %2 <= 4
%assign xp 0
%else
%assign xp %2-4
%endif
%if %1 <= 4
%assign locals 0
%else
%assign locals %1-4
%endif
;   Subtract enough for locals and then parameters past 6
%assign n (4+locals+xp+1)/2*16
        sub rsp, n
%endmacro

;   Windows
;   First 4 local variables are in the stack frame

local1 equ 16
local2 equ 24
local3 equ 32
local4 equ 40

;   Local variables after 4 relative to rfp with negative offsets
;
;   local5 will be -8
;   local6 will be -16
;   ...
;   local100 be be -768
;   [rfp+localX]

%macro local 1
local%1 equ   -8*(%1-4)
%endmacro
%assign i 5
%rep 100
     local i
     %assign i i+1
%endrep
   
;   Windows
;   Where to place extra parameters when you call a function with
;   more than 4 parameters.
;
;   newpar5 will be 32
;   newpar6 will be 40
;   ...
;   newpar100 will be 99*8 = 792
;   [rsp+newparX]

%macro newpar 1
newpar%1  equ   8*(i-1)
%endmacro
%assign i 5
%rep 96
        newpar i
        %assign i i+1
%endrep

;   Windows
;   Where to find additional parameters to current function
;   past parameter 4
;
;   currpar5 will be 48
;   currpar6 will be 56
;   ...
;   currpar100 will be 808
;   [rfp+currparX]

%macro parequ 1
currpar%1  equ 16+8*(%1+1)
%endmacro

%assign i 5
%rep 96
        parequ i
        %assign i i+1
%endrep

%else                       ; Linux
%define par1  rdi
%define par1q rdi
%define par1d edi
%define par1w di 
%define par1b dil 
                            ; Linux
%define par2  rsi
%define par2q rsi
%define par2d esi
%define par2w si 
%define par2b sil
                            ; Linux
%define par3  rdx
%define par3q rdx
%define par3d edx
%define par3w dx
%define par3b dl
%define par3h dh
                            ; Linux
%define par4  rcx
%define par4q rcx
%define par4d ecx
%define par4w cx
%define par4b cl
%define par4h ch
                            ; Linux
%define par5  r8
%define par5q r8
%define par5d r8d
%define par5w r8w
%define par5b r8b
                            ; Linux
%define par6  r9
%define par6q r9
%define par6d r9d
%define par6w r9w
%define par6b r9b

;   Linux
;   reserve locals [parameters]
;   parameters > 6 on stack
;
%macro reserve 1-2 0   ; %2 defaults to 0 
%if %2 <= 6
%assign xp 0
%else
%assign xp %2-6
%endif
;   Subtract enough for locals and then parameters past 6
%assign n (%1+xp+1)/2*16
%if n > 0
        sub rsp, n
%endif
%endmacro

;   Linux
;   Local variables accessed relative to rfp with negative offsets
;
;   local1 will be -8
;   local2 will be -16
;   ...
;   local100 be be -800
;   [rfp+localX]

%macro local 1
local%1 equ   -8*%1
%endmacro
%assign i 1
%rep 100
     local i
     %assign i i+1
%endrep
   
;   Linux
;   Where to place extra parameters when you call a function with
;   more than 6 parameters.
;
;   newpar7 will be 0
;   newpar8 will be 8
;   ...
;   newpar100 will be 93*8 = 744
;   [rsp+newparX]

%macro newpar 1
newpar%1  equ   8*(i-7)
%endmacro
%assign i 7
%rep 94
        newpar i
        %assign i i+1
%endrep

;   Linux
;   Where to find additional parameters to current function
;   past parameter 6
;
;   currpar7 will be 16
;   currpar8 will be 24
;   ...
;   currpar100 will be 760
;   [rfp+currparX]

%macro parequ 1
currpar%1  equ 16+8*(%1-7)
%endmacro

%assign i 7
%rep 94
        parequ i
        %assign i i+1
%endrep

%endif

%macro alias 2
%define %1q %2
%define %1d %2d
%define %1w %2w
%define %1b %2b
%endmacro

%macro unalias 1-*
%rep %0
%undef %1q
%undef %1d
%undef %1q
%undef %1b
%rotate 1
%endmacro

%macro fpalias 2
%define %1y ymm%2
%define %1x xmm%2
%endmacro

%macro fpunalias 1-*
%rep %0
%undef %1y
%undef %1x
%rotate 1
%endmacro

        alias    def, par1  
        fpalias  sum, 0

        segment .data
a       dq      1.5
b       dq      10.1

        segment .text
        global  main
main:
        push    rbp
        mov     rbp, rsp

        mov     defd, 1
        movsd   sumx, [a]
        addsd   sumx, [b]
        leave
        ret
