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

	.extern ov07_0221BFD0
	.extern ov07_0221C410
	.extern ov07_0221C448
	.extern ov07_0221C468
	.extern ov07_0221F9E8
	.extern ov07_0221FA48
	.extern ov07_02222EF8
	.extern ov07_02222F10
	.extern ov07_0222A8D8
	.extern ov07_0223192C
	.extern ov07_02231E08
	.extern ov07_02231FE4
	.extern ov07_022324D8

	.text

	thumb_func_start ov07_0222AA20
ov07_0222AA20: ; 0x0222AA20
	push {r4, r5, r6, r7, lr}
	sub sp, #0x24
	add r5, r1, #0
	mov r4, #0
	str r0, [sp, #0x1c]
	str r4, [sp, #0x20]
	add r6, r5, #0
	add r7, r5, #0
_0222AA30:
	mov r0, #0x22
	ldrsh r0, [r6, r0]
	add r0, r0, #1
	strh r0, [r6, #0x22]
	mov r0, #0x22
	ldrsh r0, [r6, r0]
	cmp r0, #0x14
	blt _0222AA82
	ldr r0, [r7, #0x2c]
	bl ov07_0222A8D8
	mov r0, #0x22
	ldrsh r0, [r5, r0]
	cmp r0, #0x6e
	bne _0222AA82
	mov r1, #0
	mvn r1, r1
	ldr r0, [r5, #4]
	add r2, r1, #0
	bl ov07_02231E08
	ldr r0, [r5, #0x2c]
	mov r1, #1
	bl ManagedSprite_SetOamMode
	ldr r0, [r5, #0x30]
	mov r1, #1
	bl ManagedSprite_SetOamMode
	ldr r0, [r5, #0x34]
	mov r1, #1
	bl ManagedSprite_SetOamMode
	ldr r0, [r5, #0x38]
	mov r1, #1
	bl ManagedSprite_SetOamMode
	mov r0, #0xf
	str r0, [r5, #0x54]
	mov r0, #0
	str r0, [r5, #0x58]
_0222AA82:
	add r4, r4, #1
	add r6, r6, #2
	add r7, r7, #4
	cmp r4, #4
	blt _0222AA30
	mov r0, #0x22
	ldrsh r0, [r5, r0]
	cmp r0, #0x32
	bne _0222AAC6
	ldr r0, [r5, #4]
	bl ov07_0221BFD0
	add r1, r0, #0
	mov r0, #0x10
	str r0, [sp]
	sub r0, #0x12
	str r0, [sp, #4]
	mov r2, #2
	str r2, [sp, #8]
	mov r0, #0
	str r0, [sp, #0xc]
	mov r0, #0xe
	str r0, [sp, #0x10]
	ldr r0, _0222ACA0 ; =0x0000FFFF
	str r0, [sp, #0x14]
	ldr r0, _0222ACA4 ; =0x000003EA
	str r0, [sp, #0x18]
	ldr r3, [r5, #0x1c]
	ldr r0, [r5, #0x18]
	lsl r3, r3, #0x14
	lsr r3, r3, #0x10
	bl ov07_02222F10
	str r0, [r5, #0x3c]
_0222AAC6:
	mov r0, #0x22
	ldrsh r0, [r5, r0]
	cmp r0, #0x3c
	bne _0222AB00
	ldr r0, [r5, #4]
	bl ov07_0221BFD0
	add r1, r0, #0
	mov r0, #0x10
	str r0, [sp]
	sub r0, #0x12
	str r0, [sp, #4]
	mov r2, #2
	str r2, [sp, #8]
	mov r0, #0xe
	str r0, [sp, #0xc]
	mov r0, #0
	str r0, [sp, #0x10]
	ldr r0, _0222ACA0 ; =0x0000FFFF
	str r0, [sp, #0x14]
	ldr r0, _0222ACA4 ; =0x000003EA
	str r0, [sp, #0x18]
	ldr r3, [r5, #0x1c]
	ldr r0, [r5, #0x18]
	lsl r3, r3, #0x14
	lsr r3, r3, #0x10
	bl ov07_02222F10
	str r0, [r5, #0x40]
_0222AB00:
	mov r0, #0x22
	ldrsh r0, [r5, r0]
	cmp r0, #0x46
	bne _0222AB3A
	ldr r0, [r5, #4]
	bl ov07_0221BFD0
	add r1, r0, #0
	mov r0, #0x10
	str r0, [sp]
	sub r0, #0x12
	str r0, [sp, #4]
	mov r2, #2
	str r2, [sp, #8]
	mov r0, #0
	str r0, [sp, #0xc]
	mov r0, #0xe
	str r0, [sp, #0x10]
	ldr r0, _0222ACA0 ; =0x0000FFFF
	str r0, [sp, #0x14]
	ldr r0, _0222ACA4 ; =0x000003EA
	str r0, [sp, #0x18]
	ldr r3, [r5, #0x1c]
	ldr r0, [r5, #0x18]
	lsl r3, r3, #0x14
	lsr r3, r3, #0x10
	bl ov07_02222F10
	str r0, [r5, #0x44]
_0222AB3A:
	mov r0, #0x22
	ldrsh r0, [r5, r0]
	cmp r0, #0x50
	bne _0222AB74
	ldr r0, [r5, #4]
	bl ov07_0221BFD0
	add r1, r0, #0
	mov r0, #0x10
	str r0, [sp]
	sub r0, #0x12
	str r0, [sp, #4]
	mov r2, #2
	str r2, [sp, #8]
	mov r0, #0xe
	str r0, [sp, #0xc]
	mov r0, #0
	str r0, [sp, #0x10]
	ldr r0, _0222ACA0 ; =0x0000FFFF
	str r0, [sp, #0x14]
	ldr r0, _0222ACA4 ; =0x000003EA
	str r0, [sp, #0x18]
	ldr r3, [r5, #0x1c]
	ldr r0, [r5, #0x18]
	lsl r3, r3, #0x14
	lsr r3, r3, #0x10
	bl ov07_02222F10
	str r0, [r5, #0x48]
_0222AB74:
	mov r0, #0x22
	ldrsh r0, [r5, r0]
	cmp r0, #0x5a
	bne _0222ABAE
	ldr r0, [r5, #4]
	bl ov07_0221BFD0
	add r1, r0, #0
	mov r0, #0x10
	str r0, [sp]
	sub r0, #0x12
	str r0, [sp, #4]
	mov r2, #2
	str r2, [sp, #8]
	mov r0, #0
	str r0, [sp, #0xc]
	mov r0, #0xe
	str r0, [sp, #0x10]
	ldr r0, _0222ACA0 ; =0x0000FFFF
	str r0, [sp, #0x14]
	ldr r0, _0222ACA4 ; =0x000003EA
	str r0, [sp, #0x18]
	ldr r3, [r5, #0x1c]
	ldr r0, [r5, #0x18]
	lsl r3, r3, #0x14
	lsr r3, r3, #0x10
	bl ov07_02222F10
	str r0, [r5, #0x4c]
_0222ABAE:
	mov r0, #0x22
	ldrsh r0, [r5, r0]
	cmp r0, #0x64
	bne _0222ABE8
	ldr r0, [r5, #4]
	bl ov07_0221BFD0
	add r1, r0, #0
	mov r0, #0x10
	str r0, [sp]
	sub r0, #0x12
	str r0, [sp, #4]
	mov r2, #2
	str r2, [sp, #8]
	mov r0, #0xe
	str r0, [sp, #0xc]
	mov r0, #0
	str r0, [sp, #0x10]
	ldr r0, _0222ACA0 ; =0x0000FFFF
	str r0, [sp, #0x14]
	ldr r0, _0222ACA4 ; =0x000003EA
	str r0, [sp, #0x18]
	ldr r3, [r5, #0x1c]
	ldr r0, [r5, #0x18]
	lsl r3, r3, #0x14
	lsr r3, r3, #0x10
	bl ov07_02222F10
	str r0, [r5, #0x50]
_0222ABE8:
	mov r1, #0x22
	ldrsh r0, [r5, r1]
	cmp r0, #0x6e
	bne _0222AC22
	sub r1, #0x23
	ldr r0, [r5, #4]
	add r2, r1, #0
	bl ov07_02231E08
	ldr r0, [r5, #0x2c]
	mov r1, #1
	bl ManagedSprite_SetOamMode
	ldr r0, [r5, #0x30]
	mov r1, #1
	bl ManagedSprite_SetOamMode
	ldr r0, [r5, #0x34]
	mov r1, #1
	bl ManagedSprite_SetOamMode
	ldr r0, [r5, #0x38]
	mov r1, #1
	bl ManagedSprite_SetOamMode
	mov r0, #0xf
	str r0, [r5, #0x54]
	mov r0, #0
	str r0, [r5, #0x58]
_0222AC22:
	mov r0, #0x22
	ldrsh r0, [r5, r0]
	cmp r0, #0x6e
	blt _0222AC5A
	ldr r0, [r5, #0x54]
	cmp r0, #0
	ble _0222AC34
	sub r0, r0, #1
	str r0, [r5, #0x54]
_0222AC34:
	ldr r0, [r5, #0x58]
	cmp r0, #0xf
	bge _0222AC3E
	add r0, r0, #1
	str r0, [r5, #0x58]
_0222AC3E:
	ldr r0, [r5, #0x58]
	ldr r1, [r5, #0x54]
	lsl r0, r0, #8
	orr r1, r0
	ldr r0, _0222ACA8 ; =0x04000052
	strh r1, [r0]
	ldr r0, [r5, #0x54]
	cmp r0, #0
	bne _0222AC5A
	ldr r0, [r5, #0x58]
	cmp r0, #0xf
	bne _0222AC5A
	mov r0, #4
	str r0, [sp, #0x20]
_0222AC5A:
	ldr r0, [sp, #0x20]
	cmp r0, #4
	bne _0222AC96
	mov r6, #0
	add r4, r5, #0
_0222AC64:
	ldr r0, [r4, #0x2c]
	bl Sprite_DeleteAndFreeResources
	add r6, r6, #1
	add r4, r4, #4
	cmp r6, #4
	blt _0222AC64
	mov r6, #0
	add r4, r5, #0
_0222AC76:
	ldr r0, [r4, #0x3c]
	bl ov07_02222EF8
	add r6, r6, #1
	add r4, r4, #4
	cmp r6, #6
	blt _0222AC76
	ldr r0, [r5, #4]
	ldr r1, [sp, #0x1c]
	bl ov07_0221C448
	add r0, r5, #0
	bl Heap_Free
	add sp, #0x24
	pop {r4, r5, r6, r7, pc}
_0222AC96:
	ldr r0, [r5, #0x10]
	bl SpriteSystem_DrawSprites
	add sp, #0x24
	pop {r4, r5, r6, r7, pc}
	.balign 4, 0
_0222ACA0: .word 0x0000FFFF
_0222ACA4: .word 0x000003EA
_0222ACA8: .word 0x04000052
	thumb_func_end ov07_0222AA20

	thumb_func_start ov07_0222ACAC
ov07_0222ACAC: ; 0x0222ACAC
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x40
	mov r1, #0x5c
	str r0, [sp]
	add r5, r3, #0
	bl ov07_022324D8
	add r4, r0, #0
	mov r0, #0xa
	strh r0, [r4, #0x20]
	ldr r0, [sp]
	add r1, r4, #0
	bl ov07_02231FE4
	ldr r1, [r4, #4]
	add r0, sp, #0xc
	bl ov07_0221F9E8
	str r5, [r4, #0x2c]
	mov r0, #0
	strh r0, [r4, #0x22]
	mov r0, #1
	str r0, [sp, #8]
	mov r7, #5
	add r6, r4, #2
	add r5, r4, #4
_0222ACE0:
	bl LCRandom
	lsr r1, r0, #0x1f
	lsl r2, r0, #0x1e
	sub r2, r2, r1
	mov r0, #0x1e
	ror r2, r0
	add r0, r1, r2
	add r0, r7, r0
	strh r0, [r6, #0x22]
	ldr r0, [r4, #8]
	ldr r1, [r4, #0x10]
	add r2, sp, #0xc
	bl SpriteSystem_NewSprite
	str r0, [r5, #0x2c]
	ldr r0, [sp, #8]
	add r7, r7, #5
	add r0, r0, #1
	add r6, r6, #2
	add r5, r5, #4
	str r0, [sp, #8]
	cmp r0, #4
	blt _0222ACE0
	ldr r0, [r4, #0x30]
	mov r1, #1
	bl ManagedSprite_SetFlipMode
	ldr r0, [r4, #0x34]
	mov r1, #1
	bl ManagedSprite_SetFlipMode
	ldr r0, [sp]
	bl ov07_0221C468
	add r1, r0, #0
	ldr r0, [sp]
	bl ov07_0221FA48
	mov r1, #0
	add r5, r0, #0
	bl Pokepic_GetAttr
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	str r0, [sp, #4]
	add r0, r5, #0
	mov r1, #1
	bl Pokepic_GetAttr
	add r0, r5, #0
	mov r1, #0x29
	bl Pokepic_GetAttr
	ldr r0, [sp]
	bl ov07_0221C468
	add r1, r0, #0
	ldr r0, [sp]
	bl ov07_0223192C
	cmp r0, #3
	bne _0222AD62
	mov r7, #0x8c
	b _0222AD64
_0222AD62:
	mov r7, #0x54
_0222AD64:
	mov r6, #0
	add r5, r4, #0
_0222AD68:
	ldr r0, [r5, #0x2c]
	ldr r1, [sp, #4]
	add r2, r7, #0
	bl ManagedSprite_SetPositionXY
	add r6, r6, #1
	add r5, r5, #4
	cmp r6, #4
	blt _0222AD68
	ldr r0, [sp]
	bl ov07_0221C468
	add r1, r0, #0
	ldr r0, [sp]
	bl ov07_0223192C
	cmp r0, #3
	ldr r0, [r4, #0x2c]
	bne _0222ADAE
	mov r1, #0xa
	bl ManagedSprite_SetDrawPriority
	ldr r0, [r4, #0x34]
	mov r1, #0xa
	bl ManagedSprite_SetDrawPriority
	ldr r0, [r4, #0x30]
	mov r1, #0x12
	bl ManagedSprite_SetDrawPriority
	ldr r0, [r4, #0x38]
	mov r1, #0x12
	bl ManagedSprite_SetDrawPriority
	b _0222ADCC
_0222ADAE:
	mov r1, #0x12
	bl ManagedSprite_SetDrawPriority
	ldr r0, [r4, #0x34]
	mov r1, #0x12
	bl ManagedSprite_SetDrawPriority
	ldr r0, [r4, #0x30]
	mov r1, #0xa
	bl ManagedSprite_SetDrawPriority
	ldr r0, [r4, #0x38]
	mov r1, #0xa
	bl ManagedSprite_SetDrawPriority
_0222ADCC:
	mov r1, #0x1f
	ldr r0, [r4, #0x2c]
	mvn r1, r1
	mov r2, #0
	bl ManagedSprite_OffsetPositionXY
	mov r1, #0x27
	ldr r0, [r4, #0x38]
	mvn r1, r1
	mov r2, #4
	bl ManagedSprite_OffsetPositionXY
	ldr r0, [r4, #0x34]
	mov r1, #0x20
	mov r2, #0
	bl ManagedSprite_OffsetPositionXY
	ldr r0, [r4, #0x30]
	mov r1, #0x28
	mov r2, #4
	bl ManagedSprite_OffsetPositionXY
	ldr r0, [r4, #0x2c]
	bl ManagedSprite_GetPaletteOverrideOffset
	str r0, [r4, #0x1c]
	ldr r0, [r4, #4]
	ldr r1, _0222AE10 ; =ov07_0222AA20
	add r2, r4, #0
	bl ov07_0221C410
	add sp, #0x40
	pop {r3, r4, r5, r6, r7, pc}
	nop
_0222AE10: .word ov07_0222AA20
	thumb_func_end ov07_0222ACAC
