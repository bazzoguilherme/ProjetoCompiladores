	.file	"fun_doisParam.c"
	.text
	.comm	simples1,4,4
	.comm	simples2,4,4
	.globl	inc
	.type	inc, @function
inc:
.LFB0:
	endbr64
	pushq	%rbp
	movq	%rsp, %rbp
	subq	$16, %rsp
	movl	%edi, -4(%rbp)
	cmpl	$10, -4(%rbp)
	jle	.L2
	movl	-4(%rbp), %eax
	jmp	.L3
.L2:
	movl	-4(%rbp), %eax
	addl	$1, %eax
	movl	%eax, %edi
	call	inc
.L3:
	leave
	ret
.LFE0:
	.size	inc, .-inc
	.globl	main
	.type	main, @function
main:
.LFB1:
	endbr64
	pushq	%rbp
	movq	%rsp, %rbp
	movl	$0, simples1(%rip)
	movl	simples1(%rip), %eax
	movl	%eax, %edi
	call	inc
	movl	%eax, simples1(%rip)
	movl	simples1(%rip), %eax
	popq	%rbp
	ret
.LFE1:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 9.3.0-10ubuntu2) 9.3.0"
