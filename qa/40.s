	.file	"40.c"
	.text
	.type	materials__init, @function
materials__init:
.LFB7:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rax
	movl	$1, (%rax)
	movq	-8(%rbp), %rax
	movl	$2, 4(%rax)
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE7:
	.size	materials__init, .-materials__init
	.section	.rodata
.LC0:
	.string	"mat1"
.LC1:
	.string	"mat2"
	.text
	.type	materials_get, @function
materials_get:
.LFB8:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	leaq	.LC0(%rip), %rax
	cmpq	%rax, -16(%rbp)
	jne	.L3
	movq	-8(%rbp), %rax
	jmp	.L2
.L3:
	leaq	.LC1(%rip), %rax
	cmpq	%rax, -16(%rbp)
	jne	.L5
	movq	-8(%rbp), %rax
	addq	$4, %rax
	jmp	.L2
.L5:
.L2:
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE8:
	.size	materials_get, .-materials_get
	.type	materials_init, @function
materials_init:
.LFB9:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$8, %rsp
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rax
	movq	%rax, %rdi
	call	materials__init
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE9:
	.size	materials_init, .-materials_init
	.section	.rodata
.LC2:
	.string	"glob %d {material "
.LC3:
	.string	"%d"
.LC4:
	.string	"n/a"
.LC5:
	.string	"}"
	.text
	.type	glob_print, @function
glob_print:
.LFB10:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rax
	movl	16(%rax), %eax
	movl	%eax, %esi
	leaq	.LC2(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	movq	-8(%rbp), %rax
	movq	8(%rax), %rax
	testq	%rax, %rax
	je	.L8
	movq	-8(%rbp), %rax
	movq	8(%rax), %rax
	movl	(%rax), %eax
	movl	%eax, %esi
	leaq	.LC3(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	jmp	.L9
.L8:
	leaq	.LC4(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
.L9:
	leaq	.LC5(%rip), %rdi
	call	puts@PLT
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE10:
	.size	glob_print, .-glob_print
	.type	globs_get, @function
globs_get:
.LFB11:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	-8(%rbp), %rax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE11:
	.size	globs_get, .-globs_get
	.section	.rodata
.LC6:
	.string	"copy of"
.LC7:
	.string	"a"
.LC8:
	.string	"reference to"
	.text
	.type	global_main, @function
global_main:
.LFB12:
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
	leaq	.LC6(%rip), %rdi
	call	puts@PLT
	movq	-120(%rbp), %rax
	leaq	.LC7(%rip), %rsi
	movq	%rax, %rdi
	call	globs_get
	movq	%rax, %rcx
	movq	(%rcx), %rax
	movq	8(%rcx), %rdx
	movq	%rax, -96(%rbp)
	movq	%rdx, -88(%rbp)
	movq	16(%rcx), %rax
	movq	%rax, -80(%rbp)
	leaq	-96(%rbp), %rax
	movq	%rax, %rdi
	call	glob_print
	movq	-120(%rbp), %rax
	addq	$48, %rax
	leaq	.LC1(%rip), %rsi
	movq	%rax, %rdi
	call	materials_get
	movq	%rax, -88(%rbp)
	leaq	-96(%rbp), %rax
	movq	%rax, %rdi
	call	glob_print
	movq	-120(%rbp), %rax
	leaq	.LC7(%rip), %rsi
	movq	%rax, %rdi
	call	globs_get
	movq	%rax, %rcx
	movq	(%rcx), %rax
	movq	8(%rcx), %rdx
	movq	%rax, -64(%rbp)
	movq	%rdx, -56(%rbp)
	movq	16(%rcx), %rax
	movq	%rax, -48(%rbp)
	leaq	-64(%rbp), %rax
	movq	%rax, %rdi
	call	glob_print
	leaq	.LC8(%rip), %rdi
	call	puts@PLT
	movq	-120(%rbp), %rax
	leaq	.LC7(%rip), %rsi
	movq	%rax, %rdi
	call	globs_get
	movq	%rax, -112(%rbp)
	movq	-112(%rbp), %rax
	movq	%rax, %rdi
	call	glob_print
	movq	-120(%rbp), %rax
	addq	$48, %rax
	leaq	.LC1(%rip), %rsi
	movq	%rax, %rdi
	call	materials_get
	movq	%rax, %rdx
	movq	-112(%rbp), %rax
	movq	%rdx, 8(%rax)
	movq	-112(%rbp), %rax
	movq	%rax, %rdi
	call	glob_print
	leaq	.LC8(%rip), %rdi
	call	puts@PLT
	movq	-120(%rbp), %rax
	leaq	.LC7(%rip), %rsi
	movq	%rax, %rdi
	call	globs_get
	movq	%rax, -104(%rbp)
	movq	-104(%rbp), %rax
	movq	%rax, %rdi
	call	glob_print
	leaq	.LC6(%rip), %rdi
	call	puts@PLT
	movq	-120(%rbp), %rax
	leaq	.LC7(%rip), %rsi
	movq	%rax, %rdi
	call	globs_get
	movq	%rax, %rcx
	movq	(%rcx), %rax
	movq	8(%rcx), %rdx
	movq	%rax, -32(%rbp)
	movq	%rdx, -24(%rbp)
	movq	16(%rcx), %rax
	movq	%rax, -16(%rbp)
	movq	-104(%rbp), %rax
	movq	%rax, %rdi
	call	glob_print
	nop
	movq	-8(%rbp), %rax
	xorq	%fs:40, %rax
	je	.L13
	call	__stack_chk_fail@PLT
.L13:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE12:
	.size	global_main, .-global_main
	.type	global_init, @function
global_init:
.LFB13:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$8, %rsp
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rax
	addq	$48, %rax
	movq	%rax, %rdi
	call	materials_init
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE13:
	.size	global_init, .-global_init
	.type	global_free, @function
global_free:
.LFB14:
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
.LFE14:
	.size	global_free, .-global_free
	.globl	main
	.type	main, @function
main:
.LFB15:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$80, %rsp
	movl	%edi, -68(%rbp)
	movq	%rsi, -80(%rbp)
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	movq	$0, -64(%rbp)
	movq	$0, -56(%rbp)
	movl	$1, -48(%rbp)
	movq	$0, -40(%rbp)
	movq	$0, -32(%rbp)
	movl	$1, -24(%rbp)
	movl	$1, -16(%rbp)
	movl	$1, -12(%rbp)
	leaq	-64(%rbp), %rax
	movq	%rax, %rdi
	call	global_init
	leaq	-64(%rbp), %rax
	movq	%rax, %rdi
	call	global_main
	leaq	-64(%rbp), %rax
	movq	%rax, %rdi
	call	global_free
	movl	$0, %eax
	movq	-8(%rbp), %rdx
	xorq	%fs:40, %rdx
	je	.L18
	call	__stack_chk_fail@PLT
.L18:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE15:
	.size	main, .-main
	.ident	"GCC: (GNU) 8.2.1 20180831"
	.section	.note.GNU-stack,"",@progbits
