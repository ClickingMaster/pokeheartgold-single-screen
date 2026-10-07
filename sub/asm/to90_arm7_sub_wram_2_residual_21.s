	.include "asm/macros.inc"
	.include "global.inc"


.text

	arm_func_start RTCi_GpioStart
RTCi_GpioStart: ; 0x038057BC
	mov ip, #0x4000000
	add ip, ip, #0x138
	ldrh r0, [ip]
	bic r0, r0, #0x77
	orr r0, r0, #0x72
	strh r0, [ip]
	mov r3, #2
_038057D8:
	subs r3, r3, #1
	bne _038057D8
	bic r0, r0, #4
	orr r0, r0, #4
	strh r0, [ip]
	mov r3, #2
_038057F0:
	subs r3, r3, #1
	bne _038057F0
	bx lr
	arm_func_end RTCi_GpioStart

	arm_func_start RTCi_GpioEnd
RTCi_GpioEnd: ; 0x038057FC
	mov ip, #0x4000000
	add ip, ip, #0x138
	mov r3, #2
_03805808:
	subs r3, r3, #1
	bne _03805808
	ldrh r0, [ip]
	bic r0, r0, #4
	orr r0, r0, #0
	strh r0, [ip]
	mov r3, #2
_03805824:
	subs r3, r3, #1
	bne _03805824
	bx lr
	arm_func_end RTCi_GpioEnd

	arm_func_start RTCi_GpioSendCommand
RTCi_GpioSendCommand: ; 0x03805830
	mov ip, #0x4000000
	add ip, ip, #0x138
	orr r1, r0, r1
	ldrh r0, [ip]
	bic r0, r0, #0x77
	orr r0, r0, #0x74
	mov r2, #0
_0380584C:
	bic r0, r0, #3
	orr r0, r0, #0
	mov r3, #1
	tst r3, r1, lsr r2
	movne r3, #1
	moveq r3, #0
	orr r0, r0, r3
	strh r0, [ip]
	mov r3, #9
_03805870:
	subs r3, r3, #1
	bne _03805870
	bic r0, r0, #2
	orr r0, r0, #2
	strh r0, [ip]
	mov r3, #9
_03805888:
	subs r3, r3, #1
	bne _03805888
	add r2, r2, #1
	cmp r2, #8
	bne _0380584C
	bx lr
	arm_func_end RTCi_GpioSendCommand

	arm_func_start RTCi_GpioSendData
RTCi_GpioSendData: ; 0x038058A0
	mov ip, #0x4000000
	add ip, ip, #0x138
_038058A8:
	stmdb sp!, {r0, r1}
	tst r0, #1
	ldreqh r1, [r0]
	ldrneh r1, [r0, #-1]
	movne r1, r1, lsr #8
	ldrh r0, [ip]
	bic r0, r0, #0x77
	orr r0, r0, #0x74
	mov r2, #0
_038058CC:
	bic r0, r0, #3
	orr r0, r0, #0
	mov r3, #1
	tst r3, r1, lsr r2
	movne r3, #1
	moveq r3, #0
	orr r0, r0, r3
	strh r0, [ip]
	mov r3, #9
_038058F0:
	subs r3, r3, #1
	bne _038058F0
	bic r0, r0, #2
	orr r0, r0, #2
	strh r0, [ip]
	mov r3, #9
_03805908:
	subs r3, r3, #1
	bne _03805908
	add r2, r2, #1
	cmp r2, #8
	bne _038058CC
	ldmia sp!, {r0, r1}
	add r0, r0, #1
	subs r1, r1, #1
	bne _038058A8
	bx lr
	arm_func_end RTCi_GpioSendData

	arm_func_start RTCi_GpioReceiveData
RTCi_GpioReceiveData: ; 0x03805930
	mov ip, #0x4000000
	add ip, ip, #0x138
_03805938:
	stmdb sp!, {r0, r1}
	ldrh r0, [ip]
	bic r0, r0, #0x77
	orr r0, r0, #0x64
	mov r2, #0
	mov r1, #0
_03805950:
	bic r0, r0, #3
	orr r0, r0, #0
	strh r0, [ip]
	mov r3, #9
_03805960:
	subs r3, r3, #1
	bne _03805960
	ldrh r0, [ip]
	and r3, r0, #1
	cmp r3, #1
	moveq r3, #0x80
	movne r3, #0
	orr r2, r3, r2, lsr #1
	bic r0, r0, #2
	orr r0, r0, #2
	strh r0, [ip]
	mov r3, #9
_03805990:
	subs r3, r3, #1
	bne _03805990
	add r1, r1, #1
	cmp r1, #8
	bne _03805950
	ldmia sp!, {r0, r1}
	tst r0, #1
	beq _038059C8
	ldrh r3, [r0, #-1]
	bic r3, r3, #0xff00
	mov r2, r2, lsl #8
	orr r3, r2, r3
	strh r3, [r0, #-1]
	b _038059D8
_038059C8:
	ldrh r3, [r0]
	bic r3, r3, #0xff
	orr r3, r3, r2
	strh r3, [r0]
_038059D8:
	add r0, r0, #1
	subs r1, r1, #1
	bne _03805938
	bx lr
	arm_func_end RTCi_GpioReceiveData

	; wvr.s(.wram)
