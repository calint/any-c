	.file	"52.c"
	.text
	.section	.rodata
.LC0:
	.string	"true"
.LC1:
	.string	"false"
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
	subq	$32, %rsp
	movq	%rdi, -24(%rbp)
	movl	$1, -12(%rbp)
	movl	$2, -8(%rbp)
	movl	$3, -4(%rbp)
	cmpl	$1, -12(%rbp)
	jne	.L2
	leaq	.LC0(%rip), %rdi
	call	puts@PLT
	jmp	.L3
.L2:
	leaq	.LC1(%rip), %rdi
	call	puts@PLT
.L3:
	cmpl	$1, -12(%rbp)
	jne	.L4
	cmpl	$2, -8(%rbp)
	jne	.L4
	leaq	.LC0(%rip), %rdi
	call	puts@PLT
	jmp	.L5
.L4:
	leaq	.LC1(%rip), %rdi
	call	puts@PLT
.L5:
	cmpl	$1, -12(%rbp)
	je	.L6
	leaq	.LC0(%rip), %rdi
	call	puts@PLT
	jmp	.L7
.L6:
	leaq	.LC1(%rip), %rdi
	call	puts@PLT
.L7:
	cmpl	$1, -12(%rbp)
	jne	.L8
	cmpl	$2, -8(%rbp)
	jne	.L9
	cmpl	$3, -4(%rbp)
	je	.L8
.L9:
	leaq	.LC0(%rip), %rdi
	call	puts@PLT
	jmp	.L10
.L8:
	leaq	.LC1(%rip), %rdi
	call	puts@PLT
.L10:
	nop
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
	je	.L15
	call	__stack_chk_fail@PLT
.L15:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE9:
	.size	main, .-main
	.ident	"GCC: (GNU) 8.2.1 20180831"
	.section	.note.GNU-stack,"",@progbits
