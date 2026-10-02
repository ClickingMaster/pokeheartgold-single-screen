	.extern ov39_02227DEC
	.extern ov39_02227E3C
#include "config.h"
	.include "asm/macros.inc"
	.include "overlay_39_thumb.inc"
	.include "global.inc"

.text
	thumb_func_start ov39_02227208
ov39_02227208: ; 0x02227208
	push {r4, r5, r6, lr}
	ldr r2, _022272E4 ; =0x00000414
	add r4, r1, #0
	mov r1, #0
	add r5, r0, #0
	bl MI_CpuFill8
	mov r0, #0x51
	add r3, r4, #0
	ldr r1, [r4, #4]
	lsl r0, r0, #2
	str r1, [r5, r0]
	ldr r1, [r4, #8]
	add r0, #0xc
	str r1, [r5]
	add r3, #0xc
	add r2, r5, r0
	mov r6, #5
_0222722C:
	ldmia r3!, {r0, r1}
	stmia r2!, {r0, r1}
	sub r6, r6, #1
	bne _0222722C
	ldr r0, [r3]
	mov r1, #0x53
	str r0, [r2]
	ldr r0, [r4, #0x3c]
	lsl r1, r1, #2
	str r0, [r5, #8]
	ldr r0, [r4, #0x38]
	ldr r2, _022272E8 ; =0x000059DC
	str r0, [r5, #4]
	mov r0, #0
	mvn r0, r0
	str r0, [r5, r1]
	mov r1, #0xfa
	lsl r1, r1, #2
	str r2, [r5, r1]
	add r1, r1, #4
	str r2, [r5, r1]
	bl ov39_0222A2C0
	add r1, r0, #0
	ldr r0, [r4, #4]
	bl Heap_Alloc
	mov r1, #0xed
	lsl r1, r1, #2
	str r0, [r5, r1]
	mov r0, #0
	mvn r0, r0
	bl ov39_0222A2C0
	add r2, r0, #0
	mov r0, #0xed
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	mov r1, #0
	bl MI_CpuFill8
	mov r2, #0x32
	ldr r3, [r4, #4]
	mov r0, #0
	mov r1, #0x1b
	lsl r2, r2, #4
	bl NewMsgDataFromNarc
	mov r1, #0xfd
	lsl r1, r1, #2
	str r0, [r5, r1]
	ldr r0, [r4, #4]
	bl MessageFormat_New
	mov r1, #0xfe
	lsl r1, r1, #2
	str r0, [r5, r1]
	mov r0, #1
	ldr r1, [r4, #4]
	lsl r0, r0, #8
	bl String_New
	mov r1, #0xff
	lsl r1, r1, #2
	str r0, [r5, r1]
	ldr r0, [r4, #4]
	bl ov39_02227DEC
	mov r1, #0x63
	lsl r1, r1, #2
	str r0, [r5, r1]
	ldr r0, [r4]
	str r0, [r5, #0xc]
	mov r0, #GAME_VERSION
	strb r0, [r5, #0x10]
	mov r0, #2
	strb r0, [r5, #0x11]
	add r0, r5, #0
	add r0, #0xc
	bl ov39_0222A110
	cmp r0, #1
	beq _022272D6
	bl GF_AssertFail
_022272D6:
	mov r1, #0xf1
	mov r0, #1
	lsl r1, r1, #2
	str r0, [r5, r1]
	sub r1, #0xc
	str r0, [r5, r1]
	pop {r4, r5, r6, pc}
	.balign 4, 0
_022272E4: .word 0x00000414
_022272E8: .word 0x000059DC
	thumb_func_end ov39_02227208
