	.file	"25.c"
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"free entity %d\n"
.LC1:
	.string	"free id %d\n"
	.section	.text.unlikely,"ax",@progbits
.LCOLDB2:
	.text
.LHOTB2:
	.p2align 4,,15
	.type	entity_free, @function
entity_free:
.LFB42:
	.cfi_startproc
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	movl	(%rdi), %edx
	movq	%rdi, %rbx
	movl	$.LC0, %esi
	movl	$1, %edi
	xorl	%eax, %eax
	call	__printf_chk
	movl	(%rbx), %edx
	movl	$.LC1, %esi
	movl	$1, %edi
	popq	%rbx
	.cfi_def_cfa_offset 8
	xorl	%eax, %eax
	jmp	__printf_chk
	.cfi_endproc
.LFE42:
	.size	entity_free, .-entity_free
	.section	.text.unlikely
.LCOLDE2:
	.text
.LHOTE2:
	.section	.rodata.str1.1
.LC5:
	.string	"entity: %d %f\n"
	.section	.text.unlikely
.LCOLDB6:
	.section	.text.startup,"ax",@progbits
.LHOTB6:
	.p2align 4,,15
	.globl	main
	.type	main, @function
main:
.LFB46:
	.cfi_startproc
	subq	$40, %rsp
	.cfi_def_cfa_offset 48
	movl	$1, %edx
	movl	$.LC5, %esi
	movsd	.LC4(%rip), %xmm0
	movl	$1, %edi
	movq	%fs:40, %rax
	movq	%rax, 24(%rsp)
	xorl	%eax, %eax
	movl	$1, %eax
	movl	$0x400ccccd, 4(%rsp)
	movl	$0, 16(%rsp)
	movl	$0x400ccccd, 20(%rsp)
	movl	$1, (%rsp)
	call	__printf_chk
	pxor	%xmm0, %xmm0
	movl	$2, %edx
	movl	$.LC5, %esi
	movl	$1, %edi
	movl	$1, %eax
	movl	$2, 16(%rsp)
	cvtss2sd	20(%rsp), %xmm0
	call	__printf_chk
	pxor	%xmm0, %xmm0
	movl	$3, %edx
	movl	$.LC5, %esi
	movl	$1, %edi
	movl	$1, %eax
	movl	$3, (%rsp)
	cvtss2sd	4(%rsp), %xmm0
	call	__printf_chk
	leaq	16(%rsp), %rdi
	call	entity_free
	movq	%rsp, %rdi
	call	entity_free
	movq	24(%rsp), %rcx
	xorq	%fs:40, %rcx
	jne	.L7
	xorl	%eax, %eax
	addq	$40, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L7:
	.cfi_restore_state
	call	__stack_chk_fail
	.cfi_endproc
.LFE46:
	.size	main, .-main
	.section	.text.unlikely
.LCOLDE6:
	.section	.text.startup
.LHOTE6:
	.section	.rodata.cst8,"aM",@progbits,8
	.align 8
.LC4:
	.long	2684354560
	.long	1073846681
	.ident	"GCC: (Ubuntu 5.4.0-6ubuntu1~16.04.4) 5.4.0 20160609"
	.section	.note.GNU-stack,"",@progbits
