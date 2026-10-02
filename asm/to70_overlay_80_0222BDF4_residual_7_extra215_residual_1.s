	.extern FrontierScript_ReadVar
	.extern ov80_0222BE24
	.extern ov80_0223B9D0
	.include "asm/macros.inc"
	.include "overlay_80_0222BDF4.inc"
	.include "global.inc"

.text
	thumb_func_start FrtCmd_034
FrtCmd_034: ; 0x0222C52C
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x10
	str r0, [sp]
	ldr r0, [r0]
	str r0, [sp, #8]
	ldr r0, [r0]
	bl sub_0209680C
	add r4, r0, #0
	ldr r0, [sp]
	bl FrontierScriptContext_ReadWord
	ldr r1, [sp]
	ldr r1, [r1, #0x1c]
	str r1, [sp, #4]
	add r1, r1, r0
	ldr r0, [sp]
	str r1, [r0, #0x1c]
_0222C550:
	ldr r0, [sp]
	bl FrontierScript_ReadVar
	add r1, sp, #0xc
	strh r0, [r1]
	ldrh r2, [r1]
	ldr r0, _0222C5E4 ; =0x0000FD13
	cmp r2, r0
	beq _0222C5D8
	ldr r0, [sp]
	ldr r3, [r0, #0x1c]
	add r2, r3, #1
	str r2, [r0, #0x1c]
	ldrb r0, [r3]
	strb r0, [r1, #2]
	ldrh r1, [r1]
	ldr r0, _0222C5E8 ; =0x0000EEEE
	cmp r1, r0
	bne _0222C596
	ldr r0, [sp, #8]
	ldr r0, [r0]
	bl Frontier_GetLaunchArgs
	ldr r0, [r0, #8]
	bl Save_PlayerData_GetProfile
	bl ov80_0222A7EC
	add r1, sp, #0xc
	strh r0, [r1]
	add r0, r4, #0
	add r1, sp, #0xc
	bl ov80_0223947C
	b _0222C550
_0222C596:
	add r0, r0, #1
	cmp r1, r0
	bne _0222C5CE
	bl sub_02037474
	cmp r0, #1
	bne _0222C550
	bl sub_02037454
	add r6, r0, #0
	mov r5, #0
	cmp r6, #0
	ble _0222C550
	add r7, sp, #0xc
_0222C5B2:
	add r0, r5, #0
	bl sub_02034818
	bl ov80_0222A7EC
	strh r0, [r7]
	add r0, r4, #0
	add r1, sp, #0xc
	bl ov80_0223947C
	add r5, r5, #1
	cmp r5, r6
	blt _0222C5B2
	b _0222C550
_0222C5CE:
	add r0, r4, #0
	add r1, sp, #0xc
	bl ov80_0223947C
	b _0222C550
_0222C5D8:
	ldr r1, [sp, #4]
	ldr r0, [sp]
	str r1, [r0, #0x1c]
	mov r0, #0
	add sp, #0x10
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
_0222C5E4: .word 0x0000FD13
_0222C5E8: .word 0x0000EEEE
	thumb_func_end FrtCmd_034
