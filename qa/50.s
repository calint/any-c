	.file	"50.c"
	.text
	.type	ext_get_name, @function
ext_get_name:
.LFB6:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rax
	movq	(%rax), %rax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE6:
	.size	ext_get_name, .-ext_get_name
	.type	ext_set_name, @function
ext_set_name:
.LFB7:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	-8(%rbp), %rax
	movq	-16(%rbp), %rdx
	movq	%rdx, (%rax)
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE7:
	.size	ext_set_name, .-ext_set_name
	.section	.rodata
.LC0:
	.string	"%s leaves to %s\n"
	.text
	.type	entity_move_to, @function
entity_move_to:
.LFB8:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	-8(%rbp), %rax
	movq	-16(%rbp), %rdx
	movq	%rdx, 16(%rax)
	movq	-16(%rbp), %rax
	movq	8(%rax), %rdx
	movq	-8(%rbp), %rax
	movq	8(%rax), %rax
	movq	%rax, %rsi
	leaq	.LC0(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE8:
	.size	entity_move_to, .-entity_move_to
	.section	.rodata
.LC1:
	.string	""
.LC2:
	.string	"roome"
.LC3:
	.string	"north"
.LC4:
	.string	"hall"
.LC5:
	.string	"south"
.LC6:
	.string	"me"
.LC7:
	.string	"%d\n"
	.text
	.type	global_main, @function
global_main:
.LFB9:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movq	%rdi, -24(%rbp)
	movl	$3, -12(%rbp)
	leaq	.LC1(%rip), %rax
	movq	%rax, -8(%rbp)
	leaq	.LC2(%rip), %rax
	movq	%rax, -8(%rbp)
	movq	-24(%rbp), %rax
	movl	-12(%rbp), %edx
	movl	%edx, (%rax)
	movq	-24(%rbp), %rax
	movq	-8(%rbp), %rdx
	movq	%rdx, 8(%rax)
	movq	-24(%rbp), %rax
	leaq	.LC3(%rip), %rdx
	movq	%rdx, 16(%rax)
	movq	-24(%rbp), %rax
	movl	$1, 24(%rax)
	movq	-24(%rbp), %rax
	movl	$2, 80(%rax)
	movq	-24(%rbp), %rax
	leaq	.LC4(%rip), %rdx
	movq	%rdx, 88(%rax)
	movq	-24(%rbp), %rax
	subq	$-128, %rax
	leaq	.LC5(%rip), %rsi
	movq	%rax, %rdi
	call	ext_set_name
	movq	-24(%rbp), %rax
	movl	$1, 136(%rax)
	movq	-24(%rbp), %rax
	leaq	.LC6(%rip), %rdx
	movq	%rdx, 168(%rax)
	movq	-24(%rbp), %rdx
	movq	-24(%rbp), %rax
	movq	%rdx, 176(%rax)
	movq	-24(%rbp), %rax
	movq	176(%rax), %rax
	movl	(%rax), %eax
	movl	%eax, %esi
	leaq	.LC7(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	movq	-24(%rbp), %rax
	movq	176(%rax), %rax
	movq	8(%rax), %rax
	movq	%rax, %rdi
	call	puts@PLT
	movq	-24(%rbp), %rax
	movq	176(%rax), %rax
	movq	16(%rax), %rax
	movq	%rax, %rdi
	call	puts@PLT
	movq	-24(%rbp), %rax
	leaq	80(%rax), %rdx
	movq	-24(%rbp), %rax
	addq	$160, %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	entity_move_to
	movq	-24(%rbp), %rax
	movq	176(%rax), %rax
	movl	(%rax), %eax
	movl	%eax, %esi
	leaq	.LC7(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	movq	-24(%rbp), %rax
	movq	176(%rax), %rax
	movq	8(%rax), %rax
	movq	%rax, %rdi
	call	puts@PLT
	movq	-24(%rbp), %rax
	movq	176(%rax), %rax
	addq	$48, %rax
	movq	%rax, %rdi
	call	ext_get_name
	movq	%rax, %rdi
	call	puts@PLT
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE9:
	.size	global_main, .-global_main
	.type	global_init, @function
global_init:
.LFB10:
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
.LFE10:
	.size	global_init, .-global_init
	.type	global_free, @function
global_free:
.LFB11:
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
.LFE11:
	.size	global_free, .-global_free
	.globl	main
	.type	main, @function
main:
.LFB12:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$208, %rsp
	movl	%edi, -196(%rbp)
	movq	%rsi, -208(%rbp)
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	movl	$0, -192(%rbp)
	leaq	.LC1(%rip), %rax
	movq	%rax, -184(%rbp)
	leaq	.LC1(%rip), %rax
	movq	%rax, -176(%rbp)
	movl	$0, -168(%rbp)
	leaq	.LC1(%rip), %rax
	movq	%rax, -160(%rbp)
	movl	$0, -152(%rbp)
	leaq	.LC1(%rip), %rax
	movq	%rax, -144(%rbp)
	movl	$0, -136(%rbp)
	leaq	.LC1(%rip), %rax
	movq	%rax, -128(%rbp)
	movl	$0, -120(%rbp)
	movl	$0, -112(%rbp)
	leaq	.LC1(%rip), %rax
	movq	%rax, -104(%rbp)
	leaq	.LC1(%rip), %rax
	movq	%rax, -96(%rbp)
	movl	$0, -88(%rbp)
	leaq	.LC1(%rip), %rax
	movq	%rax, -80(%rbp)
	movl	$0, -72(%rbp)
	leaq	.LC1(%rip), %rax
	movq	%rax, -64(%rbp)
	movl	$0, -56(%rbp)
	leaq	.LC1(%rip), %rax
	movq	%rax, -48(%rbp)
	movl	$0, -40(%rbp)
	movl	$0, -32(%rbp)
	leaq	.LC1(%rip), %rax
	movq	%rax, -24(%rbp)
	movq	$0, -16(%rbp)
	leaq	-192(%rbp), %rax
	movq	%rax, %rdi
	call	global_init
	leaq	-192(%rbp), %rax
	movq	%rax, %rdi
	call	global_main
	leaq	-192(%rbp), %rax
	movq	%rax, %rdi
	call	global_free
	movl	$0, %eax
	movq	-8(%rbp), %rdx
	xorq	%fs:40, %rdx
	je	.L10
	call	__stack_chk_fail@PLT
.L10:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE12:
	.size	main, .-main
	.ident	"GCC: (GNU) 8.2.1 20180831"
	.section	.note.GNU-stack,"",@progbits
