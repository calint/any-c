	.file	"40.c"
	.text
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"glob %d {material "
.LC1:
	.string	"%d"
.LC2:
	.string	"n/a"
.LC3:
	.string	"}"
	.text
	.p2align 4,,15
	.type	glob_print.isra.3, @function
glob_print.isra.3:
.LFB35:
	.cfi_startproc
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	xorl	%eax, %eax
	movq	%rdi, %rbx
	leaq	.LC0(%rip), %rdi
	call	printf@PLT
	movq	(%rbx), %rax
	testq	%rax, %rax
	je	.L2
	movl	(%rax), %esi
	leaq	.LC1(%rip), %rdi
	xorl	%eax, %eax
	call	printf@PLT
.L3:
	leaq	.LC3(%rip), %rdi
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	jmp	puts@PLT
.L2:
	.cfi_restore_state
	leaq	.LC2(%rip), %rdi
	xorl	%eax, %eax
	call	printf@PLT
	jmp	.L3
	.cfi_endproc
.LFE35:
	.size	glob_print.isra.3, .-glob_print.isra.3
	.section	.rodata.str1.1
.LC4:
	.string	"copy of"
.LC5:
	.string	"mat2"
.LC6:
	.string	"mat1"
.LC7:
	.string	"reference to"
	.section	.text.startup,"ax",@progbits
	.p2align 4,,15
	.globl	main
	.type	main, @function
main:
.LFB31:
	.cfi_startproc
	pushq	%r12
	.cfi_def_cfa_offset 16
	.cfi_offset 12, -16
	pxor	%xmm0, %xmm0
	leaq	.LC4(%rip), %rdi
	pushq	%rbp
	.cfi_def_cfa_offset 24
	.cfi_offset 6, -24
	pushq	%rbx
	.cfi_def_cfa_offset 32
	.cfi_offset 3, -32
	addq	$-128, %rsp
	.cfi_def_cfa_offset 160
	movq	%fs:40, %rax
	movq	%rax, 120(%rsp)
	xorl	%eax, %eax
	movq	%rsp, %rbp
	movaps	%xmm0, 64(%rsp)
	leaq	64(%rsp), %rbx
	movabsq	$8589934593, %rax
	movups	%xmm0, 88(%rsp)
	movl	$1, 80(%rsp)
	movl	$1, 104(%rsp)
	movq	%rax, 112(%rsp)
	call	puts@PLT
	movdqa	64(%rsp), %xmm1
	movq	80(%rsp), %rsi
	leaq	8(%rbp), %rdi
	movaps	%xmm1, (%rsp)
	movq	%rsi, 16(%rsp)
	call	glob_print.isra.3
	leaq	.LC5(%rip), %rdx
	leaq	.LC6(%rip), %rax
	cmpq	%rax, %rdx
	je	.L9
	leaq	52(%rbx), %r12
.L7:
	movl	16(%rsp), %esi
	leaq	8(%rbp), %rdi
	movq	%r12, 8(%rsp)
	addq	$8, %rbx
	call	glob_print.isra.3
	movdqa	64(%rsp), %xmm2
	movq	80(%rsp), %rsi
	leaq	40(%rsp), %rdi
	movaps	%xmm2, 32(%rsp)
	movq	%rsi, 48(%rsp)
	call	glob_print.isra.3
	leaq	.LC7(%rip), %rdi
	call	puts@PLT
	movl	80(%rsp), %esi
	movq	%rbx, %rdi
	call	glob_print.isra.3
	movl	80(%rsp), %esi
	movq	%rbx, %rdi
	movq	%r12, 72(%rsp)
	call	glob_print.isra.3
	leaq	.LC7(%rip), %rdi
	call	puts@PLT
	movl	80(%rsp), %esi
	movq	%rbx, %rdi
	call	glob_print.isra.3
	leaq	.LC4(%rip), %rdi
	call	puts@PLT
	movl	80(%rsp), %esi
	movq	%rbx, %rdi
	call	glob_print.isra.3
	xorl	%eax, %eax
	movq	120(%rsp), %rcx
	xorq	%fs:40, %rcx
	jne	.L11
	subq	$-128, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 32
	popq	%rbx
	.cfi_def_cfa_offset 24
	popq	%rbp
	.cfi_def_cfa_offset 16
	popq	%r12
	.cfi_def_cfa_offset 8
	ret
.L9:
	.cfi_restore_state
	leaq	112(%rsp), %r12
	jmp	.L7
.L11:
	call	__stack_chk_fail@PLT
	.cfi_endproc
.LFE31:
	.size	main, .-main
	.ident	"GCC: (GNU) 8.2.1 20180831"
	.section	.note.GNU-stack,"",@progbits
