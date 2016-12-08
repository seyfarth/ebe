	.file	"tfun.c"
	.intel_syntax noprefix
	.text
	.globl	fun
	.type	fun, @function
fun:
.LFB0:
	.cfi_startproc
	add	edi, esi
	add	edx, edi
	add	ecx, edx
	add	r8d, ecx
	add	r9d, r8d
	movsx	eax, BYTE PTR [rsp+8]
	add	eax, r9d
	movsx	edx, BYTE PTR [rsp+16]
	add	eax, edx
	ret
	.cfi_endproc
.LFE0:
	.size	fun, .-fun
	.ident	"GCC: (Ubuntu 4.8.2-19ubuntu1) 4.8.2"
	.section	.note.GNU-stack,"",@progbits
