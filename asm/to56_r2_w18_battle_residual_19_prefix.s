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

	thumb_func_start ov12_0225F4E0
ov12_0225F4E0: ; 0x0225F4E0
	push {r4, r5, r6, r7, lr}
	sub sp, #0x24
	add r4, r1, #0
	add r6, r0, #0
	ldr r0, [r4]
	bl BattleSystem_GetPaletteData
	ldrb r1, [r4, #0xa]
	add r5, r0, #0
	cmp r1, #3
	bhi _0225F57C
	add r1, r1, r1
	add r1, pc
	ldrh r1, [r1, #6]
	lsl r1, r1, #0x10
	asr r1, r1, #0x10
	add pc, r1
_0225F502: ; jump table
	.short _0225F50A - _0225F502 - 2 ; case 0
	.short _0225F574 - _0225F502 - 2 ; case 1
	.short _0225F7EA - _0225F502 - 2 ; case 2
	.short _0225F846 - _0225F502 - 2 ; case 3
_0225F50A:
	ldr r0, [r4]
	mov r1, #0
	bl BattleSystem_GetWindow
	add r6, r0, #0
	mov r1, #0xff
	bl FillWindowPixelBuffer
	add r0, r6, #0
	bl CopyWindowPixelsToVram_TextMode
	ldr r0, [r4]
	bl BattleSystem_GetBattleInput
	bl BattleInput_GetKeyPressed
	strb r0, [r4, #0x17]
	ldr r0, [r4]
	bl BattleSystem_GetMessageIcon
	mov r1, #1
	bl sub_0201649C
	mov r1, #0
	mov r2, #3
	str r1, [sp]
	mov r0, #7
	str r0, [sp, #4]
	str r1, [sp, #8]
	mov r1, #5
	add r3, r1, #0
	add r0, r5, #0
	lsl r2, r2, #0xa
	sub r3, #0xd
	bl PaletteData_BeginPaletteFade
	mov r1, #0
	str r1, [sp]
	mov r0, #0x10
	str r0, [sp, #4]
	str r1, [sp, #8]
	mov r1, #0xa
	add r3, r1, #0
	ldr r2, _0225F870 ; =0x0000FFFF
	add r0, r5, #0
	sub r3, #0x12
	bl PaletteData_BeginPaletteFade
	ldrb r0, [r4, #0xa]
	add sp, #0x24
	add r0, r0, #1
	strb r0, [r4, #0xa]
	pop {r4, r5, r6, r7, pc}
_0225F574:
	bl PaletteData_GetSelectedBuffersBitmask
	cmp r0, #0
	beq _0225F57E
_0225F57C:
	b _0225F8A6
_0225F57E:
	ldr r0, [r4]
	bl ov12_02237B0C
	mov r0, #5
	mov r1, #0x38
	bl Heap_Alloc
	str r0, [r4, #4]
	mov r0, #5
	bl SaveArray_Party_Alloc
	ldr r1, [r4, #4]
	str r0, [r1]
	ldr r0, [r4]
	bl BattleSystem_GetBattleType
	mov r1, #0xc
	and r0, r1
	cmp r0, #0xc
	beq _0225F5B2
	ldr r0, [r4]
	bl BattleSystem_GetBattleType
	cmp r0, #0xcb
	beq _0225F5B2
	b _0225F6C6
_0225F5B2:
	ldrb r1, [r4, #9]
	ldr r0, [r4]
	bl ov12_0223AB0C
	cmp r0, #2
	bne _0225F5CE
	ldrb r0, [r4, #9]
	str r0, [sp, #0x18]
	ldr r0, [r4]
	ldr r1, [sp, #0x18]
	bl BattleSystem_GetBattlerIdPartner
	str r0, [sp, #0x14]
	b _0225F5DC
_0225F5CE:
	ldrb r1, [r4, #9]
	ldr r0, [r4]
	bl BattleSystem_GetBattlerIdPartner
	str r0, [sp, #0x18]
	ldrb r0, [r4, #9]
	str r0, [sp, #0x14]
_0225F5DC:
	mov r0, #5
	bl AllocMonZeroed
	add r6, r0, #0
	mov r5, #0
_0225F5E6:
	ldr r0, [r4, #4]
	add r1, r6, #0
	ldr r0, [r0]
	bl Party_AddMon
	add r5, r5, #1
	cmp r5, #6
	blt _0225F5E6
	add r0, r6, #0
	bl Heap_Free
	ldr r0, [r4]
	ldr r1, [sp, #0x18]
	mov r7, #0
	bl BattleSystem_GetPartySize
	cmp r0, #0
	ble _0225F64E
	ldr r0, [sp, #0x18]
	mov r1, #6
	mul r1, r0
	add r5, r4, r1
	add r6, r7, #0
_0225F614:
	ldrb r2, [r5, #0x1c]
	ldr r0, [r4]
	ldr r1, [sp, #0x18]
	bl BattleSystem_GetPartyMon
	str r0, [sp, #0x1c]
	ldr r0, [r4, #4]
	add r1, r6, #0
	ldr r0, [r0]
	bl Party_GetMonByIndex
	add r1, r0, #0
	ldr r0, [sp, #0x1c]
	bl CopyPokemonToPokemon
	ldr r0, [r4, #4]
	ldrb r1, [r5, #0x1c]
	add r0, r0, r6
	add r0, #0x2c
	strb r1, [r0]
	ldr r0, [r4]
	ldr r1, [sp, #0x18]
	add r5, r5, #1
	add r6, r6, #2
	add r7, r7, #1
	bl BattleSystem_GetPartySize
	cmp r7, r0
	blt _0225F614
_0225F64E:
	ldr r0, [r4]
	ldr r1, [sp, #0x14]
	mov r6, #0
	bl BattleSystem_GetPartySize
	cmp r0, #0
	ble _0225F6AA
	ldr r0, [sp, #0x14]
	mov r1, #6
	mul r1, r0
	mov r0, #1
	add r5, r4, r1
	str r0, [sp, #0xc]
	add r7, r6, #0
_0225F66A:
	ldrb r2, [r5, #0x1c]
	ldr r0, [r4]
	ldr r1, [sp, #0x14]
	bl BattleSystem_GetPartyMon
	str r0, [sp, #0x20]
	ldr r0, [r4, #4]
	ldr r1, [sp, #0xc]
	ldr r0, [r0]
	bl Party_GetMonByIndex
	add r1, r0, #0
	ldr r0, [sp, #0x20]
	bl CopyPokemonToPokemon
	ldr r0, [r4, #4]
	ldrb r1, [r5, #0x1c]
	add r0, r0, r7
	add r0, #0x2d
	strb r1, [r0]
	ldr r0, [sp, #0xc]
	ldr r1, [sp, #0x14]
	add r0, r0, #2
	str r0, [sp, #0xc]
	ldr r0, [r4]
	add r5, r5, #1
	add r7, r7, #2
	add r6, r6, #1
	bl BattleSystem_GetPartySize
	cmp r6, r0
	blt _0225F66A
_0225F6AA:
	ldrb r1, [r4, #9]
	ldr r0, [r4]
	bl ov12_0223AB0C
	cmp r0, #4
	bne _0225F6BE
	ldr r0, [r4, #4]
	mov r1, #1
	strb r1, [r0, #0x11]
	b _0225F746
_0225F6BE:
	ldr r0, [r4, #4]
	mov r1, #0
	strb r1, [r0, #0x11]
	b _0225F746
_0225F6C6:
	ldr r0, [r4]
	bl BattleSystem_GetBattleType
	mov r1, #2
	tst r0, r1
	beq _0225F6E8
	ldr r0, [r4]
	bl BattleSystem_GetBattleType
	mov r1, #8
	tst r0, r1
	bne _0225F6E8
	ldrb r1, [r4, #9]
	mov r0, #1
	add r7, r1, #0
	and r7, r0
	b _0225F6EC
_0225F6E8:
	ldrb r1, [r4, #9]
	add r7, r1, #0
_0225F6EC:
	ldr r0, [r4]
	bl ov12_0223AB0C
	cmp r0, #4
	bne _0225F6FA
	mov r1, #1
	b _0225F6FC
_0225F6FA:
	mov r1, #0
_0225F6FC:
	ldr r0, [r4, #4]
	strb r1, [r0, #0x11]
	ldrb r1, [r4, #9]
	ldr r0, [r4]
	bl BattleSystem_GetParty
	str r0, [sp, #0x10]
	mov r6, #0
	bl Party_GetCount
	cmp r0, #0
	ble _0225F746
	mov r0, #6
	mul r0, r7
	add r5, r4, r0
_0225F71A:
	ldrb r2, [r5, #0x1c]
	ldr r0, [r4]
	add r1, r7, #0
	bl BattleSystem_GetPartyMon
	add r1, r0, #0
	ldr r0, [r4, #4]
	ldr r0, [r0]
	bl Party_AddMon
	ldr r0, [r4, #4]
	ldrb r1, [r5, #0x1c]
	add r0, r0, r6
	add r0, #0x2c
	strb r1, [r0]
	ldr r0, [sp, #0x10]
	add r5, r5, #1
	add r6, r6, #1
	bl Party_GetCount
	cmp r6, r0
	blt _0225F71A
_0225F746:
	ldr r1, [r4]
	ldr r0, [r4, #4]
	str r1, [r0, #8]
	ldr r0, [r4, #4]
	mov r1, #5
	str r1, [r0, #0xc]
	ldr r0, [r4, #4]
	mov r1, #0
	add r0, #0x36
	strb r1, [r0]
	ldr r1, [r4, #0x10]
	ldr r0, [r4, #4]
	strh r1, [r0, #0x24]
	ldrb r1, [r4, #0x16]
	ldr r0, [r4, #4]
	strb r1, [r0, #0x12]
	ldr r0, [r4, #4]
	ldrb r1, [r4, #0xb]
	add r0, #0x35
	strb r1, [r0]
	ldrh r1, [r4, #0x14]
	ldr r0, [r4, #4]
	strh r1, [r0, #0x22]
	ldrb r1, [r4, #9]
	ldr r0, [r4, #4]
	str r1, [r0, #0x28]
	ldr r0, [r4, #4]
	ldrb r1, [r4, #0x17]
	add r0, #0x32
	strb r1, [r0]
	ldrb r0, [r4, #9]
	bl MaskOfFlagNo
	ldrb r1, [r4, #0x18]
	tst r0, r1
	bne _0225F796
	ldrb r0, [r4, #9]
	add r0, r4, r0
	ldrb r1, [r0, #0xc]
	b _0225F798
_0225F796:
	mov r1, #6
_0225F798:
	ldr r0, [r4, #4]
	strb r1, [r0, #0x14]
	ldr r0, [r4]
	bl BattleSystem_GetBattleType
	mov r1, #8
	tst r0, r1
	beq _0225F7B0
	ldr r0, [r4, #4]
	mov r1, #6
	strb r1, [r0, #0x15]
	b _0225F7DA
_0225F7B0:
	ldrb r1, [r4, #9]
	ldr r0, [r4]
	bl BattleSystem_GetBattlerIdPartner
	bl MaskOfFlagNo
	ldrb r1, [r4, #0x18]
	tst r0, r1
	bne _0225F7D4
	ldrb r1, [r4, #9]
	ldr r0, [r4]
	bl BattleSystem_GetBattlerIdPartner
	add r0, r4, r0
	ldrb r1, [r0, #0xc]
	ldr r0, [r4, #4]
	strb r1, [r0, #0x15]
	b _0225F7DA
_0225F7D4:
	ldr r0, [r4, #4]
	mov r1, #6
	strb r1, [r0, #0x15]
_0225F7DA:
	ldr r0, [r4, #4]
	bl ov10_0221BE20
	ldrb r0, [r4, #0xa]
	add sp, #0x24
	add r0, r0, #1
	strb r0, [r4, #0xa]
	pop {r4, r5, r6, r7, pc}
_0225F7EA:
	ldr r0, [r4, #4]
	add r0, #0x36
	ldrb r0, [r0]
	cmp r0, #0
	beq _0225F8A6
	ldr r0, [r4]
	bl ov12_02237BB8
	ldr r0, [r4]
	bl BattleSystem_GetBattleInput
	ldr r1, [r4, #4]
	add r1, #0x32
	ldrb r1, [r1]
	bl BattleInput_SetKeyPressed
	mov r0, #7
	str r0, [sp]
	mov r0, #0
	mov r1, #5
	str r0, [sp, #4]
	mov r2, #3
	add r3, r1, #0
	str r0, [sp, #8]
	add r0, r5, #0
	lsl r2, r2, #0xa
	sub r3, #0xd
	bl PaletteData_BeginPaletteFade
	mov r0, #0x10
	str r0, [sp]
	mov r0, #0
	mov r1, #0xa
	str r0, [sp, #4]
	add r3, r1, #0
	str r0, [sp, #8]
	ldr r2, _0225F870 ; =0x0000FFFF
	add r0, r5, #0
	sub r3, #0x12
	bl PaletteData_BeginPaletteFade
	ldrb r0, [r4, #0xa]
	add sp, #0x24
	add r0, r0, #1
	strb r0, [r4, #0xa]
	pop {r4, r5, r6, r7, pc}
_0225F846:
	bl PaletteData_GetSelectedBuffersBitmask
	cmp r0, #0
	bne _0225F8A6
	ldr r0, [r4]
	bl BattleSystem_GetMessageIcon
	mov r1, #0
	bl sub_0201649C
	ldr r3, [r4, #4]
	ldr r0, [r4]
	ldrb r2, [r3, #0x11]
	cmp r2, #6
	bne _0225F874
	ldrb r1, [r4, #9]
	mov r2, #0xff
	bl ov12_02263360
	b _0225F882
	nop
_0225F870: .word 0x0000FFFF
_0225F874:
	add r2, r3, r2
	add r2, #0x2c
	ldrb r2, [r2]
	ldrb r1, [r4, #9]
	add r2, r2, #1
	bl ov12_02263360
_0225F882:
	ldrb r1, [r4, #9]
	ldrb r2, [r4, #8]
	ldr r0, [r4]
	bl ov12_0226430C
	ldr r0, [r4, #4]
	ldr r0, [r0]
	bl Heap_Free
	ldr r0, [r4, #4]
	bl Heap_Free
	add r0, r4, #0
	bl Heap_Free
	add r0, r6, #0
	bl SysTask_Destroy
_0225F8A6:
	add sp, #0x24
	pop {r4, r5, r6, r7, pc}
	.balign 4, 0
	thumb_func_end ov12_0225F4E0
