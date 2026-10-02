	.extern ov52_021E9424
#include "msgdata/msg/msg_0252.h"
	.include "asm/macros.inc"
	.include "overlay_trainer_card_signature.inc"
	.include "global.inc"

.text
	thumb_func_start ov52_021E9364
ov52_021E9364: ; 0x021E9364
	push {r3, r4, r5, lr}
	ldr r1, _021E9410 ; =gSystem + 0x40
	add r4, r0, #0
	ldrh r0, [r1, #0x20]
	ldr r3, _021E9414 ; =0x0000FFFF
	cmp r0, r3
	beq _021E93FE
	ldrh r1, [r1, #0x22]
	cmp r1, r3
	beq _021E93FE
	ldr r2, _021E9418 ; =0x00005CA8
	ldr r5, [r4, r2]
	cmp r5, r3
	beq _021E93FE
	add r1, r2, #4
	ldr r1, [r4, r1]
	cmp r1, r3
	beq _021E93FE
	cmp r5, r0
	ble _021E9394
	mov r1, #0
	sub r3, r5, r0
	mvn r1, r1
	b _021E9398
_021E9394:
	sub r3, r0, r5
	mov r1, #1
_021E9398:
	sub r0, r2, #6
	strb r1, [r4, r0]
	cmp r3, #3
	blt _021E93CE
	cmp r3, #0x28
	bgt _021E93CE
	ldr r0, _021E9410 ; =gSystem + 0x40
	ldrh r2, [r0, #0x22]
	ldr r0, _021E941C ; =0x00005CAC
	ldr r1, [r4, r0]
	cmp r1, r2
	ble _021E93B8
	sub r2, r1, r2
	mov r1, #0
	mvn r1, r1
	b _021E93BC
_021E93B8:
	sub r2, r2, r1
	mov r1, #1
_021E93BC:
	sub r0, #9
	strb r1, [r4, r0]
	cmp r2, #0x28
	bgt _021E93FE
	ldr r0, _021E9420 ; =0x00005CA0
	add r0, r4, r0
	bl ov52_021E9424
	b _021E93FE
_021E93CE:
	cmp r3, #0x28
	bgt _021E93FE
	ldr r0, _021E9410 ; =gSystem + 0x40
	ldrh r2, [r0, #0x22]
	ldr r0, _021E941C ; =0x00005CAC
	ldr r1, [r4, r0]
	cmp r1, r2
	ble _021E93E6
	sub r2, r1, r2
	mov r1, #0
	mvn r1, r1
	b _021E93EA
_021E93E6:
	sub r2, r2, r1
	mov r1, #1
_021E93EA:
	sub r0, #9
	strb r1, [r4, r0]
	cmp r2, #3
	blt _021E93FE
	cmp r2, #0x28
	bgt _021E93FE
	ldr r0, _021E9420 ; =0x00005CA0
	add r0, r4, r0
	bl ov52_021E9424
_021E93FE:
	ldr r1, _021E9410 ; =gSystem + 0x40
	ldr r0, _021E9418 ; =0x00005CA8
	ldrh r2, [r1, #0x20]
	str r2, [r4, r0]
	ldrh r1, [r1, #0x22]
	add r0, r0, #4
	str r1, [r4, r0]
	pop {r3, r4, r5, pc}
	nop
_021E9410: .word gSystem + 0x40
_021E9414: .word 0x0000FFFF
_021E9418: .word 0x00005CA8
_021E941C: .word 0x00005CAC
_021E9420: .word 0x00005CA0
	thumb_func_end ov52_021E9364
