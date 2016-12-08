	.file	"llms.c"
	.text
.Ltext0:
	.globl	main
	.type	main, @function
main:
.LFB2:
	.file 1 "llms.c"
	.loc 1 19 0
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$48, %rsp
	.loc 1 19 0
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	.loc 1 22 0
	movl	$12, -32(%rbp)
	.loc 1 23 0
	movq	$0, -24(%rbp)
	.loc 1 25 0
	call	build_list
	movq	%rax, -40(%rbp)
	.loc 1 26 0
	movq	-40(%rbp), %rax
	movq	%rax, %rdi
	call	merge_sort
	movq	%rax, -40(%rbp)
	.loc 1 27 0
	movq	-40(%rbp), %rax
	movq	%rax, %rdi
	call	print_list
	.loc 1 28 0
	leaq	-32(%rbp), %rax
	movq	%rax, %rdi
	call	print_list
	.loc 1 29 0
	movl	$0, %eax
	.loc 1 30 0
	movq	-8(%rbp), %rdx
	xorq	%fs:40, %rdx
	je	.L3
	call	__stack_chk_fail
.L3:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE2:
	.size	main, .-main
	.globl	split
	.type	split, @function
split:
.LFB3:
	.loc 1 33 0
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -40(%rbp)
	.loc 1 37 0
	movl	$0, -20(%rbp)
	.loc 1 38 0
	movq	-40(%rbp), %rax
	movq	%rax, -16(%rbp)
	.loc 1 39 0
	jmp	.L5
.L6:
	.loc 1 40 0
	addl	$1, -20(%rbp)
	.loc 1 41 0
	movq	-16(%rbp), %rax
	movq	8(%rax), %rax
	movq	%rax, -16(%rbp)
.L5:
	.loc 1 39 0
	cmpq	$0, -16(%rbp)
	jne	.L6
	.loc 1 44 0
	movq	-40(%rbp), %rax
	movq	%rax, -16(%rbp)
	.loc 1 45 0
	movl	$2, -24(%rbp)
	jmp	.L7
.L8:
	.loc 1 46 0 discriminator 3
	movq	-16(%rbp), %rax
	movq	8(%rax), %rax
	movq	%rax, -16(%rbp)
	.loc 1 45 0 discriminator 3
	addl	$2, -24(%rbp)
.L7:
	.loc 1 45 0 is_stmt 0 discriminator 1
	movl	-24(%rbp), %eax
	cmpl	-20(%rbp), %eax
	jl	.L8
	.loc 1 49 0 is_stmt 1
	movq	-16(%rbp), %rax
	movq	8(%rax), %rax
	movq	%rax, -8(%rbp)
	.loc 1 50 0
	movq	-16(%rbp), %rax
	movq	$0, 8(%rax)
	.loc 1 52 0
	movq	-8(%rbp), %rax
	.loc 1 53 0
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE3:
	.size	split, .-split
	.globl	merge
	.type	merge, @function
merge:
.LFB4:
	.loc 1 56 0
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -24(%rbp)
	movq	%rsi, -32(%rbp)
	.loc 1 59 0
	cmpq	$0, -24(%rbp)
	jne	.L11
	.loc 1 59 0 is_stmt 0 discriminator 1
	movq	-32(%rbp), %rax
	jmp	.L12
.L11:
	.loc 1 60 0 is_stmt 1
	cmpq	$0, -32(%rbp)
	jne	.L13
	.loc 1 60 0 is_stmt 0 discriminator 1
	movq	-24(%rbp), %rax
	jmp	.L12
.L13:
	.loc 1 62 0 is_stmt 1
	movq	-24(%rbp), %rax
	movl	(%rax), %edx
	movq	-32(%rbp), %rax
	movl	(%rax), %eax
	cmpl	%eax, %edx
	jge	.L14
	.loc 1 63 0
	movq	-24(%rbp), %rax
	movq	%rax, -8(%rbp)
	movq	-8(%rbp), %rax
	movq	%rax, -16(%rbp)
	.loc 1 64 0
	movq	-24(%rbp), %rax
	movq	8(%rax), %rax
	movq	%rax, -24(%rbp)
	jmp	.L15
