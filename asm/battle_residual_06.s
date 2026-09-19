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

	thumb_func_start ov12_02259968
ov12_02259968: ; 0x02259968
	push {r4, r5, r6, r7, lr}
	sub sp, #0x6c
	str r0, [sp, #0x24]
	add r6, r1, #0
	add r5, r2, #0
	bl BattleSystem_GetPokepicManager
	str r0, [sp, #0x28]
	ldr r0, [sp, #0x24]
	bl BattleSystem_GetBattleType
	mov r0, #5
	mov r1, #0x30
	bl Heap_Alloc
	add r4, r0, #0
	mov r2, #0
	ldr r0, _02259B8C ; =0x00000195
	strb r2, [r4, #0x12]
	ldrb r0, [r6, r0]
	mov r1, #1
	tst r0, r1
	beq _022599BE
	mov r0, #2
	strb r0, [r4, #0x13]
	ldr r0, [sp, #0x24]
	bl ov12_0223A8F4
	str r0, [r4, #0xc]
	ldr r1, _02259B8C ; =0x00000195
	ldr r0, [r0]
	ldrb r2, [r6, r1]
	mov r1, #1
	and r2, r1
	add r3, r2, #0
	mov r1, #6
	mul r3, r1
	ldr r1, _02259B90 ; =ov07_022377F4
	mov r2, #0x58
	ldrsh r1, [r1, r3]
	bl ManagedSprite_SetPositionXY
	b _022599E4
_022599BE:
	ldr r0, [sp, #0x24]
	add r1, r2, #0
	strb r2, [r4, #0x13]
	bl ov12_0223A8F4
	str r0, [r4, #0xc]
	ldr r1, _02259B8C ; =0x00000195
	ldr r0, [r0]
	ldrb r2, [r6, r1]
	mov r1, #1
	and r2, r1
	add r3, r2, #0
	mov r1, #6
	mul r3, r1
	ldr r1, _02259B90 ; =ov07_022377F4
	mov r2, #0x88
	ldrsh r1, [r1, r3]
	bl ManagedSprite_SetPositionXY
_022599E4:
	ldrb r0, [r5, #1]
	lsl r0, r0, #0x1d
	lsr r0, r0, #0x1f
	beq _022599F0
	mov r0, #1
	b _022599F2
_022599F0:
	mov r0, #0
_022599F2:
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	str r0, [sp]
	ldrb r0, [r5, #1]
	lsl r0, r0, #0x18
	lsr r0, r0, #0x1b
	str r0, [sp, #4]
	ldr r0, [r5, #4]
	str r0, [sp, #8]
	ldrb r2, [r5, #1]
	ldrh r1, [r5, #2]
	ldrb r3, [r4, #0x13]
	lsl r2, r2, #0x1e
	add r0, sp, #0x5c
	lsr r2, r2, #0x1e
	bl GetMonSpriteCharAndPlttNarcIdsEx
	ldr r0, [r5, #4]
	str r0, [sp]
	ldrb r3, [r5, #1]
	ldrh r0, [r5, #2]
	ldrb r2, [r4, #0x13]
	lsl r1, r3, #0x1e
	lsl r3, r3, #0x18
	lsr r1, r1, #0x1e
	lsr r3, r3, #0x1b
	bl GetMonPicHeightBySpeciesGenderForm
	str r0, [sp, #0x2c]
	mov r0, #0x69
	lsl r0, r0, #2
	add r1, sp, #0x30
	ldrh r2, [r5, #2]
	ldr r0, [r6, r0]
	add r1, #2
	bl sub_020729D8
	mov r0, #0x69
	lsl r0, r0, #2
	add r1, sp, #0x30
	ldrh r2, [r5, #2]
	ldr r0, [r6, r0]
	add r1, #1
	bl sub_020729FC
	mov r0, #0x69
	lsl r0, r0, #2
	ldrh r2, [r5, #2]
	ldr r0, [r6, r0]
	add r1, sp, #0x30
	bl sub_02072A20
	mov r3, #0x69
	lsl r3, r3, #2
	ldr r0, [r6, r3]
	sub r3, #0xf
	add r1, sp, #0x30
	ldrh r2, [r5, #2]
	ldrb r3, [r6, r3]
	add r1, #3
	bl NARC_ReadPokepicAnimScript
	ldr r1, _02259B8C ; =0x00000195
	mov r0, #6
	ldrb r2, [r6, r1]
	ldr r3, _02259B90 ; =ov07_022377F4
	add r7, r2, #0
	mul r7, r0
	ldr r0, _02259B94 ; =ov07_022377F4 + 2
	mov r2, #2
	ldrsh r0, [r0, r7]
	ldrsh r3, [r3, r7]
	str r0, [sp]
	ldr r0, _02259B98 ; =ov07_022377F4 + 4
	ldrsh r0, [r0, r7]
	str r0, [sp, #4]
	ldr r0, [sp, #0x2c]
	str r0, [sp, #8]
	add r0, sp, #0x30
	ldrsb r2, [r0, r2]
	str r2, [sp, #0xc]
	mov r2, #1
	ldrsb r2, [r0, r2]
	str r2, [sp, #0x10]
	ldrb r0, [r0]
	add r2, sp, #0x5c
	str r0, [sp, #0x14]
	sub r0, r1, #1
	ldrb r0, [r6, r0]
	ldr r1, [sp, #0x28]
	str r0, [sp, #0x18]
	add r0, sp, #0x30
	add r0, #3
	str r0, [sp, #0x1c]
	mov r0, #0
	str r0, [sp, #0x20]
	ldr r0, [sp, #0x24]
	bl ov12_022612A4
	str r0, [r6, #0x20]
	ldr r0, [r6, #0x20]
	str r0, [r4, #8]
	ldrb r0, [r4, #0x13]
	cmp r0, #2
	bne _02259AD2
	mov r3, #0
	str r3, [sp]
	mov r1, #8
	ldr r0, [r4, #8]
	add r2, r1, #0
	bl Pokepic_StartPaletteFade
_02259AD2:
	ldrb r0, [r4, #0x13]
	cmp r0, #2
	bne _02259B12
	ldr r0, [sp, #0x24]
	bl BattleSystem_GetBattleSpecial
	mov r1, #0x40
	tst r0, r1
	beq _02259B12
	ldr r0, [r4, #8]
	mov r1, #1
	bl Pokepic_GetAttr
	add r7, r0, #0
	ldr r0, [r4, #8]
	mov r1, #0x2e
	mov r2, #0
	bl Pokepic_SetAttr
	ldr r0, [r4, #8]
	mov r1, #0
	mov r2, #0xc0
	bl Pokepic_SetAttr
	add r2, r7, #0
	ldr r0, [r4, #8]
	mov r1, #1
	sub r2, #0x88
	bl Pokepic_SetAttr
	strh r7, [r4, #0x14]
	b _02259B1E
_02259B12:
	ldr r0, _02259B8C ; =0x00000195
	ldrb r0, [r6, r0]
	lsl r1, r0, #2
	ldr r0, _02259B9C ; =ov07_022377DC
	ldrsh r0, [r0, r1]
	strh r0, [r4, #0x14]
_02259B1E:
	ldr r0, [sp, #0x24]
	str r0, [r4]
	str r6, [r4, #4]
	ldrb r0, [r5]
	strb r0, [r4, #0x10]
	mov r0, #0x65
	lsl r0, r0, #2
	ldrb r1, [r6, r0]
	add r0, r0, #1
	strb r1, [r4, #0x11]
	ldrh r1, [r5, #2]
	strh r1, [r4, #0x16]
	ldrb r1, [r5, #1]
	lsl r1, r1, #0x18
	lsr r2, r1, #0x1b
	add r1, r4, #0
	add r1, #0x2c
	strb r2, [r1]
	ldr r1, [r5, #8]
	str r1, [r4, #0x18]
	ldrb r0, [r6, r0]
	str r0, [r4, #0x1c]
	ldr r0, [r5, #4]
	bl GetNatureFromPersonality
	str r0, [r4, #0x24]
	ldrb r0, [r5, #1]
	lsl r0, r0, #0x1d
	lsr r0, r0, #0x1f
	str r0, [r4, #0x28]
	ldrb r0, [r4, #0x13]
	cmp r0, #2
	bne _02259B78
	ldr r0, [sp, #0x24]
	bl BattleSystem_GetBattleSpecial
	mov r1, #0x40
	tst r0, r1
	beq _02259B78
	ldr r0, _02259BA0 ; =ov12_0225B7B8
	add r1, r4, #0
	mov r2, #0
	bl SysTask_CreateOnMainQueue
	b _02259B82
_02259B78:
	ldr r0, _02259BA4 ; =ov12_0225B494
	add r1, r4, #0
	mov r2, #0
	bl SysTask_CreateOnMainQueue
_02259B82:
	mov r0, #1
	bl sub_02005B58
	add sp, #0x6c
	pop {r4, r5, r6, r7, pc}
	.balign 4, 0
_02259B8C: .word 0x00000195
_02259B90: .word ov07_022377F4
_02259B94: .word ov07_022377F4 + 2
_02259B98: .word ov07_022377F4 + 4
_02259B9C: .word ov07_022377DC
_02259BA0: .word ov12_0225B7B8
_02259BA4: .word ov12_0225B494
	thumb_func_end ov12_02259968

	thumb_func_start ov12_02259BA8
ov12_02259BA8: ; 0x02259BA8
	push {r4, r5, r6, r7, lr}
	sub sp, #0xc
	add r7, r0, #0
	add r6, r1, #0
	add r5, r2, #0
	bl BattleSystem_GetBattleType
	mov r0, #5
	mov r1, #0x9c
	bl Heap_Alloc
	add r4, r0, #0
	ldr r0, _02259D3C ; =0x00000195
	ldrb r1, [r6, r0]
	mov r0, #1
	tst r0, r1
	beq _02259BCE
	mov r1, #2
	b _02259BD0
_02259BCE:
	mov r1, #0
_02259BD0:
	add r0, r4, #0
	add r0, #0x84
	strb r1, [r0]
	ldrb r0, [r5, #1]
	lsl r0, r0, #0x1d
	lsr r0, r0, #0x1f
	beq _02259BE2
	mov r0, #1
	b _02259BE4
_02259BE2:
	mov r0, #0
_02259BE4:
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	str r0, [sp]
	ldrb r0, [r5, #1]
	add r3, r4, #0
	add r3, #0x84
	lsl r0, r0, #0x18
	lsr r0, r0, #0x1b
	str r0, [sp, #4]
	ldr r0, [r5, #4]
	str r0, [sp, #8]
	ldrb r2, [r5, #1]
	add r0, r4, #0
	ldrh r1, [r5, #2]
	lsl r2, r2, #0x1e
	ldrb r3, [r3]
	add r0, #0x14
	lsr r2, r2, #0x1e
	bl GetMonSpriteCharAndPlttNarcIdsEx
	ldr r0, [r5, #4]
	add r2, r4, #0
	str r0, [sp]
	ldrb r3, [r5, #1]
	add r2, #0x84
	ldrh r0, [r5, #2]
	lsl r1, r3, #0x1e
	lsl r3, r3, #0x18
	ldrb r2, [r2]
	lsr r1, r1, #0x1e
	lsr r3, r3, #0x1b
	bl GetMonPicHeightBySpeciesGenderForm
	add r1, r4, #0
	add r1, #0x85
	strb r0, [r1]
	mov r0, #0x69
	lsl r0, r0, #2
	add r1, r4, #0
	ldrh r2, [r5, #2]
	ldr r0, [r6, r0]
	add r1, #0x90
	bl sub_020729D8
	mov r0, #0x69
	lsl r0, r0, #2
	add r1, r4, #0
	ldrh r2, [r5, #2]
	ldr r0, [r6, r0]
	add r1, #0x91
	bl sub_020729FC
	mov r0, #0x69
	lsl r0, r0, #2
	add r1, r4, #0
	ldrh r2, [r5, #2]
	ldr r0, [r6, r0]
	add r1, #0x93
	bl sub_02072A20
	add r0, r6, #0
	bl ov12_02261284
	str r7, [r4]
	add r0, r4, #0
	str r6, [r4, #4]
	mov r1, #0
	add r0, #0x83
	strb r1, [r0]
	add r0, r4, #0
	add r0, #0x96
	strb r1, [r0]
	add r0, r4, #0
	ldrb r1, [r5]
	add r0, #0x80
	strb r1, [r0]
	mov r0, #0x65
	lsl r0, r0, #2
	ldrb r2, [r6, r0]
	add r1, r4, #0
	add r1, #0x81
	strb r2, [r1]
	add r1, r4, #0
	ldrh r2, [r5, #2]
	add r1, #0x86
	add r0, r0, #1
	strh r2, [r1]
	ldrb r1, [r5, #1]
	lsl r1, r1, #0x18
	lsr r2, r1, #0x1b
	add r1, r4, #0
	add r1, #0x97
	strb r2, [r1]
	ldrb r1, [r6, r0]
	add r0, r4, #0
	add r0, #0x82
	strb r1, [r0]
	add r0, r4, #0
	ldr r1, [r5, #8]
	add r0, #0x88
	str r1, [r0]
	add r0, r4, #0
	ldr r1, [r5, #0xc]
	add r0, #0x8c
	strb r1, [r0]
	ldr r0, [r5, #4]
	bl GetNatureFromPersonality
	add r1, r4, #0
	add r1, #0x8d
	strb r0, [r1]
	add r0, r4, #0
	ldr r1, [r5, #0x10]
	add r0, #0x8e
	strh r1, [r0]
	ldrb r0, [r5, #1]
	lsl r0, r0, #0x1d
	lsr r1, r0, #0x1f
	add r0, r4, #0
	add r0, #0x92
	strb r1, [r0]
	add r0, r4, #0
	mov r1, #0
	add r0, #0x94
	strh r1, [r0]
	mov r0, #1
	bl sub_02005B58
	add r0, r7, #0
	bl BattleSystem_GetBattleType
	mov r1, #0x65
	lsl r1, r1, #2
	add r5, r0, #0
	ldrb r1, [r6, r1]
	add r0, r7, #0
	bl ov12_0223C140
	cmp r0, #0xff
	beq _02259D2C
	mov r0, #2
	tst r0, r5
	beq _02259D1E
	mov r0, #8
	tst r0, r5
	bne _02259D1E
	ldr r0, _02259D3C ; =0x00000195
	ldrb r0, [r6, r0]
	cmp r0, #3
	bls _02259D1E
	ldr r0, _02259D40 ; =ov12_0225B960
	add r1, r4, #0
	mov r2, #0
	bl SysTask_CreateOnMainQueue
	add sp, #0xc
	pop {r4, r5, r6, r7, pc}
_02259D1E:
	ldr r0, _02259D44 ; =ov12_0225BE38
	add r1, r4, #0
	mov r2, #0
	bl SysTask_CreateOnMainQueue
	add sp, #0xc
	pop {r4, r5, r6, r7, pc}
_02259D2C:
	ldr r0, _02259D40 ; =ov12_0225B960
	add r1, r4, #0
	mov r2, #0
	bl SysTask_CreateOnMainQueue
	add sp, #0xc
	pop {r4, r5, r6, r7, pc}
	nop
_02259D3C: .word 0x00000195
_02259D40: .word ov12_0225B960
_02259D44: .word ov12_0225BE38
	thumb_func_end ov12_02259BA8

	thumb_func_start ov12_02259D48
ov12_02259D48: ; 0x02259D48
	push {r4, r5, r6, r7, lr}
	sub sp, #0x14
	str r0, [sp, #0xc]
	str r1, [sp, #0x10]
	add r4, r2, #0
	bl BattleSystem_GetBattleType
	mov r0, #5
	mov r1, #0x9c
	bl Heap_Alloc
	add r5, r0, #0
	add r1, r5, #0
	mov r0, #0
	add r1, #0x83
	strb r0, [r1]
	ldr r2, _02259F24 ; =0x00000195
	ldr r1, [sp, #0x10]
	ldrb r2, [r1, r2]
	mov r1, #1
	tst r1, r2
	beq _02259D7E
	add r0, r5, #0
	mov r1, #2
	add r0, #0x84
	strb r1, [r0]
	b _02259D84
_02259D7E:
	add r1, r5, #0
	add r1, #0x84
	strb r0, [r1]
_02259D84:
	ldrb r0, [r4, #1]
	lsl r0, r0, #0x1d
	lsr r0, r0, #0x1f
	beq _02259D90
	mov r0, #1
	b _02259D92
_02259D90:
	mov r0, #0
_02259D92:
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	str r0, [sp]
	ldrb r0, [r4, #1]
	add r3, r5, #0
	add r3, #0x84
	lsl r0, r0, #0x18
	lsr r0, r0, #0x1b
	str r0, [sp, #4]
	ldr r0, [r4, #4]
	str r0, [sp, #8]
	ldrb r2, [r4, #1]
	add r0, r5, #0
	ldrh r1, [r4, #2]
	lsl r2, r2, #0x1e
	ldrb r3, [r3]
	add r0, #0x14
	lsr r2, r2, #0x1e
	bl GetMonSpriteCharAndPlttNarcIdsEx
	ldr r0, [r4, #4]
	add r2, r5, #0
	str r0, [sp]
	ldrb r3, [r4, #1]
	add r2, #0x84
	ldrh r0, [r4, #2]
	lsl r1, r3, #0x1e
	lsl r3, r3, #0x18
	ldrb r2, [r2]
	lsr r1, r1, #0x1e
	lsr r3, r3, #0x1b
	bl GetMonPicHeightBySpeciesGenderForm
	add r1, r5, #0
	add r1, #0x85
	strb r0, [r1]
	mov r1, #0x69
	ldr r0, [sp, #0x10]
	lsl r1, r1, #2
	ldr r0, [r0, r1]
	add r1, r5, #0
	ldrh r2, [r4, #2]
	add r1, #0x90
	bl sub_020729D8
	mov r1, #0x69
	ldr r0, [sp, #0x10]
	lsl r1, r1, #2
	ldr r0, [r0, r1]
	add r1, r5, #0
	ldrh r2, [r4, #2]
	add r1, #0x91
	bl sub_020729FC
	mov r1, #0x69
	ldr r0, [sp, #0x10]
	lsl r1, r1, #2
	ldr r0, [r0, r1]
	add r1, r5, #0
	ldrh r2, [r4, #2]
	add r1, #0x93
	bl sub_02072A20
	ldr r0, [sp, #0x10]
	bl ov12_02261284
	ldr r0, [sp, #0xc]
	str r0, [r5]
	ldr r0, [sp, #0x10]
	str r0, [r5, #4]
	add r0, r5, #0
	ldrb r1, [r4]
	add r0, #0x80
	strb r1, [r0]
	mov r1, #0x65
	ldr r0, [sp, #0x10]
	lsl r1, r1, #2
	ldrb r2, [r0, r1]
	add r0, r5, #0
	add r0, #0x81
	strb r2, [r0]
	add r0, r5, #0
	ldrh r2, [r4, #2]
	add r0, #0x86
	add r1, r1, #1
	strh r2, [r0]
	ldrb r0, [r4, #1]
	lsl r0, r0, #0x18
	lsr r2, r0, #0x1b
	add r0, r5, #0
	add r0, #0x97
	strb r2, [r0]
	ldr r0, [sp, #0x10]
	ldrb r1, [r0, r1]
	add r0, r5, #0
	add r0, #0x82
	strb r1, [r0]
	add r0, r5, #0
	ldr r1, [r4, #8]
	add r0, #0x88
	str r1, [r0]
	add r0, r5, #0
	ldr r1, [r4, #0xc]
	add r0, #0x8c
	strb r1, [r0]
	ldr r0, [r4, #4]
	bl GetNatureFromPersonality
	add r1, r5, #0
	add r1, #0x8d
	strb r0, [r1]
	add r0, r5, #0
	ldr r1, [r4, #0x10]
	add r0, #0x8e
	strh r1, [r0]
	ldrb r0, [r4, #1]
	lsl r0, r0, #0x1d
	lsr r1, r0, #0x1f
	add r0, r5, #0
	add r0, #0x92
	strb r1, [r0]
	add r0, r5, #0
	ldr r1, [r4, #0x14]
	add r0, #0x94
	strh r1, [r0]
	add r1, r5, #0
	mov r0, #0
	add r1, #0x96
	strb r0, [r1]
	add r1, r5, #0
	ldr r2, [r4, #0x4c]
	add r1, #0x98
	str r2, [r1]
	mov ip, r4
	add r7, r5, #0
	add r6, r4, #0
	add r1, r5, #0
_02259EA4:
	mov r2, ip
	add r2, #0x50
	ldrh r3, [r2]
	add r2, r7, #0
	add r2, #0x40
	strh r3, [r2]
	add r2, r4, r0
	add r2, #0x58
	ldrb r3, [r2]
	add r2, r5, r0
	add r2, #0x48
	strb r3, [r2]
	add r2, r4, r0
	add r2, #0x5c
	ldrb r3, [r2]
	add r2, r5, r0
	add r2, #0x4c
	strb r3, [r2]
	add r2, r4, r0
	add r2, #0x60
	ldrb r3, [r2]
	add r2, r5, r0
	add r2, #0x50
	strb r3, [r2]
	ldr r2, [r6, #0x64]
	add r0, r0, #1
	str r2, [r1, #0x54]
	mov r2, ip
	add r2, r2, #2
	mov ip, r2
	add r7, r7, #2
	add r6, r6, #4
	add r1, r1, #4
	cmp r0, #4
	blt _02259EA4
	mov r2, #0x65
	ldr r1, [sp, #0x10]
	lsl r2, r2, #2
	ldrb r1, [r1, r2]
	ldr r0, [sp, #0xc]
	bl ov12_0223C140
	cmp r0, #0xff
	beq _02259F06
	add r1, r5, #0
	add r1, #0x8c
	ldrb r1, [r1]
	cmp r0, r1
	beq _02259F14
_02259F06:
	ldr r0, _02259F28 ; =ov12_0225C18C
	add r1, r5, #0
	mov r2, #0
	bl SysTask_CreateOnMainQueue
	add sp, #0x14
	pop {r4, r5, r6, r7, pc}
_02259F14:
	ldr r0, _02259F2C ; =ov12_0225C6C8
	add r1, r5, #0
	mov r2, #0
	bl SysTask_CreateOnMainQueue
	add sp, #0x14
	pop {r4, r5, r6, r7, pc}
	nop
_02259F24: .word 0x00000195
_02259F28: .word ov12_0225C18C
_02259F2C: .word ov12_0225C6C8
	thumb_func_end ov12_02259D48

	thumb_func_start ov12_02259F30
ov12_02259F30: ; 0x02259F30
	push {r4, r5, r6, r7, lr}
	sub sp, #0xc
	str r0, [sp]
	str r1, [sp, #4]
	mov r0, #5
	mov r1, #0x78
	str r2, [sp, #8]
	bl Heap_Alloc
	add r4, r0, #0
	ldr r0, [sp]
	add r7, r4, #0
	str r0, [r4]
	ldr r0, [sp, #4]
	str r0, [r4, #4]
	ldr r0, [r0, #0x20]
	str r0, [r4, #8]
	ldr r0, [sp, #8]
	ldrb r1, [r0]
	add r0, r4, #0
	add r0, #0x68
	strb r1, [r0]
	mov r1, #0x65
	ldr r0, [sp, #4]
	lsl r1, r1, #2
	ldrb r2, [r0, r1]
	add r0, r4, #0
	add r0, #0x69
	strb r2, [r0]
	ldr r0, [sp, #4]
	add r1, r1, #1
	ldrb r1, [r0, r1]
	add r0, r4, #0
	add r0, #0x6a
	strb r1, [r0]
	add r1, r4, #0
	mov r0, #0
	add r1, #0x6b
	strb r0, [r1]
	ldr r1, [sp, #8]
	ldrb r2, [r1, #1]
	add r1, r4, #0
	add r1, #0x6c
	strb r2, [r1]
	ldr r1, [sp, #8]
	ldrh r2, [r1, #2]
	add r1, r4, #0
	add r1, #0x6e
	strh r2, [r1]
	ldr r1, [sp, #8]
	add r2, r4, #0
	ldr r1, [r1, #4]
	str r1, [r4, #0x70]
	ldr r1, [sp, #8]
	ldr r1, [r1, #0x2c]
	str r1, [r4, #0x74]
	ldr r1, [sp, #8]
	mov ip, r1
_02259FA4:
	mov r3, ip
	ldrh r3, [r3, #8]
	strh r3, [r7, #0x28]
	ldr r3, [sp, #8]
	add r7, r7, #2
	add r6, r3, r0
	add r3, r4, r0
	ldrb r5, [r6, #0x10]
	add r3, #0x30
	strb r5, [r3]
	add r3, r4, r0
	ldrb r5, [r6, #0x14]
	add r3, #0x34
	strb r5, [r3]
	add r3, r4, r0
	ldrb r5, [r6, #0x18]
	add r3, #0x38
	add r0, r0, #1
	strb r5, [r3]
	ldr r3, [r1, #0x1c]
	add r1, r1, #4
	str r3, [r2, #0x3c]
	mov r3, ip
	add r3, r3, #2
	add r2, r2, #4
	mov ip, r3
	cmp r0, #4
	blt _02259FA4
	mov r2, #0x65
	ldr r1, [sp, #4]
	lsl r2, r2, #2
	ldrb r1, [r1, r2]
	ldr r0, [sp]
	bl ov12_0223C140
	cmp r0, #0xff
	beq _02259FF4
	ldr r1, [r4, #0x74]
	cmp r0, r1
	beq _0225A002
_02259FF4:
	ldr r0, _0225A010 ; =ov12_0225C9BC
	add r1, r4, #0
	mov r2, #0
	bl SysTask_CreateOnMainQueue
	add sp, #0xc
	pop {r4, r5, r6, r7, pc}
_0225A002:
	ldr r0, _0225A014 ; =ov12_0225CC58
	add r1, r4, #0
	mov r2, #0
	bl SysTask_CreateOnMainQueue
	add sp, #0xc
	pop {r4, r5, r6, r7, pc}
	.balign 4, 0
_0225A010: .word ov12_0225C9BC
_0225A014: .word ov12_0225CC58
	thumb_func_end ov12_02259F30

	thumb_func_start ov12_0225A018
ov12_0225A018: ; 0x0225A018
	push {r3, r4, r5, r6, r7, lr}
	add r7, r0, #0
	add r5, r1, #0
	mov r0, #5
	mov r1, #0x10
	add r6, r2, #0
	bl Heap_Alloc
	add r4, r0, #0
	str r7, [r4]
	ldr r0, [r5, #0x20]
	str r0, [r4, #4]
	ldrb r0, [r6]
	strb r0, [r4, #8]
	mov r0, #0x65
	lsl r0, r0, #2
	ldrb r1, [r5, r0]
	add r0, r0, #1
	strb r1, [r4, #9]
	ldrb r0, [r5, r0]
	mov r1, #0
	add r3, r1, #0
	strb r0, [r4, #0xa]
	strb r1, [r4, #0xb]
	ldrb r0, [r6, #1]
	strb r0, [r4, #0xc]
	ldrh r0, [r6, #2]
	lsl r2, r0, #1
	ldr r0, _0225A074 ; =ov12_0226D15A
	ldrh r0, [r0, r2]
	mov r2, #0x10
	str r0, [sp]
	ldr r0, [r4, #4]
	bl Pokepic_StartPaletteFade
	ldr r0, [r4, #4]
	mov r1, #0x2d
	mov r2, #1
	bl Pokepic_SetAttr
	ldr r0, _0225A078 ; =ov12_0225CDB8
	add r1, r4, #0
	mov r2, #0
	bl SysTask_CreateOnMainQueue
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
_0225A074: .word ov12_0226D15A
_0225A078: .word ov12_0225CDB8
	thumb_func_end ov12_0225A018

	thumb_func_start ov12_0225A07C
ov12_0225A07C: ; 0x0225A07C
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x50
	add r6, r0, #0
	add r5, r1, #0
	mov r0, #5
	mov r1, #0x24
	add r7, r2, #0
	bl Heap_Alloc
	add r4, r0, #0
	mov r2, #0
	ldr r0, _0225A288 ; =0x00000195
	strb r2, [r4, #0x12]
	ldrb r0, [r5, r0]
	mov r1, #1
	tst r0, r1
	beq _0225A0C4
	mov r0, #2
	strb r0, [r4, #0x13]
	add r0, r6, #0
	bl ov12_0223A8F4
	str r0, [r4, #8]
	ldr r1, _0225A288 ; =0x00000195
	ldr r0, [r0]
	ldrb r2, [r5, r1]
	mov r1, #1
	and r1, r2
	mov r2, #6
	mul r2, r1
	ldr r1, _0225A28C ; =ov07_022377F4
	ldrsh r1, [r1, r2]
	mov r2, #0x58
	bl ManagedSprite_SetPositionXY
	b _0225A0E8
_0225A0C4:
	add r0, r6, #0
	add r1, r2, #0
	strb r2, [r4, #0x13]
	bl ov12_0223A8F4
	str r0, [r4, #8]
	ldr r1, _0225A288 ; =0x00000195
	ldr r0, [r0]
	ldrb r2, [r5, r1]
	mov r1, #1
	and r1, r2
	mov r2, #6
	mul r2, r1
	ldr r1, _0225A28C ; =ov07_022377F4
	ldrsh r1, [r1, r2]
	mov r2, #0x88
	bl ManagedSprite_SetPositionXY
_0225A0E8:
	add r0, r6, #0
	bl BattleSystem_GetBattleType
	mov r1, #8
	tst r0, r1
	bne _0225A10A
	add r0, r6, #0
	bl BattleSystem_GetBattleType
	mov r1, #0x10
	tst r0, r1
	beq _0225A112
	ldr r0, _0225A288 ; =0x00000195
	ldrb r1, [r5, r0]
	mov r0, #1
	tst r0, r1
	beq _0225A112
_0225A10A:
	ldr r0, _0225A288 ; =0x00000195
	ldrb r0, [r5, r0]
	str r0, [sp, #0x10]
	b _0225A11C
_0225A112:
	ldr r0, _0225A288 ; =0x00000195
	ldrb r1, [r5, r0]
	mov r0, #1
	and r0, r1
	str r0, [sp, #0x10]
_0225A11C:
	mov r1, #0x65
	ldrh r2, [r7, #2]
	lsl r1, r1, #2
	ldrb r1, [r5, r1]
	lsl r2, r2, #0x18
	add r0, r6, #0
	lsr r2, r2, #0x18
	bl ov12_02261EF0
	strh r0, [r7, #2]
	add r0, r6, #0
	bl BattleSystem_GetBattleType
	mov r1, #0x65
	lsl r1, r1, #2
	str r0, [sp, #0x20]
	mov r0, #0
	str r0, [sp, #0x1c]
	ldrb r1, [r5, r1]
	add r0, r6, #0
	bl ov12_0223C140
	cmp r0, #0xff
	beq _0225A166
	ldr r0, [sp, #0x20]
	mov r1, #2
	tst r0, r1
	beq _0225A162
	ldr r0, [sp, #0x20]
	mov r1, #8
	tst r0, r1
	bne _0225A162
	mov r0, #0
	str r0, [sp, #0x1c]
	b _0225A166
_0225A162:
	mov r0, #1
	str r0, [sp, #0x1c]
_0225A166:
	ldr r1, [sp, #0x10]
	mov r0, #6
	mul r0, r1
	ldr r1, _0225A28C ; =ov07_022377F4
	ldr r3, _0225A288 ; =0x00000195
	ldrsh r1, [r1, r0]
	str r1, [sp, #0x14]
	ldr r1, _0225A290 ; =ov07_022377F4 + 2
	ldrsh r0, [r1, r0]
	ldr r1, [sp, #0x10]
	str r0, [sp, #0x18]
	ldr r0, [sp, #0x1c]
	str r0, [sp]
	ldr r0, [sp, #0x14]
	str r0, [sp, #4]
	ldr r0, [sp, #0x18]
	str r0, [sp, #8]
	ldrh r2, [r7, #2]
	ldrb r3, [r5, r3]
	add r0, r6, #0
	bl ov12_02261B2C
	str r0, [r5, #0x18]
	str r0, [r4, #0xc]
	ldrb r0, [r4, #0x13]
	cmp r0, #0
	bne _0225A23A
	add r0, r6, #0
	bl BattleSystem_GetBattleType
	cmp r0, #0
	beq _0225A1E8
	add r0, r6, #0
	bl BattleSystem_GetBattleType
	cmp r0, #0x20
	beq _0225A1E8
	add r0, r6, #0
	bl BattleSystem_GetBattleType
	mov r1, #1
	lsl r1, r1, #8
	cmp r0, r1
	beq _0225A1E8
	add r0, r6, #0
	bl BattleSystem_GetBattleType
	mov r1, #2
	lsl r1, r1, #8
	cmp r0, r1
	beq _0225A1E8
	add r0, r6, #0
	bl BattleSystem_GetBattleType
	mov r1, #1
	lsl r1, r1, #0xa
	cmp r0, r1
	beq _0225A1E8
	add r0, r6, #0
	bl BattleSystem_GetBattleType
	mov r1, #1
	lsl r1, r1, #0xc
	cmp r0, r1
	bne _0225A23A
_0225A1E8:
	add r0, r6, #0
	bl BattleSystem_GetPokepicManager
	str r0, [sp, #0x24]
	ldrh r0, [r7, #2]
	ldrb r1, [r4, #0x13]
	add r2, sp, #0x28
	bl sub_02070D84
	ldr r1, [sp, #0x28]
	add r0, sp, #0x28
	strh r1, [r0, #0x18]
	ldr r1, [sp, #0x3c]
	ldr r2, [sp, #0x10]
	strh r1, [r0, #0x1a]
	ldr r1, [sp, #0x30]
	add r3, sp, #0x40
	strh r1, [r0, #0x1c]
	mov r1, #0
	strh r1, [r0, #0x1e]
	mov r0, #6
	mul r0, r2
	ldr r2, _0225A294 ; =ov07_022377F4 + 4
	strb r1, [r3, #8]
	ldrsh r0, [r2, r0]
	str r1, [sp, #0x4c]
	str r0, [sp]
	mov r0, #0x65
	lsl r0, r0, #2
	ldrb r0, [r5, r0]
	str r0, [sp, #4]
	str r1, [sp, #8]
	str r1, [sp, #0xc]
	add r1, r3, #0
	ldr r0, [sp, #0x24]
	ldr r2, [sp, #0x14]
	ldr r3, [sp, #0x18]
	bl PokepicManager_CreatePokepic
	str r0, [r4, #4]
	b _0225A23E
_0225A23A:
	mov r0, #0
	str r0, [r4, #4]
_0225A23E:
	ldr r0, [sp, #0x10]
	lsl r1, r0, #2
	ldr r0, _0225A298 ; =ov07_022377DC
	ldrsh r0, [r0, r1]
	strh r0, [r4, #0x14]
	str r6, [r4]
	ldrb r0, [r7]
	strb r0, [r4, #0x10]
	mov r0, #0x65
	lsl r0, r0, #2
	ldrb r1, [r5, r0]
	add r0, r0, #1
	strb r1, [r4, #0x11]
	ldrb r0, [r5, r0]
	str r0, [r4, #0x18]
	mov r0, #0
	str r0, [r4, #0x20]
	ldr r0, [r4, #0x18]
	cmp r0, #0
	beq _0225A26A
	cmp r0, #2
	bne _0225A27A
_0225A26A:
	add r0, r6, #0
	bl BattleSystem_GetBgConfig
	mov r1, #3
	mov r2, #2
	mov r3, #0x84
	bl BgSetPosTextAndCommit
_0225A27A:
	ldr r0, _0225A29C ; =ov12_0225CE28
	add r1, r4, #0
	mov r2, #0
	bl SysTask_CreateOnMainQueue
	add sp, #0x50
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
_0225A288: .word 0x00000195
_0225A28C: .word ov07_022377F4
_0225A290: .word ov07_022377F4 + 2
_0225A294: .word ov07_022377F4 + 4
_0225A298: .word ov07_022377DC
_0225A29C: .word ov12_0225CE28
	thumb_func_end ov12_0225A07C

	thumb_func_start ov12_0225A2A0
ov12_0225A2A0: ; 0x0225A2A0
	push {r3, r4, r5, r6, r7, lr}
	add r6, r0, #0
	add r5, r1, #0
	mov r0, #5
	mov r1, #0x1c
	add r7, r2, #0
	bl Heap_Alloc
	add r4, r0, #0
	mov r0, #0
	strb r0, [r4, #0xa]
	str r6, [r4]
	ldrb r1, [r7]
	strb r1, [r4, #8]
	ldrb r1, [r7, #1]
	str r1, [r4, #0x10]
	mov r1, #0x65
	lsl r1, r1, #2
	ldrb r2, [r5, r1]
	strb r2, [r4, #9]
	str r5, [r4, #4]
	add r2, r1, #1
	ldrb r3, [r5, r2]
	mov r2, #1
	tst r2, r3
	beq _0225A2DA
	mov r1, #2
	strb r1, [r4, #0xb]
	b _0225A2FC
_0225A2DA:
	ldrb r1, [r5, r1]
	add r0, r6, #0
	bl BattleSystem_GetTrainer
	add r2, r0, #0
	mov r0, #0
	strb r0, [r4, #0xb]
	mov r1, #0x65
	lsl r1, r1, #2
	ldrb r1, [r5, r1]
	ldrb r2, [r2, #1]
	add r0, r6, #0
	bl ov12_02261EF0
	mov r1, #0
	bl TrainerClassToBackpicID
_0225A2FC:
	str r0, [r4, #0xc]
	add r0, r6, #0
	bl BattleSystem_GetBattleType
	mov r1, #0x65
	lsl r1, r1, #2
	ldrb r1, [r5, r1]
	add r0, r6, #0
	bl ov12_0223C140
	cmp r0, #0xff
	beq _0225A320
	ldr r0, _0225A32C ; =ov12_0225D644
	add r1, r4, #0
	mov r2, #0
	bl SysTask_CreateOnMainQueue
	pop {r3, r4, r5, r6, r7, pc}
_0225A320:
	ldr r0, _0225A330 ; =ov12_0225D138
	add r1, r4, #0
	mov r2, #0
	bl SysTask_CreateOnMainQueue
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
_0225A32C: .word ov12_0225D644
_0225A330: .word ov12_0225D138
	thumb_func_end ov12_0225A2A0

	thumb_func_start ov12_0225A334
ov12_0225A334: ; 0x0225A334
	push {r3, r4, r5, lr}
	add r5, r0, #0
	add r4, r1, #0
	mov r0, #5
	mov r1, #0x10
	bl Heap_Alloc
	add r1, r0, #0
	mov r0, #0
	strb r0, [r1, #0xa]
	add r2, r4, #0
	str r5, [r1]
	add r2, #0x94
	ldrb r2, [r2]
	strb r2, [r1, #8]
	mov r2, #0x65
	lsl r2, r2, #2
	ldrb r3, [r4, r2]
	add r2, r2, #1
	strb r3, [r1, #9]
	str r4, [r1, #4]
	ldrb r3, [r4, r2]
	mov r2, #1
	tst r2, r3
	beq _0225A36A
	mov r0, #2
	b _0225A36A
_0225A36A:
	strb r0, [r1, #0xb]
	ldr r0, _0225A378 ; =ov12_0225D890
	mov r2, #0
	bl SysTask_CreateOnMainQueue
	pop {r3, r4, r5, pc}
	nop
_0225A378: .word ov12_0225D890
	thumb_func_end ov12_0225A334

	thumb_func_start ov12_0225A37C
ov12_0225A37C: ; 0x0225A37C
	push {r4, r5, r6, r7, lr}
	sub sp, #0xc
	add r7, r0, #0
	add r5, r1, #0
	add r6, r2, #0
	bl BattleSystem_GetPokepicManager
	mov r0, #5
	mov r1, #0x10
	bl Heap_Alloc
	add r4, r0, #0
	mov r0, #0
	ldr r1, _0225A400 ; =0x00000195
	strb r0, [r4, #0xa]
	ldrb r2, [r5, r1]
	mov r1, #1
	tst r1, r2
	beq _0225A3A6
	mov r0, #2
	b _0225A3A6
_0225A3A6:
	strb r0, [r4, #0xb]
	ldr r0, _0225A400 ; =0x00000195
	mov r2, #0
	ldrb r3, [r5, r0]
	mov r0, #1
	add r1, r3, #0
	and r1, r0
	mov r0, #6
	str r2, [sp]
	ldr r2, _0225A404 ; =ov07_022377F4 + 36
	mul r0, r1
	ldrsh r2, [r2, r0]
	str r2, [sp, #4]
	ldr r2, _0225A408 ; =ov07_022377F4 + 38
	ldrsh r0, [r2, r0]
	str r0, [sp, #8]
	ldrh r2, [r6, #2]
	add r0, r7, #0
	bl ov12_02261B2C
	str r0, [r5, #0x18]
	str r0, [r4, #4]
	ldr r3, _0225A400 ; =0x00000195
	ldr r2, [r6, #4]
	ldrb r0, [r5, r3]
	lsl r2, r2, #1
	lsl r0, r0, #0x1f
	lsr r1, r0, #0x1d
	ldr r0, _0225A40C ; =ov07_022377DC
	add r0, r0, r1
	ldrsh r0, [r2, r0]
	add r1, r4, #0
	mov r2, #0
	strh r0, [r4, #0xc]
	str r7, [r4]
	ldrb r0, [r6]
	strb r0, [r4, #8]
	sub r0, r3, #1
	ldrb r0, [r5, r0]
	strb r0, [r4, #9]
	ldr r0, _0225A410 ; =ov12_0225D990
	bl SysTask_CreateOnMainQueue
	add sp, #0xc
	pop {r4, r5, r6, r7, pc}
	.balign 4, 0
_0225A400: .word 0x00000195
_0225A404: .word ov07_022377F4 + 36
_0225A408: .word ov07_022377F4 + 38
_0225A40C: .word ov07_022377DC
_0225A410: .word ov12_0225D990
	thumb_func_end ov12_0225A37C

	thumb_func_start ov12_0225A414
ov12_0225A414: ; 0x0225A414
	push {r3, r4, r5, r6, r7, lr}
	add r6, r1, #0
	add r4, r6, #0
	add r4, #0x28
	add r7, r0, #0
	add r5, r2, #0
	mov r0, #0
	add r1, r4, #0
	mov r2, #1
	bl MIi_CpuClearFast
	mov r0, #0x65
	str r7, [r4, #0xc]
	lsl r0, r0, #2
	ldrb r1, [r6, r0]
	add r0, r4, #0
	add r0, #0x24
	strb r1, [r0]
	add r0, r7, #0
	bl BattleSystem_GetBattleType
	add r1, r0, #0
	ldr r0, _0225A4D4 ; =0x00000195
	ldrb r0, [r6, r0]
	bl BattleHpBar_Util_GetBarTypeFromBattlerSide
	add r1, r4, #0
	add r1, #0x25
	strb r0, [r1]
	add r0, r4, #0
	ldrb r1, [r5]
	add r0, #0x4c
	strb r1, [r0]
	mov r0, #2
	ldrsh r0, [r5, r0]
	str r0, [r4, #0x28]
	ldrh r0, [r5, #4]
	str r0, [r4, #0x2c]
	add r0, r4, #0
	ldrb r1, [r5, #1]
	add r0, #0x48
	strb r1, [r0]
	ldrb r0, [r5, #7]
	lsl r0, r0, #0x19
	lsr r1, r0, #0x1e
	add r0, r4, #0
	add r0, #0x49
	strb r1, [r0]
	mov r1, #0
	str r1, [r4, #0x30]
	ldr r0, [r5, #8]
	str r0, [r4, #0x38]
	ldr r0, [r5, #0xc]
	str r0, [r4, #0x3c]
	add r0, r4, #0
	ldrb r2, [r5, #6]
	add r0, #0x26
	strb r2, [r0]
	ldrb r0, [r5, #7]
	lsl r0, r0, #0x1b
	lsr r2, r0, #0x1b
	add r0, r4, #0
	add r0, #0x4a
	strb r2, [r0]
	ldrb r0, [r5, #7]
	lsl r0, r0, #0x18
	lsr r2, r0, #0x1f
	add r0, r4, #0
	add r0, #0x4b
	strb r2, [r0]
	add r0, r4, #0
	ldrb r2, [r5, #0x14]
	add r0, #0x4d
	strb r2, [r0]
	add r0, r4, #0
	ldr r2, [r5, #0x10]
	add r0, #0x27
	strb r2, [r0]
	add r0, r4, #0
	bl BattleHpBar_SetEnabled
	mov r2, #0
	ldr r1, [r4, #0x28]
	add r0, r4, #0
	mvn r2, r2
	bl ov12_0226498C
	mov r2, #0xfa
	ldr r0, _0225A4D8 ; =ov12_0225DA18
	add r1, r4, #0
	lsl r2, r2, #2
	bl SysTask_CreateOnMainQueue
	str r0, [r4, #0x10]
	pop {r3, r4, r5, r6, r7, pc}
	nop
_0225A4D4: .word 0x00000195
_0225A4D8: .word ov12_0225DA18
	thumb_func_end ov12_0225A414

	thumb_func_start ov12_0225A4DC
ov12_0225A4DC: ; 0x0225A4DC
	push {r4, r5, r6, lr}
	add r5, r1, #0
	add r4, r5, #0
	add r4, #0x28
	add r6, r0, #0
	mov r0, #0
	add r1, r4, #0
	mov r2, #1
	bl MIi_CpuClearFast
	mov r0, #0x65
	str r6, [r4, #0xc]
	lsl r0, r0, #2
	ldrb r1, [r5, r0]
	add r0, r4, #0
	add r0, #0x24
	strb r1, [r0]
	add r5, #0x94
	add r0, r4, #0
	ldrb r1, [r5]
	add r0, #0x4c
	strb r1, [r0]
	add r0, r4, #0
	mov r1, #1
	bl ov12_02264FB0
	mov r2, #0xfa
	ldr r0, _0225A520 ; =ov12_0225DA8C
	add r1, r4, #0
	lsl r2, r2, #2
	bl SysTask_CreateOnMainQueue
	str r0, [r4, #0x10]
	pop {r4, r5, r6, pc}
	.balign 4, 0
_0225A520: .word ov12_0225DA8C
	thumb_func_end ov12_0225A4DC

	thumb_func_start ov12_0225A524
ov12_0225A524: ; 0x0225A524
	push {r3, r4, r5, r6, r7, lr}
	add r5, r0, #0
	str r1, [sp]
	mov r0, #5
	mov r1, #0x3c
	add r7, r2, #0
	bl Heap_Alloc
	add r4, r0, #0
	mov r0, #0
	add r1, r4, #0
	mov r2, #0x3c
	bl MIi_CpuClearFast
	mov r2, #0
	strb r2, [r4, #0xa]
	strb r2, [r4, #0xb]
	str r5, [r4]
	ldrb r0, [r7]
	mov r1, #0x65
	lsl r1, r1, #2
	strb r0, [r4, #8]
	ldr r0, [sp]
	add r5, r7, #0
	ldrb r0, [r0, r1]
	add r1, r1, #1
	add r6, r4, #0
	strb r0, [r4, #9]
	ldr r0, [sp]
	ldrb r1, [r0, r1]
	add r0, r4, #0
	add r0, #0x34
	strb r1, [r0]
	ldr r0, [sp]
	add r0, #0x28
	str r0, [r4, #4]
	add r0, r4, #0
	ldrb r1, [r7, #1]
	add r0, #0x23
	strb r1, [r0]
	mov r0, #0x24
	ldrsh r0, [r7, r0]
	strh r0, [r4, #0x36]
	ldrh r0, [r7, #0x26]
	strh r0, [r4, #0x38]
	add r0, r7, #0
	add r0, #0x28
	ldrb r1, [r0]
	add r0, r4, #0
	add r0, #0x3a
	strb r1, [r0]
	add r0, r7, #0
	add r0, #0x29
	ldrb r1, [r0]
	add r0, r4, #0
	add r0, #0x3b
	strb r1, [r0]
_0225A596:
	mov r3, #0
_0225A598:
	add r0, r5, r3
	ldrb r1, [r0, #8]
	add r0, r6, r3
	add r3, r3, #1
	strb r1, [r0, #0x10]
	cmp r3, #6
	blt _0225A598
	add r2, r2, #1
	add r5, r5, #6
	add r6, r6, #6
	cmp r2, #2
	blt _0225A596
	mov r0, #0
	add r3, r0, #0
_0225A5B4:
	add r2, r7, r0
	ldrb r1, [r2, #8]
	cmp r1, #2
	bne _0225A5C2
	add r1, r4, r0
	strb r3, [r1, #0x1c]
	b _0225A5C8
_0225A5C2:
	ldrb r2, [r2, #2]
	add r1, r4, r0
	strb r2, [r1, #0x1c]
_0225A5C8:
	add r0, r0, #1
	cmp r0, #6
	blt _0225A5B4
	mov r3, #0
	add r5, r7, #0
	add r6, r4, #0
_0225A5D4:
	ldrh r0, [r5, #0x14]
	add r2, r7, r3
	add r5, r5, #2
	strh r0, [r6, #0x24]
	ldrb r1, [r2, #0x1c]
	add r0, r4, r3
	add r0, #0x2c
	strb r1, [r0]
	add r0, r4, r3
	add r2, #0x20
	ldrb r1, [r2]
	add r0, #0x30
	add r3, r3, #1
	add r6, r6, #2
	strb r1, [r0]
	cmp r3, #4
	blt _0225A5D4
	ldr r0, [sp]
	add r1, r4, #0
	ldr r0, [r0]
	mov r2, #0
	bl SysTask_CreateOnMainQueue
	pop {r3, r4, r5, r6, r7, pc}
	thumb_func_end ov12_0225A524

	thumb_func_start ov12_0225A604
ov12_0225A604: ; 0x0225A604
	push {r3, r4, r5, r6, r7, lr}
	add r5, r0, #0
	str r1, [sp]
	mov r0, #5
	mov r1, #0x24
	add r7, r2, #0
	bl Heap_Alloc
	add r3, r0, #0
	mov r1, #0x65
	mov r4, #0
	add r0, #0x20
	strb r4, [r0]
	ldr r0, [sp]
	str r5, [r3]
	add r0, #0x94
	ldrb r0, [r0]
	lsl r1, r1, #2
	add r5, r7, #0
	strb r0, [r3, #0x1c]
	ldr r0, [sp]
	add r6, r3, #0
	ldrb r0, [r0, r1]
	add r1, r1, #1
	strb r0, [r3, #0x1d]
	ldr r0, [sp]
	ldrb r0, [r0, r1]
	strb r0, [r3, #0x1e]
	ldr r0, [sp]
	add r0, #0x28
	str r0, [r3, #4]
	ldrb r0, [r7, #1]
	strb r0, [r3, #0x1f]
_0225A646:
	ldrh r0, [r5, #4]
	add r2, r7, r4
	add r1, r3, r4
	strh r0, [r6, #0xc]
	ldrb r0, [r2, #0xc]
	add r4, r4, #1
	add r5, r5, #2
	strb r0, [r1, #0x14]
	ldrb r0, [r2, #0x10]
	add r6, r6, #2
	cmp r4, #4
	strb r0, [r1, #0x18]
	blt _0225A646
	ldrh r0, [r7, #2]
	add r1, r3, #0
	mov r2, #0
	strh r0, [r3, #0x22]
	ldr r0, [sp]
	ldr r0, [r0, #4]
	bl SysTask_CreateOnMainQueue
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
	thumb_func_end ov12_0225A604

	thumb_func_start ov12_0225A674
ov12_0225A674: ; 0x0225A674
	push {r4, r5, r6, r7, lr}
	sub sp, #0xc
	str r0, [sp]
	add r7, r1, #0
	mov r0, #5
	mov r1, #0x34
	add r5, r2, #0
	bl Heap_Alloc
	add r6, r0, #0
	mov r0, #0
	strb r0, [r6, #0xf]
	ldr r0, [sp]
	str r0, [r6]
	add r0, r7, #0
	add r0, #0x94
	ldrb r0, [r0]
	strb r0, [r6, #0xc]
	mov r0, #0x65
	lsl r0, r0, #2
	ldrb r1, [r7, r0]
	add r0, r0, #1
	strb r1, [r6, #0xd]
	ldrb r0, [r7, r0]
	strb r0, [r6, #0xe]
	ldrh r0, [r5, #2]
	strh r0, [r6, #0x30]
	add r0, r7, #0
	add r0, #0x28
	str r0, [r6, #4]
	add r0, r6, #0
	ldrb r1, [r5, #1]
	add r0, #0x32
	strb r1, [r0]
	ldr r0, [sp]
	add r1, sp, #4
	bl ov12_0223C1C4
	ldr r0, [sp]
	bl BattleSystem_GetMaxBattlers
	add r4, r0, #0
	ldr r0, [sp]
	bl BattleSystem_GetBattleType
	mov r0, #0
	cmp r4, #0
	ble _0225A6F0
	add r1, r6, #0
_0225A6D6:
	ldrh r2, [r5, #4]
	add r0, r0, #1
	strh r2, [r1, #0x10]
	ldrh r2, [r5, #6]
	strh r2, [r1, #0x12]
	ldrh r2, [r5, #8]
	strh r2, [r1, #0x14]
	ldrh r2, [r5, #0xa]
	add r5, #8
	strh r2, [r1, #0x16]
	add r1, #8
	cmp r0, r4
	blt _0225A6D6
_0225A6F0:
	ldr r0, [r7, #8]
	add r1, r6, #0
	mov r2, #0
	bl SysTask_CreateOnMainQueue
	add sp, #0xc
	pop {r4, r5, r6, r7, pc}
	.balign 4, 0
	thumb_func_end ov12_0225A674

	thumb_func_start ov12_0225A700
ov12_0225A700: ; 0x0225A700
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #8
	add r5, r0, #0
	str r1, [sp]
	mov r0, #5
	mov r1, #0x34
	str r2, [sp, #4]
	bl Heap_Alloc
	add r4, r0, #0
	mov r0, #5
	mov r1, #0x34
	bl Heap_Alloc
	str r0, [r4, #8]
	mov r0, #5
	mov r1, #0x38
	bl Heap_Alloc
	ldr r1, [r4, #8]
	str r0, [r1, #4]
	mov r0, #5
	bl SaveArray_Party_Alloc
	ldr r1, [r4, #8]
	mov r6, #0
	ldr r1, [r1, #4]
	ldr r3, [sp, #4]
	str r0, [r1]
	strb r6, [r4, #0xe]
	str r5, [r4]
	ldr r0, [sp, #4]
	mov r1, #0x65
	ldrb r0, [r0]
	lsl r1, r1, #2
	add r5, r4, #0
	strb r0, [r4, #0xc]
	ldr r0, [sp]
	ldrb r0, [r0, r1]
	add r1, r1, #1
	strb r0, [r4, #0xd]
	ldr r0, [sp]
	ldrb r0, [r0, r1]
	strb r0, [r4, #0xf]
	ldr r0, [sp, #4]
	ldrb r0, [r0, #1]
	strb r0, [r4, #0x14]
	ldr r0, [sp, #4]
	ldrb r0, [r0, #2]
	strb r0, [r4, #0x15]
	ldr r0, [sp, #4]
	ldrb r0, [r0, #3]
	strb r0, [r4, #0x16]
_0225A76A:
	ldr r0, [sp, #4]
	ldr r1, [r4, #8]
	add r7, r0, r6
	ldrb r0, [r7, #4]
	add r1, r1, r6
	mov r2, #0
	strb r0, [r1, #0xc]
_0225A778:
	add r0, r3, r2
	ldrb r1, [r0, #8]
	add r0, r5, r2
	add r2, r2, #1
	strb r1, [r0, #0x18]
	cmp r2, #6
	blt _0225A778
	add r7, #0x20
	ldrb r1, [r7]
	add r0, r4, r6
	add r0, #0x30
	add r6, r6, #1
	strb r1, [r0]
	add r3, r3, #6
	add r5, r5, #6
	cmp r6, #4
	blt _0225A76A
	ldr r0, [sp]
	add r1, r4, #0
	ldr r0, [r0, #0xc]
	mov r2, #0
	bl SysTask_CreateOnMainQueue
	add sp, #8
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
	thumb_func_end ov12_0225A700

	thumb_func_start ov12_0225A7AC
ov12_0225A7AC: ; 0x0225A7AC
	push {r3, r4, r5, r6, r7, lr}
	add r4, r0, #0
	str r1, [sp]
	mov r0, #5
	mov r1, #0x34
	add r7, r2, #0
	bl Heap_Alloc
	add r2, r0, #0
	mov r3, #0
	strb r3, [r2, #0xa]
	str r4, [r2]
	ldrb r0, [r7]
	add r5, r7, #0
	add r6, r2, #0
	strb r0, [r2, #8]
	ldrb r0, [r7, #1]
	strb r0, [r2, #9]
	ldrb r0, [r7, #2]
	strb r0, [r2, #0xb]
	ldr r0, [r7, #0x20]
	str r0, [r2, #0x10]
	ldrb r0, [r7, #3]
	strb r0, [r2, #0x16]
	add r0, r7, #0
	strh r3, [r2, #0x14]
	add r0, #0x24
	ldrb r0, [r0]
	strb r0, [r2, #0x18]
_0225A7E6:
	add r0, r7, r3
	ldrb r1, [r0, #4]
	add r0, r2, r3
	mov r4, #0
	strb r1, [r0, #0xc]
_0225A7F0:
	add r0, r5, r4
	ldrb r1, [r0, #8]
	add r0, r6, r4
	add r4, r4, #1
	strb r1, [r0, #0x1c]
	cmp r4, #6
	blt _0225A7F0
	add r3, r3, #1
	add r5, r5, #6
	add r6, r6, #6
	cmp r3, #4
	blt _0225A7E6
	ldr r0, [sp]
	add r1, r2, #0
	ldr r0, [r0, #0x10]
	mov r2, #0
	bl SysTask_CreateOnMainQueue
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
	thumb_func_end ov12_0225A7AC

	thumb_func_start ov12_0225A818
ov12_0225A818: ; 0x0225A818
	push {r4, r5, r6, lr}
	add r6, r0, #0
	add r5, r1, #0
	mov r0, #5
	mov r1, #0x1c
	add r4, r2, #0
	bl Heap_Alloc
	add r1, r0, #0
	mov r2, #0
	strb r2, [r1, #0xe]
	str r6, [r1]
	ldrb r0, [r4]
	strb r0, [r1, #0xc]
	mov r0, #0x65
	lsl r0, r0, #2
	ldrb r0, [r5, r0]
	strb r0, [r1, #0xd]
	add r0, r5, #0
	add r0, #0x28
	str r0, [r1, #4]
	ldrh r0, [r4, #2]
	str r0, [r1, #0x10]
	ldrb r0, [r4, #1]
	strb r0, [r1, #0xf]
	ldr r0, [r4, #4]
	strh r0, [r1, #0x18]
	ldr r0, [r4, #8]
	str r0, [r1, #0x14]
	ldr r0, [r5, #0x14]
	bl SysTask_CreateOnMainQueue
	pop {r4, r5, r6, pc}
	.balign 4, 0
	thumb_func_end ov12_0225A818

	thumb_func_start ov12_0225A85C
ov12_0225A85C: ; 0x0225A85C
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x28
	add r6, r0, #0
	add r7, r1, #0
	add r5, r2, #0
	bl ov12_0223A934
	str r0, [sp]
	mov r0, #5
	mov r1, #8
	bl Heap_Alloc
	add r4, r0, #0
	str r6, [r4]
	ldrb r0, [r5]
	mov r1, #0x65
	lsl r1, r1, #2
	strb r0, [r4, #4]
	ldrb r0, [r7, r1]
	strb r0, [r4, #5]
	ldrh r2, [r5, #2]
	lsl r0, r2, #1
	add r2, r2, r0
	add r0, sp, #4
	strh r2, [r0, #2]
	mov r2, #2
	strb r2, [r0, #1]
	ldrb r0, [r5, #1]
	ldrb r1, [r7, r1]
	lsl r0, r0, #8
	orr r0, r1
	str r0, [sp, #8]
	add r0, r6, #0
	bl BattleSystem_GetTextFrameDelay
	add r3, r0, #0
	ldr r1, [sp]
	add r0, r6, #0
	add r2, sp, #4
	bl BattleSystem_PrintBattleMessage
	strb r0, [r4, #6]
	ldr r0, _0225A8C0 ; =ov12_022605D0
	add r1, r4, #0
	mov r2, #0
	bl SysTask_CreateOnMainQueue
	add sp, #0x28
	pop {r3, r4, r5, r6, r7, pc}
	nop
_0225A8C0: .word ov12_022605D0
	thumb_func_end ov12_0225A85C

	thumb_func_start ov12_0225A8C4
ov12_0225A8C4: ; 0x0225A8C4
	push {r3, r4, r5, r6, r7, lr}
	add r5, r0, #0
	add r6, r1, #0
	add r7, r2, #0
	bl BattleSystem_GetMessageLoader
	str r0, [sp]
	mov r0, #5
	mov r1, #8
	bl Heap_Alloc
	add r4, r0, #0
	add r0, r6, #0
	str r5, [r4]
	add r0, #0x94
	ldrb r0, [r0]
	strb r0, [r4, #4]
	mov r0, #0x65
	lsl r0, r0, #2
	ldrb r0, [r6, r0]
	strb r0, [r4, #5]
	add r0, r5, #0
	bl BattleSystem_GetTextFrameDelay
	add r3, r0, #0
	ldr r1, [sp]
	add r0, r5, #0
	add r2, r7, #0
	bl BattleSystem_PrintBattleMessage
	strb r0, [r4, #6]
	ldr r0, _0225A910 ; =ov12_022605D0
	add r1, r4, #0
	mov r2, #0
	bl SysTask_CreateOnMainQueue
	pop {r3, r4, r5, r6, r7, pc}
	nop
_0225A910: .word ov12_022605D0
	thumb_func_end ov12_0225A8C4

	thumb_func_start ov12_0225A914
ov12_0225A914: ; 0x0225A914
	push {r3, r4, r5, r6, r7, lr}
	add r7, r0, #0
	add r5, r1, #0
	mov r0, #5
	mov r1, #0x70
	add r4, r2, #0
	bl Heap_Alloc
	add r6, r0, #0
	mov r1, #0
	add r0, #0x6a
	strb r1, [r0]
	str r7, [r6]
	add r0, r5, #0
	str r5, [r6, #4]
	add r0, #0x94
	ldrb r1, [r0]
	add r0, r6, #0
	add r0, #0x68
	strb r1, [r0]
	mov r0, #0x65
	lsl r0, r0, #2
	ldrb r1, [r5, r0]
	add r0, r6, #0
	add r0, #0x69
	strb r1, [r0]
	add r0, r7, #0
	bl ov12_0223A8DC
	add r3, r6, #0
	str r0, [r6, #0xc]
	add r7, r4, #0
	add r3, #0x10
	mov r2, #0xb
_0225A958:
	ldmia r7!, {r0, r1}
	stmia r3!, {r0, r1}
	sub r2, r2, #1
	bne _0225A958
	ldr r0, [r5, #0x20]
	str r0, [r6, #8]
	ldr r0, [r4, #0x4c]
	cmp r0, #1
	bne _0225A978
	ldr r0, [r4, #0x50]
	cmp r0, #0x19
	bne _0225A978
	mov r0, #0x1a
	mov r1, #1
	lsl r0, r0, #4
	str r1, [r5, r0]
_0225A978:
	ldr r0, [r4, #0x4c]
	cmp r0, #1
	bne _0225A98C
	ldr r0, [r4, #0x50]
	cmp r0, #0x1a
	bne _0225A98C
	mov r0, #0x1a
	mov r1, #0
	lsl r0, r0, #4
	str r1, [r5, r0]
_0225A98C:
	ldrh r0, [r4, #2]
	add r1, r6, #0
	add r1, #0x6c
	str r0, [sp]
	add r0, r6, #0
	ldr r2, [r4, #0x4c]
	ldr r3, [r4, #0x50]
	add r0, #0x6b
	bl ov12_02261D30
	ldr r0, _0225A9AC ; =ov12_0225FD14
	add r1, r6, #0
	mov r2, #0
	bl SysTask_CreateOnMainQueue
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
_0225A9AC: .word ov12_0225FD14
	thumb_func_end ov12_0225A914
