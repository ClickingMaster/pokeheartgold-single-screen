	.extern ov41_022463DC
	.include "asm/macros.inc"
	.include "overlay_41.inc"
	.include "global.inc"

.text
	thumb_func_start ov41_02246BEC
ov41_02246BEC: ; 0x02246BEC
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x10
	add r5, r0, #0
	mov r0, #0x87
	add r4, r1, #0
	mov r6, #0
	str r0, [sp, #8]
_02246BFA:
	mov r0, #1
	str r0, [sp]
	mov r0, #6
	lsl r0, r0, #6
	add r1, r6, #0
	ldr r0, [r5, r0]
	add r1, #0xce
	mov r2, #0
	mov r3, #0xe
	bl GfGfxLoader_LoadFromOpenNarc
	add r2, r6, #0
	add r1, r0, #0
	add r2, #0x64
	add r0, r5, #0
	lsl r7, r2, #3
	bl ov41_022463DC
	ldr r1, [r4]
	mov r2, #0
	add r1, r1, r7
	str r0, [r1, #4]
	ldr r1, [r5]
	ldr r0, [r4]
	mov r3, #0xe
	str r1, [r0, r7]
	add r0, r6, #1
	str r0, [sp, #4]
	mov r0, #1
	str r0, [sp]
	mov r0, #6
	lsl r0, r0, #6
	ldr r0, [r5, r0]
	ldr r1, [sp, #8]
	bl GfGfxLoader_LoadFromOpenNarc
	str r0, [sp, #0xc]
	ldr r0, [r4, #0x14]
	ldr r1, [sp, #0xc]
	ldr r2, [sp, #4]
	bl GF2dGfxRawResMan_AllocObj
	ldr r1, [sp, #4]
	mov r0, #0xc
	add r7, r1, #0
	mul r7, r0
	ldr r1, [r4, #8]
	ldr r0, [sp, #0xc]
	add r1, r1, r7
	add r1, r1, #4
	bl NNS_G2dGetUnpackedPaletteData
	ldr r0, [r4, #8]
	add r0, r0, r7
	ldr r0, [r0, #4]
	cmp r0, #0
	bne _02246C70
	bl GF_AssertFail
_02246C70:
	ldr r1, [r5]
	ldr r0, [r4, #8]
	add r6, r6, #1
	str r1, [r0, r7]
	ldr r0, [r4, #8]
	add r1, r0, r7
	mov r0, #1
	str r0, [r1, #8]
	ldr r0, [sp, #8]
	add r0, r0, #4
	str r0, [sp, #8]
	cmp r6, #0x12
	blt _02246BFA
	add sp, #0x10
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
	thumb_func_end ov41_02246BEC
