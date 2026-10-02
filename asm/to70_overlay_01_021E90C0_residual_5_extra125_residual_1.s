	.extern ov01_021E9610
#include "constants/sndseq.h"
#include "constants/moves.h"
#include "constants/std_script.h"
#include "constants/species.h"
#include "constants/sprites.h"
#include "constants/maps.h"
#include "constants/mmodel.h"
#include "constants/items.h"
#include "msgdata/msg/msg_0096_D31R0201.h"
#include "msgdata/msg/msg_0066_D23R0102.h"
	.include "asm/macros.inc"
	.include "overlay_01_021E90C0.inc"
	.include "global.inc"

.text
	thumb_func_start ov01_021E9FF8
ov01_021E9FF8: ; 0x021E9FF8
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x18
	str r0, [sp, #0x10]
	bl TaskManager_GetFieldSystem
	add r5, r0, #0
	ldr r0, [sp, #0x10]
	bl TaskManager_GetEnvironment
	add r4, r0, #0
	ldrb r0, [r4]
	cmp r0, #3
	bhi _021EA110
	add r0, r0, r0
	add r0, pc
	ldrh r0, [r0, #6]
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	add pc, r0
_021EA01E: ; jump table
	.short _021EA026 - _021EA01E - 2 ; case 0
	.short _021EA0B4 - _021EA01E - 2 ; case 1
	.short _021EA0D2 - _021EA01E - 2 ; case 2
	.short _021EA0F0 - _021EA01E - 2 ; case 3
_021EA026:
	ldr r0, [r5, #0x40]
	bl PlayerAvatar_GetMapObject
	str r0, [sp, #0x14]
	ldr r0, [r5, #0x40]
	bl PlayerAvatar_GetFacingDirection
	lsl r0, r0, #0x18
	lsr r7, r0, #0x18
	cmp r7, #1
	bne _021EA048
	ldr r0, [sp, #0x14]
	mov r1, #1
	bl MapObject_SetVisible
	mov r0, #1
	b _021EA052
_021EA048:
	ldr r0, [sp, #0x14]
	mov r1, #0
	bl MapObject_SetVisible
	mov r0, #3
_021EA052:
	strb r0, [r4]
	mov r0, #0
	str r0, [r4, #4]
	strb r0, [r4, #1]
	ldr r0, [r5, #0x24]
	bl Camera_GetPerspectiveAngle
	strh r0, [r4, #2]
	ldr r0, _021EA124 ; =0x0000FFA0
	ldr r1, [r5, #0x24]
	bl Camera_AdjustPerspectiveAngle
	cmp r7, #3
	bhi _021EA092
	add r0, r7, r7
	add r0, pc
	ldrh r0, [r0, #6]
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	add pc, r0
_021EA07A: ; jump table
	.short _021EA082 - _021EA07A - 2 ; case 0
	.short _021EA086 - _021EA07A - 2 ; case 1
	.short _021EA08A - _021EA07A - 2 ; case 2
	.short _021EA08E - _021EA07A - 2 ; case 3
_021EA082:
	mov r6, #3
	b _021EA096
_021EA086:
	mov r6, #5
	b _021EA096
_021EA08A:
	mov r6, #7
	b _021EA096
_021EA08E:
	mov r6, #0x27
	b _021EA096
_021EA092:
	bl GF_AssertFail
_021EA096:
	mov r1, #0
	str r1, [sp]
	mov r0, #6
	str r0, [sp, #4]
	mov r3, #1
	str r3, [sp, #8]
	mov r0, #0xb
	str r0, [sp, #0xc]
	ldr r0, [sp, #0x10]
	add r2, r6, #0
	bl NewFieldFadeEnvironment
	mov r0, #1
	str r0, [r4, #4]
	b _021EA110
_021EA0B4:
	ldr r0, [r5, #0x40]
	bl PlayerAvatar_GetMapObject
	add r6, r0, #0
	mov r1, #0
	bl MapObject_SetVisible
	add r0, r6, #0
	mov r1, #0xd
	bl MapObject_SetHeldMovement
	ldrb r0, [r4]
	add r0, r0, #1
	strb r0, [r4]
	b _021EA110
_021EA0D2:
	ldr r0, [r5, #0x40]
	bl PlayerAvatar_GetMapObject
	add r6, r0, #0
	bl MapObject_IsMovementPaused
	cmp r0, #1
	bne _021EA110
	add r0, r6, #0
	bl MapObject_ClearHeldMovementIfActive
	ldrb r0, [r4]
	add r0, r0, #1
	strb r0, [r4]
	b _021EA110
_021EA0F0:
	bl IsPaletteFadeFinished
	cmp r0, #0
	beq _021EA110
	ldr r0, [r5, #0x24]
	bl Camera_GetPerspectiveAngle
	ldrh r1, [r4, #2]
	cmp r1, r0
	bne _021EA110
	add r0, r4, #0
	bl Heap_Free
	add sp, #0x18
	mov r0, #1
	pop {r3, r4, r5, r6, r7, pc}
_021EA110:
	ldr r0, [r4, #4]
	cmp r0, #0
	beq _021EA11E
	ldr r0, [r5, #0x24]
	add r1, r4, #1
	bl ov01_021E9610
_021EA11E:
	mov r0, #0
	add sp, #0x18
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
_021EA124: .word 0x0000FFA0
	thumb_func_end ov01_021E9FF8
