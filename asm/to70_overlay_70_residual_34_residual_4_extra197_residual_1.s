	.extern ov70_02238F9C
	.extern ov70_02242164
	.extern ov70_02245DC0
	.extern ov70_02245DC1
	.extern ov70_02245DC2
	.extern ov70_02245DC3
	.extern ov70_02245E26
	.extern ov70_02245E27
	.extern ov70_02245E84
	.extern ov70_02245F5C
#include "config.h"
#include "constants/pokemon.h"
	.include "asm/macros.inc"
	.include "overlay_70.inc"
	.include "global.inc"
	.extern ov70_02242364

.text
	thumb_func_start ov70_02242390
ov70_02242390: ; 0x02242390
	push {r4, r5, r6, lr}
	add r5, r0, #0
	add r4, r1, #0
	ldr r1, [r5, #0x48]
	cmp r1, #3
	bge _022423A0
	add r0, #0x7e
	strb r1, [r0]
_022423A0:
	ldr r0, _022424E4 ; =gSystem
	mov r2, #0x40
	ldr r0, [r0, #0x4c]
	tst r2, r0
	beq _022423B6
	ldr r0, [r5, #0x48]
	lsl r2, r0, #2
	ldr r0, _022424E8 ; =ov70_02245DC0
	ldrb r0, [r0, r2]
	str r0, [r5, #0x48]
	b _022423EA
_022423B6:
	mov r2, #0x80
	tst r2, r0
	beq _022423C8
	ldr r0, [r5, #0x48]
	lsl r2, r0, #2
	ldr r0, _022424EC ; =ov70_02245DC1
	ldrb r0, [r0, r2]
	str r0, [r5, #0x48]
	b _022423EA
_022423C8:
	mov r2, #0x20
	tst r2, r0
	beq _022423DA
	ldr r0, [r5, #0x48]
	lsl r2, r0, #2
	ldr r0, _022424F0 ; =ov70_02245DC2
	ldrb r0, [r0, r2]
	str r0, [r5, #0x48]
	b _022423EA
_022423DA:
	mov r2, #0x10
	tst r0, r2
	beq _022423EA
	ldr r0, [r5, #0x48]
	lsl r2, r0, #2
	ldr r0, _022424F4 ; =ov70_02245DC3
	ldrb r0, [r0, r2]
	str r0, [r5, #0x48]
_022423EA:
	cmp r1, #3
	bne _022423FC
	ldr r0, [r5, #0x48]
	cmp r0, #3
	bge _022423FC
	add r0, r5, #0
	add r0, #0x7e
	ldrb r0, [r0]
	str r0, [r5, #0x48]
_022423FC:
	ldr r0, [r5, #0x48]
	cmp r1, r0
	beq _02242440
	ldr r0, _022424F8 ; =0x000005DC
	bl PlaySE
	ldr r0, [r5, #0x48]
	cmp r0, #3
	bne _02242422
	ldr r0, [r5, #0xc]
	mov r1, #0xc0
	mov r2, #0x88
	bl ov70_02238F9C
	ldr r0, [r5, #0xc]
	mov r1, #0x30
	bl Sprite_SetAnimCtrlSeq
	b _02242440
_02242422:
	ldr r1, _022424FC ; =ov70_02245E26
	lsl r2, r0, #1
	ldrb r1, [r1, r2]
	ldr r3, _02242500 ; =ov70_02245E27
	ldr r0, [r5, #0xc]
	ldrb r2, [r3, r2]
	add r1, #0x10
	lsl r1, r1, #3
	lsl r2, r2, #3
	bl ov70_02238F9C
	ldr r0, [r5, #0xc]
	mov r1, #0x2f
	bl Sprite_SetAnimCtrlSeq
_02242440:
	add r0, r5, #0
	mov r1, #5
	bl ov70_02242164
	add r1, r0, #0
	mov r0, #0
	mvn r0, r0
	cmp r1, r0
	beq _02242488
	add r0, r5, #0
	bl ov70_02242364
	mov r1, #0
	add r6, r0, #0
	mvn r1, r1
	cmp r6, r1
	beq _022424E0
	sub r0, r1, #1
	cmp r6, r0
	beq _0224247E
	cmp r4, #0
	beq _0224247E
	mov r0, #0x3c
	ldrsh r0, [r5, r0]
	lsl r1, r0, #2
	ldr r0, _02242504 ; =ov70_02245E84
	ldr r1, [r0, r1]
	add r0, r4, r6
	ldrb r0, [r1, r0]
	cmp r0, #0
	beq _022424DC
_0224247E:
	ldr r0, _022424F8 ; =0x000005DC
	bl PlaySE
	add r0, r6, #0
	pop {r4, r5, r6, pc}
_02242488:
	ldr r0, _022424E4 ; =gSystem
	ldr r1, [r0, #0x48]
	mov r0, #1
	tst r0, r1
	beq _022424CA
	ldr r1, [r5, #0x48]
	add r0, r5, #0
	bl ov70_02242364
	mov r1, #0
	add r6, r0, #0
	mvn r1, r1
	cmp r6, r1
	beq _022424E0
	sub r0, r1, #1
	cmp r6, r0
	beq _022424C0
	cmp r4, #0
	beq _022424C0
	mov r0, #0x3c
	ldrsh r0, [r5, r0]
	lsl r1, r0, #2
	ldr r0, _02242504 ; =ov70_02245E84
	ldr r1, [r0, r1]
	add r0, r4, r6
	ldrb r0, [r1, r0]
	cmp r0, #0
	beq _022424DC
_022424C0:
	ldr r0, _022424F8 ; =0x000005DC
	bl PlaySE
	add r0, r6, #0
	pop {r4, r5, r6, pc}
_022424CA:
	mov r0, #2
	tst r0, r1
	beq _022424DC
	ldr r0, _022424F8 ; =0x000005DC
	bl PlaySE
	mov r0, #1
	mvn r0, r0
	pop {r4, r5, r6, pc}
_022424DC:
	mov r0, #0
	mvn r0, r0
_022424E0:
	pop {r4, r5, r6, pc}
	nop
_022424E4: .word gSystem
_022424E8: .word ov70_02245DC0
_022424EC: .word ov70_02245DC1
_022424F0: .word ov70_02245DC2
_022424F4: .word ov70_02245DC3
_022424F8: .word 0x000005DC
_022424FC: .word ov70_02245E26
_02242500: .word ov70_02245E27
_02242504: .word ov70_02245E84
	thumb_func_end ov70_02242390