.L14:
	.loc 1 66 0
	movq	-32(%rbp), %rax
	movq	%rax, -8(%rbp)
	movq	-8(%rbp), %rax
	movq	%rax, -16(%rbp)
	.loc 1 67 0
	movq	-32(%rbp), %rax
	movq	8(%rax), %rax
	movq	%rax, -32(%rbp)
.L15:
	.loc 1 70 0
	jmp	.L16
.L20:
	.loc 1 71 0
	movq	-24(%rbp), %rax
	movl	(%rax), %edx
	movq	-32(%rbp), %rax
	movl	(%rax), %eax
	cmpl	%eax, %edx
	jge	.L17
	.loc 1 72 0
	movq	-8(%rbp), %rax
	movq	-24(%rbp), %rdx
	movq	%rdx, 8(%rax)
	.loc 1 73 0
	movq	-24(%rbp), %rax
	movq	8(%rax), %rax
	movq	%rax, -24(%rbp)
	jmp	.L18
.L17:
	.loc 1 75 0
	movq	-8(%rbp), %rax
	movq	-32(%rbp), %rdx
	movq	%rdx, 8(%rax)
	.loc 1 76 0
	movq	-32(%rbp), %rax
	movq	8(%rax), %rax
	movq	%rax, -32(%rbp)
.L18:
	.loc 1 78 0
	movq	-8(%rbp), %rax
	movq	8(%rax), %rax
	movq	%rax, -8(%rbp)
.L16:
	.loc 1 70 0
	cmpq	$0, -24(%rbp)
	je	.L19
	.loc 1 70 0 is_stmt 0 discriminator 1
	cmpq	$0, -32(%rbp)
	jne	.L20
.L19:
	.loc 1 81 0 is_stmt 1
	cmpq	$0, -24(%rbp)
	je	.L21
	.loc 1 81 0 is_stmt 0 discriminator 1
	movq	-8(%rbp), %rax
	movq	-24(%rbp), %rdx
	movq	%rdx, 8(%rax)
	jmp	.L22
.L21:
	.loc 1 82 0 is_stmt 1
	movq	-8(%rbp), %rax
	movq	-32(%rbp), %rdx
	movq	%rdx, 8(%rax)
.L22:
	.loc 1 84 0
	movq	-16(%rbp), %rax
.L12:
	.loc 1 85 0
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE4:
	.size	merge, .-merge
	.globl	merge_sort
	.type	merge_sort, @function
