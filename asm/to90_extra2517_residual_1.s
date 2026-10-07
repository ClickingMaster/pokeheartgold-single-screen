	.extern ov08_0221DBCC
	.extern ov08_0221F550
	.extern ov08_0221F5B0
	.extern ov08_02220064
	.extern ov08_022201C0
	.extern ov08_02220224
	.extern ov08_022225A4
#include "constants/moves.h"
	.include "asm/macros.inc"
	.include "overlay_08.inc"
	.include "global.inc"

.text
	.force_active on
	thumb_func_start ov08_0221CC38
ov08_0221CC38: ; 0x0221CC38
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #8
	add r5, r0, #0
	ldr r0, [r5]
	str r0, [sp, #4]
	ldr r0, _0221CD58 ; =0x0000207C
	ldrb r0, [r5, r0]
	cmp r0, #0
	beq _0221CC54
	cmp r0, #1
	beq _0221CCBE
	cmp r0, #2
	beq _0221CD28
	b _0221CD50
_0221CC54:
	ldr r2, [sp, #4]
	ldr r0, [sp, #4]
	ldrb r3, [r2, #0x11]
	ldr r1, [sp, #4]
	ldr r0, [r0, #8]
	add r2, r2, r3
	add r2, #0x2c
	ldrb r2, [r2]
	ldr r1, [r1, #0x28]
	bl BattleSystem_GetPartyMon
	ldr r1, [sp, #4]
	mov r7, #0
	ldrb r2, [r1, #0x11]
	mov r1, #0x50
	add r6, r7, #0
	mul r1, r2
	add r1, r5, r1
	str r0, [r1, #4]
	add r4, r5, #0
_0221CC7C:
	ldr r0, [sp, #4]
	ldrb r1, [r0, #0x11]
	mov r0, #0x50
	mul r0, r1
	add r1, r5, r0
	add r0, r1, r6
	ldrh r0, [r0, #0x34]
	cmp r0, #0
	beq _0221CCA0
	ldr r0, [r1, #4]
	add r1, r7, #0
	add r1, #0x3a
	mov r2, #0
	bl GetMonData
	mov r1, #0x82
	lsl r1, r1, #6
	strh r0, [r4, r1]
_0221CCA0:
	add r7, r7, #1
	add r6, #8
	add r4, r4, #2
	cmp r7, #4
	blo _0221CC7C
	add r0, r5, #0
	bl ov08_02220224
	ldr r0, _0221CD5C ; =0x000005EC
	bl PlaySE
	ldr r0, _0221CD58 ; =0x0000207C
	mov r1, #1
	strb r1, [r5, r0]
	b _0221CD50
_0221CCBE:
	mov r6, #0
	add r4, r6, #0
	str r6, [sp]
	add r7, r5, #0
_0221CCC6:
	ldr r0, [sp, #4]
	ldrb r1, [r0, #0x11]
	mov r0, #0x50
	mul r0, r1
	add r1, r5, r0
	ldr r0, [sp]
	add r1, r1, r0
	ldrh r0, [r1, #0x34]
	cmp r0, #0
	bne _0221CCDE
	add r6, r6, #1
	b _0221CD0E
_0221CCDE:
	mov r0, #0x82
	add r2, r1, #0
	lsl r0, r0, #6
	add r2, #0x36
	ldrh r0, [r7, r0]
	ldrb r2, [r2]
	cmp r0, r2
	beq _0221CD0C
	add r0, r1, #0
	add r0, #0x36
	ldrb r0, [r0]
	lsl r2, r4, #0x10
	add r1, #0x36
	add r0, r0, #1
	strb r0, [r1]
	add r1, r4, #1
	lsl r1, r1, #0x10
	add r0, r5, #0
	lsr r1, r1, #0x10
	lsr r2, r2, #0x10
	bl ov08_02220064
	b _0221CD0E
_0221CD0C:
	add r6, r6, #1
_0221CD0E:
	ldr r0, [sp]
	add r4, r4, #1
	add r0, #8
	add r7, r7, #2
	str r0, [sp]
	cmp r4, #4
	blo _0221CCC6
	cmp r6, #4
	bne _0221CD50
	ldr r0, _0221CD58 ; =0x0000207C
	mov r1, #2
	strb r1, [r5, r0]
	b _0221CD50
_0221CD28:
	ldr r2, [sp, #4]
	ldr r1, [sp, #4]
	ldr r0, [sp, #4]
	add r2, #0x33
	ldr r3, [sp, #4]
	ldrh r1, [r1, #0x22]
	ldrb r2, [r2]
	ldr r0, [r0, #8]
	ldr r3, [r3, #0xc]
	bl ov08_0221DBCC
	add r0, r5, #0
	bl ov08_022201C0
	ldr r0, _0221CD60 ; =0x00002079
	mov r1, #0x19
	strb r1, [r5, r0]
	add sp, #8
	mov r0, #0x11
	pop {r3, r4, r5, r6, r7, pc}
_0221CD50:
	mov r0, #0x18
	add sp, #8
	pop {r3, r4, r5, r6, r7, pc}
	nop
_0221CD58: .word 0x0000207C
_0221CD5C: .word 0x000005EC
_0221CD60: .word 0x00002079
	thumb_func_end ov08_0221CC38
