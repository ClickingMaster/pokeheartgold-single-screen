	.extern ov82_0223EB9C
	.extern ov82_0223EC0C
	.extern ov82_0223EC48
	.extern ov82_0223EC68
	.extern ov82_0223ED94
	.extern ov82_0223EDF0
	.extern ov82_0223EE38
	.extern ov82_0223EE6C
	.extern ov82_0223F558
	.extern ov82_0223F570
	.extern ov82_0223F580
	.extern ov82_0223FC48
	.extern ov82_0223FD18
	.extern ov82_0223FD2C
	.extern ov82_0223FDB8
	.include "asm/macros.inc"
	.include "overlay_82.inc"
	.include "global.inc"
	.extern ov82_0223EB3C

.text
	thumb_func_start ov82_0223E9E8
ov82_0223E9E8: ; 0x0223E9E8
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #8
	add r5, r0, #0
	mov r0, #0xb7
	mov r1, #0x69
	bl NARC_New
	mov r1, #0x22
	lsl r1, r1, #4
	str r0, [r5, r1]
	add r0, r5, #0
	bl ov82_0223EB3C
	add r0, r5, #0
	bl ov82_0223EB9C
	ldr r2, _0223EB2C ; =0x000001B9
	mov r0, #1
	mov r1, #0x1b
	mov r3, #0x69
	bl NewMsgDataFromNarc
	str r0, [r5, #0x20]
	mov r0, #0x69
	bl MessageFormat_New
	str r0, [r5, #0x24]
	mov r0, #0x96
	lsl r0, r0, #2
	mov r1, #0x69
	bl String_New
	str r0, [r5, #0x28]
	mov r0, #0x96
	lsl r0, r0, #2
	mov r1, #0x69
	bl String_New
	str r0, [r5, #0x2c]
	mov r6, #0
	add r4, r5, #0
	mov r7, #0x20
_0223EA3C:
	add r0, r7, #0
	mov r1, #0x69
	bl String_New
	str r0, [r4, #0x30]
	add r6, r6, #1
	add r4, r4, #4
	cmp r6, #2
	blt _0223EA3C
	mov r1, #0x1a
	mov r0, #0
	lsl r1, r1, #4
	mov r2, #0x69
	bl LoadFontPal0
	mov r1, #6
	mov r0, #0
	lsl r1, r1, #6
	mov r2, #0x69
	bl LoadFontPal1
	mov r0, #4
	mov r1, #0x40
	mov r2, #0x69
	bl LoadFontPal0
	mov r0, #0xf
	mov r1, #0xe
	mov r2, #0
	mov r3, #0x69
	bl MessagePrinter_New
	add r1, r5, #0
	add r1, #0x98
	str r0, [r1]
	add r1, r5, #0
	ldr r0, [r5, #0x48]
	add r1, #0x4c
	bl ov82_0223FD2C
	ldr r1, [r5, #0x48]
	add r0, r5, #0
	bl ov82_0223F580
	ldr r2, _0223EB30 ; =0x04000304
	ldr r0, _0223EB34 ; =0xFFFF7FFF
	ldrh r1, [r2]
	and r0, r1
	strh r0, [r2]
	bl GfGfx_BothDispOn
	add r0, r5, #0
	bl ov82_0223F558
	add r4, r0, #0
	add r0, r5, #0
	bl ov82_0223F570
	str r0, [sp]
	add r0, r5, #0
	mov r1, #0
	add r0, #0xa8
	mov r2, #1
	add r3, r4, #0
	str r1, [sp, #4]
	bl ov82_0223FC48
	mov r1, #0x81
	lsl r1, r1, #2
	str r0, [r5, r1]
	mov r3, #0xa0
	mov r1, #1
	str r3, [sp]
	mov r0, #0
	str r0, [sp, #4]
	add r0, r5, #0
	add r0, #0xa8
	add r2, r1, #0
	bl ov82_0223FC48
	mov r1, #0x82
	lsl r1, r1, #2
	str r0, [r5, r1]
	add r1, #0xc
	ldr r0, [r5, r1]
	mov r1, #0
	bl Party_GetMonByIndex
	add r1, r0, #0
	mov r0, #0x82
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	bl ov82_0223FD18
	bl sub_02037474
	cmp r0, #0
	beq _0223EB12
	mov r0, #1
	mov r1, #0x10
	bl G2dRenderer_SetObjCharTransferReservedRegion
	mov r0, #1
	bl G2dRenderer_SetPlttTransferReservedRegion
	bl sub_0203A880
_0223EB12:
	mov r0, #0x69
	bl ov82_0223FDB8
	add r1, r5, #0
	add r1, #0x8c
	str r0, [r1]
	ldr r0, _0223EB38 ; =ov82_0223EC0C
	add r1, r5, #0
	bl Main_SetVBlankIntrCB
	add sp, #8
	pop {r3, r4, r5, r6, r7, pc}
	nop
_0223EB2C: .word 0x000001B9
_0223EB30: .word 0x04000304
_0223EB34: .word 0xFFFF7FFF
_0223EB38: .word ov82_0223EC0C
	thumb_func_end ov82_0223E9E8
