	.extern ov12_022612A4
	.extern ov12_02261B80
	.extern ov12_02261CA8
	.extern ov12_02261F38
	.extern ov12_0226D120
	.extern ov12_0226D128
	.extern ov12_0226D15A
#include "constants/pokemon.h"
#include "constants/sndseq.h"
	.include "asm/macros.inc"
	.include "overlay_12_battle_controller_opponent.inc"
	.include "global.inc"

.text
	thumb_func_start ov12_0225C18C
ov12_0225C18C: ; 0x0225C18C
	push {r4, r5, r6, lr}
	sub sp, #0x1d8
	add r4, r1, #0
	add r6, r0, #0
	ldr r0, [r4]
	bl ov12_0223A8DC
	add r1, r4, #0
	add r1, #0x83
	ldrb r1, [r1]
	add r5, r0, #0
	cmp r1, #0xb
	bls _0225C1A8
	b _0225C6A6
_0225C1A8:
	add r1, r1, r1
	add r1, pc
	ldrh r1, [r1, #6]
	lsl r1, r1, #0x10
	asr r1, r1, #0x10
	add pc, r1
_0225C1B4: ; jump table
	.short _0225C1CC - _0225C1B4 - 2 ; case 0
	.short _0225C2E6 - _0225C1B4 - 2 ; case 1
	.short _0225C332 - _0225C1B4 - 2 ; case 2
	.short _0225C3C0 - _0225C1B4 - 2 ; case 3
	.short _0225C3D8 - _0225C1B4 - 2 ; case 4
	.short _0225C552 - _0225C1B4 - 2 ; case 5
	.short _0225C566 - _0225C1B4 - 2 ; case 6
	.short _0225C5DA - _0225C1B4 - 2 ; case 7
	.short _0225C5FE - _0225C1B4 - 2 ; case 8
	.short _0225C5DA - _0225C1B4 - 2 ; case 9
	.short _0225C64A - _0225C1B4 - 2 ; case 10
	.short _0225C5DA - _0225C1B4 - 2 ; case 11
_0225C1CC:
	add r0, r4, #0
	add r0, #0x82
	ldrb r1, [r0]
	ldr r0, _0225C4FC ; =ov12_0226D120
	ldrb r0, [r0, r1]
	str r0, [sp, #0x58]
	mov r0, #5
	str r0, [sp, #0x5c]
	add r0, r4, #0
	add r0, #0x81
	ldrb r0, [r0]
	str r0, [sp, #0x64]
	add r0, r4, #0
	add r0, #0x8e
	ldrh r0, [r0]
	str r0, [sp, #0x68]
	ldr r0, [r4]
	bl BattleSystem_GetSpriteSystem
	str r0, [sp, #0x74]
	ldr r0, [r4]
	bl BattleSystem_GetPaletteData
	str r0, [sp, #0x78]
	mov r2, #1
	mov r1, #0
	add r0, r4, #0
	str r2, [sp, #0x6c]
	str r1, [sp, #0x70]
	add r0, #0x94
	ldrh r0, [r0]
	cmp r0, #1
	bne _0225C212
	str r2, [sp, #0x60]
	b _0225C214
_0225C212:
	str r1, [sp, #0x60]
_0225C214:
	add r0, sp, #0x58
	bl ov07_02233DB8
	str r0, [r4, #0xc]
	ldr r0, [r4]
	bl BattleSystem_GetPokepicManager
	add r2, r4, #0
	add r3, r4, #0
	add r5, r0, #0
	mov r0, #0x69
	add r2, #0x86
	add r3, #0x82
	ldr r1, [r4, #4]
	lsl r0, r0, #2
	ldr r0, [r1, r0]
	ldrh r2, [r2]
	ldrb r3, [r3]
	add r1, sp, #0x30
	bl NARC_ReadPokepicAnimScript
	add r0, r4, #0
	add r0, #0x82
	ldrb r3, [r0]
	mov r0, #6
	add r2, r4, #0
	add r1, r3, #0
	mul r1, r0
	ldr r0, _0225C500 ; =ov07_022377F4 + 2
	add r2, #0x14
	ldrsh r0, [r0, r1]
	str r0, [sp]
	ldr r0, _0225C504 ; =ov07_022377F4 + 4
	ldrsh r0, [r0, r1]
	add r1, r5, #0
	lsl r5, r3, #2
	str r0, [sp, #4]
	add r0, r4, #0
	add r0, #0x85
	ldrb r0, [r0]
	ldr r3, _0225C508 ; =ov07_022377DC
	str r0, [sp, #8]
	mov r0, #0x90
	ldrsb r0, [r4, r0]
	ldrsh r3, [r3, r5]
	str r0, [sp, #0xc]
	mov r0, #0x91
	ldrsb r0, [r4, r0]
	str r0, [sp, #0x10]
	add r0, r4, #0
	add r0, #0x93
	ldrb r0, [r0]
	str r0, [sp, #0x14]
	add r0, r4, #0
	add r0, #0x81
	ldrb r0, [r0]
	str r0, [sp, #0x18]
	add r0, sp, #0x30
	str r0, [sp, #0x1c]
	mov r0, #0
	str r0, [sp, #0x20]
	ldr r0, [r4]
	bl ov12_022612A4
	ldr r1, [r4, #4]
	mov r2, #0
	str r0, [r1, #0x20]
	ldr r0, [r4, #4]
	mov r1, #0xc
	ldr r0, [r0, #0x20]
	bl Pokepic_SetAttr
	ldr r0, [r4, #4]
	mov r1, #0xd
	ldr r0, [r0, #0x20]
	mov r2, #0
	bl Pokepic_SetAttr
	ldr r0, [r4, #4]
	mov r1, #0x2c
	ldr r0, [r0, #0x20]
	mov r2, #0
	bl Pokepic_SetAttr
	ldr r0, [r4, #4]
	mov r1, #6
	ldr r0, [r0, #0x20]
	mov r2, #1
	bl Pokepic_SetAttr
	mov r0, #0
	str r0, [r4, #0x10]
	ldr r0, [r4]
	mov r1, #5
	bl ov07_0221FDFC
	str r0, [r4, #0x10]
	add r0, r4, #0
	add r0, #0x83
	ldrb r0, [r0]
	add r4, #0x83
	add sp, #0x1d8
	add r0, r0, #1
	strb r0, [r4]
	pop {r4, r5, r6, pc}
_0225C2E6:
	add r1, sp, #0x24
	mov r0, #0
	str r0, [r1]
	str r0, [r1, #4]
	str r0, [r1, #8]
	add r0, r4, #0
	add r0, #0x82
	ldrb r0, [r0]
	add r1, r4, #0
	add r2, r4, #0
	str r0, [sp, #0x24]
	add r1, #0x81
	add r2, #0x8c
	ldrb r1, [r1]
	ldrb r2, [r2]
	ldr r0, [r4]
	bl BattleSystem_GetPartyMon
	str r0, [sp, #0x2c]
	add r0, r4, #0
	add r0, #0x8e
	ldrh r0, [r0]
	add r1, sp, #0x24
	str r0, [sp, #0x28]
	mov r0, #5
	bl ov07_02232694
	str r0, [r4, #8]
	bl ov07_022329B0
	add r0, r4, #0
	add r0, #0x83
	ldrb r0, [r0]
	add r4, #0x83
	add sp, #0x1d8
	add r0, r0, #1
	strb r0, [r4]
	pop {r4, r5, r6, pc}
_0225C332:
	ldr r0, [r4, #0xc]
	bl ov07_02233F20
	cmp r0, #0
	beq _0225C346
	add r0, r4, #0
	add r0, #0x94
	ldrh r0, [r0]
	cmp r0, #0
	beq _0225C35A
_0225C346:
	ldr r0, [r4, #8]
	bl ov07_02232A04
	cmp r0, #1
	bne _0225C35A
	ldr r0, [r4, #0xc]
	bl ov07_02233EA0
	cmp r0, #1
	beq _0225C35C
_0225C35A:
	b _0225C6C4
_0225C35C:
	ldr r0, [r4, #0x10]
	cmp r0, #0
	beq _0225C366
	bl ov07_0221FE08
_0225C366:
	add r0, r4, #0
	add r0, #0x8e
	ldrh r0, [r0]
	mov r3, #0
	lsl r1, r0, #1
	ldr r0, _0225C50C ; =ov12_0226D15A
	ldrh r0, [r0, r1]
	mov r1, #0x10
	add r2, r1, #0
	str r0, [sp]
	ldr r0, [r4, #4]
	ldr r0, [r0, #0x20]
	bl Pokepic_StartPaletteFade
	ldr r0, [r4, #4]
	mov r1, #6
	ldr r0, [r0, #0x20]
	mov r2, #0
	bl Pokepic_SetAttr
	ldr r0, [r4, #8]
	bl ov07_02232A44
	add r0, r4, #0
	add r0, #0x84
	ldrb r0, [r0]
	cmp r0, #2
	ldr r0, _0225C510 ; =0x00000706
	bne _0225C3A8
	mov r1, #0x75
	bl sub_0200602C
	b _0225C3B0
_0225C3A8:
	mov r1, #0x74
	mvn r1, r1
	bl sub_0200602C
_0225C3B0:
	add r0, r4, #0
	add r0, #0x83
	ldrb r0, [r0]
	add r4, #0x83
	add sp, #0x1d8
	add r0, r0, #1
	strb r0, [r4]
	pop {r4, r5, r6, pc}
_0225C3C0:
	ldr r0, [r4, #0xc]
	bl ov07_02233E88
	cmp r0, #1
	beq _0225C3D8
	add r0, r4, #0
	add r0, #0x83
	ldrb r0, [r0]
	add r1, r0, #1
	add r0, r4, #0
	add r0, #0x83
	strb r1, [r0]
_0225C3D8:
	ldr r0, [r4, #4]
	mov r1, #0xc
	ldr r0, [r0, #0x20]
	bl Pokepic_GetAttr
	mov r1, #1
	lsl r1, r1, #8
	cmp r0, r1
	bne _0225C46E
	ldr r0, [r4, #8]
	bl ov07_02232A54
	cmp r0, #0
	bne _0225C46E
	add r0, r4, #0
	add r0, #0x84
	ldrb r0, [r0]
	cmp r0, #2
	bne _0225C40A
	ldr r0, [r4, #4]
	mov r1, #0x2d
	ldr r0, [r0, #0x20]
	mov r2, #0
	bl Pokepic_SetAttr
_0225C40A:
	mov r0, #0x69
	ldr r3, [r4, #4]
	lsl r0, r0, #2
	ldr r0, [r3, r0]
	add r1, r4, #0
	str r0, [sp]
	add r0, r4, #0
	add r0, #0x86
	ldrh r0, [r0]
	add r2, r4, #0
	add r1, #0x81
	str r0, [sp, #4]
	add r0, r4, #0
	add r0, #0x97
	ldrb r0, [r0]
	add r2, #0x82
	str r0, [sp, #8]
	add r0, r4, #0
	add r0, #0x84
	ldrb r0, [r0]
	str r0, [sp, #0xc]
	add r0, r4, #0
	add r0, #0x88
	ldr r0, [r0]
	str r0, [sp, #0x10]
	ldrb r1, [r1]
	ldrb r2, [r2]
	ldr r0, [r4]
	ldr r3, [r3, #0x20]
	bl ov12_02261F38
	add r0, r4, #0
	add r0, #0x8e
	ldrh r0, [r0]
	mov r2, #0
	add r3, r2, #0
	lsl r1, r0, #1
	ldr r0, _0225C50C ; =ov12_0226D15A
	ldrh r0, [r0, r1]
	mov r1, #0x10
	str r0, [sp]
	ldr r0, [r4, #4]
	ldr r0, [r0, #0x20]
	bl Pokepic_StartPaletteFade
	mov r0, #5
	add r4, #0x83
	add sp, #0x1d8
	strb r0, [r4]
	pop {r4, r5, r6, pc}
_0225C46E:
	ldr r0, [r4, #4]
	mov r1, #0xc
	ldr r0, [r0, #0x20]
	bl Pokepic_GetAttr
	mov r2, #1
	lsl r2, r2, #8
	cmp r0, r2
	ldr r0, [r4, #4]
	blt _0225C52C
	ldr r0, [r0, #0x20]
	mov r1, #0xc
	bl Pokepic_SetAttr
	ldr r0, [r4, #4]
	mov r1, #0xd
	add r2, r1, #0
	ldr r0, [r0, #0x20]
	add r2, #0xf3
	bl Pokepic_SetAttr
	add r0, r4, #0
	add r0, #0x84
	ldrb r0, [r0]
	cmp r0, #2
	bne _0225C4AE
	ldr r0, [r4, #4]
	mov r1, #0x2d
	ldr r0, [r0, #0x20]
	mov r2, #0
	bl Pokepic_SetAttr
_0225C4AE:
	mov r0, #0x69
	ldr r3, [r4, #4]
	lsl r0, r0, #2
	ldr r0, [r3, r0]
	add r1, r4, #0
	str r0, [sp]
	add r0, r4, #0
	add r0, #0x86
	ldrh r0, [r0]
	add r2, r4, #0
	add r1, #0x81
	str r0, [sp, #4]
	add r0, r4, #0
	add r0, #0x97
	ldrb r0, [r0]
	add r2, #0x82
	str r0, [sp, #8]
	add r0, r4, #0
	add r0, #0x84
	ldrb r0, [r0]
	str r0, [sp, #0xc]
	add r0, r4, #0
	add r0, #0x88
	ldr r0, [r0]
	str r0, [sp, #0x10]
	ldrb r1, [r1]
	ldrb r2, [r2]
	ldr r0, [r4]
	ldr r3, [r3, #0x20]
	bl ov12_02261F38
	add r0, r4, #0
	add r0, #0x8e
	ldrh r0, [r0]
	mov r2, #0
	mov r3, #1
	lsl r1, r0, #1
	ldr r0, _0225C50C ; =ov12_0226D15A
	b _0225C514
	.balign 4, 0
_0225C4FC: .word ov12_0226D120
_0225C500: .word ov07_022377F4 + 2
_0225C504: .word ov07_022377F4 + 4
_0225C508: .word ov07_022377DC
_0225C50C: .word ov12_0226D15A
_0225C510: .word 0x00000706
_0225C514:
	ldrh r0, [r0, r1]
	mov r1, #0x10
	str r0, [sp]
	ldr r0, [r4, #4]
	ldr r0, [r0, #0x20]
	bl Pokepic_StartPaletteFade
	mov r0, #5
	add r4, #0x83
	add sp, #0x1d8
	strb r0, [r4]
	pop {r4, r5, r6, pc}
_0225C52C:
	ldr r0, [r0, #0x20]
	mov r1, #0xc
	mov r2, #0x20
	bl Pokepic_AddAttr
	ldr r0, [r4, #4]
	mov r1, #0xd
	ldr r0, [r0, #0x20]
	mov r2, #0x20
	bl Pokepic_AddAttr
	ldr r0, [r4, #4]
	mov r1, #0x90
	ldrsb r1, [r4, r1]
	ldr r0, [r0, #0x20]
	bl sub_0200914C
	add sp, #0x1d8
	pop {r4, r5, r6, pc}
_0225C552:
	ldr r0, [r4, #8]
	bl ov07_02232A54
	cmp r0, #0
	bne _0225C586
	mov r0, #6
	add r4, #0x83
	add sp, #0x1d8
	strb r0, [r4]
	pop {r4, r5, r6, pc}
_0225C566:
	ldr r0, [r4]
	bl ov12_0223B750
	add r1, r4, #0
	add r1, #0x81
	ldrb r1, [r1]
	bl sub_02017068
	cmp r0, #1
	bne _0225C586
	ldr r0, [r4, #4]
	ldr r0, [r0, #0x20]
	bl Pokepic_IsAnimFinished
	cmp r0, #0
	beq _0225C588
_0225C586:
	b _0225C6C4
_0225C588:
	ldr r0, [r4, #0xc]
	bl ov07_02233ECC
	ldr r0, [r4, #8]
	bl ov07_02232AB8
	add r0, r4, #0
	add r0, #0x92
	ldrb r0, [r0]
	cmp r0, #0
	beq _0225C5D0
	add r0, r4, #0
	add r0, #0x81
	ldrb r1, [r0]
	mov r0, #0xb
	add r2, sp, #0x180
	str r0, [sp]
	str r1, [sp, #4]
	str r1, [sp, #8]
	mov r1, #0
	str r1, [sp, #0xc]
	ldr r0, [r4]
	mov r3, #1
	bl ov12_022643C8
	ldr r0, [r4]
	ldr r1, [r4, #4]
	add r2, r5, #0
	add r3, sp, #0x180
	bl ov12_02261B80
	mov r0, #7
	add r4, #0x83
	add sp, #0x1d8
	strb r0, [r4]
	pop {r4, r5, r6, pc}
_0225C5D0:
	mov r0, #8
	add r4, #0x83
	add sp, #0x1d8
	strb r0, [r4]
	pop {r4, r5, r6, pc}
_0225C5DA:
	bl ov07_0221C394
	add r0, r5, #0
	bl ov07_0221C3B0
	cmp r0, #0
	bne _0225C6C4
	add r0, r5, #0
	bl ov07_0221C3C0
	add r0, r4, #0
	add r0, #0x83
	ldrb r0, [r0]
	add r4, #0x83
	add sp, #0x1d8
	add r0, r0, #1
	strb r0, [r4]
	pop {r4, r5, r6, pc}
_0225C5FE:
	add r0, r4, #0
	add r0, #0x98
	ldr r0, [r0]
	cmp r0, #0
	beq _0225C640
	add r0, r4, #0
	add r0, #0x81
	ldrb r1, [r0]
	mov r0, #0xf
	add r2, sp, #0x128
	str r0, [sp]
	str r1, [sp, #4]
	str r1, [sp, #8]
	mov r1, #0
	str r1, [sp, #0xc]
	ldr r0, [r4]
	mov r3, #1
	bl ov12_022643C8
	ldr r0, [r4]
	ldr r1, [r4, #4]
	add r2, r5, #0
	add r3, sp, #0x128
	bl ov12_02261B80
	add r0, r4, #0
	add r0, #0x83
	ldrb r0, [r0]
	add r4, #0x83
	add sp, #0x1d8
	add r0, r0, #1
	strb r0, [r4]
	pop {r4, r5, r6, pc}
_0225C640:
	mov r0, #0xff
	add r4, #0x83
	add sp, #0x1d8
	strb r0, [r4]
	pop {r4, r5, r6, pc}
_0225C64A:
	add r3, r4, #0
	add r3, #0x81
	ldrb r3, [r3]
	add r1, r4, #0
	ldr r0, [r4]
	add r1, #0x28
	add r2, sp, #0xd8
	bl ov12_02261CA8
	add r0, sp, #0xd8
	mov r1, #5
	bl ov07_02234A20
	add r0, r4, #0
	add r0, #0x81
	ldrb r1, [r0]
	mov r0, #0x10
	add r2, sp, #0x80
	str r0, [sp]
	str r1, [sp, #4]
	str r1, [sp, #8]
	mov r1, #0
	str r1, [sp, #0xc]
	ldr r0, [r4]
	mov r3, #1
	bl ov12_022643C8
	ldr r0, [r4]
	ldr r1, [r4, #4]
	add r2, r5, #0
	add r3, sp, #0x80
	bl ov12_02261B80
	mov r0, #0x1a
	ldr r1, [r4, #4]
	mov r2, #1
	lsl r0, r0, #4
	str r2, [r1, r0]
	add r0, r4, #0
	add r0, #0x83
	ldrb r0, [r0]
	add r4, #0x83
	add sp, #0x1d8
	add r0, r0, #1
	strb r0, [r4]
	pop {r4, r5, r6, pc}
_0225C6A6:
	add r1, r4, #0
	add r2, r4, #0
	add r1, #0x81
	add r2, #0x80
	ldrb r1, [r1]
	ldrb r2, [r2]
	ldr r0, [r4]
	bl ov12_0226430C
	add r0, r4, #0
	bl Heap_Free
	add r0, r6, #0
	bl SysTask_Destroy
_0225C6C4:
	add sp, #0x1d8
	pop {r4, r5, r6, pc}
	thumb_func_end ov12_0225C18C

	thumb_func_start ov12_0225C6C8
ov12_0225C6C8: ; 0x0225C6C8
	push {r3, r4, r5, r6, lr}
	sub sp, #0x1fc
	add r4, r1, #0
	add r6, r0, #0
	ldr r0, [r4]
	bl ov12_0223A8DC
	add r1, r4, #0
	add r1, #0x83
	ldrb r1, [r1]
	add r5, r0, #0
	cmp r1, #9
	bls _0225C6E4
	b _0225C98C
_0225C6E4:
	add r1, r1, r1
	add r1, pc
	ldrh r1, [r1, #6]
	lsl r1, r1, #0x10
	asr r1, r1, #0x10
	add pc, r1
_0225C6F0: ; jump table
	.short _0225C704 - _0225C6F0 - 2 ; case 0
	.short _0225C7C2 - _0225C6F0 - 2 ; case 1
	.short _0225C8C0 - _0225C6F0 - 2 ; case 2
	.short _0225C7FA - _0225C6F0 - 2 ; case 3
	.short _0225C858 - _0225C6F0 - 2 ; case 4
	.short _0225C8C0 - _0225C6F0 - 2 ; case 5
	.short _0225C8E4 - _0225C6F0 - 2 ; case 6
	.short _0225C8C0 - _0225C6F0 - 2 ; case 7
	.short _0225C930 - _0225C6F0 - 2 ; case 8
	.short _0225C8C0 - _0225C6F0 - 2 ; case 9
_0225C704:
	ldr r0, [r4]
	bl BattleSystem_GetPokepicManager
	add r2, r4, #0
	add r3, r4, #0
	add r5, r0, #0
	mov r0, #0x69
	add r2, #0x86
	add r3, #0x82
	ldr r1, [r4, #4]
	lsl r0, r0, #2
	ldr r0, [r1, r0]
	ldrh r2, [r2]
	ldrb r3, [r3]
	add r1, sp, #0x24
	bl NARC_ReadPokepicAnimScript
	add r0, r4, #0
	add r0, #0x82
	ldrb r1, [r0]
	mov r0, #6
	add r2, r4, #0
	add r3, r1, #0
	mul r3, r0
	ldr r0, _0225C9B0 ; =ov07_022377F4 + 38
	add r1, r5, #0
	ldrsh r0, [r0, r3]
	ldr r5, _0225C9B4 ; =ov07_022377F4 + 36
	add r2, #0x14
	str r0, [sp]
	ldr r0, _0225C9B8 ; =ov07_022377F4 + 4
	ldrsh r0, [r0, r3]
	ldrsh r3, [r5, r3]
	str r0, [sp, #4]
	add r0, r4, #0
	add r0, #0x85
	ldrb r0, [r0]
	str r0, [sp, #8]
	mov r0, #0x90
	ldrsb r0, [r4, r0]
	str r0, [sp, #0xc]
	mov r0, #0x91
	ldrsb r0, [r4, r0]
	str r0, [sp, #0x10]
	add r0, r4, #0
	add r0, #0x93
	ldrb r0, [r0]
	str r0, [sp, #0x14]
	add r0, r4, #0
	add r0, #0x81
	ldrb r0, [r0]
	str r0, [sp, #0x18]
	add r0, sp, #0x24
	str r0, [sp, #0x1c]
	mov r0, #0
	str r0, [sp, #0x20]
	ldr r0, [r4]
	bl ov12_022612A4
	ldr r1, [r4, #4]
	str r0, [r1, #0x20]
	ldr r0, [r4, #4]
	mov r1, #0xc
	add r2, r1, #0
	ldr r0, [r0, #0x20]
	add r2, #0xf4
	bl Pokepic_SetAttr
	ldr r0, [r4, #4]
	mov r1, #0xd
	add r2, r1, #0
	ldr r0, [r0, #0x20]
	add r2, #0xf3
	bl Pokepic_SetAttr
	ldr r0, [r4, #4]
	mov r1, #0x2c
	ldr r0, [r0, #0x20]
	mov r2, #0
	bl Pokepic_SetAttr
	ldr r0, [r4, #4]
	mov r1, #6
	ldr r0, [r0, #0x20]
	mov r2, #0
	bl Pokepic_SetAttr
	add r0, r4, #0
	add r0, #0x83
	ldrb r0, [r0]
	add r4, #0x83
	add sp, #0x1fc
	add r0, r0, #1
	strb r0, [r4]
	pop {r3, r4, r5, r6, pc}
_0225C7C2:
	add r0, r4, #0
	add r0, #0x81
	ldrb r1, [r0]
	mov r0, #0x29
	add r2, sp, #0x1a4
	str r0, [sp]
	str r1, [sp, #4]
	str r1, [sp, #8]
	mov r1, #0
	str r1, [sp, #0xc]
	ldr r0, [r4]
	mov r3, #1
	bl ov12_022643C8
	ldr r0, [r4]
	ldr r1, [r4, #4]
	add r2, r5, #0
	add r3, sp, #0x1a4
	bl ov12_02261B80
	add r0, r4, #0
	add r0, #0x83
	ldrb r0, [r0]
	add r4, #0x83
	add sp, #0x1fc
	add r0, r0, #1
	strb r0, [r4]
	pop {r3, r4, r5, r6, pc}
_0225C7FA:
	add r0, r4, #0
	add r0, #0x84
	ldrb r0, [r0]
	cmp r0, #2
	bne _0225C810
	ldr r0, [r4, #4]
	mov r1, #0x2d
	ldr r0, [r0, #0x20]
	mov r2, #0
	bl Pokepic_SetAttr
_0225C810:
	mov r0, #0x69
	ldr r3, [r4, #4]
	lsl r0, r0, #2
	ldr r0, [r3, r0]
	add r1, r4, #0
	str r0, [sp]
	add r0, r4, #0
	add r0, #0x86
	ldrh r0, [r0]
	add r2, r4, #0
	add r1, #0x81
	str r0, [sp, #4]
	add r0, r4, #0
	add r0, #0x97
	ldrb r0, [r0]
	add r2, #0x82
	str r0, [sp, #8]
	add r0, r4, #0
	add r0, #0x84
	ldrb r0, [r0]
	str r0, [sp, #0xc]
	add r0, r4, #0
	add r0, #0x88
	ldr r0, [r0]
	str r0, [sp, #0x10]
	ldrb r1, [r1]
	ldrb r2, [r2]
	ldr r0, [r4]
	ldr r3, [r3, #0x20]
	bl ov12_02261F38
	mov r0, #4
	add r4, #0x83
	add sp, #0x1fc
	strb r0, [r4]
	pop {r3, r4, r5, r6, pc}
_0225C858:
	ldr r0, [r4]
	bl ov12_0223B750
	add r1, r4, #0
	add r1, #0x81
	ldrb r1, [r1]
	bl sub_02017068
	cmp r0, #1
	bne _0225C878
	ldr r0, [r4, #4]
	ldr r0, [r0, #0x20]
	bl Pokepic_IsAnimFinished
	cmp r0, #0
	beq _0225C87A
_0225C878:
	b _0225C9AA
_0225C87A:
	add r0, r4, #0
	add r0, #0x92
	ldrb r0, [r0]
	cmp r0, #0
	beq _0225C8B6
	add r0, r4, #0
	add r0, #0x81
	ldrb r1, [r0]
	mov r0, #0xb
	add r2, sp, #0x14c
	str r0, [sp]
	str r1, [sp, #4]
	str r1, [sp, #8]
	mov r1, #0
	str r1, [sp, #0xc]
	ldr r0, [r4]
	mov r3, #1
	bl ov12_022643C8
	ldr r0, [r4]
	ldr r1, [r4, #4]
	add r2, r5, #0
	add r3, sp, #0x14c
	bl ov12_02261B80
	mov r0, #5
	add r4, #0x83
	add sp, #0x1fc
	strb r0, [r4]
	pop {r3, r4, r5, r6, pc}
_0225C8B6:
	mov r0, #6
	add r4, #0x83
	add sp, #0x1fc
	strb r0, [r4]
	pop {r3, r4, r5, r6, pc}
_0225C8C0:
	bl ov07_0221C394
	add r0, r5, #0
	bl ov07_0221C3B0
	cmp r0, #0
	bne _0225C9AA
	add r0, r5, #0
	bl ov07_0221C3C0
	add r0, r4, #0
	add r0, #0x83
	ldrb r0, [r0]
	add r4, #0x83
	add sp, #0x1fc
	add r0, r0, #1
	strb r0, [r4]
	pop {r3, r4, r5, r6, pc}
_0225C8E4:
	add r0, r4, #0
	add r0, #0x98
	ldr r0, [r0]
	cmp r0, #0
	beq _0225C926
	add r0, r4, #0
	add r0, #0x81
	ldrb r1, [r0]
	mov r0, #0xf
	add r2, sp, #0xf4
	str r0, [sp]
	str r1, [sp, #4]
	str r1, [sp, #8]
	mov r1, #0
	str r1, [sp, #0xc]
	ldr r0, [r4]
	mov r3, #1
	bl ov12_022643C8
	ldr r0, [r4]
	ldr r1, [r4, #4]
	add r2, r5, #0
	add r3, sp, #0xf4
	bl ov12_02261B80
	add r0, r4, #0
	add r0, #0x83
	ldrb r0, [r0]
	add r4, #0x83
	add sp, #0x1fc
	add r0, r0, #1
	strb r0, [r4]
	pop {r3, r4, r5, r6, pc}
_0225C926:
	mov r0, #0xff
	add r4, #0x83
	add sp, #0x1fc
	strb r0, [r4]
	pop {r3, r4, r5, r6, pc}
_0225C930:
	add r3, r4, #0
	add r3, #0x81
	ldrb r3, [r3]
	add r1, r4, #0
	ldr r0, [r4]
	add r1, #0x28
	add r2, sp, #0xa4
	bl ov12_02261CA8
	add r0, sp, #0xa4
	mov r1, #5
	bl ov07_02234A20
	add r0, r4, #0
	add r0, #0x81
	ldrb r1, [r0]
	mov r0, #0x10
	add r2, sp, #0x4c
	str r0, [sp]
	str r1, [sp, #4]
	str r1, [sp, #8]
	mov r1, #0
	str r1, [sp, #0xc]
	ldr r0, [r4]
	mov r3, #1
	bl ov12_022643C8
	ldr r0, [r4]
	ldr r1, [r4, #4]
	add r2, r5, #0
	add r3, sp, #0x4c
	bl ov12_02261B80
	mov r0, #0x1a
	ldr r1, [r4, #4]
	mov r2, #1
	lsl r0, r0, #4
	str r2, [r1, r0]
	add r0, r4, #0
	add r0, #0x83
	ldrb r0, [r0]
	add r4, #0x83
	add sp, #0x1fc
	add r0, r0, #1
	strb r0, [r4]
	pop {r3, r4, r5, r6, pc}
_0225C98C:
	add r1, r4, #0
	add r2, r4, #0
	add r1, #0x81
	add r2, #0x80
	ldrb r1, [r1]
	ldrb r2, [r2]
	ldr r0, [r4]
	bl ov12_0226430C
	add r0, r4, #0
	bl Heap_Free
	add r0, r6, #0
	bl SysTask_Destroy
_0225C9AA:
	add sp, #0x1fc
	pop {r3, r4, r5, r6, pc}
	nop
_0225C9B0: .word ov07_022377F4 + 38
_0225C9B4: .word ov07_022377F4 + 36
_0225C9B8: .word ov07_022377F4 + 4
	thumb_func_end ov12_0225C6C8

	thumb_func_start ov12_0225C9BC
ov12_0225C9BC: ; 0x0225C9BC
	push {r4, r5, r6, lr}
	sub sp, #0x178
	add r4, r1, #0
	add r6, r0, #0
	ldr r0, [r4]
	bl ov12_0223A8DC
	add r1, r4, #0
	add r1, #0x6b
	ldrb r1, [r1]
	add r5, r0, #0
	cmp r1, #8
	bhi _0225CAA6
	add r1, r1, r1
	add r1, pc
	ldrh r1, [r1, #6]
	lsl r1, r1, #0x10
	asr r1, r1, #0x10
	add pc, r1
_0225C9E2: ; jump table
	.short _0225C9F4 - _0225C9E2 - 2 ; case 0
	.short _0225CA98 - _0225C9E2 - 2 ; case 1
	.short _0225CA3C - _0225C9E2 - 2 ; case 2
	.short _0225CA98 - _0225C9E2 - 2 ; case 3
	.short _0225CABE - _0225C9E2 - 2 ; case 4
	.short _0225CBB0 - _0225C9E2 - 2 ; case 5
	.short _0225CBCA - _0225C9E2 - 2 ; case 6
	.short _0225CC0A - _0225C9E2 - 2 ; case 7
	.short _0225CC24 - _0225C9E2 - 2 ; case 8
_0225C9F4:
	ldr r0, [r4, #0x70]
	cmp r0, #0
	beq _0225CA32
	add r0, r4, #0
	add r0, #0x69
	ldrb r1, [r0]
	mov r0, #0xf
	add r2, sp, #0x120
	str r0, [sp]
	str r1, [sp, #4]
	str r1, [sp, #8]
	mov r1, #0
	str r1, [sp, #0xc]
	ldr r0, [r4]
	mov r3, #1
	bl ov12_022643C8
	ldr r0, [r4]
	ldr r1, [r4, #4]
	add r2, r5, #0
	add r3, sp, #0x120
	bl ov12_02261B80
	add r0, r4, #0
	add r0, #0x6b
	ldrb r0, [r0]
	add r4, #0x6b
	add sp, #0x178
	add r0, r0, #1
	strb r0, [r4]
	pop {r4, r5, r6, pc}
_0225CA32:
	mov r0, #4
	add r4, #0x6b
	add sp, #0x178
	strb r0, [r4]
	pop {r4, r5, r6, pc}
_0225CA3C:
	add r3, r4, #0
	add r3, #0x69
	ldrb r3, [r3]
	add r1, r4, #0
	ldr r0, [r4]
	add r1, #0x10
	add r2, sp, #0xd0
	bl ov12_02261CA8
	add r0, sp, #0xd0
	mov r1, #5
	bl ov07_0223494C
	add r0, r4, #0
	add r0, #0x69
	ldrb r1, [r0]
	mov r0, #0x10
	add r2, sp, #0x78
	str r0, [sp]
	str r1, [sp, #4]
	str r1, [sp, #8]
	mov r1, #0
	str r1, [sp, #0xc]
	ldr r0, [r4]
	mov r3, #1
	bl ov12_022643C8
	ldr r0, [r4]
	ldr r1, [r4, #4]
	add r2, r5, #0
	add r3, sp, #0x78
	bl ov12_02261B80
	mov r0, #0x1a
	ldr r1, [r4, #4]
	mov r2, #0
	lsl r0, r0, #4
	str r2, [r1, r0]
	add r0, r4, #0
	add r0, #0x6b
	ldrb r0, [r0]
	add r4, #0x6b
	add sp, #0x178
	add r0, r0, #1
	strb r0, [r4]
	pop {r4, r5, r6, pc}
_0225CA98:
	bl ov07_0221C394
	add r0, r5, #0
	bl ov07_0221C3B0
	cmp r0, #0
	beq _0225CAA8
_0225CAA6:
	b _0225CC42
_0225CAA8:
	add r0, r5, #0
	bl ov07_0221C3C0
	add r0, r4, #0
	add r0, #0x6b
	ldrb r0, [r0]
	add r4, #0x6b
	add sp, #0x178
	add r0, r0, #1
	strb r0, [r4]
	pop {r4, r5, r6, pc}
_0225CABE:
	add r0, r4, #0
	add r0, #0x6a
	ldrb r5, [r0]
	mov r0, #1
	tst r0, r5
	beq _0225CB06
	ldr r0, _0225CC48 ; =ov12_0226D120
	ldrb r0, [r0, r5]
	str r0, [sp, #0x50]
	mov r0, #5
	str r0, [sp, #0x54]
	str r0, [sp, #0x58]
	add r0, r4, #0
	add r0, #0x69
	ldrb r0, [r0]
	str r0, [sp, #0x5c]
	add r0, r4, #0
	add r0, #0x6e
	ldrh r0, [r0]
	str r0, [sp, #0x60]
	ldr r0, [r4]
	bl BattleSystem_GetSpriteSystem
	str r0, [sp, #0x6c]
	ldr r0, [r4]
	bl BattleSystem_GetPaletteData
	str r0, [sp, #0x70]
	mov r0, #1
	str r0, [sp, #0x64]
	mov r0, #0
	str r0, [sp, #0x68]
	add r0, sp, #0x50
	bl ov07_02233DB8
	b _0225CB52
_0225CB06:
	ldr r3, _0225CC4C ; =ov12_0226D128
	add r2, sp, #0x10
	add r6, r2, #0
	ldmia r3!, {r0, r1}
	stmia r2!, {r0, r1}
	ldmia r3!, {r0, r1}
	stmia r2!, {r0, r1}
	ldmia r3!, {r0, r1}
	stmia r2!, {r0, r1}
	lsl r0, r5, #2
	ldr r0, [r6, r0]
	str r0, [sp, #0x28]
	mov r0, #5
	str r0, [sp, #0x2c]
	str r0, [sp, #0x30]
	add r0, r4, #0
	add r0, #0x69
	ldrb r0, [r0]
	str r0, [sp, #0x34]
	add r0, r4, #0
	add r0, #0x6e
	ldrh r0, [r0]
	str r0, [sp, #0x38]
	ldr r0, [r4]
	bl BattleSystem_GetSpriteSystem
	str r0, [sp, #0x44]
	ldr r0, [r4]
	bl BattleSystem_GetPaletteData
	str r0, [sp, #0x48]
	mov r0, #1
	str r0, [sp, #0x3c]
	mov r0, #0
	str r0, [sp, #0x40]
	add r0, sp, #0x28
	bl ov07_02233DB8
_0225CB52:
	str r0, [r4, #0xc]
	add r0, r4, #0
	mov r1, #5
	add r0, #0x6b
	strb r1, [r0]
	add r0, r4, #0
	add r0, #0x6e
	ldrh r0, [r0]
	mov r2, #0x10
	lsl r1, r0, #1
	ldr r0, _0225CC50 ; =ov12_0226D15A
	ldrh r0, [r0, r1]
	mov r1, #0
	add r3, r1, #0
	str r0, [sp]
	ldr r0, [r4, #8]
	bl Pokepic_StartPaletteFade
	add r4, #0x6a
	ldrb r0, [r4]
	cmp r0, #5
	bhi _0225CC42
	add r0, r0, r0
	add r0, pc
	ldrh r0, [r0, #6]
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	add pc, r0
_0225CB8A: ; jump table
	.short _0225CB96 - _0225CB8A - 2 ; case 0
	.short _0225CBA4 - _0225CB8A - 2 ; case 1
	.short _0225CB96 - _0225CB8A - 2 ; case 2
	.short _0225CBA4 - _0225CB8A - 2 ; case 3
	.short _0225CB96 - _0225CB8A - 2 ; case 4
	.short _0225CBA4 - _0225CB8A - 2 ; case 5
_0225CB96:
	mov r1, #0x74
	ldr r0, _0225CC54 ; =0x00000706
	mvn r1, r1
	bl sub_0200602C
	add sp, #0x178
	pop {r4, r5, r6, pc}
_0225CBA4:
	ldr r0, _0225CC54 ; =0x00000706
	mov r1, #0x75
	bl sub_0200602C
	add sp, #0x178
	pop {r4, r5, r6, pc}
_0225CBB0:
	ldr r0, [r4, #8]
	bl Pokepic_ResumePaletteFade
	cmp r0, #0
	bne _0225CC42
	add r0, r4, #0
	add r0, #0x6b
	ldrb r0, [r0]
	add r4, #0x6b
	add sp, #0x178
	add r0, r0, #1
	strb r0, [r4]
	pop {r4, r5, r6, pc}
_0225CBCA:
	mov r1, #0xc
	add r2, r1, #0
	ldr r0, [r4, #8]
	sub r2, #0x2c
	bl Pokepic_AddAttr
	mov r1, #0xd
	add r2, r1, #0
	ldr r0, [r4, #8]
	sub r2, #0x2d
	bl Pokepic_AddAttr
	add r1, r4, #0
	add r1, #0x6c
	ldrb r1, [r1]
	ldr r0, [r4, #8]
	bl sub_0200914C
	ldr r0, [r4, #8]
	mov r1, #0xc
	bl Pokepic_GetAttr
	cmp r0, #0
	bgt _0225CC42
	ldr r0, [r4, #8]
	bl Pokepic_Delete
	mov r0, #7
	add r4, #0x6b
	add sp, #0x178
	strb r0, [r4]
	pop {r4, r5, r6, pc}
_0225CC0A:
	ldr r0, [r4, #0xc]
	bl ov07_02233E88
	cmp r0, #0
	bne _0225CC42
	ldr r0, [r4, #0xc]
	bl ov07_02233ECC
	mov r0, #8
	add r4, #0x6b
	add sp, #0x178
	strb r0, [r4]
	pop {r4, r5, r6, pc}
_0225CC24:
	add r1, r4, #0
	add r2, r4, #0
	add r1, #0x69
	add r2, #0x68
	ldrb r1, [r1]
	ldrb r2, [r2]
	ldr r0, [r4]
	bl ov12_0226430C
	add r0, r4, #0
	bl Heap_Free
	add r0, r6, #0
	bl SysTask_Destroy
_0225CC42:
	add sp, #0x178
	pop {r4, r5, r6, pc}
	nop
_0225CC48: .word ov12_0226D120
_0225CC4C: .word ov12_0226D128
_0225CC50: .word ov12_0226D15A
_0225CC54: .word 0x00000706
	thumb_func_end ov12_0225C9BC

	thumb_func_start ov12_0225CC58
ov12_0225CC58: ; 0x0225CC58
	push {r4, r5, r6, lr}
	sub sp, #0x168
	add r4, r1, #0
	add r6, r0, #0
	ldr r0, [r4]
	bl ov12_0223A8DC
	add r1, r4, #0
	add r1, #0x6b
	ldrb r1, [r1]
	add r5, r0, #0
	cmp r1, #7
	bls _0225CC74
	b _0225CDB4
_0225CC74:
	add r1, r1, r1
	add r1, pc
	ldrh r1, [r1, #6]
	lsl r1, r1, #0x10
	asr r1, r1, #0x10
	add pc, r1
_0225CC80: ; jump table
	.short _0225CC90 - _0225CC80 - 2 ; case 0
	.short _0225CD34 - _0225CC80 - 2 ; case 1
	.short _0225CCD8 - _0225CC80 - 2 ; case 2
	.short _0225CD34 - _0225CC80 - 2 ; case 3
	.short _0225CD58 - _0225CC80 - 2 ; case 4
	.short _0225CD34 - _0225CC80 - 2 ; case 5
	.short _0225CD90 - _0225CC80 - 2 ; case 6
	.short _0225CD96 - _0225CC80 - 2 ; case 7
_0225CC90:
	ldr r0, [r4, #0x70]
	cmp r0, #0
	beq _0225CCCE
	add r0, r4, #0
	add r0, #0x69
	ldrb r1, [r0]
	mov r0, #0xf
	add r2, sp, #0x110
	str r0, [sp]
	str r1, [sp, #4]
	str r1, [sp, #8]
	mov r1, #0
	str r1, [sp, #0xc]
	ldr r0, [r4]
	mov r3, #1
	bl ov12_022643C8
	ldr r0, [r4]
	ldr r1, [r4, #4]
	add r2, r5, #0
	add r3, sp, #0x110
	bl ov12_02261B80
	add r0, r4, #0
	add r0, #0x6b
	ldrb r0, [r0]
	add r4, #0x6b
	add sp, #0x168
	add r0, r0, #1
	strb r0, [r4]
	pop {r4, r5, r6, pc}
_0225CCCE:
	mov r0, #4
	add r4, #0x6b
	add sp, #0x168
	strb r0, [r4]
	pop {r4, r5, r6, pc}
_0225CCD8:
	add r3, r4, #0
	add r3, #0x69
	ldrb r3, [r3]
	add r1, r4, #0
	ldr r0, [r4]
	add r1, #0x10
	add r2, sp, #0xc0
	bl ov12_02261CA8
	add r0, sp, #0xc0
	mov r1, #5
	bl ov07_0223494C
	add r0, r4, #0
	add r0, #0x69
	ldrb r1, [r0]
	mov r0, #0x10
	add r2, sp, #0x68
	str r0, [sp]
	str r1, [sp, #4]
	str r1, [sp, #8]
	mov r1, #0
	str r1, [sp, #0xc]
	ldr r0, [r4]
	mov r3, #1
	bl ov12_022643C8
	ldr r0, [r4]
	ldr r1, [r4, #4]
	add r2, r5, #0
	add r3, sp, #0x68
	bl ov12_02261B80
	mov r0, #0x1a
	ldr r1, [r4, #4]
	mov r2, #0
	lsl r0, r0, #4
	str r2, [r1, r0]
	add r0, r4, #0
	add r0, #0x6b
	ldrb r0, [r0]
	add r4, #0x6b
	add sp, #0x168
	add r0, r0, #1
	strb r0, [r4]
	pop {r4, r5, r6, pc}
_0225CD34:
	bl ov07_0221C394
	add r0, r5, #0
	bl ov07_0221C3B0
	cmp r0, #0
	bne _0225CDB4
	add r0, r5, #0
	bl ov07_0221C3C0
	add r0, r4, #0
	add r0, #0x6b
	ldrb r0, [r0]
	add r4, #0x6b
	add sp, #0x168
	add r0, r0, #1
	strb r0, [r4]
	pop {r4, r5, r6, pc}
_0225CD58:
	add r0, r4, #0
	add r0, #0x69
	ldrb r1, [r0]
	mov r0, #0x2a
	add r2, sp, #0x10
	str r0, [sp]
	str r1, [sp, #4]
	str r1, [sp, #8]
	mov r1, #0
	str r1, [sp, #0xc]
	ldr r0, [r4]
	mov r3, #1
	bl ov12_022643C8
	ldr r0, [r4]
	ldr r1, [r4, #4]
	add r2, r5, #0
	add r3, sp, #0x10
	bl ov12_02261B80
	add r0, r4, #0
	add r0, #0x6b
	ldrb r0, [r0]
	add r4, #0x6b
	add sp, #0x168
	add r0, r0, #1
	strb r0, [r4]
	pop {r4, r5, r6, pc}
_0225CD90:
	ldr r0, [r4, #8]
	bl Pokepic_Delete
_0225CD96:
	add r1, r4, #0
	add r2, r4, #0
	add r1, #0x69
	add r2, #0x68
	ldrb r1, [r1]
	ldrb r2, [r2]
	ldr r0, [r4]
	bl ov12_0226430C
	add r0, r4, #0
	bl Heap_Free
	add r0, r6, #0
	bl SysTask_Destroy
_0225CDB4:
	add sp, #0x168
	pop {r4, r5, r6, pc}
	thumb_func_end ov12_0225CC58
