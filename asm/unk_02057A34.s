	.include "asm/macros.inc"
	.include "unk_02056D7C.inc"
	.include "global.inc"
	.text
	thumb_func_start CommPlayerManager_GetPlayerX
CommPlayerManager_GetPlayerX: ; 0x02057A34
	push {r4, lr}
	add r4, r0, #0
	ldr r0, _02057A80 ; =gCommPlayerManager
	ldr r0, [r0]
	cmp r0, #0
	bne _02057A44
	ldr r0, _02057A84 ; =0x0000FFFF
	pop {r4, pc}
_02057A44:
	bl Comm_GetLocalNetId
	cmp r4, r0
	bne _02057A5A
	ldr r0, _02057A80 ; =gCommPlayerManager
	ldr r1, [r0]
	lsl r0, r4, #3
	add r0, r1, r0
	add r0, #0x74
	ldrh r0, [r0]
	pop {r4, pc}
_02057A5A:
	bl CommPlayerManager_IsLocalPlayerConnected
	cmp r0, #0
	beq _02057A70
	ldr r0, _02057A80 ; =gCommPlayerManager
	ldr r1, [r0]
	add r0, r1, r4
	add r0, #0x24
	ldrb r0, [r0]
	cmp r0, #0
	bne _02057A74
_02057A70:
	ldr r0, _02057A84 ; =0x0000FFFF
	pop {r4, pc}
_02057A74:
	lsl r0, r4, #3
	add r0, r1, r0
	add r0, #0x74
	ldrh r0, [r0]
	pop {r4, pc}
	nop
_02057A80: .word gCommPlayerManager
_02057A84: .word 0x0000FFFF
	thumb_func_end CommPlayerManager_GetPlayerX
	thumb_func_start CommPlayerManager_GetPlayerZ
CommPlayerManager_GetPlayerZ: ; 0x02057A88
	push {r4, lr}
	add r4, r0, #0
	ldr r0, _02057AD4 ; =gCommPlayerManager
	ldr r0, [r0]
	cmp r0, #0
	bne _02057A98
	ldr r0, _02057AD8 ; =0x0000FFFF
	pop {r4, pc}
_02057A98:
	bl Comm_GetLocalNetId
	cmp r4, r0
	bne _02057AAE
	ldr r0, _02057AD4 ; =gCommPlayerManager
	ldr r1, [r0]
	lsl r0, r4, #3
	add r0, r1, r0
	add r0, #0x76
	ldrh r0, [r0]
	pop {r4, pc}
_02057AAE:
	bl CommPlayerManager_IsLocalPlayerConnected
	cmp r0, #0
	beq _02057AC4
	ldr r0, _02057AD4 ; =gCommPlayerManager
	ldr r1, [r0]
	add r0, r1, r4
	add r0, #0x24
	ldrb r0, [r0]
	cmp r0, #0
	bne _02057AC8
_02057AC4:
	ldr r0, _02057AD8 ; =0x0000FFFF
	pop {r4, pc}
_02057AC8:
	lsl r0, r4, #3
	add r0, r1, r0
	add r0, #0x76
	ldrh r0, [r0]
	pop {r4, pc}
	nop
_02057AD4: .word gCommPlayerManager
_02057AD8: .word 0x0000FFFF
	thumb_func_end CommPlayerManager_GetPlayerZ
	thumb_func_start CommPlayerManager_GetPlayerFrontX
CommPlayerManager_GetPlayerFrontX: ; 0x02057ADC
	push {r4, lr}
	add r4, r0, #0
	bl CommPlayerManager_GetPlayerX
	ldr r1, _02057B0C ; =0x0000FFFF
	cmp r0, r1
	bne _02057AEE
	add r0, r1, #0
	pop {r4, pc}
_02057AEE:
	ldr r0, _02057B10 ; =gCommPlayerManager
	lsl r4, r4, #3
	ldr r0, [r0]
	add r1, r0, r4
	mov r0, #0x78
	ldrsb r0, [r1, r0]
	bl GetDeltaXByFacingDirection
	ldr r1, _02057B10 ; =gCommPlayerManager
	ldr r1, [r1]
	add r1, r1, r4
	add r1, #0x74
	ldrh r1, [r1]
	add r0, r1, r0
	pop {r4, pc}
	.balign 4, 0
_02057B0C: .word 0x0000FFFF
_02057B10: .word gCommPlayerManager
	thumb_func_end CommPlayerManager_GetPlayerFrontX
	thumb_func_start CommPlayerManager_GetPlayerFrontZ
CommPlayerManager_GetPlayerFrontZ: ; 0x02057B14
	push {r4, lr}
	add r4, r0, #0
	bl CommPlayerManager_GetPlayerZ
	ldr r1, _02057B44 ; =0x0000FFFF
	cmp r0, r1
	bne _02057B26
	add r0, r1, #0
	pop {r4, pc}
_02057B26:
	ldr r0, _02057B48 ; =gCommPlayerManager
	lsl r4, r4, #3
	ldr r0, [r0]
	add r1, r0, r4
	mov r0, #0x78
	ldrsb r0, [r1, r0]
	bl GetDeltaYByFacingDirection
	ldr r1, _02057B48 ; =gCommPlayerManager
	ldr r1, [r1]
	add r1, r1, r4
	add r1, #0x76
	ldrh r1, [r1]
	add r0, r1, r0
	pop {r4, pc}
	.balign 4, 0
_02057B44: .word 0x0000FFFF
_02057B48: .word gCommPlayerManager
	thumb_func_end CommPlayerManager_GetPlayerFrontZ
