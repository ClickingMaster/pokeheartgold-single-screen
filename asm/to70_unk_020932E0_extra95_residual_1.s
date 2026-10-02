	.extern sub_02093A50
	.extern sub_02093B40
	.extern sub_02093B84
	.extern sub_02094004
	.extern sub_020941CC
	.extern sub_020942B0
	.extern sub_020943EC
	.extern sub_02094D1C
	.extern sub_02094D9C
	.extern sub_020950C0
	.extern sub_020950D4
	.extern sub_020950F8
	.extern sub_0209515C
	.extern sub_02095794
#include "constants/easy_chat.h"
#include "constants/items.h"
#include "constants/maps.h"
#include "constants/moves.h"
#include "constants/pokemon.h"
#include "constants/species.h"
	.include "asm/macros.inc"
	.include "unk_020932E0.inc"
	.include "global.inc"

.text
	thumb_func_start sub_02093354
sub_02093354: ; 0x02093354
	push {r3, r4, r5, r6, r7, lr}
	add r5, r0, #0
	ldr r0, _0209341C ; =0x000046B8
	ldr r0, [r5, r0]
	bl sub_02095D2C
	add r0, r5, #0
	bl sub_0209515C
	ldr r0, _02093420 ; =0x00004650
	mov r1, #1
	add r0, r5, r0
	bl sub_020950F8
	ldr r0, _02093424 ; =0x00004670
	add r0, r5, r0
	bl RemoveWindow
	add r0, r5, #0
	bl sub_020950D4
	ldr r0, _02093428 ; =0x000046C8
	ldr r0, [r5, r0]
	bl Heap_Free
	ldr r0, _0209342C ; =0x000046CC
	ldr r0, [r5, r0]
	bl Heap_Free
	mov r0, #0x57
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	bl SpriteTransfer_DeleteCharTransferTask
	mov r0, #0x16
	lsl r0, r0, #4
	ldr r0, [r5, r0]
	bl SpriteTransfer_DeletePlttTransferTask
	mov r7, #0x51
	mov r6, #0
	add r4, r5, #0
	lsl r7, r7, #2
