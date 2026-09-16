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

	thumb_func_start ov12_02261390
ov12_02261390: ; 0x02261390
	push {r3, r4, r5, r6, r7, lr}
	add r5, r1, #0
	ldr r1, _0226144C ; =0x00000195
	add r6, r2, #0
	ldrb r2, [r5, r1]
	mov r1, #1
	add r7, r0, #0
	add r4, r3, #0
	tst r1, r2
	beq _022613EA
	bl BattleSystem_GetBattleType
	mov r1, #4
	tst r0, r1
	beq _022613CA
	ldr r0, _02261450 ; =0x000003DE
	strh r0, [r4, #2]
	mov r0, #0x1b
	strb r0, [r4, #1]
	mov r0, #0x65
	lsl r0, r0, #2
	ldrb r1, [r5, r0]
	str r1, [r4, #4]
	ldrb r1, [r5, r0]
	ldrb r0, [r6, #1]
	lsl r0, r0, #8
	orr r0, r1
	str r0, [r4, #8]
	pop {r3, r4, r5, r6, r7, pc}
_022613CA:
	ldr r0, _02261454 ; =0x000003DD
	strh r0, [r4, #2]
	mov r0, #0x32
	strb r0, [r4, #1]
	mov r0, #0x65
	lsl r0, r0, #2
	ldrb r1, [r5, r0]
	str r1, [r4, #4]
	ldrb r1, [r5, r0]
	str r1, [r4, #8]
	ldrb r1, [r5, r0]
	ldrb r0, [r6, #1]
	lsl r0, r0, #8
	orr r0, r1
	str r0, [r4, #0xc]
	pop {r3, r4, r5, r6, r7, pc}
_022613EA:
	bl BattleSystem_GetBattleType
	mov r1, #2
	tst r0, r1
	bne _02261434
	add r0, r7, #0
	bl BattleSystem_GetBattleType
	mov r1, #4
	tst r0, r1
	bne _02261434
	ldrh r0, [r6, #2]
	cmp r0, #0
	bne _0226140E
	mov r0, #0xf6
	lsl r0, r0, #2
	strh r0, [r4, #2]
	b _02261438
_0226140E:
	cmp r0, #0x19
	bhs _02261418
	ldr r0, _02261458 ; =0x000003D9
	strh r0, [r4, #2]
	b _02261438
_02261418:
	cmp r0, #0x32
	bhs _02261424
	mov r0, #0xf7
	lsl r0, r0, #2
	strh r0, [r4, #2]
	b _02261438
_02261424:
	cmp r0, #0x4b
	bhs _0226142E
	ldr r0, _0226145C ; =0x000003DA
	strh r0, [r4, #2]
	b _02261438
_0226142E:
	ldr r0, _02261460 ; =0x000003DB
	strh r0, [r4, #2]
	b _02261438
_02261434:
	ldr r0, _02261458 ; =0x000003D9
	strh r0, [r4, #2]
_02261438:
	mov r0, #2
	strb r0, [r4, #1]
	mov r0, #0x65
	lsl r0, r0, #2
	ldrb r1, [r5, r0]
	ldrb r0, [r6, #1]
	lsl r0, r0, #8
	orr r0, r1
	str r0, [r4, #4]
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
_0226144C: .word 0x00000195
_02261450: .word 0x000003DE
_02261454: .word 0x000003DD
_02261458: .word 0x000003D9
_0226145C: .word 0x000003DA
_02261460: .word 0x000003DB
	thumb_func_end ov12_02261390

	thumb_func_start ov12_02261464
ov12_02261464: ; 0x02261464
	push {r3, r4, r5, r6, r7, lr}
	add r5, r1, #0
	ldr r1, _02261524 ; =0x00000195
	add r6, r2, #0
	ldrb r2, [r5, r1]
	mov r1, #1
	add r7, r0, #0
	add r4, r3, #0
	tst r1, r2
	beq _022614C0
	bl BattleSystem_GetBattleType
	mov r1, #4
	tst r0, r1
	bne _022614A4
	mov r0, #0xf3
	lsl r0, r0, #2
	strh r0, [r4, #2]
	mov r0, #0x32
	strb r0, [r4, #1]
	mov r0, #0x65
	lsl r0, r0, #2
	ldrb r1, [r5, r0]
	str r1, [r4, #4]
	ldrb r1, [r5, r0]
	str r1, [r4, #8]
	ldrb r1, [r5, r0]
	ldrb r0, [r6, #1]
	lsl r0, r0, #8
	orr r0, r1
	str r0, [r4, #0xc]
	pop {r3, r4, r5, r6, r7, pc}
_022614A4:
	ldr r0, _02261528 ; =0x000003CE
	strh r0, [r4, #2]
	mov r0, #0x1b
	strb r0, [r4, #1]
	mov r0, #0x65
	lsl r0, r0, #2
	ldrb r1, [r5, r0]
	str r1, [r4, #4]
	ldrb r1, [r5, r0]
	ldrb r0, [r6, #1]
	lsl r0, r0, #8
	orr r0, r1
	str r0, [r4, #8]
	pop {r3, r4, r5, r6, r7, pc}
_022614C0:
	bl BattleSystem_GetBattleType
	mov r1, #2
	tst r0, r1
	bne _0226150C
	add r0, r7, #0
	bl BattleSystem_GetBattleType
	mov r1, #4
	tst r0, r1
	bne _0226150C
	ldrh r1, [r6, #2]
	cmp r1, #0x64
	bhs _022614E2
	ldr r0, _0226152C ; =0x000003D6
	strh r0, [r4, #2]
	b _02261510
_022614E2:
	ldr r0, _02261530 ; =0x00000145
	cmp r1, r0
	bhs _022614EE
	ldr r0, _02261534 ; =0x000003D7
	strh r0, [r4, #2]
	b _02261510
_022614EE:
	add r0, #0xe1
	cmp r1, r0
	bhs _022614FA
	ldr r0, _02261538 ; =0x000003D5
	strh r0, [r4, #2]
	b _02261510
_022614FA:
	ldr r0, _0226153C ; =0x00000307
	cmp r1, r0
	bhs _02261506
	add r0, #0xcd
	strh r0, [r4, #2]
	b _02261510
_02261506:
	add r0, #0xcc
	strh r0, [r4, #2]
	b _02261510
_0226150C:
	ldr r0, _02261540 ; =0x000003D3
	strh r0, [r4, #2]
_02261510:
	mov r0, #2
	strb r0, [r4, #1]
	mov r0, #0x65
	lsl r0, r0, #2
	ldrb r1, [r5, r0]
	ldrb r0, [r6, #1]
	lsl r0, r0, #8
	orr r0, r1
	str r0, [r4, #4]
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
_02261524: .word 0x00000195
_02261528: .word 0x000003CE
_0226152C: .word 0x000003D6
_02261530: .word 0x00000145
_02261534: .word 0x000003D7
_02261538: .word 0x000003D5
_0226153C: .word 0x00000307
_02261540: .word 0x000003D3
	thumb_func_end ov12_02261464

	thumb_func_start ov12_02261544
ov12_02261544: ; 0x02261544
	push {r3, r4, r5, r6, r7, lr}
	add r7, r0, #0
	add r5, r2, #0
	bl BattleSystem_GetBattleType
	add r4, r0, #0
	mov r0, #2
	tst r0, r4
	beq _0226156A
	add r0, r7, #0
	mov r1, #3
	bl BattleSystem_GetBattlerFromBattlerType
	add r6, r0, #0
	add r0, r7, #0
	mov r1, #5
	bl BattleSystem_GetBattlerFromBattlerType
	b _02261574
_0226156A:
	add r0, r7, #0
	mov r1, #1
	bl BattleSystem_GetBattlerFromBattlerType
	add r6, r0, #0
_02261574:
	mov r1, #4
	tst r1, r4
	beq _022615B4
	mov r1, #0x80
	tst r1, r4
	beq _02261594
	mov r1, #0x3e
	lsl r1, r1, #4
	strh r1, [r5, #2]
	mov r1, #0x3b
	strb r1, [r5, #1]
	str r6, [r5, #4]
	str r6, [r5, #8]
	str r0, [r5, #0xc]
	str r0, [r5, #0x10]
	pop {r3, r4, r5, r6, r7, pc}
_02261594:
	mov r1, #8
	add r2, r4, #0
	tst r2, r1
	beq _022615AA
	ldr r1, _022615E4 ; =0x000003CB
	strh r1, [r5, #2]
	mov r1, #0x1a
	strb r1, [r5, #1]
	str r6, [r5, #4]
	str r0, [r5, #8]
	pop {r3, r4, r5, r6, r7, pc}
_022615AA:
	ldr r0, _022615E8 ; =0x000003CA
	strh r0, [r5, #2]
	strb r1, [r5, #1]
	str r6, [r5, #4]
	pop {r3, r4, r5, r6, r7, pc}
_022615B4:
	mov r1, #0x10
	tst r1, r4
	bne _022615C0
	mov r1, #8
	tst r1, r4
	beq _022615D4
_022615C0:
	mov r1, #0x3e
	lsl r1, r1, #4
	strh r1, [r5, #2]
	mov r1, #0x3b
	strb r1, [r5, #1]
	str r6, [r5, #4]
	str r6, [r5, #8]
	str r0, [r5, #0xc]
	str r0, [r5, #0x10]
	pop {r3, r4, r5, r6, r7, pc}
_022615D4:
	ldr r0, _022615EC ; =0x000003C9
	strh r0, [r5, #2]
	mov r0, #0x1e
	strb r0, [r5, #1]
	str r6, [r5, #4]
	str r6, [r5, #8]
	pop {r3, r4, r5, r6, r7, pc}
	nop
_022615E4: .word 0x000003CB
_022615E8: .word 0x000003CA
_022615EC: .word 0x000003C9
	thumb_func_end ov12_02261544

	thumb_func_start ov12_022615F0
ov12_022615F0: ; 0x022615F0
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x18
	str r1, [sp, #4]
	str r0, [sp]
	add r6, r2, #0
	add r4, r3, #0
	bl BattleSystem_GetBattleType
	str r0, [sp, #0x14]
	ldr r2, _02261904 ; =0x00000195
	ldr r0, [sp, #4]
	ldrb r1, [r0, r2]
	mov r0, #1
	tst r0, r1
	bne _02261610
	b _02261758
_02261610:
	ldr r0, [sp, #0x14]
	mov r1, #2
	and r0, r1
	str r0, [sp, #0x10]
	beq _0226162C
	ldr r0, [sp, #4]
	sub r1, r2, #1
	ldrb r5, [r0, r1]
	ldr r0, [sp]
	add r1, r5, #0
	bl BattleSystem_GetBattlerIdPartner
	add r7, r0, #0
	b _02261634
_0226162C:
	ldr r0, [sp, #4]
	sub r1, r2, #1
	ldrb r5, [r0, r1]
	add r7, r5, #0
_02261634:
	ldr r0, [sp, #0x14]
	mov r1, #4
	tst r0, r1
	beq _022616DA
	ldr r0, [sp, #0x14]
	mov r1, #0x80
	tst r0, r1
	beq _0226166C
	ldr r0, _02261908 ; =0x000003DF
	add sp, #0x18
	strh r0, [r4, #2]
	mov r0, #0x3c
	strb r0, [r4, #1]
	str r5, [r4, #4]
	str r5, [r4, #8]
	add r0, r6, r5
	ldrb r0, [r0, #4]
	lsl r0, r0, #8
	orr r0, r5
	str r0, [r4, #0xc]
	str r7, [r4, #0x10]
	str r7, [r4, #0x14]
	add r0, r6, r7
	ldrb r0, [r0, #4]
	lsl r0, r0, #8
	orr r0, r7
	str r0, [r4, #0x18]
	pop {r3, r4, r5, r6, r7, pc}
_0226166C:
	ldr r0, [sp, #0x14]
	mov r1, #8
	tst r0, r1
	beq _0226169A
	mov r0, #0x3d
	lsl r0, r0, #4
	strh r0, [r4, #2]
	mov r0, #0x38
	strb r0, [r4, #1]
	str r5, [r4, #4]
	add r0, r6, r5
	ldrb r0, [r0, #4]
	add sp, #0x18
	lsl r0, r0, #8
	orr r0, r5
	str r0, [r4, #8]
	str r7, [r4, #0xc]
	add r0, r6, r7
	ldrb r0, [r0, #4]
	lsl r0, r0, #8
	orr r0, r7
	str r0, [r4, #0x10]
	pop {r3, r4, r5, r6, r7, pc}
_0226169A:
	ldr r0, [sp, #0x10]
	cmp r0, #0
	beq _022616C2
	ldr r0, _0226190C ; =0x000003CF
	add sp, #0x18
	strh r0, [r4, #2]
	mov r0, #0x31
	strb r0, [r4, #1]
	str r5, [r4, #4]
	add r0, r6, r5
	ldrb r0, [r0, #4]
	lsl r0, r0, #8
	orr r0, r5
	str r0, [r4, #8]
	add r0, r6, r7
	ldrb r0, [r0, #4]
	lsl r0, r0, #8
	orr r0, r7
	str r0, [r4, #0xc]
	pop {r3, r4, r5, r6, r7, pc}
_022616C2:
	ldr r0, _02261910 ; =0x000003CE
	add sp, #0x18
	strh r0, [r4, #2]
	mov r0, #0x1b
	strb r0, [r4, #1]
	str r5, [r4, #4]
	add r0, r6, r5
	ldrb r0, [r0, #4]
	lsl r0, r0, #8
	orr r0, r5
	str r0, [r4, #8]
	pop {r3, r4, r5, r6, r7, pc}
_022616DA:
	ldr r0, [sp, #0x14]
	mov r1, #0x10
	tst r0, r1
	bne _022616EA
	ldr r0, [sp, #0x14]
	mov r1, #8
	tst r0, r1
	beq _02261712
_022616EA:
	ldr r0, _02261908 ; =0x000003DF
	add sp, #0x18
	strh r0, [r4, #2]
	mov r0, #0x3c
	strb r0, [r4, #1]
	str r5, [r4, #4]
	str r5, [r4, #8]
	add r0, r6, r5
	ldrb r0, [r0, #4]
	lsl r0, r0, #8
	orr r0, r5
	str r0, [r4, #0xc]
	str r7, [r4, #0x10]
	str r7, [r4, #0x14]
	add r0, r6, r7
	ldrb r0, [r0, #4]
	lsl r0, r0, #8
	orr r0, r7
	str r0, [r4, #0x18]
	pop {r3, r4, r5, r6, r7, pc}
_02261712:
	ldr r0, [sp, #0x10]
	cmp r0, #0
	beq _0226173C
	ldr r0, _02261914 ; =0x000003CD
	add sp, #0x18
	strh r0, [r4, #2]
	mov r0, #0x39
	strb r0, [r4, #1]
	str r5, [r4, #4]
	str r5, [r4, #8]
	add r0, r6, r5
	ldrb r0, [r0, #4]
	lsl r0, r0, #8
	orr r0, r5
	str r0, [r4, #0xc]
	add r0, r6, r7
	ldrb r0, [r0, #4]
	lsl r0, r0, #8
	orr r0, r7
	str r0, [r4, #0x10]
	pop {r3, r4, r5, r6, r7, pc}
_0226173C:
	mov r0, #0xf3
	lsl r0, r0, #2
	strh r0, [r4, #2]
	mov r0, #0x32
	strb r0, [r4, #1]
	str r5, [r4, #4]
	str r5, [r4, #8]
	add r0, r6, r5
	ldrb r0, [r0, #4]
	add sp, #0x18
	lsl r0, r0, #8
	orr r0, r5
	str r0, [r4, #0xc]
	pop {r3, r4, r5, r6, r7, pc}
_02261758:
	ldr r0, [sp, #0x14]
	mov r1, #4
	and r0, r1
	str r0, [sp, #0xc]
	beq _022617EC
	ldr r0, [sp]
	bl ov12_0223BFC0
	lsl r0, r0, #0x18
	lsr r2, r0, #0x18
	ldr r0, [sp, #0x14]
	mov r1, #8
	and r0, r1
	str r0, [sp, #8]
	beq _022617C2
	ldr r0, [sp]
	add r1, r2, #0
	bl ov12_0223BFCC
	cmp r0, #3
	bhi _02261832
	add r0, r0, r0
	add r0, pc
	ldrh r0, [r0, #6]
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	add pc, r0
_0226178E: ; jump table
	.short _02261796 - _0226178E - 2 ; case 0
	.short _022617AC - _0226178E - 2 ; case 1
	.short _022617AC - _0226178E - 2 ; case 2
	.short _02261796 - _0226178E - 2 ; case 3
_02261796:
	ldr r0, [sp]
	mov r1, #4
	bl BattleSystem_GetBattlerFromBattlerType
	add r5, r0, #0
	ldr r0, [sp]
	mov r1, #2
	bl BattleSystem_GetBattlerFromBattlerType
	add r7, r0, #0
	b _02261832
_022617AC:
	ldr r0, [sp]
	mov r1, #2
	bl BattleSystem_GetBattlerFromBattlerType
	add r5, r0, #0
	ldr r0, [sp]
	mov r1, #4
	bl BattleSystem_GetBattlerFromBattlerType
	add r7, r0, #0
	b _02261832
_022617C2:
	ldr r0, [sp, #0x14]
	mov r1, #2
	tst r0, r1
	beq _022617DE
	ldr r0, [sp]
	bl BattleSystem_GetBattlerFromBattlerType
	add r5, r0, #0
	ldr r0, [sp]
	mov r1, #4
	bl BattleSystem_GetBattlerFromBattlerType
	add r7, r0, #0
	b _02261832
_022617DE:
	ldr r0, [sp]
	mov r1, #0
	bl BattleSystem_GetBattlerFromBattlerType
	add r5, r0, #0
	add r7, r5, #0
	b _02261832
_022617EC:
	ldr r0, [sp, #0x14]
	mov r1, #8
	and r0, r1
	str r0, [sp, #8]
	beq _0226180E
	ldr r1, [sp, #4]
	sub r2, r2, #1
	ldrb r1, [r1, r2]
	ldr r0, [sp]
	bl BattleSystem_GetBattlerIdPartner
	add r5, r0, #0
	mov r1, #0x65
	ldr r0, [sp, #4]
	lsl r1, r1, #2
	ldrb r7, [r0, r1]
	b _02261832
_0226180E:
	ldr r0, [sp, #0x14]
	mov r1, #2
	tst r0, r1
	beq _0226182A
	ldr r0, [sp]
	bl BattleSystem_GetBattlerFromBattlerType
	add r5, r0, #0
	ldr r0, [sp]
	mov r1, #4
	bl BattleSystem_GetBattlerFromBattlerType
	add r7, r0, #0
	b _02261832
_0226182A:
	ldr r0, [sp, #4]
	sub r1, r2, #1
	ldrb r5, [r0, r1]
	add r7, r5, #0
_02261832:
	ldr r0, [sp, #0xc]
	cmp r0, #0
	beq _0226189C
	ldr r0, [sp, #8]
	cmp r0, #0
	beq _02261860
	ldr r0, _02261918 ; =0x000003D1
	add sp, #0x18
	strh r0, [r4, #2]
	mov r0, #0x31
	strb r0, [r4, #1]
	str r5, [r4, #4]
	add r0, r6, r5
	ldrb r0, [r0, #4]
	lsl r0, r0, #8
	orr r0, r5
	str r0, [r4, #8]
	add r0, r6, r7
	ldrb r0, [r0, #4]
	lsl r0, r0, #8
	orr r0, r7
	str r0, [r4, #0xc]
	pop {r3, r4, r5, r6, r7, pc}
_02261860:
	ldr r0, [sp, #0x14]
	mov r1, #2
	tst r0, r1
	beq _02261888
	ldr r0, _0226191C ; =0x000003D2
	add sp, #0x18
	strh r0, [r4, #2]
	mov r0, #9
	strb r0, [r4, #1]
	add r0, r6, r5
	ldrb r0, [r0, #4]
	lsl r0, r0, #8
	orr r0, r5
	str r0, [r4, #4]
	add r0, r6, r7
	ldrb r0, [r0, #4]
	lsl r0, r0, #8
	orr r0, r7
	str r0, [r4, #8]
	pop {r3, r4, r5, r6, r7, pc}
_02261888:
	ldr r0, _02261920 ; =0x000003D3
	add sp, #0x18
	strh r0, [r4, #2]
	strb r1, [r4, #1]
	add r0, r6, r5
	ldrb r0, [r0, #4]
	lsl r0, r0, #8
	orr r0, r5
	str r0, [r4, #4]
	pop {r3, r4, r5, r6, r7, pc}
_0226189C:
	ldr r0, [sp, #8]
	cmp r0, #0
	beq _022618C6
	ldr r0, _02261924 ; =0x000003E1
	add sp, #0x18
	strh r0, [r4, #2]
	mov r0, #0x39
	strb r0, [r4, #1]
	str r5, [r4, #4]
	str r5, [r4, #8]
	add r0, r6, r5
	ldrb r0, [r0, #4]
	lsl r0, r0, #8
	orr r0, r5
	str r0, [r4, #0xc]
	add r0, r6, r7
	ldrb r0, [r0, #4]
	lsl r0, r0, #8
	orr r0, r7
	str r0, [r4, #0x10]
	pop {r3, r4, r5, r6, r7, pc}
_022618C6:
	ldr r0, [sp, #0x14]
	mov r1, #2
	tst r0, r1
	beq _022618EE
	ldr r0, _0226191C ; =0x000003D2
	add sp, #0x18
	strh r0, [r4, #2]
	mov r0, #9
	strb r0, [r4, #1]
	add r0, r6, r5
	ldrb r0, [r0, #4]
	lsl r0, r0, #8
	orr r0, r5
	str r0, [r4, #4]
	add r0, r6, r7
	ldrb r0, [r0, #4]
	lsl r0, r0, #8
	orr r0, r7
	str r0, [r4, #8]
	pop {r3, r4, r5, r6, r7, pc}
_022618EE:
	ldr r0, _02261920 ; =0x000003D3
	strh r0, [r4, #2]
	strb r1, [r4, #1]
	add r0, r6, r5
	ldrb r0, [r0, #4]
	lsl r0, r0, #8
	orr r0, r5
	str r0, [r4, #4]
	add sp, #0x18
	pop {r3, r4, r5, r6, r7, pc}
	nop
_02261904: .word 0x00000195
_02261908: .word 0x000003DF
_0226190C: .word 0x000003CF
_02261910: .word 0x000003CE
_02261914: .word 0x000003CD
_02261918: .word 0x000003D1
_0226191C: .word 0x000003D2
_02261920: .word 0x000003D3
_02261924: .word 0x000003E1
	thumb_func_end ov12_022615F0

	thumb_func_start ov12_02261928
ov12_02261928: ; 0x02261928
	push {r3, r4, r5, r6, r7, lr}
	str r0, [sp]
	add r5, r2, #0
	bl BattleSystem_GetBattleType
	add r6, r0, #0
	ldr r0, [sp]
	bl BattleSystem_GetBattleOutcomeFlags
	add r7, r0, #0
	mov r0, #2
	tst r0, r6
	beq _02261956
	ldr r0, [sp]
	mov r1, #3
	bl BattleSystem_GetBattlerFromBattlerType
	add r4, r0, #0
	ldr r0, [sp]
	mov r1, #5
	bl BattleSystem_GetBattlerFromBattlerType
	b _02261960
_02261956:
	ldr r0, [sp]
	mov r1, #1
	bl BattleSystem_GetBattlerFromBattlerType
	add r4, r0, #0
_02261960:
	cmp r7, #1
	beq _0226196E
	cmp r7, #2
	beq _0226198E
	cmp r7, #3
	beq _022619B0
	pop {r3, r4, r5, r6, r7, pc}
_0226196E:
	mov r1, #8
	add r2, r6, #0
	tst r2, r1
	beq _02261984
	ldr r1, _022619D0 ; =0x00000312
	strh r1, [r5, #2]
	mov r1, #0x1a
	strb r1, [r5, #1]
	str r4, [r5, #4]
	str r0, [r5, #8]
	pop {r3, r4, r5, r6, r7, pc}
_02261984:
	ldr r0, _022619D4 ; =0x00000311
	strh r0, [r5, #2]
	strb r1, [r5, #1]
	str r4, [r5, #4]
	pop {r3, r4, r5, r6, r7, pc}
_0226198E:
	mov r1, #8
	add r2, r6, #0
	tst r2, r1
	beq _022619A6
	mov r1, #0xc5
	lsl r1, r1, #2
	strh r1, [r5, #2]
	mov r1, #0x1a
	strb r1, [r5, #1]
	str r4, [r5, #4]
	str r0, [r5, #8]
	pop {r3, r4, r5, r6, r7, pc}
_022619A6:
	ldr r0, _022619D8 ; =0x00000313
	strh r0, [r5, #2]
	strb r1, [r5, #1]
	str r4, [r5, #4]
	pop {r3, r4, r5, r6, r7, pc}
_022619B0:
	mov r1, #8
	add r2, r6, #0
	tst r2, r1
	beq _022619C6
	ldr r1, _022619DC ; =0x00000316
	strh r1, [r5, #2]
	mov r1, #0x1a
	strb r1, [r5, #1]
	str r4, [r5, #4]
	str r0, [r5, #8]
	pop {r3, r4, r5, r6, r7, pc}
_022619C6:
	ldr r0, _022619E0 ; =0x00000315
	strh r0, [r5, #2]
	strb r1, [r5, #1]
	str r4, [r5, #4]
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
_022619D0: .word 0x00000312
_022619D4: .word 0x00000311
_022619D8: .word 0x00000313
_022619DC: .word 0x00000316
_022619E0: .word 0x00000315
	thumb_func_end ov12_02261928

	thumb_func_start ov12_022619E4
ov12_022619E4: ; 0x022619E4
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #8
	mov r6, #0
	add r5, r0, #0
	str r2, [sp]
	add r7, r3, #0
	str r6, [sp, #4]
	add r4, r6, #0
	bl BattleSystem_GetMaxBattlers
	cmp r0, #0
	ble _02261A2C
_022619FC:
	add r0, r4, #0
	bl MaskOfFlagNo
	ldr r1, [sp]
	ldrb r1, [r1, #1]
	tst r0, r1
	beq _02261A20
	add r0, r5, #0
	add r1, r4, #0
	bl BattleSystem_GetFieldSide
	cmp r0, #0
	beq _02261A1E
	ldr r0, [sp, #4]
	add r0, r0, #1
	str r0, [sp, #4]
	b _02261A20
_02261A1E:
	add r6, r6, #1
_02261A20:
	add r0, r5, #0
	add r4, r4, #1
	bl BattleSystem_GetMaxBattlers
	cmp r4, r0
	blt _022619FC
_02261A2C:
	cmp r6, #0
	beq _02261A4A
	ldr r0, [sp, #4]
	cmp r0, #0
	beq _02261A4A
	ldr r0, _02261ACC ; =0x0000030D
	mov r1, #0xc3
	strh r0, [r7, #2]
	mov r0, #0
	strb r0, [r7, #1]
	add r0, r5, #0
	bl BattleSystem_SetBattleOutcomeFlags
	add sp, #8
	pop {r3, r4, r5, r6, r7, pc}
_02261A4A:
	cmp r6, #0
	beq _02261A62
	ldr r0, _02261ACC ; =0x0000030D
	mov r1, #0xc2
	strh r0, [r7, #2]
	mov r0, #0
	strb r0, [r7, #1]
	add r0, r5, #0
	bl BattleSystem_SetBattleOutcomeFlags
	add sp, #8
	pop {r3, r4, r5, r6, r7, pc}
_02261A62:
	add r0, r5, #0
	bl BattleSystem_GetBattleType
	mov r1, #8
	tst r0, r1
	beq _02261A8E
	mov r0, #0xc6
	lsl r0, r0, #2
	strh r0, [r7, #2]
	mov r0, #0x1a
	strb r0, [r7, #1]
	add r0, r5, #0
	mov r1, #3
	bl BattleSystem_GetBattlerFromBattlerType
	str r0, [r7, #4]
	add r0, r5, #0
	mov r1, #5
	bl BattleSystem_GetBattlerFromBattlerType
	str r0, [r7, #8]
	b _02261ABE
_02261A8E:
	add r0, r5, #0
	bl BattleSystem_GetBattleType
	mov r1, #2
	tst r0, r1
	ldr r0, _02261AD0 ; =0x00000317
	beq _02261AAE
	strh r0, [r7, #2]
	mov r0, #8
	strb r0, [r7, #1]
	add r0, r5, #0
	mov r1, #3
	bl BattleSystem_GetBattlerFromBattlerType
	str r0, [r7, #4]
	b _02261ABE
_02261AAE:
	strh r0, [r7, #2]
	mov r0, #8
	strb r0, [r7, #1]
	add r0, r5, #0
	mov r1, #1
	bl BattleSystem_GetBattlerFromBattlerType
	str r0, [r7, #4]
_02261ABE:
	add r0, r5, #0
	mov r1, #0xc1
	bl BattleSystem_SetBattleOutcomeFlags
	add sp, #8
	pop {r3, r4, r5, r6, r7, pc}
	nop
_02261ACC: .word 0x0000030D
_02261AD0: .word 0x00000317
	thumb_func_end ov12_022619E4

	thumb_func_start ov12_02261AD4
ov12_02261AD4: ; 0x02261AD4
	push {r4, r5, r6, lr}
	add r6, r1, #0
	mov r1, #0xef
	add r4, r2, #0
	lsl r1, r1, #2
	strh r1, [r4, #2]
	mov r1, #8
	add r5, r0, #0
	strb r1, [r4, #1]
	bl BattleSystem_GetBattleType
	mov r1, #4
	tst r0, r1
	beq _02261B1A
	add r0, r5, #0
	bl ov12_0223BFC0
	add r1, r0, #0
	add r0, r5, #0
	bl ov12_0223BFCC
	cmp r0, #0
	beq _02261B0E
	add r0, r5, #0
	mov r1, #4
	bl BattleSystem_GetBattlerFromBattlerType
	str r0, [r4, #4]
	b _02261B22
_02261B0E:
	add r0, r5, #0
	mov r1, #2
	bl BattleSystem_GetBattlerFromBattlerType
	str r0, [r4, #4]
	b _02261B22
_02261B1A:
	mov r0, #0x65
	lsl r0, r0, #2
	ldrb r0, [r6, r0]
	str r0, [r4, #4]
_02261B22:
	add r0, r5, #0
	mov r1, #0xc2
	bl BattleSystem_SetBattleOutcomeFlags
	pop {r4, r5, r6, pc}
	thumb_func_end ov12_02261AD4
