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

	thumb_func_start ov12_0225CE28
ov12_0225CE28: ; 0x0225CE28
	push {r4, r5, r6, lr}
	sub sp, #8
	add r4, r1, #0
	add r6, r0, #0
	ldr r0, [r4]
	bl BattleSystem_GetBattleType
	add r5, r0, #0
	ldr r0, [r4, #0x20]
	cmp r0, #0x21
	bge _0225CE64
	ldrb r0, [r4, #0x12]
	cmp r0, #2
	blo _0225CE64
	ldr r0, [r4, #0x18]
	cmp r0, #0
	beq _0225CE4E
	cmp r0, #2
	bne _0225CE64
_0225CE4E:
	ldr r0, [r4]
	bl BattleSystem_GetBgConfig
	mov r1, #3
	mov r2, #1
	mov r3, #4
	bl BgSetPosTextAndCommit
	ldr r0, [r4, #0x20]
	add r0, r0, #1
	str r0, [r4, #0x20]
_0225CE64:
	ldrb r0, [r4, #0x12]
	cmp r0, #4
	bls _0225CE6C
	b _0225D132
_0225CE6C:
	add r0, r0, r0
	add r0, pc
	ldrh r0, [r0, #6]
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	add pc, r0
_0225CE78: ; jump table
	.short _0225CE82 - _0225CE78 - 2 ; case 0
	.short _0225CEA4 - _0225CE78 - 2 ; case 1
	.short _0225CEB0 - _0225CE78 - 2 ; case 2
	.short _0225D102 - _0225CE78 - 2 ; case 3
	.short _0225D11C - _0225CE78 - 2 ; case 4
_0225CE82:
	ldr r0, [r4, #0xc]
	bl ManagedSprite_GetNumFrames
	cmp r0, #2
	bls _0225CE9C
	ldr r0, [r4, #0xc]
	mov r1, #0
	bl ManagedSprite_SetAnimationFrame
	ldr r0, [r4, #0xc]
	mov r1, #2
	bl ManagedSprite_SetAnim
_0225CE9C:
	mov r0, #0x1c
	str r0, [r4, #0x1c]
	mov r0, #1
	strb r0, [r4, #0x12]
_0225CEA4:
	ldr r0, [r4, #0x1c]
	sub r0, r0, #1
	str r0, [r4, #0x1c]
	bne _0225CF9A
	mov r0, #2
	strb r0, [r4, #0x12]
_0225CEB0:
	ldr r0, [r4, #8]
	add r1, sp, #4
	ldr r0, [r0]
	add r1, #2
	add r2, sp, #4
	bl ManagedSprite_GetPositionXY
	add r1, sp, #0
	ldr r0, [r4, #0xc]
	add r1, #2
	add r2, sp, #0
	bl ManagedSprite_GetPositionXY
	ldrb r0, [r4, #0x13]
	cmp r0, #2
	ldr r0, [r4, #0x18]
	beq _0225CED4
	b _0225CFEE
_0225CED4:
	cmp r0, #1
	beq _0225CEDC
	cmp r0, #3
	bne _0225CEFE
_0225CEDC:
	add r1, sp, #0
	mov r0, #6
	ldrsh r0, [r1, r0]
	cmp r0, #0xc0
	ldr r0, [r4, #8]
	bge _0225CEF4
	ldr r0, [r0]
	mov r1, #8
	mov r2, #0
	bl ManagedSprite_OffsetPositionXY
	b _0225CEFE
_0225CEF4:
	ldr r0, [r0]
	mov r1, #0xc0
	mov r2, #0x58
	bl ManagedSprite_SetPositionXY
_0225CEFE:
	ldr r0, [r4, #8]
	add r1, sp, #4
	ldr r0, [r0]
	add r1, #2
	add r2, sp, #4
	bl ManagedSprite_GetPositionXY
	ldr r0, [r4, #0x18]
	cmp r0, #1
	beq _0225CF22
	cmp r0, #3
	bne _0225CF34
	cmp r5, #3
	beq _0225CF22
	cmp r5, #0x83
	beq _0225CF22
	cmp r5, #7
	bne _0225CF34
_0225CF22:
	add r3, sp, #0
	mov r1, #6
	mov r2, #0
	ldrsh r1, [r3, r1]
	ldrsh r2, [r3, r2]
	ldr r0, [r4, #0xc]
	bl ManagedSprite_SetPositionXY
	b _0225CF80
_0225CF34:
	cmp r0, #3
	bne _0225CF5E
	add r2, sp, #0
	mov r0, #2
	mov r1, #6
	ldrsh r3, [r2, r0]
	ldrsh r0, [r2, r1]
	sub r0, r3, r0
	strh r0, [r2, #6]
	ldrsh r0, [r2, r1]
	sub r0, #0x18
	strh r0, [r2, #6]
	ldrsh r1, [r2, r1]
	ldr r0, [r4, #0xc]
	mov r2, #0
	neg r1, r1
	lsl r1, r1, #0x10
	asr r1, r1, #0x10
	bl ManagedSprite_OffsetPositionXY
	b _0225CF80
_0225CF5E:
	cmp r0, #5
	bne _0225CF80
	add r2, sp, #0
	mov r1, #6
	mov r0, #2
	ldrsh r3, [r2, r1]
	ldrsh r0, [r2, r0]
	sub r0, r3, r0
	strh r0, [r2, #6]
	ldrsh r0, [r2, r1]
	sub r0, #0x10
	strh r0, [r2, #6]
	ldrsh r1, [r2, r1]
	ldr r0, [r4, #0xc]
	mov r2, #0
	bl ManagedSprite_OffsetPositionXY
_0225CF80:
	add r1, sp, #0
	ldr r0, [r4, #0xc]
	add r1, #2
	add r2, sp, #0
	bl ManagedSprite_GetPositionXY
	mov r0, #0x14
	ldrsh r1, [r4, r0]
	add r3, sp, #0
	mov r0, #2
	ldrsh r0, [r3, r0]
	cmp r0, r1
	bge _0225CF9C
_0225CF9A:
	b _0225D132
_0225CF9C:
	mov r2, #0
	ldrsh r2, [r3, r2]
	ldr r0, [r4, #0xc]
	bl ManagedSprite_SetPositionXY
	ldr r0, [r4, #0x18]
	cmp r0, #1
	beq _0225CFB0
	cmp r0, #3
	bne _0225CFBC
_0225CFB0:
	ldr r0, [r4, #8]
	mov r1, #0xc0
	ldr r0, [r0]
	mov r2, #0x58
	bl ManagedSprite_SetPositionXY
_0225CFBC:
	ldr r0, [r4, #0xc]
	bl ManagedSprite_GetNumFrames
	cmp r0, #1
	bls _0225CFE6
	ldr r0, [r4, #0xc]
	mov r1, #0
	bl ManagedSprite_SetAnimationFrame
	ldr r0, [r4, #0xc]
	mov r1, #1
	bl ManagedSprite_SetAnim
	ldrh r1, [r4, #0x16]
	mov r0, #1
	add sp, #8
	bic r1, r0
	strh r1, [r4, #0x16]
	mov r0, #3
	strb r0, [r4, #0x12]
	pop {r4, r5, r6, pc}
_0225CFE6:
	mov r0, #4
	add sp, #8
	strb r0, [r4, #0x12]
	pop {r4, r5, r6, pc}
_0225CFEE:
	cmp r0, #0
	beq _0225CFF6
	cmp r0, #2
	bne _0225D018
_0225CFF6:
	add r0, sp, #0
	mov r1, #6
	ldrsh r0, [r0, r1]
	cmp r0, #0x40
	ldr r0, [r4, #8]
	ble _0225D00E
	ldr r0, [r0]
	sub r1, #0xe
	mov r2, #0
	bl ManagedSprite_OffsetPositionXY
	b _0225D018
_0225D00E:
	ldr r0, [r0]
	mov r1, #0x40
	mov r2, #0x88
	bl ManagedSprite_SetPositionXY
_0225D018:
	ldr r0, [r4, #8]
	add r1, sp, #4
	ldr r0, [r0]
	add r1, #2
	add r2, sp, #4
	bl ManagedSprite_GetPositionXY
	ldr r0, [r4, #0x18]
	cmp r0, #0
	beq _0225D040
	cmp r0, #2
	bne _0225D052
	cmp r5, #3
	beq _0225D040
	cmp r5, #0x83
	beq _0225D040
	cmp r5, #0x13
	beq _0225D040
	cmp r5, #7
	bne _0225D052
_0225D040:
	add r3, sp, #0
	mov r1, #6
	mov r2, #0
	ldrsh r1, [r3, r1]
	ldrsh r2, [r3, r2]
	ldr r0, [r4, #0xc]
	bl ManagedSprite_SetPositionXY
	b _0225D09E
_0225D052:
	cmp r0, #2
	bne _0225D076
	add r2, sp, #0
	mov r1, #6
	mov r0, #2
	ldrsh r3, [r2, r1]
	ldrsh r0, [r2, r0]
	sub r0, r3, r0
	strh r0, [r2, #6]
	ldrsh r0, [r2, r1]
	sub r0, #0x18
	strh r0, [r2, #6]
	ldrsh r1, [r2, r1]
	ldr r0, [r4, #0xc]
	mov r2, #0
	bl ManagedSprite_OffsetPositionXY
	b _0225D09E
_0225D076:
	cmp r0, #4
	bne _0225D09E
	add r2, sp, #0
	mov r0, #2
	mov r1, #6
	ldrsh r3, [r2, r0]
	ldrsh r0, [r2, r1]
	sub r0, r3, r0
	strh r0, [r2, #6]
	ldrsh r0, [r2, r1]
	sub r0, #0x10
	strh r0, [r2, #6]
	ldrsh r1, [r2, r1]
	ldr r0, [r4, #0xc]
	mov r2, #0
	neg r1, r1
	lsl r1, r1, #0x10
	asr r1, r1, #0x10
	bl ManagedSprite_OffsetPositionXY
_0225D09E:
	add r1, sp, #0
	ldr r0, [r4, #0xc]
	add r1, #2
	add r2, sp, #0
	bl ManagedSprite_GetPositionXY
	ldr r0, [r4, #4]
	cmp r0, #0
	beq _0225D0BC
	add r3, sp, #0
	mov r2, #2
	ldrsh r2, [r3, r2]
	mov r1, #0
	bl Pokepic_SetAttr
_0225D0BC:
	add r1, sp, #0
	mov r0, #2
	ldrsh r1, [r1, r0]
	mov r0, #0x14
	ldrsh r0, [r4, r0]
	cmp r1, r0
	bgt _0225D132
	ldr r0, [r4, #4]
	cmp r0, #0
	beq _0225D0D4
	bl Pokepic_Delete
_0225D0D4:
	mov r1, #0x14
	add r3, sp, #0
	mov r2, #0
	ldrsh r1, [r4, r1]
	ldrsh r2, [r3, r2]
	ldr r0, [r4, #0xc]
	bl ManagedSprite_SetPositionXY
	ldr r0, [r4, #0x18]
	cmp r0, #0
	beq _0225D0EE
	cmp r0, #2
	bne _0225D0FA
_0225D0EE:
	ldr r0, [r4, #8]
	mov r1, #0x40
	ldr r0, [r0]
	mov r2, #0x88
	bl ManagedSprite_SetPositionXY
_0225D0FA:
	mov r0, #4
	add sp, #8
	strb r0, [r4, #0x12]
	pop {r4, r5, r6, pc}
_0225D102:
	ldr r0, [r4, #0xc]
	bl ManagedSprite_IsAnimated
	cmp r0, #1
	bne _0225D118
	ldr r1, [r4, #0xc]
	add r0, r4, #0
	bl ov12_02261DC8
	cmp r0, #0
	beq _0225D132
_0225D118:
	mov r0, #4
	strb r0, [r4, #0x12]
_0225D11C:
	ldrb r1, [r4, #0x11]
	ldrb r2, [r4, #0x10]
	ldr r0, [r4]
	bl ov12_0226430C
	add r0, r4, #0
	bl Heap_Free
	add r0, r6, #0
	bl SysTask_Destroy
_0225D132:
	add sp, #8
	pop {r4, r5, r6, pc}
	.balign 4, 0
	thumb_func_end ov12_0225CE28

	thumb_func_start ov12_0225D138
ov12_0225D138: ; 0x0225D138
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x30
	add r4, r1, #0
	add r5, r0, #0
	ldr r0, [r4]
	bl BattleSystem_GetBattleType
	add r6, r0, #0
	ldrb r0, [r4, #0xa]
	cmp r0, #6
	bls _0225D150
	b _0225D62E
_0225D150:
	add r0, r0, r0
	add r0, pc
	ldrh r0, [r0, #6]
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	add pc, r0
_0225D15C: ; jump table
	.short _0225D16A - _0225D15C - 2 ; case 0
	.short _0225D244 - _0225D15C - 2 ; case 1
	.short _0225D368 - _0225D15C - 2 ; case 2
	.short _0225D522 - _0225D15C - 2 ; case 3
	.short _0225D5D2 - _0225D15C - 2 ; case 4
	.short _0225D5F0 - _0225D15C - 2 ; case 5
	.short _0225D618 - _0225D15C - 2 ; case 6
_0225D16A:
	ldr r0, [r4, #0x10]
	cmp r0, #0
	bne _0225D178
	mov r0, #1
	add sp, #0x30
	strb r0, [r4, #0xa]
	pop {r3, r4, r5, r6, r7, pc}
_0225D178:
	cmp r0, #4
	bhi _0225D192
	add r0, r0, r0
	add r0, pc
	ldrh r0, [r0, #6]
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	add pc, r0
_0225D188: ; jump table
	.short _0225D192 - _0225D188 - 2 ; case 0
	.short _0225D192 - _0225D188 - 2 ; case 1
	.short _0225D1A0 - _0225D188 - 2 ; case 2
	.short _0225D1AE - _0225D188 - 2 ; case 3
	.short _0225D1BC - _0225D188 - 2 ; case 4
_0225D192:
	mov r0, #0xf
	str r0, [sp, #8]
	mov r0, #3
	str r0, [r4, #0x18]
	mov r0, #5
	str r0, [sp, #0x18]
	b _0225D1CA
_0225D1A0:
	mov r0, #0xc
	str r0, [sp, #8]
	mov r0, #0
	str r0, [r4, #0x18]
	ldr r0, _0225D4F0 ; =0x00000401
	str r0, [sp, #0x18]
	b _0225D1CA
_0225D1AE:
	mov r0, #0xd
	str r0, [sp, #8]
	mov r0, #0
	str r0, [r4, #0x18]
	ldr r0, _0225D4F4 ; =0x00000402
	str r0, [sp, #0x18]
	b _0225D1CA
_0225D1BC:
	mov r0, #0xf
	str r0, [sp, #8]
	mov r0, #3
	str r0, [r4, #0x18]
	mov r0, #1
	lsl r0, r0, #0xa
	str r0, [sp, #0x18]
_0225D1CA:
	mov r0, #5
	str r0, [sp, #0xc]
	mov r0, #4
	str r0, [sp, #0x10]
	ldrb r0, [r4, #9]
	str r0, [sp, #0x14]
	ldr r0, [r4]
	bl BattleSystem_GetSpriteSystem
	str r0, [sp, #0x24]
	ldr r0, [r4]
	bl BattleSystem_GetPaletteData
	str r0, [sp, #0x28]
	mov r0, #0
	str r0, [sp, #0x20]
	ldr r0, [r4]
	str r0, [sp, #0x2c]
	add r0, sp, #8
	bl ov07_02233DB8
	ldr r1, [r4, #4]
	add r1, #0x88
	str r0, [r1]
	ldr r0, [r4, #4]
	mov r1, #0x64
	add r0, #0x88
	ldr r0, [r0]
	bl ov07_022344C4
	ldr r0, [r4, #4]
	mov r1, #2
	add r0, #0x88
	ldr r0, [r0]
	bl ov07_022344D0
	ldr r0, [r4, #4]
	mov r1, #0
	add r0, #0x88
	ldr r0, [r0]
	bl ov07_022344C0
	ldr r0, [r4, #4]
	mov r1, #0
	ldr r0, [r0, #0x18]
	bl ManagedSprite_SetAnimationFrame
	ldr r0, [r4, #4]
	mov r1, #1
	ldr r0, [r0, #0x18]
	bl ManagedSprite_SetAnim
	ldr r0, [r4, #4]
	mov r1, #1
	ldr r0, [r0, #0x18]
	bl ManagedSprite_SetAnimateFlag
	mov r0, #3
	add sp, #0x30
	strb r0, [r4, #0xa]
	pop {r3, r4, r5, r6, r7, pc}
_0225D244:
	ldrb r0, [r4, #0xb]
	cmp r0, #2
	ldr r0, [r4, #4]
	bne _0225D344
	ldr r0, [r0, #0x18]
	mov r1, #5
	mov r2, #0
	bl ManagedSprite_OffsetPositionXY
	ldr r0, [r4, #4]
	add r1, sp, #4
	ldr r0, [r0, #0x18]
	add r1, #2
	add r2, sp, #4
	bl ManagedSprite_GetPositionXY
	add r1, sp, #4
	mov r0, #2
	ldrsh r0, [r1, r0]
	cmp r0, #0xa0
	blt _0225D2D6
	ldr r0, [r4, #4]
	add r0, #0x88
	ldr r0, [r0]
	bl ov07_02233F20
	cmp r0, #0
	beq _0225D2D6
	ldr r0, [r4, #4]
	mov r1, #1
	add r0, #0x88
	ldr r0, [r0]
	bl ov07_0223449C
	ldr r0, [r4, #4]
	mov r1, #0
	add r0, #0x88
	ldr r0, [r0]
	bl ov07_02233EFC
	mov r0, #2
	tst r0, r6
	beq _0225D2E4
	mov r0, #8
	tst r0, r6
	bne _0225D2E4
	ldrb r1, [r4, #9]
	ldr r0, [r4]
	bl BattleSystem_GetBattlerIdPartner
	add r1, r0, #0
	ldr r0, [r4]
	bl BattleSystem_GetOpponentData
	add r4, r0, #0
	add r0, #0x88
	ldr r0, [r0]
	mov r1, #1
	bl ov07_0223449C
	add r0, r4, #0
	add r0, #0x88
	ldr r0, [r0]
	mov r1, #0
	bl ov07_02233EFC
	add r4, #0x88
	ldr r0, [r4]
	mov r1, #0xc
	bl ov07_022344DC
	add sp, #0x30
	pop {r3, r4, r5, r6, r7, pc}
_0225D2D6:
	add r1, sp, #4
	mov r0, #2
	ldrsh r1, [r1, r0]
	mov r0, #0x4a
	lsl r0, r0, #2
	cmp r1, r0
	bge _0225D2E6
_0225D2E4:
	b _0225D62E
_0225D2E6:
	ldr r0, [r4]
	bl BattleSystem_GetSpriteManager
	add r5, r0, #0
	ldr r0, [r4, #4]
	ldr r0, [r0, #0x18]
	bl Sprite_DeleteAndFreeResources
	ldr r0, [r4, #4]
	mov r1, #0
	str r1, [r0, #0x18]
	ldr r2, [r4, #4]
	ldr r1, _0225D4F8 ; =0x00000195
	add r0, r5, #0
	ldrb r2, [r2, r1]
	ldr r1, _0225D4FC ; =0x00004E2F
	add r1, r2, r1
	bl SpriteManager_UnloadCharObjById
	ldr r2, [r4, #4]
	ldr r1, _0225D4F8 ; =0x00000195
	add r0, r5, #0
	ldrb r2, [r2, r1]
	ldr r1, _0225D500 ; =0x00004E2A
	add r1, r2, r1
	bl SpriteManager_UnloadPlttObjById
	ldr r2, [r4, #4]
	ldr r1, _0225D4F8 ; =0x00000195
	add r0, r5, #0
	ldrb r2, [r2, r1]
	ldr r1, _0225D504 ; =0x00004E27
	add r1, r2, r1
	bl SpriteManager_UnloadCellObjById
	ldr r2, [r4, #4]
	ldr r1, _0225D4F8 ; =0x00000195
	add r0, r5, #0
	ldrb r2, [r2, r1]
	ldr r1, _0225D504 ; =0x00004E27
	add r1, r2, r1
	bl SpriteManager_UnloadAnimObjById
	mov r0, #6
	add sp, #0x30
	strb r0, [r4, #0xa]
	pop {r3, r4, r5, r6, r7, pc}
_0225D344:
	ldr r0, [r0, #0x18]
	mov r1, #0
	bl ManagedSprite_SetAnimationFrame
	ldr r0, [r4, #4]
	mov r1, #1
	ldr r0, [r0, #0x18]
	bl ManagedSprite_SetAnim
	ldr r0, [r4, #4]
	mov r1, #1
	ldr r0, [r0, #0x18]
	bl ManagedSprite_SetAnimateFlag
	mov r0, #2
	add sp, #0x30
	strb r0, [r4, #0xa]
	pop {r3, r4, r5, r6, r7, pc}
_0225D368:
	ldr r0, [r4, #4]
	mov r1, #4
	ldr r0, [r0, #0x18]
	mvn r1, r1
	mov r2, #0
	bl ManagedSprite_OffsetPositionXY
	ldr r0, [r4, #4]
	add r1, sp, #4
	ldr r0, [r0, #0x18]
	add r1, #2
	add r2, sp, #4
	bl ManagedSprite_GetPositionXY
	ldr r1, [r4, #4]
	add r0, r1, #0
	add r0, #0x88
	ldr r0, [r0]
	cmp r0, #0
	beq _0225D48E
	ldr r0, [r1, #0x18]
	bl ManagedSprite_GetAnimationFrame
	ldr r1, [r4, #0xc]
	str r0, [sp]
	lsl r5, r0, #2
	mov r0, #0x18
	add r2, r1, #0
	mul r2, r0
	ldr r0, _0225D508 ; =ov12_0226D1E8
	add r0, r0, r2
	ldrsh r1, [r5, r0]
	ldr r0, _0225D50C ; =0x00007FFF
	cmp r1, r0
	beq _0225D48E
	ldr r0, [r4, #4]
	mov r1, #1
	add r0, #0x88
	ldr r0, [r0]
	bl ov07_0223449C
	ldr r1, [r4, #0xc]
	mov r0, #0x18
	add r3, r1, #0
	mul r3, r0
	ldr r2, _0225D508 ; =ov12_0226D1E8
	add r7, sp, #4
	mov r1, #2
	add r2, r2, r3
	ldrsh r1, [r7, r1]
	ldrsh r2, [r5, r2]
	ldr r0, [r4, #4]
	add r1, r1, r2
	mov r2, #0
	ldrsh r7, [r7, r2]
	ldr r2, _0225D510 ; =ov12_0226D1EA
	add r0, #0x88
	add r2, r2, r3
	ldrsh r2, [r5, r2]
	lsl r1, r1, #0x10
	ldr r0, [r0]
	add r2, r7, r2
	lsl r2, r2, #0x10
	asr r1, r1, #0x10
	asr r2, r2, #0x10
	bl ov07_022344A8
	ldr r0, [sp]
	cmp r0, #3
	bne _0225D48E
	ldr r0, [r4, #4]
	add r0, #0x88
	ldr r0, [r0]
	bl ov07_02233F20
	cmp r0, #0
	beq _0225D48E
	ldr r0, [r4, #4]
	mov r1, #0
	add r0, #0x88
	ldr r0, [r0]
	bl ov07_02233EFC
	ldr r0, [r4, #4]
	mov r1, #1
	add r0, #0x88
	ldr r0, [r0]
	bl ov07_022344C0
	mov r0, #2
	tst r0, r6
	beq _0225D48E
	mov r0, #8
	tst r0, r6
	bne _0225D48E
	ldrb r1, [r4, #9]
	ldr r0, [r4]
	bl BattleSystem_GetBattlerIdPartner
	add r1, r0, #0
	ldr r0, [r4]
	bl BattleSystem_GetOpponentData
	ldr r1, [r4, #0xc]
	add r6, r0, #0
	add r3, r1, #0
	mov r0, #0x18
	mul r3, r0
	ldr r2, _0225D508 ; =ov12_0226D1E8
	add r0, r6, #0
	add r2, r2, r3
	add r0, #0x88
	add r7, sp, #4
	mov r1, #2
	ldrsh r1, [r7, r1]
	ldrsh r2, [r5, r2]
	ldr r0, [r0]
	add r1, r1, r2
	mov r2, #0
	ldrsh r7, [r7, r2]
	ldr r2, _0225D510 ; =ov12_0226D1EA
	lsl r1, r1, #0x10
	add r2, r2, r3
	ldrsh r2, [r5, r2]
	asr r1, r1, #0x10
	add r2, r7, r2
	lsl r2, r2, #0x10
	asr r2, r2, #0x10
	bl ov07_022344A8
	add r0, r6, #0
	add r0, #0x88
	ldr r0, [r0]
	mov r1, #1
	bl ov07_0223449C
	add r0, r6, #0
	add r0, #0x88
	ldr r0, [r0]
	mov r1, #0
	bl ov07_02233EFC
	add r6, #0x88
	ldr r0, [r6]
	mov r1, #1
	bl ov07_022344C0
_0225D48E:
	add r1, sp, #4
	mov r0, #2
	ldrsh r1, [r1, r0]
	sub r0, #0x2a
	cmp r1, r0
	ble _0225D49C
	b _0225D62E
_0225D49C:
	ldr r0, [r4]
	bl BattleSystem_GetSpriteManager
	add r5, r0, #0
	ldr r0, [r4, #4]
	ldr r0, [r0, #0x18]
	bl Sprite_DeleteAndFreeResources
	ldr r0, [r4, #4]
	mov r1, #0
	str r1, [r0, #0x18]
	ldr r2, [r4, #4]
	ldr r1, _0225D4F8 ; =0x00000195
	add r0, r5, #0
	ldrb r2, [r2, r1]
	ldr r1, _0225D4FC ; =0x00004E2F
	add r1, r2, r1
	bl SpriteManager_UnloadCharObjById
	ldr r2, [r4, #4]
	ldr r1, _0225D4F8 ; =0x00000195
	add r0, r5, #0
	ldrb r2, [r2, r1]
	ldr r1, _0225D500 ; =0x00004E2A
	add r1, r2, r1
	bl SpriteManager_UnloadPlttObjById
	ldr r2, [r4, #4]
	ldr r1, _0225D4F8 ; =0x00000195
	add r0, r5, #0
	ldrb r2, [r2, r1]
	ldr r1, _0225D504 ; =0x00004E27
	add r1, r2, r1
	bl SpriteManager_UnloadCellObjById
	ldr r2, [r4, #4]
	ldr r1, _0225D4F8 ; =0x00000195
	add r0, r5, #0
	ldrb r2, [r2, r1]
	ldr r1, _0225D504 ; =0x00004E27
	b _0225D514
	nop
_0225D4F0: .word 0x00000401
_0225D4F4: .word 0x00000402
_0225D4F8: .word 0x00000195
_0225D4FC: .word 0x00004E2F
_0225D500: .word 0x00004E2A
_0225D504: .word 0x00004E27
_0225D508: .word ov12_0226D1E8
_0225D50C: .word 0x00007FFF
_0225D510: .word ov12_0226D1EA
_0225D514:
	add r1, r2, r1
	bl SpriteManager_UnloadAnimObjById
	mov r0, #6
	add sp, #0x30
	strb r0, [r4, #0xa]
	pop {r3, r4, r5, r6, r7, pc}
_0225D522:
	ldr r0, [r4, #4]
	add r1, sp, #4
	ldr r0, [r0, #0x18]
	add r1, #2
	add r2, sp, #4
	bl ManagedSprite_GetPositionXY
	ldr r0, [r4, #4]
	ldr r0, [r0, #0x18]
	bl ManagedSprite_GetAnimationFrame
	add r7, r0, #0
	cmp r7, #4
	bne _0225D54A
	mov r0, #8
	str r0, [r4, #0x14]
	mov r0, #4
	add sp, #0x30
	strb r0, [r4, #0xa]
	pop {r3, r4, r5, r6, r7, pc}
_0225D54A:
	ldr r0, [r4, #4]
	add r0, #0x88
	ldr r0, [r0]
	cmp r0, #0
	beq _0225D62E
	ldr r2, [r4, #0xc]
	mov r1, #0x18
	add r5, r2, #0
	mul r5, r1
	ldr r1, _0225D634 ; =ov12_0226D1E8
	lsl r6, r7, #2
	add r1, r1, r5
	ldrsh r1, [r6, r1]
	ldr r2, _0225D638 ; =0x00007FFF
	cmp r1, r2
	beq _0225D62E
	add r3, sp, #4
	mov r2, #2
	ldrsh r2, [r3, r2]
	add r1, r2, r1
	mov r2, #0
	ldrsh r3, [r3, r2]
	ldr r2, _0225D63C ; =ov12_0226D1EA
	lsl r1, r1, #0x10
	add r2, r2, r5
	ldrsh r2, [r6, r2]
	asr r1, r1, #0x10
	add r2, r3, r2
	lsl r2, r2, #0x10
	asr r2, r2, #0x10
	bl ov07_022344A8
	cmp r7, #3
	bne _0225D62E
	ldr r0, [r4, #4]
	ldr r5, [r4, #0x18]
	add r0, #0x88
	ldr r0, [r0]
	bl ov07_02233F20
	cmp r5, r0
	beq _0225D62E
	ldr r0, [r4, #4]
	add r1, r5, #0
	add r0, #0x88
	ldr r0, [r0]
	bl ov07_02233EFC
	ldr r0, [r4, #4]
	mov r1, #1
	add r0, #0x88
	ldr r0, [r0]
	bl ov07_022344D0
	ldr r0, [r4, #4]
	mov r1, #1
	add r0, #0x88
	ldr r0, [r0]
	bl ov07_022344C0
	ldr r0, [r4, #0x18]
	cmp r0, #3
	beq _0225D62E
	ldr r0, _0225D640 ; =0x0000070A
	bl PlaySE
	add sp, #0x30
	pop {r3, r4, r5, r6, r7, pc}
_0225D5D2:
	ldr r0, [r4, #0x14]
	sub r0, r0, #1
	str r0, [r4, #0x14]
	bne _0225D62E
	ldr r0, [r4, #0x18]
	cmp r0, #3
	bne _0225D5E8
	mov r0, #6
	add sp, #0x30
	strb r0, [r4, #0xa]
	pop {r3, r4, r5, r6, r7, pc}
_0225D5E8:
	mov r0, #5
	add sp, #0x30
	strb r0, [r4, #0xa]
	pop {r3, r4, r5, r6, r7, pc}
_0225D5F0:
	ldr r0, [r4, #4]
	add r0, #0x88
	ldr r0, [r0]
	bl ov07_02233E88
	cmp r0, #1
	beq _0225D62E
	ldr r0, [r4, #4]
	add r0, #0x88
	ldr r0, [r0]
	bl ov07_02233ECC
	ldr r0, [r4, #4]
	mov r1, #0
	add r0, #0x88
	str r1, [r0]
	mov r0, #6
	add sp, #0x30
	strb r0, [r4, #0xa]
	pop {r3, r4, r5, r6, r7, pc}
_0225D618:
	ldrb r1, [r4, #9]
	ldrb r2, [r4, #8]
	ldr r0, [r4]
	bl ov12_0226430C
	add r0, r4, #0
	bl Heap_Free
	add r0, r5, #0
	bl SysTask_Destroy
_0225D62E:
	add sp, #0x30
	pop {r3, r4, r5, r6, r7, pc}
	nop
_0225D634: .word ov12_0226D1E8
_0225D638: .word 0x00007FFF
_0225D63C: .word ov12_0226D1EA
_0225D640: .word 0x0000070A
	thumb_func_end ov12_0225D138

	thumb_func_start ov12_0225D644
ov12_0225D644: ; 0x0225D644
	push {r3, r4, r5, r6, r7, lr}
	add r5, r1, #0
	add r6, r0, #0
	ldr r0, [r5]
	bl BattleSystem_GetBattleType
	add r4, r0, #0
	ldrb r0, [r5, #0xa]
	cmp r0, #0
	beq _0225D664
	cmp r0, #1
	beq _0225D71C
	cmp r0, #2
	bne _0225D662
	b _0225D85A
_0225D662:
	pop {r3, r4, r5, r6, r7, pc}
_0225D664:
	ldrb r0, [r5, #0xb]
	cmp r0, #2
	ldr r0, [r5, #4]
	bne _0225D6F0
	ldr r0, [r0, #0x18]
	mov r1, #5
	mov r2, #0
	bl ManagedSprite_OffsetPositionXY
	ldr r0, [r5, #4]
	add r1, sp, #0
	ldr r0, [r0, #0x18]
	add r1, #2
	add r2, sp, #0
	bl ManagedSprite_GetPositionXY
	add r1, sp, #0
	mov r0, #2
	ldrsh r1, [r1, r0]
	mov r0, #0x4a
	lsl r0, r0, #2
	cmp r1, r0
	bge _0225D694
	b _0225D870
_0225D694:
	ldr r0, [r5]
	bl BattleSystem_GetSpriteManager
	add r4, r0, #0
	ldr r0, [r5, #4]
	ldr r0, [r0, #0x18]
	bl Sprite_DeleteAndFreeResources
	ldr r0, [r5, #4]
	mov r1, #0
	str r1, [r0, #0x18]
	ldr r2, [r5, #4]
	ldr r1, _0225D874 ; =0x00000195
	add r0, r4, #0
	ldrb r2, [r2, r1]
	ldr r1, _0225D878 ; =0x00004E2F
	add r1, r2, r1
	bl SpriteManager_UnloadCharObjById
	ldr r2, [r5, #4]
	ldr r1, _0225D874 ; =0x00000195
	add r0, r4, #0
	ldrb r2, [r2, r1]
	ldr r1, _0225D87C ; =0x00004E2A
	add r1, r2, r1
	bl SpriteManager_UnloadPlttObjById
	ldr r2, [r5, #4]
	ldr r1, _0225D874 ; =0x00000195
	add r0, r4, #0
	ldrb r2, [r2, r1]
	ldr r1, _0225D880 ; =0x00004E27
	add r1, r2, r1
	bl SpriteManager_UnloadCellObjById
	ldr r2, [r5, #4]
	ldr r1, _0225D874 ; =0x00000195
	add r0, r4, #0
	ldrb r2, [r2, r1]
	ldr r1, _0225D880 ; =0x00004E27
	add r1, r2, r1
	bl SpriteManager_UnloadAnimObjById
	mov r0, #2
	strb r0, [r5, #0xa]
	pop {r3, r4, r5, r6, r7, pc}
_0225D6F0:
	ldr r0, [r0, #0x18]
	mov r1, #1
	bl ManagedSprite_SetPriority
	ldr r0, [r5, #4]
	mov r1, #0
	ldr r0, [r0, #0x18]
	bl ManagedSprite_SetAnimationFrame
	ldr r0, [r5, #4]
	mov r1, #1
	ldr r0, [r0, #0x18]
	bl ManagedSprite_SetAnim
	ldr r0, [r5, #4]
	mov r1, #1
	ldr r0, [r0, #0x18]
	bl ManagedSprite_SetAnimateFlag
	mov r0, #1
	strb r0, [r5, #0xa]
	pop {r3, r4, r5, r6, r7, pc}
_0225D71C:
	ldr r0, [r5, #4]
	mov r1, #4
	ldr r0, [r0, #0x18]
	mvn r1, r1
	mov r2, #0
	bl ManagedSprite_OffsetPositionXY
	ldr r0, [r5, #4]
	add r1, sp, #0
	ldr r0, [r0, #0x18]
	add r1, #2
	add r2, sp, #0
	bl ManagedSprite_GetPositionXY
	ldr r0, [r5, #4]
	add r1, r0, #0
	add r1, #0x88
	ldr r1, [r1]
	cmp r1, #0
	beq _0225D7F2
	mov r1, #2
	tst r1, r4
	beq _0225D7F2
	mov r1, #8
	tst r1, r4
	bne _0225D7F2
	ldr r0, [r0, #0x18]
	bl ManagedSprite_GetAnimationFrame
	add r7, r0, #0
	ldr r1, [r5, #0xc]
	mov r0, #0x18
	add r2, r1, #0
	mul r2, r0
	ldr r0, _0225D884 ; =ov12_0226D1E8
	lsl r6, r7, #2
	add r0, r0, r2
	ldrsh r1, [r6, r0]
	ldr r0, _0225D888 ; =0x00007FFF
	cmp r1, r0
	beq _0225D7F2
	ldrb r1, [r5, #9]
	ldr r0, [r5]
	bl BattleSystem_GetBattlerIdPartner
	add r1, r0, #0
	ldr r0, [r5]
	bl BattleSystem_GetOpponentData
	add r4, r0, #0
	cmp r7, #3
	bne _0225D7F2
	add r0, #0x88
	ldr r0, [r0]
	bl ov07_02233F20
	cmp r0, #0
	beq _0225D7F2
	ldr r1, [r5, #0xc]
	mov r0, #0x18
	add r3, r1, #0
	mul r3, r0
	ldr r2, _0225D884 ; =ov12_0226D1E8
	add r7, sp, #0
	mov r1, #2
	add r2, r2, r3
	ldrsh r1, [r7, r1]
	ldrsh r2, [r6, r2]
	add r0, r4, #0
	add r0, #0x88
	add r1, r1, r2
	mov r2, #0
	ldrsh r7, [r7, r2]
	ldr r2, _0225D88C ; =ov12_0226D1EA
	lsl r1, r1, #0x10
	add r2, r2, r3
	ldrsh r2, [r6, r2]
	ldr r0, [r0]
	asr r1, r1, #0x10
	add r2, r7, r2
	lsl r2, r2, #0x10
	asr r2, r2, #0x10
	bl ov07_022344A8
	add r0, r4, #0
	add r0, #0x88
	ldr r0, [r0]
	mov r1, #1
	bl ov07_0223449C
	add r0, r4, #0
	add r0, #0x88
	ldr r0, [r0]
	mov r1, #0
	bl ov07_02233EFC
	add r0, r4, #0
	add r0, #0x88
	ldr r0, [r0]
	mov r1, #1
	bl ov07_022344C0
	add r4, #0x88
	ldr r0, [r4]
	mov r1, #1
	bl ov07_022344D0
_0225D7F2:
	add r1, sp, #0
	mov r0, #2
	ldrsh r1, [r1, r0]
	sub r0, #0x2a
	cmp r1, r0
	bgt _0225D870
	ldr r0, [r5]
	bl BattleSystem_GetSpriteManager
	add r4, r0, #0
	ldr r0, [r5, #4]
	ldr r0, [r0, #0x18]
	bl Sprite_DeleteAndFreeResources
	ldr r0, [r5, #4]
	mov r1, #0
	str r1, [r0, #0x18]
	ldr r2, [r5, #4]
	ldr r1, _0225D874 ; =0x00000195
	add r0, r4, #0
	ldrb r2, [r2, r1]
	ldr r1, _0225D878 ; =0x00004E2F
	add r1, r2, r1
	bl SpriteManager_UnloadCharObjById
	ldr r2, [r5, #4]
	ldr r1, _0225D874 ; =0x00000195
	add r0, r4, #0
	ldrb r2, [r2, r1]
	ldr r1, _0225D87C ; =0x00004E2A
	add r1, r2, r1
	bl SpriteManager_UnloadPlttObjById
	ldr r2, [r5, #4]
	ldr r1, _0225D874 ; =0x00000195
	add r0, r4, #0
	ldrb r2, [r2, r1]
	ldr r1, _0225D880 ; =0x00004E27
	add r1, r2, r1
	bl SpriteManager_UnloadCellObjById
	ldr r2, [r5, #4]
	ldr r1, _0225D874 ; =0x00000195
	add r0, r4, #0
	ldrb r2, [r2, r1]
	ldr r1, _0225D880 ; =0x00004E27
	add r1, r2, r1
	bl SpriteManager_UnloadAnimObjById
	mov r0, #2
	strb r0, [r5, #0xa]
	pop {r3, r4, r5, r6, r7, pc}
_0225D85A:
	ldrb r1, [r5, #9]
	ldrb r2, [r5, #8]
	ldr r0, [r5]
	bl ov12_0226430C
	add r0, r5, #0
	bl Heap_Free
	add r0, r6, #0
	bl SysTask_Destroy
_0225D870:
	pop {r3, r4, r5, r6, r7, pc}
	nop
_0225D874: .word 0x00000195
_0225D878: .word 0x00004E2F
_0225D87C: .word 0x00004E2A
_0225D880: .word 0x00004E27
_0225D884: .word ov12_0226D1E8
_0225D888: .word 0x00007FFF
_0225D88C: .word ov12_0226D1EA
	thumb_func_end ov12_0225D644
