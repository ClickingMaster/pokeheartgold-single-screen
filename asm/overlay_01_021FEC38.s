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
	.public PuddleSplash_New
	.public PuddleSplash_Free
	.public PuddleSplash_LoadGfx
	.public PuddleSplash_FreeGfx
	.public PuddleSplash_Spawn
	.public PuddleSplashEffect_Init
	.public PuddleSplashEffect_Destroy
	.public PuddleSplashEffect_Update
	.public PuddleSplashEffect_Draw
	.public sPuddleSplashEffectTemplate
	.include "overlay_01_021FEC38.inc"
	.include "global.inc"

	.text

	thumb_func_start PuddleSplash_New
PuddleSplash_New: ; 0x021FEC38
	push {r3, r4, r5, lr}
	mov r2, #0
	mov r1, #0x3c
	add r3, r2, #0
	add r5, r0, #0
	bl FieldEffectManager_HeapAllocAndFill
	add r4, r0, #0
	str r5, [r4]
	bl PuddleSplash_LoadGfx
	add r0, r4, #0
	pop {r3, r4, r5, pc}
	.balign 4, 0
	thumb_func_end PuddleSplash_New

	thumb_func_start PuddleSplash_Free
PuddleSplash_Free: ; 0x021FEC54
	push {r4, lr}
	add r4, r0, #0
	bl PuddleSplash_FreeGfx
	add r0, r4, #0
	bl FieldEffectManager_HeapFree
	pop {r4, pc}
	thumb_func_end PuddleSplash_Free

	thumb_func_start PuddleSplash_LoadGfx
PuddleSplash_LoadGfx: ; 0x021FEC64
	push {r3, r4, lr}
	sub sp, #4
	mov r2, #0
	add r4, r0, #0
	str r2, [sp]
	ldr r0, [r4]
	add r1, r4, #4
	mov r3, #0x1d
	bl FieldEffectManager_LoadRenderModel
	mov r2, #0
	str r2, [sp]
	ldr r0, [r4]
	add r4, #0x18
	add r1, r4, #0
	mov r3, #0x86
	bl FieldEffectManager_LoadRenderAnimation
	add sp, #4
	pop {r3, r4, pc}
	thumb_func_end PuddleSplash_LoadGfx

	thumb_func_start PuddleSplash_FreeGfx
PuddleSplash_FreeGfx: ; 0x021FEC8C
	push {r4, lr}
	add r4, r0, #0
	add r0, r4, #4
	bl RenderModel_Free
	add r4, #0x18
	add r0, r4, #0
	bl RenderAnimation_Free
	pop {r4, pc}
	thumb_func_end PuddleSplash_FreeGfx

	thumb_func_start PuddleSplash_Spawn
PuddleSplash_Spawn: ; 0x021FECA0
	push {r4, r5, r6, r7, lr}
	sub sp, #0x1c
	add r6, r1, #0
	mov r1, #2
	lsl r1, r1, #8
	add r5, r0, #0
	add r4, r2, #0
	add r7, r3, #0
	bl MapObject_TestFlagsBits
	cmp r0, #1
	beq _021FED0A
	add r0, r5, #0
	bl FieldEffectManager_GetFromMapObject
	mov r1, #6
	str r0, [sp, #8]
	bl FieldEffectManager_GetRendererData
	str r0, [sp, #0xc]
	add r0, r6, #0
	add r1, r7, #0
	add r2, sp, #0x10
	bl SetPositionToTileCenter
	mov r0, #1
	ldr r2, [sp, #0x18]
	lsl r1, r4, #3
	lsl r0, r0, #0xc
	add r0, r1, r0
	add r0, r2, r0
	str r0, [sp, #0x18]
	add r0, r5, #0
	bl MapObject_GetPreviousYCoord
	lsl r1, r0, #0xf
	mov r0, #2
	lsl r0, r0, #0xe
	sub r0, r1, r0
	str r0, [sp, #0x14]
	add r0, r5, #0
	mov r1, #2
	bl MapObject_GetPriorityPlusValue
	add r1, sp, #8
	str r1, [sp]
	str r0, [sp, #4]
	ldr r0, [sp, #8]
	ldr r1, _021FED10 ; =sPuddleSplashEffectTemplate
	add r2, sp, #0x10
	mov r3, #0
	bl FieldEffectManager_CreateEffect
_021FED0A:
	add sp, #0x1c
	pop {r4, r5, r6, r7, pc}
	nop
_021FED10: .word sPuddleSplashEffectTemplate
	thumb_func_end PuddleSplash_Spawn

	thumb_func_start PuddleSplashEffect_Init
PuddleSplashEffect_Init: ; 0x021FED14
	push {r3, r4, lr}
	sub sp, #4
	add r4, r1, #0
	bl FieldEffect_GetUserData
	ldr r3, [r0, #4]
	ldr r1, [r0]
	mov r0, #0
	str r1, [r4, #0x78]
	str r3, [r4, #0x7c]
	str r0, [sp]
	add r2, r3, #4
	ldr r0, [r4, #0x78]
	add r1, r4, #0
	add r3, #0x18
	bl FieldEffectManager_InitRenderAnimation
	ldr r1, [r4, #0x7c]
	add r0, r4, #0
	add r0, #0x24
	add r1, r1, #4
	add r2, r4, #0
	bl RenderObj_InitWithAnimation
	mov r0, #1
	add sp, #4
	pop {r3, r4, pc}
	.balign 4, 0
	thumb_func_end PuddleSplashEffect_Init

	thumb_func_start PuddleSplashEffect_Destroy
PuddleSplashEffect_Destroy: ; 0x021FED4C
	ldr r3, _021FED54 ; =RenderAnimation_Free
	add r0, r1, #0
	bx r3
	nop
_021FED54: .word RenderAnimation_Free
	thumb_func_end PuddleSplashEffect_Destroy

	thumb_func_start PuddleSplashEffect_Update
PuddleSplashEffect_Update: ; 0x021FED58
	push {r3, r4, r5, lr}
	add r4, r1, #0
	add r5, r0, #0
	add r0, r4, #0
	bl RenderAnimation_IsDone
	cmp r0, #1
	bne _021FED70
	add r0, r5, #0
	bl FieldEffectManager_DestroyEffect
	pop {r3, r4, r5, pc}
_021FED70:
	mov r1, #1
	add r0, r4, #0
	lsl r1, r1, #0xc
	mov r2, #0
	bl RenderAnimation_Advance
	pop {r3, r4, r5, pc}
	.balign 4, 0
	thumb_func_end PuddleSplashEffect_Update

	thumb_func_start PuddleSplashEffect_Draw
PuddleSplashEffect_Draw: ; 0x021FED80
	push {r3, r4, lr}
	sub sp, #0xc
	add r4, r1, #0
	add r1, sp, #0
	bl FieldEffect_GetPosition
	add r4, #0x24
	add r0, r4, #0
	add r1, sp, #0
	bl RenderObj_DrawAt
	add sp, #0xc
	pop {r3, r4, pc}
	.balign 4, 0
	thumb_func_end PuddleSplashEffect_Draw

	.rodata

sPuddleSplashEffectTemplate: ; 0x02209110
	.word 0x00000080
	.word PuddleSplashEffect_Init
	.word PuddleSplashEffect_Destroy
	.word PuddleSplashEffect_Update
	.word PuddleSplashEffect_Draw
