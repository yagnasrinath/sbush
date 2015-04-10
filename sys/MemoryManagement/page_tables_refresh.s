.global _set_k_ptable_cr3


_set_k_ptable_cr3:
    movq %rdi, %rax
    movq %rax, %cr3
    retq
