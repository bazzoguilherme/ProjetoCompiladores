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
	.globl	main
	.type	main, @function
.L1:
inc:
	endbr64
	pushq	%rbp
	movq	%rsp, %rbp
	
	
	leave
	ret
.L2:
main:
	endbr64
	pushq	%rbp
	movq	%rsp, %rbp
	subq	$16, %rsp
	subq	$4, %rsp
	movl	$0, (%rsp)
	movl	(%rsp), %eax
	addq	$4, %rsp
	movl	%eax, simples1(%rip)

	subq	$4, %rsp
	movl	simples1(%rip), %eax
	movl	%eax, (%rsp)
	movl	(%rsp), %eax
	addq	$4, %rsp
	movl	%eax, -16(%rsp)
	call	inc
	movl	-16(%rsp), %eax
	leave
	ret
