	.include "asm/macros.inc"
	.include "unk_02037C94.inc"
	.include "global.inc"
	.text
	thumb_func_start CommState_WifiConnected
CommState_WifiConnected: ; 0x02038EB8
	push {r3, lr}
	mov r0, #1
	bl sub_0203786C
	mov r0, #0
	bl WifiConnection_Update
	cmp r0, #0xa
	blt _02038ED8
	cmp r0, #0x14
	bge _02038ED8
	ldr r0, _02038F60 ; =CommState_WifiCancelled
	mov r1, #0
	bl CommState_SetState
	b _02038F3A
_02038ED8:
	cmp r0, #0
	bge _02038EE6
	ldr r0, _02038F64 ; =CommState_WifiFailed
	mov r1, #0
	bl CommState_SetState
	b _02038F3A
_02038EE6:
	cmp r0, #0x17
	bne _02038EF4
	ldr r0, _02038F68 ; =CommState_WifiTimedOut
	mov r1, #0
	bl CommState_SetState
	b _02038F3A
_02038EF4:
	cmp r0, #0x18
	bne _02038F18
	ldr r0, _02038F6C ; =gCommState
	ldr r0, [r0]
	add r0, #0x53
	ldrb r0, [r0]
	cmp r0, #0
	beq _02038F0E
	ldr r0, _02038F64 ; =CommState_WifiFailed
	mov r1, #0
	bl CommState_SetState
	b _02038F3A
_02038F0E:
	ldr r0, _02038F70 ; =CommState_WifiDisconnected
	mov r1, #0
	bl CommState_SetState
	b _02038F3A
_02038F18:
	cmp r0, #0x15
	bne _02038F3A
	ldr r0, _02038F6C ; =gCommState
	ldr r0, [r0]
	add r0, #0x53
	ldrb r0, [r0]
	cmp r0, #0
	beq _02038F32
	ldr r0, _02038F64 ; =CommState_WifiFailed
	mov r1, #0
	bl CommState_SetState
	b _02038F3A
_02038F32:
	ldr r0, _02038F70 ; =CommState_WifiDisconnected
	mov r1, #0
	bl CommState_SetState
_02038F3A:
	ldr r0, _02038F6C ; =gCommState
	ldr r0, [r0]
	add r0, #0x53
	ldrb r0, [r0]
	cmp r0, #0
	beq _02038F5E
	bl Comm_CountConnected
	ldr r1, _02038F6C ; =gCommState
	ldr r1, [r1]
	add r1, #0x4b
	ldrb r1, [r1]
	cmp r1, r0
	beq _02038F5E
	ldr r0, _02038F64 ; =CommState_WifiFailed
	mov r1, #0
	bl CommState_SetState
_02038F5E:
	pop {r3, pc}
	.balign 4, 0
_02038F60: .word CommState_WifiCancelled
_02038F64: .word CommState_WifiFailed
_02038F68: .word CommState_WifiTimedOut
_02038F6C: .word gCommState
_02038F70: .word CommState_WifiDisconnected
	thumb_func_end CommState_WifiConnected
	thumb_func_start CommState_SetWifiError
CommState_SetWifiError: ; 0x02038F74
	push {r3, r4}
	ldr r3, _02038F98 ; =gCommState
	ldr r4, [r3]
	cmp r4, #0
	beq _02038F94
	add r3, r0, #0
	sub r3, #0x19
	cmp r3, #1
	bls _02038F88
	neg r0, r0
_02038F88:
	str r0, [r4, #0x34]
	ldr r0, _02038F98 ; =gCommState
	ldr r3, [r0]
	str r1, [r3, #0x38]
	ldr r0, [r0]
	str r2, [r0, #0x3c]
_02038F94:
	pop {r3, r4}
	bx lr
	.balign 4, 0
_02038F98: .word gCommState
	thumb_func_end CommState_SetWifiError
	thumb_func_start CommState_WifiLobby
CommState_WifiLobby: ; 0x02038F9C
	push {r3, lr}
	mov r0, #0
	bl WifiConnection_Update
	cmp r0, #0xa
	blt _02038FB6
	cmp r0, #0x14
	bge _02038FB6
	ldr r0, _02038FFC ; =CommState_WifiCancelled
	mov r1, #0
	bl CommState_SetState
	pop {r3, pc}
_02038FB6:
	cmp r0, #0
	bge _02038FC4
	ldr r0, _02039000 ; =CommState_WifiFailed
	mov r1, #0
	bl CommState_SetState
	pop {r3, pc}
_02038FC4:
	cmp r0, #0x14
	bne _02038FD2
	ldr r0, _02039004 ; =CommState_WifiConnected
	mov r1, #0
	bl CommState_SetState
	pop {r3, pc}
_02038FD2:
	cmp r0, #0x15
	bne _02038FE0
	ldr r0, _02039008 ; =CommState_WifiDisconnected
	mov r1, #0
	bl CommState_SetState
	pop {r3, pc}
_02038FE0:
	cmp r0, #0x16
	bne _02038FEE
	ldr r0, _02038FFC ; =CommState_WifiCancelled
	mov r1, #0
	bl CommState_SetState
	pop {r3, pc}
_02038FEE:
	cmp r0, #0x18
	bne _02038FFA
	ldr r0, _02039008 ; =CommState_WifiDisconnected
	mov r1, #0
	bl CommState_SetState
_02038FFA:
	pop {r3, pc}
	.balign 4, 0
_02038FFC: .word CommState_WifiCancelled
_02039000: .word CommState_WifiFailed
_02039004: .word CommState_WifiConnected
_02039008: .word CommState_WifiDisconnected
	thumb_func_end CommState_WifiLobby
	thumb_func_start CommState_WifiStartMatch
CommState_WifiStartMatch: ; 0x0203900C
	push {r3, lr}
	mov r0, #1
	bl WifiConnection_Update
	cmp r0, #0
	bge _02039022
	ldr r0, _02039074 ; =CommState_WifiFailed
	mov r1, #0
	bl CommState_SetState
	pop {r3, pc}
_02039022:
	ldr r0, _02039078 ; =gCommState
	ldr r0, [r0]
	add r0, #0x4e
	ldrb r0, [r0]
	bl sub_02033FC4
	add r2, r0, #0
	ldr r0, _02039078 ; =gCommState
	ldr r1, [r0]
	mov r0, #0x51
	ldrsb r0, [r1, r0]
	add r1, r2, #1
	mov r2, #0
	bl WifiConnection_StartMatchmaking
	add r0, r0, #3
	cmp r0, #3
	bhi _02039070
	add r0, r0, r0
	add r0, pc
	ldrh r0, [r0, #6]
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	add pc, r0
_02039052: ; jump table
	.short _02039068 - _02039052 - 2 ; case 0
	.short _02039070 - _02039052 - 2 ; case 1
	.short _02039070 - _02039052 - 2 ; case 2
	.short _0203905A - _02039052 - 2 ; case 3
_0203905A:
	bl sub_0203622C
	ldr r0, _0203907C ; =CommState_WifiLobby
	mov r1, #0
	bl CommState_SetState
	pop {r3, pc}
_02039068:
	ldr r0, _02039074 ; =CommState_WifiFailed
	mov r1, #0
	bl CommState_SetState
_02039070:
	pop {r3, pc}
	nop
_02039074: .word CommState_WifiFailed
_02039078: .word gCommState
_0203907C: .word CommState_WifiLobby
	thumb_func_end CommState_WifiStartMatch
