	.extern _0223B4F8
	.extern _0223B508
	.extern _0223B524
	.extern _0223B540
	.extern _0223B55C
	.extern _0223B578
	.extern ov74_02229200
	.extern ov74_02229294
	.extern ov74_02229450
	.extern ov74_02229DA0
	.extern ov74_02229DCC
	.extern ov74_02229DD4
	.extern ov74_02229DE0
	.extern ov74_02229E60
	.extern ov74_02229F90
	.extern ov74_02229FF4
	.extern ov74_0222A2A4
	.extern ov74_0222A2E0
	.extern ov74_0222A35C
	.extern ov74_0222A3CC
	.extern ov74_0222A494
	.extern ov74_0222A4CC
	.extern ov74_0222A504
	.extern ov74_0222A538
	.extern ov74_0222A560
	.extern ov74_0222A5AC
	.extern ov74_0222A5CC
	.extern ov74_0222A650
	.extern ov74_0222A688
	.extern ov74_0222A6C0
	.extern ov74_0222ACD8
	.extern ov74_0222AE6C
	.extern ov74_0222C2EC
	.extern ov74_0222CD94
	.extern ov74_0222D2D4
	.extern ov74_0222D308
	.extern ov74_0222D358
	.extern ov74_0222D414
	.extern ov74_0222D490
	.extern ov74_0222D494
	.extern ov74_0222D4E4
	.extern ov74_0222D55C
	.extern ov74_0222D5C4
	.extern ov74_0222D614
	.extern ov74_0222D690
	.extern ov74_0222D774
	.extern ov74_0222D78C
	.extern ov74_0222D7D0
	.extern ov74_0222DDFC
	.extern ov74_0222E1F4
	.extern ov74_0222EB44
	.extern ov74_0222EE08
	.extern ov74_0222EE0C
	.extern ov74_0222EE58
	.extern ov74_0222EEB0
	.extern ov74_0222EF18
	.extern ov74_0222EF64
	.extern ov74_0222EF68
	.extern ov74_0222F598
	.extern ov74_0222F7D4
	.extern ov74_0222FB8C
	.extern ov74_02230018
	.extern ov74_02230030
	.extern ov74_02230070
	.extern ov74_022300A8
	.extern ov74_02230110
	.extern ov74_02230138
	.extern ov74_02230404
	.extern ov74_02230520
	.extern ov74_02230590
	.extern ov74_022305E4
	.extern ov74_022306C8
	.extern ov74_022308E0
	.extern ov74_02230964
	.extern ov74_02230988
	.extern ov74_022309AC
	.extern ov74_022309F0
	.extern ov74_02231054
	.extern ov74_022310AC
	.extern ov74_022310C4
	.extern ov74_022310D0
	.extern ov74_022310DC
	.extern ov74_022310E8
	.extern ov74_022310F4
	.extern ov74_02231100
	.extern ov74_02231154
	.extern ov74_0223115C
	.extern ov74_022311D8
	.extern ov74_022311DC
	.extern ov74_022311E8
	.extern ov74_02231260
	.extern ov74_02231A1C
	.extern ov74_02231D48
	.extern ov74_02231D70
	.extern ov74_02231D94
	.extern ov74_02231DB8
	.extern ov74_02231EC4
	.extern ov74_0223C960
	.include "asm/macros.inc"
	.include "overlay_74_thumb.inc"
	.include "global.inc"
	.extern ov74_02232B18

.text
	thumb_func_start ov74_02232BD4
ov74_02232BD4: ; 0x02232BD4
	push {r4, r5, r6, r7, lr}
	sub sp, #0x4c
	add r6, r0, #0
	mov r7, #0x6b
	mov r5, #0
	add r4, r6, #0
	lsl r7, r7, #2
_02232BE2:
	mov r0, #0x6a
	lsl r0, r0, #2
	ldr r0, [r4, r0]
	mov r1, #0
	bl Sprite_SetDrawFlag
	ldr r0, [r4, r7]
	mov r1, #0
	bl Sprite_SetDrawFlag
	add r5, r5, #1
	add r4, #0xc
	cmp r5, #0x1e
	blt _02232BE2
	mov r7, #0xf2
	mov r5, #0
	add r4, r6, #0
	lsl r7, r7, #2
