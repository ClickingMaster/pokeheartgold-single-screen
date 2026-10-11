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
	.public _021FF720
	.public _021FF7AC
	.public _021FF7FC
	.public _021FF82E
	.public GroundDust_New
	.public GroundDust_Free
	.public GroundDust_LoadGfx
	.public GroundDust_FreeGfx
	.public GroundDust_Spawn
	.public GroundDustEffect_Init
	.public GroundDustEffect_Destroy
	.public GroundDustEffect_Update
	.public GroundDustEffect_Draw
	.public sGroundDustEffectTemplate
	.public sGroundDustRecordParams
	.include "overlay_01_021FF6B0.inc"
	.include "global.inc"

	.text

	thumb_func_start GroundDust_New
GroundDust_New: ; 0x021FF6B0
	push {r3, r4, r5, lr}
	mov r2, #0
	mov r1, #4
	add r3, r2, #0
	add r5, r0, #0
	bl FieldEffectManager_HeapAllocAndFill
	add r4, r0, #0
	str r5, [r4]
	bl GroundDust_LoadGfx
	add r0, r4, #0
	pop {r3, r4, r5, pc}
	.balign 4, 0
	thumb_func_end GroundDust_New

	thumb_func_start GroundDust_Free
GroundDust_Free: ; 0x021FF6CC
	push {r4, lr}
	add r4, r0, #0
	bl GroundDust_FreeGfx
	add r0, r4, #0
	bl FieldEffectManager_HeapFree
	pop {r4, pc}
	thumb_func_end GroundDust_Free

	thumb_func_start GroundDust_LoadGfx
