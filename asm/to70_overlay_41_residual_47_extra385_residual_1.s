	.extern ov41_022497A0
	.extern ov41_0224989C
	.extern ov41_022498E8
	.extern ov41_02249978
	.include "asm/macros.inc"
	.include "overlay_41.inc"
	.include "global.inc"

.text
	thumb_func_start ov41_02249604
ov41_02249604: ; 0x02249604
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x20
	add r6, r2, #0
	add r5, r0, #0
	add r7, r1, #0
	add r4, r3, #0
	add r0, r6, #0
	mov r1, #5
	mov r2, #0
	bl GetMonData
	add r0, r4, #0
	add r1, r6, #0
	mov r2, #2
	bl sub_02070130
	mov r0, #0
	str r0, [sp]
	str r0, [sp, #4]
	str r0, [sp, #8]
	str r0, [sp, #0xc]
	add r0, r7, #0
	add r1, r4, #0
	mov r2, #0xc0
	mov r3, #0x38
	bl PokepicManager_CreatePokepic
	str r0, [r5]
	add r0, r5, #0
	add r1, sp, #0x1c
	add r2, sp, #0x18
	bl ov41_022497A0
	ldr r1, [sp, #0x1c]
	mov r2, #0x38
	lsr r0, r1, #0x1f
	add r0, r1, r0
	asr r0, r0, #1
	ldr r1, [sp, #0x18]
	str r0, [sp, #0x1c]
	lsr r0, r1, #0x1f
	add r0, r1, r0
	asr r0, r0, #1
	str r0, [sp, #0x18]
	sub r0, r2, r0
	strb r0, [r5, #4]
	ldr r0, [sp, #0x18]
	mov r1, #0xc0
	add r0, #0x38
	strb r0, [r5, #5]
	ldr r0, [sp, #0x1c]
	sub r0, r1, r0
	strb r0, [r5, #6]
	ldr r0, [sp, #0x1c]
	add r0, #0xc0
	strb r0, [r5, #7]
	ldr r0, [sp, #0x18]
	str r0, [sp]
	ldr r3, [sp, #0x1c]
	add r0, r5, #4
	bl ov41_02249978
	add r0, r6, #0
	mov r1, #2
	bl sub_02070848
	mov r2, #0
	str r2, [sp]
	add r7, r0, #0
	ldrh r0, [r4]
	ldrh r1, [r4, #2]
	ldr r3, [sp, #0x38]
	bl GfGfxLoader_LoadFromNarc
	add r1, sp, #0x14
	str r0, [sp, #0x10]
	bl NNS_G2dGetUnpackedCharacterData
	ldr r0, [sp, #0x14]
	ldrh r1, [r4]
	ldr r0, [r0, #0x14]
	bl UnscanPokepic
	ldr r0, [sp, #0x3c]
	cmp r0, #0
	bne _022496C6
	ldr r2, [sp, #0x14]
	add r3, r5, #0
	ldrh r1, [r2, #2]
	ldr r0, [r2, #0x14]
	ldrh r2, [r2]
	lsl r1, r1, #3
	add r3, #8
	lsl r2, r2, #3
	bl ov41_022498E8
	b _022496DC
_022496C6:
	ldr r1, [sp, #0x14]
	ldr r0, [r1, #0x14]
	ldrh r1, [r1, #2]
	lsl r1, r1, #3
	bl ov41_0224989C
	strb r0, [r5, #8]
	ldrb r0, [r5, #8]
	strb r0, [r5, #9]
	strb r7, [r5, #0xb]
	strb r7, [r5, #0xa]
_022496DC:
	ldr r0, [sp, #0x10]
	bl Heap_Free
	str r6, [r5, #0xc]
	add sp, #0x20
	pop {r3, r4, r5, r6, r7, pc}
	thumb_func_end ov41_02249604
