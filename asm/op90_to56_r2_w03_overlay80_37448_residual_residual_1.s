	.extern ov80_02237820
	.extern ov80_02237850
	.extern ov80_02237888
	.extern ov80_02237894
	.extern ov80_022378F8
	.extern ov80_02237980
	.include "asm/macros.inc"
	.include "overlay_80_022372D8.inc"
	.include "global.inc"

.text
	thumb_func_start ov80_022375D0
ov80_022375D0: ; 0x022375D0
	push {r4, r5, r6, r7, lr}
	sub sp, #0x64
	add r5, r0, #0
	ldrb r0, [r5, #5]
	add r6, r1, #0
	lsl r0, r0, #0x19
	lsr r7, r0, #0x18
	ldrb r0, [r5, #4]
	bl ov80_0223787C
	str r0, [sp, #0x18]
	ldrb r0, [r5, #4]
	bl ov80_02237888
	str r0, [sp, #0x10]
	ldr r0, _02237818 ; =0x000006FC
	ldr r0, [r5, r0]
	bl SaveArray_Party_Get
	str r0, [sp, #0x1c]
	bl HealParty
	ldrb r0, [r5, #4]
	bl ov80_02237850
	add r1, r0, #0
	mov r0, #0xb
	bl BattleSetup_New
	ldr r1, [r6, #0xc]
	add r4, r0, #0
	str r1, [sp]
	ldr r1, [r6, #0x1c]
	str r1, [sp, #4]
	ldr r2, [r6, #8]
	ldr r3, [r6, #0x18]
	mov r1, #0
	bl sub_02051D18
	mov r0, #0x53
	mov r1, #0x16
	lsl r0, r0, #2
	str r1, [r4, r0]
	add r0, r0, #4
	str r1, [r4, r0]
	ldr r0, [r4, #4]
	ldr r1, [sp, #0x18]
	bl Party_InitWithMaxSize
	mov r0, #0xb
	bl AllocMonZeroed
	str r0, [sp, #0x20]
	ldr r0, [sp, #0x18]
	mov r6, #0
	cmp r0, #0
	ble _02237668
_02237642:
	mov r1, #0x26
	add r2, r5, r6
	lsl r1, r1, #4
	ldrb r1, [r2, r1]
	ldr r0, [sp, #0x1c]
	bl Party_GetMonByIndex
	ldr r1, [sp, #0x20]
	bl CopyPokemonToPokemon
	ldr r1, [sp, #0x20]
	add r0, r4, #0
	mov r2, #0
	bl BattleSetup_AddMonToParty
	ldr r0, [sp, #0x18]
	add r6, r6, #1
	cmp r6, r0
	blt _02237642
_02237668:
	ldr r0, [sp, #0x20]
	bl Heap_Free
	add r0, r4, #0
	bl BattleSetup_SetAllySideBattlersToPlayer
	lsl r0, r7, #1
	str r0, [sp, #0x28]
	add r6, r5, #0
	ldr r1, [sp, #0x28]
	add r6, #0x18
	ldrh r1, [r6, r1]
	add r0, sp, #0x34
	mov r2, #0xb
	mov r3, #0xcc
	bl ov80_02229F04
	bl Heap_Free
	mov r0, #0xb
	str r0, [sp]
	ldr r2, [sp, #0x10]
	add r0, r4, #0
	add r1, sp, #0x34
	mov r3, #1
	bl ov80_0222A480
	ldr r0, [r4, #8]
	ldr r1, [sp, #0x10]
	bl Party_InitWithMaxSize
	ldr r1, _0223781C ; =0x000006F5
	ldrb r2, [r5, #4]
	ldrb r0, [r5, r1]
	add r1, #0xf
	add r3, r5, r1
	lsl r1, r2, #3
	add r1, r2, r1
	add r1, r3, r1
	bl sub_02030BD0
	str r0, [sp, #0x24]
	ldrb r0, [r5, #4]
	cmp r0, #2
	bne _022376C6
	mov r0, #9
	str r0, [sp, #0x24]
_022376C6:
	ldr r2, [sp, #0x24]
	add r0, r5, #0
	add r1, r7, #0
	bl ov80_02237980
	mov r2, #0
	add r1, r4, #0
_022376D4:
	add r2, r2, #1
	str r0, [r1, #0x34]
	add r1, #0x34
	cmp r2, #4
	blt _022376D4
	mov r0, #0x38
	mul r0, r7
	str r0, [sp, #0x14]
	ldr r0, [sp, #0x10]
	mov r3, #0x29
	str r0, [sp]
	mov r0, #0xb
	str r0, [sp, #4]
	mov r0, #0xce
	str r0, [sp, #8]
	ldr r2, [sp, #0x28]
	lsl r3, r3, #4
	add r1, r5, r3
	ldr r0, [sp, #0x14]
	ldrh r2, [r6, r2]
	sub r3, #0x28
	add r0, r1, r0
	add r6, r5, r3
	lsl r3, r7, #1
	ldr r1, [sp, #0x24]
	add r3, r6, r3
	bl ov80_02237894
	mov r0, #0xb
	bl AllocMonZeroed
	add r6, r0, #0
	mov r0, #0
	str r0, [sp, #0xc]
	ldr r0, [sp, #0x10]
	cmp r0, #0
	ble _0223776E
	mov r0, #0x29
	lsl r0, r0, #4
	add r0, r5, r0
	str r0, [sp, #0x2c]
	mov r0, #0x38
	mul r0, r7
	str r0, [sp, #0x30]
_0223772C:
	add r0, r5, #0
	add r1, r7, #0
	bl ov80_02237820
	cmp r0, #0
	bne _0223772C
	ldr r1, [sp, #0x24]
	add r0, r5, #0
	bl ov80_022378F8
	add r2, r0, #0
	lsl r2, r2, #0x18
	ldr r1, [sp, #0x2c]
	ldr r0, [sp, #0x30]
	lsr r2, r2, #0x18
	add r0, r1, r0
	add r1, r6, #0
	bl ov80_0222A140
	add r0, r6, #0
	bl UpdateMonAbility
	add r0, r4, #0
	add r1, r6, #0
	mov r2, #1
	bl BattleSetup_AddMonToParty
	ldr r0, [sp, #0xc]
	add r1, r0, #1
	ldr r0, [sp, #0x10]
	str r1, [sp, #0xc]
	cmp r1, r0
	blt _0223772C
_0223776E:
	add r0, r6, #0
	bl Heap_Free
	ldrb r0, [r5, #4]
	cmp r0, #2
	beq _0223777E
	cmp r0, #3
	bne _02237810
_0223777E:
	add r0, r4, #0
	bl BattleSetup_SetAllySideBattlersToPlayer
	bl sub_0203769C
	mov r1, #1
	sub r0, r1, r0
	bl sub_02034818
	mov r1, #1
	lsl r1, r1, #8
	ldr r1, [r4, r1]
	bl PlayerProfile_Copy
	add r1, r7, #1
	lsl r1, r1, #1
	add r1, r5, r1
	ldrh r1, [r1, #0x18]
	add r0, sp, #0x34
	mov r2, #0xb
	mov r3, #0xcc
	bl ov80_02229F04
	bl Heap_Free
	mov r0, #0xb
	str r0, [sp]
	ldr r2, [sp, #0x10]
	add r0, r4, #0
	add r1, sp, #0x34
	mov r3, #3
	bl ov80_0222A480
	ldr r0, [r4, #0x10]
	ldr r1, [sp, #0x10]
	bl Party_InitWithMaxSize
	mov r0, #0xb
	bl AllocMonZeroed
	add r6, r0, #0
_022377D0:
	add r0, r5, #0
	add r1, r7, #0
	bl ov80_02237820
	cmp r0, #0
	bne _022377D0
	ldr r1, [sp, #0x24]
	add r0, r5, #0
	bl ov80_022378F8
	add r2, r0, #0
	mov r0, #0x29
	lsl r0, r0, #4
	add r1, r5, r0
	ldr r0, [sp, #0x14]
	lsl r2, r2, #0x18
	add r0, r1, r0
	add r1, r6, #0
	lsr r2, r2, #0x18
	bl ov80_0222A140
	add r0, r6, #0
	bl UpdateMonAbility
	add r0, r4, #0
	add r1, r6, #0
	mov r2, #3
	bl BattleSetup_AddMonToParty
	add r0, r6, #0
	bl Heap_Free
_02237810:
	add r0, r4, #0
	add sp, #0x64
	pop {r4, r5, r6, r7, pc}
	nop
_02237818: .word 0x000006FC
_0223781C: .word 0x000006F5
	thumb_func_end ov80_022375D0
