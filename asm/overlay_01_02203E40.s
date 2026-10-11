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
	.include "overlay_01_02203E40.inc"
	.include "global.inc"

	.text

	thumb_func_start FlashDarkness_New
FlashDarkness_New: ; 0x02203E40
	push {r3, r4, r5, lr}
	mov r2, #0
	mov r1, #0x68
	add r3, r2, #0
	add r5, r0, #0
	bl FieldEffectManager_HeapAllocAndFill
	add r4, r0, #0
	str r5, [r4]
	mov r1, #0
	str r1, [r4, #0x60]
	mov r1, #0xfe
	lsl r1, r1, #0x16
	str r1, [r4, #0x64]
	bl FlashDarkness_LoadModel
	add r0, r4, #0
	pop {r3, r4, r5, pc}
	thumb_func_end FlashDarkness_New

	thumb_func_start FlashDarkness_Free
FlashDarkness_Free: ; 0x02203E64
	push {r4, lr}
	add r4, r0, #0
	bl FlashDarkness_FreeModel
	add r0, r4, #0
	bl FieldEffectManager_HeapFree
	pop {r4, pc}
	thumb_func_end FlashDarkness_Free

	thumb_func_start FlashDarkness_LoadModel
FlashDarkness_LoadModel: ; 0x02203E74
	push {r3, lr}
	sub sp, #8
	add r3, r0, #0
	add r0, #0x58
	str r0, [sp]
	add r0, r3, #0
	add r0, #0x5c
	str r0, [sp, #4]
	ldr r0, [r3]
	mov r1, #0x6a
	mov r2, #0
	add r3, r3, #4
	bl FieldEffectManager_LoadModel
	add sp, #8
	pop {r3, pc}
	thumb_func_end FlashDarkness_LoadModel

	thumb_func_start FlashDarkness_FreeModel
FlashDarkness_FreeModel: ; 0x02203E94
	ldr r3, _02203E9C ; =FieldEffectManager_HeapFree
	ldr r0, [r0, #0x5c]
	bx r3
	nop
_02203E9C: .word FieldEffectManager_HeapFree
	thumb_func_end FlashDarkness_FreeModel

	thumb_func_start FlashDarkness_Show
FlashDarkness_Show: ; 0x02203EA0
	push {r4, r5, r6, lr}
	sub sp, #0x48
	add r5, r0, #0
	bl FieldEffectManager_GetFromMapObject
	add r4, r0, #0
	bl FieldEffectManager_GetFieldSystem
	add r6, r0, #0
	str r4, [sp, #0x2c]
	add r0, r4, #0
	mov r1, #0x13
	bl FieldEffectManager_GetRendererData
	str r0, [sp, #0x30]
	ldr r0, [r0, #0x60]
	cmp r0, #0
	bne _02203F24
	str r5, [sp, #0x34]
	add r0, r5, #0
	add r1, sp, #0x14
	bl MapObject_CopyPositionVector
	ldr r1, [r6, #0x24]
	add r0, sp, #8
	bl Camera_GetLookAtCamPos
	add r0, sp, #8
	add r1, sp, #0x14
	add r2, sp, #0x3c
	bl VEC_Subtract
	add r0, sp, #8
	add r1, sp, #0x14
	bl VEC_Distance
	str r0, [sp, #0x38]
	mov r1, #3
	bl _s32_div_f
	str r0, [sp, #0x38]
	add r0, sp, #0x3c
	add r1, r0, #0
	bl VEC_Normalize
	add r0, r5, #0
	add r1, sp, #0x20
	bl MapObject_CopyPositionVector
	add r0, r5, #0
	mov r1, #2
	bl MapObject_GetPriorityPlusValue
	add r1, sp, #0x2c
	str r1, [sp]
	str r0, [sp, #4]
	ldr r1, _02203F28 ; =ov01_022095B4
	add r0, r4, #0
	add r2, sp, #0x20
	mov r3, #0
	bl FieldEffectManager_CreateEffect
	ldr r1, [sp, #0x30]
	str r0, [r1, #0x60]
	ldr r0, [sp, #0x30]
	ldr r0, [r0, #0x60]
_02203F24:
	add sp, #0x48
	pop {r4, r5, r6, pc}
	.balign 4, 0
_02203F28: .word ov01_022095B4
	thumb_func_end FlashDarkness_Show

	thumb_func_start FlashDarkness_SetScale
FlashDarkness_SetScale: ; 0x02203F2C
	push {r4, lr}
	add r4, r1, #0
	bl FieldEffect_GetState
	ldr r0, [r0, #8]
	str r4, [r0, #0x64]
	pop {r4, pc}
	.balign 4, 0
	thumb_func_end FlashDarkness_SetScale

	thumb_func_start FlashDarknessEffect_Init
FlashDarknessEffect_Init: ; 0x02203F3C
	push {r3, r4, r5, lr}
	add r5, r0, #0
	add r4, r1, #0
	bl FieldEffect_GetUserData
	add r3, r0, #0
	ldmia r3!, {r0, r1}
	add r2, r4, #4
	stmia r2!, {r0, r1}
	ldmia r3!, {r0, r1}
	stmia r2!, {r0, r1}
	ldmia r3!, {r0, r1}
	stmia r2!, {r0, r1}
	ldr r0, [r3]
	str r0, [r2]
	add r0, r5, #0
	bl FieldEffect_GetParam
	str r0, [r4]
	mov r0, #1
	pop {r3, r4, r5, pc}
	.balign 4, 0
	thumb_func_end FlashDarknessEffect_Init

	thumb_func_start FlashDarknessEffect_Destroy
FlashDarknessEffect_Destroy: ; 0x02203F68
	bx lr
	.balign 4, 0
	thumb_func_end FlashDarknessEffect_Destroy

	thumb_func_start FlashDarknessEffect_Update
FlashDarknessEffect_Update: ; 0x02203F6C
	push {r3, r4, r5, lr}
	sub sp, #0x18
	add r4, r1, #0
	add r5, r0, #0
	ldr r0, [r4, #0xc]
	add r1, sp, #0xc
	bl MapObject_CopyPositionVector
	ldr r0, [r4, #0x10]
	add r4, #0x14
	add r1, r4, #0
	add r2, sp, #0xc
	add r3, sp, #0
	bl VEC_MultAdd
	add r0, r5, #0
	add r1, sp, #0
	bl FieldEffect_SetPosition
	add sp, #0x18
	pop {r3, r4, r5, pc}
	.balign 4, 0
	thumb_func_end FlashDarknessEffect_Update

	thumb_func_start FlashDarknessEffect_Draw
FlashDarknessEffect_Draw: ; 0x02203F98
	push {r3, r4, r5, r6, lr}
	sub sp, #0x3c
	ldr r6, _02203FFC ; =ov01_022095C8
	add r5, r0, #0
	add r4, r1, #0
	add r3, sp, #0
	mov r2, #4
_02203FA6:
	ldmia r6!, {r0, r1}
	stmia r3!, {r0, r1}
	sub r2, r2, #1
	bne _02203FA6
	ldr r0, [r6]
	str r0, [r3]
	ldr r1, [r4, #8]
	ldr r0, _02204000 ; =0x45800000
	ldr r1, [r1, #0x64]
	bl _fmul
	bl _ffix
	str r0, [sp, #0x24]
	ldr r1, [r4, #8]
	ldr r0, _02204000 ; =0x45800000
	ldr r1, [r1, #0x64]
	bl _fmul
	bl _ffix
	str r0, [sp, #0x28]
	ldr r1, [r4, #8]
	ldr r0, _02204000 ; =0x45800000
	ldr r1, [r1, #0x64]
	bl _fmul
	bl _ffix
	str r0, [sp, #0x2c]
	add r0, r5, #0
	add r1, sp, #0x30
	bl FieldEffect_GetPosition
	ldr r0, [r4, #8]
	add r1, sp, #0x30
	add r0, r0, #4
	add r2, sp, #0
	add r3, sp, #0x24
	bl GF3dRender_DrawModel
	add sp, #0x3c
	pop {r3, r4, r5, r6, pc}
	.balign 4, 0
_02203FFC: .word ov01_022095C8
_02204000: .word 0x45800000
	thumb_func_end FlashDarknessEffect_Draw

	.rodata

ov01_022095B4: ; 0x022095B4
	.word 0x00000020
	.word FlashDarknessEffect_Init
	.word FlashDarknessEffect_Destroy
	.word FlashDarknessEffect_Update
	.word FlashDarknessEffect_Draw

ov01_022095C8: ; 0x022095C8
	.byte 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00