GroundDust_LoadGfx: ; 0x021FF6DC
	push {r3, r4, lr}
	sub sp, #0xc
	add r4, r0, #0
	ldr r0, [r4]
	mov r1, #6
	mov r2, #0x79
	bl FieldEffectManager_LoadBillboardResource
	ldr r0, [r4]
	mov r1, #6
	mov r2, #0x91
	bl FieldEffectManager_LoadBillboardSequence
	ldr r0, [r4]
	mov r1, #7
	mov r2, #0x14
	mov r3, #1
	bl FieldEffectManager_LoadTexResource
	mov r0, #7
	str r0, [sp]
	mov r0, #0
	str r0, [sp, #4]
	ldr r0, _021FF720 ; =sGroundDustRecordParams
	mov r2, #6
	str r0, [sp, #8]
	ldr r0, [r4]
	mov r1, #8
	add r3, r2, #0
	bl FieldEffectManager_AddRecord
	add sp, #0xc
	pop {r3, r4, pc}
	nop
_021FF720: .word sGroundDustRecordParams
	thumb_func_end GroundDust_LoadGfx

	thumb_func_start GroundDust_FreeGfx
GroundDust_FreeGfx: ; 0x021FF724
	push {r4, lr}
	add r4, r0, #0
	ldr r0, [r4]
	mov r1, #6
	bl FieldEffectManager_FreeBillboardResource
	ldr r0, [r4]
	mov r1, #6
	bl FieldEffectManager_FreeBillboardSequence
	ldr r0, [r4]
	mov r1, #7
	bl FieldEffectManager_FreeTexResource
	ldr r0, [r4]
	mov r1, #8
	bl FieldEffectManager_ReleaseRecord
	pop {r4, pc}
	.balign 4, 0
	thumb_func_end GroundDust_FreeGfx

	thumb_func_start GroundDust_Spawn
GroundDust_Spawn: ; 0x021FF74C
	push {r3, r4, r5, r6, lr}
	sub sp, #0x24
	add r5, r0, #0
	bl FieldEffectManager_GetFromMapObject
	add r4, r0, #0
	add r0, r5, #0
	bl MapObject_GetFieldSystem
	str r0, [sp, #0x14]
	add r0, r4, #0
	mov r1, #0xb
	str r4, [sp, #0x18]
	bl FieldEffectManager_GetRendererData
	str r0, [sp, #0x1c]
	add r0, r5, #0
	add r1, sp, #8
	str r5, [sp, #0x20]
	bl MapObject_CopyPositionVector
	add r0, r5, #0
	bl MapObject_GetXCoord
	add r6, r0, #0
	add r0, r5, #0
	bl MapObject_GetZCoord
	add r1, r0, #0
	add r0, r6, #0
	add r2, sp, #8
	bl sub_020611C8
	add r0, r5, #0
	mov r1, #2
	bl MapObject_GetPriorityPlusValue
	add r1, sp, #0x14
	str r1, [sp]
	str r0, [sp, #4]
	ldr r1, _021FF7AC ; =sGroundDustEffectTemplate
	add r0, r4, #0
	add r2, sp, #8
	mov r3, #0
	bl FieldEffectManager_CreateEffect
	add sp, #0x24
	pop {r3, r4, r5, r6, pc}
	.balign 4, 0
_021FF7AC: .word sGroundDustEffectTemplate
	thumb_func_end GroundDust_Spawn

	thumb_func_start GroundDustEffect_Init
GroundDustEffect_Init: ; 0x021FF7B0
	push {r4, r5, lr}
	sub sp, #0xc
	add r4, r1, #0
	add r5, r0, #0
	bl FieldEffect_GetUserData
	add r2, r4, #0
	add r3, r0, #0
	add r2, #0x10
	ldmia r3!, {r0, r1}
	stmia r2!, {r0, r1}
	ldmia r3!, {r0, r1}
	stmia r2!, {r0, r1}
	ldr r0, [r4, #0x1c]
	bl MapObject_GetID
	str r0, [r4, #4]
	ldr r0, [r4, #0x1c]
	bl MapObject_GetMapID
	str r0, [r4, #8]
	add r0, r5, #0
	add r1, sp, #0
	bl FieldEffect_GetPosition
	ldr r0, [r4, #0x14]
	mov r1, #8
	add r2, sp, #0
	bl FieldEffectManager_CreateBillboardFromRecord
	str r0, [r4, #0x20]
	mov r0, #1
	add sp, #0xc
	pop {r4, r5, pc}
	thumb_func_end GroundDustEffect_Init

	thumb_func_start GroundDustEffect_Destroy
GroundDustEffect_Destroy: ; 0x021FF7F4
	ldr r3, _021FF7FC ; =BillboardObject_Delete
	ldr r0, [r1, #0x20]
	bx r3
	nop
_021FF7FC: .word BillboardObject_Delete
	thumb_func_end GroundDustEffect_Destroy

	thumb_func_start GroundDustEffect_Update
GroundDustEffect_Update: ; 0x021FF800
	push {r3, r4, r5, lr}
	add r4, r1, #0
	add r5, r0, #0
	ldr r0, [r4]
	cmp r0, #0
	bne _021FF82E
	mov r1, #1
	ldr r0, [r4, #0x20]
	lsl r1, r1, #0xc
	bl BillboardObject_AdvanceFrame
	ldr r0, [r4, #0x20]
	bl BillboardObject_GetSequenceFrame
	asr r1, r0, #0xb
	lsr r1, r1, #0x14
	add r1, r0, r1
	asr r0, r1, #0xc
	cmp r0, #9
	blt _021FF82E
	add r0, r5, #0
	bl FieldEffectManager_DestroyEffect
_021FF82E:
	pop {r3, r4, r5, pc}
	thumb_func_end GroundDustEffect_Update

	thumb_func_start GroundDustEffect_Draw
GroundDustEffect_Draw: ; 0x021FF830
	push {r3, r4, lr}
	sub sp, #0xc
	add r4, r1, #0
	add r1, sp, #0
	bl FieldEffect_GetPosition
	mov r0, #2
	ldr r1, [sp, #8]
	lsl r0, r0, #0xe
	add r0, r1, r0
	str r0, [sp, #8]
	ldr r0, [r4, #0x20]
	add r1, sp, #0
	bl BillboardObject_SetPosition
	add sp, #0xc
	pop {r3, r4, pc}
	.balign 4, 0
	thumb_func_end GroundDustEffect_Draw

	.rodata

sGroundDustEffectTemplate: ; 0x022091EC
	.word 0x00000024
	.word GroundDustEffect_Init
	.word GroundDustEffect_Destroy
	.word GroundDustEffect_Update
	.word GroundDustEffect_Draw

sGroundDustRecordParams: ; 0x02209200
	.byte 0x00, 0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00
