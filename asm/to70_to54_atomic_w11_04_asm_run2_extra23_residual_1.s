	.extern _02234B80
	.extern _02237840
	.extern ov07_0221BFD0
	.extern ov07_0221C410
	.extern ov07_0221C448
	.extern ov07_0221C468
	.extern ov07_0221C4E8
	.extern ov07_0221FA48
	.extern ov07_0221FA78
	.extern ov07_0221FA80
	.extern ov07_0221FA90
	.extern ov07_0221FAE8
	.extern ov07_02222268
	.extern ov07_022222F0
	.extern ov07_02222314
	.extern ov07_02231E08
	.extern ov07_02231FE4
	.extern ov07_022324D8
	.extern ov07_02232508
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
	.include "asm/macros.inc"
	.include "overlay_07.inc"
	.include "global.inc"
	.public ov07_0222AFAC

.text
	thumb_func_start ov07_0222AFAC
ov07_0222AFAC: ; 0x0222AFAC
	push {r3, r4, r5, lr}
	sub sp, #8
	add r4, r1, #0
	add r2, r0, #0
	ldrb r0, [r4]
	cmp r0, #0
	beq _0222AFC0
	cmp r0, #1
	beq _0222B0BE
	b _0222B10C
_0222AFC0:
	add r0, r4, #0
	add r0, #0xa0
	ldrb r0, [r0]
	cmp r0, #0
	bne _0222B03A
	mov r0, #0x22
	ldrsh r3, [r4, r0]
	mov r0, #0x20
	ldrsh r1, [r4, r0]
	mov r0, #3
	add r2, r1, #0
	str r3, [sp]
	add r2, #0x30
	str r0, [sp, #4]
	add r0, r4, #0
	lsl r2, r2, #0x10
	add r0, #0x34
	asr r2, r2, #0x10
	bl ov07_02222268
	mov r0, #0x22
	ldrsh r1, [r4, r0]
	mov r0, #0xa4
	ldrsh r0, [r4, r0]
	add r2, r1, r0
	mov r0, #0x20
	ldrsh r1, [r4, r0]
	lsl r0, r2, #0x10
	asr r3, r0, #0x10
	add r2, r1, #0
	add r2, #0x30
	lsl r2, r2, #0x10
	str r3, [sp]
	mov r0, #6
	str r0, [sp, #4]
	add r0, r4, #0
	add r0, #0x58
	asr r2, r2, #0x10
	bl ov07_02222268
	mov r0, #0x22
	ldrsh r1, [r4, r0]
	mov r0, #0xa4
	ldrsh r0, [r4, r0]
	add r2, r1, r0
	mov r0, #0x20
	ldrsh r1, [r4, r0]
	lsl r0, r2, #0x10
	asr r3, r0, #0x10
	add r2, r1, #0
	add r2, #0x30
	lsl r2, r2, #0x10
	str r3, [sp]
	mov r0, #8
	str r0, [sp, #4]
	add r0, r4, #0
	add r0, #0x7c
	asr r2, r2, #0x10
	bl ov07_02222268
	b _0222B0A8
_0222B03A:
	mov r0, #0x22
	ldrsh r3, [r4, r0]
	mov r0, #0x20
	ldrsh r2, [r4, r0]
	mov r0, #3
	add r1, r2, #0
	str r3, [sp]
	add r1, #0x30
	str r0, [sp, #4]
	add r0, r4, #0
	lsl r1, r1, #0x10
	add r0, #0x34
	asr r1, r1, #0x10
	bl ov07_02222268
	mov r0, #0x22
	ldrsh r1, [r4, r0]
	mov r0, #0xa4
	ldrsh r0, [r4, r0]
	add r1, r1, r0
	mov r0, #0x20
	ldrsh r2, [r4, r0]
	lsl r0, r1, #0x10
	asr r3, r0, #0x10
	add r1, r2, #0
	add r1, #0x30
	lsl r1, r1, #0x10
	str r3, [sp]
	mov r0, #6
	str r0, [sp, #4]
	add r0, r4, #0
	add r0, #0x58
	asr r1, r1, #0x10
	bl ov07_02222268
	mov r0, #0x22
	ldrsh r1, [r4, r0]
	mov r0, #0xa4
	ldrsh r0, [r4, r0]
	add r1, r1, r0
	mov r0, #0x20
	ldrsh r2, [r4, r0]
	lsl r0, r1, #0x10
	asr r3, r0, #0x10
	add r1, r2, #0
	add r1, #0x30
	lsl r1, r1, #0x10
	str r3, [sp]
	mov r0, #9
	str r0, [sp, #4]
	add r0, r4, #0
	add r0, #0x7c
	asr r1, r1, #0x10
	bl ov07_02222268
_0222B0A8:
	add r0, r4, #0
	add r0, #0xa0
	ldrb r0, [r0]
	add r1, r0, #1
	add r0, r4, #0
	add r0, #0xa0
	strb r1, [r0]
	ldrb r0, [r4]
	add r0, r0, #1
	strb r0, [r4]
	b _0222B11E
_0222B0BE:
	add r0, r4, #0
	ldr r1, [r4, #0x28]
	add r0, #0x58
	mov r5, #0
	bl ov07_022222F0
	cmp r0, #0
	bne _0222B0D0
	add r5, r5, #1
_0222B0D0:
	add r0, r4, #0
	ldr r1, [r4, #0x24]
	add r0, #0x34
	bl ov07_02222314
	cmp r0, #0
	bne _0222B0E0
	add r5, r5, #1
_0222B0E0:
	add r0, r4, #0
	ldr r1, [r4, #0x30]
	add r0, #0x7c
	bl ov07_022222F0
	cmp r0, #0
	bne _0222B0F0
	add r5, r5, #1
_0222B0F0:
	cmp r5, #3
	blt _0222B11E
	add r0, r4, #0
	add r0, #0xa0
	ldrb r0, [r0]
	cmp r0, #1
	ldrb r0, [r4]
	bne _0222B106
	sub r0, r0, #1
	strb r0, [r4]
	b _0222B11E
_0222B106:
	add r0, r0, #1
	strb r0, [r4]
	b _0222B11E
_0222B10C:
	ldr r0, [r4, #4]
	add r1, r2, #0
	bl ov07_0221C448
	add r0, r4, #0
	bl ov07_02232508
	add sp, #8
	pop {r3, r4, r5, pc}
_0222B11E:
	ldr r0, [r4, #0x28]
	bl ManagedSprite_TickFrame
	ldr r0, [r4, #0xc]
	bl SpriteSystem_DrawSprites
	add sp, #8
	pop {r3, r4, r5, pc}
	.balign 4, 0
	thumb_func_end ov07_0222AFAC
