	.include "asm/macros.inc"
	.include "unk_02037C94.inc"
	.include "global.inc"
	.text
	thumb_func_start CommState_CheckDwcError
CommState_CheckDwcError: ; 0x020395B0
	push {r3, lr}
	sub sp, #8
	ldr r0, _02039608 ; =gCommState
	ldr r0, [r0]
	cmp r0, #0
	bne _020395C2
	add sp, #8
	mov r0, #0
	pop {r3, pc}
_020395C2:
	add r0, #0x4e
	ldrb r0, [r0]
	cmp r0, #0x18
	beq _020395CE
	cmp r0, #0x24
	bne _020395E4
_020395CE:
	add r0, sp, #4
	add r1, sp, #0
	bl ov00_021EC11C
	cmp r0, #0
	beq _02039600
	bl ov00_021EC210
	add sp, #8
	mov r0, #1
	pop {r3, pc}
_020395E4:
	cmp r0, #0x19
	bne _02039600
	add r0, sp, #4
	add r1, sp, #0
	bl ov00_021EC11C
	cmp r0, #0
	beq _02039600
	ldr r0, [sp]
	cmp r0, #7
	bne _02039600
	add sp, #8
	mov r0, #1
	pop {r3, pc}
_02039600:
	mov r0, #0
	add sp, #8
	pop {r3, pc}
	nop
_02039608: .word gCommState
	thumb_func_end CommState_CheckDwcError
	thumb_func_start CommState_WifiLogin
