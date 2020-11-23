	.file	"other_fun.c"
	.text
	.comm	global,4,4
	.globl	inc
	.type	inc, @function
	.globl	main
	.type	main, @function
inc:
	pushq	%rbp
	movq	%rsp, %rbp
	movl	%edi, -4(%rbp)
	movl	-4(%rbp), %eax
	addl	$1, %eax
	popq	%rbp
	ret
main:
	pushq	%rbp
	movq	%rsp, %rbp
	movl	$0, global(%rip)
	jmp	.L4
.L5:
	movl	global(%rip), %eax
	movl	%eax, %edi
	call	inc
	movl	%eax, global(%rip)
.L4:
	movl	global(%rip), %eax
	cmpl	$10, %eax
	jle	.L5
	movl	global(%rip), %eax
	popq	%rbp
	ret
