	.file	"hello.c"
	.comm	z,4,4
	.comm	data,80,32
	.section	.rodata
.LC1:
	.string	"Hello Worl=d!"
.LC2:
	.string	"x = %ld, %lf\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$64, %rsp
	movl	%edi, -36(%rbp)
	movq	%rsi, -48(%rbp)
	movl	$305419896, -20(%rbp)
	movl	-20(%rbp), %eax
	addl	$12, %eax
	pxor	%xmm0, %xmm0
	cvtsi2sd	%eax, %xmm0
	movsd	%xmm0, -16(%rbp)
	movabsq	$1311768467294899695, %rax
	movq	%rax, -8(%rbp)
	movss	.LC0(%rip), %xmm0
	movss	%xmm0, z(%rip)
	movss	z(%rip), %xmm0
	addss	%xmm0, %xmm0
	cvttss2si	%xmm0, %eax
	movl	%eax, -20(%rbp)
	movl	-20(%rbp), %eax
	movl	%eax, -36(%rbp)
	movl	$0, -24(%rbp)
	jmp	.L2
.L3:
	movl	-24(%rbp), %eax
	imull	-24(%rbp), %eax
	movslq	%eax, %rdx
	movl	-24(%rbp), %eax
	cltq
	movq	%rdx, data(,%rax,8)
	addl	$1, -24(%rbp)
.L2:
	cmpl	$9, -24(%rbp)
	jle	.L3
	movl	$.LC1, %edi
	call	puts
	movl	-20(%rbp), %eax
	movslq	%eax, %rdx
	movq	-8(%rbp), %rax
	addq	%rax, %rdx
	movss	z(%rip), %xmm0
	cvttss2si	%xmm0, %eax
	cltq
	addq	%rax, %rdx
	movq	-16(%rbp), %rax
	movq	%rax, -56(%rbp)
	movsd	-56(%rbp), %xmm0
	movq	%rdx, %rsi
	movl	$.LC2, %edi
	movl	$1, %eax
	call	printf
	movl	$0, %eax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.section	.rodata
	.align 4
.LC0:
	.long	1092616192
	.ident	"GCC: (Ubuntu 5.2.1-22ubuntu2) 5.2.1 20151010"
	.section	.note.GNU-stack,"",@progbits
