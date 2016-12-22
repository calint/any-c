	.file	"8.c"
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"hello"
.LC1:
	.string	" loop %d\n"
	.section	.text.unlikely,"ax",@progbits
.LCOLDB2:
	.section	.text.startup,"ax",@progbits
.LHOTB2:
	.p2align 4,,15
	.globl	main
	.type	main, @function
main:
.LFB41:
	.cfi_startproc
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	movl	$.LC0, %esi
	movl	$1, %edi
	xorl	%eax, %eax
	movl	$3, %ebx
	call	__printf_chk
.L2:
	movl	%ebx, %edx
	xorl	%eax, %eax
	movl	$.LC1, %esi
	movl	$1, %edi
	subl	$1, %ebx
	call	__printf_chk
	cmpl	$-1, %ebx
	jne	.L2
	xorl	%eax, %eax
	popq	%rbx
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE41:
	.size	main, .-main
	.section	.text.unlikely
.LCOLDE2:
	.section	.text.startup
.LHOTE2:
	.ident	"GCC: (Ubuntu 5.4.0-6ubuntu1~16.04.4) 5.4.0 20160609"
	.section	.note.GNU-stack,"",@progbits
