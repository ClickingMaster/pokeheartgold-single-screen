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

	thumb_func_start ov12_0225E760
ov12_0225E760: ; 0x0225E760
	push {r3, r4, r5, r6, r7, lr}
	add r4, r1, #0
	ldrb r1, [r4, #0xd]
	add r7, r0, #0
	ldr r0, [r4]
	add r2, sp, #0
	bl ov12_0223BE0C
	cmp r0, #1
	bne _0225E77A
	ldr r0, [r4]
	bl ov12_02261ED4
_0225E77A:
	add r0, sp, #0
	ldrb r0, [r0]
	cmp r0, #0
	beq _0225E786
	cmp r0, #4
	bls _0225E78E
_0225E786:
	ldr r0, [r4]
	bl ov12_02261EB8
	b _0225E80C
_0225E78E:
	sub r5, r0, #1
	ldrh r0, [r4, #0x30]
	cmp r0, #0x40
	bgt _0225E7C4
	bge _0225E80C
	cmp r0, #0x10
	bgt _0225E7C0
	bge _0225E80C
	cmp r0, #8
	bhi _0225E80C
	add r1, r0, r0
	add r1, pc
	ldrh r1, [r1, #6]
	lsl r1, r1, #0x10
	asr r1, r1, #0x10
	add pc, r1
_0225E7AE: ; jump table
	.short _0225E7E2 - _0225E7AE - 2 ; case 0
	.short _0225E80C - _0225E7AE - 2 ; case 1
	.short _0225E80C - _0225E7AE - 2 ; case 2
	.short _0225E80C - _0225E7AE - 2 ; case 3
	.short _0225E80C - _0225E7AE - 2 ; case 4
	.short _0225E80C - _0225E7AE - 2 ; case 5
	.short _0225E80C - _0225E7AE - 2 ; case 6
	.short _0225E80C - _0225E7AE - 2 ; case 7
	.short _0225E80C - _0225E7AE - 2 ; case 8
_0225E7C0:
	cmp r0, #0x20
	b _0225E80C
_0225E7C4:
	mov r2, #1
	lsl r2, r2, #8
	cmp r0, r2
	bgt _0225E7D2
	bge _0225E80C
	cmp r0, #0x80
	b _0225E80C
_0225E7D2:
	lsl r1, r2, #1
	cmp r0, r1
	bgt _0225E7DC
	beq _0225E7F0
	b _0225E80C
_0225E7DC:
	lsl r1, r2, #2
	cmp r0, r1
	b _0225E80C
_0225E7E2:
	ldrb r0, [r4, #0xd]
	cmp r0, r5
	bne _0225E80C
	ldr r0, [r4]
	bl ov12_02261EB8
	b _0225E80C
_0225E7F0:
	ldrb r1, [r4, #0xd]
	ldr r0, [r4]
	bl BattleSystem_GetFieldSide
	add r6, r0, #0
	ldr r0, [r4]
	add r1, r5, #0
	bl BattleSystem_GetFieldSide
	cmp r6, r0
	beq _0225E80C
	ldr r0, [r4]
	bl ov12_02261EB8
_0225E80C:
	add r2, sp, #0
	ldrb r1, [r4, #0xd]
	ldrb r2, [r2]
	ldr r0, [r4]
	bl ov12_0226311C
	ldrb r1, [r4, #0xd]
	ldrb r2, [r4, #0xc]
	ldr r0, [r4]
	bl ov12_0226430C
	add r0, r4, #0
	bl Heap_Free
	add r0, r7, #0
	bl SysTask_Destroy
	pop {r3, r4, r5, r6, r7, pc}
	thumb_func_end ov12_0225E760

	thumb_func_start ov12_0225E830
ov12_0225E830: ; 0x0225E830
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x1fc
	sub sp, #0x14
	add r4, r1, #0
	add r7, r0, #0
	ldr r0, [r4]
	bl BattleSystem_GetPaletteData
	ldrb r1, [r4, #0xe]
	add r5, r0, #0
	cmp r1, #0x1e
	bls _0225E84C
	bl _0225F376
_0225E84C:
	add r1, r1, r1
	add r1, pc
	ldrh r1, [r1, #6]
	lsl r1, r1, #0x10
	asr r1, r1, #0x10
	add pc, r1
_0225E858: ; jump table
	.short _0225E896 - _0225E858 - 2 ; case 0
	.short _0225E8EC - _0225E858 - 2 ; case 1
	.short _0225E978 - _0225E858 - 2 ; case 2
	.short _0225E98C - _0225E858 - 2 ; case 3
	.short _0225E9EA - _0225E858 - 2 ; case 4
	.short _0225EB32 - _0225E858 - 2 ; case 5
	.short _0225EB6E - _0225E858 - 2 ; case 6
	.short _0225EBBE - _0225E858 - 2 ; case 7
	.short _0225ED62 - _0225E858 - 2 ; case 8
	.short _0225EDE0 - _0225E858 - 2 ; case 9
	.short _0225EE1C - _0225E858 - 2 ; case 10
	.short _0225EE3E - _0225E858 - 2 ; case 11
	.short _0225F326 - _0225E858 - 2 ; case 12
	.short _0225EEC4 - _0225E858 - 2 ; case 13
	.short _0225F326 - _0225E858 - 2 ; case 14
	.short _0225EF78 - _0225E858 - 2 ; case 15
	.short _0225F354 - _0225E858 - 2 ; case 16
	.short _0225EE7E - _0225E858 - 2 ; case 17
	.short _0225F326 - _0225E858 - 2 ; case 18
	.short _0225F02A - _0225E858 - 2 ; case 19
	.short _0225F326 - _0225E858 - 2 ; case 20
	.short _0225F070 - _0225E858 - 2 ; case 21
	.short _0225F10A - _0225E858 - 2 ; case 22
	.short _0225F140 - _0225E858 - 2 ; case 23
	.short _0225F354 - _0225E858 - 2 ; case 24
	.short _0225EE7E - _0225E858 - 2 ; case 25
	.short _0225F326 - _0225E858 - 2 ; case 26
	.short _0225F02A - _0225E858 - 2 ; case 27
	.short _0225F326 - _0225E858 - 2 ; case 28
	.short _0225F1B2 - _0225E858 - 2 ; case 29
	.short _0225F354 - _0225E858 - 2 ; case 30
_0225E896:
	ldr r0, [r4]
	bl BattleSystem_GetBattleInput
	bl BattleInput_GetKeyPressed
	strb r0, [r4, #0x10]
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
	ldr r2, _0225EC14 ; =0x0000FFFF
	add r0, r5, #0
	sub r3, #0x12
	bl PaletteData_BeginPaletteFade
	ldrb r0, [r4, #0xe]
	add sp, #0x1fc
	add sp, #0x14
	add r0, r0, #1
	strb r0, [r4, #0xe]
	pop {r3, r4, r5, r6, r7, pc}
_0225E8EC:
	bl PaletteData_GetSelectedBuffersBitmask
	cmp r0, #0
	bne _0225E9B4
	ldr r0, [r4]
	bl ov12_02237B0C
	mov r0, #5
	mov r1, #0x34
	bl Heap_Alloc
	str r0, [r4, #4]
	mov r1, #0
	mov r2, #0x34
	bl memset
	ldr r1, [r4]
	ldr r0, [r4, #4]
	str r1, [r0]
	ldrb r1, [r4, #0xd]
	ldr r0, [r4]
	bl BattleSystem_GetPlayerProfile
	ldr r1, [r4, #4]
	str r0, [r1, #4]
	ldr r0, [r4, #4]
	mov r1, #5
	str r1, [r0, #0xc]
	ldr r0, [r4, #4]
	mov r1, #0
	add r0, #0x26
	strb r1, [r0]
	ldr r0, [r4]
	bl BattleSystem_GetBag
	ldr r1, [r4, #4]
	str r0, [r1, #8]
	ldrb r1, [r4, #0xd]
	ldr r0, [r4, #4]
	str r1, [r0, #0x10]
	ldr r0, [r4, #4]
	ldrb r1, [r4, #0x10]
	add r0, #0x25
	strb r1, [r0]
	ldr r0, [r4, #4]
	ldrb r1, [r4, #0x14]
	add r0, #0x22
	strb r1, [r0]
	ldr r0, [r4, #4]
	ldrb r1, [r4, #0x15]
	add r0, #0x23
	strb r1, [r0]
	ldr r0, [r4, #4]
	ldrb r1, [r4, #0x16]
	add r0, #0x24
	strb r1, [r0]
	ldrb r0, [r4, #0xd]
	add r0, r4, r0
	add r0, #0x30
	ldrb r1, [r0]
	ldr r0, [r4, #4]
	str r1, [r0, #0x18]
	ldr r0, [r4, #4]
	bl ov08_022225D4
	add sp, #0x1fc
	mov r0, #3
	add sp, #0x14
	strb r0, [r4, #0xe]
	pop {r3, r4, r5, r6, r7, pc}
_0225E978:
	ldr r0, [r4, #4]
	ldrb r1, [r4, #0x10]
	add r0, #0x25
	strb r1, [r0]
	ldr r0, [r4, #4]
	bl ov08_022225D4
	ldrb r0, [r4, #0xe]
	add r0, r0, #1
	strb r0, [r4, #0xe]
_0225E98C:
	ldr r1, [r4, #4]
	add r0, r1, #0
	add r0, #0x26
	ldrb r0, [r0]
	cmp r0, #0
	beq _0225E9B4
	mov r0, #0
	add r1, #0x26
	strb r0, [r1]
	ldr r0, [r4, #4]
	add r0, #0x25
	ldrb r0, [r0]
	strb r0, [r4, #0x10]
	ldr r1, [r4, #4]
	ldrh r0, [r1, #0x1c]
	cmp r0, #0
	beq _0225E9E0
	ldrb r0, [r1, #0x1e]
	cmp r0, #3
	bls _0225E9B8
_0225E9B4:
	bl _0225F376
_0225E9B8:
	add r0, r0, r0
	add r0, pc
	ldrh r0, [r0, #6]
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	add pc, r0
_0225E9C4: ; jump table
	.short _0225E9CC - _0225E9C4 - 2 ; case 0
	.short _0225E9CC - _0225E9C4 - 2 ; case 1
	.short _0225E9D6 - _0225E9C4 - 2 ; case 2
	.short _0225E9D6 - _0225E9C4 - 2 ; case 3
_0225E9CC:
	add sp, #0x1fc
	mov r0, #4
	add sp, #0x14
	strb r0, [r4, #0xe]
	pop {r3, r4, r5, r6, r7, pc}
_0225E9D6:
	add sp, #0x1fc
	mov r0, #6
	add sp, #0x14
	strb r0, [r4, #0xe]
	pop {r3, r4, r5, r6, r7, pc}
_0225E9E0:
	add sp, #0x1fc
	mov r0, #6
	add sp, #0x14
	strb r0, [r4, #0xe]
	pop {r3, r4, r5, r6, r7, pc}
_0225E9EA:
	ldrb r1, [r4, #0xd]
	ldr r0, [r4]
	bl BattleSystem_GetParty
	str r0, [sp, #0x14]
	ldr r0, [r4]
	bl BattleSystem_GetBattleType
	mov r1, #2
	tst r0, r1
	beq _0225EA16
	ldr r0, [r4]
	bl BattleSystem_GetBattleType
	mov r1, #8
	tst r0, r1
	bne _0225EA16
	ldrb r1, [r4, #0xd]
	mov r0, #1
	add r7, r1, #0
	and r7, r0
	b _0225EA18
_0225EA16:
	ldrb r7, [r4, #0xd]
_0225EA18:
	ldr r0, [r4, #8]
	mov r1, #6
	ldr r0, [r0, #4]
	ldr r0, [r0]
	bl Party_InitWithMaxSize
	ldr r0, [sp, #0x14]
	mov r6, #0
	bl Party_GetCount
	cmp r0, #0
	ble _0225EA66
	mov r0, #6
	mul r0, r7
	add r5, r4, r0
_0225EA36:
	ldrb r2, [r5, #0x18]
	ldr r0, [r4]
	add r1, r7, #0
	bl BattleSystem_GetPartyMon
	add r1, r0, #0
	ldr r0, [r4, #8]
	ldr r0, [r0, #4]
	ldr r0, [r0]
	bl Party_AddMon
	ldr r1, [r4, #8]
	ldrb r0, [r5, #0x18]
	ldr r1, [r1, #4]
	add r5, r5, #1
	add r1, r1, r6
	add r1, #0x2c
	strb r0, [r1]
	ldr r0, [sp, #0x14]
	add r6, r6, #1
	bl Party_GetCount
	cmp r6, r0
	blt _0225EA36
_0225EA66:
	ldr r0, [r4, #8]
	ldr r1, [r4]
	ldr r0, [r0, #4]
	str r1, [r0, #8]
	ldr r0, [r4, #8]
	mov r1, #5
	ldr r0, [r0, #4]
	str r1, [r0, #0xc]
	ldr r1, [r4, #8]
	mov r0, #0
	ldr r1, [r1, #4]
	strb r0, [r1, #0x11]
	ldr r1, [r4, #8]
	ldr r1, [r1, #4]
	add r1, #0x36
	strb r0, [r1]
	ldr r1, [r4, #8]
	ldr r1, [r1, #4]
	strh r0, [r1, #0x24]
	ldr r0, [r4, #8]
	mov r1, #2
	ldr r0, [r0, #4]
	add r0, #0x35
	strb r1, [r0]
	ldr r0, [r4, #4]
	ldrh r1, [r0, #0x1c]
	ldr r0, [r4, #8]
	ldr r0, [r0, #4]
	strh r1, [r0, #0x22]
	ldr r0, [r4, #4]
	ldrb r1, [r0, #0x1e]
	ldr r0, [r4, #8]
	ldr r0, [r0, #4]
	add r0, #0x33
	strb r1, [r0]
	ldr r0, [r4, #4]
	ldr r1, [r0, #0x10]
	ldr r0, [r4, #8]
	ldr r0, [r0, #4]
	str r1, [r0, #0x28]
	ldr r0, [r4, #8]
	ldrb r1, [r4, #0x10]
	ldr r0, [r0, #4]
	add r0, #0x32
	strb r1, [r0]
	ldrb r0, [r4, #0xd]
	ldr r2, [r4, #8]
	add r0, r2, r0
	ldrb r1, [r0, #0xc]
	ldr r0, [r2, #4]
	strb r1, [r0, #0x14]
	ldrb r1, [r4, #0xd]
	ldr r5, [r4, #8]
	ldr r0, [r4]
	bl BattleSystem_GetBattlerIdPartner
	add r0, r5, r0
	ldrb r1, [r0, #0xc]
	ldr r0, [r5, #4]
	strb r1, [r0, #0x15]
	ldrb r0, [r4, #0xf]
	cmp r0, #4
	bne _0225EAFC
	ldrb r1, [r4, #0xd]
	ldr r0, [r4]
	bl BattleSystem_GetBattlerIdPartner
	add r0, r4, r0
	add r0, #0x30
	ldrb r1, [r0]
	ldr r0, [r4, #8]
	ldr r0, [r0, #4]
	str r1, [r0, #0x18]
	ldrb r0, [r4, #0xd]
	b _0225EB12
_0225EAFC:
	ldrb r0, [r4, #0xd]
	add r0, r4, r0
	add r0, #0x30
	ldrb r1, [r0]
	ldr r0, [r4, #8]
	ldr r0, [r0, #4]
	str r1, [r0, #0x18]
	ldrb r1, [r4, #0xd]
	ldr r0, [r4]
	bl BattleSystem_GetBattlerIdPartner
_0225EB12:
	add r0, r4, r0
	add r0, #0x30
	ldrb r1, [r0]
	ldr r0, [r4, #8]
	ldr r0, [r0, #4]
	str r1, [r0, #0x1c]
	ldr r0, [r4, #8]
	ldr r0, [r0, #4]
	bl ov10_0221BE20
	ldrb r0, [r4, #0xe]
	add sp, #0x1fc
	add sp, #0x14
	add r0, r0, #1
	strb r0, [r4, #0xe]
	pop {r3, r4, r5, r6, r7, pc}
_0225EB32:
	ldr r0, [r4, #8]
	ldr r1, [r0, #4]
	add r0, r1, #0
	add r0, #0x36
	ldrb r0, [r0]
	cmp r0, #0
	beq _0225EBC6
	add r1, #0x32
	ldrb r0, [r1]
	mov r1, #0
	strb r0, [r4, #0x10]
	ldr r0, [r4, #8]
	ldr r0, [r0, #4]
	add r0, #0x36
	strb r1, [r0]
	ldr r0, [r4, #8]
	ldr r0, [r0, #4]
	ldrb r0, [r0, #0x11]
	cmp r0, #6
	bne _0225EB64
	add sp, #0x1fc
	mov r0, #2
	add sp, #0x14
	strb r0, [r4, #0xe]
	pop {r3, r4, r5, r6, r7, pc}
_0225EB64:
	add sp, #0x1fc
	mov r0, #6
	add sp, #0x14
	strb r0, [r4, #0xe]
	pop {r3, r4, r5, r6, r7, pc}
_0225EB6E:
	ldr r0, [r4]
	bl ov12_02237BB8
	ldr r0, [r4]
	bl BattleSystem_GetBattleInput
	ldrb r1, [r4, #0x10]
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
	ldr r2, _0225EC14 ; =0x0000FFFF
	add r0, r5, #0
	sub r3, #0x12
	bl PaletteData_BeginPaletteFade
	ldrb r0, [r4, #0xe]
	add sp, #0x1fc
	add sp, #0x14
	add r0, r0, #1
	strb r0, [r4, #0xe]
	pop {r3, r4, r5, r6, r7, pc}
_0225EBBE:
	bl PaletteData_GetSelectedBuffersBitmask
	cmp r0, #0
	beq _0225EBC8
_0225EBC6:
	b _0225F376
_0225EBC8:
	ldr r0, [r4]
	bl BattleSystem_GetMessageIcon
	mov r1, #0
	bl sub_0201649C
	ldr r0, [r4, #4]
	ldrh r0, [r0, #0x1c]
	cmp r0, #0
	bne _0225EBDE
	b _0225ED4A
_0225EBDE:
	mov r0, #9
	strb r0, [r4, #0xe]
	ldr r0, [r4, #4]
	ldrb r1, [r0, #0x1e]
	cmp r1, #3
	bls _0225EBEC
	b _0225ED4E
_0225EBEC:
	add r1, r1, r1
	add r1, pc
	ldrh r1, [r1, #6]
	lsl r1, r1, #0x10
	asr r1, r1, #0x10
	add pc, r1
_0225EBF8: ; jump table
	.short _0225EC90 - _0225EBF8 - 2 ; case 0
	.short _0225EC00 - _0225EBF8 - 2 ; case 1
	.short _0225ED44 - _0225EBF8 - 2 ; case 2
	.short _0225ECEE - _0225EBF8 - 2 ; case 3
_0225EC00:
	ldrh r1, [r0, #0x1c]
	ldr r0, _0225EC18 ; =0x0000FFE4
	add r0, r1, r0
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	cmp r0, #1
	bhi _0225EC1C
	mov r0, #8
	strb r0, [r4, #0xe]
	b _0225ED4E
	.balign 4, 0
_0225EC14: .word 0x0000FFFF
_0225EC18: .word 0x0000FFE4
_0225EC1C:
	ldr r0, [r4, #8]
	ldr r0, [r0, #4]
	ldrb r0, [r0, #0x11]
	cmp r0, #2
	bhs _0225EC3A
	ldr r0, [r4]
	bl BattleSystem_GetBattleType
	cmp r0, #3
	beq _0225EC44
	ldr r0, [r4]
	bl BattleSystem_GetBattleType
	cmp r0, #0x13
	beq _0225EC44
_0225EC3A:
	ldr r0, [r4, #8]
	ldr r0, [r0, #4]
	ldrb r0, [r0, #0x11]
	cmp r0, #1
	bhs _0225EC8A
_0225EC44:
	ldr r0, [r4, #4]
	ldrh r0, [r0, #0x1c]
	cmp r0, #0x17
	ldr r0, [r4]
	bne _0225EC6C
	bl BattleSystem_AreBattleAnimationsOn
	cmp r0, #1
	bne _0225EC5C
	mov r0, #0x11
	strh r0, [r4, #0x12]
	b _0225ED4E
_0225EC5C:
	mov r1, #0x74
	ldr r0, _0225EF7C ; =0x000005EC
	mvn r1, r1
	bl sub_0200602C
	mov r0, #0x15
	strh r0, [r4, #0x12]
	b _0225ED4E
_0225EC6C:
	bl BattleSystem_AreBattleAnimationsOn
	cmp r0, #1
	bne _0225EC7A
	mov r0, #0x19
	strh r0, [r4, #0x12]
	b _0225ED4E
_0225EC7A:
	mov r1, #0x74
	ldr r0, _0225EF7C ; =0x000005EC
	mvn r1, r1
	bl sub_0200602C
	mov r0, #0x1d
	strh r0, [r4, #0x12]
	b _0225ED4E
_0225EC8A:
	mov r0, #8
	strb r0, [r4, #0xe]
	b _0225ED4E
_0225EC90:
	ldr r0, [r4, #8]
	ldr r0, [r0, #4]
	ldrb r0, [r0, #0x11]
	cmp r0, #2
	bhs _0225ECAE
	ldr r0, [r4]
	bl BattleSystem_GetBattleType
	cmp r0, #3
	beq _0225ECB8
	ldr r0, [r4]
	bl BattleSystem_GetBattleType
	cmp r0, #0x13
	beq _0225ECB8
_0225ECAE:
	ldr r0, [r4, #8]
	ldr r0, [r0, #4]
	ldrb r0, [r0, #0x11]
	cmp r0, #1
	bhs _0225ECE8
_0225ECB8:
	ldr r0, [r4, #4]
	mov r1, #0x26
	ldrh r0, [r0, #0x1c]
	mov r2, #5
	bl GetItemAttr
	cmp r0, #0
	beq _0225ECE8
	ldr r0, [r4]
	bl BattleSystem_AreBattleAnimationsOn
	cmp r0, #1
	bne _0225ECD8
	mov r0, #0x11
	strh r0, [r4, #0x12]
	b _0225ED4E
_0225ECD8:
	mov r1, #0x74
	ldr r0, _0225EF7C ; =0x000005EC
	mvn r1, r1
	bl sub_0200602C
	mov r0, #0x15
	strh r0, [r4, #0x12]
	b _0225ED4E
_0225ECE8:
	mov r0, #8
	strb r0, [r4, #0xe]
	b _0225ED4E
_0225ECEE:
	ldrh r1, [r0, #0x1c]
	ldr r0, _0225EF80 ; =0x0000FFC1
	add r0, r1, r0
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	cmp r0, #1
	bhi _0225ED02
	mov r0, #8
	strb r0, [r4, #0xe]
	b _0225ED4E
_0225ED02:
	cmp r1, #0x37
	ldr r0, [r4]
	bne _0225ED26
	bl BattleSystem_AreBattleAnimationsOn
	cmp r0, #1
	bne _0225ED16
	mov r0, #0xd
	strh r0, [r4, #0x12]
	b _0225ED4E
_0225ED16:
	mov r1, #0x74
	ldr r0, _0225EF7C ; =0x000005EC
	mvn r1, r1
	bl sub_0200602C
	mov r0, #0xf
	strh r0, [r4, #0x12]
	b _0225ED4E
_0225ED26:
	bl BattleSystem_AreBattleAnimationsOn
	cmp r0, #1
	bne _0225ED34
	mov r0, #0xb
	strh r0, [r4, #0x12]
	b _0225ED4E
_0225ED34:
	mov r1, #0x74
	ldr r0, _0225EF7C ; =0x000005EC
	mvn r1, r1
	bl sub_0200602C
	mov r0, #0xf
	strh r0, [r4, #0x12]
	b _0225ED4E
_0225ED44:
	mov r0, #8
	strb r0, [r4, #0xe]
	b _0225ED4E
_0225ED4A:
	mov r0, #8
	strb r0, [r4, #0xe]
_0225ED4E:
	ldrb r0, [r4, #0xe]
	cmp r0, #8
	beq _0225EE32
	ldr r0, [r4]
	mov r1, #0
	bl ov12_02237ED0
	add sp, #0x1fc
	add sp, #0x14
	pop {r3, r4, r5, r6, r7, pc}
_0225ED62:
	ldr r2, [r4, #4]
	ldrh r0, [r2, #0x1c]
	cmp r0, #0
	bne _0225ED72
	mov r1, #0xff
	add r0, sp, #0x1c
	strh r1, [r0]
	b _0225ED8E
_0225ED72:
	add r1, sp, #0x1c
	strh r0, [r1]
	ldrb r0, [r2, #0x1e]
	strb r0, [r1, #2]
	cmp r0, #1
	bhi _0225ED8E
	ldr r0, [r4, #8]
	ldr r2, [r0, #4]
	ldrb r0, [r2, #0x11]
	add r0, r2, r0
	add r0, #0x2c
	ldrb r0, [r0]
	add r0, r0, #1
	strb r0, [r1, #3]
_0225ED8E:
	add r3, sp, #0x1c
	ldrb r1, [r4, #0xd]
	mov r2, sp
	ldrh r5, [r3]
	ldr r0, [r4]
	sub r2, r2, #4
	strh r5, [r2]
	ldrh r3, [r3, #2]
	strh r3, [r2, #2]
	ldr r2, [r2]
	bl ov12_022632C0
	ldrb r1, [r4, #0xd]
	ldrb r2, [r4, #0xc]
	ldr r0, [r4]
	bl ov12_0226430C
	ldr r0, [r4, #8]
	ldr r0, [r0, #4]
	ldr r0, [r0]
	bl Heap_Free
	ldr r0, [r4, #8]
	ldr r0, [r0, #4]
	bl Heap_Free
	ldr r0, [r4, #8]
	bl Heap_Free
	ldr r0, [r4, #4]
	bl Heap_Free
	add r0, r4, #0
	bl Heap_Free
	add r0, r7, #0
	bl SysTask_Destroy
	add sp, #0x1fc
	add sp, #0x14
	pop {r3, r4, r5, r6, r7, pc}
_0225EDE0:
	ldr r1, _0225EF84 ; =0x000004B6
	add r0, sp, #0x8c
	strh r1, [r0, #2]
	mov r1, #5
	strb r1, [r0, #1]
	ldr r0, [r4, #4]
	ldrh r0, [r0, #0x1c]
	str r0, [sp, #0x90]
	ldr r0, [r4]
	bl BattleSystem_GetMessageLoader
	add r5, r0, #0
	ldr r0, [r4]
	bl BattleSystem_GetTextFrameDelay
	add r3, r0, #0
	ldr r0, [r4]
	add r1, r5, #0
	add r2, sp, #0x8c
	bl BattleSystem_PrintBattleMessage
	strb r0, [r4, #0x11]
	mov r0, #0x1e
	strb r0, [r4, #0x17]
	ldrb r0, [r4, #0xe]
	add sp, #0x1fc
	add sp, #0x14
	add r0, r0, #1
	strb r0, [r4, #0xe]
	pop {r3, r4, r5, r6, r7, pc}
_0225EE1C:
	ldrb r0, [r4, #0x11]
	bl TextPrinterCheckActive
	cmp r0, #0
	bne _0225EE32
	ldrb r0, [r4, #0x17]
	sub r0, r0, #1
	strb r0, [r4, #0x17]
	ldrb r0, [r4, #0x17]
	cmp r0, #0
	beq _0225EE34
_0225EE32:
	b _0225F376
_0225EE34:
	ldrh r0, [r4, #0x12]
	add sp, #0x1fc
	add sp, #0x14
	strb r0, [r4, #0xe]
	pop {r3, r4, r5, r6, r7, pc}
_0225EE3E:
	ldrb r1, [r4, #0xd]
	mov r0, #9
	add r2, sp, #0x1b8
	str r0, [sp]
	str r1, [sp, #4]
	str r1, [sp, #8]
	mov r1, #0
	str r1, [sp, #0xc]
	ldr r0, [r4]
	mov r3, #1
	bl ov12_022643C8
	ldrb r1, [r4, #0xd]
	ldr r0, [r4]
	bl BattleSystem_GetOpponentData
	add r5, r0, #0
	ldr r0, [r4]
	bl ov12_0223A8DC
	add r2, r0, #0
	ldr r0, [r4]
	add r1, r5, #0
	add r3, sp, #0x1b8
	bl ov12_02261B80
	ldrb r0, [r4, #0xe]
	add sp, #0x1fc
	add sp, #0x14
	add r0, r0, #1
	strb r0, [r4, #0xe]
	pop {r3, r4, r5, r6, r7, pc}
_0225EE7E:
	ldr r0, [r4, #8]
	mov r1, #0
	ldr r0, [r0, #4]
	add r2, sp, #0x160
	ldrb r0, [r0, #0x11]
	mov r3, #1
	lsl r5, r0, #1
	mov r0, #9
	str r0, [sp]
	str r5, [sp, #4]
	str r5, [sp, #8]
	str r1, [sp, #0xc]
	ldr r0, [r4]
	bl ov12_022643C8
	ldr r0, [r4]
	add r1, r5, #0
	bl BattleSystem_GetOpponentData
	add r5, r0, #0
	ldr r0, [r4]
	bl ov12_0223A8DC
	add r2, r0, #0
	ldr r0, [r4]
	add r1, r5, #0
	add r3, sp, #0x160
	bl ov12_02261B80
	ldrb r0, [r4, #0xe]
	add sp, #0x1fc
	add sp, #0x14
	add r0, r0, #1
	strb r0, [r4, #0xe]
	pop {r3, r4, r5, r6, r7, pc}
_0225EEC4:
	ldr r0, [r4, #4]
	add r2, sp, #0x108
	ldrh r0, [r0, #0x1c]
	cmp r0, #0x37
	bne _0225EF02
	ldrb r0, [r4, #0xd]
	mov r1, #0
	add r3, r1, #0
	str r1, [sp]
	str r0, [sp, #4]
	str r0, [sp, #8]
	mov r0, #0x36
	str r0, [sp, #0xc]
	ldr r0, [r4]
	bl ov12_022643C8
	ldrb r1, [r4, #0xd]
	ldr r0, [r4]
	bl BattleSystem_GetOpponentData
	add r5, r0, #0
	ldr r0, [r4]
	bl ov12_0223A8DC
	add r2, r0, #0
	ldr r0, [r4]
	add r1, r5, #0
	add r3, sp, #0x108
	bl ov12_02261B80
	b _0225EF6C
_0225EF02:
	cmp r0, #0x38
	bne _0225EF3A
	ldrb r0, [r4, #0xd]
	mov r1, #0
	add r3, r1, #0
	str r1, [sp]
	str r0, [sp, #4]
	str r0, [sp, #8]
	mov r0, #0x74
	str r0, [sp, #0xc]
	ldr r0, [r4]
	bl ov12_022643C8
	ldrb r1, [r4, #0xd]
	ldr r0, [r4]
	bl BattleSystem_GetOpponentData
	add r5, r0, #0
	ldr r0, [r4]
	bl ov12_0223A8DC
	add r2, r0, #0
	ldr r0, [r4]
	add r1, r5, #0
	add r3, sp, #0x108
	bl ov12_02261B80
	b _0225EF6C
_0225EF3A:
	ldrb r1, [r4, #0xd]
	mov r0, #0xc
	mov r3, #1
	str r0, [sp]
	str r1, [sp, #4]
	str r1, [sp, #8]
	mov r1, #0
	str r1, [sp, #0xc]
	ldr r0, [r4]
	bl ov12_022643C8
	ldrb r1, [r4, #0xd]
	ldr r0, [r4]
	bl BattleSystem_GetOpponentData
	add r5, r0, #0
	ldr r0, [r4]
	bl ov12_0223A8DC
	add r2, r0, #0
	ldr r0, [r4]
	add r1, r5, #0
	add r3, sp, #0x108
	bl ov12_02261B80
_0225EF6C:
	ldrb r0, [r4, #0xe]
	add sp, #0x1fc
	add sp, #0x14
	add r0, r0, #1
	strb r0, [r4, #0xe]
	pop {r3, r4, r5, r6, r7, pc}
_0225EF78:
	ldr r2, _0225EF88 ; =0x000004B3
	b _0225EF8C
	.balign 4, 0
_0225EF7C: .word 0x000005EC
_0225EF80: .word 0x0000FFC1
_0225EF84: .word 0x000004B6
_0225EF88: .word 0x000004B3
_0225EF8C:
	add r0, sp, #0x68
	strh r2, [r0, #2]
	mov r1, #0xc
	strb r1, [r0, #1]
	ldrb r1, [r4, #0xd]
	ldr r3, [r4, #8]
	add r3, r3, r1
	ldrb r3, [r3, #0xc]
	lsl r3, r3, #8
	orr r1, r3
	str r1, [sp, #0x6c]
	ldr r1, [r4, #4]
	ldrh r1, [r1, #0x1c]
	sub r1, #0x37
	cmp r1, #7
	bhi _0225EFFE
	add r1, r1, r1
	add r1, pc
	ldrh r1, [r1, #6]
	lsl r1, r1, #0x10
	asr r1, r1, #0x10
	add pc, r1
_0225EFB8: ; jump table
	.short _0225EFEC - _0225EFB8 - 2 ; case 0
	.short _0225EFF6 - _0225EFB8 - 2 ; case 1
	.short _0225EFC8 - _0225EFB8 - 2 ; case 2
	.short _0225EFCE - _0225EFB8 - 2 ; case 3
	.short _0225EFD4 - _0225EFB8 - 2 ; case 4
	.short _0225EFDA - _0225EFB8 - 2 ; case 5
	.short _0225EFE0 - _0225EFB8 - 2 ; case 6
	.short _0225EFE6 - _0225EFB8 - 2 ; case 7
_0225EFC8:
	mov r0, #1
	str r0, [sp, #0x70]
	b _0225EFFE
_0225EFCE:
	mov r0, #2
	str r0, [sp, #0x70]
	b _0225EFFE
_0225EFD4:
	mov r0, #3
	str r0, [sp, #0x70]
	b _0225EFFE
_0225EFDA:
	mov r0, #6
	str r0, [sp, #0x70]
	b _0225EFFE
_0225EFE0:
	mov r0, #4
	str r0, [sp, #0x70]
	b _0225EFFE
_0225EFE6:
	mov r0, #5
	str r0, [sp, #0x70]
	b _0225EFFE
_0225EFEC:
	add r1, r2, #1
	strh r1, [r0, #2]
	mov r1, #0
	strb r1, [r0, #1]
	b _0225EFFE
_0225EFF6:
	add r1, r2, #2
	strh r1, [r0, #2]
	mov r1, #2
	strb r1, [r0, #1]
_0225EFFE:
	ldr r0, [r4]
	bl BattleSystem_GetMessageLoader
	add r5, r0, #0
	ldr r0, [r4]
	bl BattleSystem_GetTextFrameDelay
	add r3, r0, #0
	ldr r0, [r4]
	add r1, r5, #0
	add r2, sp, #0x68
	bl BattleSystem_PrintBattleMessage
	strb r0, [r4, #0x11]
	mov r0, #0x1e
	strb r0, [r4, #0x17]
	ldrb r0, [r4, #0xe]
	add sp, #0x1fc
	add sp, #0x14
	add r0, r0, #1
	strb r0, [r4, #0xe]
	pop {r3, r4, r5, r6, r7, pc}
_0225F02A:
	ldr r0, [r4, #8]
	mov r1, #0
	ldr r0, [r0, #4]
	add r2, sp, #0xb0
	ldrb r0, [r0, #0x11]
	mov r3, #1
	lsl r5, r0, #1
	mov r0, #0xe
	str r0, [sp]
	str r5, [sp, #4]
	str r5, [sp, #8]
	str r1, [sp, #0xc]
	ldr r0, [r4]
	bl ov12_022643C8
	ldr r0, [r4]
	add r1, r5, #0
	bl BattleSystem_GetOpponentData
	add r5, r0, #0
	ldr r0, [r4]
	bl ov12_0223A8DC
	add r2, r0, #0
	ldr r0, [r4]
	add r1, r5, #0
	add r3, sp, #0xb0
	bl ov12_02261B80
	ldrb r0, [r4, #0xe]
	add sp, #0x1fc
	add sp, #0x14
	add r0, r0, #1
	strb r0, [r4, #0xe]
	pop {r3, r4, r5, r6, r7, pc}
_0225F070:
	ldr r0, [r4, #8]
	ldr r1, [r0, #4]
	ldrb r0, [r1, #0x11]
	lsl r6, r0, #1
	add r0, r1, r0
	add r0, #0x2c
	ldrb r7, [r0]
	ldr r0, [r4]
	add r1, r6, #0
	bl BattleSystem_GetHpBar
	mov r1, #0
	mov r2, #1
	add r5, r0, #0
	bl MI_CpuFill8
	ldr r0, [r4]
	add r1, r6, #0
	bl ov12_0223AB0C
	str r0, [sp, #0x18]
	ldr r0, [r4]
	bl BattleSystem_GetBattleType
	add r1, r0, #0
	ldr r0, [sp, #0x18]
	bl BattleHpBar_Util_GetBarTypeFromBattlerSide
	add r1, r5, #0
	add r1, #0x25
	strb r0, [r1]
	ldr r0, [r4]
	add r1, r6, #0
	add r2, r7, #0
	bl BattleSystem_GetPartyMon
	mov r1, #0xa3
	mov r2, #0
	add r6, r0, #0
	bl GetMonData
	ldr r1, [r4, #8]
	mov r2, #0
	ldr r1, [r1, #4]
	ldrh r1, [r1, #0x20]
	sub r0, r0, r1
	str r0, [r5, #0x28]
	add r0, r6, #0
	mov r1, #0xa4
	bl GetMonData
	str r0, [r5, #0x2c]
	ldr r0, [r4, #8]
	mov r1, #0xa0
	ldr r0, [r0, #4]
	mov r2, #0
	ldrh r0, [r0, #0x20]
	str r0, [r5, #0x30]
	add r0, r6, #0
	bl GetMonData
	cmp r0, #0
	bne _0225F0F6
	add r0, r5, #0
	mov r1, #0
	add r0, #0x4a
	strb r1, [r0]
_0225F0F6:
	ldr r1, [r5, #0x30]
	add r0, r5, #0
	bl ov12_02264DCC
	ldrb r0, [r4, #0xe]
	add sp, #0x1fc
	add sp, #0x14
	add r0, r0, #1
	strb r0, [r4, #0xe]
	pop {r3, r4, r5, r6, r7, pc}
_0225F10A:
	ldr r1, [r4, #8]
	ldr r0, [r4]
	ldr r1, [r1, #4]
	ldrb r1, [r1, #0x11]
	lsl r1, r1, #1
	bl BattleSystem_GetHpBar
	add r5, r0, #0
	bl ov12_02264E00
	mov r1, #0
	mvn r1, r1
	cmp r0, r1
	beq _0225F128
	b _0225F376
_0225F128:
	mov r2, #1
	add r0, r5, #0
	mov r1, #0
	lsl r2, r2, #8
	bl ov12_0226498C
	ldrb r0, [r4, #0xe]
	add sp, #0x1fc
	add sp, #0x14
	add r0, r0, #1
	strb r0, [r4, #0xe]
	pop {r3, r4, r5, r6, r7, pc}
_0225F140:
	ldr r0, [r4]
	bl BattleSystem_GetMessageLoader
	add r5, r0, #0
	ldr r0, [r4, #8]
	ldr r1, [r0, #4]
	ldrb r0, [r1, #0x11]
	ldrh r1, [r1, #0x20]
	lsl r0, r0, #1
	cmp r1, #0
	add r1, sp, #0x1c
	beq _0225F178
	ldr r2, _0225F37C ; =0x000004BE
	strh r2, [r1, #0x2a]
	mov r2, #0x11
	add r1, sp, #0x44
	strb r2, [r1, #1]
	ldr r1, [r4, #8]
	add r1, r1, r0
	ldrb r1, [r1, #0xc]
	lsl r1, r1, #8
	orr r0, r1
	str r0, [sp, #0x48]
	ldr r0, [r4, #8]
	ldr r0, [r0, #4]
	ldrh r0, [r0, #0x20]
	str r0, [sp, #0x4c]
	b _0225F18E
_0225F178:
	ldr r2, _0225F380 ; =0x000004E2
	strh r2, [r1, #0x2a]
	mov r2, #2
	add r1, sp, #0x44
	strb r2, [r1, #1]
	ldr r1, [r4, #8]
	add r1, r1, r0
	ldrb r1, [r1, #0xc]
	lsl r1, r1, #8
	orr r0, r1
	str r0, [sp, #0x48]
_0225F18E:
	ldr r0, [r4]
	bl BattleSystem_GetTextFrameDelay
	add r3, r0, #0
	ldr r0, [r4]
	add r1, r5, #0
	add r2, sp, #0x44
	bl BattleSystem_PrintBattleMessage
	strb r0, [r4, #0x11]
	mov r0, #0x1e
	strb r0, [r4, #0x17]
	ldrb r0, [r4, #0xe]
	add sp, #0x1fc
	add sp, #0x14
	add r0, r0, #1
	strb r0, [r4, #0xe]
	pop {r3, r4, r5, r6, r7, pc}
_0225F1B2:
	ldr r0, [r4, #8]
	mov r5, #0
	ldr r0, [r0, #4]
	ldrb r0, [r0, #0x11]
	lsl r7, r0, #1
	ldr r0, [r4]
	add r1, r7, #0
	bl BattleSystem_GetHpBar
	str r0, [sp, #0x10]
	ldr r0, [r4, #8]
	add r1, r7, #0
	ldr r3, [r0, #4]
	ldr r0, [r4]
	ldrb r2, [r3, #0x11]
	add r2, r3, r2
	add r2, #0x2c
	ldrb r2, [r2]
	bl BattleSystem_GetPartyMon
	mov r1, #0xa0
	add r2, r5, #0
	bl GetMonData
	cmp r0, #0
	bne _0225F1EE
	ldr r0, [sp, #0x10]
	add r1, r5, #0
	add r0, #0x4a
	strb r1, [r0]
_0225F1EE:
	ldr r0, [sp, #0x10]
	mov r2, #1
	add r1, r0, #0
	ldr r1, [r1, #0x28]
	lsl r2, r2, #8
	bl ov12_0226498C
	mov r1, #2
	add r0, sp, #0x1c
	strb r1, [r0, #5]
	ldr r0, [r4, #8]
	mov r1, #0xf
	add r0, r0, r7
	ldrb r0, [r0, #0xc]
	mov r2, #5
	lsl r0, r0, #8
	orr r0, r7
	str r0, [sp, #0x24]
	ldr r0, [r4, #4]
	ldrh r0, [r0, #0x1c]
	bl GetItemAttr
	cmp r0, #0
	beq _0225F222
	mov r6, #0
	add r5, r5, #1
_0225F222:
	ldr r0, [r4, #4]
	mov r1, #0x10
	ldrh r0, [r0, #0x1c]
	mov r2, #5
	bl GetItemAttr
	cmp r0, #0
	beq _0225F236
	mov r6, #1
	add r5, r5, #1
_0225F236:
	ldr r0, [r4, #4]
	mov r1, #0x11
	ldrh r0, [r0, #0x1c]
	mov r2, #5
	bl GetItemAttr
	cmp r0, #0
	beq _0225F24A
	mov r6, #2
	add r5, r5, #1
_0225F24A:
	ldr r0, [r4, #4]
	mov r1, #0x12
	ldrh r0, [r0, #0x1c]
	mov r2, #5
	bl GetItemAttr
	cmp r0, #0
	beq _0225F25E
	mov r6, #3
	add r5, r5, #1
_0225F25E:
	ldr r0, [r4, #4]
	mov r1, #0x13
	ldrh r0, [r0, #0x1c]
	mov r2, #5
	bl GetItemAttr
	cmp r0, #0
	beq _0225F272
	mov r6, #4
	add r5, r5, #1
_0225F272:
	ldr r0, [r4, #4]
	mov r1, #0x14
	ldrh r0, [r0, #0x1c]
	mov r2, #5
	bl GetItemAttr
	cmp r0, #0
	beq _0225F286
	mov r6, #5
	add r5, r5, #1
_0225F286:
	ldr r0, [r4, #4]
	mov r1, #0x15
	ldrh r0, [r0, #0x1c]
	mov r2, #5
	bl GetItemAttr
	cmp r0, #0
	beq _0225F29A
	mov r6, #6
	add r5, r5, #1
_0225F29A:
	cmp r5, #1
	beq _0225F2A6
	ldr r1, _0225F384 ; =0x000004CD
	add r0, sp, #0x1c
	strh r1, [r0, #6]
	b _0225F2FA
_0225F2A6:
	cmp r6, #6
	bhi _0225F2FA
	add r0, r6, r6
	add r0, pc
	ldrh r0, [r0, #6]
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	add pc, r0
_0225F2B6: ; jump table
	.short _0225F2C4 - _0225F2B6 - 2 ; case 0
	.short _0225F2CC - _0225F2B6 - 2 ; case 1
	.short _0225F2D4 - _0225F2B6 - 2 ; case 2
	.short _0225F2DC - _0225F2B6 - 2 ; case 3
	.short _0225F2E4 - _0225F2B6 - 2 ; case 4
	.short _0225F2EC - _0225F2B6 - 2 ; case 5
	.short _0225F2F4 - _0225F2B6 - 2 ; case 6
_0225F2C4:
	ldr r1, _0225F388 ; =0x000004BA
	add r0, sp, #0x1c
	strh r1, [r0, #6]
	b _0225F2FA
_0225F2CC:
	ldr r1, _0225F38C ; =0x000004B7
	add r0, sp, #0x1c
	strh r1, [r0, #6]
	b _0225F2FA
_0225F2D4:
	ldr r1, _0225F390 ; =0x000004B9
	add r0, sp, #0x1c
	strh r1, [r0, #6]
	b _0225F2FA
_0225F2DC:
	ldr r1, _0225F394 ; =0x000004BB
	add r0, sp, #0x1c
	strh r1, [r0, #6]
	b _0225F2FA
_0225F2E4:
	ldr r1, _0225F398 ; =0x000004B8
	add r0, sp, #0x1c
	strh r1, [r0, #6]
	b _0225F2FA
_0225F2EC:
	ldr r1, _0225F39C ; =0x000004BC
	add r0, sp, #0x1c
	strh r1, [r0, #6]
	b _0225F2FA
_0225F2F4:
	ldr r1, _0225F3A0 ; =0x000004BD
	add r0, sp, #0x1c
	strh r1, [r0, #6]
_0225F2FA:
	ldr r0, [r4]
	bl BattleSystem_GetMessageLoader
	add r5, r0, #0
	ldr r0, [r4]
	bl BattleSystem_GetTextFrameDelay
	add r3, r0, #0
	ldr r0, [r4]
	add r1, r5, #0
	add r2, sp, #0x20
	bl BattleSystem_PrintBattleMessage
	strb r0, [r4, #0x11]
	mov r0, #0x1e
	strb r0, [r4, #0x17]
	ldrb r0, [r4, #0xe]
	add sp, #0x1fc
	add sp, #0x14
	add r0, r0, #1
	strb r0, [r4, #0xe]
	pop {r3, r4, r5, r6, r7, pc}
_0225F326:
	ldr r0, [r4]
	bl ov12_0223A8DC
	bl ov07_0221C394
	ldr r0, [r4]
	bl ov12_0223A8DC
	bl ov07_0221C3B0
	cmp r0, #0
	bne _0225F376
	ldr r0, [r4]
	bl ov12_0223A8DC
	bl ov07_0221C3C0
	ldrb r0, [r4, #0xe]
	add sp, #0x1fc
	add sp, #0x14
	add r0, r0, #1
	strb r0, [r4, #0xe]
	pop {r3, r4, r5, r6, r7, pc}
_0225F354:
	ldrb r0, [r4, #0x11]
	bl TextPrinterCheckActive
	cmp r0, #0
	bne _0225F376
	ldrb r0, [r4, #0x17]
	sub r0, r0, #1
	strb r0, [r4, #0x17]
	ldrb r0, [r4, #0x17]
	cmp r0, #0
	bne _0225F376
	ldr r0, [r4]
	mov r1, #1
	bl ov12_02237ED0
	mov r0, #8
	strb r0, [r4, #0xe]
_0225F376:
	add sp, #0x1fc
	add sp, #0x14
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
_0225F37C: .word 0x000004BE
_0225F380: .word 0x000004E2
_0225F384: .word 0x000004CD
_0225F388: .word 0x000004BA
_0225F38C: .word 0x000004B7
_0225F390: .word 0x000004B9
_0225F394: .word 0x000004BB
_0225F398: .word 0x000004B8
_0225F39C: .word 0x000004BC
_0225F3A0: .word 0x000004BD
	thumb_func_end ov12_0225E830
