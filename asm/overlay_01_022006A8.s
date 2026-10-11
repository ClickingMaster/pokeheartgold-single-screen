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
	.public UnionChatMarker_New
	.public UnionChatMarker_Free
	.public UnionChatMarker_LoadGfx
	.public UnionChatMarker_FreeGfx
	.public UnionChatMarker_Spawn
	.public UnionChatMarkerEffect_Init
	.public UnionChatMarkerEffect_Destroy
	.public UnionChatMarkerEffect_Update
	.public UnionChatMarkerEffect_Draw
	.public sUnionChatMarkerEffectTemplate
	.public sUnionChatMarkerRecordParams
	.include "overlay_01_022006A8.inc"
	.include "global.inc"

	.text

	thumb_func_start UnionChatMarker_New
UnionChatMarker_New: ; 0x022006A8
	push {r3, r4, r5, lr}
	mov r2, #0
	mov r1, #4
	add r3, r2, #0
	add r5, r0, #0
	bl FieldEffectManager_HeapAllocAndFill
	add r4, r0, #0
	str r5, [r4]
	bl UnionChatMarker_LoadGfx
	add r0, r4, #0
	pop {r3, r4, r5, pc}
	.balign 4, 0
	thumb_func_end UnionChatMarker_New

	thumb_func_start UnionChatMarker_Free
UnionChatMarker_Free: ; 0x022006C4
	push {r4, lr}
	add r4, r0, #0
	bl UnionChatMarker_FreeGfx
	add r0, r4, #0
	bl FieldEffectManager_HeapFree
	pop {r4, pc}
	thumb_func_end UnionChatMarker_Free

	thumb_func_start UnionChatMarker_LoadGfx
