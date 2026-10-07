.public OSi_AlarmHandler
.extern OSi_ArrangeTimer
.text
.align 2
.arm
.type OSi_AlarmHandler, @function
OSi_AlarmHandler:
    stmdb sp!, {r0, lr}
    bl OSi_ArrangeTimer
    ldmia sp!, {r0, lr}
    bx lr
.size OSi_AlarmHandler, .-OSi_AlarmHandler
