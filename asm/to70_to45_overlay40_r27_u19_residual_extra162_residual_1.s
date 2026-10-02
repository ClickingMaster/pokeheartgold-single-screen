	.extern Heap_Alloc
	.extern Heap_Free
	.extern MI_CpuFill8
	.extern NewMsgDataFromNarc
	.extern Pokedex_CheckMonSeenFlag
	.extern Save_Pokedex_Get
	.extern ov40_0222DD68
	.extern ov40_02245CD4
	.include "asm/macros.inc"
	.include "overlay_40.inc"
	.include "global.inc"

.text
	thumb_func_start ov40_02235E34
ov40_02235E34: ; 0x02235E34
	push {r4, r5, r6, r7, lr}
	sub sp, #0xc
	add r5, r0, #0
	mov r0, #0x86
	lsl r0, r0, #4
	ldr r4, [r5, r0]
	add r0, r1, #1
	ldr r2, _02235F98 ; =ov40_02245CD4
	lsl r0, r0, #1
	ldrh r6, [r2, r0]
	lsl r0, r1, #1
	ldrh r0, [r2, r0]
	mov r2, #0x76
	lsl r2, r2, #2
	str r0, [sp]
	mov r0, #0x6d
	mov r1, #0
	add r2, r4, r2
	bl ov40_0222DD68
	mov r2, #0x77
	lsl r2, r2, #2
	str r0, [r4, r2]
	sub r2, r2, #4
	mov r0, #0x6d
	mov r1, #0
	add r2, r4, r2
	bl ov40_0222DD68
	str r0, [sp, #8]
	mov r0, #0x83
	lsl r0, r0, #4
	ldr r0, [r5, r0]
	bl Save_Pokedex_Get
	str r0, [sp, #4]
	mov r0, #0x75
	ldr r7, [sp]
	mov r1, #0
	lsl r0, r0, #2
	str r1, [r4, r0]
	add r0, r7, #0
	cmp r0, r6
	bge _02235EBA
	lsl r1, r0, #1
	ldr r0, [sp, #8]
	add r5, r0, r1
_02235E92:
	ldrh r1, [r5]
	ldr r0, [sp, #4]
	bl Pokedex_CheckMonSeenFlag
	cmp r0, #0
	bne _02235EA4
	ldr r0, _02235F9C ; =0x0000FFFF
	strh r0, [r5]
	b _02235EB2
_02235EA4:
	mov r0, #0x75
	lsl r0, r0, #2
	ldr r0, [r4, r0]
	add r1, r0, #1
	mov r0, #0x75
	lsl r0, r0, #2
	str r1, [r4, r0]
_02235EB2:
	add r7, r7, #1
	add r5, r5, #2
	cmp r7, r6
	blt _02235E92
_02235EBA:
	ldr r1, [sp]
	mov r0, #0
	cmp r1, r6
	bge _02235EEA
	lsl r2, r1, #1
	ldr r1, [sp, #8]
	ldr r7, _02235F9C ; =0x0000FFFF
	add r2, r1, r2
	add r3, r0, #0
_02235ECC:
	ldrh r1, [r2]
	cmp r1, r7
	beq _02235EDE
	mov r5, #0x77
	lsl r5, r5, #2
	ldr r5, [r4, r5]
	add r0, r0, #1
	strh r1, [r5, r3]
	add r3, r3, #2
_02235EDE:
	ldr r1, [sp]
	add r2, r2, #2
	add r1, r1, #1
	str r1, [sp]
	cmp r1, r6
	blt _02235ECC
_02235EEA:
	mov r1, #0x76
	lsl r1, r1, #2
	ldr r1, [r4, r1]
	cmp r0, r1
	bge _02235F12
	mov r7, #0x77
	ldr r1, [sp, #8]
	lsl r6, r0, #1
	lsl r7, r7, #2
	add r5, r1, r6
	sub r1, r7, #4
_02235F00:
	ldrh r3, [r5]
	ldr r2, [r4, r7]
	add r0, r0, #1
	strh r3, [r2, r6]
	ldr r2, [r4, r1]
	add r5, r5, #2
	add r6, r6, #2
	cmp r0, r2
	blt _02235F00
_02235F12:
	ldr r0, [sp, #8]
	bl Heap_Free
	mov r1, #0x75
	lsl r1, r1, #2
	ldr r1, [r4, r1]
	mov r0, #0x6d
	lsl r1, r1, #4
	bl Heap_Alloc
	mov r2, #0x7a
	lsl r2, r2, #2
	str r0, [r4, r2]
	ldr r0, [r4, r2]
	sub r2, #0x14
	ldr r2, [r4, r2]
	mov r1, #0
	lsl r2, r2, #4
	bl MI_CpuFill8
	mov r0, #0x75
	lsl r0, r0, #2
	ldr r0, [r4, r0]
	mov r5, #0
	cmp r0, #0
	ble _02235F7C
	mov r7, #0x7a
	lsl r7, r7, #2
	add r0, r7, #0
	add r2, r5, #0
	add r3, r5, #0
	sub r0, #0x14
_02235F52:
	mov r1, #0x77
	lsl r1, r1, #2
	ldr r1, [r4, r1]
	mov r6, #0x7a
	lsl r6, r6, #2
	ldrh r1, [r1, r2]
	ldr r6, [r4, r6]
	add r5, r5, #1
	str r1, [r6, r3]
	mov r1, #0x77
	lsl r1, r1, #2
	ldr r1, [r4, r1]
	ldr r6, [r4, r7]
	ldrh r1, [r1, r2]
	add r6, r6, r3
	add r2, r2, #2
	str r1, [r6, #4]
	ldr r1, [r4, r0]
	add r3, #0x10
	cmp r5, r1
	blt _02235F52
_02235F7C:
	mov r0, #0
	mov r1, #0x1b
	mov r2, #0xed
	mov r3, #0x6d
	bl NewMsgDataFromNarc
	mov r1, #0x1e
	lsl r1, r1, #4
	str r0, [r4, r1]
	mov r2, #1
	add r0, r1, #4
	str r2, [r4, r0]
	add sp, #0xc
	pop {r4, r5, r6, r7, pc}
	.balign 4, 0
_02235F98: .word ov40_02245CD4
_02235F9C: .word 0x0000FFFF
	thumb_func_end ov40_02235E34
