	.file	"programa.c"
	.text
	.comm	simples1,4
	.comm	simples2,4
	.globl	main
	.type	main, @function
.L1:
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
	movl	$2, (%rsp)
	subq	$4, %rsp
	movl	$5, (%rsp)
	movl	(%rsp), %edx
	addq	$4, %rsp
	movl	(%rsp), %eax
	addq	$4, %rsp
	cmp	%edx, %eax
	jge	.L4
.L2:
	subq	$4, %rsp
	movl	$3, (%rsp)
	subq	$4, %rsp
	movl	$4, (%rsp)
	movl	(%rsp), %edx
	addq	$4, %rsp
	movl	(%rsp), %eax
	addq	$4, %rsp
	cmp	%edx, %eax
	jne	.L4
.L3:
	subq	$4, %rsp
	movl	$1, (%rsp)
	movl	(%rsp), %eax
	addq	$4, %rsp
	movl	%eax, simples1(%rip)
.L4:
	subq	$4, %rsp
	movl	$9, (%rsp)
	movl	(%rsp), %eax
	addq	$4, %rsp
	movl	%eax, simples1(%rip)
.L5:

	leave
	ret
