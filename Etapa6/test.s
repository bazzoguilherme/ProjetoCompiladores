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
	movl	-16(%rbp), %eax
	subq	$4, %rsp
	movl	%eax, (%rsp)
	movl	-20(%rbp), %eax
	subq	$4, %rsp
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
mult:
	endbr64
	pushq	%rbp
	movq	%rsp, %rbp
	subq	$24, %rsp
	movl	-16(%rbp), %eax
	subq	$4, %rsp
	movl	%eax, (%rsp)
	movl	-20(%rbp), %eax
	subq	$4, %rsp
	movl	%eax, (%rsp)
	movl	(%rsp), %edx
	addq	$4, %rsp
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
	subq	$4, %rsp
	movl	%eax, (%rsp)
	movl	(%rsp), %edx
	addq	$4, %rsp
	movl	(%rsp), %eax
	addq	$4, %rsp
	addl	%edx, %eax
	subq	$4, %rsp
	movl	%eax, (%rsp)
	movl	-24(%rbp), %eax
	subq	$4, %rsp
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
fib:
	endbr64
	pushq	%rbp
	movq	%rsp, %rbp
	subq	$20, %rsp
	movl	-16(%rbp), %eax
	subq	$4, %rsp
	movl	%eax, (%rsp)
	subq	$4, %rsp
	movl	$0, (%rsp)
	movl	(%rsp), %edx
	addq	$4, %rsp
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
	subq	$4, %rsp
	movl	$1, (%rsp)
	movl	(%rsp), %edx
	addq	$4, %rsp
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
	subq	$4, %rsp
	movl	$1, (%rsp)
	movl	(%rsp), %edx
	addq	$4, %rsp
	movl	(%rsp), %eax
	addq	$4, %rsp
	subl	%edx, %eax
	subq	$4, %rsp
	movl	%eax, (%rsp)
	movl	(%rsp), %eax
	addq	$4, %rsp
	movl	%eax, -32(%rsp)
	call	fib
	subq	$4, %rsp
	movl	%eax, (%rsp)
	movl	-16(%rbp), %eax
	subq	$4, %rsp
	movl	%eax, (%rsp)
	subq	$4, %rsp
	movl	$2, (%rsp)
	movl	(%rsp), %edx
	addq	$4, %rsp
	movl	(%rsp), %eax
	addq	$4, %rsp
	subl	%edx, %eax
	subq	$4, %rsp
	movl	%eax, (%rsp)
	movl	(%rsp), %eax
	addq	$4, %rsp
	movl	%eax, -32(%rsp)
	call	fib
	subq	$4, %rsp
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
main:
	endbr64
	pushq	%rbp
	movq	%rsp, %rbp
	subq	$32, %rsp
	subq	$4, %rsp
	movl	$2, (%rsp)
	subq	$4, %rsp
	movl	$5, (%rsp)
	movl	(%rsp), %edx
	addq	$4, %rsp
	movl	(%rsp), %eax
	addq	$4, %rsp
	addl	%edx, %eax
	subq	$4, %rsp
	movl	%eax, (%rsp)
	movl	(%rsp), %eax
	addq	$4, %rsp
	movl	%eax, simples1(%rip)
	movl	simples1(%rip), %eax
	subq	$4, %rsp
	movl	%eax, (%rsp)
	subq	$4, %rsp
	movl	$3, (%rsp)
	movl	(%rsp), %edx
	addq	$4, %rsp
	movl	(%rsp), %eax
	addq	$4, %rsp
	imull	%edx, %eax
	subq	$4, %rsp
	movl	%eax, (%rsp)
	movl	(%rsp), %eax
	addq	$4, %rsp
	movl	%eax, simples2(%rip)
	subq	$4, %rsp
	movl	$1, (%rsp)
	movl	(%rsp), %eax
	addq	$4, %rsp
	movl	%eax, -16(%rbp)
	movl	-16(%rbp), %eax
	subq	$4, %rsp
	movl	%eax, (%rsp)
	subq	$4, %rsp
	movl	$2, (%rsp)
	movl	(%rsp), %edx
	addq	$4, %rsp
	movl	(%rsp), %eax
	addq	$4, %rsp
	cmpl	%edx, %eax
	jne	.L12
.L11:
	subq	$4, %rsp
	movl	$1, (%rsp)
	movl	(%rsp), %eax
	addq	$4, %rsp
	movl	%eax, if_test(%rip)
	jmp	.L13
.L12:
	subq	$4, %rsp
	movl	$0, (%rsp)
	movl	(%rsp), %eax
	addq	$4, %rsp
	movl	%eax, if_test(%rip)
	jmp	.L13
