	.include "asm/macros.inc"
	.include "global.inc"

	.extern EXIi_SelectRcnt
	.extern RTCi_GpioEnd
	.extern RTCi_GpioSendCommand
	.extern RTCi_GpioSendData
	.extern RTCi_GpioStart
	.extern RtcChangeAlarmFormat12to24
	.extern RtcChangeAlarmFormat24to12
	.extern RtcGpioTransfer

.text

	arm_func_start RTC_SetHourFormat
RTC_SetHourFormat: ; 0x03804F1C
	stmdb sp!, {r4, lr}
	sub sp, sp, #8
	mov r4, r0
	and r4, r4, #1
	cmp r4, #1
	bne _03805034
	mov r0, #0x8000
	bl EXIi_SelectRcnt
	add r2, sp, #0
	mov r0, #0x86
	mov r1, #0
	mov r3, #1
	bl RtcGpioTransfer
	ldrh r0, [sp]
	mov r1, r0, lsl #0x1e
	mov r1, r1, lsr #0x1f
	cmp r1, r4
	beq _03805034
	bic r1, r0, #2
	mov r0, r4, lsl #0x1f
	orr r1, r1, r0, lsr #30
	mov r0, #0x8000
	strh r1, [sp]
	bl EXIi_SelectRcnt
	bl RTCi_GpioStart
	mov r0, #6
	mov r1, #0
	bl RTCi_GpioSendCommand
	add r0, sp, #0
	mov r1, #1
	bl RTCi_GpioSendData
	bl RTCi_GpioEnd
	mov r0, #0x86
	mov r1, #0x10
	add r2, sp, #4
	mov r3, #3
	bl RtcGpioTransfer
	cmp r4, #0
	add r0, sp, #4
	bne _03804FC4
	bl RtcChangeAlarmFormat24to12
	b _03804FC8
_03804FC4:
	bl RtcChangeAlarmFormat12to24
_03804FC8:
	bl RTCi_GpioStart
	mov r0, #6
	mov r1, #0x10
	bl RTCi_GpioSendCommand
	add r0, sp, #4
	mov r1, #3
	bl RTCi_GpioSendData
	bl RTCi_GpioEnd
	mov r0, #0x86
	mov r1, #0x50
	add r2, sp, #4
	mov r3, #3
	bl RtcGpioTransfer
	cmp r4, #0
	add r0, sp, #4
	bne _03805010
	bl RtcChangeAlarmFormat24to12
	b _03805014
_03805010:
	bl RtcChangeAlarmFormat12to24
_03805014:
	bl RTCi_GpioStart
	mov r0, #6
	mov r1, #0x50
	bl RTCi_GpioSendCommand
	add r0, sp, #4
	mov r1, #3
	bl RTCi_GpioSendData
	bl RTCi_GpioEnd
_03805034:
	add sp, sp, #8
	ldmia sp!, {r4, lr}
	bx lr
	arm_func_end RTC_SetHourFormat
