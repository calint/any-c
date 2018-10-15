	.file	"25.c"
	.text
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC1:
	.string	"entity: %d %f\n"
.LC2:
	.string	"free entity %d\n"
.LC3:
	.string	"free id %d\n"
	.section	.text.startup,"ax",@progbits
	.p2align 4,,15
	.globl	main
	.type	main, @function
main:
.LFB30:
	.cfi_startproc
	subq	$8, %rsp
	.cfi_def_cfa_offset 16
	movl	$1, %esi
	movl	$1, %eax
	movsd	.LC0(%rip), %xmm0
	leaq	.LC1(%rip), %rdi
	call	printf@PLT
	movl	$2, %esi
	movl	$1, %eax
	movsd	.LC0(%rip), %xmm0
	leaq	.LC1(%rip), %rdi
	call	printf@PLT
	movl	$3, %esi
	movl	$1, %eax
	movsd	.LC0(%rip), %xmm0
	leaq	.LC1(%rip), %rdi
	call	printf@PLT
	movl	$2, %esi
	leaq	.LC2(%rip), %rdi
	xorl	%eax, %eax
	call	printf@PLT
	movl	$2, %esi
	leaq	.LC3(%rip), %rdi
	xorl	%eax, %eax
	call	printf@PLT
	movl	$3, %esi
	leaq	.LC2(%rip), %rdi
	xorl	%eax, %eax
	call	printf@PLT
	movl	$3, %esi
	leaq	.LC3(%rip), %rdi
	xorl	%eax, %eax
	call	printf@PLT
	xorl	%eax, %eax
	addq	$8, %rsp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE30:
	.size	main, .-main
	.section	.rodata.cst8,"aM",@progbits,8
	.align 8
.LC0:
	.long	2684354560
	.long	1073846681
	.ident	"GCC: (GNU) 8.2.1 20180831"
	.section	.note.GNU-stack,"",@progbits
