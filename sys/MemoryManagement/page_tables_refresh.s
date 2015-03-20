.global _set_k_ptable_crm3


_set_k_ptable_crm3:
    movq %rdi, %rax
    movq %rax, %cr3
    retq
