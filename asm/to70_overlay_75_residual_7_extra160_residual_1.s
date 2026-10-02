	.extern ov75_02246BCC
	.extern ov75_02249974
	.extern ov75_02249978
	.extern ov75_0224998C
	.extern ov75_0224999C
	.extern ov75_022499BC
	.extern ov75_022499DC
	.include "asm/macros.inc"
	.include "overlay_75.inc"
	.include "global.inc"

.text
	thumb_func_start ov75_022478E0
ov75_022478E0: ; 0x022478E0
	push {r4, r5, r6, r7, lr}
	sub sp, #0x34
	add r5, r0, #0
	add r0, #0x94
	ldr r0, [r0]
	cmp r0, #0
	beq _022478F4
	cmp r0, #1
	beq _022479C0
	b _02247A1C
_022478F4:
	ldr r0, [r5]
	ldr r0, [r0, #4]
	bl sub_02031744
	cmp r0, #1
	bne _0224790A
	ldr r3, _02247A5C ; =ov75_02249974
	ldr r4, _02247A60 ; =ov75_022499BC
	mov r7, #4
	ldr r2, _02247A64 ; =ov75_022499DC
	b _02247912
_0224790A:
	ldr r3, _02247A68 ; =ov75_02249978
	ldr r4, _02247A6C ; =ov75_0224998C
	ldr r2, _02247A70 ; =ov75_0224999C
	mov r7, #2
_02247912:
	add r6, sp, #0x14
	ldmia r2!, {r0, r1}
	stmia r6!, {r0, r1}
	ldmia r2!, {r0, r1}
	stmia r6!, {r0, r1}
	ldmia r2!, {r0, r1}
	stmia r6!, {r0, r1}
	ldmia r2!, {r0, r1}
	stmia r6!, {r0, r1}
	ldrb r0, [r3, #1]
	add r1, r5, #0
	add r1, #0x78
	str r0, [sp]
	ldrb r0, [r3, #2]
	mov r2, #0
	str r0, [sp, #4]
	ldrb r0, [r3, #3]
	str r0, [sp, #8]
	mov r0, #0xd
	str r0, [sp, #0xc]
	mov r0, #0x94
	str r0, [sp, #0x10]
	ldrb r3, [r3]
	ldr r0, [r5, #4]
	bl AddWindowParameterized
	add r0, r7, #0
	mov r1, #0x74
	bl ListMenuItems_New
	add r1, r5, #0
	add r1, #0xa0
	mov r6, #0
	str r0, [r1]
	cmp r7, #0
	ble _02247972
_0224795A:
	add r0, r5, #0
	add r0, #0xa0
	ldr r0, [r0]
	ldr r1, [r5, #0x34]
	ldr r2, [r4]
	ldr r3, [r4, #4]
	bl ListMenuItems_AppendFromMsgData
	add r6, r6, #1
	add r4, #8
	cmp r6, r7
	blt _0224795A
_02247972:
	add r0, r5, #0
	add r0, #0x78
	str r0, [sp, #0x20]
	add r0, r5, #0
	add r0, #0xa0
	ldr r0, [r0]
	mov r1, #0
	str r0, [sp, #0x14]
	add r0, sp, #0x14
	add r2, r1, #0
	mov r3, #0x74
	bl ListMenuInit
	add r1, r5, #0
	add r1, #0xa4
	str r0, [r1]
	add r0, r5, #0
	add r0, #0x78
	mov r1, #1
	mov r2, #0x1f
	mov r3, #0xb
	bl DrawFrameAndWindow1
	add r0, r5, #0
	add r0, #0x48
	mov r1, #1
	bl ClearFrameAndWindow2
	add r0, r5, #0
	add r0, #0x78
	bl CopyWindowToVram
	add r0, r5, #0
	add r0, #0x94
	ldr r0, [r0]
	add r5, #0x94
	add r0, r0, #1
	str r0, [r5]
	b _02247A56
_022479C0:
	add r0, r5, #0
	add r0, #0xa4
	ldr r0, [r0]
	bl ListMenu_ProcessInput
	add r4, r0, #0
	mov r0, #1
	mvn r0, r0
	cmp r4, r0
	beq _022479DC
	add r0, r0, #1
	cmp r4, r0
	beq _02247A56
	b _02247A02
_022479DC:
	ldr r0, _02247A74 ; =0x000005DC
	bl PlaySE
	ldr r0, [r5]
	mov r1, #6
	mov r2, #0
	bl ov75_02246BCC
	add r0, r5, #0
	mov r1, #0x1d
	add r0, #0x9c
	str r1, [r0]
	add r0, r5, #0
	add r0, #0x94
	ldr r0, [r0]
	add r5, #0x94
	add r0, r0, #1
	str r0, [r5]
	b _02247A56
_02247A02:
	ldr r0, _02247A74 ; =0x000005DC
	bl PlaySE
	add r0, r5, #0
	add r0, #0x9c
	str r4, [r0]
	add r0, r5, #0
	add r0, #0x94
	ldr r0, [r0]
	add r5, #0x94
	add r0, r0, #1
	str r0, [r5]
	b _02247A56
_02247A1C:
	add r0, r5, #0
	add r0, #0xa0
	ldr r0, [r0]
	bl ListMenuItems_Delete
	add r0, r5, #0
	add r0, #0xa4
	mov r1, #0
	ldr r0, [r0]
	add r2, r1, #0
	bl DestroyListMenu
	add r0, r5, #0
	add r0, #0x78
	mov r1, #1
	bl sub_0200E5D4
	add r0, r5, #0
	add r0, #0x78
	bl ClearWindowTilemapAndCopyToVram
	add r0, r5, #0
	add r0, #0x78
	bl RemoveWindow
	add r0, r5, #0
	add r0, #0x9c
	ldr r0, [r0]
	str r0, [r5, #8]
_02247A56:
	mov r0, #0
	add sp, #0x34
	pop {r4, r5, r6, r7, pc}
	.balign 4, 0
_02247A5C: .word ov75_02249974
_02247A60: .word ov75_022499BC
_02247A64: .word ov75_022499DC
_02247A68: .word ov75_02249978
_02247A6C: .word ov75_0224998C
_02247A70: .word ov75_0224999C
_02247A74: .word 0x000005DC
	thumb_func_end ov75_022478E0
