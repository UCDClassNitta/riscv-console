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
    ret
    .cfi_endproc

.section .text, "ax"
.global RVCInitialize, RVCThreadCreate, RVCThreadDelete, RVCThreadActivate, RVCThreadTerminate 
.global RVCThreadWait, RVCThreadID, RVCThreadState, RVCThreadSleep, RVCTickMS, RVCTickCount
.global RVCWriteText, RVCReadController

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
    .end
