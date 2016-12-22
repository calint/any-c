	.file	"39.c"
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"entity %d\n"
	.section	.text.unlikely,"ax",@progbits
.LCOLDB1:
	.section	.text.startup,"ax",@progbits
.LHOTB1:
	.p2align 4,,15
	.globl	main
	.type	main, @function
main:
.LFB42:
	.cfi_startproc
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	movl	$1, %ebx
.L2:
	movl	%ebx, %edx
	xorl	%eax, %eax
	movl	$.LC0, %esi
	movl	$1, %edi
	addl	$1, %ebx
	call	__printf_chk
	cmpl	$4, %ebx
	jne	.L2
	xorl	%eax, %eax
	popq	%rbx
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE42:
	.size	main, .-main
	.section	.text.unlikely
.LCOLDE1:
	.section	.text.startup
.LHOTE1:
	.ident	"GCC: (Ubuntu 5.4.0-6ubuntu1~16.04.4) 5.4.0 20160609"
	.section	.note.GNU-stack,"",@progbits
