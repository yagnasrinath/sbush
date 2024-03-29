#
# gdt.s
#
#  Created on: Dec 29, 2010
#      Author: cds
#

.text

######
# load a new GDT
#  parameter 1: address of gdtr
#  parameter 2: new code descriptor offset
#  parameter 3: new data descriptor offset
.global _x86_64_asm_lgdt
_x86_64_asm_lgdt:

	lgdt (%rdi)

	pushq %rsi                  # push code selector
	movabsq $.done, %r10
	pushq %r10                  # push return address
	lretq                       # far-return to new cs descriptor ( the retq below )
.done:
	movq %rdx, %es
	movq %rdx, %fs
	movq %rdx, %gs
	movq %rdx, %ds
	movq %rdx, %ss
	retq

#Load new tss
.global _X86_64_asm_ltss
_X86_64_asm_ltss:
	mov $0x2b, %ax
	ltr %ax
	retq 	

.global _switch_to_ring3
_switch_to_ring3:
     mov $0x23, %ax
     mov %ax, %ds
     mov %ax, %es 
     mov %ax, %fs 
     mov %ax, %gs 	
	
