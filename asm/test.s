.text
	.global _start
_start:
	# Allocate space
	sub $256, %rsp
	# Initalise pointer
	mov %rsp, %rbx

	movb $65, (%rbx)
	add $1, %rbx
	movb $26, (%rbx)

	mov $1, %rax # sys_write
	mov $1, %rdi # stdout
	mov $1, %rdx # 1 byte
.1:
	sub $1, %rbx
	# write
	mov %rbx, %rsi
	syscall

	addb $1, (%rbx)
	add $1, %rbx
	subb $1, (%rbx)
	cmpb $0, (%rbx)
	jne .1

	# exit
    mov $60, %rax
	mov $0, %rdi
    syscall
