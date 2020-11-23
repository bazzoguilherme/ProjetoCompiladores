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
	.globl	foo
	.type	foo, @function
foo:
.LFB1:
	endbr64
	pushq	%rbp
	movq	%rsp, %rbp
	movl	%edi, -4(%rbp)
	movl	%esi, -8(%rbp)
	movl	%edx, -12(%rbp)
	movl	%ecx, -16(%rbp)
	movl	%r8d, -20(%rbp)
	movl	%r9d, -24(%rbp)
	movl	-4(%rbp), %edx
	movl	16(%rbp), %eax
	addl	%eax, %edx
	movl	24(%rbp), %eax
	addl	%eax, %edx
	movl	32(%rbp), %eax
	addl	%edx, %eax
	popq	%rbp
	ret
.LFE1:
	.size	foo, .-foo
	.globl	main
	.type	main, @function
main:
.LFB2:
	endbr64
	pushq	%rbp
	movq	%rsp, %rbp
	pushq	$9
	pushq	$8
	pushq	$7
	movl	$6, %r9d
	movl	$5, %r8d
	movl	$4, %ecx
	movl	$3, %edx
	movl	$2, %esi
	movl	$1, %edi
	call	foo
	addq	$24, %rsp
	movl	$0, %eax
	leave
	ret
.LFE2:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 9.3.0-10ubuntu2) 9.3.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	 1f - 0f
	.long	 4f - 1f
	.long	 5
0:
	.string	 "GNU"
1:
	.align 8
	.long	 0xc0000002
	.long	 3f - 2f
2:
	.long	 0x3
3:
	.align 8
4:
