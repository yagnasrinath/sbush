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

.extern irq_handler
.extern timer_handler

.global x86_64_isr32
.global x86_64_isr33
.global irq_common

irq_common:
    PUSHA
    movq %rsp, %rdi
    callq irq_handler
    POPA
    add $0x10, %rsp
    sti
    iretq

x86_64_isr32:
    PUSHA
    movq %rsp, %rdi
    callq timer_handler
    POPA
    sti
    iretq
 
x86_64_isr33:
    cli
    pushq $0x0
    pushq $0x21
    jmp irq_common