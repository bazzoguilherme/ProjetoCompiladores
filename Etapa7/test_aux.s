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
	.globl	mult
	.type	mult, @function
	.globl	multi_add
	.type	multi_add, @function
	.globl	fib
	.type	fib, @function
	.globl	main
	.type	main, @function
inc:
	endbr64
	pushq	%rbp
	movq	%rsp, %rbp
	subq	$20, %rsp
	movl	-16(%rbp), %eax
	subq	$4, %rsp
	movl	%eax, (%rsp)
	movl	$1, %edx
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
	movl	-16(%rbp), %eax
	subq	$4, %rsp
	movl	%eax, (%rsp)
	movl	-20(%rbp), %eax
	movl	%eax, %edx
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
mult:
	endbr64
	pushq	%rbp
	movq	%rsp, %rbp
	subq	$24, %rsp
	movl	-16(%rbp), %eax
	subq	$4, %rsp
	movl	%eax, (%rsp)
	movl	-20(%rbp), %eax
	movl	%eax, %edx
	movl	(%rsp), %eax
	addq	$4, %rsp
	imull	%edx, %eax
	subq	$4, %rsp
	movl	%eax, (%rsp)
	movl	(%rsp), %eax
	addq	$4, %rsp
	leave
	ret
	leave
	ret
multi_add:
	endbr64
	pushq	%rbp
	movq	%rsp, %rbp
	subq	$28, %rsp
	movl	-16(%rbp), %eax
	subq	$4, %rsp
	movl	%eax, (%rsp)
	movl	-20(%rbp), %eax
	movl	%eax, %edx
	movl	(%rsp), %eax
	addq	$4, %rsp
	addl	%edx, %eax
	subq	$4, %rsp
	movl	%eax, (%rsp)
	movl	-24(%rbp), %eax
	movl	%eax, %edx
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
fib:
	endbr64
	pushq	%rbp
	movq	%rsp, %rbp
	subq	$20, %rsp
	movl	-16(%rbp), %eax
	subq	$4, %rsp
	movl	%eax, (%rsp)
	movl	$0, %edx
	movl	(%rsp), %eax
	addq	$4, %rsp
	cmpl	%edx, %eax
	jne	.L7
.L6:
	subq	$4, %rsp
	movl	$0, (%rsp)
	movl	(%rsp), %eax
	addq	$4, %rsp
	leave
	ret
.L7:
	movl	-16(%rbp), %eax
	subq	$4, %rsp
	movl	%eax, (%rsp)
	movl	$1, %edx
	movl	(%rsp), %eax
	addq	$4, %rsp
	cmpl	%edx, %eax
	jne	.L9
.L8:
	subq	$4, %rsp
	movl	$1, (%rsp)
	movl	(%rsp), %eax
	addq	$4, %rsp
	leave
	ret
.L9:
	movl	-16(%rbp), %eax
	subq	$4, %rsp
	movl	%eax, (%rsp)
	movl	$1, %edx
	movl	(%rsp), %eax
	addq	$4, %rsp
	subl	%edx, %eax
	movl	%eax, %eax
	movl	%eax, -32(%rsp)
	call	fib
	subq	$4, %rsp
	movl	%eax, (%rsp)
	movl	-16(%rbp), %eax
	subq	$4, %rsp
	movl	%eax, (%rsp)
	movl	$2, %edx
	movl	(%rsp), %eax
	addq	$4, %rsp
	subl	%edx, %eax
	movl	%eax, %eax
	movl	%eax, -32(%rsp)
	call	fib
	movl	%eax, %edx
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
main:
	endbr64
	pushq	%rbp
	movq	%rsp, %rbp
	subq	$32, %rsp
	subq	$4, %rsp
	movl	$2, (%rsp)
	movl	$5, %edx
	movl	(%rsp), %eax
	addq	$4, %rsp
	addl	%edx, %eax
	movl	%eax, %eax
	movl	%eax, simples1(%rip)
	movl	simples1(%rip), %eax
	subq	$4, %rsp
	movl	%eax, (%rsp)
	movl	$3, %edx
	movl	(%rsp), %eax
	addq	$4, %rsp
	imull	%edx, %eax
	movl	%eax, %eax
	movl	%eax, simples2(%rip)
	movl	$1, %eax
	movl	%eax, -16(%rbp)
	movl	-16(%rbp), %eax
	subq	$4, %rsp
	movl	%eax, (%rsp)
	movl	$2, %edx
	movl	(%rsp), %eax
	addq	$4, %rsp
	cmpl	%edx, %eax
	jne	.L12
.L11:
	movl	$1, %eax
	movl	%eax, if_test(%rip)
	jmp	.L13
.L12:
	movl	$0, %eax
	movl	%eax, if_test(%rip)
	jmp	.L13
.L13:
	movl	$0, %eax
	movl	%eax, while_test(%rip)
	movl	$10, %eax
	movl	%eax, -20(%rbp)
.L16:
	movl	-20(%rbp), %eax
	subq	$4, %rsp
	movl	%eax, (%rsp)
	movl	$0, %edx
	movl	(%rsp), %eax
	addq	$4, %rsp
	cmpl	%edx, %eax
	jle	.L15
