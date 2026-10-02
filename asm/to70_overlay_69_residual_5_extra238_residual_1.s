	.extern ov69_021E68D8
	.include "asm/macros.inc"
	.include "overlay_69.inc"
	.include "global.inc"

.text
	thumb_func_start ov69_021E6994
ov69_021E6994: ; 0x021E6994
	push {r4, r5, r6, r7, lr}
	sub sp, #0x2c
	add r5, r0, #0
	ldr r0, _021E6A40 ; =0x0000C010
	str r1, [sp]
	ldr r0, [r5, r0]
	str r3, [sp, #4]
	ldr r7, [sp, #0x44]
	ldr r6, [sp, #0x48]
	bl AddWindow
	ldr r2, [sp, #0x40]
	ldr r3, [r5]
	mov r0, #0
	mov r1, #0x1b
	bl NewMsgDataFromNarc
	str r0, [sp, #8]
	ldr r1, [r5]
	add r0, r6, #0
	bl ListMenuItems_New
	ldr r1, _021E6A44 ; =0x0000C068
	mov r4, #0
	str r0, [r5, r1]
	cmp r6, #0
	bls _021E69DE
_021E69CA:
	ldr r0, _021E6A44 ; =0x0000C068
	ldrb r2, [r7, r4]
	ldr r0, [r5, r0]
	ldr r1, [sp, #8]
	add r3, r4, #0
	bl ListMenuItems_AppendFromMsgData
	add r4, r4, #1
	cmp r4, r6
	blo _021E69CA
_021E69DE:
	ldr r0, [sp, #8]
	bl DestroyMsgData
	ldr r4, [sp, #4]
	add r3, sp, #0xc
	ldmia r4!, {r0, r1}
	add r2, r3, #0
	stmia r3!, {r0, r1}
	str r4, [sp, #4]
	ldmia r4!, {r0, r1}
	stmia r3!, {r0, r1}
	str r4, [sp, #4]
	ldmia r4!, {r0, r1}
	stmia r3!, {r0, r1}
	str r4, [sp, #4]
	ldmia r4!, {r0, r1}
	stmia r3!, {r0, r1}
	ldr r0, _021E6A44 ; =0x0000C068
	mov r1, #0
	ldr r0, [r5, r0]
	str r4, [sp, #4]
	str r0, [sp, #0xc]
	add r0, sp, #0xc
	strh r6, [r0, #0x10]
	ldr r0, [sp]
	str r0, [sp, #0x18]
	ldr r0, _021E6A48 ; =ov69_021E68D8
	str r0, [sp, #0x10]
	ldr r3, [r5]
	add r0, r2, #0
	lsl r3, r3, #0x18
	add r2, r1, #0
	lsr r3, r3, #0x18
	bl ListMenuInit
	ldr r1, _021E6A4C ; =0x0000C064
	ldr r2, _021E6A50 ; =0x000001D9
	str r0, [r5, r1]
	ldr r0, [sp, #0x18]
	mov r1, #1
	mov r3, #7
	bl DrawFrameAndWindow1
	ldr r0, [sp]
	bl CopyWindowToVram
	add sp, #0x2c
	pop {r4, r5, r6, r7, pc}
	nop
_021E6A40: .word 0x0000C010
_021E6A44: .word 0x0000C068
_021E6A48: .word ov69_021E68D8
_021E6A4C: .word 0x0000C064
_021E6A50: .word 0x000001D9
	thumb_func_end ov69_021E6994
