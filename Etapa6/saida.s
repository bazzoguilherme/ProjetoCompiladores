	.file	"programa.c"
	.text
	.comm	simples1,4
	.comm	simples2,4
	.comm	if_test,4
	.comm	while_test,4
	.comm	for_test,4
	.comm	ternario_test,4
	.comm	fun1,4
	.comm	fun_add,4
	.comm	fun_mult,4
	.comm	fun3param1,4
	.comm	fib0,4
	.comm	fib1,4
	.comm	fib2,4
	.comm	fib3,4
	.comm	fib4,4
	.comm	fib5,4
	.comm	fib6,4
	.comm	fib7,4
	.comm	fib8,4
	.globl	inc
	.type	inc, @function
	.globl	add
	.type	add, @function
	.globl	main
	.type	main, @function
.L1:
inc:
	endbr64
	pushq	%rbp
	movq	%rsp, %rbp
	subq	$20, %rsp
	subq	$4, %rsp
	movl	-16(%rbp), %eax
	movl	%eax, (%rsp)
	subq	$4, %rsp
	movl	$1, (%rsp)
	movl	(%rsp), %edx
	addq	$4, %rsp
	movl	(%rsp), %eax
	addq	$4, %rsp
	addl	%edx, %eax
	subq	$4, %rsp
	movl	%eax, (%rsp)
	movl	(%rsp), %eax
	addq	$4, %rsp
	leave
	ret
	leave
	ret
add:
	endbr64
	pushq	%rbp
	movq	%rsp, %rbp
	subq	$24, %rsp
	subq	$4, %rsp
	movl	-16(%rbp), %eax
	movl	%eax, (%rsp)
	subq	$4, %rsp
	movl	-20(%rbp), %eax
	movl	%eax, (%rsp)
	movl	(%rsp), %edx
	addq	$4, %rsp
	movl	(%rsp), %eax
	addq	$4, %rsp
	addl	%edx, %eax
	subq	$4, %rsp
	movl	%eax, (%rsp)
	movl	(%rsp), %eax
	addq	$4, %rsp
	leave
	ret
	leave
	ret
.L4:
main:
	endbr64
	pushq	%rbp
	movq	%rsp, %rbp
	subq	$16, %rsp
	subq	$4, %rsp
	movl	$0, (%rsp)
	movl	(%rsp), %eax
	addq	$4, %rsp
	movl	%eax, -32(%rsp)
	call	inc
	subq	$4, %rsp
	movl	%eax, (%rsp)
	movl	(%rsp), %eax
	addq	$4, %rsp
	movl	%eax, simples1(%rip)
	subq	$4, %rsp
	movl	$7, (%rsp)
	movl	(%rsp), %eax
	addq	$4, %rsp
	movl	%eax, -32(%rsp)
	subq	$4, %rsp
	movl	$3, (%rsp)
	movl	(%rsp), %eax
	addq	$4, %rsp
	movl	%eax, -36(%rsp)
	call	add
	subq	$4, %rsp
	movl	%eax, (%rsp)
	movl	(%rsp), %eax

	
	leave
	ret
