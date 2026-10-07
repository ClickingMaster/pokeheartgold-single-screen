	.extern ov70_02238F9C
	.extern ov70_0224190C
	.extern ov70_02242FC4
	.extern ov70_02243E8C
	.extern ov70_02243EB8
	.extern ov70_02243F54
	.extern ov70_02243F7C
	.extern ov70_02245E26
	.extern ov70_02245E27
	.extern ov70_02245E84
	.extern ov70_02245F58
	.extern ov70_02245F5C
	.extern ov70_02245FA0
#include "config.h"
#include "constants/pokemon.h"
	.include "asm/macros.inc"
	.include "overlay_70.inc"
	.include "global.inc"

.text
	thumb_func_start ov70_02243254
ov70_02243254: ; 0x02243254
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #8
	add r5, r0, #0
	mov r0, #1
	str r0, [sp]
	ldr r0, [r5, #0x1c]
	mov r1, #0
	mov r2, #0x64
	mov r3, #0x20
	bl sub_02019688
	ldr r0, [r5, #0x1c]
	mov r1, #0
	bl sub_02019B08
	add r0, r5, #0
	mov r1, #5
	bl ov70_0224190C
	add r0, r5, #0
	add r0, #0x64
	mov r1, #1
	mov r2, #0x1a
	bl MI_CpuFill8
	mov r0, #2
	str r0, [sp]
	ldr r0, _02243388 ; =0x000F0E02
	mov r3, #0x3c
	str r0, [sp, #4]
	ldrsh r3, [r5, r3]
	ldr r0, [r5, #0x1c]
	ldr r1, [r5, #0x24]
	ldr r2, [r5, #4]
	add r3, #0x6e
	bl ov70_02243E8C
	mov r0, #0x3c
	ldrsh r1, [r5, r0]
	ldr r0, _0224338C ; =ov70_02245F5C
	mov r4, #1
	lsl r2, r1, #3
	ldr r0, [r0, r2]
	cmp r0, #1
	blt _0224332C
	mov r7, #0x10
_022432B0:
	lsl r2, r1, #2
	ldr r1, _02243390 ; =ov70_02245E84
	add r0, r5, #0
	ldr r1, [r1, r2]
	add r1, r4, r1
	sub r1, r1, #1
	bl ov70_02243F54
	cmp r0, #0
	ble _022432DA
	mov r1, #0x3c
	ldrsh r1, [r5, r1]
	ldr r0, _02243388 ; =0x000F0E02
	lsl r2, r1, #2
	ldr r1, _02243390 ; =ov70_02245E84
	ldr r2, [r1, r2]
	add r1, r5, r4
	add r2, r2, r1
	add r2, #0x63
	mov r1, #1
	b _022432EE
_022432DA:
	mov r1, #0x3c
	ldrsh r1, [r5, r1]
	ldr r0, _02243394 ; =0x00080902
	lsl r2, r1, #2
	ldr r1, _02243390 ; =ov70_02245E84
	ldr r2, [r1, r2]
	add r1, r5, r4
	add r2, r2, r1
	add r2, #0x63
	mov r1, #0
_022432EE:
	strb r1, [r2]
	mov r1, #5
	str r1, [sp]
	str r0, [sp, #4]
	mov r3, #0x3c
	ldrsh r3, [r5, r3]
	ldr r2, [r5, #4]
	ldr r0, [r5, #0x1c]
	lsl r6, r3, #3
	ldr r3, _02243398 ; =ov70_02245F58
	ldr r1, [r5, #0x24]
	ldr r3, [r3, r6]
	add r2, r2, r7
	add r3, r4, r3
	lsl r6, r3, #2
	ldr r3, _0224339C ; =ov70_02245FA0
	add r3, r3, r6
	sub r3, r3, #4
	ldr r3, [r3]
	bl ov70_02243E8C
	mov r0, #0x3c
	ldrsh r1, [r5, r0]
	ldr r0, _02243398 ; =ov70_02245F58
	add r4, r4, #1
	lsl r2, r1, #3
	add r0, r0, r2
	ldr r0, [r0, #4]
	add r7, #0x10
	cmp r4, r0
	ble _022432B0
_0224332C:
	ldr r2, [r5, #4]
	ldr r0, [r5, #0x1c]
	ldr r1, [r5, #0x24]
	add r2, #0xe0
	mov r3, #0x44
	bl ov70_02243EB8
	mov r1, #0
	ldr r0, [r5, #0x1c]
	mov r2, #0x10
	add r3, r1, #0
	bl sub_020196E8
	ldr r0, [r5, #0xc]
	mov r1, #0x2f
	bl Sprite_SetAnimCtrlSeq
	mov r0, #0x3e
	ldrsh r0, [r5, r0]
	cmp r0, #0
	bge _0224335A
	mov r0, #0
	b _0224335A
_0224335A:
	str r0, [r5, #0x48]
	ldr r0, [r5, #0x48]
	ldr r1, _022433A0 ; =ov70_02245E26
	lsl r3, r0, #1
	ldr r2, _022433A4 ; =ov70_02245E27
	ldrb r1, [r1, r3]
	ldrb r2, [r2, r3]
	ldr r0, [r5, #0xc]
	add r1, #0x10
	lsl r1, r1, #3
	lsl r2, r2, #3
	bl ov70_02238F9C
	ldr r0, [r5, #0xc]
	mov r1, #1
	bl Sprite_SetDrawFlag
	mov r0, #9
	str r0, [r5, #0x4c]
	sub r0, #0xa
	add sp, #8
	pop {r3, r4, r5, r6, r7, pc}
	nop
_02243388: .word 0x000F0E02
_0224338C: .word ov70_02245F5C
_02243390: .word ov70_02245E84
_02243394: .word 0x00080902
_02243398: .word ov70_02245F58
_0224339C: .word ov70_02245FA0
_022433A0: .word ov70_02245E26
_022433A4: .word ov70_02245E27
	thumb_func_end ov70_02243254
