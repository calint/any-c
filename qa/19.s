	.file	"19.c"
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC1:
	.string	"entity: %d %f\n"
.LC2:
	.string	"free global"
.LC3:
	.string	"free entity: %d\n"
.LC4:
	.string	"free id"
	.section	.text.unlikely,"ax",@progbits
.LCOLDB5:
	.section	.text.startup,"ax",@progbits
.LHOTB5:
	.p2align 4,,15
	.globl	main
	.type	main, @function
main:
.LFB47:
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
	movl	$2, %edx
	movl	$.LC1, %esi
	movl	$1, %edi
	movl	$1, %eax
	call	__printf_chk
	movsd	.LC0(%rip), %xmm0
	movl	$3, %edx
	movl	$.LC1, %esi
	movl	$1, %edi
	movl	$1, %eax
	call	__printf_chk
	movl	$.LC2, %edi
	call	puts
	movl	$2, %edx
	movl	$.LC3, %esi
	movl	$1, %edi
	xorl	%eax, %eax
	call	__printf_chk
	movl	$.LC4, %edi
	call	puts
	movl	$3, %edx
	movl	$.LC3, %esi
	movl	$1, %edi
	xorl	%eax, %eax
	call	__printf_chk
	movl	$.LC4, %edi
	call	puts
	xorl	%eax, %eax
	addq	$8, %rsp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE47:
	.size	main, .-main
	.section	.text.unlikely
.LCOLDE5:
	.section	.text.startup
.LHOTE5:
	.section	.rodata.cst8,"aM",@progbits,8
	.align 8
.LC0:
	.long	2684354560
	.long	1073846681
	.ident	"GCC: (Ubuntu 5.4.0-6ubuntu1~16.04.4) 5.4.0 20160609"
	.section	.note.GNU-stack,"",@progbits