	.include "asm/macros.inc"
	.include "global.inc"

	.extern InitApList
	.extern InitCAM
	.extern InitRxCtrl
	.extern InitTxCtrl
	.extern RND_Init
	.extern StartBeaconFrame
	.extern WDisableTmpttPowerSave
	.extern WSetPowerState
	.extern WSetStaState
	.extern WStop
	.extern WaitLoop_Rxpe
	.extern _ll_udiv

.text

	arm_func_start WStart
WStart: ; 0x0600AE14
	stmdb sp!, {r3, r4, r5, r6, r7, lr}
	sub sp, sp, #8
	ldr r0, _0600B180 ; =0x0380FFF4
	ldr r0, [r0]
	add r6, r0, #0x344
	add r4, r0, #0x31c
	bl WStop
	ldr r1, _0600B184 ; =0x04808044
	ldrh r2, [r1]
	ldrh r0, [r1]
	ldrh r1, [r1]
	add r0, r2, r0, lsl #8
	bl RND_Init
	mov r0, #1
	strh r0, [r6, #0x7c]
	ldrh r0, [r4, #0x1e]
	ldr r2, _0600B188 ; =0x04808032
	mov r0, r0, lsl #0x1d
	mov r0, r0, lsr #0x1f
	cmp r0, #1
	ldreqh r0, [r6, #0x7c]
	ldr r1, _0600B18C ; =0x04808134
	orreq r0, r0, #0x20
	streqh r0, [r6, #0x7c]
	ldrh r0, [r4, #0x18]
	mov r4, #0
	cmp r0, #0
	ldrneh r0, [r6, #0x7c]
	mov r3, #0x8000
	orrne r0, r0, #0x10
	strneh r0, [r6, #0x7c]
	strh r4, [r6, #0x12]
	ldr r0, _0600B190 ; =0x0000FFFF
	strh r3, [r2]
	strh r0, [r1]
	strh r4, [r2, #-8]
	sub r0, r1, #0x10c
	strh r4, [r0]
	mov r0, #0xf
	strh r0, [r1, #-0xfc]
	bl InitCAM
	bl InitApList
	bl InitTxCtrl
	bl InitRxCtrl
	ldr r2, _0600B194 ; =0x04808030
	mov r1, #0x8000
	strh r1, [r2]
	ldr r0, _0600B190 ; =0x0000FFFF
	ldr r1, _0600B198 ; =0x048081AE
	strh r0, [r2, #-0x20]
	sub r2, r0, #0xe000
	ldr r0, _0600B180 ; =0x0380FFF4
	strh r2, [r1]
	ldr r0, [r0]
	add r0, r0, #0x600
	ldrh r0, [r0, #0x90]
	tst r0, #8
	movne r0, #0x400
	strneh r0, [r1, #-4]
	moveq r0, r4
	streqh r0, [r1, #-4]
	ldr r2, _0600B19C ; =0x04808008
	mov r3, #0
	strh r3, [r2]
	strh r3, [r2, #2]
	ldrh r0, [r6, #0xc]
	cmp r0, #4
	addls pc, pc, r0, lsl #2
	b _0600B144
_0600AF28: ; jump table
	b _0600AF3C ; case 0
	b _0600AF6C ; case 1
	b _0600B050 ; case 2
	b _0600B0D0 ; case 3
	b _0600B128 ; case 4
_0600AF3C:
	ldr r0, _0600B190 ; =0x0000FFFF
	mov r1, #0x3f
	strh r1, [r2, #0xa]
	strh r0, [r2, #0xc8]
	mov r0, #8
	strh r0, [r2, #0xd8]
	strh r3, [r2]
	strh r3, [r2, #2]
	strh r3, [r2, #0xe0]
	mov r0, #1
	strh r0, [r2, #-4]
	b _0600B144
_0600AF6C:
	ldr r3, _0600B1A0 ; =0x0000703F
	ldr r0, _0600B1A4 ; =0x00001FFF
	ldr r1, _0600B198 ; =0x048081AE
	strh r3, [r2, #0xa]
	strh r0, [r1]
	rsb r0, r0, #0x2300
	strh r0, [r1, #-0xde]
	mov r0, #0xd
	strh r0, [r1, #-0xce]
	mov r0, #0xe000
	strh r0, [r2]
	mov r0, #1
	strh r0, [r2, #-4]
	ldrh r0, [r1, #-0xb6]
	add r7, sp, #0
	strh r0, [r7]
	ldrh r0, [r1, #-0xb4]
	mov r4, #0
	strh r0, [r7, #2]
	ldrh r2, [r1, #-0xb2]
	ldr r0, [sp]
	strh r2, [r7, #4]
	ldrh r1, [r1, #-0xb0]
	mov r3, r4
	strh r1, [r7, #6]
	ldrh r2, [r6, #0x6e]
	ldr r1, [sp, #4]
	mov r5, r2, lsl #0xa
	mov r2, r5
	bl __VENEER__ll_udiv
	adds r3, r0, #1
	umull r2, r0, r3, r5
	mla r0, r3, r4, r0
	adc r1, r1, #0
	mla r0, r1, r5, r0
	str r0, [sp, #4]
	ldrh r0, [r7, #6]
	ldr r3, _0600B1A8 ; =0x048080F6
	str r2, [sp]
	strh r0, [r3]
	ldrh r0, [r7, #4]
	mov r1, #1
	strh r0, [r3, #-2]
	ldrh r2, [r7, #2]
	mov r0, #0x40
	strh r2, [r3, #-4]
	ldrh r2, [r7]
	orr r2, r2, #1
	strh r2, [r3, #-6]
	strh r1, [r3, #-0xe]
	strh r1, [r3, #-0xc]
	bl WSetStaState
	bl StartBeaconFrame
	ldr r0, _0600B1AC ; =0x048080AE
	mov r1, #2
	strh r1, [r0]
	b _0600B144
_0600B050:
	ldr r1, _0600B1B0 ; =0x0000E0BF
	ldr r0, _0600B180 ; =0x0380FFF4
	strh r1, [r2, #0xa]
	ldr r0, [r0]
	add r0, r0, #0x600
	ldrh r0, [r0, #0x90]
	tst r0, #0x20
	beq _0600B08C
	ldrh r0, [r2, #0xa]
	ldr r1, _0600B1B4 ; =0x048081AA
	orr r0, r0, #0x40
	strh r0, [r2, #0xa]
	ldrh r0, [r1]
	orr r0, r0, #0x68
	strh r0, [r1]
_0600B08C:
	ldrh r0, [r6, #0x64]
	tst r0, #1
	ldr r0, _0600B1B8 ; =0x048080D0
	ldrne r1, _0600B1BC ; =0x00000581
	strneh r1, [r0]
	ldreq r1, _0600B1C0 ; =0x00000181
	streqh r1, [r0]
	ldr r2, _0600B1C4 ; =0x048080E0
	mov r0, #0xb
	strh r0, [r2]
	mov r1, #1
	strh r1, [r2, #-0xdc]
	strh r1, [r2, #8]
	mov r0, #0x20
	strh r1, [r2, #0xa]
	bl WSetStaState
	b _0600B144
_0600B0D0:
	ldr r1, _0600B190 ; =0x0000FFFF
	strh r1, [r2, #8]
	sub r0, r1, #0x3fc0
	strh r0, [r2, #0xa]
	ldrh r0, [r6, #0x64]
	tst r0, #1
	rsbne r0, r1, #0x10400
	strneh r0, [r2, #0xc8]
	moveq r0, #1
	streqh r0, [r2, #0xc8]
	ldr r2, _0600B1C4 ; =0x048080E0
	mov r0, #0xb
	strh r0, [r2]
	mov r0, #1
	strh r0, [r2, #-0xdc]
	strh r0, [r2, #8]
	strh r0, [r2, #0xa]
	mov r1, #0
	mov r0, #0x20
	strh r1, [r2, #-0x98]
	bl WSetStaState
	b _0600B144
_0600B128:
	ldr r0, _0600B198 ; =0x048081AE
	strh r3, [r2, #0xa]
	strh r3, [r0]
	mov r1, #1
	mov r0, #0x20
	strh r1, [r2, #-4]
	bl WSetStaState
_0600B144:
	ldr r0, _0600B1C8 ; =0x04808048
	mov r1, #0
	strh r1, [r0]
	bl WDisableTmpttPowerSave
	ldr r1, _0600B1AC ; =0x048080AE
	mov r0, #2
	strh r0, [r1]
	ldrh r1, [r6, #0xe]
	cmp r1, #1
	bne _0600B170
	bl WSetPowerState
_0600B170:
	bl WaitLoop_Rxpe
	add sp, sp, #8
	ldmia sp!, {r3, r4, r5, r6, r7, lr}
	bx lr
	.align 2, 0
_0600B180: .word 0x0380FFF4
_0600B184: .word 0x04808044
_0600B188: .word 0x04808032
_0600B18C: .word 0x04808134
_0600B190: .word 0x0000FFFF
_0600B194: .word 0x04808030
_0600B198: .word 0x048081AE
_0600B19C: .word 0x04808008
_0600B1A0: .word 0x0000703F
_0600B1A4: .word 0x00001FFF
_0600B1A8: .word 0x048080F6
_0600B1AC: .word 0x048080AE
_0600B1B0: .word 0x0000E0BF
_0600B1B4: .word 0x048081AA
_0600B1B8: .word 0x048080D0
_0600B1BC: .word 0x00000581
_0600B1C0: .word 0x00000181
_0600B1C4: .word 0x048080E0
_0600B1C8: .word 0x04808048
	arm_func_end WStart

	arm_func_start __VENEER__ll_udiv
__VENEER__ll_udiv: ; 0x0600B1CC
	ldr pc, _0600B1D0 ; =_ll_udiv
	.align 2, 0
_0600B1D0: .word _ll_udiv
	arm_func_end __VENEER__ll_udiv
