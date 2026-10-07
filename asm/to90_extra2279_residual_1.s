	.extern ov08_02225CE0
#include "constants/moves.h"
	.include "asm/macros.inc"
	.include "overlay_08.inc"
	.include "global.inc"

.text
	.force_active on
	thumb_func_start ov08_02223BF4
ov08_02223BF4: ; 0x02223BF4
	push {r4, r5, r6, r7, lr}
	sub sp, #0xc
	add r4, r0, #0
	mov r0, #0
	str r0, [sp, #4]
_02223BFE:
	mov r0, #0
	str r0, [sp]
	ldr r0, [sp, #4]
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	str r0, [sp, #8]
_02223C0A:
	ldr r0, [r4]
	ldr r2, [sp]
	ldr r0, [r0, #8]
	lsl r2, r2, #0x10
	ldr r1, [sp, #8]
	lsr r2, r2, #0x10
	bl Bag_GetPocketSlotN
	add r5, r0, #0
	beq _02223C7C
	ldrh r0, [r5]
	cmp r0, #0
	beq _02223C74
	ldrh r1, [r5, #2]
	cmp r1, #0
	beq _02223C74
	ldr r2, [r4]
	mov r1, #0xd
	ldr r2, [r2, #0xc]
	bl GetItemAttr
	add r7, r0, #0
	ldr r1, _02223CC8 ; =ov08_02225CE0
	mov r0, #0
_02223C3A:
	mov r2, #1
	lsl r2, r0
	tst r2, r7
	beq _02223C6C
	ldrb r2, [r1]
	mov r3, #0x90
	add r6, r2, #0
	mul r6, r3
	add r3, r4, r2
	ldr r2, _02223CCC ; =0x0000114F
	ldrb r2, [r3, r2]
	add r3, r4, r6
	lsl r2, r2, #2
	add r2, r2, r3
	ldrh r3, [r5]
	strh r3, [r2, #0x3c]
	ldrh r3, [r5, #2]
	strh r3, [r2, #0x3e]
	ldrb r2, [r1]
	add r6, r4, r2
	ldr r2, _02223CCC ; =0x0000114F
	ldrb r2, [r6, r2]
	add r3, r2, #1
	ldr r2, _02223CCC ; =0x0000114F
	strb r3, [r6, r2]
_02223C6C:
	add r0, r0, #1
	add r1, r1, #1
	cmp r0, #5
	blo _02223C3A
_02223C74:
	ldr r0, [sp]
	add r0, r0, #1
	str r0, [sp]
	b _02223C0A
_02223C7C:
	ldr r0, [sp, #4]
	add r0, r0, #1
	str r0, [sp, #4]
	cmp r0, #8
	blo _02223BFE
	mov r5, #0
	add r7, r5, #0
_02223C8A:
	ldr r0, _02223CCC ; =0x0000114F
	add r6, r4, r5
	ldrb r0, [r6, r0]
	cmp r0, #0
	bne _02223C9A
	ldr r0, _02223CD0 ; =0x00001154
	strb r7, [r6, r0]
	b _02223CA6
_02223C9A:
	sub r0, r0, #1
	mov r1, #6
	bl _s32_div_f
	ldr r1, _02223CD0 ; =0x00001154
	strb r0, [r6, r1]
_02223CA6:
	ldr r1, [r4]
	ldr r0, _02223CD0 ; =0x00001154
	add r2, r1, r5
	add r2, #0x2c
	ldrb r0, [r6, r0]
	ldrb r2, [r2]
	cmp r0, r2
	bhs _02223CBC
	add r1, r1, r5
	add r1, #0x2c
	strb r0, [r1]
_02223CBC:
	add r5, r5, #1
	cmp r5, #5
	blo _02223C8A
	add sp, #0xc
	pop {r4, r5, r6, r7, pc}
	nop
_02223CC8: .word ov08_02225CE0
_02223CCC: .word 0x0000114F
_02223CD0: .word 0x00001154
	thumb_func_end ov08_02223BF4
