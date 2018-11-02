	.file	"2.c"
	.text
	.section	.rodata
.LC0:
	.string	"%d %f %d %s %c\n"
	.text
	.type	player_print, @function
player_print:
.LFB6:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rax
	movzbl	24(%rax), %eax
	movsbl	%al, %esi
	movq	-8(%rbp), %rax
	movq	16(%rax), %rcx
	movq	-8(%rbp), %rax
	movzbl	8(%rax), %eax
	movsbl	%al, %edx
	movq	-8(%rbp), %rax
	movss	4(%rax), %xmm0
	cvtss2sd	%xmm0, %xmm0
	movq	-8(%rbp), %rax
	movl	(%rax), %eax
	movl	%esi, %r8d
	movl	%eax, %esi
	leaq	.LC0(%rip), %rdi
	movl	$1, %eax
	call	printf@PLT
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE6:
	.size	player_print, .-player_print
	.section	.rodata
.LC2:
	.string	"hello"
	.text
	.type	global_main, @function
global_main:
.LFB7:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$64, %rsp
	movq	%rdi, -56(%rbp)
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	movl	$1, -48(%rbp)
	movss	.LC1(%rip), %xmm0
	movss	%xmm0, -44(%rbp)
	movb	$1, -40(%rbp)
	leaq	.LC2(%rip), %rax
	movq	%rax, -32(%rbp)
	movb	$46, -24(%rbp)
	leaq	-48(%rbp), %rax
	movq	%rax, %rdi
	call	player_print
	nop
	movq	-8(%rbp), %rax
	xorq	%fs:40, %rax
	je	.L3
	call	__stack_chk_fail@PLT
.L3:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE7:
	.size	global_main, .-global_main
	.type	global_init, @function
global_init:
.LFB8:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -8(%rbp)
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE8:
	.size	global_init, .-global_init
	.type	global_free, @function
global_free:
.LFB9:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -8(%rbp)
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE9:
	.size	global_free, .-global_free
	.globl	main
	.type	main, @function
main:
.LFB10:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movl	%edi, -20(%rbp)
	movq	%rsi, -32(%rbp)
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	leaq	-9(%rbp), %rax
	movq	%rax, %rdi
	call	global_init
	leaq	-9(%rbp), %rax
	movq	%rax, %rdi
	call	global_main
	leaq	-9(%rbp), %rax
	movq	%rax, %rdi
	call	global_free
	movl	$0, %eax
	movq	-8(%rbp), %rdx
	xorq	%fs:40, %rdx
	je	.L8
	call	__stack_chk_fail@PLT
.L8:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE10:
	.size	main, .-main
	.section	.rodata
	.align 4
.LC1:
	.long	1074580685
	.ident	"GCC: (GNU) 8.2.1 20180831"
	.section	.note.GNU-stack,"",@progbits
