	.extern ov48_0225A668
	.extern ov48_0225AD38
	.include "asm/macros.inc"
	.include "overlay_48.inc"
	.include "global.inc"

.text
	thumb_func_start ov48_0225AAAC
ov48_0225AAAC: ; 0x0225AAAC
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x88
	str r0, [sp, #0x2c]
	str r2, [sp, #0x30]
	mov r0, #0
	add r5, r1, #0
	add r2, sp, #0x44
	add r1, r0, #0
	stmia r2!, {r0, r1}
	stmia r2!, {r0, r1}
	stmia r2!, {r0, r1}
	stmia r2!, {r0, r1}
	mov r2, #0x4b
	ldr r0, [sp, #0x2c]
	lsl r2, r2, #2
	str r3, [sp, #0x34]
	bl memset
	ldr r0, [sp, #0x2c]
	mov r2, #2
	strh r2, [r0, #0x30]
	mov r1, #0x80
	strh r1, [r0, #0x32]
	ldr r0, [r5, #4]
	ldr r3, [sp, #0x34]
	str r0, [sp, #0x44]
	add r0, sp, #0x64
	str r0, [sp, #0x48]
	mov r0, #0x40
	str r0, [sp, #0x58]
	str r3, [sp, #0x60]
	str r2, [sp, #0x5c]
	mov r3, #0
	str r3, [sp]
	str r2, [sp, #4]
	mov r2, #0xc
	str r2, [sp, #8]
	ldr r2, [sp, #0x34]
	add r0, #0xf4
	str r2, [sp, #0xc]
	add r1, #0xc4
	ldr r0, [r5, r0]
	ldr r1, [r5, r1]
	mov r2, #0x14
	bl AddPlttResObjFromOpenNarc
	ldr r1, [sp, #0x2c]
	str r0, [r1, #0x70]
	bl SpriteTransfer_CreatePlttTransferTask
	cmp r0, #0
	bne _0225AB18
	bl GF_AssertFail
_0225AB18:
	ldr r0, [sp, #0x2c]
	ldr r0, [r0, #0x70]
	bl sub_0200A740
	mov r0, #0x15
	str r0, [sp, #0x40]
	ldr r4, [sp, #0x2c]
	mov r0, #0x16
	str r0, [sp, #0x3c]
	mov r0, #0x17
	mov r6, #0
	str r0, [sp, #0x38]
	add r7, r4, #0
_0225AB32:
	str r6, [sp]
	mov r0, #2
	str r0, [sp, #4]
	ldr r0, [sp, #0x34]
	mov r1, #0x51
	str r0, [sp, #8]
	mov r0, #0x13
	lsl r0, r0, #4
	lsl r1, r1, #2
	ldr r0, [r5, r0]
	ldr r1, [r5, r1]
	ldr r2, [sp, #0x40]
	mov r3, #0
	bl AddCharResObjFromOpenNarc
	str r0, [r4, #0x6c]
	mov r1, #0x51
	str r6, [sp]
	mov r0, #2
	str r0, [sp, #4]
	ldr r0, [sp, #0x34]
	lsl r1, r1, #2
	str r0, [sp, #8]
	mov r0, #0x4e
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	ldr r1, [r5, r1]
	ldr r2, [sp, #0x3c]
	mov r3, #0
	bl AddCellOrAnimResObjFromOpenNarc
	str r0, [r4, #0x74]
	mov r1, #0x51
	str r6, [sp]
	mov r0, #3
	str r0, [sp, #4]
	ldr r0, [sp, #0x34]
	lsl r1, r1, #2
	str r0, [sp, #8]
	mov r0, #0x4f
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	ldr r1, [r5, r1]
	ldr r2, [sp, #0x38]
	mov r3, #0
	bl AddCellOrAnimResObjFromOpenNarc
	str r0, [r4, #0x78]
	ldr r0, [r4, #0x6c]
	bl SpriteTransfer_CreateCharTransferTask_AllocAtEnd
	cmp r0, #0
	bne _0225ABA0
	bl GF_AssertFail
_0225ABA0:
	ldr r0, [r4, #0x6c]
	bl sub_0200A740
	mov r0, #0
	str r6, [sp]
	mvn r0, r0
	str r0, [sp, #4]
	str r0, [sp, #8]
	mov r0, #0
	str r0, [sp, #0xc]
	mov r0, #1
	str r0, [sp, #0x10]
	mov r0, #0x13
	lsl r0, r0, #4
	ldr r0, [r5, r0]
	add r1, r6, #0
	str r0, [sp, #0x14]
	mov r0, #0x4d
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	mov r2, #0
	str r0, [sp, #0x18]
	mov r0, #0x4e
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	add r3, r6, #0
	str r0, [sp, #0x1c]
	mov r0, #0x4f
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	str r0, [sp, #0x20]
	mov r0, #0
	str r0, [sp, #0x24]
	str r0, [sp, #0x28]
	add r0, sp, #0x64
	bl CreateSpriteResourcesHeader
	add r0, sp, #0x44
	bl Sprite_Create
	str r0, [r7, #0x3c]
	mov r1, #1
	bl Sprite_SetAnimActiveFlag
	mov r1, #1
	ldr r0, [r7, #0x3c]
	lsl r1, r1, #0xc
	bl Sprite_SetAnimSpeed
	ldr r0, [sp, #0x40]
	add r6, r6, #1
	add r0, r0, #3
	str r0, [sp, #0x40]
	ldr r0, [sp, #0x3c]
	add r4, #0x10
	add r0, r0, #3
	str r0, [sp, #0x3c]
	ldr r0, [sp, #0x38]
	add r7, r7, #4
	add r0, r0, #3
	str r0, [sp, #0x38]
	cmp r6, #0xc
	blt _0225AB32
	ldr r0, [sp, #0x2c]
	bl ov48_0225AD38
	mov r1, #0
	ldr r0, [sp, #0x30]
	add r2, r1, #0
	bl ov48_0225A668
	add sp, #0x88
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
	thumb_func_end ov48_0225AAAC
