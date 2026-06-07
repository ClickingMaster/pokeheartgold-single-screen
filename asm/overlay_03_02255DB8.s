	.include "asm/macros.inc"
	.include "overlay_03.inc"
	.include "global.inc"

	.text

	thumb_func_start BattleRegulationMenu_ShowListMenuRegulations
BattleRegulationMenu_ShowListMenuRegulations: ; 0x02255DB8
	push {r4, r5, r6, r7, lr}
	sub sp, #0x34
	add r5, r0, #0
	ldr r0, [r5, #0x10]
	add r4, r5, #0
	ldr r0, [r0, #0xc]
	mov r1, #0
	add r4, #0x34
	bl Save_LinkBattleRuleset_GetByIndex
	mov r6, #5
	cmp r0, #0
	beq _02255DD4
	add r6, r6, #1
_02255DD4:
	add r0, r4, #0
	bl WindowIsInUse
	cmp r0, #0
	bne _02255E66
	add r0, r6, #2
	mov r1, #4
	bl ListMenuItems_New
	str r0, [r5, #8]
	mov r3, #1
	str r3, [sp]
	mov r0, #0x10
	str r0, [sp, #4]
	add r0, r6, #2
	lsl r0, r0, #0x19
	lsr r0, r0, #0x18
	str r0, [sp, #8]
	mov r0, #0xd
	str r0, [sp, #0xc]
	str r3, [sp, #0x10]
	ldr r0, [r5, #0x10]
	add r1, r4, #0
	ldr r0, [r0, #8]
	mov r2, #3
	bl AddWindowParameterized
	add r0, r5, #0
	ldr r2, _02255EB4 ; =0x000003D9
	add r0, #0x34
	mov r1, #1
	mov r3, #0xb
	bl DrawFrameAndWindow1
	ldr r0, [r5, #8]
	ldr r1, [r5, #0x68]
	mov r2, #0x8a
	mov r3, #0xc
	bl ListMenuItems_AppendFromMsgData
	mov r4, #0
	cmp r6, #0
	ble _02255E58
	mov r7, #0x80
_02255E2C:
	add r0, r5, #0
	add r1, r4, #0
	bl BattleRegulationMenu_GetRegulationName
	ldr r0, [r5, #0x68]
	ldr r2, [r5, #0x1c]
	add r1, r7, #0
	bl ReadMsgDataIntoString
	ldr r0, [r5, #0x64]
	ldr r1, [r5, #0x20]
	ldr r2, [r5, #0x1c]
	bl StringExpandPlaceholders
	ldr r0, [r5, #8]
	ldr r1, [r5, #0x20]
	add r2, r4, #0
	bl ListMenuItems_AddItem
	add r4, r4, #1
	cmp r4, r6
	blt _02255E2C
_02255E58:
	mov r2, #0x81
	add r3, r2, #0
	ldr r0, [r5, #8]
	ldr r1, [r5, #0x68]
	sub r3, #0x83
	bl ListMenuItems_AppendFromMsgData
_02255E66:
	ldr r4, _02255EB8 ; =ov03_0225943C
	add r3, sp, #0x14
	add r2, r3, #0
	ldmia r4!, {r0, r1}
	stmia r3!, {r0, r1}
	ldmia r4!, {r0, r1}
	stmia r3!, {r0, r1}
	ldmia r4!, {r0, r1}
	stmia r3!, {r0, r1}
	ldmia r4!, {r0, r1}
	stmia r3!, {r0, r1}
	add r0, r6, #2
	lsl r0, r0, #0x10
	lsr r1, r0, #0x10
	add r0, sp, #0x14
	strh r1, [r0, #0x10]
	strh r1, [r0, #0x12]
	ldr r0, [r5, #8]
	mov r1, #0
	str r0, [sp, #0x14]
	add r0, r5, #0
	add r0, #0x34
	str r0, [sp, #0x20]
	add r0, r2, #0
	add r2, r5, #0
	str r5, [sp, #0x30]
	add r2, #0x78
	ldrh r2, [r2]
	mov r3, #4
	bl ListMenuInit
	str r0, [r5]
	add r5, #0x34
	add r0, r5, #0
	bl CopyWindowToVram
	add sp, #0x34
	pop {r4, r5, r6, r7, pc}
	nop
_02255EB4: .word 0x000003D9
_02255EB8: .word ov03_0225943C
	thumb_func_end BattleRegulationMenu_ShowListMenuRegulations
	