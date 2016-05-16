	.file	"sub_array.c"
	.intel_syntax noprefix
	.text
	.globl	check_negative
	.type	check_negative, @function
check_negative:
.LFB23:
	.cfi_startproc
	test	esi, esi
	jle	.L5
	cmp	DWORD PTR [rdi], 0
	jg	.L6
	lea	rax, [rdi+4]
	lea	edx, [rsi-1]
	lea	rdx, [rdi+4+rdx*4]
	jmp	.L3
.L4:
	add	rax, 4
	cmp	DWORD PTR [rax-4], 0
	jg	.L7
.L3:
	cmp	rax, rdx
	jne	.L4
	mov	eax, 1
	ret
.L5:
	mov	eax, 1
	ret
.L6:
	mov	eax, 0
	ret
.L7:
	mov	eax, 0
	ret
	.cfi_endproc
.LFE23:
	.size	check_negative, .-check_negative
	.globl	find_max_index
	.type	find_max_index, @function
find_max_index:
.LFB24:
	.cfi_startproc
	cmp	esi, 1
	jle	.L12
	lea	rcx, [rdi+4]
	mov	eax, 0
	mov	edx, 1
.L11:
	movsx	r8, eax
	mov	r8d, DWORD PTR [rdi+r8*4]
	cmp	r8d, DWORD PTR [rcx]
	cmovl	eax, edx
	add	edx, 1
	add	rcx, 4
	cmp	esi, edx
	jne	.L11
	rep ret
.L12:
	mov	eax, 0
	ret
	.cfi_endproc
.LFE24:
	.size	find_max_index, .-find_max_index
	.globl	find_max_sub
	.type	find_max_sub, @function
find_max_sub:
.LFB25:
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	push	rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	mov	rbx, rdi
	mov	ebp, esi
	call	check_negative
	cmp	eax, 1
	jne	.L15
	mov	esi, ebp
	mov	rdi, rbx
	call	find_max_index
	mov	eax, eax
	mov	rdx, rax
	sal	rdx, 32
	or	rax, rdx
	jmp	.L16
.L15:
	mov	r10d, DWORD PTR [rbx]
	test	r10d, r10d
	mov	r8d, 0
	cmovle	r8d, r10d
	cmp	ebp, 1
	jle	.L21
	lea	rdi, [rbx+4]
	mov	esi, ebp
	mov	r11d, r10d
	mov	edx, 1
	mov	ebp, 0
	mov	eax, 0
.L20:
	mov	ebx, edx
	mov	ecx, r10d
	add	ecx, DWORD PTR [rdi]
	mov	r10d, ecx
	mov	r9d, ecx
	sub	r9d, r8d
	cmp	r9d, r11d
	jle	.L18
	mov	eax, edx
	mov	r11d, r9d
.L18:
	cmp	ecx, r8d
	jge	.L19
	mov	ebp, ebx
	mov	r8d, ecx
.L19:
	add	edx, 1
	add	rdi, 4
	cmp	edx, esi
	jne	.L20
	jmp	.L17
.L21:
	mov	ebp, 0
	mov	eax, 0
.L17:
	sal	rax, 32
	mov	ebp, ebp
	or	rax, rbp
.L16:
	pop	rbx
	.cfi_def_cfa_offset 16
	pop	rbp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE25:
	.size	find_max_sub, .-find_max_sub
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"%d %d\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB26:
	.cfi_startproc
	sub	rsp, 40
	.cfi_def_cfa_offset 48
	mov	rax, QWORD PTR fs:40
	mov	QWORD PTR [rsp+24], rax
	xor	eax, eax
	mov	DWORD PTR [rsp], 4
	mov	DWORD PTR [rsp+4], -1
	mov	DWORD PTR [rsp+8], 3
	mov	DWORD PTR [rsp+12], -5
	mov	esi, 4
	mov	rdi, rsp
	call	find_max_sub
	mov	rcx, rax
	sar	rcx, 32
	mov	edx, eax
	mov	esi, OFFSET FLAT:.LC0
	mov	edi, 1
	mov	eax, 0
	call	__printf_chk
	mov	rdx, QWORD PTR [rsp+24]
	xor	rdx, QWORD PTR fs:40
	je	.L25
	call	__stack_chk_fail
.L25:
	mov	eax, 0
	add	rsp, 40
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE26:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 5.3.1-14ubuntu2) 5.3.1 20160413"
	.section	.note.GNU-stack,"",@progbits
