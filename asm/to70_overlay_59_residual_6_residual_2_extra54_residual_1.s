	.extern ov59_02239C90
	.extern ov59_0223C6C4
	.extern ov59_0223C6C6
	.include "asm/macros.inc"
	.include "overlay_59.inc"
	.include "global.inc"
	.include "overlay59_remaining_bank.inc"

.text
	thumb_func_start ov59_0223919C
ov59_0223919C: ; 0x0223919C
	push {r4, r5, r6, r7, lr}
	sub sp, #0xc
	add r7, r0, #0
	ldr r0, [r7]
	add r5, r1, #0
	mov r1, #0x20
	add r6, r2, #0
	bl Heap_Alloc
	mov r1, #0
	mov r2, #0x20
	add r4, r0, #0
	bl MI_CpuFill8
	add r0, r7, #0
	add r0, #0x4d
	str r7, [r4]
	ldrb r0, [r0]
	cmp r5, #0
	strb r0, [r4, #7]
	bne _022391DA
	add r0, r7, #0
	add r0, #0x4d
	ldrb r0, [r0]
	add r0, r0, #2
	lsl r0, r0, #2
	add r1, r7, r0
	mov r0, #0x95
	lsl r0, r0, #2
	ldr r0, [r1, r0]
	b _022391E0
_022391DA:
	mov r0, #0x96
	lsl r0, r0, #2
	ldr r0, [r7, r0]
_022391E0:
	str r0, [r4, #0x1c]
	add r1, sp, #8
	ldr r0, [r4, #0x1c]
	add r1, #2
	add r2, sp, #8
	bl Sprite_GetPositionXY
	cmp r6, #0
	bne _02239202
	lsl r0, r5, #2
	add r0, #0xc0
	strh r0, [r4, #8]
	mov r0, #9
	mvn r0, r0
	mul r0, r5
	add r0, #0x60
	b _02239220
_02239202:
	ldrb r0, [r4, #7]
	lsl r1, r0, #2
	ldr r0, _022393C8 ; =ov59_0223C6C4
	ldrh r1, [r0, r1]
	lsl r0, r5, #2
	add r0, r1, r0
	strh r0, [r4, #8]
	ldrb r0, [r4, #7]
	lsl r1, r0, #2
	ldr r0, _022393CC ; =ov59_0223C6C6
	ldrh r1, [r0, r1]
	mov r0, #9
	mvn r0, r0
	mul r0, r5
	add r0, r1, r0
_02239220:
	strh r0, [r4, #0xa]
	mov r0, #8
	ldrsh r2, [r4, r0]
	add r0, sp, #8
	mov r1, #2
	ldrsh r1, [r0, r1]
	sub r1, r2, r1
	lsl r1, r1, #0x10
	asr r5, r1, #0x10
	mov r1, #0xa
	ldrsh r2, [r4, r1]
	mov r1, #0
	ldrsh r0, [r0, r1]
	sub r0, r2, r0
	lsl r0, r0, #0x10
	asr r6, r0, #0x10
	bpl _02239246
	neg r0, r6
	b _02239248
_02239246:
	add r0, r6, #0
_02239248:
	cmp r5, #0
	bge _02239250
	neg r1, r5
	b _02239252
_02239250:
	add r1, r5, #0
_02239252:
	cmp r1, r0
	blt _0223926C
	cmp r5, #0
	bge _0223925E
	neg r1, r5
	b _02239260
_0223925E:
	add r1, r5, #0
_02239260:
	asr r0, r1, #2
	lsr r0, r0, #0x1d
	add r0, r1, r0
	asr r0, r0, #3
	strb r0, [r4, #5]
	b _02239280
_0223926C:
	cmp r6, #0
	bge _02239274
	neg r1, r6
	b _02239276
_02239274:
	add r1, r6, #0
_02239276:
	asr r0, r1, #2
	lsr r0, r0, #0x1d
	add r0, r1, r0
	asr r0, r0, #3
	strb r0, [r4, #5]
_02239280:
	add r1, sp, #8
	mov r0, #2
	ldrsh r0, [r1, r0]
	cmp r0, #0
	ble _0223929C
	lsl r0, r0, #0xc
	bl _fflt
	add r1, r0, #0
	mov r0, #0x3f
	lsl r0, r0, #0x18
	bl _fadd
	b _022392AA
_0223929C:
	lsl r0, r0, #0xc
	bl _fflt
	mov r1, #0x3f
	lsl r1, r1, #0x18
	bl _fsub
_022392AA:
	bl _ffix
	str r0, [r4, #0x14]
	add r1, sp, #8
	mov r0, #0
	ldrsh r0, [r1, r0]
	cmp r0, #0
	ble _022392CC
	lsl r0, r0, #0xc
	bl _fflt
	add r1, r0, #0
	mov r0, #0x3f
	lsl r0, r0, #0x18
	bl _fadd
	b _022392DA
_022392CC:
	lsl r0, r0, #0xc
	bl _fflt
	mov r1, #0x3f
	lsl r1, r1, #0x18
	bl _fsub
_022392DA:
	bl _ffix
	str r0, [r4, #0x18]
	mov r0, #0
	strb r0, [r4, #6]
	ldrb r0, [r4, #5]
	cmp r0, #0
	beq _022392FE
	lsl r0, r0, #0xc
	bl _fflt
	add r1, r0, #0
	mov r0, #0x3f
	lsl r0, r0, #0x18
	bl _fadd
	str r0, [sp, #4]
	b _0223930E
_022392FE:
	lsl r0, r0, #0xc
	bl _fflt
	mov r1, #0x3f
	lsl r1, r1, #0x18
	bl _fsub
	str r0, [sp, #4]
_0223930E:
	cmp r5, #0
	ble _02239324
	lsl r0, r5, #0xc
	bl _fflt
	add r1, r0, #0
	mov r0, #0x3f
	lsl r0, r0, #0x18
	bl _fadd
	b _02239332
_02239324:
	lsl r0, r5, #0xc
	bl _fflt
	mov r1, #0x3f
	lsl r1, r1, #0x18
	bl _fsub
_02239332:
	bl _ffix
	add r5, r0, #0
	ldr r0, [sp, #4]
	bl _ffix
	add r1, r0, #0
	add r0, r5, #0
	bl FX_Div
	str r0, [r4, #0xc]
	ldrb r0, [r4, #5]
	cmp r0, #0
	beq _02239362
	lsl r0, r0, #0xc
	bl _fflt
	add r1, r0, #0
	mov r0, #0x3f
	lsl r0, r0, #0x18
	bl _fadd
	str r0, [sp]
	b _02239372
_02239362:
	lsl r0, r0, #0xc
	bl _fflt
	mov r1, #0x3f
	lsl r1, r1, #0x18
	bl _fsub
	str r0, [sp]
_02239372:
	cmp r6, #0
	ble _02239388
	lsl r0, r6, #0xc
	bl _fflt
	add r1, r0, #0
	mov r0, #0x3f
	lsl r0, r0, #0x18
	bl _fadd
	b _02239396
_02239388:
	lsl r0, r6, #0xc
	bl _fflt
	mov r1, #0x3f
	lsl r1, r1, #0x18
	bl _fsub
_02239396:
	bl _ffix
	add r5, r0, #0
	ldr r0, [sp]
	bl _ffix
	add r1, r0, #0
	add r0, r5, #0
	bl FX_Div
	str r0, [r4, #0x10]
	ldr r0, _022393D0 ; =ov59_02239C90
	add r1, r4, #0
	mov r2, #0
	bl SysTask_CreateOnMainQueue
	add r0, r7, #0
	add r0, #0x50
	ldrb r0, [r0]
	add r7, #0x50
	add r0, r0, #1
	strb r0, [r7]
	add sp, #0xc
	pop {r4, r5, r6, r7, pc}
	nop
_022393C8: .word ov59_0223C6C4
_022393CC: .word ov59_0223C6C6
_022393D0: .word ov59_02239C90
	thumb_func_end ov59_0223919C
