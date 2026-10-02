	.extern ov106_021E66FC
	.extern ov106_021E6F48
	.extern ov106_021E6F5C
	.extern ov106_021E6FC8
	.extern ov106_021E7010
#include "constants/mmodel.h"
#include "constants/species.h"
	.include "asm/macros.inc"
	.include "overlay_106.inc"
	.include "global.inc"

.text
	thumb_func_start ov106_021E6520
ov106_021E6520: ; 0x021E6520
	push {r4, r5, r6, r7, lr}
	sub sp, #0x5c
	add r7, r0, #0
	ldr r0, _021E661C ; =0x00000418
	ldr r4, [r7, r0]
	mov r0, #0x99
	bl SpriteSystem_Alloc
	str r0, [r4, #8]
	bl SpriteManager_New
	add r2, sp, #0x3c
	ldr r3, _021E6620 ; =ov106_021E6FC8
	str r0, [r4, #0xc]
	ldmia r3!, {r0, r1}
	add r6, r2, #0
	stmia r2!, {r0, r1}
	ldmia r3!, {r0, r1}
	stmia r2!, {r0, r1}
	ldmia r3!, {r0, r1}
	stmia r2!, {r0, r1}
	ldmia r3!, {r0, r1}
	ldr r5, _021E6624 ; =ov106_021E6F48
	stmia r2!, {r0, r1}
	add r3, sp, #0x28
	ldmia r5!, {r0, r1}
	add r2, r3, #0
	stmia r3!, {r0, r1}
	ldmia r5!, {r0, r1}
	stmia r3!, {r0, r1}
	ldr r0, [r5]
	add r1, r6, #0
	str r0, [r3]
	ldr r0, [r4, #8]
	mov r3, #0x20
	bl SpriteSystem_Init
	ldr r3, _021E6628 ; =ov106_021E6F5C
	add r2, sp, #0x10
	ldmia r3!, {r0, r1}
	stmia r2!, {r0, r1}
	ldmia r3!, {r0, r1}
	stmia r2!, {r0, r1}
	ldmia r3!, {r0, r1}
	stmia r2!, {r0, r1}
	ldr r0, [r4, #8]
	ldr r1, [r4, #0xc]
	mov r2, #1
	bl SpriteSystem_InitSprites
	ldr r0, [r4, #8]
	ldr r1, [r4, #0xc]
	add r2, sp, #0x10
	bl SpriteSystem_InitManagerWithCapacities
	ldr r0, [r4, #8]
	bl SpriteSystem_GetRenderer
	mov r2, #1
	mov r1, #0
	lsl r2, r2, #0x16
	bl G2dRenderer_SetSubSurfaceCoords
	mov r0, #1
	str r0, [sp]
	ldr r0, _021E662C ; =0x0000C8E9
	mov r3, #0xcd
	str r0, [sp, #4]
	ldr r0, [r4, #8]
	ldr r1, [r4, #0xc]
	ldr r2, [r4, #4]
	bl SpriteSystem_LoadCellResObjFromOpenNarc
	mov r0, #1
	str r0, [sp]
	ldr r0, _021E662C ; =0x0000C8E9
	mov r3, #0xce
	str r0, [sp, #4]
	ldr r0, [r4, #8]
	ldr r1, [r4, #0xc]
	ldr r2, [r4, #4]
	bl SpriteSystem_LoadAnimResObjFromOpenNarc
	mov r0, #0
	str r0, [sp]
	mov r0, #1
	str r0, [sp, #4]
	str r0, [sp, #8]
	ldr r0, _021E662C ; =0x0000C8E9
	mov r3, #0xcf
	str r0, [sp, #0xc]
	ldr r0, [r4, #8]
	ldr r1, [r4, #0xc]
	ldr r2, [r4, #4]
	bl SpriteSystem_LoadPlttResObjFromOpenNarc
	mov r0, #1
	str r0, [sp]
	str r0, [sp, #4]
	ldr r0, _021E662C ; =0x0000C8E9
	mov r3, #0xcc
	str r0, [sp, #8]
	ldr r0, [r4, #8]
	ldr r1, [r4, #0xc]
	ldr r2, [r4, #4]
	bl SpriteSystem_LoadCharResObjFromOpenNarc
	ldr r0, [r4, #8]
	ldr r1, [r4, #0xc]
	ldr r2, _021E6630 ; =ov106_021E7010
	bl SpriteSystem_NewSprite
	mov r1, #0
	str r0, [r4, #0x10]
	bl ManagedSprite_SetDrawFlag
	add r0, r7, #0
	bl ov106_021E66FC
	mov r0, #0x10
	mov r1, #1
	bl GfGfx_EngineATogglePlanes
	add sp, #0x5c
	pop {r4, r5, r6, r7, pc}
	nop
_021E661C: .word 0x00000418
_021E6620: .word ov106_021E6FC8
_021E6624: .word ov106_021E6F48
_021E6628: .word ov106_021E6F5C
_021E662C: .word 0x0000C8E9
_021E6630: .word ov106_021E7010
	thumb_func_end ov106_021E6520
