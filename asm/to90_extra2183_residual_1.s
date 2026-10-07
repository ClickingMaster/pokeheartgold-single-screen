.include "asm/macros.inc"
.include "overlay_12_battle_controller.inc"
.include "global.inc"

.text
	thumb_func_start BattleController_RecvData
BattleController_RecvData: ; 0x02262130
	push {r3, r4, r5, r6, r7, lr}
	add r3, r1, #0
	ldrb r6, [r3, #3]
	ldrb r1, [r3]
	ldrb r7, [r3, #2]
	lsl r6, r6, #8
	add r5, r0, #0
	mov r2, #0
	orr r6, r7
	ldrb r4, [r3, #1]
	cmp r1, #0
	bne _02262176
	ldr r0, [r5, #0x30]
	lsl r1, r4, #8
	add r4, r0, r1
	mov r0, #0x23
	lsl r0, r0, #8
	ldrb r0, [r4, r0]
	cmp r0, #0
	bne _022621C0
	cmp r6, #0
	ble _02262172
	mov r4, #0x23
	lsl r4, r4, #8
_02262160:
	add r0, r3, r2
	ldr r7, [r5, #0x30]
	ldrb r0, [r0, #4]
	add r7, r1, r7
	add r7, r2, r7
	add r2, r2, #1
	strb r0, [r7, r4]
	cmp r2, r6
	blt _02262160
_02262172:
	mov r2, #1
	b _022621C0
_02262176:
	cmp r1, #1
	bne _022621A2
	lsl r0, r4, #2
	add r0, r5, r0
	ldr r1, [r0, #0x34]
	add r1, #0x94
	ldrb r1, [r1]
	cmp r1, #0
	bne _022621C0
	cmp r6, #0
	ble _0226219E
_0226218C:
	add r1, r3, r2
	ldrb r4, [r1, #4]
	ldr r1, [r0, #0x34]
	add r1, r1, r2
	add r1, #0x94
	add r2, r2, #1
	strb r4, [r1]
	cmp r2, r6
	blt _0226218C
_0226219E:
	mov r2, #1
	b _022621C0
_022621A2:
	cmp r1, #2
	bne _022621C0
	ldrb r7, [r3, #4]
	ldrb r6, [r3, #5]
	bl ov12_0223B688
	cmp r0, #0
	beq _022621BE
	ldr r0, [r5, #0x30]
	add r1, r6, #0
	add r2, r4, #0
	add r3, r7, #0
	bl ov12_0224ED00
_022621BE:
	mov r2, #1
_022621C0:
	add r0, r2, #0
	pop {r3, r4, r5, r6, r7, pc}
	thumb_func_end BattleController_RecvData
