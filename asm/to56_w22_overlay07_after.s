	.include "asm/macros.inc"
	.include "overlay_07.inc"
	.include "global.inc"

	.extern _02234B80
	.extern _02237840
	.extern ov07_02233EB8
	.extern ov07_02233EBC
	.extern ov07_022344B4
	.extern ov07_022344C0
	.extern ov07_02234530
	.extern ov07_0223474C
	.extern ov07_0223475C
	.extern ov07_0223494C
	.extern ov07_02234A20
	.extern ov07_02234B8C
	.extern ov07_02234B98
	.extern ov07_02234BA8
	.extern ov07_02234BC0
	.extern ov07_02234BD8
	.extern ov07_02234BF0
	.extern ov07_02234C08
	.extern ov07_02234C20
	.extern ov07_02234C40
	.extern ov07_02234C64
	.extern ov07_02234CC8
	.extern ov07_02234D58
	.extern ov07_02234DE8
	.extern ov07_02234F48
	.extern ov07_022353E4
	.extern ov07_02235440
	.extern ov07_02235488
	.extern ov07_022354D0
	.extern ov07_022354E0
	.extern ov07_02235510
	.extern ov07_02235540
	.extern ov07_02235578
	.extern ov07_022355B0
	.extern ov07_02235620
	.extern ov07_02235624
	.extern ov07_02235628
	.extern ov07_022357D0
	.extern ov07_02235980
	.extern ov07_02235984
	.extern ov07_02235988
	.extern ov07_02235B30
	.extern ov07_02235B34
	.extern ov07_02235B38
	.extern ov07_02235CE0
	.extern ov07_02235CE4
	.extern ov07_02235CE8
	.extern ov07_02235E90
	.extern ov07_02235E94
	.extern ov07_02235E98
	.extern ov07_02236040
	.extern ov07_02236044
	.extern ov07_02236048
	.extern ov07_022361F0
	.extern ov07_022361F4
	.extern ov07_022361F8
	.extern ov07_022363A0
	.extern ov07_022363D8
	.extern ov07_02236414
	.extern ov07_02236444
	.extern ov07_02236468
	.extern ov07_0223646A
	.extern ov07_0223649C
	.extern ov07_02236520
	.extern ov07_02236670
	.extern ov07_022366C8
	.extern ov07_022366CC
	.extern ov07_022366CD
	.extern ov07_022366CE
	.extern ov07_022366D2
	.extern ov07_022366D8
	.extern ov07_022366D9
	.extern ov07_022366DA
	.extern ov07_022366E1
	.extern ov07_022366EB
	.extern ov07_022366EC
	.extern ov07_022366ED
	.extern ov07_022366EE
	.extern ov07_022366EF
	.extern ov07_022366F5
	.extern ov07_022366F6
	.extern ov07_022366F7
	.extern ov07_022366F8
	.extern ov07_022366F9
	.extern ov07_02236704
	.extern ov07_02236705
	.extern ov07_02236706
	.extern ov07_02236707
	.extern ov07_02236708
	.extern ov07_02236713
	.extern ov07_02236714
	.extern ov07_02236715
	.extern ov07_02236716
	.extern ov07_02236717
	.extern ov07_02236722
	.extern ov07_02236732
	.extern ov07_02236734
	.extern ov07_02236736
	.extern ov07_0223674C
	.extern ov07_02236750
	.extern ov07_02236774
	.extern ov07_02236778
	.extern ov07_0223677E
	.extern ov07_02236780
	.extern ov07_02236796
	.extern ov07_022367AE
	.extern ov07_022367B0
	.extern ov07_022367B2
	.extern ov07_022367B4
	.extern ov07_022367CE
	.extern ov07_02236800
	.extern ov07_0223680C
	.extern ov07_02236810
	.extern ov07_02236814
	.extern ov07_02236858
	.extern ov07_02236864
	.extern ov07_0223687C
	.extern ov07_0223717C
	.extern ov07_02237188
	.extern ov07_022371B8
	.extern ov07_02237200
	.extern ov07_02237228
	.extern ov07_02237254
	.extern ov07_0223729C
	.extern ov07_0223730E
	.extern ov07_02237310
	.extern ov07_0223738C
	.extern ov07_02237390
	.extern ov07_0223746C
	.extern ov07_02237470
	.extern ov07_02237474
	.extern ov07_022375BC
	.extern ov07_02237870
	.extern ov07_0223789C
	.extern ov07_022378B0

	.extern ov07_0221BFC0
	.extern ov07_0221BFD0
	.extern ov07_0221C410
	.extern ov07_0221C448
	.extern ov07_0221C468
	.extern ov07_0221C470
	.extern ov07_0221C4E8
	.extern ov07_0221F9E8
	.extern ov07_0221FA10
	.extern ov07_0221FA1C
	.extern ov07_0221FA2C
	.extern ov07_0221FA38
	.extern ov07_0221FA48
	.extern ov07_0221FB04
	.extern ov07_0221FB78
	.extern ov07_02222004
	.extern ov07_02222268
	.extern ov07_022222B4
	.extern ov07_02222EE8
	.extern ov07_02222EF8
	.extern ov07_02222F10
	.extern ov07_0222B2A8
	.extern ov07_02231924
	.extern ov07_0223192C
	.extern ov07_02231A20
	.extern ov07_02231E08
	.extern ov07_02231FE4
	.extern ov07_022324D8

	.extern ov07_0222B4E0

	.text

	thumb_func_start ov07_0222B640
