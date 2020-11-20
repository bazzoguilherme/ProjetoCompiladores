	.file	"fun_doisParam.c"
	.text
	.globl	mult
	.type	mult, @function
mult:
	endbr64
	pushq	%rbp
	movq	%rsp, %rbp
	movl	%edi, -20(%rbp)
	movl	%esi, -24(%rbp)
	cmpl	$0, -20(%rbp)
	jle	.L2
	movl	-20(%rbp), %eax
	imull	-24(%rbp), %eax
	movl	%eax, -4(%rbp)
	jmp	.L3
.L2:
	movl	-24(%rbp), %eax
	movl	%eax, -4(%rbp)
.L3:
	movl	-4(%rbp), %eax
	popq	%rbp
	ret
.LFE0:
	.size	mult, .-mult
	.globl	main
	.type	main, @function
main:
.LFB1:
	endbr64
	pushq	%rbp
	movq	%rsp, %rbp
	subq	$16, %rsp
	movl	$2, -8(%rbp)
	movl	-8(%rbp), %edx
	movl	-8(%rbp), %eax
	movl	%edx, %esi
	movl	%eax, %edi
	call	mult
	movl	%eax, -4(%rbp)
	movl	$0, %eax
	leave
	ret
.LFE1:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 9.3.0-10ubuntu2) 9.3.0"
