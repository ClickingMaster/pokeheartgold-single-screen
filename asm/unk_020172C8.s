	.include "asm/macros.inc"
	.include "unk_02016EDC.inc"
	.include "global.inc"
	.text
	.public PokepicScriptAnim_ReadDestAngleAndAmplitude
	.public PokepicScriptAnim_ReadDestVarAndOperands
	.public PokepicScriptAnim_ReadDestAndOperands
	.public PokepicScriptAnim_Compare
	.public PokepicScriptAnim_AdjustYForScale
	.public PokepicScriptAnimCmd_End
	.public PokepicScriptAnimCmd_Yield
	.public PokepicScriptAnimCmd_ResetTransform
	thumb_func_start PokepicScriptAnim_ReadDestAngleAndAmplitude
PokepicScriptAnim_ReadDestAngleAndAmplitude: ; 0x020173C8
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #8
	add r4, r2, #0
	add r2, sp, #0
	add r2, #3
	add r5, r0, #0
	add r6, r3, #0
	bl PokepicScriptAnim_ReadVarIndexPair
	add r0, sp, #0
	ldrb r0, [r0, #3]
	add r1, sp, #0
	lsl r0, r0, #2
	add r0, r5, r0
	ldr r7, [r0, #0x24]
	add r0, r5, #0
	bl PokepicScriptAnim_ReadArgByte
	add r0, sp, #0
	ldrb r0, [r0]
	cmp r0, #0x14
	bne _020173FE
	add r0, r5, #0
	add r1, r6, #0
	bl PokepicScriptAnim_ReadArg
	b _0201741E
_020173FE:
	cmp r0, #0x15
	bne _0201741A
	add r1, sp, #0
	add r0, r5, #0
	add r1, #2
	bl PokepicScriptAnim_ReadVarIndex
	add r0, sp, #0
	ldrb r0, [r0, #2]
	lsl r0, r0, #2
	add r0, r5, r0
	ldr r0, [r0, #0x24]
	str r0, [r6]
	b _0201741E
_0201741A:
	bl GF_AssertFail
_0201741E:
	add r0, r5, #0
	add r1, sp, #0
	bl PokepicScriptAnim_ReadArgByte
	add r0, sp, #0
	ldrb r0, [r0]
	cmp r0, #0x14
	bne _02017438
	add r0, r5, #0
	add r1, sp, #4
	bl PokepicScriptAnim_ReadArg
	b _02017458
_02017438:
	cmp r0, #0x15
	bne _02017454
	add r1, sp, #0
	add r0, r5, #0
	add r1, #1
	bl PokepicScriptAnim_ReadVarIndex
	add r0, sp, #0
	ldrb r0, [r0, #1]
	lsl r0, r0, #2
	add r0, r5, r0
	ldr r0, [r0, #0x24]
	str r0, [sp, #4]
	b _02017458
_02017454:
	bl GF_AssertFail
_02017458:
	ldr r0, [sp, #4]
	add r0, r7, r0
	lsr r2, r0, #0x1f
	lsl r1, r0, #0x10
	sub r1, r1, r2
	mov r0, #0x10
	ror r1, r0
	add r0, r2, r1
	str r0, [r4]
	add sp, #8
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
	thumb_func_end PokepicScriptAnim_ReadDestAngleAndAmplitude
