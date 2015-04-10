.global _set_cr3
.global _set_rsp

_set_cr3:
    movq %rdi, %rax
    movq %rax, %cr3
    retq
    