	.extern ov74_02229200
	.extern ov74_02229294
	.extern ov74_02229450
	.extern ov74_02229DA0
	.extern ov74_02229DCC
	.extern ov74_02229DD4
	.extern ov74_02229DE0
	.extern ov74_02229E60
	.extern ov74_02229F90
	.extern ov74_02229FF4
	.extern ov74_0222A2A4
	.extern ov74_0222A2E0
	.extern ov74_0222A35C
	.extern ov74_0222A3CC
	.extern ov74_0222A494
	.extern ov74_0222A4CC
	.extern ov74_0222A504
	.extern ov74_0222A538
	.extern ov74_0222A560
	.extern ov74_0222A5AC
	.extern ov74_0222A5CC
	.extern ov74_0222A650
	.extern ov74_0222A688
	.extern ov74_0222A6C0
	.extern ov74_0222ACD8
	.extern ov74_0222AE6C
	.extern ov74_0222C2EC
	.extern ov74_0222CD94
	.extern ov74_0222D2D4
	.extern ov74_0222D308
	.extern ov74_0222D358
	.extern ov74_0222D414
	.extern ov74_0222D490
	.extern ov74_0222D494
	.extern ov74_0222D4E4
	.extern ov74_0222D55C
	.extern ov74_0222D5C4
	.extern ov74_0222D614
	.extern ov74_0222D690
	.extern ov74_0222D774
	.extern ov74_0222D78C
	.extern ov74_0222D7D0
	.extern ov74_0222DDFC
	.extern ov74_0222E1F4
	.extern ov74_0222EB44
	.extern ov74_0222EE08
	.extern ov74_0222EE0C
	.extern ov74_0222EE58
	.extern ov74_0222EEB0
	.extern ov74_0222EF18
	.extern ov74_0222EF64
	.extern ov74_0222EF68
	.extern ov74_0222F598
	.extern ov74_0222F7D4
	.extern ov74_0222FB8C
	.extern ov74_02230018
	.extern ov74_02230030
	.extern ov74_02230070
	.extern ov74_022300A8
	.extern ov74_02230110
	.extern ov74_02230138
	.extern ov74_02230404
	.extern ov74_02230520
	.extern ov74_02230590
	.extern ov74_022305E4
	.extern ov74_022306C8
	.extern ov74_022308E0
	.extern ov74_02230964
	.extern ov74_02230988
	.extern ov74_022309AC
	.extern ov74_022309F0
	.extern ov74_02231054
	.extern ov74_022310AC
	.extern ov74_022310C4
	.extern ov74_022310D0
	.extern ov74_022310DC
	.extern ov74_022310E8
	.extern ov74_022310F4
	.extern ov74_02231100
	.extern ov74_02231154
	.extern ov74_0223115C
	.extern ov74_022311D8
	.extern ov74_022311DC
	.extern ov74_022311E8
	.extern ov74_02231260
	.extern ov74_0223D454
	.extern ov74_0223D488
	.extern ov74_0223D654
#include "config.h"
#include "msgdata/msg/msg_0247.h"
#include "msgdata/msg/msg_0442.h"
#include "constants/sndseq.h"
	.include "asm/macros.inc"
	.include "overlay_74_thumb.inc"
	.include "global.inc"

.text
	thumb_func_start ov74_02235930
