	.file	"c.c"
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC1:
	.string	"entity: %d %f\n"
	.section	.text.unlikely,"ax",@progbits
.LCOLDB2:
	.section	.text.startup,"ax",@progbits
.LHOTB2:
	.p2align 4,,15
	.globl	main
	.type	main, @function
main:
.LFB42:
	.cfi_startproc
	subq	$8, %rsp
	.cfi_def_cfa_offset 16
	movl	$1, %edx
	movl	$.LC1, %esi
	movsd	.LC0(%rip), %xmm0
	movl	$1, %edi
	movl	$1, %eax
	call	__printf_chk
	movsd	.LC0(%rip), %xmm0
	movl	$1, %edx
	movl	$.LC1, %esi
	movl	$1, %edi
	movl	$1, %eax
	call	__printf_chk
	movsd	.LC0(%rip), %xmm0
	movl	$2, %edx
	movl	$.LC1, %esi
	movl	$1, %edi
	movl	$1, %eax
	call	__printf_chk
	xorl	%eax, %eax
	addq	$8, %rsp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE42:
	.size	main, .-main
	.section	.text.unlikely
.LCOLDE2:
	.section	.text.startup
.LHOTE2:
	.section	.rodata.cst8,"aM",@progbits,8
	.align 8
.LC0:
	.long	2684354560
	.long	1073846681
	.ident	"GCC: (Ubuntu 5.3.1-14ubuntu2) 5.3.1 20160413"
	.section	.note.GNU-stack,"",@progbits
