	.include "asm/macros.inc"
	.include "overlay_03.inc"
	.include "global.inc"

	.text

	thumb_func_start BattleRegulationMenu_ShowListMenuConfirm
BattleRegulationMenu_ShowListMenuConfirm: ; 0x02255F98
	push {r3, r4, r5, r6, lr}
	sub sp, #0x34
	add r5, r0, #0
	mov r0, #3
	mov r1, #4
	ldr r4, _0225603C ; =ov03_02259838
	bl ListMenuItems_New
	str r0, [r5, #0xc]
	mov r0, #0xa
	str r0, [sp]
	mov r0, #9
	str r0, [sp, #4]
	mov r0, #6
	str r0, [sp, #8]
	mov r0, #0xd
	str r0, [sp, #0xc]
	ldr r0, _02256040 ; =0x00000201
	add r1, r5, #0
	str r0, [sp, #0x10]
	ldr r0, [r5, #0x10]
	add r1, #0x44
	ldr r0, [r0, #8]
	mov r2, #3
	mov r3, #0x16
	bl AddWindowParameterized
	add r0, r5, #0
	ldr r2, _02256044 ; =0x000003D9
	add r0, #0x44
	mov r1, #1
	mov r3, #0xb
	bl DrawFrameAndWindow1
	mov r6, #0
_02255FDE:
	ldr r0, [r5, #0xc]
	ldr r1, [r5, #0x68]
	ldr r2, [r4]
	ldr r3, [r4, #4]
	bl ListMenuItems_AppendFromMsgData
	add r6, r6, #1
	add r4, #8
	cmp r6, #3
	blt _02255FDE
	ldr r4, _02256048 ; =ov03_0225943C
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
	mov r1, #3
	add r0, sp, #0x14
	strh r1, [r0, #0x10]
	strh r1, [r0, #0x12]
	ldr r0, [r5, #0xc]
	mov r1, #0
	str r0, [sp, #0x14]
	add r0, r5, #0
	add r0, #0x44
	str r0, [sp, #0x20]
	add r0, r2, #0
	add r2, r5, #0
	str r5, [sp, #0x30]
	add r2, #0x7a
	ldrh r2, [r2]
	mov r3, #4
	bl ListMenuInit
	str r0, [r5, #4]
	add r5, #0x44
	add r0, r5, #0
	bl CopyWindowToVram
	add sp, #0x34
	pop {r3, r4, r5, r6, pc}
	nop
_0225603C: .word ov03_02259838
_02256040: .word 0x00000201
_02256044: .word 0x000003D9
_02256048: .word ov03_0225943C
	thumb_func_end BattleRegulationMenu_ShowListMenuConfirm
	