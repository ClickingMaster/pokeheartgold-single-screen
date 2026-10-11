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
	.public RockClimb_New
	.public RockClimb_Free
	.public RockClimb_Retain
	.public RockClimb_Release
	.public RockClimb_LoadGfx
	.public RockClimb_FreeGfx
	.public RockClimb_LoadGfxIfUnused
	.public RockClimb_FreeGfxIfUnused
	.public RockClimbEffect_Create
	.public RockClimbEffect_SetFollow
	.public RockClimbEffect_Init
	.public RockClimbEffect_Destroy
	.public RockClimbEffect_Update
	.public RockClimbEffect_Draw
	.public sRockClimbScale
	.public sRockClimbEffectTemplate
	.include "overlay_01_021FEA0C.inc"
	.include "global.inc"

	.text

	thumb_func_start RockClimb_New
RockClimb_New: ; 0x021FEA0C
	push {r4, lr}
	mov r2, #0
	mov r1, #0x7c
	add r3, r2, #0
	add r4, r0, #0
	bl FieldEffectManager_HeapAllocAndFill
	str r4, [r0, #0x10]
	pop {r4, pc}
	.balign 4, 0
	thumb_func_end RockClimb_New

	thumb_func_start RockClimb_Free
RockClimb_Free: ; 0x021FEA20
	push {r4, lr}
	add r4, r0, #0
	bl RockClimb_FreeGfx
	add r0, r4, #0
	bl FieldEffectManager_HeapFree
	pop {r4, pc}
	thumb_func_end RockClimb_Free

	thumb_func_start RockClimb_Retain
RockClimb_Retain: ; 0x021FEA30
	ldr r1, [r0]
	add r1, r1, #1
	str r1, [r0]
	bx lr
	thumb_func_end RockClimb_Retain

	thumb_func_start RockClimb_Release
RockClimb_Release: ; 0x021FEA38
	push {r3, lr}
	ldr r1, [r0]
	sub r1, r1, #1
	str r1, [r0]
	bpl _021FEA46
	bl GF_AssertFail
_021FEA46:
	pop {r3, pc}
	thumb_func_end RockClimb_Release

	thumb_func_start RockClimb_LoadGfx
RockClimb_LoadGfx: ; 0x021FEA48
	push {r3, r4, lr}
	sub sp, #4
	add r4, r0, #0
	ldr r0, [r4, #4]
	cmp r0, #0
	bne _021FEA76
	mov r0, #1
	str r0, [sp]
	add r1, r4, #0
	ldr r0, [r4, #0x10]
	add r1, #0x14
	mov r2, #0
	mov r3, #0x57
	bl FieldEffectManager_LoadRenderModel
	add r0, r4, #0
	add r1, r4, #0
	add r0, #0x28
	add r1, #0x14
	bl RenderObj_InitFromModel
	mov r0, #1
	str r0, [r4, #4]
_021FEA76:
	add sp, #4
	pop {r3, r4, pc}
	.balign 4, 0
	thumb_func_end RockClimb_LoadGfx

	thumb_func_start RockClimb_FreeGfx
RockClimb_FreeGfx: ; 0x021FEA7C
	push {r3, lr}
	ldr r1, [r0, #4]
	cmp r1, #1
	bne _021FEA8E
	mov r1, #0
	str r1, [r0, #4]
	add r0, #0x14
	bl RenderModel_Free
_021FEA8E:
	pop {r3, pc}
	thumb_func_end RockClimb_FreeGfx

	thumb_func_start RockClimb_LoadGfxIfUnused
RockClimb_LoadGfxIfUnused: ; 0x021FEA90
	push {r3, lr}
	ldr r1, [r0]
	cmp r1, #0
	bne _021FEA9C
	bl RockClimb_LoadGfx
_021FEA9C:
	pop {r3, pc}
	.balign 4, 0
	thumb_func_end RockClimb_LoadGfxIfUnused

	thumb_func_start RockClimb_FreeGfxIfUnused
RockClimb_FreeGfxIfUnused: ; 0x021FEAA0
	push {r3, lr}
	ldr r1, [r0]
	cmp r1, #0
	bne _021FEAAC
	bl RockClimb_FreeGfx
_021FEAAC:
	pop {r3, pc}
	.balign 4, 0
	thumb_func_end RockClimb_FreeGfxIfUnused

	thumb_func_start RockClimbEffect_Create
RockClimbEffect_Create: ; 0x021FEAB0
	push {r4, r5, r6, r7, lr}
	sub sp, #0x2c
	add r6, r1, #0
	add r7, r2, #0
	add r2, sp, #0xc
	mov r1, #0
	str r1, [r2]
	str r1, [r2, #4]
	str r3, [sp, #8]
	add r5, r0, #0
	str r1, [r2, #8]
	bl FieldEffectManager_GetFromMapObject
	add r4, r0, #0
	ldr r0, [sp, #8]
	str r4, [sp, #0x1c]
	str r0, [sp, #0x18]
	add r0, r5, #0
	bl MapObject_GetFieldSystem
	str r0, [sp, #0x20]
	add r0, r4, #0
	mov r1, #5
	bl FieldEffectManager_GetRendererData
	str r0, [sp, #0x24]
	ldr r0, [sp, #0x40]
	str r5, [sp, #0x28]
	cmp r0, #0
	bne _021FEB06
	mov r0, #2
	lsl r1, r6, #0x10
	lsl r0, r0, #0xe
	add r1, r1, r0
	str r1, [sp, #0xc]
	lsl r1, r7, #0x10
	add r0, r1, r0
	str r0, [sp, #0x14]
	ldr r0, [sp, #0x20]
	add r1, sp, #0xc
	bl sub_0206121C
	b _021FEB0E
_021FEB06:
	add r0, r5, #0
	add r1, sp, #0xc
	bl MapObject_CopyPositionVector
_021FEB0E:
	add r0, r5, #0
	mov r1, #2
	bl MapObject_GetPriorityPlusValue
	add r1, sp, #0x18
	str r1, [sp]
	str r0, [sp, #4]
	ldr r1, _021FEB2C ; =sRockClimbEffectTemplate
	ldr r3, [sp, #0x40]
	add r0, r4, #0
	add r2, sp, #0xc
	bl FieldEffectManager_CreateEffect
	add sp, #0x2c
	pop {r4, r5, r6, r7, pc}
	.balign 4, 0
_021FEB2C: .word sRockClimbEffectTemplate
	thumb_func_end RockClimbEffect_Create

	thumb_func_start RockClimbEffect_SetFollow
RockClimbEffect_SetFollow: ; 0x021FEB30
	push {r4, lr}
	add r4, r1, #0
	bl FieldEffect_GetState
	str r4, [r0, #0xc]
	pop {r4, pc}
	thumb_func_end RockClimbEffect_SetFollow

	thumb_func_start RockClimbEffect_Init
RockClimbEffect_Init: ; 0x021FEB3C
	push {r4, r5, r6, lr}
	add r5, r0, #0
	add r4, r1, #0
	bl FieldEffect_GetUserData
	add r2, r0, #0
	add r6, r2, #0
	add r3, r4, #0
	ldmia r6!, {r0, r1}
	add r3, #0x10
	stmia r3!, {r0, r1}
	ldmia r6!, {r0, r1}
	stmia r3!, {r0, r1}
	ldr r0, [r6]
	str r0, [r3]
	ldr r0, [r2]
	str r0, [r4, #4]
	add r0, r5, #0
	bl FieldEffect_GetParam
	str r0, [r4, #0xc]
	ldr r0, [r4, #0x1c]
	bl RockClimb_LoadGfxIfUnused
	ldr r0, [r4, #0x1c]
	bl RockClimb_Retain
	mov r0, #1
	pop {r4, r5, r6, pc}
	.balign 4, 0
	thumb_func_end RockClimbEffect_Init

	thumb_func_start RockClimbEffect_Destroy
RockClimbEffect_Destroy: ; 0x021FEB78
	push {r4, lr}
	add r4, r1, #0
	ldr r0, [r4, #0x1c]
	bl RockClimb_Release
	ldr r0, [r4, #0x1c]
	bl RockClimb_FreeGfxIfUnused
	pop {r4, pc}
	.balign 4, 0
	thumb_func_end RockClimbEffect_Destroy

	thumb_func_start RockClimbEffect_Update
RockClimbEffect_Update: ; 0x021FEB8C
	push {r3, r4, r5, lr}
	sub sp, #0x18
	add r5, r0, #0
	ldr r0, [r1, #0xc]
	ldr r4, [r1, #0x20]
	cmp r0, #0
	beq _021FEBBA
	add r1, sp, #0xc
	mov r0, #0
	str r0, [r1]
	str r0, [r1, #4]
	str r0, [r1, #8]
	add r0, r4, #0
	bl sub_0205F9A0
	add r0, r4, #0
	add r1, sp, #0
	bl MapObject_CopyPositionVector
	add r0, r5, #0
	add r1, sp, #0
	bl FieldEffect_SetPosition
_021FEBBA:
	add sp, #0x18
	pop {r3, r4, r5, pc}
	.balign 4, 0
	thumb_func_end RockClimbEffect_Update

	thumb_func_start RockClimbEffect_Draw
RockClimbEffect_Draw: ; 0x021FEBC0
	push {r3, r4, r5, r6, lr}
	sub sp, #0x3c
	add r3, r1, #0
	add r6, r0, #0
	ldr r0, [r3]
	cmp r0, #1
	beq _021FEC2C
	ldr r0, [r3, #8]
	ldr r5, _021FEC30 ; =sRockClimbScale
	add r0, r0, #1
	str r0, [r3, #8]
	ldmia r5!, {r0, r1}
	add r4, sp, #0
	stmia r4!, {r0, r1}
	ldr r0, [r5]
	mov r2, #0
	str r0, [r4]
	ldr r4, [r3, #0x1c]
	ldr r0, [r3, #4]
	add r4, #0x28
	cmp r0, #3
	bhi _021FEC0A
	add r0, r0, r0
	add r0, pc
	ldrh r0, [r0, #6]
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	add pc, r0
_021FEBF8: ; jump table
	.short _021FEC00 - _021FEBF8 - 2 ; case 0
	.short _021FEC0A - _021FEBF8 - 2 ; case 1
	.short _021FEC04 - _021FEBF8 - 2 ; case 2
	.short _021FEC08 - _021FEBF8 - 2 ; case 3
_021FEC00:
	mov r2, #0xb4
	b _021FEC0A
_021FEC04:
	ldr r2, _021FEC34 ; =0x0000010E
	b _021FEC0A
_021FEC08:
	mov r2, #0x5a
_021FEC0A:
	mov r1, #0
	lsl r2, r2, #0x10
	add r0, sp, #0x18
	lsr r2, r2, #0x10
	add r3, r1, #0
	bl Math_CalcRotationMatrixXYZDeg
	add r0, r6, #0
	add r1, sp, #0xc
	bl FieldEffect_GetPosition
	add r0, r4, #0
	add r1, sp, #0xc
	add r2, sp, #0
	add r3, sp, #0x18
	bl RenderObj_Draw
_021FEC2C:
	add sp, #0x3c
	pop {r3, r4, r5, r6, pc}
	.balign 4, 0
_021FEC30: .word sRockClimbScale
_021FEC34: .word 0x0000010E
	thumb_func_end RockClimbEffect_Draw

	.rodata

sRockClimbScale: ; 0x022090F0
	.byte 0x00, 0x10, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00

sRockClimbEffectTemplate: ; 0x022090FC
	.word 0x00000024
	.word RockClimbEffect_Init
	.word RockClimbEffect_Destroy
	.word RockClimbEffect_Update
	.word RockClimbEffect_Draw
