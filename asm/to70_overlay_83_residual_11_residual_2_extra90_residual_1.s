	.extern ov83_02240528
	.extern ov83_02240664
	.extern ov83_02240748
	.extern ov83_022407FC
	.extern ov83_0224088C
	.extern ov83_022408E0
	.extern ov83_02240C48
	.extern ov83_02241DD8
	.extern ov83_0224755C
	.extern ov83_0224759C
	.extern ov83_0224777C
	.extern ov83_022479E4
	.extern ov83_02247B7C
	.extern ov83_02247D12
	.extern ov83_02247D24
	.extern ov83_02247EE0
	.extern ov83_02247F88
	.include "asm/macros.inc"
	.include "overlay_83.inc"
	.include "global.inc"
	.public ov83_02240984

.text
	thumb_func_start ov83_02240984
ov83_02240984: ; 0x02240984
	push {r4, r5, r6, r7, lr}
	sub sp, #0x14
	add r5, r0, #0
	mov r0, #0x43
	lsl r0, r0, #4
	mov r6, #0x3e
	add r4, r5, r0
	mov r7, #0
_02240994:
	add r0, r4, #0
	add r1, r7, #0
	bl FillWindowPixelBuffer
	add r6, r6, #1
	add r4, #0x10
	cmp r6, #0x45
	bls _02240994
	mov r3, #0
	str r3, [sp]
	ldr r0, _02240B34 ; =0x00010200
	str r3, [sp, #4]
	str r0, [sp, #8]
	str r3, [sp, #0xc]
	mov r0, #0x45
	lsl r0, r0, #4
	ldr r1, [r5, #0x20]
	add r0, r5, r0
	mov r2, #0x58
	bl ov83_022479E4
	mov r3, #0
	str r3, [sp]
	ldr r0, _02240B34 ; =0x00010200
	str r3, [sp, #4]
	str r0, [sp, #8]
	str r3, [sp, #0xc]
	mov r0, #0x49
	lsl r0, r0, #4
	ldr r1, [r5, #0x20]
	add r0, r5, r0
	mov r2, #0x46
	bl ov83_022479E4
	mov r3, #0
	str r3, [sp]
	ldr r0, _02240B34 ; =0x00010200
	str r3, [sp, #4]
	str r0, [sp, #8]
	str r3, [sp, #0xc]
	mov r0, #0x47
	lsl r0, r0, #4
	ldr r1, [r5, #0x20]
	add r0, r5, r0
	mov r2, #0x59
	bl ov83_022479E4
	mov r1, #0
	ldr r2, _02240B38 ; =0x00000818
	str r1, [sp]
	ldrh r2, [r5, r2]
	add r0, r5, #0
	mov r3, #3
	bl ov83_02240C48
	mov r0, #0
	str r0, [sp]
	ldr r2, _02240B3C ; =0x0000081A
	add r0, r5, #0
	ldrh r2, [r5, r2]
	mov r1, #1
	mov r3, #3
	bl ov83_02240C48
	mov r0, #0x12
	lsl r0, r0, #6
	add r0, r5, r0
	bl GetWindowWidth
	lsl r1, r0, #3
	lsr r0, r1, #0x1f
	add r0, r1, r0
	asr r0, r0, #1
	str r0, [sp]
	mov r0, #0
	str r0, [sp, #4]
	str r0, [sp, #8]
	ldr r0, _02240B34 ; =0x00010200
	mov r1, #0x12
	str r0, [sp, #0xc]
	mov r0, #2
	str r0, [sp, #0x10]
	lsl r1, r1, #6
	ldr r2, [r5, #0x20]
	add r0, r5, #0
	add r1, r5, r1
	mov r3, #0x5f
	bl ov83_02241DD8
	ldr r0, _02240B40 ; =0x00000804
	ldr r0, [r5, r0]
	bl Mon_GetBoxMon
	add r2, r0, #0
	ldr r0, [r5, #0x24]
	mov r1, #0
	bl BufferBoxMonNickname
	mov r1, #0
	str r1, [sp]
	str r1, [sp, #4]
	ldr r0, _02240B34 ; =0x00010200
	str r1, [sp, #8]
	str r0, [sp, #0xc]
	str r1, [sp, #0x10]
	mov r1, #0x43
	lsl r1, r1, #4
	ldr r2, [r5, #0x20]
	add r0, r5, #0
	add r1, r5, r1
	mov r3, #0x5b
	bl ov83_02241DD8
	ldr r0, _02240B44 ; =0x0000080E
	ldrb r0, [r5, r0]
	lsl r1, r0, #0x18
	lsr r1, r1, #0x1f
	bne _02240AC2
	lsl r0, r0, #0x19
	lsr r0, r0, #0x19
	bne _02240AA2
	mov r3, #0
	str r3, [sp]
	ldr r0, _02240B48 ; =0x00050600
	str r3, [sp, #4]
	str r0, [sp, #8]
	str r3, [sp, #0xc]
	mov r0, #0x11
	lsl r0, r0, #6
	ldr r1, [r5, #0x20]
	add r0, r5, r0
	mov r2, #0x56
	bl ov83_022479E4
	b _02240AC2
_02240AA2:
	cmp r0, #1
	bne _02240AC2
	mov r3, #0
	str r3, [sp]
	mov r0, #0xc1
	str r3, [sp, #4]
	lsl r0, r0, #0xa
	str r0, [sp, #8]
	str r3, [sp, #0xc]
	mov r0, #0x11
	lsl r0, r0, #6
	ldr r1, [r5, #0x20]
	add r0, r5, r0
	mov r2, #0x57
	bl ov83_022479E4
_02240AC2:
	mov r1, #0
	ldr r2, _02240B4C ; =0x0000080F
	str r1, [sp]
	ldrb r2, [r5, r2]
	add r0, r5, #0
	mov r3, #3
	bl ov83_02240C48
	mov r1, #0
	str r1, [sp]
	str r1, [sp, #4]
	ldr r0, _02240B34 ; =0x00010200
	str r1, [sp, #8]
	str r0, [sp, #0xc]
	str r1, [sp, #0x10]
	mov r1, #0x46
	lsl r1, r1, #4
	ldr r2, [r5, #0x20]
	add r0, r5, #0
	add r1, r5, r1
	mov r3, #0x5e
	bl ov83_02241DD8
	ldr r2, _02240B50 ; =0x00000812
	ldr r0, [r5, #0x24]
	ldrh r2, [r5, r2]
	mov r1, #0
	bl BufferItemName
	mov r1, #0
	str r1, [sp]
	str r1, [sp, #4]
	ldr r0, _02240B34 ; =0x00010200
	str r1, [sp, #8]
	str r0, [sp, #0xc]
	str r1, [sp, #0x10]
	mov r1, #0x4a
	lsl r1, r1, #4
	ldr r2, [r5, #0x20]
	add r0, r5, #0
	add r1, r5, r1
	mov r3, #0x47
	bl ov83_02241DD8
	mov r0, #0x43
	lsl r0, r0, #4
	mov r4, #0x3e
	add r5, r5, r0
_02240B22:
	add r0, r5, #0
	bl ScheduleWindowCopyToVram
	add r4, r4, #1
	add r5, #0x10
	cmp r4, #0x45
	bls _02240B22
	add sp, #0x14
	pop {r4, r5, r6, r7, pc}
	.balign 4, 0
_02240B34: .word 0x00010200
_02240B38: .word 0x00000818
_02240B3C: .word 0x0000081A
_02240B40: .word 0x00000804
_02240B44: .word 0x0000080E
_02240B48: .word 0x00050600
_02240B4C: .word 0x0000080F
_02240B50: .word 0x00000812
	thumb_func_end ov83_02240984
