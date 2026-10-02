	.extern ov41_022464BC
	.extern ov41_02249768
	.extern ov41_02249774
	.extern ov41_02249780
	.extern ov41_022497A0
	.include "asm/macros.inc"
	.include "overlay_41.inc"
	.include "global.inc"

.text
	thumb_func_start ov41_02249820
ov41_02249820: ; 0x02249820
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x10
	add r5, r0, #0
	add r4, r1, #0
	add r6, r2, #0
	add r7, r3, #0
	bl ov41_02249774
	cmp r0, #0
	bne _0224983A
	add sp, #0x10
	mov r0, #0
	pop {r3, r4, r5, r6, r7, pc}
_0224983A:
	add r0, r5, #0
	add r1, sp, #4
	add r2, sp, #0
	bl ov41_02249780
	add r0, r5, #0
	add r1, sp, #0xc
	add r2, sp, #8
	bl ov41_022497A0
	ldr r1, [sp, #0xc]
	ldr r2, [sp, #4]
	lsr r0, r1, #0x1f
	add r0, r1, r0
	asr r0, r0, #1
	sub r1, r2, r0
	ldr r3, [sp, #8]
	str r1, [sp, #4]
	lsr r2, r3, #0x1f
	add r2, r3, r2
	ldr r0, [sp]
	asr r2, r2, #1
	sub r0, r0, r2
	str r0, [sp]
	sub r2, r6, r0
	sub r1, r4, r1
	add r0, r7, #0
	mov r3, #0
	bl ov41_022464BC
	cmp r0, #0
	bne _02249880
	add sp, #0x10
	mov r0, #1
	pop {r3, r4, r5, r6, r7, pc}
_02249880:
	mov r0, #0
	add sp, #0x10
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
	thumb_func_end ov41_02249820
