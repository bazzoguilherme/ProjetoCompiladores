	.file	"e5.c"
	.text
	.comm	simples1,4,4
	.comm	simples2,4,4
	.comm	if_test,4,4
	.comm	while_test,4,4
	.comm	for_test,4,4
	.comm	ternario_test,4,4
	.comm	fun1,4,4
	.comm	fun_add,4,4
	.comm	fun_mult,4,4
	.comm	fun3param1,4,4
	.comm	fib0,4,4
	.comm	fib1,4,4
	.comm	fib2,4,4
	.comm	fib3,4,4
	.comm	fib4,4,4
	.comm	fib5,4,4
	.comm	fib6,4,4
	.comm	fib7,4,4
	.comm	fib8,4,4
	.globl	inc
	.type	inc, @function
inc:
.LFB0:
	endbr64
	pushq	%rbp
	movq	%rsp, %rbp
	movl	%edi, -4(%rbp)
	movl	-4(%rbp), %eax
	addl	$1, %eax
	popq	%rbp
	ret
.LFE0:
	.size	inc, .-inc
	.globl	add
	.type	add, @function
add:
.LFB1:
	endbr64
	pushq	%rbp
	movq	%rsp, %rbp
	movl	%edi, -4(%rbp)
	movl	%esi, -8(%rbp)
	movl	-4(%rbp), %edx
	movl	-8(%rbp), %eax
	addl	%edx, %eax
	popq	%rbp
	ret
.LFE1:
	.size	add, .-add
	.globl	mult
	.type	mult, @function
mult:
.LFB2:
	endbr64
	pushq	%rbp
	movq	%rsp, %rbp
	movl	%edi, -4(%rbp)
	movl	%esi, -8(%rbp)
	movl	-4(%rbp), %eax
	imull	-8(%rbp), %eax
	popq	%rbp
	ret
.LFE2:
	.size	mult, .-mult
	.globl	multi_add
	.type	multi_add, @function
multi_add:
.LFB3:
	endbr64
	pushq	%rbp
	movq	%rsp, %rbp
	movl	%edi, -4(%rbp)
	movl	%esi, -8(%rbp)
	movl	%edx, -12(%rbp)
	movl	-4(%rbp), %edx
	movl	-8(%rbp), %eax
	addl	%eax, %edx
	movl	-12(%rbp), %eax
	addl	%edx, %eax
	popq	%rbp
	ret
.LFE3:
	.size	multi_add, .-multi_add
	.globl	fib
	.type	fib, @function
fib:
.LFB4:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movl	%edi, -20(%rbp)
	cmpl	$0, -20(%rbp)
	jne	.L10
	movl	$0, %eax
	jmp	.L11
.L10:
	cmpl	$1, -20(%rbp)
	jne	.L12
	movl	$1, %eax
	jmp	.L11
.L12:
	movl	-20(%rbp), %eax
	subl	$1, %eax
	movl	%eax, %edi
	call	fib
	movl	%eax, -8(%rbp)
	movl	-20(%rbp), %eax
	subl	$2, %eax
	movl	%eax, %edi
	call	fib
	movl	%eax, -4(%rbp)
	movl	-8(%rbp), %edx
	movl	-4(%rbp), %eax
	addl	%edx, %eax
.L11:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE4:
	.size	fib, .-fib
	.globl	main
	.type	main, @function
main:
.LFB5:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	$0, while_test(%rip)
	movl	$10, -4(%rbp)
	jmp	.L14
.L15:
	subl	$1, -4(%rbp)
	movl	while_test(%rip), %eax
	addl	$1, %eax
	movl	%eax, while_test(%rip)
.L14:
	cmpl	$0, -4(%rbp)
	jg	.L15
	movl	$0, %eax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE5:
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
