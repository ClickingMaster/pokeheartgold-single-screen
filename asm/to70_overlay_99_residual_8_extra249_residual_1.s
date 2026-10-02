	.extern ov99_021E7180
	.extern ov99_021E7198
	.extern ov99_021E728C
	.include "asm/macros.inc"
	.include "overlay_99.inc"
	.include "global.inc"

.text
	thumb_func_start ov99_021E72C0
ov99_021E72C0: ; 0x021E72C0
	push {r3, r4, r5, r6, r7, lr}
	add r1, sp, #0
	add r5, r0, #0
	bl ov99_021E728C
	mov r1, #0x3f
	lsl r1, r1, #4
	str r0, [r5, r1]
	ldr r0, [sp]
	mov r7, #0
	cmp r0, #0
	bls _021E7332
	add r6, r7, #0
_021E72DA:
	mov r0, #0x3f
	lsl r0, r0, #4
	ldr r0, [r5, r0]
	ldrh r4, [r0, r6]
	cmp r4, #0
	beq _021E72EC
	ldr r0, _021E7384 ; =0x000001ED
	cmp r4, r0
	bls _021E72F0
_021E72EC:
	bl GF_AssertFail
_021E72F0:
	add r0, r5, #0
	add r1, r4, #0
	bl ov99_021E7180
	cmp r0, #0
	beq _021E7328
	mov r0, #0xfd
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	lsl r0, r0, #0x12
	lsr r0, r0, #0x17
	lsl r0, r0, #1
	add r0, r5, r0
	strh r4, [r0, #0x14]
	mov r0, #0xfd
	lsl r0, r0, #2
	ldr r1, [r5, r0]
	ldr r0, _021E7388 ; =0xFFFFC01F
	and r0, r1
	lsl r1, r1, #0x12
	lsr r1, r1, #0x17
	add r1, r1, #1
	lsl r1, r1, #0x17
	lsr r1, r1, #0x12
	orr r1, r0
	mov r0, #0xfd
	lsl r0, r0, #2
	str r1, [r5, r0]
_021E7328:
	ldr r0, [sp]
	add r7, r7, #1
	add r6, r6, #2
	cmp r7, r0
	blo _021E72DA
_021E7332:
	mov r0, #0xfd
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	lsl r0, r0, #0x12
	lsr r0, r0, #0x17
	beq _021E7380
	mov r1, #0x1e
	bl _u32_div_f
	mov r2, #0xfd
	lsl r2, r2, #2
	ldr r3, [r5, r2]
	mov r1, #0x1f
	bic r3, r1
	mov r1, #0x1f
	and r0, r1
	orr r0, r3
	str r0, [r5, r2]
	ldr r0, [r5, r2]
	mov r1, #0x1e
	lsl r0, r0, #0x12
	lsr r0, r0, #0x17
	bl _u32_div_f
	cmp r1, #0
	bne _021E7380
	mov r2, #0xfd
	lsl r2, r2, #2
	ldr r3, [r5, r2]
	mov r1, #0x1f
	add r0, r3, #0
	bic r0, r1
	lsl r1, r3, #0x1b
	lsr r1, r1, #0x1b
	sub r3, r1, #1
	mov r1, #0x1f
	and r1, r3
	orr r0, r1
	str r0, [r5, r2]
_021E7380:
	pop {r3, r4, r5, r6, r7, pc}
	nop
_021E7384: .word 0x000001ED
_021E7388: .word 0xFFFFC01F
	thumb_func_end ov99_021E72C0
