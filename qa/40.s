	.file	"40.c"
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"glob %d {material "
.LC1:
	.string	"%d"
.LC2:
	.string	"n/a"
.LC3:
	.string	"}"
	.section	.text.unlikely,"ax",@progbits
.LCOLDB4:
	.text
.LHOTB4:
	.p2align 4,,15
	.type	glob_print.isra.0, @function
glob_print.isra.0:
.LFB48:
	.cfi_startproc
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	movl	%esi, %edx
	movq	%rdi, %rbx
	xorl	%eax, %eax
	movl	$.LC0, %esi
	movl	$1, %edi
	call	__printf_chk
	movq	(%rbx), %rax
	testq	%rax, %rax
	je	.L2
	movl	(%rax), %edx
	movl	$.LC1, %esi
	movl	$1, %edi
	xorl	%eax, %eax
	call	__printf_chk
.L3:
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	movl	$.LC3, %edi
	jmp	puts
.L2:
	.cfi_restore_state
	movl	$.LC2, %esi
	movl	$1, %edi
	xorl	%eax, %eax
	call	__printf_chk
	jmp	.L3
	.cfi_endproc
.LFE48:
	.size	glob_print.isra.0, .-glob_print.isra.0
	.section	.text.unlikely
.LCOLDE4:
	.text
.LHOTE4:
	.section	.rodata.str1.1
.LC5:
	.string	"copy of"
.LC6:
	.string	"mat2"
.LC7:
	.string	"mat1"
.LC8:
	.string	"reference to"
	.section	.text.unlikely
.LCOLDB9:
	.section	.text.startup,"ax",@progbits
.LHOTB9:
	.p2align 4,,15
	.globl	main
	.type	main, @function
main:
.LFB47:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	movl	$.LC5, %edi
	subq	$136, %rsp
	.cfi_def_cfa_offset 160
	movq	%fs:40, %rax
	movq	%rax, 120(%rsp)
	xorl	%eax, %eax
	movq	$0, 64(%rsp)
	movq	$0, 72(%rsp)
	movl	$1, 80(%rsp)
	movq	$0, 88(%rsp)
	movq	$0, 96(%rsp)
	movl	$1, 104(%rsp)
	movl	$1, 112(%rsp)
	movl	$2, 116(%rsp)
	call	puts
	movq	64(%rsp), %rax
	movq	80(%rsp), %rsi
	leaq	8(%rsp), %rdi
	movq	%rax, (%rsp)
	movq	72(%rsp), %rax
	movq	%rsi, 16(%rsp)
	movq	%rax, 8(%rsp)
	call	glob_print.isra.0
	movl	$.LC6, %eax
	cmpq	$.LC7, %rax
	je	.L15
	leaq	64(%rsp), %rbx
	movl	16(%rsp), %esi
	leaq	8(%rsp), %rdi
	leaq	52(%rbx), %rbp
	movq	%rbp, 8(%rsp)
	call	glob_print.isra.0
	movq	64(%rsp), %rax
	movq	80(%rsp), %rsi
	movq	%rax, 32(%rsp)
	movq	72(%rsp), %rax
	movq	%rsi, 48(%rsp)
	movq	%rax, 40(%rsp)
.L14:
	leaq	40(%rsp), %rdi
	call	glob_print.isra.0
	movl	$.LC8, %edi
	call	puts
	movl	80(%rsp), %esi
	leaq	8(%rbx), %rdi
	addq	$8, %rbx
	call	glob_print.isra.0
	movl	80(%rsp), %esi
	movq	%rbx, %rdi
	movq	%rbp, 72(%rsp)
	call	glob_print.isra.0
	movl	$.LC8, %edi
	call	puts
	movl	80(%rsp), %esi
	movq	%rbx, %rdi
	call	glob_print.isra.0
	movl	$.LC5, %edi
	call	puts
	movl	80(%rsp), %esi
	movq	%rbx, %rdi
	call	glob_print.isra.0
	xorl	%eax, %eax
	movq	120(%rsp), %rdx
	xorq	%fs:40, %rdx
	jne	.L16
	addq	$136, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	ret
.L15:
	.cfi_restore_state
	leaq	64(%rsp), %rbx
	movl	16(%rsp), %esi
	leaq	8(%rsp), %rdi
	leaq	48(%rbx), %rbp
	movq	%rbp, 8(%rsp)
	call	glob_print.isra.0
	leaq	32(%rsp), %rdi
	movl	$6, %ecx
	movq	%rbx, %rsi
	rep movsl
	movl	48(%rsp), %esi
	jmp	.L14
.L16:
	call	__stack_chk_fail
	.cfi_endproc
.LFE47:
	.size	main, .-main
	.section	.text.unlikely
.LCOLDE9:
	.section	.text.startup
.LHOTE9:
	.ident	"GCC: (Ubuntu 5.4.0-6ubuntu1~16.04.4) 5.4.0 20160609"
	.section	.note.GNU-stack,"",@progbits
