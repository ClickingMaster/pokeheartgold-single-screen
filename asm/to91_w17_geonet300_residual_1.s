	.extern ov69_021E7520
	.extern ov69_021E75A0
	.extern ov69_021E75F8
	.include "asm/macros.inc"
	.include "overlay_69.inc"
	.include "global.inc"
	.extern ov69_021E6C30

.text
	thumb_func_start ov69_021E6D5C
ov69_021E6D5C: ; 0x021E6D5C
	push {r4, r5, r6, lr}
	sub sp, #0x10
	ldr r1, _021E6E78 ; =0x0000C338
	add r5, r0, #0
	ldr r1, [r5, r1]
	cmp r1, #0
	bne _021E6DB6
	mov r0, #1
	ldr r1, [r5]
	lsl r0, r0, #0xa
	bl String_New
	add r4, r0, #0
	mov r0, #0xd8
	str r0, [sp]
	mov r0, #0x30
	str r0, [sp, #4]
	ldr r0, _021E6E7C ; =0x0000C014
	mov r2, #0
	add r0, r5, r0
	mov r1, #0xf
	add r3, r2, #0
	bl FillWindowPixelRect
	ldr r0, _021E6E80 ; =0x0000C070
	mov r1, #0xf
	ldr r0, [r5, r0]
	add r2, r4, #0
	bl ReadMsgDataIntoString
	mov r3, #0
	ldr r0, _021E6E7C ; =0x0000C014
	str r3, [sp]
	str r3, [sp, #4]
	add r0, r5, r0
	mov r1, #1
	add r2, r4, #0
	str r3, [sp, #8]
	bl AddTextPrinterParameterized
	add r0, r4, #0
	bl String_Delete
	add sp, #0x10
	pop {r4, r5, r6, pc}
_021E6DB6:
	add r1, sp, #0xc
	bl ov69_021E6C30
	cmp r0, #0
	beq _021E6E6E
	ldr r1, [r5]
	mov r0, #0x40
	bl String_New
	add r6, r0, #0
	ldr r1, [r5]
	mov r0, #0x40
	bl String_New
	add r4, r0, #0
	mov r0, #0xd8
	str r0, [sp]
	mov r0, #0x30
	str r0, [sp, #4]
	ldr r0, _021E6E7C ; =0x0000C014
	mov r2, #0
	add r0, r5, r0
	mov r1, #0xf
	add r3, r2, #0
	bl FillWindowPixelRect
	ldr r1, [sp, #0xc]
	mov r0, #0x30
	add r2, r1, #0
	mul r2, r0
	ldr r0, [r5]
	add r1, r5, r2
	str r0, [sp]
	ldrh r0, [r1, #0x3a]
	ldrh r1, [r1, #0x3c]
	add r2, r6, #0
	add r3, r4, #0
	bl ov69_021E7520
	ldr r1, [sp, #0xc]
	mov r0, #0x30
	mul r0, r1
	add r0, r5, r0
	ldrh r0, [r0, #0x3c]
	cmp r0, #0
	beq _021E6E2A
	mov r0, #0x10
	str r0, [sp]
	mov r0, #0xff
	str r0, [sp, #4]
	ldr r0, _021E6E7C ; =0x0000C014
	mov r3, #0
	add r0, r5, r0
	mov r1, #1
	add r2, r4, #0
	str r3, [sp, #8]
	bl AddTextPrinterParameterized
_021E6E2A:
	mov r3, #0
	ldr r0, _021E6E7C ; =0x0000C014
	str r3, [sp]
	str r3, [sp, #4]
	add r0, r5, r0
	mov r1, #1
	add r2, r6, #0
	str r3, [sp, #8]
	bl AddTextPrinterParameterized
	add r0, r4, #0
	bl String_Delete
	add r0, r6, #0
	bl String_Delete
	ldr r0, [sp, #0xc]
	mov r1, #0x30
	add r2, r0, #0
	mul r2, r1
	add r2, r5, r2
	mov r0, #0x10
	ldrsh r2, [r2, r0]
	ldr r0, _021E6E84 ; =0x0000C2C4
	str r2, [r5, r0]
	ldr r2, [sp, #0xc]
	add r0, r0, #4
	mul r1, r2
	add r2, r5, r1
	mov r1, #0x12
	ldrsh r1, [r2, r1]
	add sp, #0x10
	str r1, [r5, r0]
	pop {r4, r5, r6, pc}
_021E6E6E:
	ldr r0, _021E6E78 ; =0x0000C338
	mov r1, #0
	str r1, [r5, r0]
	add sp, #0x10
	pop {r4, r5, r6, pc}
	.balign 4, 0
_021E6E78: .word 0x0000C338
_021E6E7C: .word 0x0000C014
_021E6E80: .word 0x0000C070
_021E6E84: .word 0x0000C2C4
	thumb_func_end ov69_021E6D5C
