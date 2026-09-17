	.include "asm/macros.inc"
	.include "overlay_119.inc"
	.include "global.inc"

	.text

	thumb_func_start ov119_0225F020
ov119_0225F020: ; 0x0225F020
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x60
	add r7, r1, #0
	str r0, [sp, #0x14]
	ldr r0, [r7]
	ldr r4, [r7, #0xc]
	cmp r0, #7
	bls _0225F032
	b _0225F35E
_0225F032:
	add r1, r0, r0
	add r1, pc
	ldrh r1, [r1, #6]
	lsl r1, r1, #0x10
	asr r1, r1, #0x10
	add pc, r1
_0225F03E: ; jump table
	.short _0225F04E - _0225F03E - 2 ; case 0
	.short _0225F10C - _0225F03E - 2 ; case 1
	.short _0225F126 - _0225F03E - 2 ; case 2
	.short _0225F132 - _0225F03E - 2 ; case 3
	.short _0225F196 - _0225F03E - 2 ; case 4
	.short _0225F1FE - _0225F03E - 2 ; case 5
	.short _0225F28A - _0225F03E - 2 ; case 6
	.short _0225F314 - _0225F03E - 2 ; case 7
_0225F04E:
	mov r1, #0x1e
	mov r0, #4
	lsl r1, r1, #4
	bl Heap_Alloc
	mov r2, #0x1e
	str r0, [r7, #0xc]
	mov r1, #0
	lsl r2, r2, #4
	bl memset
	ldr r0, [r7, #0x10]
	ldr r4, [r7, #0xc]
	ldr r1, [r0, #0x24]
	mov r0, #7
	lsl r0, r0, #6
	str r1, [r4, r0]
	bl ov01_021EFFEC
	str r0, [r4, #0x44]
	add r0, r4, #0
	add r0, #0x48
	mov r1, #2
	mov r2, #1
	bl ov01_021F05C4
	mov r0, #1
	str r0, [sp]
	mov r0, #7
	str r0, [sp, #4]
	mov r0, #9
	str r0, [sp, #8]
	mov r0, #8
	str r0, [sp, #0xc]
	ldr r0, _0225F370 ; =0x000927C0
	mov r2, #0x61
	str r0, [sp, #0x10]
	add r1, r4, #0
	lsl r2, r2, #2
	ldr r0, [r7, #0x20]
	add r1, #0x48
	add r2, r4, r2
	mov r3, #0
	bl ov01_021F0614
	mov r0, #0
	str r0, [sp, #0x18]
	add r6, r0, #0
	add r0, r4, #0
	str r0, [sp, #0x20]
	add r0, #0x48
	add r5, r4, #0
	str r0, [sp, #0x20]
_0225F0B8:
	mov r0, #0
	str r0, [sp]
	mov r1, #0x61
	str r0, [sp, #4]
	lsl r1, r1, #2
	mov r2, #2
	mov r3, #6
	ldr r0, [sp, #0x20]
	add r1, r4, r1
	lsl r2, r2, #0x12
	lsl r3, r3, #0x10
	bl ov01_021F0718
	mov r1, #0x6e
	lsl r1, r1, #2
	str r0, [r5, r1]
	add r0, r1, #0
	ldr r0, [r5, r0]
	mov r1, #0
	bl Sprite_SetDrawFlag
	mov r0, #0x6e
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	add r1, r6, #0
	bl Sprite_SetDrawPriority
	ldr r0, [sp, #0x18]
	add r5, r5, #4
	add r0, r0, #1
	add r6, r6, #2
	str r0, [sp, #0x18]
	cmp r0, #2
	blt _0225F0B8
	mov r0, #0x10
	mov r1, #1
	bl GfGfx_EngineATogglePlanes
	ldr r0, [r7]
	add r0, r0, #1
	str r0, [r7]
	b _0225F35E
_0225F10C:
	mov r0, #2
	str r0, [sp]
	mov r0, #1
	add r1, r0, #0
	sub r1, #0x11
	add r2, r1, #0
	add r3, r7, #4
	bl ov01_021EFCF8
	ldr r0, [r7]
	add r0, r0, #1
	str r0, [r7]
	b _0225F35E
_0225F126:
	ldr r1, [r7, #4]
	cmp r1, #0
	beq _0225F1F4
	add r0, r0, #1
	str r0, [r7]
	b _0225F35E
_0225F132:
	mov r0, #0xa
	mov r2, #1
	str r0, [sp]
	add r0, r4, #0
	mov r1, #0x29
	lsl r2, r2, #0xc
	mov r3, #2
	bl ov01_021EFEC8
	ldr r1, [r4]
	add r0, sp, #0x54
	add r2, r1, #0
	add r3, r1, #0
	bl ov01_021F074C
	mov r6, #0
	add r5, r4, #0
_0225F154:
	mov r0, #0x6e
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	mov r1, #1
	bl Sprite_SetDrawFlag
	mov r0, #0x6e
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	mov r1, #2
	bl Sprite_SetAffineOverwriteMode
	mov r0, #0x6e
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	add r1, sp, #0x54
	bl Sprite_SetAffineScale
	add r6, r6, #1
	add r5, r5, #4
	cmp r6, #2
	blt _0225F154
	add r0, r4, #0
	ldr r2, _0225F374 ; =0x0000FFFF
	add r0, #0x18
	mov r1, #0
	mov r3, #0xa
	bl ov01_021EFE34
	ldr r0, [r7]
	add r0, r0, #1
	str r0, [r7]
	b _0225F35E
_0225F196:
	add r0, r4, #0
	bl ov01_021EFF28
	ldr r1, [r4]
	str r0, [sp, #0x1c]
	add r0, sp, #0x54
	add r2, r1, #0
	add r3, r1, #0
	bl ov01_021F074C
	add r0, r4, #0
	add r0, #0x18
	bl ov01_021EFE44
	mov r6, #0
	add r5, r4, #0
_0225F1B6:
	mov r0, #0x6e
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	add r1, sp, #0x54
	bl Sprite_SetAffineScale
	add r6, r6, #1
	add r5, r5, #4
	cmp r6, #2
	blt _0225F1B6
	mov r0, #0x6e
	ldr r1, [r4, #0x18]
	lsl r0, r0, #2
	lsl r1, r1, #0x10
	ldr r0, [r4, r0]
	lsr r1, r1, #0x10
	bl Sprite_SetAffineZRotation
	mov r1, #0x6f
	lsl r1, r1, #2
	ldr r0, [r4, r1]
	ldr r2, [r4, #0x18]
	sub r1, #0xbc
	sub r1, r2, r1
	lsl r1, r1, #0x10
	lsr r1, r1, #0x10
	bl Sprite_SetAffineZRotation
	ldr r0, [sp, #0x1c]
	cmp r0, #1
	beq _0225F1F6
_0225F1F4:
	b _0225F35E
_0225F1F6:
	ldr r0, [r7]
	add r0, r0, #1
	str r0, [r7]
	b _0225F35E
_0225F1FE:
	mov r0, #0
	str r0, [sp]
	mov r0, #0xff
	lsl r0, r0, #0xc
	str r0, [sp, #4]
	mov r0, #0xa
	lsl r0, r0, #0xc
	str r0, [sp, #8]
	ldr r1, [r4, #0x44]
	add r0, r7, #0
	mov r2, #0x60
	mov r3, #6
	bl ov01_021F0050
	mov r0, #0x6e
	lsl r0, r0, #2
	ldr r0, [r4, r0]
	mov r1, #1
	bl Sprite_SetAnimCtrlSeq
	mov r0, #0x6f
	lsl r0, r0, #2
	ldr r0, [r4, r0]
	mov r1, #2
	bl Sprite_SetAnimCtrlSeq
	mov r0, #6
	str r0, [sp]
	add r0, r4, #0
	mov r2, #0xff
	mov r3, #0xa
	add r0, #0x2c
	mov r1, #0
	lsl r2, r2, #0xc
	lsl r3, r3, #0xc
	bl ov01_021EFEC8
	mov r0, #7
	lsl r0, r0, #6
	ldr r0, [r4, r0]
	bl Camera_GetDistance
	add r1, r0, #0
	mov r0, #6
	str r0, [sp]
	mov r0, #0x71
	mov r2, #0x7d
	lsl r0, r0, #2
	lsl r2, r2, #0xe
	ldr r3, _0225F378 ; =0xFFFF6000
	add r0, r4, r0
	sub r2, r1, r2
	bl ov01_021EFEC8
	mov r0, #0x6e
	lsl r0, r0, #2
	ldr r0, [r4, r0]
	mov r1, #0
	bl Sprite_SetAffineZRotation
	mov r0, #0x6f
	lsl r0, r0, #2
	ldr r0, [r4, r0]
	mov r1, #0
	bl Sprite_SetAffineZRotation
	ldr r0, [r7]
	add r0, r0, #1
	str r0, [r7]
	b _0225F35E
_0225F28A:
	add r0, r4, #0
	add r0, #0x2c
	bl ov01_021EFF28
	mov r1, #2
	ldr r2, [r4, #0x2c]
	lsl r1, r1, #0x12
	sub r1, r1, r2
	mov r2, #6
	add r0, sp, #0x30
	lsl r2, r2, #0x10
	mov r3, #0
	bl ov01_021F074C
	add r3, sp, #0x30
	ldmia r3!, {r0, r1}
	add r2, sp, #0x48
	stmia r2!, {r0, r1}
	ldr r0, [r3]
	mov r1, #2
	str r0, [r2]
	ldr r2, [r4, #0x2c]
	lsl r1, r1, #0x12
	add r1, r2, r1
	mov r2, #6
	add r0, sp, #0x24
	lsl r2, r2, #0x10
	mov r3, #0
	bl ov01_021F074C
	add r3, sp, #0x24
	ldmia r3!, {r0, r1}
	add r2, sp, #0x3c
	stmia r2!, {r0, r1}
	ldr r0, [r3]
	add r1, sp, #0x48
	str r0, [r2]
	mov r0, #0x6e
	lsl r0, r0, #2
	ldr r0, [r4, r0]
	bl Sprite_SetMatrix
	mov r0, #0x6f
	lsl r0, r0, #2
	ldr r0, [r4, r0]
	add r1, sp, #0x3c
	bl Sprite_SetMatrix
	mov r0, #0x71
	lsl r0, r0, #2
	add r0, r4, r0
	bl ov01_021EFF28
	mov r1, #0x71
	lsl r1, r1, #2
	ldr r0, [r4, r1]
	sub r1, r1, #4
	ldr r1, [r4, r1]
	bl Camera_SetDistance
	add r0, r7, #0
	bl ov01_021EFE30
	cmp r0, #0
	beq _0225F35E
	ldr r0, [r7]
	add r0, r0, #1
	str r0, [r7]
	b _0225F35E
_0225F314:
	mov r0, #0
	bl sub_0200FC20
	ldr r1, [r7, #0x14]
	cmp r1, #0
	beq _0225F324
	mov r0, #1
	str r0, [r1]
_0225F324:
	mov r6, #0
	add r5, r4, #0
_0225F328:
	mov r0, #0x6e
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	bl Sprite_Delete
	add r6, r6, #1
	add r5, r5, #4
	cmp r6, #2
	blt _0225F328
	mov r1, #0x61
	add r0, r4, #0
	lsl r1, r1, #2
	add r0, #0x48
	add r1, r4, r1
	bl ov01_021F06EC
	add r0, r4, #0
	add r0, #0x48
	bl ov01_021F05F4
	ldr r0, [r4, #0x44]
	bl ov01_021F0028
	ldr r1, [sp, #0x14]
	add r0, r7, #0
	bl ov01_021EFCDC
_0225F35E:
	ldr r0, [r7]
	cmp r0, #7
	beq _0225F36A
	ldr r0, [r4, #0x48]
	bl SpriteList_RenderAndAnimateSprites
_0225F36A:
	add sp, #0x60
	pop {r3, r4, r5, r6, r7, pc}
	nop
_0225F370: .word 0x000927C0
_0225F374: .word 0x0000FFFF
_0225F378: .word 0xFFFF6000
	thumb_func_end ov119_0225F020