_020933AA:
	ldr r0, [r4, r7]
	bl Destroy2DGfxResObjMan
	add r6, r6, #1
	add r4, r4, #4
	cmp r6, #6
	blt _020933AA
	ldr r0, [r5, #0x18]
	bl SpriteList_Delete
	ldr r0, _02093430 ; =0x000046A4
	ldr r0, [r5, r0]
	bl Heap_Free
	ldr r0, [r5, #8]
	bl YesNoPrompt_Destroy
	ldr r0, _02093434 ; =0x000007E4
	ldr r0, [r5, r0]
	bl Heap_Free
	mov r0, #0x8d
	lsl r0, r0, #4
	ldr r0, [r5, r0]
	bl Heap_Free
	mov r0, #4
	bl FontID_Release
	ldr r0, _02093438 ; =gSystem + 0x60
	mov r1, #0
	strb r1, [r0, #9]
	bl GfGfx_SwapDisplay
	ldr r0, _0209343C ; =0x04000050
	mov r1, #0
	strh r1, [r0]
	ldr r0, [r5]
	bl FreeBgTilemapBuffer
	ldr r0, [r5]
	mov r1, #1
	bl FreeBgTilemapBuffer
	ldr r0, [r5]
	mov r1, #2
	bl FreeBgTilemapBuffer
	ldr r0, [r5]
	mov r1, #3
	bl FreeBgTilemapBuffer
	add r0, r5, #0
	bl Heap_Free
	pop {r3, r4, r5, r6, r7, pc}
	nop
_0209341C: .word 0x000046B8
_02093420: .word 0x00004650
_02093424: .word 0x00004670
_02093428: .word 0x000046C8
_0209342C: .word 0x000046CC
_02093430: .word 0x000046A4
_02093434: .word 0x000007E4
_02093438: .word gSystem + 0x60
_0209343C: .word 0x04000050
	thumb_func_end sub_02093354

	thumb_func_start sub_02093440
sub_02093440: ; 0x02093440
	push {r4, r5, r6, r7, lr}
	sub sp, #0x14
	add r5, r0, #0
	ldr r0, [r5, #4]
	add r4, r1, #0
	add r6, r2, #0
	add r7, r3, #0
	bl YesNoPrompt_Create
	str r0, [r5, #8]
	ldr r0, _02093580 ; =0x0000463C
	ldr r2, [sp, #0x48]
	str r6, [r5, r0]
	add r1, r0, #4
	str r7, [r5, r1]
	add r1, r0, #0
	str r4, [r5]
	add r1, #0xa0
	str r2, [r5, r1]
	add r1, r0, #0
	ldr r2, [sp, #0x44]
	add r1, #0x9c
	str r2, [r5, r1]
	ldr r1, [sp, #0x34]
	mov r2, #0
	strb r1, [r5, #0xf]
	add r1, r0, #0
	mvn r2, r2
	add r1, #0xa8
	str r2, [r5, r1]
	add r0, #0xa4
	str r2, [r5, r0]
	ldr r0, [sp, #0x30]
	strb r0, [r5, #0x13]
	ldr r0, [r5]
	ldr r1, [r5, #4]
	bl sub_02094D9C
	add r0, r5, #0
	bl sub_02093A50
	add r0, r5, #0
	bl sub_02093B40
	add r0, r5, #0
	bl sub_02094004
	mov r0, #0x10
	mov r1, #1
	bl GfGfx_EngineATogglePlanes
	ldr r1, [r5, #4]
	mov r0, #4
	bl FontID_Alloc
	mov r1, #0x1a
	ldr r2, [r5, #4]
	mov r0, #0
	lsl r1, r1, #4
	bl LoadFontPal0
	mov r1, #6
	ldr r2, [r5, #4]
	mov r0, #0
	lsl r1, r1, #6
	bl LoadFontPal0
	ldr r0, [sp, #0x28]
	ldr r1, [sp, #0x30]
	strb r0, [r5, #0x11]
	ldr r0, [sp, #0x2c]
	lsl r1, r1, #0x18
	strb r0, [r5, #0x12]
	ldr r0, _02093584 ; =0x000046B8
	lsr r1, r1, #0x18
	ldr r0, [r5, r0]
	bl sub_02095D1C
	ldr r0, _02093584 ; =0x000046B8
	mov r1, #2
	ldr r0, [r5, r0]
	mov r2, #0
	bl sub_02095D40
	add r0, r5, #0
	bl sub_02093B84
	ldr r0, [sp, #0x38]
	ldr r1, _02093588 ; =0x00004648
	ldr r2, [sp, #0x3c]
	str r0, [r5, r1]
	add r0, r1, #4
	str r2, [r5, r0]
	mov r2, #1
	add r1, #8
	str r2, [sp]
	mov r3, #0xd
	str r3, [sp, #4]
	mov r0, #2
	str r0, [sp, #8]
	str r3, [sp, #0xc]
	mov r0, #0xa0
	str r0, [sp, #0x10]
	ldr r0, [r5]
	add r1, r5, r1
	mov r3, #4
	bl AddWindowParameterized
	mov r0, #0x15
	str r0, [sp]
	mov r0, #6
	str r0, [sp, #4]
	mov r0, #2
	str r0, [sp, #8]
	mov r0, #0xc
	str r0, [sp, #0xc]
	mov r0, #0xba
	ldr r1, _0209358C ; =0x00004670
	str r0, [sp, #0x10]
	ldr r0, [r5]
	add r1, r5, r1
	mov r2, #1
	mov r3, #0x19
	bl AddWindowParameterized
	add r0, r5, #0
	bl sub_02094D1C
	ldr r1, [r5]
	ldr r2, [r5, #4]
	add r0, r5, #0
	bl sub_020941CC
	add r0, r5, #0
	bl sub_020942B0
	add r0, r5, #0
	bl sub_020943EC
	add r0, r5, #0
	bl sub_020950C0
	bl sub_02095794
	ldr r0, [sp, #0x40]
	ldr r1, _02093590 ; =0x000046B4
	mov r2, #0
	str r0, [r5, r1]
	add r0, r1, #0
	str r2, [r5, #0x14]
	add r0, #8
	str r2, [r5, r0]
	add r0, r1, #0
	add r0, #0xc
	str r2, [r5, r0]
	add r1, #0x10
	str r2, [r5, r1]
	add sp, #0x14
	pop {r4, r5, r6, r7, pc}
	nop
_02093580: .word 0x0000463C
_02093584: .word 0x000046B8
_02093588: .word 0x00004648
_0209358C: .word 0x00004670
_02093590: .word 0x000046B4
	thumb_func_end sub_02093440
