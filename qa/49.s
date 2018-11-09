	.file	"49.c"
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
	subq	$240, %rsp
	movq	%rdi, -232(%rbp)
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	movl	$3, -220(%rbp)
	leaq	.LC1(%rip), %rax
	movq	%rax, -216(%rbp)
	leaq	.LC2(%rip), %rax
	movq	%rax, -216(%rbp)
	movl	$0, -176(%rbp)
	leaq	.LC1(%rip), %rax
	movq	%rax, -168(%rbp)
	leaq	.LC1(%rip), %rax
	movq	%rax, -160(%rbp)
	movl	$0, -152(%rbp)
	leaq	.LC1(%rip), %rax
	movq	%rax, -144(%rbp)
	movl	$0, -136(%rbp)
	leaq	.LC1(%rip), %rax
	movq	%rax, -128(%rbp)
	movl	$0, -120(%rbp)
	leaq	.LC1(%rip), %rax
	movq	%rax, -112(%rbp)
	movl	$0, -104(%rbp)
	movl	-220(%rbp), %eax
	movl	%eax, -176(%rbp)
	movq	-216(%rbp), %rax
	movq	%rax, -168(%rbp)
	leaq	.LC3(%rip), %rax
	movq	%rax, -160(%rbp)
	movl	$2, -152(%rbp)
	movl	$0, -96(%rbp)
	leaq	.LC1(%rip), %rax
	movq	%rax, -88(%rbp)
	leaq	.LC1(%rip), %rax
	movq	%rax, -80(%rbp)
	movl	$0, -72(%rbp)
	leaq	.LC1(%rip), %rax
	movq	%rax, -64(%rbp)
	movl	$0, -56(%rbp)
	leaq	.LC1(%rip), %rax
	movq	%rax, -48(%rbp)
	movl	$0, -40(%rbp)
	leaq	.LC1(%rip), %rax
	movq	%rax, -32(%rbp)
	movl	$0, -24(%rbp)
	movl	$2, -96(%rbp)
	leaq	.LC4(%rip), %rax
	movq	%rax, -88(%rbp)
	leaq	-96(%rbp), %rax
	addq	$48, %rax
	leaq	.LC5(%rip), %rsi
	movq	%rax, %rdi
	call	ext_set_name
	movl	$1, -40(%rbp)
	movl	$0, -208(%rbp)
	leaq	.LC1(%rip), %rax
	movq	%rax, -200(%rbp)
	movq	$0, -192(%rbp)
	leaq	.LC6(%rip), %rax
	movq	%rax, -200(%rbp)
	leaq	-176(%rbp), %rax
	movq	%rax, -192(%rbp)
	movq	-192(%rbp), %rax
	movl	(%rax), %eax
	movl	%eax, %esi
	leaq	.LC7(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	movq	-192(%rbp), %rax
	movq	8(%rax), %rax
	movq	%rax, %rdi
	call	puts@PLT
	movq	-192(%rbp), %rax
	movq	16(%rax), %rax
	movq	%rax, %rdi
	call	puts@PLT
	leaq	-96(%rbp), %rdx
	leaq	-208(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	entity_move_to
	movq	-192(%rbp), %rax
	movl	(%rax), %eax
	movl	%eax, %esi
	leaq	.LC7(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	movq	-192(%rbp), %rax
	movq	8(%rax), %rax
	movq	%rax, %rdi
	call	puts@PLT
	movq	-192(%rbp), %rax
	addq	$48, %rax
	movq	%rax, %rdi
	call	ext_get_name
	movq	%rax, %rdi
	call	puts@PLT
	nop
	movq	-8(%rbp), %rax
	xorq	%fs:40, %rax
	je	.L6
	call	__stack_chk_fail@PLT
.L6:
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
	je	.L11
	call	__stack_chk_fail@PLT
.L11:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE12:
	.size	main, .-main
	.ident	"GCC: (GNU) 8.2.1 20180831"
	.section	.note.GNU-stack,"",@progbits
