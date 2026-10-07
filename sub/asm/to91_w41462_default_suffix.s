	.include "asm/macros.inc"
	.include "global.inc"

	.extern BBP_Write
	.extern FLASH_DirectRead
	.extern FLASH_Read
	.extern RF_Write
	.extern RND_Init
	.extern WSetActiveZoneTime
	.extern WSetAuthAlgo
	.extern WSetBcSsidResponse
	.extern WSetBeaconLostThreshold
	.extern WSetBeaconPeriod
	.extern WSetBeaconSendRecvIndicate
	.extern WSetBeaconType
	.extern WSetCCA_ED
	.extern WSetDiversity
	.extern WSetEnableChannel
	.extern WSetFrameLifeTime
	.extern WSetMacAdrs
	.extern WSetMainAntenna
	.extern WSetMode
	.extern WSetNullKeyMode
	.extern WSetPreambleType
	.extern WSetRate
	.extern WSetRateSet
	.extern WSetRetryLimit
	.extern WSetSsidMask
	.extern WSetWepKey
	.extern WSetWepKeyId
	.extern WSetWepMode
	.extern c_RateSet$4351
	.extern def_SsidMask
	.extern def_WepKey

.text

	arm_func_start WSetChannel
WSetChannel: ; 0x0600A0A4
	stmdb sp!, {r3, r4, r5, r6, r7, r8, sb, sl, fp, lr}
	sub sp, sp, #8
	mov sl, r0
	ldr r0, _0600A320 ; =0x0380FFF4
	cmp r1, #0
	ldr r0, [r0]
	ldrne sb, _0600A324 ; =FLASH_DirectRead
	add r0, r0, #0x300
	ldrh r0, [r0, #0x2c]
	ldreq sb, _0600A328 ; =FLASH_Read
	mov r2, #1
	tst r0, r2, lsl sl
	moveq r0, #5
	beq _0600A314
	ldr r1, _0600A32C ; =0x04808040
	add r0, r2, #0x8000
	ldrh r6, [r1]
	strh r0, [r1]
	sub r3, r1, #4
	add r1, r1, #0x1d4
_0600A0F4:
	ldrh r2, [r3]
	ldrh r0, [r1]
	mov r2, r2, asr #8
	cmp r2, #2
	bne _0600A0F4
	cmp r0, #0
	cmpne r0, #9
	bne _0600A0F4
	ldr r5, _0600A320 ; =0x0380FFF4
	ldr r0, [r5]
	add r0, r0, #0x300
	strh sl, [r0, #0xbe]
	ldr r0, [r5]
	add r0, r0, #0x500
	ldrh r1, [r0, #0xf8]
	cmp r1, #2
	beq _0600A148
	cmp r1, #3
	beq _0600A218
	cmp r1, #5
	bne _0600A300
_0600A148:
	sub r4, sl, #1
	mov r0, #6
	mul r5, r4, r0
	mov r0, #0
	str r0, [sp, #4]
	add r2, sp, #4
	add r0, r5, #0xf2
	mov r1, #3
	mov lr, pc
	bx sb
	ldr r0, [sp, #4]
	bl RF_Write
	add r2, sp, #4
	add r0, r5, #0xf5
	mov r1, #3
	mov lr, pc
	bx sb
	ldr r0, [sp, #4]
	bl RF_Write
	mov r1, #0
	ldr r0, _0600A320 ; =0x0380FFF4
	str r1, [sp, #4]
	ldr r0, [r0]
	ldr r0, [r0, #0x604]
	tst r0, #0x10000
	beq _0600A1F0
	tst r0, #0x8000
	bne _0600A300
	add r2, sp, #4
	add r0, r4, #0x154
	mov r1, #1
	mov lr, pc
	bx sb
	ldr r1, _0600A320 ; =0x0380FFF4
	ldr r0, [sp, #4]
	ldr r1, [r1]
	mov r0, r0, lsl #0x1b
	ldr r1, [r1, #0x604]
	orr r0, r1, r0, lsr #17
	str r0, [sp, #4]
	bl RF_Write
	b _0600A300
_0600A1F0:
	add r0, r4, #0x46
	add r2, sp, #4
	add r0, r0, #0x100
	mov r1, #1
	mov lr, pc
	bx sb
	ldr r1, [sp, #4]
	mov r0, #0x1e
	bl BBP_Write
	b _0600A300
_0600A218:
	ldrh r0, [r0, #0xfc]
	mov r8, #0
	add r7, r0, #0xcf
	mov r4, r8
	mov fp, #1
	b _0600A274
_0600A230:
	mov r0, r7
	mov r1, fp
	add r2, sp, #0
	str r4, [sp, #4]
	str r4, [sp]
	mov lr, pc
	bx sb
	add r0, r7, sl
	mov r1, #1
	add r2, sp, #4
	mov lr, pc
	bx sb
	ldr r0, [sp]
	ldr r1, [sp, #4]
	bl BBP_Write
	add r7, r7, #0xf
	add r8, r8, #1
_0600A274:
	ldr r0, [r5]
	add r0, r0, #0x600
	ldrh r0, [r0]
	cmp r8, r0
	blo _0600A230
	mov r8, #0
	ldr r4, _0600A320 ; =0x0380FFF4
	mov r5, r8
	mov fp, #1
	b _0600A2EC
_0600A29C:
	str r5, [sp, #4]
	mov r0, r7
	mov r1, fp
	add r2, sp, #4
	mov lr, pc
	bx sb
	ldr r1, [sp, #4]
	add r0, r7, sl
	mov r1, r1, lsl #8
	str r1, [sp, #4]
	mov r1, #1
	add r2, sp, #4
	mov lr, pc
	bx sb
	ldr r0, [sp, #4]
	orr r0, r0, #0x50000
	str r0, [sp, #4]
	bl RF_Write
	add r7, r7, #0xf
	add r8, r8, #1
_0600A2EC:
	ldr r0, [r4]
	add r0, r0, #0x500
	ldrh r0, [r0, #0xfe]
	cmp r8, r0
	blo _0600A29C
_0600A300:
	ldr r1, _0600A32C ; =0x04808040
	mov r0, #3
	strh r6, [r1]
	strh r0, [r1, #8]
	mov r0, #0
_0600A314:
	add sp, sp, #8
	ldmia sp!, {r3, r4, r5, r6, r7, r8, sb, sl, fp, lr}
	bx lr
	.align 2, 0
_0600A320: .word 0x0380FFF4
_0600A324: .word FLASH_DirectRead
_0600A328: .word FLASH_Read
_0600A32C: .word 0x04808040
	arm_func_end WSetChannel