ov74_02235930: ; 0x02235930
	push {r3, r4, r5, lr}
	sub sp, #0x30
	add r4, r1, #0
	ldr r1, _022359B8 ; =ov74_0223D454
	bne _02235992
	ldr r4, [r1, #0x34]
	lsl r2, r2, #0xc
	str r4, [sp]
	mov r4, #0x6a
	lsl r4, r4, #2
	add r5, r1, r4
	mov r4, #0x24
	mul r4, r0
	add r4, r5, r4
	str r4, [sp, #4]
	mov r4, #1
	lsl r4, r4, #0xc
	mov r5, #0
	str r5, [sp, #0x10]
	str r4, [sp, #0x14]
	str r4, [sp, #0x18]
	str r4, [sp, #0x1c]
	add r4, sp, #0
	strh r5, [r4, #0x20]
	str r2, [sp, #8]
	lsl r2, r3, #0xc
	str r2, [sp, #0xc]
	mov r2, #0xa
	str r2, [sp, #0x24]
	cmp r0, #0
	bne _02235972
	mov r2, #1
	b _02235974
_02235972:
	mov r2, #2
_02235974:
	ldr r0, [r1, #8]
	str r2, [sp, #0x28]
	str r0, [sp, #0x2c]
	cmp r2, #2
	bne _0223598A
	mov r0, #0x21
	lsl r0, r0, #4
	ldr r2, [sp, #0xc]
	ldr r0, [r1, r0]
	add r0, r2, r0
	str r0, [sp, #0xc]
_0223598A:
	add r0, sp, #0
	bl Sprite_CreateAffine
	add r4, r0, #0
_02235992:
	add r0, r4, #0
	mov r1, #1
	bl Sprite_SetAnimActiveFlag
	add r0, r4, #0
	mov r1, #0
	bl Sprite_SetPriority
	ldr r1, [sp, #0x40]
	add r0, r4, #0
	bl Sprite_SetAnimCtrlSeq
	add r0, r4, #0
	mov r1, #1
	bl Sprite_SetDrawFlag
	add r0, r4, #0
	add sp, #0x30
	pop {r3, r4, r5, pc}
	.balign 4, 0
_022359B8: .word ov74_0223D454
	thumb_func_end ov74_02235930

	thumb_func_start ov74_022359BC
ov74_022359BC: ; 0x022359BC
	push {r3, r4, r5, r6, r7, lr}
	ldr r0, _02235A5C ; =ov74_0223D654
	ldr r4, _02235A60 ; =ov74_0223D454
	ldr r0, [r0, #8]
	cmp r0, #0
	beq _022359DA
	mov r0, #0x82
	lsl r0, r0, #2
	ldr r0, [r4, r0]
	bl Sprite_Delete
	mov r0, #0x82
	mov r1, #0
	lsl r0, r0, #2
	str r1, [r4, r0]
_022359DA:
	mov r1, #0x51
	ldr r0, _02235A64 ; =ov74_0223D488
	lsl r1, r1, #2
	ldr r0, [r0, r1]
	cmp r0, #0
	beq _022359EA
	bl SpriteTransfer_DeleteCharTransferTask
_022359EA:
	mov r1, #0x57
	ldr r0, _02235A64 ; =ov74_0223D488
	lsl r1, r1, #2
	ldr r0, [r0, r1]
	cmp r0, #0
	beq _022359FA
	bl SpriteTransfer_DeleteCharTransferTask
_022359FA:
	mov r1, #0x52
	ldr r0, _02235A64 ; =ov74_0223D488
	lsl r1, r1, #2
	ldr r0, [r0, r1]
	cmp r0, #0
	beq _02235A0A
	bl SpriteTransfer_DeletePlttTransferTask
_02235A0A:
	mov r1, #0x16
	ldr r0, _02235A64 ; =ov74_0223D488
	lsl r1, r1, #4
	ldr r0, [r0, r1]
	cmp r0, #0
	beq _02235A1A
	bl SpriteTransfer_DeletePlttTransferTask
_02235A1A:
	mov r4, #0
	mov r7, #0x4b
	ldr r5, _02235A64 ; =ov74_0223D488
	add r6, r4, #0
	lsl r7, r7, #2
_02235A24:
	mov r0, #0x4b
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	bl Destroy2DGfxResObjMan
	str r6, [r5, r7]
	add r4, r4, #1
	add r5, r5, #4
	cmp r4, #6
	blt _02235A24
	ldr r0, _02235A64 ; =ov74_0223D488
	ldr r0, [r0]
	bl SpriteList_Delete
	ldr r0, _02235A64 ; =ov74_0223D488
	mov r1, #0
	str r1, [r0]
	bl OamManager_Free
	bl ObjCharTransfer_Destroy
	bl ObjPlttTransfer_Destroy
	mov r0, #0
	add r1, r0, #0
	bl Main_SetVBlankIntrCB
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
_02235A5C: .word ov74_0223D654
_02235A60: .word ov74_0223D454
_02235A64: .word ov74_0223D488
	thumb_func_end ov74_022359BC
