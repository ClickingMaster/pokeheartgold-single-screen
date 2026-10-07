	.extern _021F49F8
	.extern _021F4A1C
	.extern _021F4A5C
	.extern _021F4A60
	.extern _021F4A76
	.extern _021F4A8A
	.extern _021F4AF0
	.extern _021F4AFA
	.extern _021F4AFC
	.extern _021F4B24
	.extern _021F4B4A
	.extern _021F4B84
	.extern _021F4B88
	.extern _021F4B8C
	.extern _021F4C1C
	.extern _021F4C98
	.extern _021F4C9C
	.extern _021F4CDA
	.extern _021F4D08
	.extern _021F4D0C
	.extern _021F4E5C
	.extern _021F4E60
	.extern _021F4E64
	.extern _021F4E76
	.extern _021F4E9C
	.extern _021F4ECC
	.extern _021F4EE2
	.extern _021F4EFC
	.extern _021F4F06
	.extern _021F4F44
	.extern _021F4F56
	.extern _021F4F66
	.extern _021F5044
	.extern _021F5050
	.extern _021F509C
	.extern _021F5104
	.extern _021F5110
	.extern _021F5162
	.extern _021F5182
	.extern _021F518C
	.extern _021F5208
	.extern _021F5218
	.extern _021F5264
	.extern _021F527E
	.extern _021F5288
	.extern _021F52D4
	.extern _021F52EE
	.extern _021F52F8
	.extern _021F5340
	.extern _021F535A
	.extern _021F5364
	.extern _021F5668
	.extern _021F5714
	.extern _021F57B4
	.extern _021F58B4
	.extern _021F58FC
	.extern _021F590E
	.extern _021F5918
	.extern _021F5948
	.extern _021F594C
	.extern _021F599A
	.extern _021F59AA
	.extern _021F5A9A
	.extern _021F5AFE
	.extern _021F5B2A
	.extern _021F5B8E
	.extern _021F5BD4
	.extern _021F5C16
	.extern _021F5C1E
	.extern _021F5C5A
	.extern _021F5C6E
	.extern _021F5C80
	.extern _021F5DB8
	.extern _021F5E4A
	.extern _021F5E90
	.extern ov14_021F49E0
	.extern ov14_021F4A20
	.extern ov14_021F4A64
	.extern ov14_021F4B90
	.extern ov14_021F4BC0
	.extern ov14_021F4CA0
	.extern ov14_021F4D10
	.extern ov14_021F4E68
	.extern ov14_021F4EA0
	.extern ov14_021F4ED0
	.extern ov14_021F4F00
	.extern ov14_021F4F24
	.extern ov14_021F4F84
	.extern ov14_021F4FBC
	.extern ov14_021F5000
	.extern ov14_021F5054
	.extern ov14_021F50A0
	.extern ov14_021F5114
	.extern ov14_021F5190
	.extern ov14_021F521C
	.extern ov14_021F528C
	.extern ov14_021F52FC
	.extern ov14_021F5368
	.extern ov14_021F53C0
	.extern ov14_021F5620
	.extern ov14_021F566C
	.extern ov14_021F5718
	.extern ov14_021F57B8
	.extern ov14_021F58B8
	.extern ov14_021F5950
	.extern ov14_021F5BD8
	.extern ov14_021F5C84
	.extern ov14_021F5C94
#include "constants/pokemon.h"
	.include "asm/macros.inc"
	.include "overlay_14.inc"
	.include "global.inc"

.text
	thumb_func_start ov14_021F5564
ov14_021F5564: ; 0x021F5564
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x10
	str r1, [sp, #0xc]
	add r6, r0, #0
	ldr r0, [r6, #0x34]
	ldr r1, _021F5618 ; =0x0000044E
	ldrb r1, [r0, r1]
	lsl r1, r1, #0x1c
	lsr r1, r1, #0x1c
	bne _021F557C
	mov r7, #0x14
	b _021F557E
_021F557C:
	mov r7, #0x16
_021F557E:
	ldr r1, _021F5618 ; =0x0000044E
	add r5, r0, #0
	ldrb r2, [r0, r1]
	mov r1, #0xf
	add r5, #0x30
	add r3, r2, #0
	bic r3, r1
	lsl r1, r2, #0x1c
	lsr r2, r1, #0x1c
	mov r1, #1
	eor r1, r2
	lsl r1, r1, #0x18
	lsr r2, r1, #0x18
	mov r1, #0xf
	and r1, r2
	add r2, r3, #0
	orr r2, r1
	ldr r1, _021F5618 ; =0x0000044E
	lsl r4, r7, #4
	strb r2, [r0, r1]
	add r0, r5, r4
	mov r1, #0xd
	bl FillWindowPixelBuffer
	add r0, r5, r4
	add r0, #0x10
	mov r1, #0xd
	bl FillWindowPixelBuffer
	ldr r0, [sp, #0xc]
	cmp r0, #0
	beq _021F5612
	ldr r0, [r6, #0x34]
	ldr r1, [sp, #0xc]
	ldr r0, [r0, #0x28]
	mov r2, #0xa
	bl GetItemNameIntoString
	mov r2, #0
	ldr r0, _021F561C ; =0x00010200
	str r2, [sp]
	str r0, [sp, #4]
	str r2, [sp, #8]
	ldr r1, [r6, #0x34]
	add r0, r5, r4
	ldr r1, [r1, #0x28]
	add r3, r2, #0
	bl ov14_021F4F24
	add r0, r5, r4
	bl CopyWindowPixelsToVram_TextMode
	ldr r0, [r6, #0x34]
	ldr r1, [sp, #0xc]
	ldr r0, [r0, #0x28]
	mov r2, #0xa
	bl GetItemDescIntoString
	mov r2, #0
	ldr r0, _021F561C ; =0x00010200
	str r2, [sp]
	str r0, [sp, #4]
	str r2, [sp, #8]
	ldr r1, [r6, #0x34]
	add r0, r5, r4
	ldr r1, [r1, #0x28]
	add r0, #0x10
	add r3, r2, #0
	bl ov14_021F4F24
	add r0, r5, r4
	add r0, #0x10
	bl CopyWindowPixelsToVram_TextMode
_021F5612:
	add r0, r7, #0
	add sp, #0x10
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
_021F5618: .word 0x0000044E
_021F561C: .word 0x00010200
	thumb_func_end ov14_021F5564
