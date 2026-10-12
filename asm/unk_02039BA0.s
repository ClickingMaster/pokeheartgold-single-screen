	.include "asm/macros.inc"
	.include "unk_02037C94.inc"
	.include "global.inc"
	.text
	thumb_func_start CommState_PlazaStart
CommState_PlazaStart: ; 0x02039BA0
	push {r3, lr}
	bl CommWireless_IsStartedUp
	cmp r0, #0
	beq _02039C04
	mov r1, #0x30
	mov r0, #3
	lsl r2, r1, #0xd
	bl Heap_CreateAtEnd
	mov r0, #1
	add r1, r0, #0
	lsl r2, r0, #9
	add r3, r0, #0
	bl sub_02035ED8
	cmp r0, #0
	beq _02039C04
	ldr r0, _02039C08 ; =gCommState
	ldr r0, [r0]
	add r0, #0x4e
	ldrb r0, [r0]
	bl sub_02033FC4
	add r3, r0, #0
	ldr r0, _02039C08 ; =gCommState
	mov r2, #0x16
	ldr r0, [r0]
	mov r1, #0x30
	ldr r0, [r0, #0x28]
	lsl r2, r2, #0xe
	add r3, r3, #1
	bl WifiConnection_Init
	ldr r0, _02039C0C ; =CommState_ShowErrorAndHalt
	bl WifiConnection_SetErrorCallback
	bl sub_02035FD8
	mov r0, #0
	bl ov00_021E70B8
	mov r0, #0
	bl sub_020378E4
	mov r1, #0xe1
	ldr r0, _02039C10 ; =CommState_WifiLogin
	lsl r1, r1, #4
	bl CommState_SetState
_02039C04:
	pop {r3, pc}
	nop
_02039C08: .word gCommState
_02039C0C: .word CommState_ShowErrorAndHalt
_02039C10: .word CommState_WifiLogin
	thumb_func_end CommState_PlazaStart
	thumb_func_start CommState_PlazaWaitLobby
CommState_PlazaWaitLobby: ; 0x02039C14
	push {r3, lr}
	ldr r0, _02039C54 ; =gCommState
	ldr r2, [r0]
	ldr r1, [r2, #0x44]
	sub r1, r1, #1
	str r1, [r2, #0x44]
	ldr r0, [r0]
	ldr r0, [r0, #0x44]
	cmp r0, #0
	bgt _02039C32
	ldr r0, _02039C58 ; =CommState_WifiFailed
	mov r1, #0
	bl CommState_SetState
	pop {r3, pc}
_02039C32:
	mov r0, #1
	bl sub_0203786C
	bl CommState_PlazaUpdate
	cmp r0, #0
	beq _02039C50
	bl ov45_0222E8F4
	cmp r0, #0
	beq _02039C50
	ldr r0, _02039C5C ; =CommState_PlazaLobby
	mov r1, #0
	bl CommState_SetState
_02039C50:
	pop {r3, pc}
	nop
_02039C54: .word gCommState
_02039C58: .word CommState_WifiFailed
_02039C5C: .word CommState_PlazaLobby
	thumb_func_end CommState_PlazaWaitLobby
	thumb_func_start CommState_PlazaLobby
CommState_PlazaLobby: ; 0x02039C60
	push {r3, lr}
	bl CommState_PlazaUpdate
	cmp r0, #0
	pop {r3, pc}
	.balign 4, 0
	thumb_func_end CommState_PlazaLobby
	thumb_func_start CommState_PlazaCheckWifiResult
CommState_PlazaCheckWifiResult: ; 0x02039C6C
	push {r4, lr}
	mov r4, #1
	cmp r0, #0xa
	blt _02039C84
	cmp r0, #0x14
	bge _02039C84
	ldr r0, _02039CF4 ; =CommState_WifiCancelled
	mov r1, #0
	bl CommState_SetState
	mov r4, #0
	b _02039CC8
_02039C84:
	cmp r0, #0
	bge _02039C94
	ldr r0, _02039CF8 ; =CommState_WifiFailed
	mov r1, #0
	bl CommState_SetState
	mov r4, #0
	b _02039CC8
_02039C94:
	cmp r0, #0x17
	bne _02039CA4
	ldr r0, _02039CFC ; =CommState_PlazaTimedOut
	mov r1, #0
	bl CommState_SetState
	mov r4, #0
	b _02039CC8
_02039CA4:
	cmp r0, #0x18
	bne _02039CC8
	ldr r0, _02039D00 ; =gCommState
	mov r4, #0
	ldr r0, [r0]
	add r0, #0x53
	ldrb r0, [r0]
	cmp r0, #0
	beq _02039CC0
	ldr r0, _02039CF8 ; =CommState_WifiFailed
	add r1, r4, #0
	bl CommState_SetState
	b _02039CC8
_02039CC0:
	ldr r0, _02039D04 ; =CommState_WifiDisconnected
	add r1, r4, #0
	bl CommState_SetState
_02039CC8:
	ldr r0, _02039D00 ; =gCommState
	ldr r0, [r0]
	add r0, #0x53
	ldrb r0, [r0]
	cmp r0, #0
	beq _02039CEE
	bl Comm_CountConnected
	ldr r1, _02039D00 ; =gCommState
	ldr r1, [r1]
	add r1, #0x4b
	ldrb r1, [r1]
	cmp r1, r0
	beq _02039CEE
	ldr r0, _02039CF8 ; =CommState_WifiFailed
	mov r1, #0
	bl CommState_SetState
	mov r4, #0
_02039CEE:
	add r0, r4, #0
	pop {r4, pc}
	nop
_02039CF4: .word CommState_WifiCancelled
_02039CF8: .word CommState_WifiFailed
_02039CFC: .word CommState_PlazaTimedOut
_02039D00: .word gCommState
_02039D04: .word CommState_WifiDisconnected
	thumb_func_end CommState_PlazaCheckWifiResult
	thumb_func_start CommState_PlazaUpdateLobby
CommState_PlazaUpdateLobby: ; 0x02039D08
	push {r4, lr}
	mov r4, #1
	bl WiFiPlazaLobbyClient_Update
	cmp r0, #4
	bhi _02039D34
	add r0, r0, r0
	add r0, pc
	ldrh r0, [r0, #6]
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	add pc, r0
_02039D20: ; jump table
	.short _02039D34 - _02039D20 - 2 ; case 0
	.short _02039D34 - _02039D20 - 2 ; case 1
	.short _02039D34 - _02039D20 - 2 ; case 2
	.short _02039D34 - _02039D20 - 2 ; case 3
	.short _02039D2A - _02039D20 - 2 ; case 4
_02039D2A:
	ldr r0, _02039D38 ; =CommState_PlazaLobbyFailed
	mov r1, #0
	bl CommState_SetState
	mov r4, #0
_02039D34:
	add r0, r4, #0
	pop {r4, pc}
	.balign 4, 0
_02039D38: .word CommState_PlazaLobbyFailed
	thumb_func_end CommState_PlazaUpdateLobby
	thumb_func_start CommState_PlazaUpdate
CommState_PlazaUpdate: ; 0x02039D3C
	push {r3, lr}
	mov r0, #0
	bl WifiConnection_Update
	bl CommState_PlazaCheckWifiResult
	cmp r0, #0
	beq _02039D50
	bl CommState_PlazaUpdateLobby
_02039D50:
	pop {r3, pc}
	.balign 4, 0
	thumb_func_end CommState_PlazaUpdate
	thumb_func_start CommState_PlazaUpdateMatch
CommState_PlazaUpdateMatch: ; 0x02039D54
	push {r4, lr}
	add r4, r0, #0
	mov r0, #0
	bl WifiConnection_Update
	cmp r0, #0x14
	blt _02039D66
	str r0, [r4]
	b _02039D72
_02039D66:
	mov r1, #0
	str r1, [r4]
	bl CommState_PlazaCheckWifiResult
	cmp r0, #0
	beq _02039D76
_02039D72:
	bl CommState_PlazaUpdateLobby
_02039D76:
	pop {r4, pc}
	thumb_func_end CommState_PlazaUpdateMatch
