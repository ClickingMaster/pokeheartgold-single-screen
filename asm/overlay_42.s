	.include "asm/macros.inc"
	.include "overlay_42.inc"
	.include "global.inc"

	.text

	thumb_func_start ov42_02227060
ov42_02227060: ; 0x02227060
	push {r3, r4, r5, r6, r7, lr}
	add r5, r1, #0
	add r6, r3, #0
	add r4, r0, #0
	ldr r1, _02227100 ; =0x0000230C
	add r0, r6, #0
	str r2, [sp]
	bl Heap_Alloc
	ldr r2, _02227100 ; =0x0000230C
	add r7, r0, #0
	mov r1, #0
	bl memset
	str r4, [r7]
	mov r0, #0xb3
	add r1, r6, #0
	str r5, [r7, #4]
	bl NARC_New
	ldr r1, _02227104 ; =0x00002304
	str r0, [r7, r1]
	mov r0, #0xba
	add r1, r6, #0
	bl NARC_New
	ldr r1, _02227108 ; =0x00002308
	mov r4, #0
	str r0, [r7, r1]
	add r5, r7, #0
_0222709C:
	mov r0, #0x9a
	add r1, r4, #0
	add r2, r6, #0
	bl Create2DGfxResObjMan
	ldr r1, _0222710C ; =0x000022F0
	add r4, r4, #1
	str r0, [r5, r1]
	add r5, r5, #4
	cmp r4, #2
	blt _0222709C
	mov r5, #0
	add r4, r7, #0
_022270B6:
	mov r0, #0x17
	add r1, r5, #2
	add r2, r6, #0
	bl Create2DGfxResObjMan
	ldr r1, _02227110 ; =0x000022F8
	add r5, r5, #1
	str r0, [r4, r1]
	add r4, r4, #4
	cmp r5, #2
	blt _022270B6
	mov r4, #0
_022270CE:
	add r0, r7, #0
	add r1, r4, #0
	add r2, r6, #0
	bl ov42_02227570
	add r4, r4, #1
	cmp r4, #3
	blt _022270CE
	ldr r0, [sp]
	ldr r2, [sp]
	mov r1, #0x18
	str r0, [r7, #0xc]
	add r0, r6, #0
	mul r1, r2
	bl Heap_Alloc
	str r0, [r7, #8]
	ldr r3, [r7, #0xc]
	mov r2, #0x18
	mov r1, #0
	mul r2, r3
	bl memset
	add r0, r7, #0
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
_02227100: .word 0x0000230C
_02227104: .word 0x00002304
_02227108: .word 0x00002308
_0222710C: .word 0x000022F0
_02227110: .word 0x000022F8
	thumb_func_end ov42_02227060

	thumb_func_start ov42_02227114
ov42_02227114: ; 0x02227114
	push {r3, r4, r5, r6, r7, lr}
	add r5, r0, #0
	ldr r0, [r5, #0xc]
	mov r6, #0
	cmp r0, #0
	bls _02227140
	add r4, r6, #0
_02227122:
	ldr r0, [r5, #8]
	add r0, r0, r4
	bl ov42_02227E68
	cmp r0, #0
	bne _02227136
	ldr r0, [r5, #8]
	add r0, r0, r4
	bl ov42_0222740C
_02227136:
	ldr r0, [r5, #0xc]
	add r6, r6, #1
	add r4, #0x18
	cmp r6, r0
	blo _02227122
_02227140:
	mov r4, #0
_02227142:
	add r0, r5, #0
	add r1, r4, #0
	bl ov42_02227604
	add r4, r4, #1
	cmp r4, #3
	blt _02227142
	add r0, r5, #0
	bl ov42_022271F8
	ldr r0, _02227188 ; =0x00002304
	ldr r0, [r5, r0]
	bl NARC_Delete
	ldr r0, _0222718C ; =0x00002308
	ldr r0, [r5, r0]
	bl NARC_Delete
	ldr r7, _02227190 ; =0x000022F0
	mov r6, #0
	add r4, r5, #0
_0222716C:
	ldr r0, [r4, r7]
	bl Destroy2DGfxResObjMan
	add r6, r6, #1
	add r4, r4, #4
	cmp r6, #4
	blt _0222716C
	ldr r0, [r5, #8]
	bl Heap_Free
	add r0, r5, #0
	bl Heap_Free
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
_02227188: .word 0x00002304
_0222718C: .word 0x00002308
_02227190: .word 0x000022F0
	thumb_func_end ov42_02227114

	thumb_func_start ov42_02227194
ov42_02227194: ; 0x02227194
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #8
	add r4, r1, #0
	add r7, r3, #0
	add r5, r0, #0
	add r0, r4, #0
	add r1, r7, #0
	add r6, r2, #0
	bl ov42_02227680
	cmp r0, #0
	bne _022271B0
	bl GF_AssertFail
_022271B0:
	add r0, r4, #0
	bl ov42_02227654
	add r1, r0, #0
	ldr r0, [sp, #0x20]
	add r2, r6, #0
	str r0, [sp]
	mov r0, #0
	str r0, [sp, #4]
	add r0, r5, #0
	add r3, r7, #0
	bl ov42_022276DC
	add sp, #8
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
	thumb_func_end ov42_02227194

	thumb_func_start ov42_022271D0
ov42_022271D0: ; 0x022271D0
	push {r4, lr}
	add r4, r0, #0
	add r0, r1, #0
	bl ov42_02227654
	add r1, r0, #0
	add r0, r4, #0
	bl ov42_02227948
	pop {r4, pc}
	thumb_func_end ov42_022271D0

	thumb_func_start ov42_022271E4
ov42_022271E4: ; 0x022271E4
	push {r4, lr}
	add r4, r0, #0
	add r0, r1, #0
	bl ov42_02227654
	add r1, r0, #0
	add r0, r4, #0
	bl ov42_022279E8
	pop {r4, pc}
	thumb_func_end ov42_022271E4

	thumb_func_start ov42_022271F8
ov42_022271F8: ; 0x022271F8
	push {r3, r4, r5, lr}
	mov r1, #0x23
	add r4, r0, #0
	lsl r1, r1, #8
	ldr r1, [r4, r1]
	cmp r1, #0
	beq _0222720A
	bl ov42_02227274
_0222720A:
	mov r5, #0
_0222720C:
	add r0, r4, #0
	add r1, r5, #0
	bl ov42_022279E8
	cmp r0, #1
	bne _02227220
	add r0, r4, #0
	add r1, r5, #0
	bl ov42_02227948
_02227220:
	add r5, r5, #1
	cmp r5, #0x99
	blt _0222720C
	pop {r3, r4, r5, pc}
	thumb_func_end ov42_022271F8

	thumb_func_start ov42_02227228
ov42_02227228: ; 0x02227228
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x10
	add r7, r0, #0
	add r6, r3, #0
	str r1, [sp, #8]
	str r2, [sp, #0xc]
	add r0, r2, #0
	bne _0222723C
	bl GF_AssertFail
_0222723C:
	ldr r1, [sp, #8]
	add r0, r7, #0
	add r2, r6, #0
	bl ov42_02227B6C
	add r0, r7, #0
	add r1, r6, #0
	bl ov42_02227C18
	ldr r5, _02227270 ; =ov42_02229B10
	mov r4, #0
_02227252:
	str r6, [sp]
	mov r0, #1
	str r0, [sp, #4]
	ldrb r1, [r5]
	ldr r2, [sp, #8]
	ldr r3, [sp, #0xc]
	add r0, r7, #0
	bl ov42_022276DC
	add r4, r4, #1
	add r5, r5, #1
	cmp r4, #0x10
	blt _02227252
	add sp, #0x10
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
_02227270: .word ov42_02229B10
	thumb_func_end ov42_02227228

	thumb_func_start ov42_02227274
ov42_02227274: ; 0x02227274
	push {r4, r5, r6, lr}
	add r6, r0, #0
	bl ov42_02227BE0
	add r0, r6, #0
	bl ov42_02227CA8
	ldr r5, _02227298 ; =ov42_02229B10
	mov r4, #0
_02227286:
	ldrb r1, [r5]
	add r0, r6, #0
	bl ov42_02227948
	add r4, r4, #1
	add r5, r5, #1
	cmp r4, #0x10
	blt _02227286
	pop {r4, r5, r6, pc}
	.balign 4, 0
_02227298: .word ov42_02229B10
	thumb_func_end ov42_02227274

	thumb_func_start ov42_0222729C
ov42_0222729C: ; 0x0222729C
	push {r3, r4, r5, lr}
	add r4, r2, #0
	add r2, r3, #0
	add r5, r0, #0
	bl ov42_022279FC
	ldr r0, _022272B0 ; =0x000022EC
	str r4, [r5, r0]
	pop {r3, r4, r5, pc}
	nop
_022272B0: .word 0x000022EC
	thumb_func_end ov42_0222729C

	thumb_func_start ov42_022272B4
ov42_022272B4: ; 0x022272B4
	ldr r3, _022272B8 ; =ov42_02227B04
	bx r3
	.balign 4, 0
_022272B8: .word ov42_02227B04
	thumb_func_end ov42_022272B4

	thumb_func_start ov42_022272BC
ov42_022272BC: ; 0x022272BC
	push {r4, r5, r6, r7, lr}
	sub sp, #0x3c
	add r7, r2, #0
	add r5, r0, #0
	add r6, r1, #0
	str r3, [sp]
	bl ov42_02227CF8
	add r4, r0, #0
	add r0, r7, #0
	bl ov42_02227654
	str r0, [sp, #4]
	ldr r1, [sp, #4]
	add r0, r5, #0
	bl ov42_022279E8
	cmp r0, #1
	beq _022272E6
	bl GF_AssertFail
_022272E6:
	mov r0, #0
	add r2, sp, #0xc
	add r1, r0, #0
	stmia r2!, {r0, r1}
	stmia r2!, {r0, r1}
	stmia r2!, {r0, r1}
	stmia r2!, {r0, r1}
	stmia r2!, {r0, r1}
	stmia r2!, {r0, r1}
	ldr r1, [r5]
	mov r2, #0x38
	str r1, [sp, #0xc]
	ldr r1, [sp, #4]
	add r7, r1, #0
	add r1, r5, #0
	mul r7, r2
	add r1, #0x24
	add r1, r1, r7
	str r1, [sp, #0x10]
	ldrsh r0, [r6, r0]
	lsl r0, r0, #0xc
	str r0, [sp, #0x14]
	mov r0, #2
	ldrsh r1, [r6, r0]
	lsl r1, r1, #0xc
	str r1, [sp, #0x18]
	lsl r1, r0, #0xb
	str r1, [sp, #0x20]
	str r1, [sp, #0x24]
	str r1, [sp, #0x28]
	mov r1, #4
	ldrsh r1, [r6, r1]
	str r1, [sp, #0x30]
	ldr r1, [sp]
	str r1, [sp, #0x38]
	add r1, r5, #0
	str r1, [sp, #8]
	add r1, #0x1c
	str r1, [sp, #8]
	ldr r1, [r1, r7]
	cmp r1, #3
	beq _0222733E
	cmp r1, #1
	bne _02227344
_0222733E:
	mov r0, #1
	str r0, [sp, #0x34]
	b _02227346
_02227344:
	str r0, [sp, #0x34]
_02227346:
	add r0, sp, #0xc
	bl Sprite_CreateAffine
	str r0, [r4, #4]
	mov r1, #6
	ldrsh r1, [r6, r1]
	lsl r1, r1, #0x18
	lsr r1, r1, #0x18
	bl Sprite_SetPriority
	add r0, r5, r7
	ldr r0, [r0, #0x20]
	cmp r0, #1
	ldr r0, [r4, #4]
	bne _0222736C
	mov r1, #6
	bl Sprite_SetAnimCtrlSeq
	b _02227372
_0222736C:
	mov r1, #1
	bl Sprite_SetAnimCtrlSeq
_02227372:
	mov r0, #9
	str r0, [r4, #0xc]
	mov r0, #0
	strh r0, [r4, #0x14]
	mov r0, #1
	str r0, [r4, #0x10]
	add r0, r5, #0
	add r0, #0x10
	add r0, r0, r7
	str r0, [r4]
	ldr r0, _02227404 ; =0x000022B8
	add r0, r5, r0
	bl ov42_02227B5C
	cmp r0, #0
	beq _022273F8
	mov r0, #0
	add r2, sp, #0xc
	add r1, r0, #0
	stmia r2!, {r0, r1}
	stmia r2!, {r0, r1}
	stmia r2!, {r0, r1}
	stmia r2!, {r0, r1}
	stmia r2!, {r0, r1}
	stmia r2!, {r0, r1}
	ldr r1, [r5]
	str r1, [sp, #0xc]
	ldr r1, _02227408 ; =0x000022C8
	add r2, r5, r1
	str r2, [sp, #0x10]
	ldrsh r0, [r6, r0]
	add r1, #0x24
	lsl r0, r0, #0xc
	str r0, [sp, #0x14]
	mov r0, #2
	ldrsh r2, [r6, r0]
	lsl r2, r2, #0xc
	str r2, [sp, #0x18]
	lsl r2, r0, #0xb
	str r2, [sp, #0x20]
	str r2, [sp, #0x24]
	str r2, [sp, #0x28]
	ldr r1, [r5, r1]
	str r1, [sp, #0x30]
	ldr r1, [sp]
	str r1, [sp, #0x38]
	ldr r1, [sp, #8]
	ldr r1, [r1, r7]
	cmp r1, #3
	beq _022273DA
	cmp r1, #1
	bne _022273E0
_022273DA:
	mov r0, #1
	str r0, [sp, #0x34]
	b _022273E2
_022273E0:
	str r0, [sp, #0x34]
_022273E2:
	add r0, sp, #0xc
	bl Sprite_CreateAffine
	str r0, [r4, #8]
	mov r1, #6
	ldrsh r1, [r6, r1]
	lsl r1, r1, #0x18
	lsr r1, r1, #0x18
	bl Sprite_SetPriority
	b _022273FC
_022273F8:
	mov r0, #0
	str r0, [r4, #8]
_022273FC:
	add r0, r4, #0
	add sp, #0x3c
	pop {r4, r5, r6, r7, pc}
	nop
_02227404: .word 0x000022B8
_02227408: .word 0x000022C8
	thumb_func_end ov42_022272BC

	thumb_func_start ov42_0222740C
ov42_0222740C: ; 0x0222740C
	push {r4, lr}
	add r4, r0, #0
	ldr r0, [r4, #8]
	cmp r0, #0
	beq _0222741A
	bl Sprite_Delete
_0222741A:
	ldr r0, [r4, #4]
	bl Sprite_Delete
	mov r1, #0x18
	mov r0, #0
_02227424:
	strb r0, [r4]
	add r4, r4, #1
	sub r1, r1, #1
	bne _02227424
	pop {r4, pc}
	.balign 4, 0
	thumb_func_end ov42_0222740C

	thumb_func_start ov42_02227430
ov42_02227430: ; 0x02227430
	ldr r0, [r0, #4]
	bx lr
	thumb_func_end ov42_02227430

	thumb_func_start ov42_02227434
ov42_02227434: ; 0x02227434
	push {r3, r4, lr}
	sub sp, #0xc
	add r4, r0, #0
	lsl r0, r1, #0xc
	str r0, [sp]
	lsl r0, r2, #0xc
	str r0, [sp, #4]
	mov r0, #0
	str r0, [sp, #8]
	ldr r0, [r4, #4]
	add r1, sp, #0
	bl Sprite_SetMatrix
	ldr r0, [r4, #8]
	cmp r0, #0
	beq _02227470
	mov r0, #2
	ldr r1, [sp]
	lsl r0, r0, #0xe
	add r0, r1, r0
	str r0, [sp]
	mov r0, #0xe
	ldr r1, [sp, #4]
	lsl r0, r0, #0xc
	add r0, r1, r0
	str r0, [sp, #4]
	ldr r0, [r4, #8]
	add r1, sp, #0
	bl Sprite_SetMatrix
_02227470:
	add sp, #0xc
	pop {r3, r4, pc}
	thumb_func_end ov42_02227434

	thumb_func_start ov42_02227474
ov42_02227474: ; 0x02227474
	ldr r3, _0222747C ; =Sprite_SetDrawPriority
	ldr r0, [r0, #4]
	bx r3
	nop
_0222747C: .word Sprite_SetDrawPriority
	thumb_func_end ov42_02227474

	thumb_func_start ov42_02227480
ov42_02227480: ; 0x02227480
	push {r3, lr}
	ldr r0, [r0, #4]
	bl Sprite_GetDrawPriority
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	pop {r3, pc}
	.balign 4, 0
	thumb_func_end ov42_02227480

	thumb_func_start ov42_02227490
ov42_02227490: ; 0x02227490
	push {r3, r4, r5, lr}
	add r5, r0, #0
	ldr r0, [r5, #4]
	add r4, r1, #0
	bl Sprite_SetDrawFlag
	ldr r0, [r5, #8]
	cmp r0, #0
	beq _022274A8
	add r1, r4, #0
	bl Sprite_SetDrawFlag
_022274A8:
	pop {r3, r4, r5, pc}
	.balign 4, 0
	thumb_func_end ov42_02227490

	thumb_func_start ov42_022274AC
ov42_022274AC: ; 0x022274AC
	ldr r3, _022274B4 ; =Sprite_GetDrawFlag
	ldr r0, [r0, #4]
	bx r3
	nop
_022274B4: .word Sprite_GetDrawFlag
	thumb_func_end ov42_022274AC

	thumb_func_start ov42_022274B8
ov42_022274B8: ; 0x022274B8
	ldr r0, [r0]
	ldr r0, [r0, #0x10]
	bx lr
	.balign 4, 0
	thumb_func_end ov42_022274B8

	thumb_func_start ov42_022274C0
ov42_022274C0: ; 0x022274C0
	ldr r0, [r0, #0xc]
	bx lr
	thumb_func_end ov42_022274C0

	thumb_func_start ov42_022274C4
ov42_022274C4: ; 0x022274C4
	ldr r3, _022274CC ; =Sprite_SetAnimationFrame
	ldr r0, [r0, #4]
	bx r3
	nop
_022274CC: .word Sprite_SetAnimationFrame
	thumb_func_end ov42_022274C4

	thumb_func_start ov42_022274D0
ov42_022274D0: ; 0x022274D0
	ldr r3, _022274D8 ; =Sprite_GetAnimationFrame
	ldr r0, [r0, #4]
	bx r3
	nop
_022274D8: .word Sprite_GetAnimationFrame
	thumb_func_end ov42_022274D0

	thumb_func_start ov42_022274DC
ov42_022274DC: ; 0x022274DC
	push {lr}
	sub sp, #0xc
	ldr r3, [r0, #8]
	cmp r3, #0
	beq _022274FE
	add r1, #8
	lsl r1, r1, #0xc
	add r2, #0xe
	str r1, [sp]
	lsl r1, r2, #0xc
	str r1, [sp, #4]
	mov r1, #0
	str r1, [sp, #8]
	ldr r0, [r0, #8]
	add r1, sp, #0
	bl Sprite_SetMatrix
_022274FE:
	add sp, #0xc
	pop {pc}
	.balign 4, 0
	thumb_func_end ov42_022274DC

	thumb_func_start ov42_02227504
ov42_02227504: ; 0x02227504
	push {r4, r5, r6, lr}
	add r4, r1, #0
	add r5, r0, #0
	add r6, r2, #0
	cmp r4, #0xa
	blt _02227514
	bl GF_AssertFail
_02227514:
	cmp r6, #4
	blt _0222751C
	bl GF_AssertFail
_0222751C:
	str r4, [r5, #0xc]
	mov r0, #0
	strh r0, [r5, #0x14]
	ldr r0, _02227534 ; =ov42_02229B20
	lsl r1, r4, #2
	ldr r2, [r0, r1]
	cmp r2, #0
	beq _02227532
	add r0, r5, #0
	add r1, r6, #0
	blx r2
_02227532:
	pop {r4, r5, r6, pc}
	.balign 4, 0
_02227534: .word ov42_02229B20
	thumb_func_end ov42_02227504

	thumb_func_start ov42_02227538
ov42_02227538: ; 0x02227538
	push {r4, lr}
	add r4, r0, #0
	ldr r2, [r4, #0xc]
	ldr r1, _0222756C ; =ov42_02229B04
	mov r0, #0
	ldrb r2, [r1, r2]
	cmp r2, #0xff
	bne _0222754C
	mov r0, #1
	b _02227558
_0222754C:
	mov r1, #0x14
	ldrsh r1, [r4, r1]
	add r1, r1, #1
	cmp r2, r1
	blt _02227558
	mov r0, #1
_02227558:
	cmp r0, #1
	bne _0222756A
	add r0, r4, #0
	bl ov42_02227D30
	mov r0, #0x14
	ldrsh r0, [r4, r0]
	add r0, r0, #1
	strh r0, [r4, #0x14]
_0222756A:
	pop {r4, pc}
	.balign 4, 0
_0222756C: .word ov42_02229B04
	thumb_func_end ov42_02227538

	thumb_func_start ov42_02227570
ov42_02227570: ; 0x02227570
	push {r4, r5, r6, r7, lr}
	sub sp, #0x1c
	str r0, [sp, #0xc]
	add r0, r1, #0
	lsl r5, r0, #4
	ldr r0, [sp, #0xc]
	str r1, [sp, #0x10]
	add r1, r0, r5
	ldr r0, _022275F0 ; =0x00002188
	str r2, [sp, #0x14]
	ldr r0, [r1, r0]
	cmp r0, #0
	beq _0222758E
	bl GF_AssertFail
_0222758E:
	ldr r0, _022275F4 ; =ov42_02229B70
	ldr r6, _022275F8 ; =_02229AF8
	add r4, r0, r5
	ldr r0, [sp, #0xc]
	mov r7, #0
	add r5, r0, r5
	ldr r0, [sp, #0x10]
	lsl r1, r0, #1
	mov r0, #1
	lsl r0, r0, #8
	add r0, r1, r0
	str r0, [sp, #0x18]
_022275A6:
	lsr r2, r7, #0x1f
	lsl r1, r7, #0x1f
	sub r1, r1, r2
	mov r0, #0x1f
	ror r1, r0
	add r0, r2, r1
	ldrb r2, [r6]
	ldr r1, [sp, #0x18]
	add r0, r0, #2
	add r1, r2, r1
	str r1, [sp]
	ldr r1, [sp, #0x14]
	str r0, [sp, #4]
	str r1, [sp, #8]
	lsl r1, r0, #2
	ldr r0, [sp, #0xc]
	ldr r2, [sp, #0xc]
	add r1, r0, r1
	ldr r0, _022275FC ; =0x000022F0
	mov r3, #0
	ldr r0, [r1, r0]
	ldr r1, _02227600 ; =0x00002304
	ldr r1, [r2, r1]
	ldr r2, [r4]
	bl AddCellOrAnimResObjFromOpenNarc
	ldr r1, _022275F0 ; =0x00002188
	add r7, r7, #1
	str r0, [r5, r1]
	add r6, r6, #1
	add r4, r4, #4
	add r5, r5, #4
	cmp r7, #4
	blt _022275A6
	add sp, #0x1c
	pop {r4, r5, r6, r7, pc}
	nop
_022275F0: .word 0x00002188
_022275F4: .word ov42_02229B70
_022275F8: .word _02229AF8
_022275FC: .word 0x000022F0
_02227600: .word 0x00002304
	thumb_func_end ov42_02227570

	thumb_func_start ov42_02227604
ov42_02227604: ; 0x02227604
	push {r3, r4, r5, r6, r7, lr}
	add r6, r0, #0
	lsl r5, r1, #4
	ldr r0, _0222764C ; =0x00002188
	add r1, r6, r5
	ldr r0, [r1, r0]
	cmp r0, #0
	bne _02227618
	bl GF_AssertFail
_02227618:
	mov r4, #0
	add r5, r6, r5
	add r7, r4, #0
_0222761E:
	lsr r2, r4, #0x1f
	lsl r1, r4, #0x1f
	sub r1, r1, r2
	mov r0, #0x1f
	ror r1, r0
	add r0, r2, r1
	add r0, r0, #2
	lsl r0, r0, #2
	add r1, r6, r0
	ldr r0, _02227650 ; =0x000022F0
	ldr r0, [r1, r0]
	ldr r1, _0222764C ; =0x00002188
	ldr r1, [r5, r1]
	bl DestroySingle2DGfxResObj
	ldr r0, _0222764C ; =0x00002188
	add r4, r4, #1
	str r7, [r5, r0]
	add r5, r5, #4
	cmp r4, #4
	blt _0222761E
	pop {r3, r4, r5, r6, r7, pc}
	nop
_0222764C: .word 0x00002188
_02227650: .word 0x000022F0
	thumb_func_end ov42_02227604

	thumb_func_start ov42_02227654
ov42_02227654: ; 0x02227654
	push {r3, lr}
	ldr r3, _02227678 ; =ov42_02229BA0
	mov r2, #0
_0222765A:
	ldrh r1, [r3]
	cmp r0, r1
	bne _02227668
	ldr r0, _0222767C ; =ov42_02229BA2
	lsl r1, r2, #2
	ldrh r0, [r0, r1]
	pop {r3, pc}
_02227668:
	add r2, r2, #1
	add r3, r3, #4
	cmp r2, #0x99
	blt _0222765A
	bl GF_AssertFail
	mov r0, #0
	pop {r3, pc}
	.balign 4, 0
_02227678: .word ov42_02229BA0
_0222767C: .word ov42_02229BA2
	thumb_func_end ov42_02227654

	thumb_func_start ov42_02227680
ov42_02227680: ; 0x02227680
	cmp r1, #0
	bne _022276A4
	cmp r0, #0
	beq _0222769C
	cmp r0, #0x61
	beq _0222769C
	cmp r0, #0xee
	beq _0222769C
	cmp r0, #0xef
	beq _0222769C
	ldr r1, _022276A8 ; =0x00000127
	sub r0, r0, r1
	cmp r0, #1
	bhi _022276A0
_0222769C:
	mov r0, #1
	bx lr
_022276A0:
	mov r0, #0
	bx lr
_022276A4:
	mov r0, #1
	bx lr
	.balign 4, 0
_022276A8: .word 0x00000127
	thumb_func_end ov42_02227680

	thumb_func_start ov42_022276AC
ov42_022276AC: ; 0x022276AC
	push {r4, r5, r6, lr}
	add r4, r1, #0
	add r5, r0, #0
	add r6, r2, #0
	cmp r4, #2
	beq _022276C0
	cmp r4, #3
	beq _022276C0
	bl GF_AssertFail
_022276C0:
	sub r1, r4, #2
	cmp r6, #0
	bne _022276C8
	add r1, r1, #2
_022276C8:
	ldr r0, _022276D8 ; =_02229AF8
	ldrb r2, [r0, r1]
	mov r0, #1
	lsl r1, r5, #1
	lsl r0, r0, #8
	add r0, r1, r0
	add r0, r2, r0
	pop {r4, r5, r6, pc}
	.balign 4, 0
_022276D8: .word _02229AF8
	thumb_func_end ov42_022276AC

	thumb_func_start ov42_022276DC
ov42_022276DC: ; 0x022276DC
	push {r3, r4, r5, r6, r7, lr}
	add r5, r0, #0
	add r4, r1, #0
	mov r0, #0x38
	mul r0, r4
	add r0, r5, r0
	ldr r0, [r0, #0x14]
	add r6, r2, #0
	add r7, r3, #0
	cmp r0, #0
	beq _022276F6
	bl GF_AssertFail
_022276F6:
	ldr r0, [sp, #0x1c]
	cmp r0, #0
	bne _0222770E
	ldr r0, [sp, #0x18]
	add r1, r4, #0
	str r0, [sp]
	add r0, r5, #0
	add r2, r6, #0
	add r3, r7, #0
	bl ov42_02227720
	pop {r3, r4, r5, r6, r7, pc}
_0222770E:
	ldr r0, [sp, #0x18]
	add r1, r4, #0
	str r0, [sp]
	add r0, r5, #0
	add r2, r6, #0
	add r3, r7, #0
	bl ov42_02227874
	pop {r3, r4, r5, r6, r7, pc}
	thumb_func_end ov42_022276DC

	thumb_func_start ov42_02227720
ov42_02227720: ; 0x02227720
	push {r4, r5, r6, r7, lr}
	sub sp, #0x44
	add r5, r0, #0
	ldr r0, [sp, #0x58]
	add r6, r5, #0
	str r0, [sp, #0x40]
	mov r0, #2
	lsl r0, r0, #8
	add r0, r1, r0
	str r0, [sp, #0x38]
	lsl r0, r1, #1
	str r0, [sp, #0x3c]
	add r4, r1, #0
	mov r0, #0x38
	mul r4, r0
	ldr r0, [sp, #0x38]
	str r3, [sp, #0x30]
	str r0, [sp]
	add r0, r2, #0
	str r0, [sp, #4]
	ldr r0, [sp, #0x40]
	str r2, [sp, #0x2c]
	ldr r2, [sp, #0x3c]
	ldr r1, _0222786C ; =0x000022F0
	str r0, [sp, #8]
	ldr r0, [r5, r1]
	add r1, #0x14
	ldr r1, [r5, r1]
	add r6, #0x14
	add r2, #0xe
	mov r3, #0
	bl AddCharResObjFromOpenNarc
	str r0, [r6, r4]
	ldr r0, [sp, #0x38]
	ldr r2, [sp, #0x3c]
	str r0, [sp]
	ldr r0, [sp, #0x2c]
	add r7, r5, #0
	str r0, [sp, #4]
	mov r0, #1
	str r0, [sp, #8]
	ldr r0, [sp, #0x40]
	add r2, #0xd
	ldr r1, _02227870 ; =0x000022F4
	str r0, [sp, #0xc]
	ldr r0, [r5, r1]
	add r1, #0x10
	ldr r1, [r5, r1]
	add r7, #0x18
	mov r3, #0
	str r2, [sp, #0x3c]
	bl AddPlttResObjFromOpenNarc
	str r0, [r7, r4]
	ldr r0, [r6, r4]
	bl GF2DGfxResObj_GetCharDataPtr
	mov r1, #6
	ldr r2, [r0, #0x10]
	lsl r1, r1, #0xa
	cmp r2, r1
	beq _022277A4
	mov r1, #1
	str r1, [sp, #0x34]
	b _022277A8
_022277A4:
	mov r1, #0
	str r1, [sp, #0x34]
_022277A8:
	ldr r1, [sp, #0x30]
	cmp r1, #2
	bne _022277B6
	mov r1, #2
	lsl r1, r1, #0xa
	str r1, [r0, #0x10]
	b _022277CE
_022277B6:
	cmp r1, #1
	bne _022277CE
	ldr r1, [sp, #0x34]
	cmp r1, #0
	beq _022277C8
	mov r1, #5
	lsl r1, r1, #0xa
	str r1, [r0, #0x10]
	b _022277CE
_022277C8:
	mov r1, #6
	lsl r1, r1, #0xa
	str r1, [r0, #0x10]
_022277CE:
	ldr r0, [r6, r4]
	bl sub_0200AE18
	cmp r0, #0
	bne _022277DC
	bl GF_AssertFail
_022277DC:
	ldr r0, [r7, r4]
	bl sub_0200B00C
	cmp r0, #0
	bne _022277EA
	bl GF_AssertFail
_022277EA:
	ldr r0, [r5, #4]
	cmp r0, #0
	beq _022277FA
	ldr r1, [r7, r4]
	add r0, r5, #0
	mov r2, #1
	bl ov42_02227E78
_022277FA:
	ldr r0, [r6, r4]
	bl sub_0200A740
	ldr r0, [r7, r4]
	bl sub_0200A740
	ldr r0, [sp, #0x2c]
	add r1, r5, r4
	str r0, [r1, #0x1c]
	ldr r0, [sp, #0x30]
	ldr r2, [sp, #0x34]
	str r0, [r1, #0x20]
	mov r0, #0
	str r0, [r1, #0x10]
	ldr r0, [sp, #0x30]
	mov r1, #2
	bl ov42_022276AC
	add r6, r0, #0
	ldr r0, [sp, #0x30]
	ldr r2, [sp, #0x34]
	mov r1, #3
	bl ov42_022276AC
	str r0, [sp]
	mov r0, #0
	mvn r0, r0
	str r0, [sp, #4]
	str r0, [sp, #8]
	mov r0, #0
	str r0, [sp, #0xc]
	ldr r2, _0222786C ; =0x000022F0
	str r0, [sp, #0x10]
	ldr r1, [r5, r2]
	add r3, r6, #0
	str r1, [sp, #0x14]
	add r1, r2, #4
	ldr r1, [r5, r1]
	str r1, [sp, #0x18]
	add r1, r2, #0
	add r1, #8
	ldr r1, [r5, r1]
	add r2, #0xc
	str r1, [sp, #0x1c]
	ldr r1, [r5, r2]
	add r5, #0x24
	str r1, [sp, #0x20]
	ldr r1, [sp, #0x38]
	str r0, [sp, #0x24]
	str r0, [sp, #0x28]
	add r0, r5, r4
	add r2, r1, #0
	bl CreateSpriteResourcesHeader
	add sp, #0x44
	pop {r4, r5, r6, r7, pc}
	nop
_0222786C: .word 0x000022F0
_02227870: .word 0x000022F4
	thumb_func_end ov42_02227720

	thumb_func_start ov42_02227874
ov42_02227874: ; 0x02227874
	push {r4, r5, r6, r7, lr}
	sub sp, #0x3c
	add r7, r1, #0
	add r5, r0, #0
	sub r0, r7, #2
	str r0, [sp, #0x38]
	mov r0, #2
	lsl r0, r0, #8
	add r0, r7, r0
	str r0, [sp, #0x34]
	mov r0, #0x38
	add r4, r7, #0
	mul r4, r0
	ldr r0, [sp, #0x34]
	add r6, r5, #0
	str r0, [sp]
	add r0, r2, #0
	str r0, [sp, #4]
	ldr r0, [sp, #0x50]
	str r2, [sp, #0x2c]
	str r3, [sp, #0x30]
	ldr r1, _02227940 ; =0x000022F0
	str r0, [sp, #8]
	ldr r0, [r5, r1]
	add r1, #0x18
	ldr r1, [r5, r1]
	add r6, #0x14
	sub r2, r7, #1
	mov r3, #0
	bl AddCharResObjFromOpenNarc
	str r0, [r6, r4]
	ldr r0, [sp, #0x30]
	cmp r0, #2
	bne _022278C8
	ldr r0, [r6, r4]
	bl GF2DGfxResObj_GetCharDataPtr
	mov r1, #2
	lsl r1, r1, #0xa
	str r1, [r0, #0x10]
	b _022278D8
_022278C8:
	cmp r0, #1
	bne _022278D8
	ldr r0, [r6, r4]
	bl GF2DGfxResObj_GetCharDataPtr
	mov r1, #5
	lsl r1, r1, #0xa
	str r1, [r0, #0x10]
_022278D8:
	ldr r0, [r6, r4]
	bl sub_0200AE18
	cmp r0, #0
	bne _022278E6
	bl GF_AssertFail
_022278E6:
	ldr r0, [r6, r4]
	bl sub_0200A740
	mov r0, #0
	add r2, r5, r4
	ldr r1, [sp, #0x2c]
	str r0, [r2, #0x18]
	str r1, [r2, #0x1c]
	ldr r1, [sp, #0x30]
	mov r6, #1
	str r1, [r2, #0x20]
	str r6, [r2, #0x10]
	mov r2, #6
	ldr r1, [sp, #0x38]
	lsl r2, r2, #6
	add r3, r1, r2
	str r3, [sp]
	sub r1, r6, #2
	str r1, [sp, #4]
	str r1, [sp, #8]
	str r0, [sp, #0xc]
	ldr r2, _02227940 ; =0x000022F0
	str r0, [sp, #0x10]
	ldr r1, [r5, r2]
	str r1, [sp, #0x14]
	add r1, r2, #4
	ldr r1, [r5, r1]
	str r1, [sp, #0x18]
	add r1, r2, #0
	add r1, #8
	ldr r1, [r5, r1]
	add r2, #0xc
	str r1, [sp, #0x1c]
	ldr r1, [r5, r2]
	add r5, #0x24
	str r1, [sp, #0x20]
	str r0, [sp, #0x24]
	str r0, [sp, #0x28]
	ldr r1, [sp, #0x34]
	ldr r2, _02227944 ; =0x00000402
	add r0, r5, r4
	bl CreateSpriteResourcesHeader
	add sp, #0x3c
	pop {r4, r5, r6, r7, pc}
	.balign 4, 0
_02227940: .word 0x000022F0
_02227944: .word 0x00000402
	thumb_func_end ov42_02227874

	thumb_func_start ov42_02227948
ov42_02227948: ; 0x02227948
	push {r4, r5, r6, lr}
	add r6, r1, #0
	add r5, r0, #0
	mov r0, #0x38
	add r4, r6, #0
	mul r4, r0
	add r0, r5, r4
	ldr r0, [r0, #0x14]
	cmp r0, #0
	bne _02227960
	bl GF_AssertFail
_02227960:
	add r0, r5, r4
	ldr r0, [r0, #0x10]
	cmp r0, #0
	bne _02227972
	add r0, r5, #0
	add r1, r6, #0
	bl ov42_0222797C
	pop {r4, r5, r6, pc}
_02227972:
	add r0, r5, #0
	add r1, r6, #0
	bl ov42_022279C0
	pop {r4, r5, r6, pc}
	thumb_func_end ov42_02227948

	thumb_func_start ov42_0222797C
ov42_0222797C: ; 0x0222797C
	push {r3, r4, r5, r6, r7, lr}
	add r5, r0, #0
	add r6, r5, #0
	mov r0, #0x38
	add r4, r1, #0
	mul r4, r0
	add r6, #0x14
	ldr r0, [r6, r4]
	bl sub_0200AEB0
	add r7, r5, #0
	add r7, #0x18
	ldr r0, [r7, r4]
	bl sub_0200B0A8
	ldr r0, _022279B8 ; =0x000022F0
	ldr r1, [r6, r4]
	ldr r0, [r5, r0]
	bl DestroySingle2DGfxResObj
	ldr r0, _022279BC ; =0x000022F4
	ldr r1, [r7, r4]
	ldr r0, [r5, r0]
	bl DestroySingle2DGfxResObj
	mov r0, #0
	str r0, [r6, r4]
	str r0, [r7, r4]
	pop {r3, r4, r5, r6, r7, pc}
	nop
_022279B8: .word 0x000022F0
_022279BC: .word 0x000022F4
	thumb_func_end ov42_0222797C

	thumb_func_start ov42_022279C0
ov42_022279C0: ; 0x022279C0
	push {r4, r5, r6, lr}
	add r6, r0, #0
	add r5, r6, #0
	mov r0, #0x38
	add r4, r1, #0
	add r5, #0x14
	mul r4, r0
	ldr r0, [r5, r4]
	bl sub_0200AEB0
	ldr r0, _022279E4 ; =0x000022F0
	ldr r1, [r5, r4]
	ldr r0, [r6, r0]
	bl DestroySingle2DGfxResObj
	mov r0, #0
	str r0, [r5, r4]
	pop {r4, r5, r6, pc}
	.balign 4, 0
_022279E4: .word 0x000022F0
	thumb_func_end ov42_022279C0

	thumb_func_start ov42_022279E8
ov42_022279E8: ; 0x022279E8
	mov r2, #0x38
	mul r2, r1
	add r0, r0, r2
	ldr r0, [r0, #0x14]
	cmp r0, #0
	beq _022279F8
	mov r0, #1
	bx lr
_022279F8:
	mov r0, #0
	bx lr
	thumb_func_end ov42_022279E8

	thumb_func_start ov42_022279FC
ov42_022279FC: ; 0x022279FC
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x38
	add r6, r0, #0
	ldr r0, _02227AF0 ; =0x000022B8
	str r1, [sp, #0x2c]
	ldr r0, [r6, r0]
	add r7, r2, #0
	cmp r0, #0
	beq _02227A12
	bl GF_AssertFail
_02227A12:
	mov r4, #0
	ldr r5, _02227AF4 ; =ov42_02229AFC
	str r4, [sp, #0x34]
_02227A18:
	ldrb r1, [r5]
	add r0, r6, #0
	bl ov42_022279E8
	cmp r0, #1
	bne _02227A30
	ldr r0, _02227AF4 ; =ov42_02229AFC
	ldrb r0, [r0, r4]
	str r0, [sp, #0x30]
	mov r0, #1
	str r0, [sp, #0x34]
	b _02227A38
_02227A30:
	add r4, r4, #1
	add r5, r5, #1
	cmp r4, #6
	blt _02227A18
_02227A38:
	ldr r0, [sp, #0x34]
	cmp r0, #1
	beq _02227A42
	bl GF_AssertFail
_02227A42:
	mov r5, #0
	add r4, r6, #0
_02227A46:
	mov r0, #0x12
	lsl r0, r0, #4
	str r0, [sp]
	add r0, r5, #2
	str r0, [sp, #4]
	ldr r0, _02227AF8 ; =0x000022F8
	str r7, [sp, #8]
	ldr r1, _02227AFC ; =0x00002304
	ldr r0, [r4, r0]
	ldr r1, [r6, r1]
	add r2, r5, #0
	mov r3, #0
	bl AddCellOrAnimResObjFromOpenNarc
	mov r1, #0x8b
	lsl r1, r1, #6
	str r0, [r4, r1]
	add r5, r5, #1
	add r4, r4, #4
	cmp r5, #2
	blt _02227A46
	mov r0, #0x12
	lsl r0, r0, #4
	str r0, [sp]
	ldr r0, [sp, #0x2c]
	add r1, #0x30
	str r0, [sp, #4]
	str r7, [sp, #8]
	ldr r0, [r6, r1]
	add r1, #0x14
	ldr r1, [r6, r1]
	mov r2, #2
	mov r3, #0
	bl AddCharResObjFromOpenNarc
	ldr r1, _02227AF0 ; =0x000022B8
	str r0, [r6, r1]
	ldr r0, [r6, r1]
	bl sub_0200AE18
	cmp r0, #0
	bne _02227A9E
	bl GF_AssertFail
_02227A9E:
	ldr r0, _02227AF0 ; =0x000022B8
	ldr r0, [r6, r0]
	bl sub_0200A740
	mov r1, #0x12
	lsl r1, r1, #4
	mov r0, #0
	str r1, [sp]
	mvn r0, r0
	str r0, [sp, #4]
	str r0, [sp, #8]
	mov r0, #0
	str r0, [sp, #0xc]
	ldr r3, _02227B00 ; =0x000022F0
	str r0, [sp, #0x10]
	ldr r2, [r6, r3]
	str r2, [sp, #0x14]
	add r2, r3, #4
	ldr r2, [r6, r2]
	str r2, [sp, #0x18]
	add r2, r3, #0
	add r2, #8
	ldr r2, [r6, r2]
	str r2, [sp, #0x1c]
	add r2, r3, #0
	add r2, #0xc
	ldr r2, [r6, r2]
	sub r3, #0x28
	str r2, [sp, #0x20]
	str r0, [sp, #0x24]
	str r0, [sp, #0x28]
	add r0, r6, r3
	add r3, r1, #0
	ldr r2, [sp, #0x30]
	add r3, #0xe0
	add r2, r2, r3
	add r3, r1, #0
	bl CreateSpriteResourcesHeader
	add sp, #0x38
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
_02227AF0: .word 0x000022B8
_02227AF4: .word ov42_02229AFC
_02227AF8: .word 0x000022F8
_02227AFC: .word 0x00002304
_02227B00: .word 0x000022F0
	thumb_func_end ov42_022279FC

	thumb_func_start ov42_02227B04
ov42_02227B04: ; 0x02227B04
	push {r3, r4, r5, r6, r7, lr}
	add r5, r0, #0
	ldr r0, _02227B50 ; =0x000022B8
	add r0, r5, r0
	bl ov42_02227B5C
	cmp r0, #1
	bne _02227B4C
	ldr r0, _02227B50 ; =0x000022B8
	ldr r0, [r5, r0]
	bl sub_0200AEB0
	ldr r1, _02227B54 ; =0x000022F0
	ldr r0, [r5, r1]
	sub r1, #0x38
	ldr r1, [r5, r1]
	bl DestroySingle2DGfxResObj
	ldr r0, _02227B50 ; =0x000022B8
	mov r4, #0
	add r7, r0, #0
	str r4, [r5, r0]
	add r6, r4, #0
	add r7, #8
_02227B34:
	ldr r0, _02227B58 ; =0x000022F8
	mov r1, #0x8b
	lsl r1, r1, #6
	ldr r0, [r5, r0]
	ldr r1, [r5, r1]
	bl DestroySingle2DGfxResObj
	str r6, [r5, r7]
	add r4, r4, #1
	add r5, r5, #4
	cmp r4, #2
	blt _02227B34
_02227B4C:
	pop {r3, r4, r5, r6, r7, pc}
	nop
_02227B50: .word 0x000022B8
_02227B54: .word 0x000022F0
_02227B58: .word 0x000022F8
	thumb_func_end ov42_02227B04

	thumb_func_start ov42_02227B5C
ov42_02227B5C: ; 0x02227B5C
	ldr r0, [r0]
	cmp r0, #0
	beq _02227B66
	mov r0, #1
	bx lr
_02227B66:
	mov r0, #0
	bx lr
	.balign 4, 0
	thumb_func_end ov42_02227B5C

	thumb_func_start ov42_02227B6C
ov42_02227B6C: ; 0x02227B6C
	push {r4, r5, r6, lr}
	sub sp, #0x10
	add r5, r0, #0
	mov r0, #0x23
	lsl r0, r0, #8
	ldr r0, [r5, r0]
	add r4, r1, #0
	add r6, r2, #0
	cmp r0, #0
	beq _02227B84
	bl GF_AssertFail
_02227B84:
	ldr r0, _02227BD8 ; =0x00000402
	mov r2, #0
	str r0, [sp]
	str r4, [sp, #4]
	mov r0, #8
	str r0, [sp, #8]
	ldr r1, _02227BDC ; =0x000022F4
	str r6, [sp, #0xc]
	ldr r0, [r5, r1]
	add r1, #0x14
	ldr r1, [r5, r1]
	add r3, r2, #0
	bl AddPlttResObjFromOpenNarc
	mov r1, #0x23
	lsl r1, r1, #8
	str r0, [r5, r1]
	ldr r0, [r5, r1]
	bl sub_0200B00C
	cmp r0, #0
	bne _02227BB4
	bl GF_AssertFail
_02227BB4:
	ldr r0, [r5, #4]
	cmp r0, #0
	beq _02227BC8
	mov r1, #0x23
	lsl r1, r1, #8
	ldr r1, [r5, r1]
	add r0, r5, #0
	mov r2, #8
	bl ov42_02227E78
_02227BC8:
	mov r0, #0x23
	lsl r0, r0, #8
	ldr r0, [r5, r0]
	bl sub_0200A740
	add sp, #0x10
	pop {r4, r5, r6, pc}
	nop
_02227BD8: .word 0x00000402
_02227BDC: .word 0x000022F4
	thumb_func_end ov42_02227B6C

	thumb_func_start ov42_02227BE0
ov42_02227BE0: ; 0x02227BE0
	push {r4, lr}
	add r4, r0, #0
	mov r0, #0x23
	lsl r0, r0, #8
	ldr r0, [r4, r0]
	cmp r0, #0
	bne _02227BF2
	bl GF_AssertFail
_02227BF2:
	mov r0, #0x23
	lsl r0, r0, #8
	ldr r0, [r4, r0]
	bl sub_0200B0A8
	ldr r1, _02227C14 ; =0x000022F4
	ldr r0, [r4, r1]
	add r1, #0xc
	ldr r1, [r4, r1]
	bl DestroySingle2DGfxResObj
	mov r0, #0x23
	mov r1, #0
	lsl r0, r0, #8
	str r1, [r4, r0]
	pop {r4, pc}
	nop
_02227C14: .word 0x000022F4
	thumb_func_end ov42_02227BE0

	thumb_func_start ov42_02227C18
ov42_02227C18: ; 0x02227C18
	push {r4, r5, r6, r7, lr}
	sub sp, #0x14
	add r5, r0, #0
	ldr r0, _02227C94 ; =0x000021B8
	str r1, [sp, #0xc]
	ldr r0, [r5, r0]
	cmp r0, #0
	beq _02227C2C
	bl GF_AssertFail
_02227C2C:
	mov r0, #0x11
	mov r4, #0
	str r0, [sp, #0x10]
	add r6, r5, #0
	mov r7, #0x12
_02227C36:
	mov r0, #6
	lsl r0, r0, #6
	add r0, r4, r0
	str r0, [sp]
	mov r0, #2
	str r0, [sp, #4]
	ldr r0, [sp, #0xc]
	ldr r1, _02227C98 ; =0x00002308
	str r0, [sp, #8]
	ldr r0, _02227C9C ; =0x000022F8
	ldr r1, [r5, r1]
	ldr r0, [r5, r0]
	ldr r2, [sp, #0x10]
	mov r3, #0
	bl AddCellOrAnimResObjFromOpenNarc
	ldr r1, _02227C94 ; =0x000021B8
	add r2, r7, #0
	str r0, [r6, r1]
	mov r0, #6
	lsl r0, r0, #6
	add r0, r4, r0
	str r0, [sp]
	mov r0, #3
	str r0, [sp, #4]
	ldr r0, [sp, #0xc]
	ldr r1, _02227C98 ; =0x00002308
	str r0, [sp, #8]
	ldr r0, _02227CA0 ; =0x000022FC
	ldr r1, [r5, r1]
	ldr r0, [r5, r0]
	mov r3, #0
	bl AddCellOrAnimResObjFromOpenNarc
	ldr r1, _02227CA4 ; =0x000021BC
	add r4, r4, #1
	str r0, [r6, r1]
	ldr r0, [sp, #0x10]
	add r6, #0x10
	add r0, r0, #2
	add r7, r7, #2
	str r0, [sp, #0x10]
	cmp r4, #0x10
	blt _02227C36
	add sp, #0x14
	pop {r4, r5, r6, r7, pc}
	nop
_02227C94: .word 0x000021B8
_02227C98: .word 0x00002308
_02227C9C: .word 0x000022F8
_02227CA0: .word 0x000022FC
_02227CA4: .word 0x000021BC
	thumb_func_end ov42_02227C18

	thumb_func_start ov42_02227CA8
ov42_02227CA8: ; 0x02227CA8
	push {r3, r4, r5, r6, r7, lr}
	add r5, r0, #0
	ldr r0, _02227CE8 ; =0x000021B8
	ldr r0, [r5, r0]
	cmp r0, #0
	bne _02227CB8
	bl GF_AssertFail
_02227CB8:
	mov r6, #0
	add r4, r5, #0
	add r7, r6, #0
_02227CBE:
	ldr r0, _02227CEC ; =0x000022F8
	ldr r1, _02227CE8 ; =0x000021B8
	ldr r0, [r5, r0]
	ldr r1, [r4, r1]
	bl DestroySingle2DGfxResObj
	ldr r0, _02227CF0 ; =0x000022FC
	ldr r1, _02227CF4 ; =0x000021BC
	ldr r0, [r5, r0]
	ldr r1, [r4, r1]
	bl DestroySingle2DGfxResObj
	ldr r0, _02227CE8 ; =0x000021B8
	add r6, r6, #1
	str r7, [r4, r0]
	add r0, r0, #4
	str r7, [r4, r0]
	add r4, #0x10
	cmp r6, #0x10
	blt _02227CBE
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
_02227CE8: .word 0x000021B8
_02227CEC: .word 0x000022F8
_02227CF0: .word 0x000022FC
_02227CF4: .word 0x000021BC
	thumb_func_end ov42_02227CA8

	thumb_func_start ov42_02227CF8
ov42_02227CF8: ; 0x02227CF8
	push {r4, r5, r6, lr}
	add r5, r0, #0
	ldr r0, [r5, #0xc]
	mov r6, #0
	cmp r0, #0
	bls _02227D26
	add r4, r6, #0
_02227D06:
	ldr r0, [r5, #8]
	add r0, r0, r4
	bl ov42_02227E68
	cmp r0, #1
	bne _02227D1C
	mov r0, #0x18
	ldr r1, [r5, #8]
	mul r0, r6
	add r0, r1, r0
	pop {r4, r5, r6, pc}
_02227D1C:
	ldr r0, [r5, #0xc]
	add r6, r6, #1
	add r4, #0x18
	cmp r6, r0
	blo _02227D06
_02227D26:
	bl GF_AssertFail
	mov r0, #0
	pop {r4, r5, r6, pc}
	.balign 4, 0
	thumb_func_end ov42_02227CF8

	thumb_func_start ov42_02227D30
ov42_02227D30: ; 0x02227D30
	push {r3, lr}
	ldr r1, [r0, #0xc]
	lsl r2, r1, #2
	ldr r1, _02227D44 ; =ov42_02229B48
	ldr r1, [r1, r2]
	cmp r1, #0
	beq _02227D40
	blx r1
_02227D40:
	pop {r3, pc}
	nop
_02227D44: .word ov42_02229B48
	thumb_func_end ov42_02227D30

	thumb_func_start ov42_02227D48
ov42_02227D48: ; 0x02227D48
	ldr r3, _02227D50 ; =Sprite_TryChangeAnimSeq
	str r1, [r0, #0x10]
	ldr r0, [r0, #4]
	bx r3
	.balign 4, 0
_02227D50: .word Sprite_TryChangeAnimSeq
	thumb_func_end ov42_02227D48

	thumb_func_start ov42_02227D54
ov42_02227D54: ; 0x02227D54
	ldr r3, _02227D60 ; =Sprite_TryChangeAnimSeq
	str r1, [r0, #0x10]
	ldr r0, [r0, #4]
	mov r1, #4
	bx r3
	nop
_02227D60: .word Sprite_TryChangeAnimSeq
	thumb_func_end ov42_02227D54

	thumb_func_start ov42_02227D64
ov42_02227D64: ; 0x02227D64
	ldr r3, _02227D70 ; =Sprite_TryChangeAnimSeq
	str r1, [r0, #0x10]
	ldr r0, [r0, #4]
	add r1, r1, #5
	bx r3
	nop
_02227D70: .word Sprite_TryChangeAnimSeq
	thumb_func_end ov42_02227D64

	thumb_func_start ov42_02227D74
ov42_02227D74: ; 0x02227D74
	push {r3, r4, r5, lr}
	add r5, r0, #0
	add r4, r1, #0
	ldr r1, [r5, #0x10]
	ldr r0, [r5, #4]
	add r1, r1, #5
	bl Sprite_TryChangeAnimSeq
	str r4, [r5, #0x10]
	pop {r3, r4, r5, pc}
	thumb_func_end ov42_02227D74

	thumb_func_start ov42_02227D88
ov42_02227D88: ; 0x02227D88
	ldr r3, _02227D94 ; =Sprite_TryChangeAnimSeq
	str r1, [r0, #0x10]
	ldr r0, [r0, #4]
	add r1, #9
	bx r3
	nop
_02227D94: .word Sprite_TryChangeAnimSeq
	thumb_func_end ov42_02227D88

	thumb_func_start ov42_02227D98
ov42_02227D98: ; 0x02227D98
	ldr r3, _02227DA4 ; =Sprite_TryChangeAnimSeq
	str r1, [r0, #0x10]
	ldr r0, [r0, #4]
	add r1, r1, #5
	bx r3
	nop
_02227DA4: .word Sprite_TryChangeAnimSeq
	thumb_func_end ov42_02227D98

	thumb_func_start ov42_02227DA8
ov42_02227DA8: ; 0x02227DA8
	ldr r3, _02227DB4 ; =Sprite_TryChangeAnimSeq
	str r1, [r0, #0x10]
	ldr r0, [r0, #4]
	add r1, r1, #5
	bx r3
	nop
_02227DB4: .word Sprite_TryChangeAnimSeq
	thumb_func_end ov42_02227DA8

	thumb_func_start ov42_02227DB8
ov42_02227DB8: ; 0x02227DB8
	ldr r3, _02227DC4 ; =Sprite_TryChangeAnimSeq
	str r1, [r0, #0x10]
	ldr r0, [r0, #4]
	add r1, r1, #5
	bx r3
	nop
_02227DC4: .word Sprite_TryChangeAnimSeq
	thumb_func_end ov42_02227DB8

	thumb_func_start ov42_02227DC8
ov42_02227DC8: ; 0x02227DC8
	ldr r3, _02227DD4 ; =Sprite_TryChangeAnimSeq
	str r1, [r0, #0x10]
	ldr r0, [r0, #4]
	add r1, r1, #5
	bx r3
	nop
_02227DD4: .word Sprite_TryChangeAnimSeq
	thumb_func_end ov42_02227DC8

	thumb_func_start ov42_02227DD8
ov42_02227DD8: ; 0x02227DD8
	ldr r3, _02227DE4 ; =Sprite_UpdateAnim
	mov r1, #2
	ldr r0, [r0, #4]
	lsl r1, r1, #0xc
	bx r3
	nop
_02227DE4: .word Sprite_UpdateAnim
	thumb_func_end ov42_02227DD8

	thumb_func_start ov42_02227DE8
ov42_02227DE8: ; 0x02227DE8
	ldr r3, _02227DF4 ; =Sprite_UpdateAnim
	mov r1, #2
	ldr r0, [r0, #4]
	lsl r1, r1, #0xc
	bx r3
	nop
_02227DF4: .word Sprite_UpdateAnim
	thumb_func_end ov42_02227DE8

	thumb_func_start ov42_02227DF8
ov42_02227DF8: ; 0x02227DF8
	push {r3, lr}
	add r1, r0, #0
	mov r0, #0x14
	ldrsh r0, [r1, r0]
	cmp r0, #0
	ldr r0, [r1, #4]
	bne _02227E0E
	mov r1, #1
	bl Sprite_SetAnimationFrame
	pop {r3, pc}
_02227E0E:
	ldr r1, [r1, #0x10]
	add r1, r1, #5
	bl Sprite_SetAnimCtrlSeq
	pop {r3, pc}
	thumb_func_end ov42_02227DF8

	thumb_func_start ov42_02227E18
ov42_02227E18: ; 0x02227E18
	ldr r3, _02227E24 ; =Sprite_UpdateAnim
	mov r1, #2
	ldr r0, [r0, #4]
	lsl r1, r1, #0xc
	bx r3
	nop
_02227E24: .word Sprite_UpdateAnim
	thumb_func_end ov42_02227E18

	thumb_func_start ov42_02227E28
ov42_02227E28: ; 0x02227E28
	ldr r3, _02227E34 ; =Sprite_UpdateAnim
	mov r1, #1
	ldr r0, [r0, #4]
	lsl r1, r1, #0xc
	bx r3
	nop
_02227E34: .word Sprite_UpdateAnim
	thumb_func_end ov42_02227E28

	thumb_func_start ov42_02227E38
ov42_02227E38: ; 0x02227E38
	ldr r3, _02227E44 ; =Sprite_UpdateAnim
	mov r1, #1
	ldr r0, [r0, #4]
	lsl r1, r1, #0xc
	bx r3
	nop
_02227E44: .word Sprite_UpdateAnim
	thumb_func_end ov42_02227E38

	thumb_func_start ov42_02227E48
ov42_02227E48: ; 0x02227E48
	ldr r3, _02227E54 ; =Sprite_UpdateAnim
	mov r1, #2
	ldr r0, [r0, #4]
	lsl r1, r1, #0xe
	bx r3
	nop
_02227E54: .word Sprite_UpdateAnim
	thumb_func_end ov42_02227E48

	thumb_func_start ov42_02227E58
ov42_02227E58: ; 0x02227E58
	ldr r3, _02227E64 ; =Sprite_UpdateAnim
	mov r1, #6
	ldr r0, [r0, #4]
	lsl r1, r1, #0xc
	bx r3
	nop
_02227E64: .word Sprite_UpdateAnim
	thumb_func_end ov42_02227E58

	thumb_func_start ov42_02227E68
ov42_02227E68: ; 0x02227E68
	ldr r0, [r0, #4]
	cmp r0, #0
	bne _02227E72
	mov r0, #1
	bx lr
_02227E72:
	mov r0, #0
	bx lr
	.balign 4, 0
	thumb_func_end ov42_02227E68

	thumb_func_start ov42_02227E78
ov42_02227E78: ; 0x02227E78
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #8
	add r6, r1, #0
	add r5, r0, #0
	add r0, r6, #0
	add r4, r2, #0
	bl GF2DGfxResObj_GetPlttDataPtr
	str r0, [sp, #4]
	add r0, r6, #0
	mov r1, #1
	bl SpriteTransfer_GetPlttOffset
	add r7, r0, #0
	add r0, r6, #0
	mov r1, #2
	bl SpriteTransfer_GetPlttOffset
	add r6, r0, #0
	mov r0, #0
	mvn r0, r0
	cmp r7, r0
	beq _02227EBC
	lsl r0, r4, #0x15
	lsr r0, r0, #0x10
	str r0, [sp]
	ldr r1, [sp, #4]
	lsl r3, r7, #0x14
	ldr r0, [r5, #4]
	ldr r1, [r1, #0xc]
	mov r2, #2
	lsr r3, r3, #0x10
	bl PaletteData_LoadPalette
_02227EBC:
	mov r0, #0
	mvn r0, r0
	cmp r6, r0
	beq _02227EDA
	lsl r0, r4, #0x15
	lsr r0, r0, #0x10
	str r0, [sp]
	ldr r1, [sp, #4]
	lsl r3, r6, #0x14
	ldr r0, [r5, #4]
	ldr r1, [r1, #0xc]
	mov r2, #3
	lsr r3, r3, #0x10
	bl PaletteData_LoadPalette
_02227EDA:
	add sp, #8
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
	thumb_func_end ov42_02227E78

	thumb_func_start ov42_02227EE0
ov42_02227EE0: ; 0x02227EE0
	push {r3, r4, r5, r6, r7, lr}
	add r7, r2, #0
	add r5, r0, #0
	add r6, r1, #0
	add r0, r7, #0
	mov r1, #8
	bl Heap_Alloc
	add r4, r0, #0
	bne _02227EF8
	bl GF_AssertFail
_02227EF8:
	strh r5, [r4]
	strh r6, [r4, #2]
	ldrh r2, [r4]
	ldrh r1, [r4, #2]
	add r0, r7, #0
	mul r1, r2
	lsl r1, r1, #2
	bl Heap_Alloc
	str r0, [r4, #4]
	cmp r0, #0
	bne _02227F14
	bl GF_AssertFail
_02227F14:
	ldrh r3, [r4]
	ldrh r2, [r4, #2]
	ldr r0, [r4, #4]
	mov r1, #0
	mul r2, r3
	lsl r2, r2, #2
	bl memset
	add r0, r4, #0
	pop {r3, r4, r5, r6, r7, pc}
	thumb_func_end ov42_02227EE0

	thumb_func_start ov42_02227F28
ov42_02227F28: ; 0x02227F28
	push {r4, lr}
	add r4, r0, #0
	bne _02227F32
	bl GF_AssertFail
_02227F32:
	ldr r0, [r4, #4]
	bl Heap_Free
	add r0, r4, #0
	bl Heap_Free
	pop {r4, pc}
	thumb_func_end ov42_02227F28

	thumb_func_start ov42_02227F40
ov42_02227F40: ; 0x02227F40
	ldrh r0, [r0]
	bx lr
	thumb_func_end ov42_02227F40

	thumb_func_start ov42_02227F44
ov42_02227F44: ; 0x02227F44
	ldrh r0, [r0, #2]
	bx lr
	thumb_func_end ov42_02227F44

	thumb_func_start ov42_02227F48
ov42_02227F48: ; 0x02227F48
	push {r3, r4, r5, lr}
	add r4, r0, #0
	add r5, r1, #0
	cmp r4, #0
	bne _02227F56
	bl GF_AssertFail
_02227F56:
	ldrh r3, [r4]
	ldrh r2, [r4, #2]
	ldr r0, [r4, #4]
	add r1, r5, #0
	mul r2, r3
	lsl r2, r2, #2
	bl memcpy
	pop {r3, r4, r5, pc}
	thumb_func_end ov42_02227F48



	.rodata

_02229AF8:
	.byte 0x00, 0x00, 0x01, 0x01

ov42_02229AFC: ; 0x02229AFC
	.byte 0x00, 0x01, 0x95, 0x96
	.byte 0x97, 0x98, 0x00, 0x00

ov42_02229B04: ; 0x02229B04
	.byte 0x01, 0xFF, 0x08, 0x02, 0x04, 0x10, 0x10, 0x02, 0x04, 0x00, 0x00, 0x00

ov42_02229B10: ; 0x02229B10
	.byte 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11

ov42_02229B20: ; 0x02229B20
	.word ov42_02227D48
	.word ov42_02227D54
	.word ov42_02227D64
	.word ov42_02227D74
	.word ov42_02227D88
	.word ov42_02227D98
	.word ov42_02227DA8
	.word ov42_02227DB8
	.word ov42_02227DC8
	.word 0

ov42_02229B48: ; 0x02229B48
	.word 0
	.word ov42_02227DD8
	.word ov42_02227DE8
	.word ov42_02227DF8
	.word ov42_02227E18
	.word ov42_02227E28
	.word ov42_02227E38
	.word ov42_02227E48
	.word ov42_02227E58
	.word 0

ov42_02229B70: ; 0x02229B70
	.byte 0x03, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00
	.byte 0x05, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00
	.byte 0x09, 0x00, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x00, 0x0B, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00

ov42_02229BA0: ; 0x02229BA0
	.byte 0x00, 0x00

ov42_02229BA2: ; 0x02229BA2
	.byte 0x00, 0x00, 0x03, 0x00, 0x02, 0x00, 0x05, 0x00, 0x03, 0x00, 0x0B, 0x00, 0x04, 0x00
	.byte 0x1F, 0x00, 0x05, 0x00, 0x32, 0x00, 0x06, 0x00, 0x33, 0x00, 0x07, 0x00, 0x3E, 0x00, 0x08, 0x00
	.byte 0x46, 0x00, 0x09, 0x00, 0x61, 0x00, 0x01, 0x00, 0x06, 0x00, 0x0A, 0x00, 0x07, 0x00, 0x0B, 0x00
	.byte 0x0D, 0x00, 0x0C, 0x00, 0x0E, 0x00, 0x0D, 0x00, 0x23, 0x00, 0x0E, 0x00, 0x25, 0x00, 0x0F, 0x00
	.byte 0x2A, 0x00, 0x10, 0x00, 0x3F, 0x00, 0x11, 0x00, 0x04, 0x00, 0x12, 0x00, 0x08, 0x00, 0x13, 0x00
	.byte 0x34, 0x00, 0x14, 0x00, 0x35, 0x00, 0x15, 0x00, 0x02, 0x00, 0x16, 0x00, 0x0F, 0x00, 0x17, 0x00
	.byte 0x10, 0x00, 0x18, 0x00, 0x3B, 0x00, 0x19, 0x00, 0x3C, 0x00, 0x1A, 0x00, 0x09, 0x00, 0x1B, 0x00
	.byte 0x0C, 0x00, 0x1C, 0x00, 0x17, 0x00, 0x1D, 0x00, 0x16, 0x00, 0x1E, 0x00, 0x29, 0x00, 0x1F, 0x00
	.byte 0x26, 0x00, 0x20, 0x00, 0x27, 0x00, 0x21, 0x00, 0x11, 0x00, 0x22, 0x00, 0x44, 0x00, 0x23, 0x00
	.byte 0x45, 0x00, 0x24, 0x00, 0x01, 0x00, 0x25, 0x00, 0x2D, 0x00, 0x26, 0x00, 0x36, 0x00, 0x27, 0x00
	.byte 0x38, 0x00, 0x28, 0x00, 0x14, 0x00, 0x29, 0x00, 0x0A, 0x00, 0x2A, 0x00, 0x13, 0x00, 0x2B, 0x00
	.byte 0x1D, 0x00, 0x2C, 0x00, 0x24, 0x00, 0x2D, 0x00, 0x28, 0x00, 0x2E, 0x00, 0x2B, 0x00, 0x2F, 0x00
	.byte 0x22, 0x00, 0x30, 0x00, 0x37, 0x00, 0x31, 0x00, 0x47, 0x00, 0x32, 0x00, 0x8D, 0x00, 0x33, 0x00
	.byte 0x8E, 0x00, 0x34, 0x00, 0x8F, 0x00, 0x35, 0x00, 0x90, 0x00, 0x36, 0x00, 0x91, 0x00, 0x37, 0x00
	.byte 0xA9, 0x00, 0x38, 0x00, 0x2C, 0x00, 0x3A, 0x00, 0x1E, 0x00, 0x3B, 0x00, 0xAF, 0x00, 0x5E, 0x00
	.byte 0x12, 0x00, 0x67, 0x00, 0x94, 0x00, 0x72, 0x00, 0x54, 0x00, 0x73, 0x00, 0x18, 0x00, 0x74, 0x00
	.byte 0x19, 0x00, 0x75, 0x00, 0xA8, 0x00, 0x84, 0x00, 0x56, 0x00, 0x87, 0x00, 0x15, 0x01, 0x8B, 0x00
	.byte 0x16, 0x01, 0x8C, 0x00, 0x17, 0x01, 0x8D, 0x00, 0x18, 0x01, 0x8E, 0x00, 0x19, 0x01, 0x8F, 0x00
	.byte 0x1A, 0x01, 0x90, 0x00, 0x1B, 0x01, 0x91, 0x00, 0x1C, 0x01, 0x92, 0x00, 0x1D, 0x01, 0x93, 0x00
	.byte 0x1E, 0x01, 0x94, 0x00, 0xEE, 0x00, 0x95, 0x00, 0xEF, 0x00, 0x96, 0x00, 0x27, 0x01, 0x97, 0x00
	.byte 0x28, 0x01, 0x98, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00

.public ov42_02229E04
ov42_02229E04: ; 0x02229E04
	.byte 0xFF, 0xFF, 0xFF, 0xFF

.public ov42_02229E08
ov42_02229E08: ; 0x02229E08
	.byte 0x01, 0x00, 0x03, 0x02

.public ov42_02229E0C
ov42_02229E0C: ; 0x02229E0C
	.byte 0x00

.public ov42_02229E0D
ov42_02229E0D: ; 0x02229E0D
	.byte 0xF0, 0x00, 0x10
	.byte 0xF0, 0x00, 0x10, 0x00

.public ov42_02229E14
ov42_02229E14: ; 0x02229E14
	.word ov42_022285A4
	.word ov42_022285FC
	.word ov42_02228668
	.word ov42_02228700
	.word ov42_02228798
	.word ov42_022287F0
	.word ov42_0222885C
	.word ov42_022288F4
	.word ov42_0222898C
	.word ov42_022289F8
	.word ov42_02228A64
	.word ov42_02228AD0

.public ov42_02229E44
ov42_02229E44: ; 0x02229E44
	.word ov42_02228B3C
	.word ov42_02228B40
	.word ov42_02228B48
	.word ov42_02228B88
	.word ov42_02228BC8
	.word ov42_02228BCC
	.word ov42_02228B48
	.word ov42_02228B48
	.word ov42_02228BD4
	.word ov42_02228BD4
	.word ov42_02228BD4
	.word ov42_02228BD4

.public ov42_02229E74
ov42_02229E74: ; 0x02229E74
	.word ov42_02228CDC
	.word ov42_02228D08
	.word ov42_02228D44
	.word ov42_02228DB8
	.word ov42_02228DC8
	.word ov42_02228DF0
	.word ov42_02228DFC
	.word ov42_02228D44
	.word ov42_02228E0C
	.word ov42_02228E0C
	.word ov42_02228E0C
	.word ov42_02228E0C

.public ov42_02229EA4
ov42_02229EA4: ; 0x02229EA4
	.byte 0x00, 0x00, 0x01, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00

.public ov42_02229EB0
ov42_02229EB0: ; 0x02229EB0
	.byte 0x00, 0x03, 0x02, 0x04, 0x00, 0x05, 0x06, 0x08, 0x07, 0x08, 0x02, 0x06

.public ov42_02229EBC
ov42_02229EBC: ; 0x02229EBC
	.byte 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
