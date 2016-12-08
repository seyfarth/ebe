#ifndef REGS_H
#define REGS_H

#include "types.h"

extern int ebe_has_avx;

extern uLong ebe_rax;
extern uLong ebe_rbx;
extern uLong ebe_rcx;
extern uLong ebe_rdx;
extern uLong ebe_rsi;
extern uLong ebe_rdi;
extern uLong ebe_rsp;
extern uLong ebe_rbp;
extern uLong ebe_r8;
extern uLong ebe_r9;
extern uLong ebe_r10;
extern uLong ebe_r11;
extern uLong ebe_r12;
extern uLong ebe_r13;
extern uLong ebe_r14;
extern uLong ebe_r15;

extern uLong ebe_ymm0[4];
extern uLong ebe_ymm1[4];
extern uLong ebe_ymm2[4];
extern uLong ebe_ymm3[4];
extern uLong ebe_ymm4[4];
extern uLong ebe_ymm5[4];
extern uLong ebe_ymm6[4];
extern uLong ebe_ymm7[4];
extern uLong ebe_ymm8[4];
extern uLong ebe_ymm9[4];
extern uLong ebe_ymm10[4];
extern uLong ebe_ymm11[4];
extern uLong ebe_ymm12[4];
extern uLong ebe_ymm13[4];
extern uLong ebe_ymm14[4];
extern uLong ebe_ymm15[4];

int has_avx();
void get_regs();

#endif
