#include "config.h"
	.include "asm/macros.inc"
	.include "overlay_112.inc"
	.include "global.inc"
	.public gApplication_TitleScreen

	.extern ov112_021E77E4
	.extern ov112_021EF15C
	.extern ov112_021EF17C
	.extern ov112_021EF19C
	.extern ov112_021EF1CC
	.extern ov112_021EF298
	.extern ov112_021EF300
	.extern ov112_021EF310
	.extern ov112_021EF3F8
	.extern ov112_021F039C
	.extern ov112_021F03BC

	.text

	thumb_func_start ov112_021EEF68
ov112_021EEF68: ; 0x021EEF68
	push {r3, r4, r5, r6, lr}
	sub sp, #0x24
	ldr r6, _021EF028 ; =0x0001D764
	add r5, r0, #0
	ldr r1, [r5, r6]
	cmp r1, #0
	beq _021EEF7C
	cmp r1, #1
	beq _021EEF86
	b _021EF022
_021EEF7C:
	bl ov112_021E77E4
	mov r0, #1
	str r0, [r5, r6]
	b _021EF022
_021EEF86:
	mov r0, #0x9a
	bl BgConfig_Alloc
	str r0, [r5, #0x18]
	bl ov112_021EF15C
	mov r0, #0x9a
	mov r1, #1
	mov r2, #0x12
	bl sub_020932E0
	add r1, r6, #4
	str r0, [r5, r1]
	mov r0, #0x9a
	bl ov112_021F039C
	add r1, r6, #0
	add r1, #8
	str r0, [r5, r1]
	ldr r0, [r5, #0x18]
	bl ov112_021EF17C
	bl ov112_021EF19C
	add r0, r6, #0
	add r0, r5, r0
	bl ov112_021EF1CC
	mov r1, #0
	add r0, r6, #0
	str r1, [r5, r0]
	ldr r0, [r5, #0x20]
	bl SaveArray_PCStorage_Get
	add r4, r0, #0
	ldr r0, [r5, #0x20]
	bl SaveArray_Party_Get
	mov r1, #0
	str r1, [sp]
	add r3, r0, #0
	str r1, [sp, #4]
	mov r0, #1
	str r0, [sp, #8]
	ldr r0, _021EF02C ; =ov112_021EF298
	str r1, [sp, #0xc]
	str r0, [sp, #0x10]
	ldr r0, _021EF030 ; =ov112_021EF300
	add r2, r4, #0
	str r0, [sp, #0x14]
	add r0, r6, #0
	add r0, #0xc
	add r1, r5, r0
	str r1, [sp, #0x18]
	ldr r1, _021EF034 ; =ov112_021EF3F8
	sub r0, #8
	str r1, [sp, #0x1c]
	str r5, [sp, #0x20]
	ldr r0, [r5, r0]
	ldr r1, [r5, #0x18]
	bl sub_02093440
	add r0, r6, #0
	add r0, #8
	ldr r0, [r5, r0]
	ldr r1, [r5, #0x18]
	bl ov112_021F03BC
	add r1, r6, #0
	ldr r0, _021EF038 ; =ov112_021EF310
	add r1, r5, r1
	bl Main_SetVBlankIntrCB
	mov r0, #0
	str r0, [r5, r6]
	add sp, #0x24
	mov r0, #2
	pop {r3, r4, r5, r6, pc}
_021EF022:
	mov r0, #1
	add sp, #0x24
	pop {r3, r4, r5, r6, pc}
	.balign 4, 0
_021EF028: .word 0x0001D764
_021EF02C: .word ov112_021EF298
_021EF030: .word ov112_021EF300
_021EF034: .word ov112_021EF3F8
_021EF038: .word ov112_021EF310
	thumb_func_end ov112_021EEF68
