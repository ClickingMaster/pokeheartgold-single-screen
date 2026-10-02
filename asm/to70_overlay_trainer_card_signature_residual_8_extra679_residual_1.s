#include "msgdata/msg/msg_0252.h"
	.include "asm/macros.inc"
	.include "overlay_trainer_card_signature.inc"
	.include "global.inc"

.text
	thumb_func_start ov52_021E9260
ov52_021E9260: ; 0x021E9260
	ldr r1, _021E9278 ; =0x0000431C
	add r3, r0, r1
	add r1, #0x11
	add r2, r0, r1
	mov r1, #0x11
_021E926A:
	ldrb r0, [r3]
	add r3, r3, #1
	strb r0, [r2]
	add r2, r2, #1
	sub r1, r1, #1
	bne _021E926A
	bx lr
	.balign 4, 0
_021E9278: .word 0x0000431C
	thumb_func_end ov52_021E9260