CommState_WifiLogin: ; 0x0203960C
	push {r3, lr}
	bl WifiConnection_UpdateLogin
	ldr r1, _02039680 ; =gCommState
	ldr r3, [r1]
	ldr r2, [r3, #0x44]
	sub r2, r2, #1
	str r2, [r3, #0x44]
	cmp r0, #0x1b
	bne _02039660
	ldr r1, [r1]
	add r0, r1, #0
	add r0, #0x4e
	ldrb r0, [r0]
	cmp r0, #0x21
	bne _02039656
	ldr r0, [r1, #0x60]
	bl ov45_0222E804
	cmp r0, #1
	bne _0203964C
	ldr r1, _02039680 ; =gCommState
	mov r2, #1
	ldr r0, [r1]
	add r0, #0x64
	strb r2, [r0]
	ldr r1, [r1]
	ldr r0, _02039684 ; =CommState_PlazaWaitLobby
	ldr r1, [r1, #0x44]
	bl CommState_SetState
	pop {r3, pc}
_0203964C:
	ldr r0, _02039688 ; =CommState_PlazaLobbyFailed
	mov r1, #0
	bl CommState_SetState
	pop {r3, pc}
_02039656:
	ldr r0, _0203968C ; =CommState_WifiJoinLobby
	mov r1, #0
	bl CommState_SetState
	pop {r3, pc}
_02039660:
	cmp r0, #0
	beq _0203966C
	ldr r0, _02039690 ; =CommState_WifiFailed
	mov r1, #0
	bl CommState_SetState
_0203966C:
	ldr r0, _02039680 ; =gCommState
	ldr r0, [r0]
	ldr r0, [r0, #0x44]
	cmp r0, #0
	bgt _0203967E
	ldr r0, _02039690 ; =CommState_WifiFailed
	mov r1, #0
	bl CommState_SetState
_0203967E:
	pop {r3, pc}
	.balign 4, 0
_02039680: .word gCommState
_02039684: .word CommState_PlazaWaitLobby
_02039688: .word CommState_PlazaLobbyFailed
_0203968C: .word CommState_WifiJoinLobby
_02039690: .word CommState_WifiFailed
	thumb_func_end CommState_WifiLogin
	thumb_func_start CommState_WifiClubStart
CommState_WifiClubStart: ; 0x02039694
	push {r3, lr}
	bl CommWireless_IsStartedUp
	cmp r0, #0
	beq _020396EE
	mov r2, #0xd5
	mov r0, #3
	mov r1, #0x30
	lsl r2, r2, #0xa
	bl Heap_CreateAtEnd
	mov r0, #1
	add r1, r0, #0
	lsl r2, r0, #9
	add r3, r0, #0
	bl sub_02035ED8
	cmp r0, #0
	beq _020396EE
	ldr r0, _020396F0 ; =gCommState
	ldr r0, [r0]
	add r0, #0x4e
	ldrb r0, [r0]
	bl sub_02033FC4
	add r3, r0, #0
	ldr r0, _020396F0 ; =gCommState
	mov r2, #0xb1
	ldr r0, [r0]
	mov r1, #0x30
	ldr r0, [r0, #0x28]
	lsl r2, r2, #0xa
	add r3, r3, #1
	bl WifiConnection_Init
	ldr r0, _020396F4 ; =CommState_ShowErrorAndHalt
	bl WifiConnection_SetErrorCallback
	bl sub_02035FD8
	mov r1, #0xe1
	ldr r0, _020396F8 ; =CommState_WifiLogin
	lsl r1, r1, #4
	bl CommState_SetState
_020396EE:
	pop {r3, pc}
	.balign 4, 0
_020396F0: .word gCommState
_020396F4: .word CommState_ShowErrorAndHalt
_020396F8: .word CommState_WifiLogin
	thumb_func_end CommState_WifiClubStart
	thumb_func_start CommState_StartWifiClub
CommState_StartWifiClub: ; 0x020396FC
	push {r3, r4, r5, lr}
	add r5, r0, #0
	add r4, r1, #0
	bl Save_PlayerData_GetProfile
	bl sub_02037474
	cmp r0, #0
	beq _02039712
	mov r0, #0
	pop {r3, r4, r5, pc}
_02039712:
	mov r0, #1
	bl sub_0201A728
	ldr r2, _02039760 ; =0x00007080
	mov r0, #3
	mov r1, #0xf
	bl Heap_CreateAtEnd
	add r0, r5, #0
	mov r1, #0x17
	bl CommState_Init
	mov r0, #0xf
	add r1, r4, #0
	bl Heap_Alloc
	ldr r1, _02039764 ; =gCommState
	ldr r2, [r1]
	str r0, [r2]
	ldr r0, [r1]
	mov r1, #0
	ldr r0, [r0]
	add r2, r4, #0
	bl MI_CpuFill8
	ldr r0, _02039764 ; =gCommState
	mov r1, #0
	ldr r2, [r0]
	add r2, #0x4f
	strb r1, [r2]
	ldr r0, [r0]
	str r5, [r0, #0x28]
	ldr r0, _02039768 ; =CommState_WifiClubStart
	bl CommState_SetState
	ldr r0, _02039764 ; =gCommState
	ldr r0, [r0]
	ldr r0, [r0]
	pop {r3, r4, r5, pc}
	.balign 4, 0
_02039760: .word 0x00007080
_02039764: .word gCommState
_02039768: .word CommState_WifiClubStart
	thumb_func_end CommState_StartWifiClub
	thumb_func_start CommState_StartWifiPlaza
CommState_StartWifiPlaza: ; 0x0203976C
	push {r3, r4, r5, lr}
	add r5, r0, #0
	add r4, r1, #0
	bl Save_PlayerData_GetProfile
	bl sub_02037474
	cmp r0, #0
	bne _020397B8
	mov r0, #1
	bl sub_0201A728
	ldr r2, _020397BC ; =0x00007080
	mov r0, #3
	mov r1, #0xf
	bl Heap_CreateAtEnd
	add r0, r5, #0
	mov r1, #0x21
	bl CommState_Init
	ldr r0, _020397C0 ; =gCommState
	mov r1, #0
	ldr r2, [r0]
	str r1, [r2]
	ldr r2, [r0]
	str r4, [r2, #0x60]
	ldr r2, [r0]
	add r2, #0x64
	strb r1, [r2]
	ldr r2, [r0]
	add r2, #0x4f
	strb r1, [r2]
	ldr r0, [r0]
	str r5, [r0, #0x28]
	ldr r0, _020397C4 ; =CommState_PlazaStart
	bl CommState_SetState
_020397B8:
	pop {r3, r4, r5, pc}
	nop
_020397BC: .word 0x00007080
_020397C0: .word gCommState
_020397C4: .word CommState_PlazaStart
	thumb_func_end CommState_StartWifiPlaza
