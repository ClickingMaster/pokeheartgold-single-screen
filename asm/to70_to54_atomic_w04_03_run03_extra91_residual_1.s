	.extern ov12_0225CE28
	.extern ov12_0225D138
	.extern ov12_0225D644
	.extern ov12_02261B2C
	.extern ov12_02261EF0
#include "constants/pokemon.h"
#include "constants/sndseq.h"
	.include "asm/macros.inc"
	.include "overlay_12_battle_controller_opponent.inc"
	.include "global.inc"

.text
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
