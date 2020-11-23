	.file	"main.c"
	.text
	.comm	global,4
	.globl	main
	.type	main, @function
main:
	pushq	%rbp
	movq	%rsp, %rbp
	movl	$2, global(%rip)
	movl	global(%rip), %eax
	popq	%rbp
	ret
