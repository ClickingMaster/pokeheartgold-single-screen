	.extern ov70_02245084
	.extern ov70_02245910
	.extern ov70_02245AAC
	.extern ov70_022465EC
#include "config.h"
#include "constants/pokemon.h"
	.include "asm/macros.inc"
	.include "overlay_70.inc"
	.include "global.inc"

.text
	thumb_func_start ov70_0223F508
ov70_0223F508: ; 0x0223F508
	push {r4, r5, r6, r7, lr}
	sub sp, #0x24
	add r4, r0, #0
	add r5, r1, #0
	mov r0, #0xb
	mov r1, #0x3d
	str r2, [sp, #8]
	str r3, [sp, #0xc]
	ldr r7, [sp, #0x38]
	bl String_New
	str r0, [sp, #0x18]
	mov r0, #0xb
	mov r1, #0x3d
	bl String_New
	str r0, [sp, #0x14]
	ldr r0, [sp, #0xc]
	ldr r2, [sp, #0x18]
	mov r1, #0x77
	bl GetBoxMonData
	ldr r0, [sp, #0xc]
	mov r1, #0x6f
	mov r2, #0
	bl GetBoxMonData
	add r6, r0, #1
	ldr r0, [sp, #0xc]
	bl CalcBoxMonLevel
	str r0, [sp, #0x10]
	add r0, r4, #0
	mov r1, #0x68
	bl NewString_ReadMsgData
	str r0, [sp, #0x20]
	mov r0, #0
	str r0, [sp]
	mov r0, #1
	mov r1, #3
	str r0, [sp, #4]
	ldr r2, [sp, #0x10]
	add r0, r5, #0
	add r3, r1, #0
	bl BufferIntegerAsString
	add r0, r5, #0
	add r1, r4, #0
	mov r2, #0x6c
	mov r3, #0x3d
	bl ReadMsgData_ExpandPlaceholders
	str r0, [sp, #0x1c]
	cmp r6, #3
	beq _0223F586
	ldr r1, _0223F624 ; =ov70_02245910
	lsl r2, r6, #2
	ldr r1, [r1, r2]
	ldr r2, [sp, #0x14]
	add r0, r4, #0
	bl ReadMsgDataIntoString
_0223F586:
	ldr r5, [sp, #8]
	mov r4, #0
_0223F58A:
	add r0, r5, #0
	mov r1, #0
	bl FillWindowPixelBuffer
	add r4, r4, #1
	add r5, #0x10
	cmp r4, #3
	blt _0223F58A
	mov r2, #0
	ldr r0, _0223F628 ; =0x000F0200
	str r2, [sp]
	str r0, [sp, #4]
	ldr r0, [sp, #8]
	ldr r1, [sp, #0x20]
	add r3, r2, #0
	bl ov70_02245084
	mov r2, #0
	ldr r0, _0223F62C ; =0x00010200
	str r2, [sp]
	str r0, [sp, #4]
	ldr r0, [sp, #8]
	ldr r1, [sp, #0x18]
	add r0, #0x10
	add r3, r2, #0
	bl ov70_02245084
	mov r0, #2
	str r0, [sp]
	ldr r0, _0223F62C ; =0x00010200
	mov r2, #0
	str r0, [sp, #4]
	ldr r0, [sp, #8]
	ldr r1, [sp, #0x1c]
	add r0, #0x20
	add r3, r2, #0
	bl ov70_02245084
	cmp r6, #3
	beq _0223F5F6
	sub r0, r6, #1
	mov r3, #0
	lsl r1, r0, #2
	ldr r0, _0223F630 ; =ov70_022465EC
	str r3, [sp]
	ldr r0, [r0, r1]
	ldr r1, [sp, #0x14]
	str r0, [sp, #4]
	ldr r0, [sp, #8]
	mov r2, #0x46
	add r0, #0x10
	str r0, [sp, #8]
	bl ov70_02245084
_0223F5F6:
	ldr r0, [sp, #0xc]
	mov r1, #5
	mov r2, #0
	bl GetBoxMonData
	strh r0, [r7]
	ldr r0, [sp, #0x10]
	strb r6, [r7, #2]
	strb r0, [r7, #3]
	ldr r0, [sp, #0x1c]
	bl String_Delete
	ldr r0, [sp, #0x14]
	bl String_Delete
	ldr r0, [sp, #0x18]
	bl String_Delete
	ldr r0, [sp, #0x20]
	bl String_Delete
	add sp, #0x24
	pop {r4, r5, r6, r7, pc}
	.balign 4, 0
_0223F624: .word ov70_02245910
_0223F628: .word 0x000F0200
_0223F62C: .word 0x00010200
_0223F630: .word ov70_022465EC
	thumb_func_end ov70_0223F508
