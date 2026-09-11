	.include "asm/macros.inc"
	.public ov05_0221BA00
	.public ov05_0221BA70
	.public ov05_0221BB00
	.public ov05_0221BB30
	.public ov05_0221BD28
	.public ov05_0221BE04
	.public ov05_0221BE80
	.public ov05_0221BF08
	.public ov05_0221C018
	.public ov05_0221C050
	.public ov05_0221C21C
	.public ov05_0221C430
	.public ov05_0221C558
	.public ov05_0221C568
	.public ov05_0221C5A8
	.public ov05_0221C5C4
	.public ov05_0221C6C8
	.public ov05_0221C7FC
	.public ov05_0221C80C
	.public ov05_0221C8A0
	.public ov05_0221C8E0
	.public ov05_0221C908
	.public ov05_0221CAB8
	.public ov05_0221CB70
	.public ov05_0221CC04
	.public ov05_0221CC58
	.public ov05_0221CC74
	.public ov05_0221CCF4
	.public ov05_0221CD24
	.public ov05_0221CDC4
	.public ov05_0221CE0C
	.public ov05_0221CE50
	.public ov05_0221CE88
	.public ov05_0221CEB8
	.public ov05_0221CFF0
	.public ov05_0221D020
	.public ov05_0221D054
	.public ov05_0221D094
	.public ov05_0221D140
	.public ov05_0221D228
	.public ov05_0221D240
	.public ov05_0221D318
	.public ov05_0221D3AC
	.public ov05_0221D414
	.public ov05_0221D4D0
	.public ov05_0221D530
	.public ov05_0221D5DC
	.public ov05_0221D664
	.public ov05_0221D690
	.public ov05_0221D6C4
	.public ov05_0221D7AC
	.public ov05_0221D890
	.public ov05_0221D904
	.public ov05_0221D9F0
	.public ov05_0221DAE0
	.public ov05_0221DB18
	.public ov05_0221DB4C
	.public ov05_0221DB70
	.public ov05_0221DB94
	.public ov05_0221DC34
	.public ov05_0221DC60
	.public ov05_0221DD08
	.public ov05_0221DDEC
	.public ov05_0221DE38
	.public ov05_0221DE6C
	.public ov05_0221DF38
	.public ov05_0221E07C
	.public ov05_0221E0A8
	.public ov05_0221E110
	.public ov05_0221E274
	.public ov05_0221E2D8
	.public ov05_0221E390
	.public ov05_0221E42C
	.public ov05_0221E4C8
	.public ov05_0221E564
	.public ov05_0221E5E4
	.public ov05_0221E60C
	.public ov05_0221E624
	.public ov05_0221E714
	.public ov05_0221E74C
	.public ov05_0221E7B8
	.public ov05_0221E8A8
	.public ov05_0221E944
	.public ov05_0221E9C4
	.public ov05_0221E9F8
	.public ov05_0221EA18
	.public ov05_0221EA38
	.public _0221EA54
	.public ov05_0221EA58
	.public ov05_0221EA60
	.public ov05_0221EA6C
	.public ov05_0221EA6D
	.public ov05_0221EA78
	.public ov05_0221EA88
	.public ov05_0221EA98
	.public ov05_0221EAAC
	.public ov05_0221EAC0
	.public ov05_0221EAD4
	.public ov05_0221EAEC
	.public ov05_0221EB04
	.public ov05_0221EB1C
	.public ov05_0221EB38
	.public ov05_0221EB54
	.public ov05_0221EB70
	.public ov05_0221EB8C
	.public ov05_0221EBA8
	.public ov05_0221EBC4
	.public ov05_0221EBE0
	.public ov05_0221EC00
	.public ov05_0221EC28
	.public ov05_0221EC5C
	.public ov05_0221EC98
	.public ov05_0221ECE4
	.public ov05_0221EDA4
	.public ov05_0221EF2C
	.include "overlay_05.inc"
	.include "global.inc"

	.text
	.public ov05_0221DC60
	.public ov05_0221DE38
	.public ov05_0221E07C
	.public ov05_0221E274
	.public ov05_0221E390
	.public ov05_0221E42C
	.public ov05_0221E4C8
	.public ov05_0221E5E4
	.public ov05_0221E60C
	.public ov05_0221E714
	.public ov05_0221E74C


	thumb_func_start ov05_0221DD08
ov05_0221DD08: ; 0x0221DD08
	push {r4, r5, r6, r7, lr}
	sub sp, #0x1c
	add r5, r0, #0
	ldr r6, _0221DDDC ; =ov05_0221EA60
	mov r4, #0
	add r7, r5, #0
