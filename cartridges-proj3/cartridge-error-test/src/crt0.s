.section .init, "ax"
.global _start
_start:
    .cfi_startproc
    .cfi_undefined ra
    .option push
    .option norelax
    la      gp, __global_pointer$
    .option pop
    la      sp, __stack_top
    addi	sp, sp, -4
    sw      ra, 0(sp)
    add     s0, sp, zero
    jal     ra, init
    nop
    mv      a0, gp
    jal     ra, RVCInitialize
    nop
    jal     ra, main
    lw      ra,0(sp)
    addi	sp, sp, 4
    ret
    .cfi_endproc

.section .text, "ax"
.global RVCInitialize, RVCThreadCreate, RVCThreadDelete, RVCThreadActivate, RVCThreadTerminate 
.global RVCThreadWait, RVCThreadID, RVCThreadState, RVCThreadSleep, RVCTickMS, RVCTickCount
.global RVCWriteText, RVCReadController
.global RVCMemoryPoolCreate, RVCMemoryPoolDelete, RVCMemoryPoolQuery, RVCMemoryPoolAllocate, RVCMemoryPoolDeallocate
.global RVCMutexCreate, RVCMutexDelete, RVCMutexQuery, RVCMutexAcquire, RVCMutexRelease

RVCInitialize:
    li      a5,0
    ecall
    ret
RVCThreadCreate:
    li      a5,1
    ecall
    ret
RVCThreadDelete:
    li      a5,2
    ecall
    ret
RVCThreadActivate:
    li      a5,3
    ecall
    ret
RVCThreadTerminate:
    li      a5,4
    ecall
    ret
RVCThreadWait:
    li      a5,5
    ecall
    ret
RVCThreadID:
    li      a5,6
    ecall
    ret
RVCThreadState:
    li      a5,7
    ecall
    ret
RVCThreadSleep:
    li      a5,8
    ecall
    ret
RVCTickMS:
    li      a5,9
    ecall
    ret
RVCTickCount:
    li      a5,10
    ecall
    ret
RVCWriteText:
    li      a5,11
    ecall
    ret
RVCReadController:
    li      a5,12
    ecall
    ret
RVCMemoryPoolCreate:
    li      a5,13
    ecall
    ret
RVCMemoryPoolDelete:
    li      a5,14
    ecall
    ret
RVCMemoryPoolQuery:
    li      a5,15
    ecall
    ret
RVCMemoryPoolAllocate:
    li      a5,16
    ecall
    ret
RVCMemoryPoolDeallocate:
    li      a5,17
    ecall
    ret
RVCMutexCreate:
    li      a5,18
    ecall
    ret
RVCMutexDelete:
    li      a5,19
    ecall
    ret
RVCMutexQuery:
    li      a5,20
    ecall
    ret
RVCMutexAcquire:
    li      a5,21
    ecall
    ret
RVCMutexRelease:
    li      a5,22
    ecall
    ret
    .end
