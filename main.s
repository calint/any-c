	.file	"main.c"
	.text
	.section	.rodata
.LC0:
	.string	""
.LC1:
	.string	"roome"
.LC2:
	.string	"north"
	.text
	.type	global_main, @function
global_main:
.LFB6:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	addq	$-128, %rsp
	movq	%rdi, -120(%rbp)
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	movl	$0, -96(%rbp)
	leaq	.LC0(%rip), %rax
	movq	%rax, -88(%rbp)
	leaq	.LC0(%rip), %rax
	movq	%rax, -80(%rbp)
	movl	$0, -72(%rbp)
	leaq	.LC0(%rip), %rax
	movq	%rax, -64(%rbp)
	movl	$0, -56(%rbp)
	leaq	.LC0(%rip), %rax
	movq	%rax, -48(%rbp)
	movl	$0, -40(%rbp)
	leaq	.LC0(%rip), %rax
	movq	%rax, -32(%rbp)
	movl	$0, -24(%rbp)
	leaq	.LC1(%rip), %rax
	movq	%rax, -88(%rbp)
	leaq	.LC2(%rip), %rax
	movq	%rax, -80(%rbp)
	movl	$0, -112(%rbp)
	movq	$0, -104(%rbp)
	leaq	-96(%rbp), %rax
	movq	%rax, -104(%rbp)
	movq	-104(%rbp), %rax
	movq	8(%rax), %rax
	movq	%rax, %rdi
	call	puts@PLT
	movq	-104(%rbp), %rax
	movq	16(%rax), %rax
	movq	%rax, %rdi
	call	puts@PLT
	nop
	movq	-8(%rbp), %rax
	xorq	%fs:40, %rax
	je	.L2
	call	__stack_chk_fail@PLT
.L2:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE6:
	.size	global_main, .-global_main
	.type	global_init, @function
global_init:
.LFB7:
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
.LFE7:
	.size	global_init, .-global_init
	.type	global_free, @function
global_free:
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
	.size	global_free, .-global_free
	.globl	main
	.type	main, @function
main:
.LFB9:
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
	je	.L7
	call	__stack_chk_fail@PLT
.L7:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE9:
	.size	main, .-main
	.ident	"GCC: (GNU) 8.2.1 20180831"
	.section	.note.GNU-stack,"",@progbits
