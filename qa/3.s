	.file	"3.c"
	.text
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"hello \" world "
.LC2:
	.string	" %f %d %d %s %c \n"
	.section	.text.startup,"ax",@progbits
	.p2align 4,,15
	.globl	main
	.type	main, @function
main:
.LFB26:
	.cfi_startproc
	subq	$8, %rsp
	.cfi_def_cfa_offset 16
	movl	$46, %r8d
	movl	$1, %edx
	movsd	.LC1(%rip), %xmm0
	leaq	.LC0(%rip), %rcx
	movl	$17, %esi
	movl	$1, %eax
	leaq	.LC2(%rip), %rdi
	call	printf@PLT
	xorl	%eax, %eax
	addq	$8, %rsp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE26:
	.size	main, .-main
	.section	.rodata.cst8,"aM",@progbits,8
	.align 8
.LC1:
	.long	2684354560
	.long	1073846681
	.ident	"GCC: (GNU) 8.2.1 20180831"
	.section	.note.GNU-stack,"",@progbits
