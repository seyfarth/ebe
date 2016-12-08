%ifdef WINDOWS
;
;   frame  [parameters] [locals] [new_parameters]
;   parameters > 4 on stack
;
%macro frame 0-2 0,0,0   ; %1, %2 & %3 default to 0 
%if %3 <= 4
%assign xp 0
%else
%assign xp %3-4
%endif
%if %2 <= 4
%assign locals 0
%else
%assign locals %2-4
%endif
;   Subtract enough for locals and then parameters past 6
%assign frame_size (4+locals+xp+1)/2*16
%endmacro
%undef xp
%undef locals

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
%undef i   
;   Windows
;   Where to place extra parameters when you call a function with
;   more than 4 parameters.
;
;   newPar5 will be 32
;   newPar6 will be 40
;   ...
;   newPar100 will be 99*8 = 792
;   [rsp+newParX]

%macro newpar 1
newPar%1  equ   8*(i-1)
%endmacro
%assign i 5
%rep 96
        newpar i
        %assign i i+1
%endrep
%undef i   

;   Windows
;   Where to find additional parameters to current function
;   past parameter 4
;
;   currPar5 will be 48
;   currPar6 will be 56
;   ...
;   currPar100 will be 808
;   [rfp+currParX]

%macro parequ 1
currPar%1  equ 16+8*(%1+1)
%endmacro

%assign i 5
%rep 96
        parequ i
        %assign i i+1
%endrep
%undef i   

%else                       ; Linux/OS X
;
;   frame [parameters] [locals] [new_parameters]
;   parameters > 6 on stack
;
%macro frame 0-3 0   ; %2 defaults to 0 
%if %3 <= 6
%assign xp 0
%else
%assign xp %3-6
%endif
;   Subtract enough for locals and then parameters past 6
%assign frame_size (%2+xp+1)/2*16
%endmacro
%undef xp

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
%undef i
   
;   Linux
;   Where to place extra parameters when you call a function with
;   more than 6 parameters.
;
;   newPar7 will be 0
;   newPar8 will be 8
;   ...
;   newPar100 will be 93*8 = 744
;   [rsp+newParX]

%macro newpar 1
newPar%1  equ   8*(i-7)
%endmacro
%assign i 7
%rep 94
        newpar i
        %assign i i+1
%endrep
%undef i

;   Linux
;   Where to find additional parameters to current function
;   past parameter 6
;
;   currPar7 will be 16
;   currPar8 will be 24
;   ...
;   currPar100 will be 760
;   [rfp+currParX]

%macro parequ 1
currPar%1  equ 16+8*(%1-7)
%endmacro

%assign i 7
%rep 94
        parequ i
        %assign i i+1
%endrep
%undef i

%endif      ; from original test on WINDOWS

%macro  multipush 2-*
%rep %0
        push %1
%rotate 1
%endrep
%endmacro

%macro  multipop 2-*
%rep %0
        pop %1
%rotate 1
%endrep
%endmacro

%macro alias 2
%define q%1 %2
%define d%1 %2d
%define w%1 %2w
%define b%1 %2b
%define h%1 %2h
%endmacro

%macro unalias 1-*
%rep %0
%undef q%1
%undef d%1
%undef w%1
%undef b%1
%undef h%1
%rotate 1
%endrep
%endmacro

%macro fpalias 2
%define y%1 ymm%2
%define x%1 xmm%2
%endmacro

%macro fpunalias 1-*
%rep %0
%undef y%1
%undef x%1
%rotate 1
%endrep
%endmacro
