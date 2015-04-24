#
# isr_common.s 
# 

.macro PUSHA
    pushq %rdi
    pushq %rax
    pushq %rbx
    pushq %rcx
    pushq %rdx
    pushq %rbp
    pushq %rsi
    pushq %r8
    pushq %r9
    pushq %r10
    pushq %r11
    pushq %r12
    pushq %r13
    pushq %r14
    pushq %r15
.endm

.macro POPA
    popq %r15
    popq %r14
    popq %r13
    popq %r12
    popq %r11
    popq %r10
    popq %r9
    popq %r8
    popq %rsi
    popq %rbp
    popq %rdx
    popq %rcx
    popq %rbx
    popq %rax
    popq %rdi
.endm

.text

.extern isr_handler
.extern handle_syscall

.global isr0
.global isr6
.global isr8
.global isr11
.global isr12
.global isr10
.global isr13
.global isr14
.global isr80
.global isr_common

isr0:
    cli
    pushq $0
    pushq $0
    jmp isr_common


isr6:
    cli
    pushq $0
    pushq $6
    jmp isr_common
    
isr8:
    cli
    pushq $8
    jmp isr_common
       
isr10:
    cli
    pushq $10
    jmp isr_common


isr11:
	cli
    pushq $11
    jmp isr_common

isr12:
	cli
    pushq $12
    jmp isr_common
        
isr13:
    cli
    pushq $13
    jmp isr_common

isr14:
    cli
    pushq $14
    jmp isr_common

isr80:
    cli
    pushq $0
    pushq $128
    callq handle_syscall
	iretq

isr_common:
	PUSHA   
    callq isr_handler
    POPA
    add $0x10, %rsp
    iretq