merge_sort:
.LFB5:
	.loc 1 88 0
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movq	%rdi, -24(%rbp)
	.loc 1 91 0
	cmpq	$0, -24(%rbp)
	je	.L24
	.loc 1 91 0 is_stmt 0 discriminator 1
	movq	-24(%rbp), %rax
	movq	8(%rax), %rax
	testq	%rax, %rax
	je	.L24
	.loc 1 92 0 is_stmt 1
	movq	-24(%rbp), %rax
	movq	%rax, %rdi
	call	split
	movq	%rax, -8(%rbp)
	.loc 1 93 0
	movq	-24(%rbp), %rax
	movq	%rax, %rdi
	call	merge_sort
	movq	%rax, -24(%rbp)
	.loc 1 94 0
	movq	-8(%rbp), %rax
	movq	%rax, %rdi
	call	merge_sort
	movq	%rax, -8(%rbp)
	.loc 1 95 0
	movq	-8(%rbp), %rdx
	movq	-24(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	merge
	jmp	.L25
.L24:
	.loc 1 97 0
	movq	-24(%rbp), %rax
.L25:
	.loc 1 99 0
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE5:
	.size	merge_sort, .-merge_sort
	.section	.rodata
.LC0:
	.string	"%d"
	.text
	.globl	build_list
	.type	build_list, @function
build_list:
.LFB6:
	.loc 1 102 0
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	.loc 1 102 0
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	.loc 1 106 0
	movq	$0, -24(%rbp)
	.loc 1 108 0
	jmp	.L27
.L28:
	.loc 1 109 0
	movl	$16, %esi
	movl	$1, %edi
	call	calloc
	movq	%rax, -16(%rbp)
	.loc 1 110 0
	movl	-28(%rbp), %edx
	movq	-16(%rbp), %rax
	movl	%edx, (%rax)
	.loc 1 111 0
	movq	-16(%rbp), %rax
	movq	-24(%rbp), %rdx
	movq	%rdx, 8(%rax)
	.loc 1 112 0
	movq	-16(%rbp), %rax
	movq	%rax, -24(%rbp)
.L27:
	.loc 1 108 0
	leaq	-28(%rbp), %rax
	movq	%rax, %rsi
	movl	$.LC0, %edi
	movl	$0, %eax
	call	__isoc99_scanf
	cmpl	$1, %eax
	je	.L28
	.loc 1 114 0
	movq	-24(%rbp), %rax
	.loc 1 115 0
	movq	-8(%rbp), %rcx
	xorq	%fs:40, %rcx
	je	.L30
	call	__stack_chk_fail
.L30:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE6:
	.size	build_list, .-build_list
	.section	.rodata
.LC1:
	.string	"%d\n"
	.text
	.globl	print_list
	.type	print_list, @function
print_list:
.LFB7:
	.loc 1 118 0
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	.loc 1 119 0
	jmp	.L32
.L33:
	.loc 1 120 0
	movq	-8(%rbp), %rax
	movl	(%rax), %eax
	movl	%eax, %esi
	movl	$.LC1, %edi
	movl	$0, %eax
	call	printf
	.loc 1 121 0
	movq	-8(%rbp), %rax
	movq	8(%rax), %rax
	movq	%rax, -8(%rbp)
.L32:
	.loc 1 119 0
	cmpq	$0, -8(%rbp)
	jne	.L33
	.loc 1 123 0
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE7:
	.size	print_list, .-print_list
.Letext0:
	.section	.debug_info,"",@progbits
.Ldebug_info0:
	.long	0x25b
	.value	0x4
	.long	.Ldebug_abbrev0
	.byte	0x8
	.uleb128 0x1
	.long	.LASF20
	.byte	0x1
	.long	.LASF21
	.long	.LASF22
	.quad	.Ltext0
	.quad	.Letext0-.Ltext0
	.long	.Ldebug_line0
	.uleb128 0x2
	.byte	0x8
	.byte	0x7
	.long	.LASF0
	.uleb128 0x2
	.byte	0x1
	.byte	0x8
	.long	.LASF1
	.uleb128 0x2
	.byte	0x2
	.byte	0x7
	.long	.LASF2
	.uleb128 0x2
	.byte	0x4
	.byte	0x7
	.long	.LASF3
	.uleb128 0x2
	.byte	0x1
	.byte	0x6
	.long	.LASF4
	.uleb128 0x2
	.byte	0x2
	.byte	0x5
	.long	.LASF5
	.uleb128 0x3
	.byte	0x4
	.byte	0x5
	.string	"int"
	.uleb128 0x2
	.byte	0x8
	.byte	0x5
	.long	.LASF6
	.uleb128 0x2
	.byte	0x8
	.byte	0x7
	.long	.LASF7
	.uleb128 0x2
	.byte	0x1
	.byte	0x6
	.long	.LASF8
	.uleb128 0x2
	.byte	0x8
	.byte	0x5
	.long	.LASF9
	.uleb128 0x2
	.byte	0x8
	.byte	0x7
	.long	.LASF10
	.uleb128 0x4
	.long	.LASF23
	.byte	0x1
	.byte	0x5
	.long	0x8c
	.uleb128 0x5
	.long	.LASF23
	.byte	0x10
	.byte	0x1
	.byte	0x7
	.long	0xb1
	.uleb128 0x6
	.string	"key"
	.byte	0x1
	.byte	0x8
	.long	0x57
	.byte	0
	.uleb128 0x7
	.long	.LASF11
	.byte	0x1
	.byte	0x9
	.long	0xb1
	.byte	0x8
	.byte	0
	.uleb128 0x8
	.byte	0x8
	.long	0x81
	.uleb128 0x9
	.long	.LASF13
	.byte	0x1
	.byte	0x12
	.long	0x57
	.quad	.LFB2
	.quad	.LFE2-.LFB2
	.uleb128 0x1
	.byte	0x9c
	.long	0xf4
	.uleb128 0xa
	.long	.LASF12
	.byte	0x1
	.byte	0x14
	.long	0xb1
	.uleb128 0x2
	.byte	0x91
	.sleb128 -56
	.uleb128 0xb
	.string	"nd"
	.byte	0x1
	.byte	0x15
	.long	0x81
	.uleb128 0x2
	.byte	0x91
	.sleb128 -48
	.byte	0
	.uleb128 0xc
	.long	.LASF14
	.byte	0x1
	.byte	0x20
	.long	0xb1
	.quad	.LFB3
	.quad	.LFE3-.LFB3
	.uleb128 0x1
	.byte	0x9c
	.long	0x156
	.uleb128 0xd
	.long	.LASF12
	.byte	0x1
	.byte	0x20
	.long	0xb1
	.uleb128 0x2
	.byte	0x91
	.sleb128 -56
	.uleb128 0xb
	.string	"t"
	.byte	0x1
	.byte	0x22
	.long	0xb1
	.uleb128 0x2
	.byte	0x91
	.sleb128 -32
	.uleb128 0xa
	.long	.LASF15
	.byte	0x1
	.byte	0x22
	.long	0xb1
	.uleb128 0x2
	.byte	0x91
	.sleb128 -24
	.uleb128 0xb
	.string	"i"
	.byte	0x1
	.byte	0x23
	.long	0x57
	.uleb128 0x2
	.byte	0x91
	.sleb128 -40
	.uleb128 0xb
	.string	"n"
	.byte	0x1
	.byte	0x23
	.long	0x57
	.uleb128 0x2
	.byte	0x91
	.sleb128 -36
	.byte	0
	.uleb128 0xc
	.long	.LASF16
	.byte	0x1
	.byte	0x37
	.long	0xb1
	.quad	.LFB4
	.quad	.LFE4-.LFB4
	.uleb128 0x1
	.byte	0x9c
	.long	0x1b0
	.uleb128 0xd
	.long	.LASF12
	.byte	0x1
	.byte	0x37
	.long	0xb1
	.uleb128 0x2
	.byte	0x91
	.sleb128 -40
	.uleb128 0xd
	.long	.LASF15
	.byte	0x1
	.byte	0x37
	.long	0xb1
	.uleb128 0x2
	.byte	0x91
	.sleb128 -48
	.uleb128 0xa
	.long	.LASF17
	.byte	0x1
	.byte	0x39
	.long	0xb1
	.uleb128 0x2
	.byte	0x91
	.sleb128 -32
	.uleb128 0xb
	.string	"end"
	.byte	0x1
	.byte	0x39
	.long	0xb1
	.uleb128 0x2
	.byte	0x91
	.sleb128 -24
	.byte	0
	.uleb128 0x9
	.long	.LASF18
	.byte	0x1
	.byte	0x57
	.long	0xb1
	.quad	.LFB5
	.quad	.LFE5-.LFB5
	.uleb128 0x1
	.byte	0x9c
	.long	0x1ee
	.uleb128 0xd
	.long	.LASF12
	.byte	0x1
	.byte	0x57
	.long	0xb1
	.uleb128 0x2
	.byte	0x91
	.sleb128 -40
	.uleb128 0xa
	.long	.LASF15
	.byte	0x1
	.byte	0x59
	.long	0xb1
	.uleb128 0x2
	.byte	0x91
	.sleb128 -24
	.byte	0
	.uleb128 0x9
	.long	.LASF19
	.byte	0x1
	.byte	0x65
	.long	0xb1
	.quad	.LFB6
	.quad	.LFE6-.LFB6
	.uleb128 0x1
	.byte	0x9c
	.long	0x236
	.uleb128 0xa
	.long	.LASF12
	.byte	0x1
	.byte	0x67
	.long	0xb1
	.uleb128 0x2
	.byte	0x91
	.sleb128 -40
	.uleb128 0xb
	.string	"n"
	.byte	0x1
	.byte	0x67
	.long	0xb1
	.uleb128 0x2
	.byte	0x91
	.sleb128 -32
	.uleb128 0xb
	.string	"k"
	.byte	0x1
	.byte	0x68
	.long	0x57
	.uleb128 0x2
	.byte	0x91
	.sleb128 -44
	.byte	0
	.uleb128 0xe
	.long	.LASF24
	.byte	0x1
	.byte	0x75
	.quad	.LFB7
	.quad	.LFE7-.LFB7
	.uleb128 0x1
	.byte	0x9c
	.uleb128 0xd
	.long	.LASF12
	.byte	0x1
	.byte	0x75
	.long	0xb1
	.uleb128 0x2
	.byte	0x91
	.sleb128 -24
	.byte	0
	.byte	0
	.section	.debug_abbrev,"",@progbits
.Ldebug_abbrev0:
	.uleb128 0x1
	.uleb128 0x11
	.byte	0x1
	.uleb128 0x25
	.uleb128 0xe
	.uleb128 0x13
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x1b
	.uleb128 0xe
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x7
	.uleb128 0x10
	.uleb128 0x17
	.byte	0
	.byte	0
	.uleb128 0x2
	.uleb128 0x24
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0xe
	.byte	0
	.byte	0
	.uleb128 0x3
	.uleb128 0x24
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0x8
	.byte	0
	.byte	0
	.uleb128 0x4
	.uleb128 0x16
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x5
	.uleb128 0x13
	.byte	0x1
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x6
	.uleb128 0xd
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x38
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x7
	.uleb128 0xd
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x38
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x8
	.uleb128 0xf
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x9
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x7
	.uleb128 0x40
	.uleb128 0x18
	.uleb128 0x2116
	.uleb128 0x19
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xa
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0xb
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0xc
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x7
	.uleb128 0x40
	.uleb128 0x18
	.uleb128 0x2117
	.uleb128 0x19
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xd
	.uleb128 0x5
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0xe
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x7
	.uleb128 0x40
	.uleb128 0x18
	.uleb128 0x2116
	.uleb128 0x19
	.byte	0
	.byte	0
	.byte	0
	.section	.debug_aranges,"",@progbits
	.long	0x2c
	.value	0x2
	.long	.Ldebug_info0
	.byte	0x8
	.byte	0
	.value	0
	.value	0
	.quad	.Ltext0
	.quad	.Letext0-.Ltext0
	.quad	0
	.quad	0
	.section	.debug_line,"",@progbits
.Ldebug_line0:
	.section	.debug_str,"MS",@progbits,1
.LASF12:
	.string	"list"
.LASF11:
	.string	"next"
.LASF17:
	.string	"start"
.LASF20:
	.string	"GNU C 4.9.2 -mtune=generic -march=x86-64 -g -O0 -fstack-protector-strong"
.LASF16:
	.string	"merge"
.LASF1:
	.string	"unsigned char"
.LASF14:
	.string	"split"
.LASF0:
	.string	"long unsigned int"
.LASF2:
	.string	"short unsigned int"
.LASF22:
	.string	"/home/seyfarth/ebe"
.LASF13:
	.string	"main"
.LASF3:
	.string	"unsigned int"
.LASF19:
	.string	"build_list"
.LASF10:
	.string	"long long unsigned int"
.LASF21:
	.string	"llms.c"
.LASF24:
	.string	"print_list"
.LASF7:
	.string	"sizetype"
.LASF9:
	.string	"long long int"
.LASF8:
	.string	"char"
.LASF5:
	.string	"short int"
.LASF15:
	.string	"tail"
.LASF23:
	.string	"node"
.LASF6:
	.string	"long int"
.LASF4:
	.string	"signed char"
.LASF18:
	.string	"merge_sort"
	.ident	"GCC: (Ubuntu 4.9.2-10ubuntu8) 4.9.2"
	.section	.note.GNU-stack,"",@progbits
