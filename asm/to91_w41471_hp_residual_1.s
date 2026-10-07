	.extern ov05_0221CDC4
	.extern ov05_0221EC5C
	.extern ov05_0221EF2C
	.include "asm/macros.inc"
	.include "overlay_05.inc"
	.include "global.inc"
	.extern ov05_0221E0A8

.text
	thumb_func_start ov05_0221E110
ov05_0221E110: ; 0x0221E110
	push {r4, r5, r6, r7, lr}
	sub sp, #0xc
	add r5, r0, #0
	mov r0, #0
	str r0, [sp, #8]
	str r1, [sp]
	add r0, r1, #0
	lsl r1, r0, #2
	ldr r0, [sp]
	ldr r2, _0221E268 ; =ov05_0221EC5C
	add r7, r0, r1
	lsl r0, r7, #2
	add r4, r5, r0
	ldr r0, [sp]
	mov r1, #0xa
	mul r1, r0
	str r4, [sp, #4]
	add r6, r2, r1
_0221E134:
	mov r1, #0x1c
	add r2, r7, #0
	mul r2, r1
	ldr r1, _0221E26C ; =ov05_0221EF2C
	add r0, r5, #0
	add r1, r1, r2
	bl ov05_0221CDC4
	mov r1, #0x66
	lsl r1, r1, #2
	str r0, [r4, r1]
	add r0, r1, #0
	ldrb r1, [r6]
	ldrb r2, [r6, #1]
	ldr r0, [r4, r0]
	bl ManagedSprite_SetPositionXY
	ldr r0, [sp, #8]
	add r7, r7, #1
	add r0, r0, #1
	add r4, r4, #4
	add r6, r6, #2
	str r0, [sp, #8]
	cmp r0, #5
	blo _0221E134
	ldr r0, [sp]
	mov r2, #0x86
	mov r1, #0x18
	add r6, r0, #0
	mul r6, r1
	add r3, r5, r6
	lsl r2, r2, #2
	ldrh r0, [r3, r2]
	cmp r0, #0
	bne _0221E19C
	mov r5, #0
	add r4, r2, #0
	add r6, r5, #0
	sub r4, #0x80
_0221E182:
	ldr r0, [sp, #4]
	add r1, r6, #0
	ldr r0, [r0, r4]
	bl ManagedSprite_SetDrawFlag
	ldr r0, [sp, #4]
	add r5, r5, #1
	add r0, r0, #4
	str r0, [sp, #4]
	cmp r5, #5
	blo _0221E182
	add sp, #0xc
	pop {r4, r5, r6, r7, pc}
_0221E19C:
	ldr r1, [sp]
	mov r4, #0x14
	mul r4, r1
	add r1, r2, #0
	sub r1, #0x80
	add r7, r5, r1
	add r1, r2, #0
	add r1, #0xd
	add r2, #0xb
	ldrb r1, [r3, r1]
	ldrb r2, [r3, r2]
	bl GetMonIconPaletteEx
	add r1, r0, #0
	ldr r0, [r7, r4]
	bl ManagedSprite_SetPaletteOverride
	ldr r1, [sp]
	add r0, r5, #0
	bl ov05_0221E0A8
	add r1, r0, #0
	ldr r0, [r7, r4]
	bl ManagedSprite_SetAnim
	ldr r1, _0221E270 ; =0x0000021E
	add r0, r5, r6
	ldrh r0, [r0, r1]
	cmp r0, #0
	bne _0221E1E6
	add r0, r5, r4
	sub r1, #0x82
	ldr r0, [r0, r1]
	mov r1, #0
	bl ManagedSprite_SetDrawFlag
	b _0221E20C
_0221E1E6:
	bl ItemIdIsMail
	cmp r0, #1
	bne _0221E1FE
	mov r0, #0x67
	add r1, r5, r4
	lsl r0, r0, #2
	ldr r0, [r1, r0]
	mov r1, #1
	bl ManagedSprite_SetAnim
	b _0221E20C
_0221E1FE:
	mov r0, #0x67
	add r1, r5, r4
	lsl r0, r0, #2
	ldr r0, [r1, r0]
	mov r1, #0
	bl ManagedSprite_SetAnim
_0221E20C:
	mov r0, #0x89
	add r1, r5, r6
	lsl r0, r0, #2
	ldrb r1, [r1, r0]
	cmp r1, #0
	bne _0221E226
	add r1, r5, r4
	sub r0, #0x84
	ldr r0, [r1, r0]
	mov r1, #0
	bl ManagedSprite_SetDrawFlag
	b _0221E232
_0221E226:
	add r1, r5, r4
	sub r0, #0x84
	ldr r0, [r1, r0]
	mov r1, #2
	bl ManagedSprite_SetAnim
_0221E232:
	mov r3, #0x8a
	add r0, r5, r6
	lsl r3, r3, #2
	ldr r0, [r0, r3]
	cmp r0, #7
	bne _0221E24E
	add r0, r5, r4
	sub r3, #0x84
	ldr r0, [r0, r3]
	mov r1, #0
	bl ManagedSprite_SetDrawFlag
	add sp, #0xc
	pop {r4, r5, r6, r7, pc}
_0221E24E:
	add r0, r3, #0
	add r1, r5, r4
	sub r0, #0x84
	ldr r0, [r1, r0]
	ldr r1, [sp, #8]
	mov r2, #0x18
	mul r2, r1
	add r1, r5, r2
	ldr r1, [r1, r3]
	bl ManagedSprite_SetAnim
	add sp, #0xc
	pop {r4, r5, r6, r7, pc}
	.balign 4, 0
_0221E268: .word ov05_0221EC5C
_0221E26C: .word ov05_0221EF2C
_0221E270: .word 0x0000021E
	thumb_func_end ov05_0221E110
