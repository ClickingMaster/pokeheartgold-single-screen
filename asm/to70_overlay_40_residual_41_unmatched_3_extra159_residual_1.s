	.include "asm/macros.inc"
	.include "overlay_40.inc"
	.include "global.inc"

.text
	thumb_func_start ov40_022440A0
ov40_022440A0: ; 0x022440A0
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x10
	str r1, [sp, #4]
	mov r1, #0x82
	lsl r1, r1, #2
	str r0, [sp]
	ldr r0, [r0, r1]
	mov r4, #0
	lsl r2, r0, #1
	ldr r0, [sp]
	str r4, [sp, #8]
	add r2, r0, r2
	add r0, r1, #0
	sub r0, #0x2c
	ldrsh r5, [r2, r0]
	ldr r0, [sp]
	sub r1, r1, #4
	ldr r0, [r0, r1]
	cmp r0, #0
	bgt _022440CA
	b _022441F2
_022440CA:
	ldr r7, [sp]
	add r6, r7, #0
_022440CE:
	mov r0, #0x21
	ldr r1, [sp]
	lsl r0, r0, #4
	ldr r0, [r1, r0]
	cmp r4, r0
	blt _022440FA
	add r2, r1, #0
	mov r1, #0x85
	lsl r1, r1, #2
	ldr r1, [r2, r1]
	cmp r4, r1
	bge _022440FA
	cmp r4, r0
	bne _022440F2
	add r5, #0x14
	lsl r0, r5, #0x10
	asr r5, r0, #0x10
	b _0224410C
_022440F2:
	add r5, #0x20
	lsl r0, r5, #0x10
	asr r5, r0, #0x10
	b _0224410C
_022440FA:
	cmp r4, #0
	bne _02244106
	add r5, #0x14
	lsl r0, r5, #0x10
	asr r5, r0, #0x10
	b _0224410C
_02244106:
	add r5, #8
	lsl r0, r5, #0x10
	asr r5, r0, #0x10
_0224410C:
	add r1, sp, #0xc
	ldr r0, [r7, #0xc]
	add r1, #2
	add r2, sp, #0xc
	bl ManagedSprite_GetPositionXY
	ldr r0, [sp, #4]
	cmp r0, #0
	bne _0224412E
	add r3, sp, #0xc
	mov r2, #0
	ldrsh r2, [r3, r2]
	ldr r0, [r7, #0xc]
	add r1, r5, #0
	bl ManagedSprite_SetPositionXY
	b _0224414A
_0224412E:
	add r1, sp, #0xc
	mov r0, #2
	ldrsh r0, [r1, r0]
	sub r1, r5, r0
	lsr r0, r1, #0x1f
	add r0, r1, r0
	asr r0, r0, #1
	strh r0, [r7, #0x14]
	mov r0, #0
	strh r0, [r7, #0x16]
	mov r0, #2
	strb r0, [r7, #0x18]
	mov r0, #0
	strb r0, [r7, #0x19]
_0224414A:
	mov r0, #0x15
	lsl r0, r0, #4
	ldr r0, [r6, r0]
	cmp r4, r0
	bne _022441E0
	ldr r0, [sp, #8]
	cmp r0, #2
	beq _022441E0
	mov r0, #0x57
	lsl r0, r0, #2
	add r1, sp, #0xc
	ldr r0, [r6, r0]
	add r1, #2
	add r2, sp, #0xc
	bl ManagedSprite_GetPositionXY
	ldr r1, [sp]
	mov r0, #0x85
	lsl r0, r0, #2
	ldr r0, [r1, r0]
	add r2, r1, #0
	mov r1, #0x21
	lsl r1, r1, #4
	ldr r1, [r2, r1]
	cmp r1, r0
	bne _02244186
	add r5, #8
	lsl r0, r5, #0x10
	asr r5, r0, #0x10
	b _0224419C
_02244186:
	cmp r4, r1
	ble _02244196
	cmp r4, r0
	bge _02244196
	add r5, #0x14
	lsl r0, r5, #0x10
	asr r5, r0, #0x10
	b _0224419C
_02244196:
	add r5, #8
	lsl r0, r5, #0x10
	asr r5, r0, #0x10
_0224419C:
	ldr r0, [sp, #4]
	cmp r0, #0
	bne _022441B6
	mov r0, #0x57
	lsl r0, r0, #2
	add r3, sp, #0xc
	mov r2, #0
	ldrsh r2, [r3, r2]
	ldr r0, [r6, r0]
	add r1, r5, #0
	bl ManagedSprite_SetPositionXY
	b _022441D8
_022441B6:
	add r1, sp, #0xc
	mov r0, #2
	ldrsh r0, [r1, r0]
	sub r1, r5, r0
	lsr r0, r1, #0x1f
	add r0, r1, r0
	asr r1, r0, #1
	mov r0, #0x59
	lsl r0, r0, #2
	strh r1, [r6, r0]
	mov r1, #0
	add r0, r0, #2
	strh r1, [r6, r0]
	mov r0, #0x5a
	mov r1, #2
	lsl r0, r0, #2
	strb r1, [r6, r0]
_022441D8:
	ldr r0, [sp, #8]
	add r6, #0x1c
	add r0, r0, #1
	str r0, [sp, #8]
_022441E0:
	mov r0, #0x81
	ldr r1, [sp]
	lsl r0, r0, #2
	ldr r0, [r1, r0]
	add r4, r4, #1
	add r7, #0x1c
	cmp r4, r0
	bge _022441F2
	b _022440CE
_022441F2:
	add sp, #0x10
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
	thumb_func_end ov40_022440A0
