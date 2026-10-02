	.extern ov86_021E73CC
	.extern ov86_021E752C
	.extern ov86_021E757C
	.extern ov86_021E7630
	.extern ov86_021E77BC
	.extern ov86_021E7860
	.extern ov86_021E7B68
	.extern ov86_021E7C70
	.extern ov86_021E7CA0
	.extern ov86_021E7CF8
	.extern ov86_021E7D8C
	.extern ov86_021E7DA8
	.extern ov86_021E7DD4
	.include "asm/macros.inc"
	.include "overlay_86.inc"
	.include "global.inc"
	.extern ov86_021E7344

.text
	thumb_func_start ov86_021E7258
ov86_021E7258: ; 0x021E7258
	push {r4, lr}
	add r4, r0, #0
	cmp r1, #0
	beq _021E726A
	cmp r1, #1
	beq _021E72A2
	cmp r1, #2
	beq _021E72E4
	b _021E7338
_021E726A:
	mov r2, #0
	ldr r0, [r4, #0xc]
	mov r1, #2
	add r3, r2, #0
	bl ScheduleSetBgPosText
	ldr r0, [r4, #0xc]
	mov r1, #2
	mov r2, #3
	mov r3, #0
	bl ScheduleSetBgPosText
	add r0, r4, #0
	bl ov86_021E7C70
	add r0, r4, #0
	bl ov86_021E73CC
	add r0, r4, #0
	bl ov86_021E7D8C
	add r0, r4, #0
	bl ov86_021E7DA8
	add r0, r4, #0
	bl ov86_021E757C
	b _021E7338
_021E72A2:
	mov r2, #0
	ldr r0, [r4, #0xc]
	mov r1, #2
	add r3, r2, #0
	bl ScheduleSetBgPosText
	mov r2, #3
	add r3, r2, #0
	ldr r0, [r4, #0xc]
	mov r1, #2
	add r3, #0xfd
	bl ScheduleSetBgPosText
	add r0, r4, #0
	bl ov86_021E7C70
	add r0, r4, #0
	add r0, #0x50
	bl ScheduleWindowCopyToVram
	add r0, r4, #0
	bl ov86_021E73CC
	add r0, r4, #0
	bl ov86_021E7630
	add r0, r4, #0
	bl ov86_021E77BC
	add r0, r4, #0
	bl ov86_021E752C
	b _021E7338
_021E72E4:
	mov r1, #2
	add r3, r1, #0
	ldr r0, [r4, #0xc]
	mov r2, #0
	add r3, #0xfe
	bl ScheduleSetBgPosText
	mov r2, #3
	add r3, r2, #0
	ldr r0, [r4, #0xc]
	mov r1, #2
	add r3, #0xfd
	bl ScheduleSetBgPosText
	add r0, r4, #0
	add r0, #0x50
	bl ClearWindowTilemapAndScheduleTransfer
	add r0, r4, #0
	bl ov86_021E7DD4
	add r0, r4, #0
	bl ov86_021E7B68
	add r0, r4, #0
	bl ov86_021E7CF8
	add r0, r4, #0
	bl ov86_021E7344
	add r0, r4, #0
	bl ov86_021E7CA0
	add r0, r4, #0
	bl ov86_021E73CC
	add r0, r4, #0
	bl ov86_021E7860
	add r0, r4, #0
	bl ov86_021E752C
_021E7338:
	ldr r0, [r4, #0xc]
	mov r1, #2
	bl ScheduleBgTilemapBufferTransfer
	pop {r4, pc}
	.balign 4, 0
	thumb_func_end ov86_021E7258