_02232C06:
	ldr r0, [r4, r7]
	mov r1, #0
	bl Sprite_SetDrawFlag
	add r5, r5, #1
	add r4, #0xc
	cmp r5, #6
	blt _02232C06
	mov r0, #0xe6
	lsl r0, r0, #2
	ldr r0, [r6, r0]
	mov r1, #0
	bl Sprite_SetDrawFlag
	mov r0, #0xee
	lsl r0, r0, #2
	ldr r0, [r6, r0]
	mov r1, #0
	bl Sprite_SetDrawFlag
	mov r0, #0xea
	lsl r0, r0, #2
	ldr r0, [r6, r0]
	mov r1, #0
	bl Sprite_SetDrawFlag
	mov r2, #0x61
	lsl r2, r2, #2
	add r0, sp, #0x1c
	add r1, r6, #0
	add r2, r6, r2
	mov r3, #1
	bl ov74_02231D48
	mov r0, #0x1c
	mov r7, #0
	str r0, [sp, #0x10]
	add r4, r6, #0
	add r5, r6, #0
_02232C54:
	ldr r0, [sp, #0x10]
	lsl r0, r0, #0xc
	str r0, [sp, #0x24]
	mov r0, #0x8e
	lsl r0, r0, #0xc
	str r0, [sp, #0x28]
	add r0, sp, #0x1c
	bl Sprite_CreateAffine
	ldr r1, _02232D94 ; =0x00000414
	str r0, [r4, r1]
	add r0, r1, #0
	ldr r0, [r4, r0]
	mov r1, #1
	bl Sprite_SetAnimActiveFlag
	ldr r0, _02232D94 ; =0x00000414
	add r1, r7, #0
	ldr r0, [r4, r0]
	add r1, #0xa
	bl Sprite_SetAnimCtrlSeq
	ldr r0, _02232D94 ; =0x00000414
	mov r1, #1
	ldr r0, [r4, r0]
	bl Sprite_SetPriority
	ldr r0, _02232D94 ; =0x00000414
	mov r1, #1
	ldr r0, [r4, r0]
	bl Sprite_SetDrawFlag
	mov r1, #0x3d
	mov r2, #0xf3
	lsl r1, r1, #4
	lsl r2, r2, #2
	ldr r1, [r5, r1]
	ldr r2, [r5, r2]
	add r0, r6, #0
	bl ov74_02231D70
	mov r1, #0x3d
	mov r2, #0xf3
	lsl r1, r1, #4
	lsl r2, r2, #2
	str r0, [sp, #0x14]
	ldr r1, [r5, r1]
	ldr r2, [r5, r2]
	add r0, r6, #0
	bl ov74_02231D94
	mov r1, #0x3d
	mov r2, #0xf3
	lsl r1, r1, #4
	lsl r2, r2, #2
	str r0, [sp, #0x18]
	ldr r1, [r5, r1]
	ldr r2, [r5, r2]
	add r0, r6, #0
	bl ov74_02231DB8
	add r2, r0, #0
	ldr r0, _02232D94 ; =0x00000414
	str r7, [sp]
	ldr r0, [r4, r0]
	ldr r3, _02232D98 ; =gSystem + 0x60
	str r0, [sp, #4]
	ldrb r3, [r3, #0xa]
	ldr r0, [sp, #0x14]
	ldr r1, [sp, #0x18]
	bl ov74_02231EC4
	ldr r0, [sp, #0x10]
	add r7, r7, #1
	add r0, #0x28
	str r0, [sp, #0x10]
	add r4, r4, #4
	add r5, #0xc
	cmp r7, #6
	blt _02232C54
	mov r0, #0
	str r0, [sp]
	mov r0, #6
	lsl r0, r0, #8
	str r0, [sp, #4]
	mov r0, #1
	str r0, [sp, #8]
	mov r0, #0x4c
	str r0, [sp, #0xc]
	ldr r2, [r6, #0x20]
	mov r0, #0x71
	mov r1, #0x19
	mov r3, #2
	bl GfGfxLoader_LoadScrnData
	mov r0, #0x20
	str r0, [sp]
	mov r0, #0x18
	str r0, [sp, #4]
	mov r2, #0
	ldr r1, [r6]
	ldr r0, _02232D9C ; =ov74_0223C960
	add r3, r2, #0
	ldrb r0, [r0, r1]
	mov r1, #2
	str r0, [sp, #8]
	ldr r0, [r6, #0x20]
	bl BgTilemapRectChangePalette
	ldr r0, [r6, #0x20]
	mov r1, #2
	bl BgCommitTilemapBufferToVram
	mov r0, #2
	mov r1, #0
	bl GfGfx_EngineATogglePlanes
	mov r1, #7
	mov r0, #0
	lsl r1, r1, #6
	mov r2, #0x4c
	bl LoadFontPal0
	mov r1, #0
	mov r2, #0x3f
	str r1, [sp]
	mov r0, #0x4c
	str r0, [sp, #4]
	ldr r0, [r6, #0x20]
	lsl r2, r2, #4
	mov r3, #0xe
	bl LoadUserFrameGfx1
	ldr r0, [r6, #0x1c]
	ldr r2, _02232DA0 ; =0x000003D2
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	str r0, [sp]
	mov r0, #0x4c
	str r0, [sp, #4]
	ldr r0, [r6, #0x20]
	mov r1, #0
	mov r3, #0xd
	bl LoadUserFrameGfx2
	add r0, r6, #0
	bl ov74_02232B18
	mov r1, #0x46
	mov r0, #0xa
	lsl r1, r1, #4
	str r0, [r6, r1]
	sub r1, #0x34
	add r0, r6, #0
	add r1, r6, r1
	mov r2, #0x18
	bl ov74_02231A1C
	add sp, #0x4c
	pop {r4, r5, r6, r7, pc}
	.balign 4, 0
_02232D94: .word 0x00000414
_02232D98: .word gSystem + 0x60
_02232D9C: .word ov74_0223C960
_02232DA0: .word 0x000003D2
	thumb_func_end ov74_02232BD4
