	.include "asm/macros.inc"
	.include "overlay_87.inc"
	.include "global.inc"
	.extern ov87_021E7EF0
	.extern ov87_021E8394
	.public ov87_021E7A44
	.public ov87_021E7AF8
	.public ov87_021E7DF8

	.extern ov87_021E7BC4
	.extern ov87_021E7C50
	.extern ov87_021E7CE0
	.extern ov87_021E7D6C

	.text

thumb_func_start ov87_021E7A44
ov87_021E7A44: ; 0x021E7A44
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x10
	add r6, r0, #0
	mov r0, #2
	mvn r0, r0
	add r7, r1, #0
	str r2, [sp, #4]
	str r0, [sp, #8]
	str r0, [sp, #0xc]
_021E7A56:
	ldr r1, [sp, #4]
	ldr r0, [sp, #8]
	ldr r4, [sp, #0xc]
	add r5, r1, r0
_021E7A5E:
	add r1, r7, r4
	cmp r1, #0
	ble _021E7A7C
	mov r0, #1
	lsl r0, r0, #8
	cmp r1, r0
	bge _021E7A7C
	cmp r5, #0
	ble _021E7A7C
	cmp r5, #0xc0
	bge _021E7A7C
	add r0, r6, #0
	add r2, r5, #0
	bl ov87_021E7AB0
_021E7A7C:
	add r4, r4, #1
	cmp r4, #3
	blt _021E7A5E
	ldr r0, [sp, #8]
	add r0, r0, #1
	str r0, [sp, #8]
	cmp r0, #3
	blt _021E7A56
	mov r1, #0
	mov r3, #0xe3
	str r1, [sp]
	lsl r3, r3, #2
	ldr r2, [r6, r3]
	sub r3, r3, #4
	ldr r3, [r6, r3]
	ldr r0, [r6, #0x58]
	ldr r3, [r3, #0x10]
	bl BG_LoadCharTilesData
	ldr r0, [r6, #0x58]
	mov r1, #0
	bl BgCommitTilemapBufferToVram
	add sp, #0x10
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
	thumb_func_end ov87_021E7A44

	thumb_func_start ov87_021E7AB0
ov87_021E7AB0: ; 0x021E7AB0
	push {r3, r4, r5, r6}
	mov r3, #1
	mov r4, #0xff
	tst r3, r1
	bne _021E7ABE
	mov r3, #0xf
	b _021E7AC0
_021E7ABE:
	mov r3, #0xf0
_021E7AC0:
	eor r3, r4
	lsl r3, r3, #0x18
	lsr r4, r1, #3
	mov r6, #7
	lsr r5, r2, #3
	and r1, r6
	lsr r3, r3, #0x18
	lsl r4, r4, #5
	lsl r5, r5, #0xa
	lsr r1, r1, #1
	cmp r2, #8
	bhs _021E7ADC
	lsl r2, r2, #2
	b _021E7AE0
_021E7ADC:
	lsl r2, r2, #0x1d
	lsr r2, r2, #0x1b
_021E7AE0:
	mov r6, #0xe3
	lsl r6, r6, #2
	add r4, r5, r4
	add r1, r1, r4
	ldr r0, [r0, r6]
	add r2, r2, r1
	ldrb r1, [r0, r2]
	and r1, r3
	strb r1, [r0, r2]
	pop {r3, r4, r5, r6}
	bx lr
	.balign 4, 0
	thumb_func_end ov87_021E7AB0

	thumb_func_start ov87_021E7AF8
ov87_021E7AF8: ; 0x021E7AF8
	push {r4, r5, r6, r7, lr}
	sub sp, #0x14
	add r5, r0, #0
	mov r0, #0x20
	mov r1, #0x7a
	bl GF_CreateVramTransferManager
	bl ov87_021E7EF0
	bl NNS_G2dInitOamManagerModule
	mov r0, #0
	str r0, [sp]
	mov r1, #0x80
	str r1, [sp, #4]
	str r0, [sp, #8]
	mov r3, #0x20
	str r3, [sp, #0xc]
	mov r2, #0x7a
	str r2, [sp, #0x10]
	add r2, r0, #0
	bl OamManager_Create
	mov r0, #0x28
	add r1, r5, #4
	mov r2, #0x7a
	bl G2dRenderer_Init
	mov r2, #2
	str r0, [r5]
	add r0, r5, #4
	mov r1, #0
	lsl r2, r2, #0x14
	bl G2dRenderer_SetSubSurfaceCoords
	ldr r4, _021E7BC0 ; =ov87_021E8394
	mov r7, #0
	add r6, r5, #0
_021E7B44:
	ldrb r0, [r4]
	add r1, r7, #0
	mov r2, #0x7a
	bl Create2DGfxResObjMan
	mov r1, #0x4b
	lsl r1, r1, #2
	str r0, [r6, r1]
	add r7, r7, #1
	add r4, r4, #1
	add r6, r6, #4
	cmp r7, #4
	blt _021E7B44
	add r0, r5, #0
	mov r1, #1
	bl ov87_021E7BC4
	mov r1, #1
	add r0, r5, #0
	add r2, r1, #0
	add r3, r1, #0
	bl ov87_021E7C50
	add r0, r5, #0
	mov r1, #1
	bl ov87_021E7CE0
	add r0, r5, #0
	mov r1, #1
	bl ov87_021E7D6C
	mov r1, #0
	add r0, r5, #0
	add r2, r1, #0
	mov r3, #2
	bl ov87_021E7C50
	mov r7, #0x4f
	lsl r7, r7, #2
	mov r4, #0
	add r6, r7, #4
_021E7B96:
	ldr r0, [r5, r7]
	bl SpriteTransfer_CreateCharTransferTask
	ldr r0, [r5, r6]
	bl SpriteTransfer_CreateExtPlttTransferTask
	add r4, r4, #1
	add r5, #0x10
	cmp r4, #5
	blt _021E7B96
	mov r0, #0x10
	mov r1, #1
	bl GfGfx_EngineBTogglePlanes
	mov r0, #0x10
	mov r1, #1
	bl GfGfx_EngineATogglePlanes
	add sp, #0x14
	pop {r4, r5, r6, r7, pc}
	nop
_021E7BC0: .word ov87_021E8394
	thumb_func_end ov87_021E7AF8