.L14:
	movl	-20(%rbp), %eax
	subq	$4, %rsp
	movl	%eax, (%rsp)
	movl	$1, %edx
	movl	(%rsp), %eax
	addq	$4, %rsp
	subl	%edx, %eax
	movl	%eax, %eax
	movl	%eax, -20(%rbp)
	movl	while_test(%rip), %eax
	subq	$4, %rsp
	movl	%eax, (%rsp)
	movl	$1, %edx
	movl	(%rsp), %eax
	addq	$4, %rsp
	addl	%edx, %eax
	movl	%eax, %eax
	movl	%eax, while_test(%rip)
	jmp	.L16
.L15:
	movl	$0, %eax
	movl	%eax, -24(%rbp)
	movl	$0, %eax
	movl	%eax, -24(%rbp)
.L19:
	movl	-24(%rbp), %eax
	subq	$4, %rsp
	movl	%eax, (%rsp)
	movl	$20, %edx
	movl	(%rsp), %eax
	addq	$4, %rsp
	cmpl	%edx, %eax
	jge	.L18
.L17:
	movl	$0, %eax
	movl	%eax, -28(%rbp)
	movl	-24(%rbp), %eax
	subq	$4, %rsp
	movl	%eax, (%rsp)
	movl	$1, %edx
	movl	(%rsp), %eax
	addq	$4, %rsp
	addl	%edx, %eax
	movl	%eax, %eax
	movl	%eax, -24(%rbp)
	jmp	.L19
.L18:
	movl	-24(%rbp), %eax
	movl	%eax, %eax
	movl	%eax, for_test(%rip)
	movl	-24(%rbp), %eax
	subq	$4, %rsp
	movl	%eax, (%rsp)
	movl	$20, %edx
	movl	(%rsp), %eax
	addq	$4, %rsp
	cmpl	%edx, %eax
	jne	.L22
.L20:
	movl	while_test(%rip), %eax
	subq	$4, %rsp
	movl	%eax, (%rsp)
	movl	$10, %edx
	movl	(%rsp), %eax
	addq	$4, %rsp
	cmpl	%edx, %eax
	jne	.L22
.L21:
	subq	$4, %rsp
	movl	$11, (%rsp)
	jmp	.L23
.L22:
	movl	$1, %eax
	negl	%eax
	subq	$4, %rsp
	movl	%eax, (%rsp)
	jmp	.L23
.L23:
	movl	(%rsp), %eax
	addq	$4, %rsp
	movl	%eax, ternario_test(%rip)
	movl	-16(%rbp), %eax
	movl	%eax, %eax
	movl	%eax, -32(%rsp)
	call	inc
	movl	%eax, %eax
	movl	%eax, fun1(%rip)
	movl	$2, %eax
	movl	%eax, -32(%rsp)
	movl	$3, %eax
	movl	%eax, -36(%rsp)
	call	add
	movl	%eax, %eax
	movl	%eax, fun_add(%rip)
	movl	$2, %eax
	movl	%eax, -32(%rsp)
	movl	$3, %eax
	movl	%eax, -36(%rsp)
	call	mult
	movl	%eax, %eax
	movl	%eax, fun_mult(%rip)
	subq	$4, %rsp
	movl	$1, (%rsp)
	movl	$1, %edx
	movl	(%rsp), %eax
	addq	$4, %rsp
	addl	%edx, %eax
	movl	%eax, %eax
	movl	%eax, -32(%rsp)
	movl	$2, %eax
	movl	%eax, -36(%rsp)
	subq	$4, %rsp
	movl	$1, (%rsp)
	movl	$1, %edx
	movl	(%rsp), %eax
	addq	$4, %rsp
	addl	%edx, %eax
	subq	$4, %rsp
	movl	%eax, (%rsp)
	movl	$1, %edx
	movl	(%rsp), %eax
	addq	$4, %rsp
	addl	%edx, %eax
	movl	%eax, %eax
	movl	%eax, -40(%rsp)
	call	multi_add
	movl	%eax, %eax
	movl	%eax, fun3param1(%rip)
	movl	$0, %eax
	movl	%eax, -32(%rsp)
	call	fib
	movl	%eax, %eax
	movl	%eax, fib0(%rip)
	movl	$1, %eax
	movl	%eax, -32(%rsp)
	call	fib
	movl	%eax, %eax
	movl	%eax, fib1(%rip)
	movl	$2, %eax
	movl	%eax, -32(%rsp)
	call	fib
	movl	%eax, %eax
	movl	%eax, fib2(%rip)
	movl	$3, %eax
	movl	%eax, -32(%rsp)
	call	fib
	movl	%eax, %eax
	movl	%eax, fib3(%rip)
	movl	$4, %eax
	movl	%eax, -32(%rsp)
	call	fib
	movl	%eax, %eax
	movl	%eax, fib4(%rip)
	movl	$5, %eax
	movl	%eax, -32(%rsp)
	call	fib
	movl	%eax, %eax
	movl	%eax, fib5(%rip)
	movl	$6, %eax
	movl	%eax, -32(%rsp)
	call	fib
	movl	%eax, %eax
	movl	%eax, fib6(%rip)
	movl	$7, %eax
	movl	%eax, -32(%rsp)
	call	fib
	movl	%eax, %eax
	movl	%eax, fib7(%rip)
	movl	$8, %eax
	movl	%eax, -32(%rsp)
	call	fib
	movl	%eax, %eax
	movl	%eax, fib8(%rip)
	movl	fib7(%rip), %eax
	subq	$4, %rsp
	movl	%eax, (%rsp)
	movl	(%rsp), %eax
	addq	$4, %rsp
	leave
	ret
