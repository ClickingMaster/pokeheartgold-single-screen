	.extern ov43_0222A358
	.extern ov43_0222AAA4
	.extern ov43_0222ADB8
	.extern ov43_0222AE2C
	.extern ov43_0222B0A0
	.extern ov43_0222B1FC
	.extern ov43_0222B374
	.extern ov43_0222B408
	.extern ov43_0222B440
	.extern ov43_0222B458
	.extern ov43_0222B4BC
	.extern ov43_0222B534
	.extern ov43_0222B55C
	.extern ov43_0222B574
	.extern ov43_0222F0C8
	.include "asm/macros.inc"
	.include "overlay_43.inc"
	.include "global.inc"

.text
	thumb_func_start ov43_0222AE64
ov43_0222AE64: ; 0x0222AE64
	push {r4, r5, r6, r7, lr}
	sub sp, #0x14
	add r4, r1, #0
	add r7, r3, #0
	ldrb r3, [r4, #8]
	add r5, r0, #0
	add r6, r2, #0
	cmp r3, #0xb
	bhi _0222AEF0
	add r3, r3, r3
	add r3, pc
	ldrh r3, [r3, #6]
	lsl r3, r3, #0x10
	asr r3, r3, #0x10
	add pc, r3
_0222AE82: ; jump table
	.short _0222AE9A - _0222AE82 - 2 ; case 0
	.short _0222AEBA - _0222AE82 - 2 ; case 1
	.short _0222AEC8 - _0222AE82 - 2 ; case 2
	.short _0222AED2 - _0222AE82 - 2 ; case 3
	.short _0222AEE6 - _0222AE82 - 2 ; case 4
	.short _0222AF3C - _0222AE82 - 2 ; case 5
	.short _0222AF5A - _0222AE82 - 2 ; case 6
	.short _0222AF68 - _0222AE82 - 2 ; case 7
	.short _0222AF88 - _0222AE82 - 2 ; case 8
	.short _0222AFF4 - _0222AE82 - 2 ; case 9
	.short _0222B00C - _0222AE82 - 2 ; case 10
	.short _0222B058 - _0222AE82 - 2 ; case 11
_0222AE9A:
	bl ov43_0222B1FC
	mov r0, #6
	str r0, [sp]
	mov r0, #1
	str r0, [sp, #4]
	mov r0, #0
	mov r1, #0x11
	add r2, r1, #0
	add r3, r0, #0
	str r7, [sp, #8]
	bl BeginNormalPaletteFade
	mov r0, #1
	strb r0, [r4, #8]
	b _0222B08A
_0222AEBA:
	bl IsPaletteFadeFinished
	cmp r0, #0
	beq _0222AEF0
	mov r0, #3
	strb r0, [r4, #8]
	b _0222B08A
_0222AEC8:
	bl ov43_0222B1FC
	mov r0, #3
	strb r0, [r4, #8]
	b _0222B08A
_0222AED2:
	bl ov43_0222B374
	str r0, [sp, #0x10]
	cmp r0, #0
	beq _0222AEF0
	mov r0, #2
	str r0, [r5, #4]
	mov r0, #4
	strb r0, [r4, #8]
	b _0222B08A
_0222AEE6:
	ldr r3, [r5, #4]
	sub r2, r3, #1
	str r2, [r5, #4]
	cmp r3, #0
	beq _0222AEF2
_0222AEF0:
	b _0222B08A
_0222AEF2:
	mov r2, #0
	str r2, [r5, #4]
	bl ov43_0222B574
	cmp r0, #1
	bne _0222AF24
	add r0, r5, #0
	add r1, r4, #0
	bl ov43_0222B440
	ldr r0, [r5]
	cmp r0, #3
	beq _0222AF10
	cmp r0, #1
	bne _0222AF16
_0222AF10:
	mov r0, #5
	strb r0, [r4, #8]
	b _0222B08A
_0222AF16:
	add r0, r5, #0
	add r1, r6, #0
	bl ov43_0222B458
	add sp, #0x14
	mov r0, #1
	pop {r4, r5, r6, r7, pc}
_0222AF24:
	ldr r0, [r5]
	cmp r0, #1
	bne _0222B01C
	add r0, r5, #0
	add r1, r6, #0
	mov r2, #0x3a
	add r3, r7, #0
	bl ov43_0222B4BC
	mov r0, #0xb
	strb r0, [r4, #8]
	b _0222B08A
_0222AF3C:
	mov r0, #6
	str r0, [sp]
	mov r0, #1
	str r0, [sp, #4]
	mov r0, #0
	mov r1, #0x10
	str r7, [sp, #8]
	add r2, r1, #0
	add r3, r0, #0
	bl BeginNormalPaletteFade
	ldrb r0, [r4, #8]
	add r0, r0, #1
	strb r0, [r4, #8]
	b _0222B08A
_0222AF5A:
	bl IsPaletteFadeFinished
	cmp r0, #1
	bne _0222B01C
	add sp, #0x14
	mov r0, #1
	pop {r4, r5, r6, r7, pc}
_0222AF68:
	bl ov43_0222B1FC
	mov r0, #6
	str r0, [sp]
	mov r0, #1
	str r0, [sp, #4]
	mov r0, #0
	mov r1, #0x11
	add r2, r1, #0
	add r3, r0, #0
	str r7, [sp, #8]
	bl BeginNormalPaletteFade
	mov r0, #8
	strb r0, [r4, #8]
	b _0222B08A
_0222AF88:
	bl IsPaletteFadeFinished
	cmp r0, #0
	beq _0222B08A
	add r0, r7, #0
	bl PlayerProfile_New
	str r0, [sp, #0xc]
	ldr r0, [r4, #0x58]
	bl String_cstr
	add r1, r0, #0
	ldr r0, [sp, #0xc]
	bl Save_Profile_PlayerName_Set
	ldr r0, [r6, #0x50]
	ldr r2, [sp, #0xc]
	mov r1, #0
	bl BufferPlayersName
	ldr r0, [sp, #0xc]
	bl Heap_Free
	ldr r0, [r4, #0x5c]
	add r1, sp, #0x10
	bl String_atoi
	add r3, r0, #0
	ldr r0, [sp, #0x10]
	add r2, r1, #0
	cmp r0, #0
	beq _0222AFE2
	add r0, r6, #0
	add r1, r3, #0
	bl ov43_0222AAA4
	add r0, r5, #0
	add r1, r6, #0
	mov r2, #0x37
	add r3, r7, #0
	bl ov43_0222B4BC
	mov r0, #9
	strb r0, [r4, #8]
	b _0222B08A
_0222AFE2:
	add r0, r5, #0
	add r1, r6, #0
	mov r2, #0x38
	add r3, r7, #0
	bl ov43_0222B4BC
	mov r0, #0xb
	strb r0, [r4, #8]
	b _0222B08A
_0222AFF4:
	bl ov43_0222B534
	cmp r0, #0
	beq _0222B08A
	add r0, r4, #0
	add r1, r6, #0
	mov r2, #0
	bl ov43_0222ADB8
	mov r0, #0xa
	strb r0, [r4, #8]
	b _0222B08A
_0222B00C:
	add r0, r4, #0
	add r1, r6, #0
	bl ov43_0222AE2C
	cmp r0, #1
	beq _0222B01E
	cmp r0, #2
	beq _0222B04C
_0222B01C:
	b _0222B08A
_0222B01E:
	ldr r1, [r4, #0x5c]
	ldr r2, [r4, #0x58]
	add r0, r4, #0
	bl ov43_0222A358
	add r2, r0, #0
	bne _0222B038
	add r0, r5, #0
	bl ov43_0222B55C
	mov r0, #3
	strb r0, [r4, #8]
	b _0222B08A
_0222B038:
	ldr r3, _0222B090 ; =ov43_0222F0C8
	add r0, r5, #0
	ldrb r2, [r3, r2]
	add r1, r6, #0
	add r3, r7, #0
	bl ov43_0222B4BC
	mov r0, #0xb
	strb r0, [r4, #8]
	b _0222B08A
_0222B04C:
	add r0, r5, #0
	bl ov43_0222B55C
	mov r0, #3
	strb r0, [r4, #8]
	b _0222B08A
_0222B058:
	bl ov43_0222B534
	cmp r0, #0
	beq _0222B08A
	ldr r0, _0222B094 ; =gSystem
	ldr r1, [r0, #0x48]
	mov r0, #1
	tst r0, r1
	bne _0222B078
	mov r0, #2
	tst r0, r1
	bne _0222B078
	bl System_GetTouchNew
	cmp r0, #0
	beq _0222B08A
_0222B078:
	add r0, r5, #0
	bl ov43_0222B55C
	add r0, r5, #0
	add r1, r6, #0
	bl ov43_0222B408
	mov r0, #3
	strb r0, [r4, #8]
_0222B08A:
	mov r0, #0
	add sp, #0x14
	pop {r4, r5, r6, r7, pc}
	.balign 4, 0
_0222B090: .word ov43_0222F0C8
_0222B094: .word gSystem
	thumb_func_end ov43_0222AE64
