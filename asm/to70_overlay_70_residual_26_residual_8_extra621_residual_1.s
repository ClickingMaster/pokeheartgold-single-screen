	.extern ov70_022459F4
	.extern ov70_02245A4C
#include "config.h"
#include "constants/pokemon.h"
	.include "asm/macros.inc"
	.include "overlay_70.inc"
	.include "global.inc"

.text
	thumb_func_start ov70_0223F864
ov70_0223F864: ; 0x0223F864
	push {r4, r5, r6, r7}
	cmp r2, #0
	bne _0223F870
	ldr r4, _0223F8A0 ; =ov70_02245A4C
	mov r3, #0xc
	b _0223F874
_0223F870:
	ldr r4, _0223F8A4 ; =ov70_022459F4
	mov r3, #0xb
_0223F874:
	mov r2, #0
	cmp r3, #0
	ble _0223F898
	mov r5, #6
	mov r6, #4
_0223F87E:
	ldrsh r7, [r4, r6]
	cmp r0, r7
	bne _0223F890
	ldrsh r7, [r4, r5]
	cmp r1, r7
	bne _0223F890
	add r0, r2, #0
	pop {r4, r5, r6, r7}
	bx lr
_0223F890:
	add r2, r2, #1
	add r4, #8
	cmp r2, r3
	blt _0223F87E
_0223F898:
	mov r0, #0
	pop {r4, r5, r6, r7}
	bx lr
	nop
_0223F8A0: .word ov70_02245A4C
_0223F8A4: .word ov70_022459F4
	thumb_func_end ov70_0223F864