_0221DD14:
	cmp r4, #0
	beq _0221DD1C
	cmp r4, #3
	bne _0221DD24
_0221DD1C:
	mov r0, #0xa9
	lsl r0, r0, #2
	add r1, r5, r0
	b _0221DD3A
_0221DD24:
	mov r0, #0x86
	lsl r0, r0, #2
	ldrh r0, [r7, r0]
	cmp r0, #0
	bne _0221DD34
	ldr r0, _0221DDE0 ; =0x00000424
	add r1, r5, r0
	b _0221DD3A
_0221DD34:
	mov r0, #0xd9
	lsl r0, r0, #2
	add r1, r5, r0
_0221DD3A:
	mov r0, #0x10
	str r0, [sp]
	mov r0, #6
	str r0, [sp, #4]
	str r1, [sp, #8]
	mov r0, #0
	str r0, [sp, #0xc]
	str r0, [sp, #0x10]
	mov r0, #0x10
	str r0, [sp, #0x14]
	mov r0, #6
	str r0, [sp, #0x18]
	ldrb r2, [r6]
	ldrb r3, [r6, #1]
	ldr r0, [r5, #0xc]
	mov r1, #2
	bl CopyToBgTilemapRect
	mov r0, #0x86
	lsl r0, r0, #2
	ldrh r0, [r7, r0]
	cmp r0, #0
	beq _0221DDC6
	mov r0, #0x10
	str r0, [sp]
	mov r0, #6
	str r0, [sp, #4]
	add r0, r4, #3
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	str r0, [sp, #8]
	ldrb r2, [r6]
	ldrb r3, [r6, #1]
	ldr r0, [r5, #0xc]
	mov r1, #2
	bl BgTilemapRectChangePalette
	cmp r4, #3
	bhs _0221DD92
	ldr r0, [r5]
	add r0, #0x29
	ldrb r0, [r0]
	cmp r0, #1
	beq _0221DDA0
_0221DD92:
	cmp r4, #3
	blo _0221DDB4
	ldr r0, [r5]
	add r0, #0x29
	ldrb r0, [r0]
	cmp r0, #0
	bne _0221DDB4
_0221DDA0:
	ldr r1, _0221DDE4 ; =0x00000504
	add r3, r4, #3
	lsl r3, r3, #0x15
	mov r0, #2
	add r1, r5, r1
	mov r2, #0x20
	lsr r3, r3, #0x10
	bl BG_LoadPlttData
	b _0221DDC6
_0221DDB4:
	ldr r1, _0221DDE8 ; =0x000004E4
	add r3, r4, #3
	lsl r3, r3, #0x15
	mov r0, #2
	add r1, r5, r1
	mov r2, #0x20
	lsr r3, r3, #0x10
	bl BG_LoadPlttData
_0221DDC6:
	add r4, r4, #1
	add r7, #0x18
	add r6, r6, #2
	cmp r4, #6
	blo _0221DD14
	ldr r0, [r5, #0xc]
	mov r1, #2
	bl ScheduleBgTilemapBufferTransfer
	add sp, #0x1c
	pop {r4, r5, r6, r7, pc}
	.balign 4, 0
_0221DDDC: .word ov05_0221EA60
_0221DDE0: .word 0x00000424
_0221DDE4: .word 0x00000504
_0221DDE8: .word 0x000004E4
	thumb_func_end ov05_0221DD08




	thumb_func_start ov05_0221DDEC
ov05_0221DDEC: ; 0x0221DDEC
	push {r3, r4, r5, r6, r7, lr}
	add r4, r0, #0
	ldr r0, [r4, #0xc]
	str r1, [sp]
	add r1, r2, #0
	bl GetBgTilemapBuffer
	add r6, r0, #0
	ldr r0, [r4]
	add r0, #0x29
	ldrb r0, [r0]
	cmp r0, #0
	bne _0221DE0A
	mov r7, #0x10
	b _0221DE0C
_0221DE0A:
	mov r7, #0
_0221DE0C:
	mov r5, #0
_0221DE0E:
	lsl r0, r5, #5
	add r0, r7, r0
	lsl r4, r0, #1
	ldr r0, [sp]
	lsl r1, r5, #5
	add r0, r0, r1
	add r1, r6, r4
	mov r2, #0x20
	bl memcpy
	add r0, r6, r4
	mov r1, #0
	mov r2, #0x20
	bl memset
	add r0, r5, #1
	lsl r0, r0, #0x10
	lsr r5, r0, #0x10
	cmp r5, #0x18
	blo _0221DE0E
	pop {r3, r4, r5, r6, r7, pc}
	thumb_func_end ov05_0221DDEC
