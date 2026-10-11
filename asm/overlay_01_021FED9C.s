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
	.public _021FEE60
	.public _021FEEA4
	.public _021FEEC0
	.public MudSplash_New
	.public MudSplash_Free
	.public MudSplash_LoadGfx
	.public MudSplash_FreeGfx
	.public MudSplash_Spawn
	.public MudSplashEffect_Init
	.public MudSplashEffect_Destroy
	.public MudSplashEffect_Update
	.public MudSplashEffect_Draw
	.public sMudSplashEffectTemplate
	.include "overlay_01_021FED9C.inc"
	.include "global.inc"

	.text

	thumb_func_start MudSplash_New
MudSplash_New: ; 0x021FED9C
	push {r3, r4, r5, lr}
	mov r2, #0
	mov r1, #0x3c
	add r3, r2, #0
	add r5, r0, #0
	bl FieldEffectManager_HeapAllocAndFill
	add r4, r0, #0
	str r5, [r4]
	bl MudSplash_LoadGfx
	add r0, r4, #0
	pop {r3, r4, r5, pc}
	.balign 4, 0
	thumb_func_end MudSplash_New

	thumb_func_start MudSplash_Free
MudSplash_Free: ; 0x021FEDB8
	push {r4, lr}
	add r4, r0, #0
	bl MudSplash_FreeGfx
	add r0, r4, #0
	bl FieldEffectManager_HeapFree
	pop {r4, pc}
	thumb_func_end MudSplash_Free

	thumb_func_start MudSplash_LoadGfx
MudSplash_LoadGfx: ; 0x021FEDC8
	push {r3, r4, lr}
	sub sp, #4
	mov r2, #0
	add r4, r0, #0
	str r2, [sp]
	ldr r0, [r4]
	add r1, r4, #4
	mov r3, #0x1e
	bl FieldEffectManager_LoadRenderModel
	mov r2, #0
	str r2, [sp]
	ldr r0, [r4]
	add r4, #0x18
	add r1, r4, #0
	mov r3, #0x87
	bl FieldEffectManager_LoadRenderAnimation
	add sp, #4
	pop {r3, r4, pc}
	thumb_func_end MudSplash_LoadGfx

	thumb_func_start MudSplash_FreeGfx
MudSplash_FreeGfx: ; 0x021FEDF0
	push {r4, lr}
	add r4, r0, #0
	add r0, r4, #4
	bl RenderModel_Free
	add r4, #0x18
	add r0, r4, #0
	bl RenderAnimation_Free
	pop {r4, pc}
	thumb_func_end MudSplash_FreeGfx

	thumb_func_start MudSplash_Spawn
MudSplash_Spawn: ; 0x021FEE04
	push {r4, r5, r6, r7, lr}
	sub sp, #0x1c
	add r5, r0, #0
	add r6, r1, #0
	add r4, r2, #0
	add r7, r3, #0
	bl FieldEffectManager_GetFromMapObject
	mov r1, #7
	str r0, [sp, #8]
	bl FieldEffectManager_GetRendererData
	str r0, [sp, #0xc]
	add r0, r6, #0
	add r1, r7, #0
	add r2, sp, #0x10
	bl sub_020611C8
	ldr r1, [sp, #0x18]
	lsl r0, r4, #3
	add r0, r1, r0
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
	ldr r1, _021FEE60 ; =sMudSplashEffectTemplate
	add r2, sp, #0x10
	mov r3, #0
	bl FieldEffectManager_CreateEffect
	add sp, #0x1c
	pop {r4, r5, r6, r7, pc}
	nop
_021FEE60: .word sMudSplashEffectTemplate
	thumb_func_end MudSplash_Spawn

	thumb_func_start MudSplashEffect_Init
MudSplashEffect_Init: ; 0x021FEE64
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
	thumb_func_end MudSplashEffect_Init

	thumb_func_start MudSplashEffect_Destroy
MudSplashEffect_Destroy: ; 0x021FEE9C
	ldr r3, _021FEEA4 ; =RenderAnimation_Free
	add r0, r1, #0
	bx r3
	nop
_021FEEA4: .word RenderAnimation_Free
	thumb_func_end MudSplashEffect_Destroy

	thumb_func_start MudSplashEffect_Update
MudSplashEffect_Update: ; 0x021FEEA8
	push {r3, r4, r5, lr}
	add r4, r1, #0
	add r5, r0, #0
	add r0, r4, #0
	bl RenderAnimation_IsDone
	cmp r0, #1
	bne _021FEEC0
	add r0, r5, #0
	bl FieldEffectManager_DestroyEffect
	pop {r3, r4, r5, pc}
_021FEEC0:
	mov r1, #1
	add r0, r4, #0
	lsl r1, r1, #0xc
	mov r2, #0
	bl RenderAnimation_Advance
	pop {r3, r4, r5, pc}
	.balign 4, 0
	thumb_func_end MudSplashEffect_Update

	thumb_func_start MudSplashEffect_Draw
MudSplashEffect_Draw: ; 0x021FEED0
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
	thumb_func_end MudSplashEffect_Draw

	.rodata

sMudSplashEffectTemplate: ; 0x02209124
	.word 0x00000080
	.word MudSplashEffect_Init
	.word MudSplashEffect_Destroy
	.word MudSplashEffect_Update
	.word MudSplashEffect_Draw
