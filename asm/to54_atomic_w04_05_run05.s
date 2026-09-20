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
	.public ov12_0225A37C
	.public ov12_0225A414
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
