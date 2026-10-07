	.extern ov08_022245DC
	.extern ov08_02224684
	.extern ov08_022246F4
	.extern ov08_02225E9C
	.extern ov08_02225E9D
	.extern ov08_02225E9E
	.extern ov08_02225E9F
	.extern ov08_02225EE0
#include "constants/moves.h"
	.include "asm/macros.inc"
	.include "overlay_08.inc"
	.include "global.inc"

.text

	.force_active on
	thumb_func_start ov08_02224768
ov08_02224768: ; 0x02224768
	push {r4, r5, r6, r7, lr}
	sub sp, #0xc
	str r2, [sp, #4]
	str r3, [sp, #8]
	add r5, r1, #0
	ldr r1, [sp, #4]
	ldr r2, [sp, #8]
	str r0, [sp]
	bl ov08_022245DC
	add r3, sp, #0x10
	add r4, r0, #0
	ldrb r3, [r3, #0x10]
	ldr r0, [sp]
	ldr r1, [sp, #4]
	ldr r2, [sp, #8]
	bl ov08_02224684
	lsl r0, r0, #0x1c
	lsr r2, r0, #0x10
	ldr r0, [sp, #4]
	ldr r6, _022247D4 ; =ov08_02225E9F
	lsl r3, r0, #2
	ldr r0, _022247D8 ; =ov08_02225E9E
	ldrb r6, [r6, r3]
	ldrb r0, [r0, r3]
	mov r1, #0
	mul r6, r0
	cmp r6, #0
	ble _022247C4
	ldr r0, _022247DC ; =ov08_02225E9C
	ldr r7, _022247E0 ; =0x00000FFF
	add r3, r0, r3
_022247AA:
	lsl r0, r1, #1
	ldrh r6, [r4, r0]
	and r6, r7
	orr r6, r2
	strh r6, [r5, r0]
	add r0, r1, #1
	lsl r0, r0, #0x10
	lsr r1, r0, #0x10
	ldrb r6, [r3, #2]
	ldrb r0, [r3, #3]
	mul r0, r6
	cmp r1, r0
	blt _022247AA
_022247C4:
	ldr r0, [sp]
	ldr r2, [sp, #4]
	ldr r3, [sp, #8]
	add r1, r5, #0
	bl ov08_022246F4
	add sp, #0xc
	pop {r4, r5, r6, r7, pc}
	.balign 4, 0
_022247D4: .word ov08_02225E9F
_022247D8: .word ov08_02225E9E
_022247DC: .word ov08_02225E9C
_022247E0: .word 0x00000FFF
	thumb_func_end ov08_02224768

	thumb_func_start ov08_022247E4
ov08_022247E4: ; 0x022247E4
	push {r4, r5, r6, r7, lr}
	sub sp, #0x1c
	str r1, [sp, #0xc]
	str r2, [sp, #0x10]
	str r3, [sp, #0x14]
	add r5, r0, #0
	ldr r0, [sp, #0xc]
	lsl r4, r0, #2
	ldr r0, _02224848 ; =ov08_02225E9F
	ldrb r7, [r0, r4]
	ldr r0, _0222484C ; =ov08_02225E9E
	ldrb r6, [r0, r4]
	ldr r0, [r5]
	add r1, r6, #0
	mul r1, r7
	ldr r0, [r0, #0xc]
	lsl r1, r1, #1
	bl Heap_Alloc
	str r0, [sp, #0x18]
	ldr r0, [sp, #0x14]
	ldr r1, [sp, #0x18]
	str r0, [sp]
	ldr r2, [sp, #0xc]
	ldr r3, [sp, #0x10]
	add r0, r5, #0
	bl ov08_02224768
	ldr r0, _02224850 ; =ov08_02225E9D
	ldr r3, _02224854 ; =ov08_02225E9C
	ldrb r0, [r0, r4]
	ldrb r3, [r3, r4]
	ldr r2, [sp, #0x18]
	str r0, [sp]
	str r6, [sp, #4]
	str r7, [sp, #8]
	ldr r0, [r5, #4]
	mov r1, #6
	bl LoadRectToBgTilemapRect
	ldr r0, [r5, #4]
	mov r1, #6
	bl ScheduleBgTilemapBufferTransfer
	ldr r0, [sp, #0x18]
	bl Heap_Free
	add sp, #0x1c
	pop {r4, r5, r6, r7, pc}
	nop
_02224848: .word ov08_02225E9F
_0222484C: .word ov08_02225E9E
_02224850: .word ov08_02225E9D
_02224854: .word ov08_02225E9C
	thumb_func_end ov08_022247E4
