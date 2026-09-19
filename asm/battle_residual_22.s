#include "constants/pokemon.h"
#include "constants/sndseq.h"
	.include "asm/macros.inc"
	.include "overlay_12_battle_controller_opponent.inc"
	.include "global.inc"
	.public ov12_02258BA0

	.text
	.public ov12_02258800
	.public ov12_02258BA0
	.public ov12_02258BB4
	.public ov12_02258D74
	.public ov12_02258DB0
	.public ov12_02258E54
	.public ov12_02258E7C
	.public ov12_02258EB0
	.public ov12_02258EB4
	.public ov12_02258EE0
	.public ov12_02258EF4
	.public ov12_02258F08
	.public ov12_02258F1C
	.public ov12_02258F30
	.public ov12_02258F44
	.public ov12_02258F68
	.public ov12_02258F7C
	.public ov12_02258F90
	.public ov12_02258FA0
	.public ov12_02258FB4
	.public ov12_02258FC8
	.public ov12_02258FD8
	.public ov12_02259000
	.public ov12_02259014
	.public ov12_02259028
	.public ov12_0225903C
	.public ov12_02259050
	.public ov12_02259064
	.public ov12_02259078
	.public ov12_0225908C
	.public ov12_022590A0
	.public ov12_022590D4
	.public ov12_022590E8
	.public ov12_022590FC
	.public ov12_02259110
	.public ov12_02259124
	.public ov12_02259134
	.public ov12_02259148
	.public ov12_0225915C
	.public ov12_02259170
	.public ov12_02259184
	.public ov12_02259198
	.public ov12_022591A8
	.public ov12_022591BC
	.public ov12_022591CC
	.public ov12_022591E0
	.public ov12_022591F4
	.public ov12_022592D0
	.public ov12_02259328
	.public ov12_02259358
	.public ov12_022593D4
	.public ov12_022593E8
	.public ov12_022593FC
	.public ov12_022594F4
	.public ov12_02259514
	.public ov12_022595B8
	.public ov12_022595CC
	.public ov12_022595E0
	.public ov12_0225961C
	.public ov12_02259658
	.public ov12_02259694
	.public ov12_022596B8
	.public ov12_02259700
	.public ov12_02259724
	.public ov12_02259738
	.public ov12_02259748
	.public ov12_02259758
	.public ov12_02259768
	.public ov12_0225978C
	.public ov12_022597B0
	.public ov12_022597C4
	.public ov12_022597D8
	.public ov12_022597EC
	.public ov12_022598F8
	.public ov12_02259928
	.public ov12_02259930
	.public ov12_02259944
	.public ov12_02259968
	.public ov12_02259BA8
	.public ov12_02259D48
	.public ov12_02259F30
	.public ov12_0225A018
	.public ov12_0225A07C
	.public ov12_0225A2A0
	.public ov12_0225A334
	.public ov12_0225A37C
	.public ov12_0225A414
	.public ov12_0225A4DC
	.public ov12_0225A524
	.public ov12_0225A604
	.public ov12_0225A674
	.public ov12_0225A700
	.public ov12_0225A7AC
	.public ov12_0225A818
	.public ov12_0225A85C
	.public ov12_0225A8C4
	.public ov12_0225A914
	.public ov12_0225A9B0
	.public ov12_0225A9E0
	.public ov12_0225AA6C
	.public ov12_0225AAE0
	.public ov12_0225ABB8
	.public ov12_0225ABE8
	.public ov12_0225AC1C
	.public ov12_0225ACB0
	.public ov12_0225ACE8
	.public ov12_0225AD44
	.public ov12_0225AD9C
	.public ov12_0225ADF4
	.public ov12_0225AE48
	.public ov12_0225AEA0
	.public ov12_0225AED8
	.public ov12_0225AF74
	.public ov12_0225B028
	.public ov12_0225B060
	.public ov12_0225B0A0
	.public ov12_0225B0E8
	.public ov12_0225B120
	.public ov12_0225B16C
	.public ov12_0225B1A8
	.public ov12_0225B200
	.public ov12_0225B234
	.public ov12_0225B26C
	.public ov12_0225B2A4
	.public ov12_0225B2F8
	.public ov12_0225B34C
	.public ov12_0225B3A0
	.public ov12_0225B3D4
	.public ov12_0225B434
	.public ov12_0225B454
	.public ov12_0225B494
	.public ov12_0225B7B8
	.public ov12_0225B960
	.public ov12_0225BE38
	.public ov12_0225C18C
	.public ov12_0225C6C8
	.public ov12_0225C9BC
	.public ov12_0225CC58
	.public ov12_0225CDB8
	.public ov12_0225CE28
	.public ov12_0225D138
	.public ov12_0225D644
	.public ov12_0225D890
	.public ov12_0225D990
	.public ov12_0225DA18
	.public ov12_0225DA8C
	.public ov12_0225DAD4
	.public ov12_0225E104
	.public ov12_0225E134
	.public ov12_0225E154
	.public ov12_0225E1D4
	.public ov12_0225E1FC
	.public ov12_0225E250
	.public ov12_0225E404
	.public ov12_0225E4CC
	.public ov12_0225E4EC
	.public ov12_0225E568
	.public ov12_0225E6FC
	.public ov12_0225E740
	.public ov12_0225E760
	.public ov12_0225E830
	.public ov12_0225F3A4
	.public ov12_0225F3FC
	.public ov12_0225F434
	.public ov12_0225F4E0
	.public ov12_0225F8AC
	.public ov12_0225F960
	.public ov12_0225F980
	.public ov12_0225FA44
	.public ov12_0225FC80
	.public ov12_0225FCA0
	.public ov12_0225FCC0
	.public ov12_0225FD14
	.public ov12_0225FF80
	.public ov12_0225FFDC
	.public ov12_02260030
	.public ov12_022600F0
	.public ov12_0226037C
	.public ov12_02260418
	.public ov12_02260584
	.public ov12_022605D0
	.public ov12_02260614
	.public ov12_02260668
	.public ov12_022609F8
	.public ov12_02260B30
	.public ov12_02260BA0
	.public ov12_02260C58
	.public ov12_02260CDC
	.public ov12_02260D28
	.public ov12_02260D84
	.public ov12_02260EA4
	.public ov12_02261258
	.public ov12_02261264
	.public ov12_02261270
	.public ov12_02261280
	.public ov12_02261284
	.public ov12_02261294
	.public ov12_022612A4
	.public ov12_02261390
	.public ov12_02261464
	.public ov12_02261544
	.public ov12_022615F0
	.public ov12_02261928
	.public ov12_022619E4
	.public ov12_02261AD4
	.public ov12_02261B2C
	.public ov12_02261B80
	.public ov12_02261CA8
	.public ov12_02261D30
	.public ov12_02261DC8
	.public ov12_02261E40
	.public ov12_02261EB8
	.public ov12_02261ED4
	.public ov12_02261EF0
	.public ov12_02261F38
	.public ov12_0226D010
	.public ov12_0226D120
	.public ov12_0226D128
	.public ov12_0226D140
	.public ov12_0226D141
	.public ov12_0226D15A
	.public ov12_0226D18C
	.public ov12_0226D1E8
	.public ov12_0226D1EA
	.public ov12_0226D350

	thumb_func_start ov12_022600F0
