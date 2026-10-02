	.extern ov02_0224C660
	.extern ov02_02250780
	.extern ov02_022507B4
	.extern ov02_02253724
	.extern ov02_0225373C
#include "constants/abilities.h"
#include "constants/species.h"
#include "constants/sndseq.h"
#include "constants/items.h"
#include "constants/pokemon.h"
#include "constants/std_script.h"
	.include "asm/macros.inc"
	.include "overlay_02.inc"
	.include "global.inc"

.text
	thumb_func_start Task_FieldTeleport
Task_FieldTeleport: ; 0x0224C558
	push {r3, r4, r5, r6, r7, lr}
	add r7, r0, #0
	bl TaskManager_GetFieldSystem
	add r6, r0, #0
	add r0, r7, #0
	bl TaskManager_GetEnvironment
	add r4, r0, #0
	add r0, r7, #0
	bl TaskManager_GetStatePtr
	add r5, r0, #0
	ldr r0, [r5]
	cmp r0, #3
	bhi _0224C64E
	add r1, r0, r0
	add r1, pc
	ldrh r1, [r1, #6]
	lsl r1, r1, #0x10
	asr r1, r1, #0x10
	add pc, r1
_0224C584: ; jump table
	.short _0224C58C - _0224C584 - 2 ; case 0
	.short _0224C5A4 - _0224C584 - 2 ; case 1
	.short _0224C5D6 - _0224C584 - 2 ; case 2
	.short _0224C608 - _0224C584 - 2 ; case 3
_0224C58C:
	ldr r0, [r4, #8]
	cmp r0, #0
	beq _0224C59C
	ldr r1, _0224C654 ; =ov01_02205A60
	add r0, r7, #0
	mov r2, #0
	bl TaskManager_Call
_0224C59C:
	ldr r0, [r5]
	add r0, r0, #1
	str r0, [r5]
	b _0224C64E
_0224C5A4:
	add r0, r0, #1
	str r0, [r5]
	ldr r0, [r4, #8]
	cmp r0, #0
	beq _0224C5D6
	add r0, r6, #0
	mov r1, #0xe
	bl ov02_02250780
	cmp r0, #0
	beq _0224C5CA
	mov r0, #0x42
	lsl r0, r0, #2
	ldr r0, [r6, r0]
	mov r1, #1
	mov r4, #2
	bl FieldSystem_UnkSub108_AddMonMood
	b _0224C5CC
_0224C5CA:
	mov r4, #1
_0224C5CC:
	add r0, r6, #0
	add r1, r4, #0
	bl ov02_022507B4
	b _0224C64E
_0224C5D6:
	ldr r0, [r4, #8]
	cmp r0, #0
	beq _0224C602
	ldr r0, [r4, #0x28]
	mov r1, #5
	mov r2, #0
	bl GetMonData
	str r0, [sp]
	ldr r0, [r4, #0x28]
	mov r1, #0x70
	mov r2, #0
	bl GetMonData
	add r1, r0, #0
	ldr r0, [sp]
	lsl r1, r1, #0x18
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	lsr r1, r1, #0x18
	bl PlayCry
_0224C602:
	ldr r0, [r5]
	add r0, r0, #1
	str r0, [r5]
_0224C608:
	ldr r0, [r4, #8]
	cmp r0, #0
	beq _0224C62E
	bl IsCryFinished
	cmp r0, #0
	beq _0224C61A
	mov r5, #0
	b _0224C640
_0224C61A:
	ldr r3, [r4]
	add r0, r7, #0
	lsl r5, r3, #2
	ldr r3, _0224C658 ; =ov02_0225373C
	add r1, r6, #0
	ldr r3, [r3, r5]
	add r2, r4, #0
	blx r3
	add r5, r0, #0
	b _0224C640
_0224C62E:
	ldr r3, [r4]
	add r0, r7, #0
	lsl r5, r3, #2
	ldr r3, _0224C65C ; =ov02_02253724
	add r1, r6, #0
	ldr r3, [r3, r5]
	add r2, r4, #0
	blx r3
	add r5, r0, #0
_0224C640:
	cmp r5, #2
	bne _0224C64A
	add r0, r4, #0
	bl Heap_Free
_0224C64A:
	cmp r5, #1
	beq _0224C608
_0224C64E:
	mov r0, #0
	pop {r3, r4, r5, r6, r7, pc}
	nop
_0224C654: .word ov01_02205A60
_0224C658: .word ov02_0225373C
_0224C65C: .word ov02_02253724
	thumb_func_end Task_FieldTeleport
