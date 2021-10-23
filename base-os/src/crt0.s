.section .init, "ax"
.global _start
_start:
    .cfi_startproc
    .cfi_undefined ra
    .option push
    .option norelax
    la gp, __global_pointer$
    .option pop
    la sp, __stack_top
    add s0, sp, zero
    la  a5, _interrupt_handler
    csrw mtvec, a5
    jal ra, init
    nop
    jal zero, main
    .cfi_endproc
    .end
