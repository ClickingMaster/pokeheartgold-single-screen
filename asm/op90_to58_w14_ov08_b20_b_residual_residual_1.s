	.extern ov08_02225A54
	.extern ov08_02225A55
	.extern ov08_02225A56
	.extern ov08_02225A57
#include "constants/moves.h"
	.include "asm/macros.inc"
	.include "overlay_08.inc"
	.include "global.inc"
	.extern ov08_02221D2C

.text
.force_active on
	thumb_func_start ov08_02221E6C
ov08_02221E6C: ; 0x02221E6C
	push {r4, r5, r6, r7, lr}
	sub sp, #0x1c
	str r1, [sp, #0xc]
	str r2, [sp, #0x10]
	str r3, [sp, #0x14]
	add r5, r0, #0
	ldr r0, [sp, #0xc]
	lsl r4, r0, #2
	ldr r0, _02221ED8 ; =ov08_02225A57
	ldrb r7, [r0, r4]
	ldr r0, _02221EDC ; =ov08_02225A56
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
	bl ov08_02221D2C
	ldr r0, _02221EE0 ; =ov08_02225A55
	ldr r3, _02221EE4 ; =ov08_02225A54
	ldrb r0, [r0, r4]
	ldrb r3, [r3, r4]
	ldr r2, [sp, #0x18]
	str r0, [sp]
	str r6, [sp, #4]
	mov r0, #0x79
	str r7, [sp, #8]
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	mov r1, #6
	bl LoadRectToBgTilemapRect
	mov r0, #0x79
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	mov r1, #6
	bl ScheduleBgTilemapBufferTransfer
	ldr r0, [sp, #0x18]
	bl Heap_Free
	add sp, #0x1c
	pop {r4, r5, r6, r7, pc}
	nop
_02221ED8: .word ov08_02225A57
_02221EDC: .word ov08_02225A56
_02221EE0: .word ov08_02225A55
_02221EE4: .word ov08_02225A54
	thumb_func_end ov08_02221E6C
