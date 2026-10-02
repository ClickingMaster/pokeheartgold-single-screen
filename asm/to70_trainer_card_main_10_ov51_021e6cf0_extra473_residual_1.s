#include "constants/sndseq.h"
	.include "asm/macros.inc"
	.include "overlay_trainer_card_main.inc"
	.include "global.inc"

.text
	thumb_func_start ov51_021E6D44
ov51_021E6D44: ; 0x021E6D44
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #8
	add r7, r1, #0
	mov r1, #1
	str r1, [sp]
	lsl r1, r7, #0x18
	mov r3, #3
	str r0, [sp, #4]
	lsr r1, r1, #0x18
	lsl r3, r3, #0xc
	bl BG_LoadCharTilesData
	lsl r1, r7, #0x18
	ldr r0, [sp, #4]
	lsr r1, r1, #0x18
	bl GetBgTilemapBuffer
	mov r4, #0
	add r1, r0, #0
	add r3, r4, #0
	add r0, r4, #0
_021E6D6E:
	add r5, r3, #0
	add r5, #0xc
	lsl r5, r5, #6
	add r2, r0, #0
	add r5, r1, r5
_021E6D78:
	lsl r6, r2, #1
	add r4, r4, #1
	add r2, r2, #1
	lsl r4, r4, #0x10
	lsl r2, r2, #0x18
	lsr r4, r4, #0x10
	add r6, r5, r6
	lsr r2, r2, #0x18
	strh r4, [r6, #8]
	cmp r2, #0x18
	blo _021E6D78
	add r2, r3, #1
	lsl r2, r2, #0x18
	lsr r3, r2, #0x18
	cmp r3, #8
	blo _021E6D6E
	lsl r1, r7, #0x18
	ldr r0, [sp, #4]
	lsr r1, r1, #0x18
	bl ScheduleBgTilemapBufferTransfer
	add sp, #8
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
	thumb_func_end ov51_021E6D44
