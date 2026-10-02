	.extern ov70_02238E50
	.extern ov70_0224182C
	.extern ov70_02241868
	.extern ov70_022418A4
	.extern ov70_02245D50
	.include "asm/macros.inc"
	.include "overlay_70.inc"
	.include "global.inc"

.text
	thumb_func_start ov70_02241648
ov70_02241648: ; 0x02241648
	push {r4, r5, r6, r7, lr}
	sub sp, #0x2c
	add r5, r0, #0
	ldr r0, [r5, #0x2c]
	mov r4, #3
	cmp r0, #0
	beq _0224165E
	cmp r0, #1
	bne _0224165C
	b _022417C8
_0224165C:
	b _022417FE
_0224165E:
	add r0, r5, #0
	add r0, #0xb8
	ldr r0, [r0]
	bl OverlayManager_Run
	cmp r0, #0
	bne _0224166E
	b _022417FE
_0224166E:
	add r0, r5, #0
	add r0, #0xb8
	ldr r0, [r0]
	bl OverlayManager_Delete
	ldr r1, [r5, #0x24]
	cmp r1, #9
	bne _022416E8
	add r0, r5, #0
	bl ov70_02241868
	mov r1, #6
	mov r2, #0
	add r6, r0, #0
	bl GetMonData
	add r3, r0, #0
	add r0, sp, #0x28
	lsl r3, r3, #0x10
	str r0, [sp]
	mov r0, #0
	add r1, r6, #0
	mov r2, #1
	lsr r3, r3, #0x10
	bl GetMonEvolution
	add r2, r0, #0
	beq _022416DA
	ldr r3, [r5]
	add r1, r6, #0
	ldr r0, [r3, #0x38]
	str r0, [sp]
	ldr r0, [r3, #0x10]
	str r0, [sp, #4]
	ldr r0, [r3, #0x2c]
	str r0, [sp, #8]
	ldr r0, [r3, #0x28]
	str r0, [sp, #0xc]
	ldr r0, [sp, #0x28]
	str r0, [sp, #0x10]
	mov r0, #4
	str r0, [sp, #0x14]
	mov r0, #0x3d
	str r0, [sp, #0x18]
	ldr r3, [r3, #0x24]
	mov r0, #0
	bl sub_02075A7C
	mov r1, #0x11
	lsl r1, r1, #4
	str r0, [r5, r1]
	mov r0, #1
	str r0, [r5, #0x2c]
	b _022417FE
_022416DA:
	add r0, r5, #0
	mov r1, #1
	mov r2, #0
	bl ov70_02238E50
	mov r4, #4
	b _022417FE
_022416E8:
	cmp r1, #8
	beq _022416F0
	cmp r1, #0xa
	bne _022417BA
_022416F0:
	add r0, r5, #0
	bl ov70_02241868
	add r6, r0, #0
	mov r0, #0x3d
	bl AllocMonZeroed
	add r7, r0, #0
	ldr r0, [r5]
	add r1, r7, #0
	ldr r0, [r0]
	bl sub_0202DB64
	add r0, r6, #0
	mov r1, #5
	mov r2, #0
	bl GetMonData
	str r0, [sp, #0x1c]
	add r0, r7, #0
	mov r1, #5
	mov r2, #0
	bl GetMonData
	ldr r1, [sp, #0x1c]
	cmp r1, r0
	bne _02241742
	mov r1, #0
	add r0, r6, #0
	add r2, r1, #0
	bl GetMonData
	mov r1, #0
	str r0, [sp, #0x20]
	add r0, r7, #0
	add r2, r1, #0
	bl GetMonData
	ldr r1, [sp, #0x20]
	cmp r1, r0
	beq _022417A6
_02241742:
	add r0, r6, #0
	mov r1, #6
	mov r2, #0
	bl GetMonData
	add r3, r0, #0
	add r0, sp, #0x24
	lsl r3, r3, #0x10
	str r0, [sp]
	mov r0, #0
	add r1, r6, #0
	mov r2, #1
	lsr r3, r3, #0x10
	bl GetMonEvolution
	add r2, r0, #0
	beq _02241798
	ldr r3, [r5]
	add r1, r6, #0
	ldr r0, [r3, #0x38]
	str r0, [sp]
	ldr r0, [r3, #0x10]
	str r0, [sp, #4]
	ldr r0, [r3, #0x2c]
	str r0, [sp, #8]
	ldr r0, [r3, #0x28]
	str r0, [sp, #0xc]
	ldr r0, [sp, #0x24]
	str r0, [sp, #0x10]
	mov r0, #4
	str r0, [sp, #0x14]
	mov r0, #0x3d
	str r0, [sp, #0x18]
	ldr r3, [r3, #0x24]
	mov r0, #0
	bl sub_02075A7C
	mov r1, #0x11
	lsl r1, r1, #4
	str r0, [r5, r1]
	mov r0, #1
	str r0, [r5, #0x2c]
	b _022417B2
_02241798:
	add r0, r5, #0
	mov r1, #1
	mov r2, #0
	bl ov70_02238E50
	mov r4, #4
	b _022417B2
_022417A6:
	add r0, r5, #0
	mov r1, #1
	mov r2, #0
	bl ov70_02238E50
	mov r4, #4
_022417B2:
	add r0, r7, #0
	bl Heap_Free
	b _022417FE
_022417BA:
	add r0, r5, #0
	mov r1, #1
	mov r2, #0
	bl ov70_02238E50
	mov r4, #4
	b _022417FE
_022417C8:
	mov r0, #0x11
	lsl r0, r0, #4
	ldr r0, [r5, r0]
	bl sub_02075D3C
	cmp r0, #0
	beq _022417FE
	mov r0, #0x11
	lsl r0, r0, #4
	ldr r0, [r5, r0]
	bl sub_02075D4C
	add r0, r5, #0
	bl ov70_022418A4
	mov r2, #1
	lsl r2, r2, #0x1a
	ldr r1, [r2]
	ldr r0, _02241804 ; =0xFFFF1FFF
	and r0, r1
	str r0, [r2]
	add r0, r5, #0
	mov r1, #7
	mov r2, #0xc
	bl ov70_02238E50
	mov r4, #4
_022417FE:
	add r0, r4, #0
	add sp, #0x2c
	pop {r4, r5, r6, r7, pc}
	.balign 4, 0
_02241804: .word 0xFFFF1FFF
	thumb_func_end ov70_02241648