.L13:
	subq	$4, %rsp
	movl	$0, (%rsp)
	movl	(%rsp), %eax
	addq	$4, %rsp
	movl	%eax, while_test(%rip)
	subq	$4, %rsp
	movl	$10, (%rsp)
	movl	(%rsp), %eax
	addq	$4, %rsp
	movl	%eax, -20(%rbp)
.L16:
	movl	-20(%rbp), %eax
	subq	$4, %rsp
	movl	%eax, (%rsp)
	subq	$4, %rsp
	movl	$0, (%rsp)
	movl	(%rsp), %edx
	addq	$4, %rsp
	movl	(%rsp), %eax
	addq	$4, %rsp
	cmpl	%edx, %eax
	jle	.L15
.L14:
	movl	-20(%rbp), %eax
	subq	$4, %rsp
	movl	%eax, (%rsp)
	subq	$4, %rsp
	movl	$1, (%rsp)
	movl	(%rsp), %edx
	addq	$4, %rsp
	movl	(%rsp), %eax
	addq	$4, %rsp
	subl	%edx, %eax
	subq	$4, %rsp
	movl	%eax, (%rsp)
	movl	(%rsp), %eax
	addq	$4, %rsp
	movl	%eax, -20(%rbp)
	movl	while_test(%rip), %eax
	subq	$4, %rsp
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
	movl	%eax, while_test(%rip)
	jmp	.L16
.L15:
	subq	$4, %rsp
	movl	$0, (%rsp)
	movl	(%rsp), %eax
	addq	$4, %rsp
	movl	%eax, -24(%rbp)
	subq	$4, %rsp
	movl	$0, (%rsp)
	movl	(%rsp), %eax
	addq	$4, %rsp
	movl	%eax, -24(%rbp)
.L19:
	movl	-24(%rbp), %eax
	subq	$4, %rsp
	movl	%eax, (%rsp)
	subq	$4, %rsp
	movl	$20, (%rsp)
	movl	(%rsp), %edx
	addq	$4, %rsp
	movl	(%rsp), %eax
	addq	$4, %rsp
	cmpl	%edx, %eax
	jge	.L18
.L17:
	subq	$4, %rsp
	movl	$0, (%rsp)
	movl	(%rsp), %eax
	addq	$4, %rsp
	movl	%eax, -28(%rbp)
	movl	-24(%rbp), %eax
	subq	$4, %rsp
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
	movl	%eax, -24(%rbp)
	jmp	.L19
.L18:
	movl	-24(%rbp), %eax
	subq	$4, %rsp
	movl	%eax, (%rsp)
	movl	(%rsp), %eax
	addq	$4, %rsp
	movl	%eax, for_test(%rip)
	movl	-24(%rbp), %eax
	subq	$4, %rsp
	movl	%eax, (%rsp)
	subq	$4, %rsp
	movl	$20, (%rsp)
	movl	(%rsp), %edx
	addq	$4, %rsp
	movl	(%rsp), %eax
	addq	$4, %rsp
	cmpl	%edx, %eax
	jne	.L22
.L20:
	movl	while_test(%rip), %eax
	subq	$4, %rsp
	movl	%eax, (%rsp)
	subq	$4, %rsp
	movl	$10, (%rsp)
	movl	(%rsp), %edx
	addq	$4, %rsp
	movl	(%rsp), %eax
	addq	$4, %rsp
	cmpl	%edx, %eax
	jne	.L22
.L21:
	subq	$4, %rsp
	movl	$11, (%rsp)
	jmp	.L23
.L22:
	subq	$4, %rsp
	movl	$1, (%rsp)
	movl	(%rsp), %eax
	addq	$4, %rsp
	neg	%eax
	subq	$4, %rsp
	movl	%eax, (%rsp)
	jmp	.L23
