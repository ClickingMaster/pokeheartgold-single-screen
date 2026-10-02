	.extern ov71_02247388
	.extern ov71_0224738C
	.extern ov71_02247398
	.extern ov71_022473BC
	.extern ov71_022473C4
	.extern ov71_022473DC
	.extern ov71_02247704
	.extern ov71_0224B6F0
	.extern ov71_0224B910
	.extern ov71_0224B960
	.extern ov71_0224B9CC
	.include "asm/macros.inc"
	.include "overlay_71.inc"
	.include "global.inc"

.text
	thumb_func_start ov71_0224B57C
ov71_0224B57C: ; 0x0224B57C
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x10
	add r5, r1, #0
	ldr r1, [r5]
	add r4, r0, #0
	cmp r1, #5
	bhi _0224B5B8
	add r1, r1, r1
	add r1, pc
	ldrh r1, [r1, #6]
	lsl r1, r1, #0x10
	asr r1, r1, #0x10
	add pc, r1
_0224B596: ; jump table
	.short _0224B5A2 - _0224B596 - 2 ; case 0
	.short _0224B5AE - _0224B596 - 2 ; case 1
	.short _0224B62C - _0224B596 - 2 ; case 2
	.short _0224B68E - _0224B596 - 2 ; case 3
	.short _0224B6AE - _0224B596 - 2 ; case 4
	.short _0224B6D8 - _0224B596 - 2 ; case 5
_0224B5A2:
	mov r0, #0
	str r0, [r4, #8]
	ldr r0, [r5]
	add r0, r0, #1
	str r0, [r5]
	b _0224B6E6
_0224B5AE:
	ldr r0, [r4, #8]
	add r0, r0, #1
	str r0, [r4, #8]
	cmp r0, #0xa
	bgt _0224B5BA
_0224B5B8:
	b _0224B6E6
_0224B5BA:
	ldr r0, [r4]
	bl ov71_0224738C
	str r0, [sp, #0xc]
	ldr r0, [r4]
	bl ov71_02247388
	add r7, r0, #0
	ldr r0, [r4]
	bl ov71_022473DC
	cmp r0, #1
	bne _0224B5DA
	mov r1, #2
	add r6, r1, #0
	b _0224B5DE
_0224B5DA:
	mov r1, #5
	mov r6, #3
_0224B5DE:
	ldr r0, [sp, #0xc]
	ldr r2, [r4, #0x6c]
	bl ReadMsgDataIntoString
	ldr r1, [r4, #0x70]
	ldr r2, [r4, #0x6c]
	add r0, r7, #0
	bl StringExpandPlaceholders
	mov r3, #0
	str r3, [sp]
	mov r0, #0xff
	str r0, [sp, #4]
	str r3, [sp, #8]
	add r0, r4, #0
	ldr r2, [r4, #0x70]
	add r0, #0x5c
	mov r1, #1
	bl AddTextPrinterParameterized
	add r0, r4, #0
	add r0, #0x5c
	mov r1, #0x6d
	mov r2, #2
	bl sub_0200E948
	add r0, r4, #0
	add r0, #0x5c
	bl CopyWindowToVram
	ldr r0, _0224B6EC ; =0x000004A4
	bl PlayFanfare
	mov r0, #0
	str r0, [r4, #8]
	mov r0, #1
	str r0, [r4, #0xc]
	str r6, [r5]
	b _0224B6E6
_0224B62C:
	mov r1, #0x3c
	bl ov71_0224B6F0
	cmp r0, #0
	beq _0224B6E6
	ldr r0, [r4]
	bl ov71_0224738C
	add r6, r0, #0
	ldr r0, [r4]
	bl ov71_02247388
	add r7, r0, #0
	ldr r2, [r4, #0x6c]
	add r0, r6, #0
	mov r1, #3
	bl ReadMsgDataIntoString
	ldr r1, [r4, #0x70]
	ldr r2, [r4, #0x6c]
	add r0, r7, #0
	bl StringExpandPlaceholders
	add r0, r4, #0
	add r0, #0x5c
	mov r1, #0xf
	bl FillWindowPixelBuffer
	mov r3, #0
	str r3, [sp]
	mov r0, #0xff
	str r0, [sp, #4]
	str r3, [sp, #8]
	add r0, r4, #0
	ldr r2, [r4, #0x70]
	add r0, #0x5c
	mov r1, #1
	bl AddTextPrinterParameterized
	add r0, r4, #0
	add r0, #0x5c
	bl CopyWindowPixelsToVram_TextMode
	mov r0, #0
	str r0, [r4, #8]
	ldr r0, [r5]
	add r0, r0, #1
	str r0, [r5]
	b _0224B6E6
_0224B68E:
	mov r1, #0x3c
	bl ov71_0224B6F0
	cmp r0, #0
	beq _0224B6E6
	add r0, r4, #0
	add r0, #0x5c
	mov r1, #0
	bl ClearFrameAndWindow2
	mov r0, #0
	str r0, [r4, #8]
	ldr r0, [r5]
	add r0, r0, #1
	str r0, [r5]
	b _0224B6E6
_0224B6AE:
	ldr r0, [r4, #8]
	add r0, r0, #1
	str r0, [r4, #8]
	cmp r0, #0xa
	ble _0224B6E6
	mov r0, #0x10
	str r0, [sp]
	mov r1, #0
	mov r0, #1
	str r0, [sp, #4]
	mov r0, #0x39
	str r0, [sp, #8]
	mov r0, #3
	add r2, r1, #0
	add r3, r1, #0
	bl BeginNormalPaletteFade
	ldr r0, [r5]
	add r0, r0, #1
	str r0, [r5]
	b _0224B6E6
_0224B6D8:
	bl IsPaletteFadeFinished
	cmp r0, #0
	beq _0224B6E6
	add sp, #0x10
	mov r0, #1
	pop {r3, r4, r5, r6, r7, pc}
_0224B6E6:
	mov r0, #0
	add sp, #0x10
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
_0224B6EC: .word 0x000004A4
	thumb_func_end ov71_0224B57C
