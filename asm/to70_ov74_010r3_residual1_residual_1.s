	.extern AGB_GetBoxMonData
	.extern TranslateAgbSpecies
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
	.extern ov74_02231D48
	.extern ov74_02231D70
	.extern ov74_02231D94
	.extern ov74_02231DB8
	.extern ov74_02231DDC
	.extern ov74_02231E54
	.extern ov74_02231F30
	.extern ov74_02231FB0
	.extern ov74_0223262C
	.extern ov74_02232678
	.include "asm/macros.inc"
	.include "overlay_74_thumb.inc"
	.include "global.inc"

.text
	thumb_func_start ov74_02231FF4
ov74_02231FF4: ; 0x02231FF4
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x20
	add r6, r0, #0
	mov r0, #0x14
	mov r1, #0x4c
	bl NARC_New
	mov r1, #1
	str r0, [sp, #0x10]
	mov r0, #0x4c
	lsl r1, r1, #0xc
	bl Heap_AllocAtEnd
	mov r7, #0
	str r0, [sp, #0x14]
	str r7, [sp, #0xc]
	add r5, r6, #0
_02232016:
	ldr r0, _02232140 ; =0x0000E880
	ldr r1, _02232144 ; =0x0000E884
	ldr r0, [r6, r0]
	ldr r2, [r6, r1]
	mov r1, #0x96
	lsl r1, r1, #4
	mul r1, r2
	add r0, r0, #4
	add r1, r0, r1
	ldr r0, [sp, #0xc]
	mov r2, #0
	add r0, r1, r0
	mov r1, #5
	bl AGB_GetBoxMonData
	cmp r0, #0
	beq _022320DA
	ldr r1, _02232144 ; =0x0000E884
	add r0, r6, #0
	ldr r1, [r6, r1]
	add r2, r7, #0
	bl ov74_02231D70
	ldr r1, _02232144 ; =0x0000E884
	str r0, [sp, #0x1c]
	ldr r1, [r6, r1]
	add r0, r6, #0
	add r2, r7, #0
	bl ov74_02231D94
	ldr r1, _02232144 ; =0x0000E884
	str r0, [sp, #0x18]
	ldr r1, [r6, r1]
	add r0, r6, #0
	add r2, r7, #0
	bl ov74_02231DB8
	add r4, r0, #0
	ldr r0, [sp, #0x1c]
	bl TranslateAgbSpecies
	ldr r2, _02232148 ; =gSystem + 0x60
	add r1, r4, #0
	ldrb r2, [r2, #0xa]
	bl ov74_02231E54
	add r4, r0, #0
	mov r0, #0x6a
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	ldr r1, [sp, #0x18]
	str r0, [sp]
	ldr r0, [sp, #0x14]
	add r2, r4, #0
	str r0, [sp, #4]
	ldr r0, [sp, #0x10]
	add r3, r7, #0
	str r0, [sp, #8]
	ldr r0, [sp, #0x1c]
	bl ov74_02231F30
	mov r0, #0x6a
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	mov r1, #1
	bl Sprite_SetDrawFlag
	ldr r0, _02232140 ; =0x0000E880
	ldr r1, _02232144 ; =0x0000E884
	ldr r0, [r6, r0]
	ldr r2, [r6, r1]
	mov r1, #0x96
	lsl r1, r1, #4
	mul r1, r2
	add r0, r0, #4
	add r1, r0, r1
	ldr r0, [sp, #0xc]
	mov r2, #0
	add r0, r1, r0
	mov r1, #0xc
	bl AGB_GetBoxMonData
	cmp r0, #0
	beq _022320CC
	mov r0, #0x6b
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	mov r1, #1
	bl Sprite_SetDrawFlag
	b _0223210A
_022320CC:
	mov r0, #0x6b
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	mov r1, #0
	bl Sprite_SetDrawFlag
	b _0223210A
_022320DA:
	mov r0, #0
	str r0, [sp]
	ldr r0, [sp, #0x14]
	ldr r1, [sp, #0x18]
	str r0, [sp, #4]
	ldr r0, [sp, #0x10]
	add r2, r4, #0
	str r0, [sp, #8]
	ldr r0, [sp, #0x1c]
	add r3, r7, #0
	bl ov74_02231F30
	mov r0, #0x6a
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	mov r1, #0
	bl Sprite_SetDrawFlag
	mov r0, #0x6b
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	mov r1, #0
	bl Sprite_SetDrawFlag
_0223210A:
	ldr r0, [sp, #0xc]
	add r7, r7, #1
	add r0, #0x50
	add r5, #0xc
	str r0, [sp, #0xc]
	cmp r7, #0x1e
	bge _0223211A
	b _02232016
_0223211A:
	ldr r0, [sp, #0x14]
	bl Heap_Free
	ldr r0, [sp, #0x10]
	bl NARC_Delete
	ldr r1, _0223214C ; =ov74_02231FB0
	ldr r0, _02232150 ; =0x00012604
	str r1, [r6, r0]
	ldr r1, _02232144 ; =0x0000E884
	add r0, r6, #0
	ldr r1, [r6, r1]
	bl ov74_02232678
	add r0, r6, #0
	bl ov74_0223262C
	add sp, #0x20
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
_02232140: .word 0x0000E880
_02232144: .word 0x0000E884
_02232148: .word gSystem + 0x60
_0223214C: .word ov74_02231FB0
_02232150: .word 0x00012604
	thumb_func_end ov74_02231FF4

	thumb_func_start ov74_02232154
ov74_02232154: ; 0x02232154
	push {r4, r5, r6, r7, lr}
	sub sp, #0x44
	mov r2, #0x61
	add r5, r0, #0
	lsl r2, r2, #2
	add r0, sp, #0x14
	add r1, r5, #0
	add r2, r5, r2
	mov r3, #1
	bl ov74_02231D48
	mov r0, #0x28
	str r0, [sp, #8]
	mov r0, #0x31
	mov r4, #0
	lsl r0, r0, #4
	str r4, [sp, #0xc]
	add r7, r5, r0
_02232178:
	mov r0, #0
	str r0, [sp, #0x10]
	ldr r0, [sp, #8]
	mov r6, #0x1c
	lsl r0, r0, #0xc
	str r0, [sp, #4]
_02232184:
	mov r0, #0x1c
	str r0, [sp]
	ldr r2, [sp, #8]
	add r0, r7, #0
	add r1, r6, #0
	mov r3, #0x1c
	bl ov74_02231DDC
	lsl r0, r6, #0xc
	str r0, [sp, #0x1c]
	ldr r0, [sp, #4]
	str r0, [sp, #0x20]
	mov r0, #0x6a
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	cmp r0, #0
	bne _022321B2
	add r0, sp, #0x14
	bl Sprite_CreateAffine
	mov r1, #0x6a
	lsl r1, r1, #2
	str r0, [r5, r1]
_022321B2:
	mov r0, #0x6a
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	mov r1, #1
	bl Sprite_SetAnimActiveFlag
	mov r0, #0x6a
	lsl r0, r0, #2
	add r1, r4, #0
	ldr r0, [r5, r0]
	add r1, #0xa
	bl Sprite_SetAnimCtrlSeq
	mov r0, #0x6a
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	mov r1, #1
	bl Sprite_SetPriority
	mov r0, #0x6a
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	mov r1, #1
	bl Sprite_SetDrawFlag
	mov r0, #0x6a
	lsl r0, r0, #2
	add r1, r4, #0
	ldr r0, [r5, r0]
	add r1, #0x64
	bl Sprite_SetDrawPriority
	mov r0, #6
	ldr r1, [sp, #0x1c]
	lsl r0, r0, #0xc
	add r0, r1, r0
	str r0, [sp, #0x1c]
	mov r0, #3
	ldr r1, [sp, #0x20]
	lsl r0, r0, #0xe
	add r0, r1, r0
	str r0, [sp, #0x20]
	mov r0, #0x6b
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	cmp r0, #0
	bne _0223221C
	add r0, sp, #0x14
	bl Sprite_CreateAffine
	mov r1, #0x6b
	lsl r1, r1, #2
	str r0, [r5, r1]
_0223221C:
	mov r0, #0x6b
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	mov r1, #1
	bl Sprite_SetAnimActiveFlag
	mov r0, #0x6b
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	mov r1, #0x28
	bl Sprite_SetAnimCtrlSeq
	mov r0, #0x6b
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	mov r1, #1
	bl Sprite_SetPriority
	mov r0, #0x6b
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	mov r1, #0
	bl Sprite_SetDrawFlag
	mov r0, #0x6b
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	add r1, r4, #0
	bl Sprite_SetDrawPriority
	add r4, r4, #1
	add r7, r7, #4
	add r5, #0xc
	cmp r4, #0x1e
	beq _02232280
	ldr r0, [sp, #0x10]
	add r6, #0x28
	add r0, r0, #1
	str r0, [sp, #0x10]
	cmp r0, #6
	blt _02232184
	ldr r0, [sp, #8]
	add r0, #0x18
	str r0, [sp, #8]
	ldr r0, [sp, #0xc]
	add r0, r0, #1
	str r0, [sp, #0xc]
	cmp r0, #5
	bge _02232280
	b _02232178
_02232280:
	add sp, #0x44
	pop {r4, r5, r6, r7, pc}
	thumb_func_end ov74_02232154