UnionChatMarker_LoadGfx: ; 0x022006D4
	push {r3, r4, lr}
	sub sp, #0xc
	add r4, r0, #0
	ldr r0, [r4]
	mov r1, #4
	mov r2, #0x78
	bl FieldEffectManager_LoadBillboardResource
	ldr r0, [r4]
	mov r1, #3
	mov r2, #0x13
	mov r3, #1
	bl FieldEffectManager_LoadTexResource
	mov r0, #3
	str r0, [sp]
	mov r3, #0
	mov r1, #4
	ldr r0, _0220070C ; =sUnionChatMarkerRecordParams
	str r3, [sp, #4]
	str r0, [sp, #8]
	ldr r0, [r4]
	add r2, r1, #0
	bl FieldEffectManager_AddRecord
	add sp, #0xc
	pop {r3, r4, pc}
	nop
_0220070C: .word sUnionChatMarkerRecordParams
	thumb_func_end UnionChatMarker_LoadGfx

	thumb_func_start UnionChatMarker_FreeGfx
UnionChatMarker_FreeGfx: ; 0x02200710
	push {r4, lr}
	add r4, r0, #0
	ldr r0, [r4]
	mov r1, #4
	bl FieldEffectManager_FreeBillboardResource
	ldr r0, [r4]
	mov r1, #3
	bl FieldEffectManager_FreeTexResource
	ldr r0, [r4]
	mov r1, #4
	bl FieldEffectManager_ReleaseRecord
	pop {r4, pc}
	.balign 4, 0
	thumb_func_end UnionChatMarker_FreeGfx

	thumb_func_start UnionChatMarker_Spawn
UnionChatMarker_Spawn: ; 0x02200730
	push {r4, r5, lr}
	sub sp, #0x24
	add r5, r0, #0
	bl FieldEffectManager_GetFromMapObject
	add r4, r0, #0
	add r0, r5, #0
	bl MapObject_GetFieldSystem
	str r0, [sp, #0x14]
	add r0, r4, #0
	mov r1, #0x15
	str r4, [sp, #0x18]
	bl FieldEffectManager_GetRendererData
	str r0, [sp, #0x1c]
	add r1, sp, #8
	mov r0, #0
	str r0, [r1]
	str r0, [r1, #4]
	str r0, [r1, #8]
	add r0, r5, #0
	mov r1, #2
	str r5, [sp, #0x20]
	bl MapObject_GetPriorityPlusValue
	add r1, sp, #0x14
	str r1, [sp]
	str r0, [sp, #4]
	ldr r1, _0220077C ; =sUnionChatMarkerEffectTemplate
	add r0, r4, #0
	add r2, sp, #8
	mov r3, #0
	bl FieldEffectManager_CreateEffect
	add sp, #0x24
	pop {r4, r5, pc}
	nop
_0220077C: .word sUnionChatMarkerEffectTemplate
	thumb_func_end UnionChatMarker_Spawn

	thumb_func_start UnionChatMarkerEffect_Init
UnionChatMarkerEffect_Init: ; 0x02200780
	push {r4, r5, lr}
	sub sp, #0xc
	add r4, r1, #0
	add r2, sp, #0
	mov r1, #0
	str r1, [r2]
	str r1, [r2, #4]
	add r5, r0, #0
	str r1, [r2, #8]
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
	bl FieldEffect_SetPosition
	ldr r0, [r4, #0x14]
	mov r1, #4
	add r2, sp, #0
	bl FieldEffectManager_CreateBillboardFromRecord
	str r0, [r4, #0x20]
	mov r0, #1
	add sp, #0xc
	pop {r4, r5, pc}
	.balign 4, 0
	thumb_func_end UnionChatMarkerEffect_Init

	thumb_func_start UnionChatMarkerEffect_Destroy
UnionChatMarkerEffect_Destroy: ; 0x022007D0
	ldr r3, _022007D8 ; =BillboardObject_Delete
	ldr r0, [r1, #0x20]
	bx r3
	nop
_022007D8: .word BillboardObject_Delete
	thumb_func_end UnionChatMarkerEffect_Destroy

	thumb_func_start UnionChatMarkerEffect_Update
UnionChatMarkerEffect_Update: ; 0x022007DC
	push {r4, lr}
	add r2, r1, #0
	add r4, r0, #0
	ldr r0, [r2, #0x1c]
	ldr r1, [r2, #4]
	ldr r2, [r2, #8]
	bl sub_0205F0A8
	cmp r0, #0
	bne _022007F6
	add r0, r4, #0
	bl FieldEffectManager_DestroyEffect
_022007F6:
	pop {r4, pc}
	thumb_func_end UnionChatMarkerEffect_Update

	thumb_func_start UnionChatMarkerEffect_Draw
UnionChatMarkerEffect_Draw: ; 0x022007F8
	push {r4, r5, r6, lr}
	sub sp, #0x18
	add r5, r1, #0
	ldr r4, [r5, #0x1c]
	ldr r1, [r5, #4]
	add r6, r0, #0
	ldr r2, [r5, #8]
	add r0, r4, #0
	bl sub_0205F0A8
	cmp r0, #0
	bne _0220081A
	add r0, r6, #0
	bl FieldEffectManager_DestroyEffect
	add sp, #0x18
	pop {r4, r5, r6, pc}
_0220081A:
	add r0, r4, #0
	add r1, sp, #0xc
	bl MapObject_CopyPositionVector
	add r0, r4, #0
	add r1, sp, #0
	bl MapObject_CopyFacingVector
	ldr r1, [sp, #0xc]
	ldr r0, [sp]
	add r0, r1, r0
	str r0, [sp, #0xc]
	ldr r1, [sp, #0x10]
	ldr r0, [sp, #4]
	add r0, r1, r0
	str r0, [sp, #0x10]
	ldr r1, [sp, #0x14]
	ldr r0, [sp, #8]
	add r1, r1, r0
	mov r0, #3
	lsl r0, r0, #0xe
	str r1, [sp, #0x14]
	add r0, r1, r0
	str r0, [sp, #0x14]
	ldr r0, [r5, #0x20]
	add r1, sp, #0xc
	bl BillboardObject_SetPosition
	add sp, #0x18
	pop {r4, r5, r6, pc}
	.balign 4, 0
	thumb_func_end UnionChatMarkerEffect_Draw

	.rodata

sUnionChatMarkerEffectTemplate: ; 0x02209308
	.word 0x00000024
	.word UnionChatMarkerEffect_Init
	.word UnionChatMarkerEffect_Destroy
	.word UnionChatMarkerEffect_Update
	.word UnionChatMarkerEffect_Draw

sUnionChatMarkerRecordParams: ; 0x0220931C
	.byte 0x00, 0x00, 0x00, 0x00
	.byte 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x02, 0x00, 0x00, 0x00
