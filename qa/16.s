	.file	"16.c"
	.text
	.section	.rodata
.LC0:
	.string	"draw_render_range"
	.text
	.type	render_range_draw, @function
render_range_draw:
.LFB6:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	leaq	.LC0(%rip), %rdi
	call	puts@PLT
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE6:
	.size	render_range_draw, .-render_range_draw
	.section	.rodata
.LC1:
	.string	"draw_glo"
	.text
	.type	glo_draw, @function
glo_draw:
.LFB7:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	leaq	.LC1(%rip), %rdi
	call	puts@PLT
	movq	-8(%rbp), %rax
	movq	%rax, %rdi
	call	render_range_draw
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE7:
	.size	glo_draw, .-glo_draw
	.section	.rodata
.LC2:
	.string	"draw_global"
	.text
	.type	global_draw, @function
global_draw:
.LFB8:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	leaq	.LC2(%rip), %rdi
	call	puts@PLT
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE8:
	.size	global_draw, .-global_draw
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
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	movq	-24(%rbp), %rax
	movq	%rax, %rdi
	call	global_draw
	movq	-24(%rbp), %rax
	movq	%rax, %rdi
	call	glo_draw
	movl	$0, -16(%rbp)
	movl	$0, -12(%rbp)
	leaq	-16(%rbp), %rax
	movq	%rax, %rdi
	call	glo_draw
	leaq	-16(%rbp), %rax
	movq	%rax, %rdi
	call	render_range_draw
	nop
	movq	-8(%rbp), %rax
	xorq	%fs:40, %rax
	je	.L5
	call	__stack_chk_fail@PLT
.L5:
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
	movl	$0, -16(%rbp)
	movl	$0, -12(%rbp)
	leaq	-16(%rbp), %rax
	movq	%rax, %rdi
	call	global_init
	leaq	-16(%rbp), %rax
	movq	%rax, %rdi
	call	global_main
	leaq	-16(%rbp), %rax
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
