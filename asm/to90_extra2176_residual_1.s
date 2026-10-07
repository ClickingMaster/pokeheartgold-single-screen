	.extern _0221EA54
	.include "asm/macros.inc"
	.include "overlay_05.inc"
	.include "global.inc"

.text
	thumb_func_start ov05_0221D3AC
ov05_0221D3AC: ; 0x0221D3AC
	push {r4, r5, r6, r7, lr}
	sub sp, #0xc
	ldr r4, _0221D408 ; =_0221EA54
	str r2, [sp]
	ldrb r5, [r4]
	str r3, [sp, #4]
	add r3, sp, #8
	strb r5, [r3]
	ldrb r5, [r4, #1]
	add r2, sp, #8
	mov r7, #0
	strb r5, [r3, #1]
	ldrb r5, [r4, #2]
	ldrb r4, [r4, #3]
	add r6, r0, r1
	strb r5, [r3, #2]
	strb r4, [r3, #3]
	ldrb r2, [r2, r1]
	add r4, r7, #0
	lsl r2, r2, #2
	add r5, r0, r2
_0221D3D6:
	ldr r1, _0221D40C ; =0x00000B64
	mov r0, #0x66
	ldrb r1, [r6, r1]
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	add r2, r1, r4
	ldr r1, [sp]
	add r1, r1, r2
	ldr r2, _0221D410 ; =0x00000B68
	lsl r1, r1, #0x10
	ldrb r3, [r6, r2]
	ldr r2, [sp, #4]
	asr r1, r1, #0x10
	add r2, r2, r3
	lsl r2, r2, #0x10
	asr r2, r2, #0x10
	bl ManagedSprite_SetPositionXY
	add r7, r7, #1
	add r4, #0x13
	add r5, r5, #4
	cmp r7, #3
	blo _0221D3D6
	add sp, #0xc
	pop {r4, r5, r6, r7, pc}
	.balign 4, 0
_0221D408: .word _0221EA54
_0221D40C: .word 0x00000B64
_0221D410: .word 0x00000B68
	thumb_func_end ov05_0221D3AC
