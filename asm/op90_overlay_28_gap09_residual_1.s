	.extern ov28_0225E374
	.include "asm/macros.inc"
	.include "overlay_28.inc"
	.include "global.inc"

.text
thumb_func_start ov28_0225E730
ov28_0225E730: ; 0x0225E730
	push {r3, r4, r5, r6, r7, lr}
	add r1, sp, #0
	add r1, #2
	add r2, sp, #0
	add r5, r0, #0
	bl ov28_0225E374
	add r0, sp, #0
	ldrh r1, [r0, #2]
	ldrh r0, [r0]
	mov r2, #0x21
	lsl r2, r2, #4
	lsl r0, r0, #1
	add r0, r1, r0
	lsl r1, r0, #6
	mov r0, #0xf
	lsl r0, r0, #6
	sub r0, r1, r0
	lsl r0, r0, #0x10
	asr r4, r0, #0x10
	ldr r0, [r5, r2]
	cmp r0, #4
	bne _0225E798
	add r1, r2, #0
	add r1, #8
	ldr r1, [r5, r1]
	add r0, r2, #4
	lsr r1, r1, #3
	ldr r0, [r5, r0]
	sub r2, #8
	sub r6, r1, #5
	ldrsh r1, [r5, r2]
	lsr r0, r0, #3
	sub r0, r0, #3
	add r1, r1, #7
	sub r0, r1, r0
	bl abs
	add r7, r0, #0
	ldr r0, _0225E7C8 ; =0x0000020A
	ldrsh r0, [r5, r0]
	add r0, r0, #7
	sub r0, r0, r6
	bl abs
	add r1, r7, r0
	mov r0, #0x10
	sub r0, r0, r1
	lsl r0, r0, #6
	add r0, r4, r0
	lsl r0, r0, #0x10
	asr r4, r0, #0x10
_0225E798:
	mov r0, #0xce
	lsl r0, r0, #2
	ldrh r0, [r5, r0]
	cmp r0, #0
	bne _0225E7AA
	ldr r0, _0225E7CC ; =0x0000093D
	add r1, r4, #0
	bl PlaySE_SetPitch
_0225E7AA:
	mov r0, #0xce
	lsl r0, r0, #2
	ldrh r1, [r5, r0]
	cmp r1, #0xa
	bne _0225E7B8
	mov r1, #0
	b _0225E7BA
_0225E7B8:
	add r1, r1, #1
_0225E7BA:
	strh r1, [r5, r0]
	ldr r0, _0225E7D0 ; =0x0000033A
	ldrsh r1, [r5, r0]
	cmp r1, r4
	beq _0225E7C6
	strh r4, [r5, r0]
_0225E7C6:
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
_0225E7C8: .word 0x0000020A
_0225E7CC: .word 0x0000093D
_0225E7D0: .word 0x0000033A
	thumb_func_end ov28_0225E730
