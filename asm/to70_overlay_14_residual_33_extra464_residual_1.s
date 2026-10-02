	.extern ov14_021E6070
	.extern ov14_021E6480
	.extern ov14_021E7588
	.extern ov14_021E765C
	.extern ov14_021E8328
	.extern ov14_021E84A4
	.extern ov14_021E8544
	.extern ov14_021E884C
	.extern ov14_021E96C8
	.extern ov14_021EA130
	.extern ov14_021F0234
	.extern ov14_021F0244
	.extern ov14_021F0660
	.extern ov14_021F0794
	.extern ov14_021F2270
	.extern ov14_021F2A18
	.extern ov14_021F3488
	.extern ov14_021F40DC
	.extern ov14_021F40E8
	.extern ov14_021F5EB4
	.extern ov14_021F5EE4
	.extern ov14_021F6654
	.extern ov14_021F67B0
	.extern ov14_021F685C
	.extern ov14_021F6A24
	.extern ov14_021F6BC0
	.extern ov14_021F7AC4
	.extern ov14_021F7D1C
#include "constants/pokemon.h"
	.include "asm/macros.inc"
	.include "overlay_14.inc"
	.include "global.inc"

.text
	thumb_func_start ov14_021EDFA0
ov14_021EDFA0: ; 0x021EDFA0
	push {r3, r4, r5, lr}
	add r4, r0, #0
	bl ov14_021F6A24
	add r5, r0, #0
	mov r0, #0
	mvn r0, r0
	cmp r5, r0
	beq _021EE0A4
	add r1, r5, #0
	add r0, r4, #0
	add r1, #0x1e
	mov r2, #0xac
	mov r3, #0
	bl ov14_021E6070
	cmp r0, #0
	beq _021EE012
	ldr r2, [r4, #0x34]
	ldr r1, _021EE254 ; =0x000040B8
	add r0, r2, r1
	add r1, r1, #4
	add r1, r2, r1
	bl System_GetTouchNewCoords
	mov r0, #0x2f
	ldr r1, [r4, #0x34]
	lsl r0, r0, #4
	ldr r0, [r1, r0]
	bl ov14_021E8544
	cmp r0, #0
	bne _021EDFEC
	ldr r1, _021EE258 ; =ov14_021F7D1C
	add r0, r4, #0
	mov r2, #4
	bl ov14_021F5EE4
_021EDFEC:
	ldr r0, _021EE25C ; =0x000005EB
	bl PlaySE
	add r1, r5, #0
	add r0, r4, #0
	add r1, #0x1e
	bl ov14_021E7588
	ldr r0, [r4, #0x34]
	mov r1, #9
	mov r2, #0
	bl ov14_021F2A18
	add r5, #0x1e
	add r0, r4, #0
	add r1, r5, #0
	bl ov14_021F0660
	pop {r3, r4, r5, pc}
_021EE012:
	mov r0, #0x2f
	ldr r1, [r4, #0x34]
	lsl r0, r0, #4
	ldr r0, [r1, r0]
	bl ov14_021E8544
	cmp r0, #1
	bne _021EE084
	add r0, r4, #0
	add r0, #0x21
	ldrb r0, [r0]
	sub r0, #0x1e
	lsl r0, r0, #0x18
	lsr r5, r0, #0x18
	ldr r0, [r4, #0x34]
	ldr r0, [r0, #0x2c]
	bl GridInputHandler_GetNextInput
	add r2, r0, #0
	ldr r0, [r4, #0x34]
	add r1, r5, #0
	bl ov14_021F7AC4
	ldr r0, [r4, #0x34]
	add r1, r5, #0
	ldr r0, [r0, #0x2c]
	bl GridInputHandler_SetNextInput
	add r0, r4, #0
	mov r1, #0
	bl ov14_021F5EB4
	mov r0, #0x2f
	ldr r1, [r4, #0x34]
	lsl r0, r0, #4
	ldr r0, [r1, r0]
	bl ov14_021E84A4
	mov r0, #0x2f
	ldr r1, [r4, #0x34]
	lsl r0, r0, #4
	ldr r0, [r1, r0]
	bl ov14_021E8328
	ldr r0, [r4, #0x34]
	bl ov14_021E884C
	add r0, r4, #0
	mov r1, #0
	bl ov14_021F40E8
	ldr r1, _021EE260 ; =ov14_021EA130
	add r0, r4, #0
	mov r2, #0x70
	bl ov14_021F0234
	pop {r3, r4, r5, pc}
_021EE084:
	ldr r0, [r4, #0x34]
	lsl r1, r5, #0x18
	ldr r0, [r0, #0x2c]
	lsr r1, r1, #0x18
	bl GridInputHandler_SetNextInput
	ldr r0, [r4, #0x34]
	mov r1, #1
	ldr r0, [r0, #0x2c]
	bl GridInputHandler_SetButtonInputMode
	add r0, r4, #0
	bl ov14_021E765C
	mov r0, #0x5b
	pop {r3, r4, r5, pc}
_021EE0A4:
	add r0, r4, #0
	bl ov14_021F6BC0
	mov r1, #2
	add r5, r0, #0
	mvn r1, r1
	cmp r5, r1
	bhi _021EE0E8
	bhs _021EE1AA
	cmp r5, #0xb
	bhi _021EE0DE
	add r0, r5, r5
	add r0, pc
	ldrh r0, [r0, #6]
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	add pc, r0
_021EE0C6: ; jump table
	.short _021EE1FA - _021EE0C6 - 2 ; case 0
	.short _021EE1FA - _021EE0C6 - 2 ; case 1
	.short _021EE1FA - _021EE0C6 - 2 ; case 2
	.short _021EE1FA - _021EE0C6 - 2 ; case 3
	.short _021EE1FA - _021EE0C6 - 2 ; case 4
	.short _021EE1FA - _021EE0C6 - 2 ; case 5
	.short _021EE1D0 - _021EE0C6 - 2 ; case 6
	.short _021EE0FC - _021EE0C6 - 2 ; case 7
	.short _021EE10E - _021EE0C6 - 2 ; case 8
	.short _021EE124 - _021EE0C6 - 2 ; case 9
	.short _021EE13E - _021EE0C6 - 2 ; case 10
	.short _021EE150 - _021EE0C6 - 2 ; case 11
_021EE0DE:
	mov r0, #3
	mvn r0, r0
	cmp r5, r0
	beq _021EE18A
	b _021EE1FA
_021EE0E8:
	add r0, r1, #1
	cmp r5, r0
	bhi _021EE0F2
	beq _021EE1E8
	b _021EE1FA
_021EE0F2:
	add r0, r1, #2
	cmp r5, r0
	bne _021EE0FA
	b _021EE24E
_021EE0FA:
	b _021EE1FA
_021EE0FC:
	ldr r0, _021EE264 ; =0x000005DD
	bl PlaySE
	add r0, r4, #0
	mov r1, #4
	mov r2, #0xa9
	bl ov14_021F2270
	pop {r3, r4, r5, pc}
_021EE10E:
	ldr r0, _021EE264 ; =0x000005DD
	bl PlaySE
	mov r0, #8
	str r0, [r4, #0x2c]
	add r0, r4, #0
	mov r1, #5
	mov r2, #0x97
	bl ov14_021F2270
	pop {r3, r4, r5, pc}
_021EE124:
	ldr r0, _021EE264 ; =0x000005DD
	bl PlaySE
	ldr r0, [r4, #0x34]
	mov r1, #0x27
	bl ov14_021F6654
	add r0, r4, #0
	mov r1, #6
	mov r2, #0x99
	bl ov14_021F2270
	pop {r3, r4, r5, pc}
_021EE13E:
	ldr r0, _021EE264 ; =0x000005DD
	bl PlaySE
	add r0, r4, #0
	mov r1, #7
	mov r2, #0x9b
	bl ov14_021F2270
	pop {r3, r4, r5, pc}
_021EE150:
	add r0, r4, #0
	add r0, #0x21
	ldrb r0, [r0]
	sub r0, #0x1e
	lsl r0, r0, #0x18
	lsr r5, r0, #0x18
	ldr r0, [r4, #0x34]
	ldr r0, [r0, #0x2c]
	bl GridInputHandler_GetNextInput
	add r2, r0, #0
	ldr r0, [r4, #0x34]
	add r1, r5, #0
	bl ov14_021F7AC4
	ldr r0, [r4, #0x34]
	add r1, r5, #0
	ldr r0, [r0, #0x2c]
	bl GridInputHandler_SetNextInput
	ldr r0, _021EE268 ; =0x000005DC
	bl PlaySE
	add r0, r4, #0
	mov r1, #0xb
	mov r2, #0xaa
	bl ov14_021F2270
	pop {r3, r4, r5, pc}
_021EE18A:
	ldr r0, [r4, #0x34]
	ldr r0, [r0, #0x2c]
	bl GridInputHandler_GetNextInput
	add r1, r0, #0
	cmp r1, #6
	bhi _021EE1A0
	add r0, r4, #0
	add r1, #0x1e
	bl ov14_021E7588
_021EE1A0:
	ldr r0, _021EE268 ; =0x000005DC
	bl PlaySE
	mov r0, #0x5b
	pop {r3, r4, r5, pc}
_021EE1AA:
	ldr r0, [r4, #0x34]
	ldr r0, [r0, #0x2c]
	bl GridInputHandler_GetNextInput
	add r1, r0, #0
	cmp r1, #6
	bhi _021EE1C0
	add r0, r4, #0
	add r1, #0x1e
	bl ov14_021E7588
_021EE1C0:
	ldr r0, _021EE268 ; =0x000005DC
	bl PlaySE
	add r0, r4, #0
	mov r1, #0x6f
	bl ov14_021F0244
	pop {r3, r4, r5, pc}
_021EE1D0:
	ldr r0, _021EE264 ; =0x000005DD
	bl PlaySE
	add r0, r4, #0
	bl ov14_021E765C
	add r0, r4, #0
	mov r1, #0xa
	mov r2, #0x93
	bl ov14_021F2270
	pop {r3, r4, r5, pc}
_021EE1E8:
	ldr r0, _021EE264 ; =0x000005DD
	bl PlaySE
	add r0, r4, #0
	mov r1, #0xa
	mov r2, #0x94
	bl ov14_021F2270
	pop {r3, r4, r5, pc}
_021EE1FA:
	add r1, r5, #0
	add r0, r4, #0
	add r1, #0x1e
	mov r2, #0xac
	mov r3, #0
	bl ov14_021E6070
	cmp r0, #0
	beq _021EE24E
	ldr r0, _021EE264 ; =0x000005DD
	bl PlaySE
	ldr r1, _021EE258 ; =ov14_021F7D1C
	add r0, r4, #0
	mov r2, #4
	bl ov14_021F5EE4
	add r1, r5, #0
	add r0, r4, #0
	add r1, #0x1e
	bl ov14_021E7588
	ldr r0, [r4, #0x34]
	ldr r0, [r0, #0x2c]
	bl GridInputHandler_GetNextInput
	add r2, r0, #0
	ldr r0, [r4, #0x34]
	mov r1, #7
	bl ov14_021F7AC4
	ldr r0, [r4, #0x34]
	mov r1, #7
	ldr r0, [r0, #0x2c]
	bl GridInputHandler_SetNextInput
	add r5, #0x1e
	add r0, r4, #0
	add r1, r5, #0
	bl ov14_021F0794
	pop {r3, r4, r5, pc}
_021EE24E:
	mov r0, #0x5b
	pop {r3, r4, r5, pc}
	nop
_021EE254: .word 0x000040B8
_021EE258: .word ov14_021F7D1C
_021EE25C: .word 0x000005EB
_021EE260: .word ov14_021EA130
_021EE264: .word 0x000005DD
_021EE268: .word 0x000005DC
	thumb_func_end ov14_021EDFA0
