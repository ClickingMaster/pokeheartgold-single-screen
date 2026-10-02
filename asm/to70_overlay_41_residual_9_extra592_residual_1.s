	.include "asm/macros.inc"
	.include "overlay_41.inc"
	.include "global.inc"

.text
	thumb_func_start ov41_0224642C
ov41_0224642C: ; 0x0224642C
	push {r3, r4}
	mov r0, #1
	lsl r0, r0, #0x1a
	ldr r2, [r0]
	ldr r1, _02246488 ; =0xFFFF1FFF
	add r3, r0, #0
	and r2, r1
	lsr r1, r0, #0xd
	orr r1, r2
	str r1, [r0]
	add r3, #0x48
	ldrh r4, [r3]
	mov r2, #0x3f
	mov r1, #0x1f
	bic r4, r2
	orr r1, r4
	strh r1, [r3]
	add r3, r0, #0
	add r3, #0x4a
	ldrh r4, [r3]
	mov r1, #0x12
	bic r4, r2
	orr r1, r4
	strh r1, [r3]
	add r1, r0, #0
	ldr r2, _0224648C ; =0x00000AF6
	add r1, #0x40
	strh r2, [r1]
	add r1, r0, #0
	ldr r2, _02246490 ; =0x0000128F
	add r1, #0x44
	strh r2, [r1]
	add r2, r0, #0
	add r2, #8
	ldrh r3, [r2]
	mov r1, #3
	add r0, #0xa
	bic r3, r1
	strh r3, [r2]
	ldrh r2, [r0]
	bic r2, r1
	mov r1, #1
	orr r1, r2
	strh r1, [r0]
	pop {r3, r4}
	bx lr
	.balign 4, 0
_02246488: .word 0xFFFF1FFF
_0224648C: .word 0x00000AF6
_02246490: .word 0x0000128F
	thumb_func_end ov41_0224642C
