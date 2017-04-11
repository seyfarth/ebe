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

%define raxd eax
%define raxw ax
%define raxb al
%define raxh ah

%define rbxd ebx
%define rbxw bx
%define rbxb bl
%define rbxh bh

%define rcxd ecx
%define rcxw cx
%define rcxb cl
%define rcxh ch

%define rdxd edx
%define rdxw dx
%define rdxb dl
%define rdxh dh

%define rdid edi
%define rdiw di
%define rdib dil

%define rsid esi
%define rsiw si
%define rsib sil

%define rbpd ebp
%define rbpw bp
%define rbpb bpl

;                           ; Windows and Linux
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
%define scr1b r10b
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

%endif