ov07_0222B640: ; 0x0222B640
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x30
	mov r1, #0x40
	add r5, r0, #0
	bl ov07_022324D8
	add r4, r0, #0
	add r0, r5, #0
	add r1, r4, #0
	bl ov07_02231FE4
	ldr r0, [r4, #4]
	mov r1, #0
	bl ov07_0221C4E8
	str r0, [r4, #0x1c]
	ldr r0, [r4, #4]
	mov r1, #1
	bl ov07_0221C4E8
	str r0, [r4, #0x20]
	ldr r0, [r4, #4]
	mov r1, #2
	bl ov07_0221C4E8
	str r0, [r4, #0x24]
	mov r6, #0
	str r6, [r4, #0x28]
	add r0, r5, #0
	sub r6, #0x20
	bl ov07_0221C468
	add r1, r0, #0
	add r0, r5, #0
	bl ov07_02222004
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	mul r0, r6
	add r6, r0, #0
	mov r0, #0xfe
	lsl r0, r0, #0x16
	str r0, [r4, #0x2c]
	str r0, [r4, #0x30]
	add r0, r5, #0
	mov r1, #1
	bl ov07_0221FB78
	cmp r0, #1
	bne _0222B6AA
	mov r1, #0
	mvn r1, r1
	b _0222B6AC
_0222B6AA:
	mov r1, #1
_0222B6AC:
	add r0, r4, #0
	add r0, #0x36
	strb r1, [r0]
	add r1, sp, #0x24
	ldr r0, [r4, #0x20]
	add r1, #2
	add r2, sp, #0x24
	bl ManagedSprite_GetPositionXY
	add r2, sp, #0x24
	mov r1, #2
	ldrsh r1, [r2, r1]
	mov r3, #0
	ldrsh r2, [r2, r3]
	add r1, r1, r6
	lsl r1, r1, #0x10
	ldr r0, [r4, #0x1c]
	asr r1, r1, #0x10
	bl ManagedSprite_SetPositionXY
	ldr r0, [r4, #0x1c]
	mov r1, #2
	bl ManagedSprite_SetAffineOverwriteMode
	mov r0, #0x36
	ldrsb r0, [r4, r0]
	bl _fflt
	add r1, r0, #0
	ldr r0, [r4, #0x2c]
	bl _fmul
	add r1, r0, #0
	ldr r0, [r4, #0x1c]
	ldr r2, [r4, #0x30]
	bl ManagedSprite_SetAffineScale
	ldr r0, [r4, #0x1c]
	ldr r0, [r0]
	bl Sprite_GetPaletteProxy
	mov r1, #1
	bl ObjPlttTransfer_GetPaletteVramOffset
	add r6, r0, #0
	add r0, r5, #0
	bl ov07_0221BFD0
	add r1, r0, #0
	mov r0, #0x10
	str r0, [sp]
	mov r2, #0
	lsl r3, r6, #0x14
	str r2, [sp, #4]
	mov r0, #1
	str r0, [sp, #8]
	str r2, [sp, #0xc]
	mov r0, #0xf
	str r0, [sp, #0x10]
	ldr r0, _0222B888 ; =0x0000FFFF
	mov r2, #2
	str r0, [sp, #0x14]
	ldr r0, _0222B88C ; =0x0000044C
	lsr r3, r3, #0x10
	str r0, [sp, #0x18]
	ldr r0, [r4, #0x18]
	bl ov07_02222F10
	str r0, [r4, #0x38]
	ldr r0, [r4, #0x24]
	ldr r0, [r0]
	bl Sprite_GetPaletteProxy
	mov r1, #1
	bl ObjPlttTransfer_GetPaletteVramOffset
	add r6, r0, #0
	add r0, r5, #0
	bl ov07_0221BFD0
	add r1, r0, #0
	mov r0, #0x10
	str r0, [sp]
	mov r2, #0
	lsl r3, r6, #0x14
	str r2, [sp, #4]
	mov r0, #1
	str r0, [sp, #8]
	str r2, [sp, #0xc]
	mov r0, #0xf
	str r0, [sp, #0x10]
	ldr r0, _0222B888 ; =0x0000FFFF
	mov r2, #2
	str r0, [sp, #0x14]
	ldr r0, _0222B88C ; =0x0000044C
	lsr r3, r3, #0x10
	str r0, [sp, #0x18]
	ldr r0, [r4, #0x18]
	bl ov07_02222F10
	str r0, [r4, #0x3c]
	ldr r0, [r4, #4]
	bl ov07_0221C468
	lsl r0, r0, #0x18
	lsr r6, r0, #0x18
	ldr r0, [r4, #4]
	add r1, r6, #0
	bl ov07_02231924
	lsl r0, r0, #0x18
	lsr r5, r0, #0x18
	ldr r0, [r4, #4]
	bl ov07_0221BFC0
	add r1, r0, #0
	add r0, r5, #0
	add r2, sp, #0x2c
	bl ov07_02231A20
	add r2, sp, #0x28
	ldr r0, [r4, #0x1c]
	add r1, sp, #0x28
	add r2, #2
	bl ManagedSprite_GetPositionXY
	ldr r0, [r4, #4]
	add r1, r6, #0
	bl ov07_0223192C
	cmp r0, #3
	bne _0222B7C8
	ldr r0, [r4, #0x1c]
	mov r1, #1
	mov r5, #0
	bl ManagedSprite_SetPriority
	ldr r0, [r4, #0x1c]
	add r1, r5, #0
	bl ManagedSprite_SetDrawPriority
	b _0222B7DA
_0222B7C8:
	ldr r0, [r4, #0x1c]
	mov r1, #2
	bl ManagedSprite_SetPriority
	ldr r0, [r4, #0x1c]
	mov r1, #0
	bl ManagedSprite_SetDrawPriority
	mov r5, #2
_0222B7DA:
	ldr r0, [r4, #4]
	bl ov07_0221C470
	lsl r0, r0, #0x18
	lsr r6, r0, #0x18
	ldr r0, [r4, #4]
	add r1, r6, #0
	bl ov07_0221FA1C
	add r7, r0, #0
	ldr r0, [r4, #4]
	add r1, r6, #0
	bl ov07_0221FA10
	str r0, [sp, #0x1c]
	ldr r0, [r4, #4]
	add r1, r6, #0
	bl ov07_0221FA2C
	str r0, [sp, #0x20]
	ldr r0, [r4, #4]
	add r1, r6, #0
	bl ov07_0221FA38
	ldr r1, [sp, #0x1c]
	ldr r3, [sp, #0x20]
	str r0, [sp]
	lsl r0, r7, #0x10
	lsl r1, r1, #0x18
	lsl r2, r5, #0x18
	lsl r3, r3, #0x18
	lsr r0, r0, #0x10
	lsr r1, r1, #0x18
	lsr r2, r2, #0x18
	lsr r3, r3, #0x18
	bl GetMonPicHeightBySpeciesGenderForm
	add r2, r0, #0
	add r5, sp, #0x24
	mov r3, #0xa
	ldrsh r3, [r5, r3]
	mov r1, #4
	ldrsh r1, [r5, r1]
	add r2, r3, r2
	lsl r2, r2, #0x10
	ldr r0, [r4, #0x1c]
	asr r2, r2, #0x10
	bl ManagedSprite_SetPositionXY
	ldr r0, [r4, #0x1c]
	mov r1, #1
	bl ManagedSprite_SetDrawFlag
	add r0, r4, #0
	mov r1, #0
	add r0, #0x34
	strb r1, [r0]
	add r0, r4, #0
	mov r1, #0xf
	add r0, #0x35
	strb r1, [r0]
	sub r1, #0x10
	ldr r0, [r4, #4]
	add r2, r1, #0
	bl ov07_02231E08
	add r0, r4, #0
	add r0, #0x34
	ldrb r1, [r0]
	add r0, r4, #0
	add r0, #0x35
	ldrb r0, [r0]
	add r2, r4, #0
	lsl r0, r0, #8
	orr r1, r0
	ldr r0, _0222B890 ; =0x04000052
	strh r1, [r0]
	ldr r0, [r4, #4]
	ldr r1, _0222B894 ; =ov07_0222B4E0
	bl ov07_0221C410
	add r1, r4, #0
	bl ov07_0222B4E0
	add sp, #0x30
	pop {r3, r4, r5, r6, r7, pc}
	nop
_0222B888: .word 0x0000FFFF
_0222B88C: .word 0x0000044C
_0222B890: .word 0x04000052
_0222B894: .word ov07_0222B4E0
	thumb_func_end ov07_0222B640