ov12_022600F0: ; 0x022600F0
	push {r3, r4, r5, r6, lr}
	sub sp, #0x114
	add r4, r1, #0
	add r6, r0, #0
	ldr r0, [r4]
	bl ov12_0223A8DC
	add r1, r4, #0
	add r1, #0x66
	ldrb r1, [r1]
	add r5, r0, #0
	cmp r1, #0xa
	bls _0226010C
	b _02260374
_0226010C:
	add r1, r1, r1
	add r1, pc
	ldrh r1, [r1, #6]
	lsl r1, r1, #0x10
	asr r1, r1, #0x10
	add pc, r1
_02260118: ; jump table
	.short _0226012E - _02260118 - 2 ; case 0
	.short _022601D6 - _02260118 - 2 ; case 1
	.short _0226017A - _02260118 - 2 ; case 2
	.short _022601D6 - _02260118 - 2 ; case 3
	.short _022601FA - _02260118 - 2 ; case 4
	.short _0226024C - _02260118 - 2 ; case 5
	.short _02260266 - _02260118 - 2 ; case 6
	.short _022602A2 - _02260118 - 2 ; case 7
	.short _022602CE - _02260118 - 2 ; case 8
	.short _0226030E - _02260118 - 2 ; case 9
	.short _02260356 - _02260118 - 2 ; case 10
_0226012E:
	add r0, r4, #0
	add r0, #0x70
	ldrh r0, [r0]
	cmp r0, #0
	beq _02260170
	add r0, r4, #0
	add r0, #0x65
	ldrb r1, [r0]
	mov r0, #0xf
	add r2, sp, #0xbc
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
	add r3, sp, #0xbc
	bl ov12_02261B80
	add r0, r4, #0
	add r0, #0x66
	ldrb r0, [r0]
	add r4, #0x66
	add sp, #0x114
	add r0, r0, #1
	strb r0, [r4]
	pop {r3, r4, r5, r6, pc}
_02260170:
	mov r0, #4
	add r4, #0x66
	add sp, #0x114
	strb r0, [r4]
	pop {r3, r4, r5, r6, pc}
_0226017A:
	add r3, r4, #0
	add r3, #0x65
	ldrb r3, [r3]
	add r1, r4, #0
	ldr r0, [r4]
	add r1, #0xc
	add r2, sp, #0x6c
	bl ov12_02261CA8
	add r0, sp, #0x6c
	mov r1, #5
	bl ov07_0223494C
	add r0, r4, #0
	add r0, #0x65
	ldrb r1, [r0]
	mov r0, #0x10
	add r2, sp, #0x14
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
	add r3, sp, #0x14
	bl ov12_02261B80
	mov r0, #0x1a
	ldr r1, [r4, #4]
	mov r2, #0
	lsl r0, r0, #4
	str r2, [r1, r0]
	add r0, r4, #0
	add r0, #0x66
	ldrb r0, [r0]
	add r4, #0x66
	add sp, #0x114
	add r0, r0, #1
	strb r0, [r4]
	pop {r3, r4, r5, r6, pc}
_022601D6:
	bl ov07_0221C394
	add r0, r5, #0
	bl ov07_0221C3B0
	cmp r0, #0
	bne _02260254
	add r0, r5, #0
	bl ov07_0221C3C0
	add r0, r4, #0
	add r0, #0x66
	ldrb r0, [r0]
	add r4, #0x66
	add sp, #0x114
	add r0, r0, #1
	strb r0, [r4]
	pop {r3, r4, r5, r6, pc}
_022601FA:
	add r0, r4, #0
	add r0, #0x67
	ldrb r0, [r0]
	cmp r0, #2
	bne _02260208
	mov r5, #0x75
	b _0226020C
_02260208:
	mov r5, #0x74
	mvn r5, r5
_0226020C:
	add r1, r4, #0
	add r1, #0x65
	ldrb r1, [r1]
	ldr r0, [r4]
	bl BattleSystem_GetChatotVoice
	str r5, [sp]
	mov r1, #0x7f
	str r1, [sp, #4]
	add r1, r4, #0
	add r1, #0x72
	ldrh r1, [r1]
	add r3, r4, #0
	mov r2, #0
	str r1, [sp, #8]
	mov r1, #5
	str r1, [sp, #0xc]
	str r2, [sp, #0x10]
	add r2, r4, #0
	add r2, #0x68
	add r3, #0x6b
	ldrh r2, [r2]
	ldrb r3, [r3]
	bl sub_0207204C
	add r0, r4, #0
	add r0, #0x66
	ldrb r0, [r0]
	add r1, r0, #1
	add r0, r4, #0
	add r0, #0x66
	strb r1, [r0]
_0226024C:
	bl IsCryFinished
	cmp r0, #0
	beq _02260256
_02260254:
	b _02260374
_02260256:
	add r0, r4, #0
	add r0, #0x66
	ldrb r0, [r0]
	add r4, #0x66
	add sp, #0x114
	add r0, r0, #1
	strb r0, [r4]
	pop {r3, r4, r5, r6, pc}
_02260266:
	add r0, r4, #0
	add r0, #0x67
	ldrb r0, [r0]
	cmp r0, #2
	ldr r0, _02260378 ; =0x00000703
	bne _0226027A
	mov r1, #0x75
	bl sub_0200602C
	b _02260282
_0226027A:
	mov r1, #0x74
	mvn r1, r1
	bl sub_0200602C
_02260282:
	ldr r0, [r4, #8]
	mov r1, #0x29
	bl Pokepic_GetAttr
	cmp r0, #0
	ble _02260298
	mov r0, #7
	add r4, #0x66
	add sp, #0x114
	strb r0, [r4]
	pop {r3, r4, r5, r6, pc}
_02260298:
	mov r0, #8
	add r4, #0x66
	add sp, #0x114
	strb r0, [r4]
	pop {r3, r4, r5, r6, pc}
_022602A2:
	ldr r0, [r4, #8]
	mov r1, #0x29
	bl Pokepic_GetAttr
	add r5, r0, #0
	sub r5, #8
	bpl _022602B2
	mov r5, #0
_022602B2:
	ldr r0, [r4, #8]
	mov r1, #0x29
	add r2, r5, #0
	bl Pokepic_SetAttr
	cmp r5, #0
	bne _02260374
	add r0, r4, #0
	add r0, #0x66
	ldrb r0, [r0]
	add r1, r0, #1
	add r0, r4, #0
	add r0, #0x66
	strb r1, [r0]
_022602CE:
	ldr r0, [r4, #0x6c]
	add r1, r4, #0
	str r0, [sp]
	add r0, r4, #0
	add r2, r4, #0
	add r3, r4, #0
	add r0, #0x68
	add r1, #0x6a
	add r2, #0x67
	add r3, #0x6b
	ldrh r0, [r0]
	ldrb r1, [r1]
	ldrb r2, [r2]
	ldrb r3, [r3]
	bl GetMonPicHeightBySpeciesGenderForm
	mov r3, #0x50
	sub r0, r3, r0
	str r0, [sp]
	mov r1, #0
	ldr r0, [r4, #8]
	add r2, r1, #0
	bl Pokepic_SetVisible
	add r0, r4, #0
	add r0, #0x66
	ldrb r0, [r0]
	add r4, #0x66
	add sp, #0x114
	add r0, r0, #1
	strb r0, [r4]
	pop {r3, r4, r5, r6, pc}
_0226030E:
	ldr r0, [r4, #8]
	mov r1, #1
	bl Pokepic_GetAttr
	add r2, r0, #0
	ldr r0, [r4, #8]
	mov r1, #1
	add r2, #8
	bl Pokepic_SetAttr
	ldr r0, [r4, #8]
	mov r1, #0x12
	bl Pokepic_GetAttr
	add r5, r0, #0
	sub r5, #8
	bpl _02260332
	mov r5, #0
_02260332:
	ldr r0, [r4, #8]
	mov r1, #0x12
	add r2, r5, #0
	bl Pokepic_SetAttr
	cmp r5, #0
	bne _02260374
	ldr r0, [r4, #8]
	bl Pokepic_Delete
	add r0, r4, #0
	add r0, #0x66
	ldrb r0, [r0]
	add r4, #0x66
	add sp, #0x114
	add r0, r0, #1
	strb r0, [r4]
	pop {r3, r4, r5, r6, pc}
_02260356:
	add r1, r4, #0
	add r2, r4, #0
	add r1, #0x65
	add r2, #0x64
	ldrb r1, [r1]
	ldrb r2, [r2]
	ldr r0, [r4]
	bl ov12_0226430C
	add r0, r4, #0
	bl Heap_Free
	add r0, r6, #0
	bl SysTask_Destroy
_02260374:
	add sp, #0x114
	pop {r3, r4, r5, r6, pc}
	.balign 4, 0
_02260378: .word 0x00000703
	thumb_func_end ov12_022600F0

	thumb_func_start ov12_0226037C
ov12_0226037C: ; 0x0226037C
	push {r4, r5, r6, r7, lr}
	sub sp, #0xc
	add r5, r1, #0
	add r6, r0, #0
	ldr r0, [r5]
	bl BattleSystem_GetPaletteData
	add r4, r0, #0
	ldr r0, [r5]
	bl BattleSystem_GetPokepicManager
	add r7, r0, #0
	ldrb r0, [r5, #6]
	cmp r0, #0
	beq _022603A6
	cmp r0, #1
	beq _022603E4
	cmp r0, #2
	beq _022603F8
	add sp, #0xc
	pop {r4, r5, r6, r7, pc}
_022603A6:
	ldr r0, [r5]
	bl BattleSystem_GetMessageIcon
	mov r1, #1
	bl sub_0201649C
	mov r1, #0
	str r1, [sp]
	mov r0, #0x10
	str r0, [sp, #4]
	str r1, [sp, #8]
	ldr r2, _02260414 ; =0x0000FFFF
	add r0, r4, #0
	mov r1, #0xf
	mov r3, #1
	bl PaletteData_BeginPaletteFade
	mov r1, #0
	add r0, r7, #0
	mov r2, #0x10
	add r3, r1, #0
	str r1, [sp]
	bl Pokepic_StartPaletteFadeAll
	mov r0, #0
	mov r1, #0x10
	bl GF_SndStartFadeOutBGM
	ldrb r0, [r5, #6]
	add r0, r0, #1
	strb r0, [r5, #6]
_022603E4:
	add r0, r4, #0
	bl PaletteData_GetSelectedBuffersBitmask
	cmp r0, #0
	bne _0226040E
	ldrb r0, [r5, #6]
	add sp, #0xc
	add r0, r0, #1
	strb r0, [r5, #6]
	pop {r4, r5, r6, r7, pc}
_022603F8:
	ldrb r1, [r5, #5]
	ldrb r2, [r5, #4]
	ldr r0, [r5]
	bl ov12_0226430C
	add r0, r5, #0
	bl Heap_Free
	add r0, r6, #0
	bl SysTask_Destroy
_0226040E:
	add sp, #0xc
	pop {r4, r5, r6, r7, pc}
	nop
_02260414: .word 0x0000FFFF
	thumb_func_end ov12_0226037C

	thumb_func_start ov12_02260418
ov12_02260418: ; 0x02260418
	push {r4, r5, r6, lr}
	sub sp, #0x110
	add r4, r1, #0
	add r6, r0, #0
	ldr r0, [r4]
	bl ov12_0223A8DC
	add r1, r4, #0
	add r1, #0x62
	ldrb r1, [r1]
	add r5, r0, #0
	cmp r1, #4
	bls _02260434
	b _02260560
_02260434:
	add r1, r1, r1
	add r1, pc
	ldrh r1, [r1, #6]
	lsl r1, r1, #0x10
	asr r1, r1, #0x10
	add pc, r1
_02260440: ; jump table
	.short _0226044A - _02260440 - 2 ; case 0
	.short _0226048C - _02260440 - 2 ; case 1
	.short _0226053C - _02260440 - 2 ; case 2
	.short _022604E0 - _02260440 - 2 ; case 3
	.short _0226053C - _02260440 - 2 ; case 4
_0226044A:
	ldr r0, [r4, #4]
	ldr r0, [r0, #0x20]
	cmp r0, #0
	beq _02260482
	add r2, r4, #0
	add r2, #0x63
	ldrb r2, [r2]
	mov r1, #6
	bl Pokepic_SetAttr
	add r0, r4, #0
	add r0, #0x63
	ldrb r0, [r0]
	cmp r0, #1
	bne _02260472
	mov r0, #0xff
	add r4, #0x62
	add sp, #0x110
	strb r0, [r4]
	pop {r4, r5, r6, pc}
_02260472:
	add r0, r4, #0
	add r0, #0x62
	ldrb r0, [r0]
	add r4, #0x62
	add sp, #0x110
	add r0, r0, #1
	strb r0, [r4]
	pop {r4, r5, r6, pc}
_02260482:
	mov r0, #0xff
	add r4, #0x62
	add sp, #0x110
	strb r0, [r4]
	pop {r4, r5, r6, pc}
_0226048C:
	ldr r0, [r4, #0x64]
	cmp r0, #0
	beq _022604D6
	mov r0, #0x1a
	ldr r1, [r4, #4]
	lsl r0, r0, #4
	ldr r0, [r1, r0]
	cmp r0, #0
	bne _022604D6
	add r0, r4, #0
	add r0, #0x61
	ldrb r1, [r0]
	mov r0, #0xf
	add r2, sp, #0xb8
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
	add r3, sp, #0xb8
	bl ov12_02261B80
	add r0, r4, #0
	add r0, #0x62
	ldrb r0, [r0]
	add r4, #0x62
	add sp, #0x110
	add r0, r0, #1
	strb r0, [r4]
	pop {r4, r5, r6, pc}
_022604D6:
	mov r0, #0xff
	add r4, #0x62
	add sp, #0x110
	strb r0, [r4]
	pop {r4, r5, r6, pc}
_022604E0:
	add r3, r4, #0
	add r3, #0x61
	ldrb r3, [r3]
	add r1, r4, #0
	ldr r0, [r4]
	add r1, #8
	add r2, sp, #0x68
	bl ov12_02261CA8
	add r0, sp, #0x68
	mov r1, #5
	bl ov07_02234A20
	add r0, r4, #0
	add r0, #0x61
	ldrb r1, [r0]
	mov r0, #0x10
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
	mov r0, #0x1a
	ldr r1, [r4, #4]
	mov r2, #1
	lsl r0, r0, #4
	str r2, [r1, r0]
	add r0, r4, #0
	add r0, #0x62
	ldrb r0, [r0]
	add r4, #0x62
	add sp, #0x110
	add r0, r0, #1
	strb r0, [r4]
	pop {r4, r5, r6, pc}
_0226053C:
	bl ov07_0221C394
	add r0, r5, #0
	bl ov07_0221C3B0
	cmp r0, #0
	bne _0226057E
	add r0, r5, #0
	bl ov07_0221C3C0
	add r0, r4, #0
	add r0, #0x62
	ldrb r0, [r0]
	add r4, #0x62
	add sp, #0x110
	add r0, r0, #1
	strb r0, [r4]
	pop {r4, r5, r6, pc}
_02260560:
	add r1, r4, #0
	add r2, r4, #0
	add r1, #0x61
	add r2, #0x60
	ldrb r1, [r1]
	ldrb r2, [r2]
	ldr r0, [r4]
	bl ov12_0226430C
	add r0, r4, #0
	bl Heap_Free
	add r0, r6, #0
	bl SysTask_Destroy
_0226057E:
	add sp, #0x110
	pop {r4, r5, r6, pc}
	.balign 4, 0
	thumb_func_end ov12_02260418
