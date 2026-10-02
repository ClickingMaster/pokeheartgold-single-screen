	.extern ov106_021E63E0
	.extern ov106_021E6408
	.extern ov106_021E6418
	.extern ov106_021E6520
	.extern ov106_021E6678
	.extern ov106_021E6694
	.extern ov106_021E66B0
	.extern ov106_021E70E0
#include "constants/mmodel.h"
#include "constants/species.h"
	.include "asm/macros.inc"
	.include "overlay_106.inc"
	.include "global.inc"

.text
	thumb_func_start ov106_021E66FC
ov106_021E66FC: ; 0x021E66FC
	push {r4, r5, r6, r7, lr}
	sub sp, #0x1c
	ldr r1, _021E680C ; =0x00000418
	ldr r1, [r0, r1]
	ldr r0, [r0]
	str r1, [sp, #0x18]
	ldr r0, [r0]
	bl SaveArray_Party_Get
	add r4, r0, #0
	bl Party_GetCount
	cmp r0, #0
	bne _021E6720
	mov r5, #0
	mov r4, #2
	str r5, [sp, #0x14]
	b _021E6764
_021E6720:
	add r0, r4, #0
	mov r1, #0
	bl Party_GetMonByIndex
	mov r1, #5
	mov r2, #0
	add r6, r0, #0
	bl GetMonData
	ldr r1, _021E6810 ; =0x000001ED
	cmp r0, r1
	bne _021E675E
	add r0, r6, #0
	mov r1, #0x6f
	mov r2, #0
	bl GetMonData
	lsl r0, r0, #0x10
	lsr r4, r0, #0x10
	add r0, r6, #0
	mov r1, #0x70
	mov r2, #0
	bl GetMonData
	lsl r0, r0, #0x10
	lsr r5, r0, #0x10
	add r0, r6, #0
	bl MonIsShiny
	str r0, [sp, #0x14]
	b _021E6764
_021E675E:
	mov r5, #0
	mov r4, #2
	str r5, [sp, #0x14]
_021E6764:
	ldr r0, _021E6810 ; =0x000001ED
	add r1, r5, #0
	add r2, r4, #0
	bl ov106_021E66B0
	add r1, r0, #0
	mov r0, #0x51 ; mmodel.narc
	mov r2, #0x99
	bl AllocAndReadWholeNarcMemberByIdPair
	str r0, [sp, #0xc]
	bl NNS_G3dGetTex
	str r0, [sp, #0x10]
	ldr r0, [sp, #0x18]
	ldr r0, [r0, #0x10]
	ldr r0, [r0]
	bl Sprite_GetImageProxy
	mov r1, #1
	bl NNS_G2dGetImageLocation
	add r5, r0, #0
	ldr r0, [sp, #0x10]
	ldr r1, [r0, #0x14]
	add r4, r0, r1
	mov r1, #2
	mov r0, #0x99
	lsl r1, r1, #0xa
	bl Heap_Alloc
	add r6, r0, #0
	mov r7, #0
_021E67A6:
	mov r0, #8
	str r0, [sp]
	str r0, [sp, #4]
	mov r2, #0
	add r0, r4, #0
	mov r1, #8
	add r3, r2, #0
	str r6, [sp, #8]
	bl sub_020145B4
	mov r2, #2
	add r0, r6, #0
	add r1, r5, #0
	lsl r2, r2, #0xa
	bl ov106_021E6678
	mov r0, #2
	lsl r0, r0, #0xa
	add r7, r7, #1
	add r4, r4, r0
	add r5, r5, r0
	cmp r7, #8
	blo _021E67A6
	add r0, r6, #0
	bl Heap_Free
	ldr r0, [sp, #0x18]
	ldr r0, [r0, #0x10]
	ldr r0, [r0]
	bl Sprite_GetPaletteProxy
	mov r1, #1
	bl NNS_G2dGetImagePaletteLocation
	add r1, r0, #0
	ldr r0, [sp, #0x10]
	ldr r2, [r0, #0x38]
	add r0, r0, r2
	ldr r2, [sp, #0x14]
	cmp r2, #1
	bne _021E67FA
	add r0, #0x20
_021E67FA:
	mov r2, #0x20
	bl ov106_021E6694
	ldr r0, [sp, #0xc]
	bl Heap_Free
	add sp, #0x1c
	pop {r4, r5, r6, r7, pc}
	nop
_021E680C: .word 0x00000418
_021E6810: .word 0x000001ED
	thumb_func_end ov106_021E66FC
