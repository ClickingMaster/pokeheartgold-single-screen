	.extern Heap_Alloc
	.extern Heap_Free
	.extern ov07_0221BFD0
	.extern ov07_0221C468
	.extern ov07_0221C470
	.extern ov07_0221C478
	.extern ov07_0221F9A8
	.extern ov07_022208B4
	.extern sub_02015504
	.include "asm/macros.inc"
	.include "overlay_07.inc"
	.include "global.inc"
	.public ov07_022215B4

.text
	thumb_func_start ov07_022215B4
ov07_022215B4: ; 0x022215B4
	push {r4, r5, r6, lr}
	add r6, r0, #0
	bl sub_02015504
	add r5, r0, #0
	bl ov07_0221BFD0
	mov r1, #0x38
	bl Heap_Alloc
	add r4, r0, #0
	str r5, [r4]
	add r0, r5, #0
	bl ov07_0221C478
	str r0, [r4, #4]
	add r1, r4, #0
	ldr r0, [r4]
	add r1, #0xc
	mov r2, #6
	bl ov07_0221F9A8
	ldr r0, [r4, #0x10]
	cmp r0, #4
	bhi _022215FC
	add r0, r0, r0
	add r0, pc
	ldrh r0, [r0, #6]
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	add pc, r0
_022215F2: ; jump table
	.short _022215FC - _022215F2 - 2 ; case 0
	.short _0222160E - _022215F2 - 2 ; case 1
	.short _02221620 - _022215F2 - 2 ; case 2
	.short _02221632 - _022215F2 - 2 ; case 3
	.short _02221644 - _022215F2 - 2 ; case 4
_022215FC:
	ldr r0, [r4]
	bl ov07_0221C468
	str r0, [r4, #0x24]
	ldr r0, [r4]
	bl ov07_0221C470
	str r0, [r4, #0x28]
	b _02221654
_0222160E:
	ldr r0, [r4]
	bl ov07_0221C470
	str r0, [r4, #0x24]
	ldr r0, [r4]
	bl ov07_0221C468
	str r0, [r4, #0x28]
	b _02221654
_02221620:
	ldr r0, [r4]
	bl ov07_0221C468
	str r0, [r4, #0x24]
	ldr r0, [r4]
	bl ov07_0221C470
	str r0, [r4, #0x28]
	b _02221654
_02221632:
	ldr r0, [r4]
	bl ov07_0221C470
	str r0, [r4, #0x24]
	ldr r0, [r4]
	bl ov07_0221C468
	str r0, [r4, #0x28]
	b _02221654
_02221644:
	ldr r0, [r4]
	bl ov07_0221C468
	str r0, [r4, #0x24]
	ldr r0, [r4]
	bl ov07_0221C470
	str r0, [r4, #0x28]
_02221654:
	add r0, r6, #0
	add r1, r4, #0
	bl ov07_022208B4
	add r0, r4, #0
	bl Heap_Free
	pop {r4, r5, r6, pc}
	thumb_func_end ov07_022215B4
