	.extern sub_02094758
	.extern sub_02094C08
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
	thumb_func_start sub_020948C4
sub_020948C4: ; 0x020948C4
	push {r4, r5, r6, r7, lr}
	sub sp, #0xc
	add r4, r1, #0
	add r5, r0, #0
	ldr r1, _020949D4 ; =0x00004680
	mov r0, #1
	str r0, [r5, r1]
	add r6, r2, #0
	add r0, r1, #4
	str r6, [r5, r0]
	add r0, r1, #0
	add r0, #0x1c
	str r4, [r5, r0]
	cmp r4, #1
	bne _020948EE
	mov r0, #0x66
	lsl r0, r0, #2
	add r7, r5, r0
	add r1, #0x20
	str r7, [r5, r1]
	b _02094900
_020948EE:
	cmp r4, #2
	bne _020948FC
	ldr r0, _020949D8 ; =0x000007E8
	add r1, #0x20
	add r7, r5, r0
	str r7, [r5, r1]
	b _02094900
_020948FC:
	bl GF_AssertFail
_02094900:
	mov r0, #0x34
	mul r0, r6
	ldr r0, [r7, r0]
	bl Sprite_GetMatrixPtr
	add r3, r0, #0
	ldr r0, _020949DC ; =0x00004688
	add r2, r5, r0
	ldmia r3!, {r0, r1}
	stmia r2!, {r0, r1}
	ldr r0, [r3]
	mov r1, #0x2e
	str r0, [r2]
	mov r0, #0x23
	lsl r0, r0, #6
	ldr r0, [r5, r0]
	bl Sprite_SetAnimCtrlSeq
	ldr r0, _020949E0 ; =0x000046A0
	ldr r2, [r5, r0]
	sub r0, #0x1c
	ldr r1, [r5, r0]
	mov r0, #0x34
	mul r0, r1
	ldr r0, [r2, r0]
	mov r1, #3
	bl Sprite_SetDrawPriority
	cmp r4, #1
	bne _020949D0
	ldr r0, _020949E0 ; =0x000046A0
	ldr r2, [r5, r0]
	sub r0, #0x1c
	ldr r1, [r5, r0]
	mov r0, #0x34
	mul r0, r1
	ldr r0, [r2, r0]
	bl Sprite_GetMatrixPtr
	add r6, r0, #0
	mov r3, #0
	mov r1, #0x83
	ldr r0, _020949E4 ; =0x000008DC
	add r4, r5, #0
	add r2, r3, #0
	lsl r1, r1, #2
_0209495C:
	add r3, r3, #1
	str r2, [r4, r0]
	add r4, r4, r1
	cmp r3, #0x1e
	blt _0209495C
	ldr r1, [r5, #4]
	mov r0, #0x14
	bl NARC_New
	add r4, r0, #0
	mov r1, #1
	ldr r0, [r5, #4]
	lsl r1, r1, #0xc
	bl Heap_AllocAtEnd
	add r7, r0, #0
	ldr r3, _020949E0 ; =0x000046A0
	str r7, [sp]
	add r1, r3, #0
	ldr r0, _020949E8 ; =0x000002B1
	str r4, [sp, #4]
	str r0, [sp, #8]
	sub r1, #0x1c
	ldr r2, [r5, r1]
	mov r1, #0x34
	mul r1, r2
	ldr r0, [r5, r3]
	mov r2, #0
	add r0, r0, r1
	lsr r1, r3, #3
	mov r3, #0x7b
	lsl r3, r3, #4
	ldr r3, [r5, r3]
	add r0, r0, #4
	add r1, r5, r1
	bl sub_02094C08
	mov r0, #0x7b
	lsl r0, r0, #4
	ldr r0, [r5, r0]
	add r1, r6, #0
	bl Sprite_SetMatrix
	mov r0, #0x7b
	lsl r0, r0, #4
	ldr r0, [r5, r0]
	mov r1, #1
	bl Sprite_SetDrawFlag
	add r0, r7, #0
	bl Heap_Free
	add r0, r4, #0
	bl NARC_Delete
	ldr r1, _020949EC ; =sub_02094758
	ldr r0, _020949F0 ; =0x00004644
	str r1, [r5, r0]
_020949D0:
	add sp, #0xc
	pop {r4, r5, r6, r7, pc}
	.balign 4, 0
_020949D4: .word 0x00004680
_020949D8: .word 0x000007E8
_020949DC: .word 0x00004688
_020949E0: .word 0x000046A0
_020949E4: .word 0x000008DC
_020949E8: .word 0x000002B1
_020949EC: .word sub_02094758
_020949F0: .word 0x00004644
	thumb_func_end sub_020948C4
