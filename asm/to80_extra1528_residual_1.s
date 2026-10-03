	.extern ov45_0222BE9C
	.extern ov45_0222BF18
	.extern ov45_0222BF98
	.extern ov45_0222C0CC
	.extern ov45_0222C130
	.extern ov45_0222C1EC
	.extern ov45_0222C2A8
	.extern ov45_0222C36C
	.extern ov45_0222C688
	.extern ov45_0222C73C
	.extern ov45_0222C79C
	.extern ov45_0222C800
	.extern ov45_0222C828
	.extern ov45_0222C83C
	.extern ov45_0222C858
	.extern ov45_0222C88C
	.extern ov45_0222CF00
	.extern ov45_0222CF24
	.extern ov45_0222CF40
	.extern ov45_0222CF68
	.extern ov45_0222CFF4
	.extern ov45_0222D01C
	.extern ov45_0222D028
	.extern ov45_0222D054
	.extern ov45_0222D078
	.extern ov45_0222D0BC
	.extern ov45_0222D0C8
	.extern ov45_0222D0D8
	.extern ov45_0222D0FC
	.extern ov45_0222D130
	.extern ov45_0222D13C
	.extern ov45_0222D14C
	.extern ov45_0222D164
	.extern ov45_0222D19C
	.extern ov45_0222D1B0
	.extern ov45_0222D1DC
	.extern ov45_0222D20C
	.extern ov45_0222D23C
	.extern ov45_0222D250
	.extern ov45_0222D27C
	.extern ov45_0222D2AC
	.extern ov45_0222D2E4
	.extern ov45_0222D2F8
	.extern ov45_0222D324
	.extern ov45_0222D354
	.extern ov45_0222D380
	.extern ov45_0222D38C
	.extern ov45_0222D3B0
	.extern ov45_0222D3C4
	.extern ov45_0222D3D8
	.extern ov45_0222D410
	.extern ov45_0222D41C
	.extern ov45_0222D428
	.extern ov45_0222D448
	.extern ov45_0222D44C
	.extern ov45_0222D484
	.extern ov45_0222D4C0
	.extern ov45_0222D4CC
	.extern ov45_0222D4DC
	.extern ov45_0222D524
	.extern ov45_0222E14C
	.extern ov45_0222E1A0
	.extern ov45_0222E200
	.extern ov45_0222E33C
	.extern ov45_0222E39C
	.extern ov45_0222E414
	.extern ov45_0222E484
	.extern ov45_0222E4A8
	.extern ov45_0222E540
	.extern ov45_0222E550
	.extern ov45_0222E560
	.extern ov45_0222E574
	.extern ov45_0222E584
	.extern ov45_0222E598
	.extern ov45_0222E59C
	.extern ov45_0222E5A0
	.extern ov45_02230B64
	.extern ov45_02230B8C
	.extern ov45_02230BFC
	.extern ov45_02230C40
	.extern ov45_02230CD8
	.extern ov45_02230D20
	.extern ov45_02230D5C
#include "config.h"
	.include "asm/macros.inc"
	.include "overlay_45_thumb.inc"
	.include "global.inc"

.text
	thumb_func_start ov45_02230F94
ov45_02230F94: ; 0x02230F94
	push {r0, r1, r2, r3}
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x10
	add r7, r3, #0
	add r5, r0, #0
	add r6, r1, #0
	mov r0, #0x62
	add r1, r7, #0
	bl NARC_New
	add r1, sp, #0xc
	str r1, [sp]
	add r1, r7, #0
	mov r2, #0xdb
	mov r3, #3
	add r4, r0, #0
	bl ov45_02231018
	add r0, sp, #8
	str r0, [sp]
	add r0, r4, #0
	add r1, r7, #0
	add r2, r5, #0
	add r3, r6, #0
	bl ov45_02231018
	ldr r1, [sp, #0xc]
	ldr r0, [sp, #8]
	sub r0, r1, r0
	lsl r0, r0, #0x10
	lsr r1, r0, #0x10
	mov r0, #0x18
	mul r0, r1
	ldr r1, _02231014 ; =0x0000FFFF
	bl _s32_div_f
	cmp r0, #0
	bge _02230FE2
	add r0, #0x18
_02230FE2:
	cmp r0, #0x18
	blt _02230FEE
	mov r1, #0x18
	bl _s32_div_f
	add r0, r1, #0
_02230FEE:
	add r2, sp, #4
	mov r1, #0
	str r1, [r2]
	add r1, sp, #4
	strb r0, [r1]
	add r0, r2, #0
	add r1, sp, #0x30
	bl ov45_0222D524
	add r0, r4, #0
	bl NARC_Delete
	ldr r0, [sp, #4]
	add sp, #0x10
	pop {r3, r4, r5, r6, r7}
	pop {r3}
	add sp, #0x10
	bx r3
	nop
_02231014: .word 0x0000FFFF
	thumb_func_end ov45_02230F94

	thumb_func_start ov45_02231018
ov45_02231018: ; 0x02231018
	push {r4, r5, r6, r7, lr}
	sub sp, #0x1c
	str r1, [sp, #0xc]
	str r3, [sp, #0x10]
	mov r7, #0
	add r5, r2, #0
	str r7, [sp]
	add r1, sp, #0x18
	str r1, [sp, #4]
	ldr r3, [sp, #0xc]
	mov r1, #0x12
	add r2, r7, #0
	str r0, [sp, #8]
	ldr r4, [sp, #0x30]
	bl GfGfxLoader_LoadFromOpenNarc_GetSizeOut
	add r6, r0, #0
	ldr r0, [sp, #0x18]
	mov r1, #6
	bl _u32_div_f
	cmp r5, r0
	blt _0223104A
	bl GF_AssertFail
_0223104A:
	mov r0, #6
	add r1, r5, #0
	mul r1, r0
	ldrh r0, [r6, r1]
	cmp r0, #2
	beq _02231060
	add r1, r6, r1
	mov r0, #4
	ldrsh r0, [r1, r0]
	mov r7, #1
	str r0, [r4]
_02231060:
	add r0, r6, #0
	bl Heap_Free
	cmp r7, #0
	bne _022310AC
	add r0, r5, #0
	bl LocationGmmDatIndexGetByCountryMsgNo
	bl LocationGmmDatGetEarthPlaceDatId
	mov r2, #0
	add r1, r0, #0
	str r2, [sp]
	add r0, sp, #0x14
	str r0, [sp, #4]
	ldr r0, [sp, #8]
	ldr r3, [sp, #0xc]
	bl GfGfxLoader_LoadFromOpenNarc_GetSizeOut
	add r5, r0, #0
	ldr r0, [sp, #0x14]
	lsr r1, r0, #2
	ldr r0, [sp, #0x10]
	cmp r0, r1
	bge _0223109C
	lsl r0, r0, #2
	add r1, r5, r0
	mov r0, #2
	ldrsh r0, [r1, r0]
	b _022310A4
_0223109C:
	bl GF_AssertFail
	mov r0, #2
	ldrsh r0, [r5, r0]
_022310A4:
	str r0, [r4]
	add r0, r5, #0
	bl Heap_Free
_022310AC:
	add sp, #0x1c
	pop {r4, r5, r6, r7, pc}
	thumb_func_end ov45_02231018