.L23:
	movl	(%rsp), %eax
	addq	$4, %rsp
	movl	%eax, ternario_test(%rip)
	movl	-16(%rbp), %eax
	subq	$4, %rsp
	movl	%eax, (%rsp)
	movl	(%rsp), %eax
	addq	$4, %rsp
	movl	%eax, -32(%rsp)
	call	inc
	subq	$4, %rsp
	movl	%eax, (%rsp)
	movl	(%rsp), %eax
	addq	$4, %rsp
	movl	%eax, fun1(%rip)
	subq	$4, %rsp
	movl	$2, (%rsp)
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
	addq	$4, %rsp
	movl	%eax, fun_add(%rip)
	subq	$4, %rsp
	movl	$2, (%rsp)
	movl	(%rsp), %eax
	addq	$4, %rsp
	movl	%eax, -32(%rsp)
	subq	$4, %rsp
	movl	$3, (%rsp)
	movl	(%rsp), %eax
	addq	$4, %rsp
	movl	%eax, -36(%rsp)
	call	mult
	subq	$4, %rsp
	movl	%eax, (%rsp)
	movl	(%rsp), %eax
	addq	$4, %rsp
	movl	%eax, fun_mult(%rip)
	subq	$4, %rsp
	movl	$1, (%rsp)
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
	movl	%eax, -32(%rsp)
	subq	$4, %rsp
	movl	$2, (%rsp)
	movl	(%rsp), %eax
	addq	$4, %rsp
	movl	%eax, -36(%rsp)
	subq	$4, %rsp
	movl	$1, (%rsp)
	subq	$4, %rsp
	movl	$1, (%rsp)
	movl	(%rsp), %edx
	addq	$4, %rsp
	movl	(%rsp), %eax
	addq	$4, %rsp
	addl	%edx, %eax
	subq	$4, %rsp
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
	movl	%eax, -40(%rsp)
	call	multi_add
	subq	$4, %rsp
	movl	%eax, (%rsp)
	movl	(%rsp), %eax
	addq	$4, %rsp
	movl	%eax, fun3param1(%rip)
	subq	$4, %rsp
	movl	$0, (%rsp)
	movl	(%rsp), %eax
	addq	$4, %rsp
	movl	%eax, -32(%rsp)
	call	fib
	subq	$4, %rsp
	movl	%eax, (%rsp)
	movl	(%rsp), %eax
	addq	$4, %rsp
	movl	%eax, fib0(%rip)
	subq	$4, %rsp
	movl	$1, (%rsp)
	movl	(%rsp), %eax
	addq	$4, %rsp
	movl	%eax, -32(%rsp)
	call	fib
	subq	$4, %rsp
	movl	%eax, (%rsp)
	movl	(%rsp), %eax
	addq	$4, %rsp
	movl	%eax, fib1(%rip)
	subq	$4, %rsp
	movl	$2, (%rsp)
	movl	(%rsp), %eax
	addq	$4, %rsp
	movl	%eax, -32(%rsp)
	call	fib
	subq	$4, %rsp
	movl	%eax, (%rsp)
	movl	(%rsp), %eax
	addq	$4, %rsp
	movl	%eax, fib2(%rip)
	subq	$4, %rsp
	movl	$3, (%rsp)
	movl	(%rsp), %eax
	addq	$4, %rsp
	movl	%eax, -32(%rsp)
	call	fib
	subq	$4, %rsp
	movl	%eax, (%rsp)
	movl	(%rsp), %eax
	addq	$4, %rsp
	movl	%eax, fib3(%rip)
	subq	$4, %rsp
	movl	$4, (%rsp)
	movl	(%rsp), %eax
	addq	$4, %rsp
	movl	%eax, -32(%rsp)
	call	fib
	subq	$4, %rsp
	movl	%eax, (%rsp)
	movl	(%rsp), %eax
	addq	$4, %rsp
	movl	%eax, fib4(%rip)
	subq	$4, %rsp
	movl	$5, (%rsp)
	movl	(%rsp), %eax
	addq	$4, %rsp
	movl	%eax, -32(%rsp)
	call	fib
	subq	$4, %rsp
	movl	%eax, (%rsp)
	movl	(%rsp), %eax
	addq	$4, %rsp
	movl	%eax, fib5(%rip)
	subq	$4, %rsp
	movl	$6, (%rsp)
	movl	(%rsp), %eax
	addq	$4, %rsp
	movl	%eax, -32(%rsp)
	call	fib
	subq	$4, %rsp
	movl	%eax, (%rsp)
	movl	(%rsp), %eax
	addq	$4, %rsp
	movl	%eax, fib6(%rip)
	subq	$4, %rsp
	movl	$7, (%rsp)
	movl	(%rsp), %eax
	addq	$4, %rsp
	movl	%eax, -32(%rsp)
	call	fib
	subq	$4, %rsp
	movl	%eax, (%rsp)
	movl	(%rsp), %eax
	addq	$4, %rsp
	movl	%eax, fib7(%rip)
	subq	$4, %rsp
	movl	$8, (%rsp)
	movl	(%rsp), %eax
	addq	$4, %rsp
	movl	%eax, -32(%rsp)
	call	fib
	subq	$4, %rsp
	movl	%eax, (%rsp)
	movl	(%rsp), %eax
	addq	$4, %rsp
	movl	%eax, fib8(%rip)
	movl	fib8(%rip), %eax
	subq	$4, %rsp
	movl	%eax, (%rsp)
	movl	(%rsp), %eax
	addq	$4, %rsp
	leave
	ret
