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

	thumb_func_start ov12_02260668
ov12_02260668: ; 0x02260668
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x18
	add r4, r1, #0
	add r6, r0, #0
	ldr r0, [r4]
	bl BattleSystem_GetBgConfig
	ldr r0, [r4]
	bl BattleSystem_GetPaletteData
	add r7, r0, #0
	ldr r0, [r4]
	bl BattleSystem_GetTerrainId
	add r5, r0, #0
	ldr r0, [r4]
	bl BattleSystem_GetBackgroundId
	lsl r1, r0, #2
	ldr r0, _022609DC ; =ov12_0226D18C
	ldr r0, [r0, r1]
	str r0, [sp, #0xc]
	ldrb r0, [r4, #0x15]
	add r0, r0, #1
	strb r0, [r4, #0x15]
	ldrb r0, [r4, #0x14]
	cmp r0, #5
	bls _022606A2
	b _022609D8
_022606A2:
	add r0, r0, r0
	add r0, pc
	ldrh r0, [r0, #6]
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	add pc, r0
_022606AE: ; jump table
	.short _022606BA - _022606AE - 2 ; case 0
	.short _022606FA - _022606AE - 2 ; case 1
	.short _022608B0 - _022606AE - 2 ; case 2
	.short _022608CC - _022606AE - 2 ; case 3
	.short _022608E8 - _022606AE - 2 ; case 4
	.short _022608FA - _022606AE - 2 ; case 5
_022606BA:
	mov r0, #5
	mov r1, #0
	bl ov07_0223458C
	str r0, [r4, #4]
	ldr r0, _022609E0 ; =ov12_0226D350
	add r1, sp, #0x10
	ldrb r5, [r0, r5]
	mov r0, #1
	str r0, [sp, #0x14]
	str r5, [sp, #0x10]
	ldr r0, [r4, #4]
	bl ov07_022345C8
	str r0, [r4, #8]
	add r0, r5, #1
	str r0, [sp, #0x10]
	mov r0, #1
	str r0, [sp, #0x14]
	ldr r0, [r4, #4]
	add r1, sp, #0x10
	bl ov07_022345C8
	str r0, [r4, #0xc]
	ldr r0, [r4, #8]
	bl ov07_02234694
	ldr r0, _022609E4 ; =0x0000084F
	bl PlaySE
	mov r0, #1
	strb r0, [r4, #0x14]
_022606FA:
	ldrb r0, [r4, #0x15]
	cmp r0, #0xa
	bne _02260734
	mov r3, #0
	str r3, [sp]
	mov r0, #0x10
	str r0, [sp, #4]
	ldr r0, [sp, #0xc]
	ldr r2, _022609E8 ; =0x0000F3FF
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	str r0, [sp, #8]
	add r0, r7, #0
	mov r1, #1
	bl PaletteData_BeginPaletteFade
	mov r3, #0
	str r3, [sp]
	mov r0, #0x10
	str r0, [sp, #4]
	ldr r0, [sp, #0xc]
	ldr r2, _022609EC ; =0x00003FFF
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	str r0, [sp, #8]
	add r0, r7, #0
	mov r1, #4
	bl PaletteData_BeginPaletteFade
_02260734:
	ldrb r0, [r4, #0x15]
	cmp r0, #0xa
	blo _0226077E
	ldrb r0, [r4, #0x16]
	cmp r0, #0x10
	bhs _0226077E
	add r0, r0, #1
	strb r0, [r4, #0x16]
	ldrb r0, [r4, #0x16]
	cmp r0, #0x10
	bls _0226074E
	mov r0, #0x10
	strb r0, [r4, #0x16]
_0226074E:
	add r0, r7, #0
	mov r1, #1
	bl PaletteData_GetFadedBuf
	add r6, r0, #0
	mov r0, #1
	mov r5, #0
	lsl r0, r0, #8
_0226075E:
	ldrb r2, [r4, #0x16]
	mov r1, #0x1f
	mul r1, r2
	lsl r1, r1, #0xc
	lsr r3, r1, #0x10
	lsl r1, r3, #5
	lsl r2, r3, #0xa
	orr r1, r3
	orr r2, r1
	lsl r1, r5, #1
	strh r2, [r6, r1]
	add r1, r5, #1
	lsl r1, r1, #0x10
	lsr r5, r1, #0x10
	cmp r5, r0
	blo _0226075E
_0226077E:
	ldrb r0, [r4, #0x15]
	cmp r0, #0x14
	bne _0226078A
	ldr r0, [r4, #0xc]
	bl ov07_02234694
_0226078A:
	ldrb r0, [r4, #0x15]
	cmp r0, #0x17
	bne _02260798
	mov r0, #0x85
	lsl r0, r0, #4
	bl PlaySE
_02260798:
	ldrb r0, [r4, #0x15]
	cmp r0, #0x1c
	bne _0226088A
	add r0, r7, #0
	mov r1, #0
	bl PaletteData_GetUnfadedBuf
	add r5, r0, #0
	ldr r0, [r4]
	bl ov12_0223BAE0
	add r1, r5, #0
	mov r2, #0xe0
	bl MIi_CpuCopy16
	ldr r0, [r4]
	bl BattleSystem_GetBattleType
	cmp r0, #0x4a
	bne _022607DA
	add r0, r7, #0
	mov r1, #2
	bl PaletteData_GetUnfadedBuf
	add r5, r0, #0
	ldr r0, [r4]
	bl ov12_0223BAEC
	add r1, r5, #0
	mov r2, #0xa0
	bl MIi_CpuCopy16
	b _0226083C
_022607DA:
	ldr r0, [r4]
	bl BattleSystem_GetBattleType
	mov r1, #2
	tst r0, r1
	beq _022607FE
	add r0, r7, #0
	bl PaletteData_GetUnfadedBuf
	add r5, r0, #0
	ldr r0, [r4]
	bl ov12_0223BAEC
	add r1, r5, #0
	mov r2, #0xe0
	bl MIi_CpuCopy16
	b _0226083C
_022607FE:
	ldr r0, [r4]
	bl BattleSystem_GetBattleType
	mov r1, #1
	tst r0, r1
	beq _02260824
	add r0, r7, #0
	mov r1, #2
	bl PaletteData_GetUnfadedBuf
	add r5, r0, #0
	ldr r0, [r4]
	bl ov12_0223BAEC
	add r1, r5, #0
	mov r2, #0xa0
	bl MIi_CpuCopy16
	b _0226083C
_02260824:
	add r0, r7, #0
	mov r1, #2
	bl PaletteData_GetUnfadedBuf
	add r5, r0, #0
	ldr r0, [r4]
	bl ov12_0223BAEC
	add r1, r5, #0
	mov r2, #0x80
	bl MIi_CpuCopy16
_0226083C:
	mov r0, #0x10
	str r0, [sp]
	ldr r0, [sp, #0xc]
	mov r3, #0
	lsl r0, r0, #0x10
	str r3, [sp, #4]
	lsr r0, r0, #0x10
	str r0, [sp, #8]
	ldr r2, _022609E8 ; =0x0000F3FF
	add r0, r7, #0
	mov r1, #1
	bl PaletteData_BeginPaletteFade
	mov r0, #0x10
	str r0, [sp]
	ldr r0, [sp, #0xc]
	mov r3, #0
	lsl r0, r0, #0x10
	str r3, [sp, #4]
	lsr r0, r0, #0x10
	str r0, [sp, #8]
	ldr r2, _022609EC ; =0x00003FFF
	add r0, r7, #0
	mov r1, #4
	bl PaletteData_BeginPaletteFade
	mov r0, #0x10
	str r0, [sp]
	ldr r0, [sp, #0xc]
	mov r3, #0
	lsl r0, r0, #0x10
	str r3, [sp, #4]
	lsr r0, r0, #0x10
	str r0, [sp, #8]
	ldr r2, _022609F0 ; =0x0000FFFF
	add r0, r7, #0
	mov r1, #0xa
	bl PaletteData_BeginPaletteFade
_0226088A:
	ldrb r0, [r4, #0x15]
	cmp r0, #0x32
	blo _022608BA
	mov r0, #0x10
	str r0, [sp]
	mov r3, #0
	str r3, [sp, #4]
	mov r2, #3
	str r3, [sp, #8]
	add r0, r7, #0
	mov r1, #1
	lsl r2, r2, #0xa
	bl PaletteData_BeginPaletteFade
	ldrb r0, [r4, #0x14]
	add sp, #0x18
	add r0, r0, #1
	strb r0, [r4, #0x14]
	pop {r3, r4, r5, r6, r7, pc}
_022608B0:
	ldr r0, [r4, #0xc]
	bl ov07_022346BC
	cmp r0, #0
	beq _022608BC
_022608BA:
	b _022609D8
_022608BC:
	ldr r0, [r4, #4]
	bl ov07_02234604
	ldrb r0, [r4, #0x14]
	add sp, #0x18
	add r0, r0, #1
	strb r0, [r4, #0x14]
	pop {r3, r4, r5, r6, r7, pc}
_022608CC:
	ldr r0, [r4]
	bl BattleSystem_GetMessageIcon
	mov r1, #0
	bl sub_0201649C
	add r0, r4, #0
	bl Heap_Free
	add r0, r6, #0
	bl SysTask_Destroy
	add sp, #0x18
	pop {r3, r4, r5, r6, r7, pc}
_022608E8:
	ldr r0, [r4, #4]
	bl ov07_02234628
	ldr r0, [r4, #4]
	bl ov07_02234604
	ldrb r0, [r4, #0x14]
	add r0, r0, #1
	strb r0, [r4, #0x14]
_022608FA:
	add r0, r7, #0
	mov r1, #0
	bl PaletteData_GetUnfadedBuf
	add r5, r0, #0
	ldr r0, [r4]
	bl ov12_0223BAE0
	add r1, r5, #0
	mov r2, #0xe0
	bl MIi_CpuCopy16
	ldr r0, [r4]
	bl BattleSystem_GetBattleType
	cmp r0, #0x4a
	bne _02260936
	add r0, r7, #0
	mov r1, #2
	bl PaletteData_GetUnfadedBuf
	add r5, r0, #0
	ldr r0, [r4]
	bl ov12_0223BAEC
	add r1, r5, #0
	mov r2, #0xa0
	bl MIi_CpuCopy16
	b _02260998
_02260936:
	ldr r0, [r4]
	bl BattleSystem_GetBattleType
	mov r1, #2
	tst r0, r1
	beq _0226095A
	add r0, r7, #0
	bl PaletteData_GetUnfadedBuf
	add r5, r0, #0
	ldr r0, [r4]
	bl ov12_0223BAEC
	add r1, r5, #0
	mov r2, #0xe0
	bl MIi_CpuCopy16
	b _02260998
_0226095A:
	ldr r0, [r4]
	bl BattleSystem_GetBattleType
	mov r1, #1
	tst r0, r1
	beq _02260980
	add r0, r7, #0
	mov r1, #2
	bl PaletteData_GetUnfadedBuf
	add r5, r0, #0
	ldr r0, [r4]
	bl ov12_0223BAEC
	add r1, r5, #0
	mov r2, #0xa0
	bl MIi_CpuCopy16
	b _02260998
_02260980:
	add r0, r7, #0
	mov r1, #2
	bl PaletteData_GetUnfadedBuf
	add r5, r0, #0
	ldr r0, [r4]
	bl ov12_0223BAEC
	add r1, r5, #0
	mov r2, #0x80
	bl MIi_CpuCopy16
_02260998:
	mov r3, #0
	str r3, [sp]
	ldr r0, _022609F4 ; =0x00007FFF
	str r3, [sp, #4]
	str r0, [sp, #8]
	ldr r2, _022609F0 ; =0x0000FFFF
	add r0, r7, #0
	mov r1, #1
	bl PaletteData_BeginPaletteFade
	mov r3, #0
	str r3, [sp]
	ldr r2, _022609F0 ; =0x0000FFFF
	str r3, [sp, #4]
	str r2, [sp, #8]
	add r0, r7, #0
	mov r1, #4
	lsr r2, r2, #2
	bl PaletteData_BeginPaletteFade
	mov r3, #0
	str r3, [sp]
	ldr r0, _022609F4 ; =0x00007FFF
	str r3, [sp, #4]
	str r0, [sp, #8]
	ldr r2, _022609F0 ; =0x0000FFFF
	add r0, r7, #0
	mov r1, #0xa
	bl PaletteData_BeginPaletteFade
	mov r0, #3
	strb r0, [r4, #0x14]
_022609D8:
	add sp, #0x18
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
_022609DC: .word ov12_0226D18C
_022609E0: .word ov12_0226D350
_022609E4: .word 0x0000084F
_022609E8: .word 0x0000F3FF
_022609EC: .word 0x00003FFF
_022609F0: .word 0x0000FFFF
_022609F4: .word 0x00007FFF
	thumb_func_end ov12_02260668

	thumb_func_start ov12_022609F8
ov12_022609F8: ; 0x022609F8
	push {r3, r4, r5, r6, lr}
	sub sp, #0xc
	add r4, r1, #0
	add r5, r0, #0
	ldr r0, [r4]
	bl BattleSystem_GetPaletteData
	ldrb r1, [r4, #0xa]
	add r6, r0, #0
	cmp r1, #3
	bls _02260A10
	b _02260B28
_02260A10:
	add r1, r1, r1
	add r1, pc
	ldrh r1, [r1, #6]
	lsl r1, r1, #0x10
	asr r1, r1, #0x10
	add pc, r1
_02260A1C: ; jump table
	.short _02260A24 - _02260A1C - 2 ; case 0
	.short _02260A44 - _02260A1C - 2 ; case 1
	.short _02260AB4 - _02260A1C - 2 ; case 2
	.short _02260AE6 - _02260A1C - 2 ; case 3
_02260A24:
	mov r2, #0
	str r2, [sp]
	mov r1, #0x10
	str r1, [sp, #4]
	mov r1, #0xa
	str r2, [sp, #8]
	add r3, r1, #0
	ldr r2, _02260B2C ; =0x0000FFFF
	sub r3, #0x12
	bl PaletteData_BeginPaletteFade
	ldrb r0, [r4, #0xa]
	add sp, #0xc
	add r0, r0, #1
	strb r0, [r4, #0xa]
	pop {r3, r4, r5, r6, pc}
_02260A44:
	bl PaletteData_GetSelectedBuffersBitmask
	cmp r0, #0
	bne _02260B28
	ldr r0, [r4]
	bl ov12_02237B0C
	mov r0, #5
	mov r1, #0x38
	bl Heap_Alloc
	str r0, [r4, #4]
	ldrb r1, [r4, #9]
	ldr r0, [r4]
	bl BattleSystem_GetParty
	ldr r1, [r4, #4]
	mov r2, #3
	str r0, [r1]
	ldr r1, [r4]
	ldr r0, [r4, #4]
	str r1, [r0, #8]
	ldr r0, [r4, #4]
	mov r1, #5
	str r1, [r0, #0xc]
	ldrb r1, [r4, #0xe]
	ldr r0, [r4, #4]
	strb r1, [r0, #0x11]
	ldrh r1, [r4, #0xc]
	ldr r0, [r4, #4]
	strh r1, [r0, #0x24]
	ldr r1, [r4, #4]
	mov r0, #0
	add r1, #0x36
	strb r0, [r1]
	ldr r1, [r4, #4]
	strb r0, [r1, #0x12]
	ldr r1, [r4, #4]
	add r1, #0x35
	strb r2, [r1]
	ldr r1, [r4, #4]
	strh r0, [r1, #0x22]
	ldrb r2, [r4, #9]
	ldr r1, [r4, #4]
	str r2, [r1, #0x28]
	ldr r1, [r4, #4]
	add r1, #0x32
	strb r0, [r1]
	ldr r0, [r4, #4]
	bl ov10_0221BE20
	ldrb r0, [r4, #0xa]
	add sp, #0xc
	add r0, r0, #1
	strb r0, [r4, #0xa]
	pop {r3, r4, r5, r6, pc}
_02260AB4:
	ldr r0, [r4, #4]
	add r0, #0x36
	ldrb r0, [r0]
	cmp r0, #0
	beq _02260B28
	ldr r0, [r4]
	bl ov12_02237BB8
	mov r0, #0x10
	str r0, [sp]
	mov r0, #0
	mov r1, #0xa
	str r0, [sp, #4]
	add r3, r1, #0
	str r0, [sp, #8]
	ldr r2, _02260B2C ; =0x0000FFFF
	add r0, r6, #0
	sub r3, #0x12
	bl PaletteData_BeginPaletteFade
	ldrb r0, [r4, #0xa]
	add sp, #0xc
	add r0, r0, #1
	strb r0, [r4, #0xa]
	pop {r3, r4, r5, r6, pc}
_02260AE6:
	bl PaletteData_GetSelectedBuffersBitmask
	cmp r0, #0
	bne _02260B28
	ldr r0, [r4, #4]
	add r0, #0x34
	ldrb r2, [r0]
	ldr r0, [r4]
	cmp r2, #4
	bne _02260B04
	ldrb r1, [r4, #9]
	mov r2, #0xff
	bl ov12_02263360
	b _02260B0C
_02260B04:
	ldrb r1, [r4, #9]
	add r2, r2, #1
	bl ov12_02263360
_02260B0C:
	ldrb r1, [r4, #9]
	ldrb r2, [r4, #8]
	ldr r0, [r4]
	bl ov12_0226430C
	ldr r0, [r4, #4]
	bl Heap_Free
	add r0, r4, #0
	bl Heap_Free
	add r0, r5, #0
	bl SysTask_Destroy
_02260B28:
	add sp, #0xc
	pop {r3, r4, r5, r6, pc}
	.balign 4, 0
_02260B2C: .word 0x0000FFFF
	thumb_func_end ov12_022609F8
