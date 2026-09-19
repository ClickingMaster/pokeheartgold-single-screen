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

	thumb_func_start ov12_0225AC1C
ov12_0225AC1C: ; 0x0225AC1C
	push {r3, r4, r5, r6, r7, lr}
	add r5, r0, #0
	add r4, r1, #0
	mov r0, #5
	mov r1, #0x68
	str r2, [sp]
	bl Heap_Alloc
	add r1, r0, #0
	str r5, [r1]
	ldr r0, [sp]
	str r4, [r1, #4]
	ldrb r2, [r0]
	add r0, r1, #0
	add r0, #0x60
	strb r2, [r0]
	mov r0, #0x65
	lsl r0, r0, #2
	ldrb r2, [r4, r0]
	add r0, r1, #0
	add r0, #0x61
	strb r2, [r0]
	add r2, r1, #0
	mov r0, #0
	add r2, #0x62
	strb r0, [r2]
	ldr r2, [sp]
	add r7, r1, #0
	ldrb r3, [r2, #1]
	add r2, r1, #0
	add r2, #0x63
	strb r3, [r2]
	ldr r2, [sp]
	add r3, r1, #0
	ldrb r2, [r2, #2]
	str r2, [r1, #0x64]
	ldr r2, [sp]
	mov ip, r2
_0225AC68:
	mov r4, ip
	ldrh r4, [r4, #4]
	strh r4, [r7, #0x20]
	ldr r4, [sp]
	add r7, r7, #2
	add r6, r4, r0
	add r4, r1, r0
	ldrb r5, [r6, #0xc]
	add r4, #0x28
	strb r5, [r4]
	add r4, r1, r0
	ldrb r5, [r6, #0x10]
	add r4, #0x2c
	strb r5, [r4]
	add r4, r1, r0
	ldrb r5, [r6, #0x14]
	add r4, #0x30
	add r0, r0, #1
	strb r5, [r4]
	ldr r4, [r2, #0x18]
	add r2, r2, #4
	str r4, [r3, #0x34]
	mov r4, ip
	add r4, r4, #2
	add r3, r3, #4
	mov ip, r4
	cmp r0, #4
	blt _0225AC68
	ldr r0, _0225ACAC ; =ov12_02260418
	mov r2, #0
	bl SysTask_CreateOnMainQueue
	pop {r3, r4, r5, r6, r7, pc}
	nop
_0225ACAC: .word ov12_02260418
	thumb_func_end ov12_0225AC1C

	thumb_func_start ov12_0225ACB0
ov12_0225ACB0: ; 0x0225ACB0
	push {r4, r5, r6, lr}
	add r5, r1, #0
	add r6, r0, #0
	ldr r0, [r5, #0x2c]
	add r4, r2, #0
	cmp r0, #0
	bne _0225ACC2
	bl GF_AssertFail
_0225ACC2:
	ldrb r1, [r4, #1]
	add r0, r5, #0
	add r0, #0x72
	strb r1, [r0]
	add r0, r5, #0
	mov r2, #1
	ldr r1, [r5, #0x50]
	add r0, #0x28
	lsl r2, r2, #8
	bl ov12_0226498C
	mov r1, #0x65
	lsl r1, r1, #2
	ldrb r1, [r5, r1]
	ldrb r2, [r4]
	add r0, r6, #0
	bl ov12_0226430C
	pop {r4, r5, r6, pc}
	thumb_func_end ov12_0225ACB0

	thumb_func_start ov12_0225ACE8
ov12_0225ACE8: ; 0x0225ACE8
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #8
	add r5, r1, #0
	mov r1, #0x65
	lsl r1, r1, #2
	ldrb r1, [r5, r1]
	add r6, r0, #0
	add r7, r2, #0
	bl BattleSystem_GetTrainerIndex
	str r0, [sp, #4]
	mov r0, #5
	mov r1, #8
	bl Heap_Alloc
	add r4, r0, #0
	str r6, [r4]
	ldrb r0, [r7]
	strb r0, [r4, #4]
	mov r0, #0x65
	lsl r0, r0, #2
	ldrb r0, [r5, r0]
	strb r0, [r4, #5]
	add r0, r6, #0
	bl BattleSystem_GetTextFrameDelay
	str r0, [sp]
	mov r2, #0x65
	lsl r2, r2, #2
	ldrb r2, [r5, r2]
	ldrb r3, [r7, #1]
	ldr r1, [sp, #4]
	add r0, r6, #0
	bl BattleSystem_PrintTrainerMessage
	strb r0, [r4, #6]
	ldr r0, _0225AD40 ; =ov12_022605D0
	add r1, r4, #0
	mov r2, #0
	bl SysTask_CreateOnMainQueue
	add sp, #8
	pop {r3, r4, r5, r6, r7, pc}
	nop
_0225AD40: .word ov12_022605D0
	thumb_func_end ov12_0225ACE8

	thumb_func_start ov12_0225AD44
ov12_0225AD44: ; 0x0225AD44
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x28
	add r5, r0, #0
	add r3, sp, #4
	add r6, r1, #0
	add r7, r2, #0
	bl ov12_02261390
	add r0, r5, #0
	bl BattleSystem_GetMessageLoader
	str r0, [sp]
	mov r0, #5
	mov r1, #8
	bl Heap_Alloc
	add r4, r0, #0
	str r5, [r4]
	ldrb r0, [r7]
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
	add r2, sp, #4
	bl BattleSystem_PrintBattleMessage
	strb r0, [r4, #6]
	ldr r0, _0225AD98 ; =ov12_022605D0
	add r1, r4, #0
	mov r2, #0
	bl SysTask_CreateOnMainQueue
	add sp, #0x28
	pop {r3, r4, r5, r6, r7, pc}
	nop
_0225AD98: .word ov12_022605D0
	thumb_func_end ov12_0225AD44

	thumb_func_start ov12_0225AD9C
ov12_0225AD9C: ; 0x0225AD9C
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x28
	add r5, r0, #0
	add r3, sp, #4
	add r6, r1, #0
	add r7, r2, #0
	bl ov12_02261464
	add r0, r5, #0
	bl BattleSystem_GetMessageLoader
	str r0, [sp]
	mov r0, #5
	mov r1, #8
	bl Heap_Alloc
	add r4, r0, #0
	str r5, [r4]
	ldrb r0, [r7]
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
	add r2, sp, #4
	bl BattleSystem_PrintBattleMessage
	strb r0, [r4, #6]
	ldr r0, _0225ADF0 ; =ov12_022605D0
	add r1, r4, #0
	mov r2, #0
	bl SysTask_CreateOnMainQueue
	add sp, #0x28
	pop {r3, r4, r5, r6, r7, pc}
	nop
_0225ADF0: .word ov12_022605D0
	thumb_func_end ov12_0225AD9C

	thumb_func_start ov12_0225ADF4
ov12_0225ADF4: ; 0x0225ADF4
	push {r4, r5, r6, r7, lr}
	sub sp, #0x24
	add r5, r0, #0
	add r2, sp, #0
	add r6, r1, #0
	bl ov12_02261544
	add r0, r5, #0
	bl BattleSystem_GetMessageLoader
	add r7, r0, #0
	mov r0, #5
	mov r1, #8
	bl Heap_Alloc
	add r4, r0, #0
	str r5, [r4]
	mov r0, #0x22
	strb r0, [r4, #4]
	mov r0, #0x65
	lsl r0, r0, #2
	ldrb r0, [r6, r0]
	strb r0, [r4, #5]
	add r0, r5, #0
	bl BattleSystem_GetTextFrameDelay
	add r3, r0, #0
	add r0, r5, #0
	add r1, r7, #0
	add r2, sp, #0
	bl BattleSystem_PrintBattleMessage
	strb r0, [r4, #6]
	ldr r0, _0225AE44 ; =ov12_022605D0
	add r1, r4, #0
	mov r2, #0
	bl SysTask_CreateOnMainQueue
	add sp, #0x24
	pop {r4, r5, r6, r7, pc}
	.balign 4, 0
_0225AE44: .word ov12_022605D0
	thumb_func_end ov12_0225ADF4

	thumb_func_start ov12_0225AE48
ov12_0225AE48: ; 0x0225AE48
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x28
	add r5, r0, #0
	add r3, sp, #4
	add r6, r1, #0
	add r7, r2, #0
	bl ov12_022615F0
	add r0, r5, #0
	bl BattleSystem_GetMessageLoader
	str r0, [sp]
	mov r0, #5
	mov r1, #8
	bl Heap_Alloc
	add r4, r0, #0
	str r5, [r4]
	ldrb r0, [r7]
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
	add r2, sp, #4
	bl BattleSystem_PrintBattleMessage
	strb r0, [r4, #6]
	ldr r0, _0225AE9C ; =ov12_022605D0
	add r1, r4, #0
	mov r2, #0
	bl SysTask_CreateOnMainQueue
	add sp, #0x28
	pop {r3, r4, r5, r6, r7, pc}
	nop
_0225AE9C: .word ov12_022605D0
	thumb_func_end ov12_0225AE48
