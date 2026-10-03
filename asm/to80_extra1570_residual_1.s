	.extern ov87_021E6AE0
	.extern ov87_021E6AF4
	.extern ov87_021E6B28
	.extern ov87_021E7008
	.extern ov87_021E7048
	.extern ov87_021E708C
	.extern ov87_021E70D0
	.extern ov87_021E7134
	.extern ov87_021E717C
	.extern ov87_021E71B4
	.extern ov87_021E71EC
	.extern ov87_021E7324
	.extern ov87_021E7334
	.extern ov87_021E734C
	.extern ov87_021E73E0
	.extern ov87_021E7460
	.extern ov87_021E7490
	.extern ov87_021E74B8
	.extern ov87_021E74D4
	.extern ov87_021E74F4
	.extern ov87_021E7550
	.extern ov87_021E75E0
	.extern ov87_021E7698
	.extern ov87_021E7734
	.extern ov87_021E788C
	.extern ov87_021E78D8
	.extern ov87_021E7990
	.extern ov87_021E7998
	.extern ov87_021E79A0
	.extern ov87_021E7A2C
	.extern ov87_021E7FD4
	.extern ov87_021E7FE0
	.extern ov87_021E7FEC
	.extern ov87_021E8014
	.extern ov87_021E803C
	.extern ov87_021E8058
	.extern ov87_021E806C
	.extern ov87_021E8078
	.extern ov87_021E8084
	.extern ov87_021E80B4
	.extern ov87_021E8134
	.extern ov87_021E8184
	.extern ov87_021E818C
	.extern ov87_021E81A0
	.extern ov87_021E81A2
	.extern ov87_021E81C0
	.extern ov87_021E82E4
	.extern ov87_021E8308
	.include "asm/macros.inc"
	.include "overlay_87.inc"
	.include "global.inc"

.text
	thumb_func_start ov87_021E6080
ov87_021E6080: ; 0x021E6080
	push {r3, r4, r5, r6, r7, lr}
	add r4, r0, #0
	ldrb r1, [r4, #8]
	cmp r1, #0xb
	bls _021E608C
	b _021E64F4
_021E608C:
	add r1, r1, r1
	add r1, pc
	ldrh r1, [r1, #6]
	lsl r1, r1, #0x10
	asr r1, r1, #0x10
	add pc, r1
_021E6098: ; jump table
	.short _021E60B0 - _021E6098 - 2 ; case 0
	.short _021E613E - _021E6098 - 2 ; case 1
	.short _021E6154 - _021E6098 - 2 ; case 2
	.short _021E6184 - _021E6098 - 2 ; case 3
	.short _021E633A - _021E6098 - 2 ; case 4
	.short _021E6356 - _021E6098 - 2 ; case 5
	.short _021E636A - _021E6098 - 2 ; case 6
	.short _021E63BA - _021E6098 - 2 ; case 7
	.short _021E63E4 - _021E6098 - 2 ; case 8
	.short _021E6414 - _021E6098 - 2 ; case 9
	.short _021E647E - _021E6098 - 2 ; case 10
	.short _021E64E8 - _021E6098 - 2 ; case 11
_021E60B0:
	ldrb r0, [r4, #0xe]
	cmp r0, #2
	bne _021E60E2
	mov r0, #0x4b
	lsl r0, r0, #2
	add r0, r4, r0
	bl ClearWindowTilemapAndCopyToVram
	add r0, r4, #0
	bl ov87_021E71B4
	mov r0, #0xcf
	lsl r0, r0, #2
	ldr r0, [r4, r0]
	mov r1, #0x58
	mov r2, #0xa0
	bl ov87_021E7FEC
	mov r0, #0xcf
	lsl r0, r0, #2
	ldr r0, [r4, r0]
	mov r1, #0
	bl ov87_021E803C
	b _021E60F2
_021E60E2:
	mov r0, #0x4f
	lsl r0, r0, #2
	add r0, r4, r0
	bl ClearWindowTilemapAndCopyToVram
	add r0, r4, #0
	bl ov87_021E717C
_021E60F2:
	mov r0, #0x1e
	strb r0, [r4, #0xf]
	mov r0, #0xd
	lsl r0, r0, #6
	ldr r0, [r4, r0]
	mov r1, #1
	bl ov87_021E7FD4
	add r0, r4, #0
	bl ov87_021E708C
	mov r5, #0
_021E610A:
	lsl r1, r5, #0x18
	add r0, r4, #0
	lsr r1, r1, #0x18
	bl ov87_021E71EC
	add r5, r5, #1
	cmp r5, #4
	blt _021E610A
	mov r0, #0xcf
	lsl r0, r0, #2
	ldr r0, [r4, r0]
	mov r1, #1
	bl ov87_021E7FD4
	mov r0, #0xcf
	lsl r0, r0, #2
	ldr r0, [r4, r0]
	mov r1, #0
	bl ov87_021E8084
	ldr r0, _021E63F4 ; =0x00000562
	bl PlaySE
	mov r0, #1
	strb r0, [r4, #8]
	b _021E64F4
_021E613E:
	mov r0, #4
	mov r1, #1
	bl GfGfx_EngineATogglePlanes
	mov r0, #1
	add r1, r0, #0
	bl GfGfx_EngineBTogglePlanes
	mov r0, #2
	strb r0, [r4, #8]
	b _021E64F4
_021E6154:
	ldrb r0, [r4, #0xf]
	cmp r0, #0
	bne _021E617E
	mov r0, #0xd
	lsl r0, r0, #6
	ldr r0, [r4, r0]
	mov r1, #0
	bl ov87_021E7FD4
	add r0, r4, #0
	add r0, #0xfc
	mov r1, #0
	bl FillWindowPixelBuffer
	add r0, r4, #0
	add r0, #0xfc
	bl ScheduleWindowCopyToVram
	mov r0, #3
	strb r0, [r4, #8]
	b _021E64F4
_021E617E:
	sub r0, r0, #1
	strb r0, [r4, #0xf]
	b _021E64F4
_021E6184:
	mov r0, #0xee
	lsl r0, r0, #2
	add r0, r4, r0
	mov r1, #4
	mov r2, #2
	bl sub_02021280
	ldr r0, _021E63F8 ; =ov87_021E8308
	bl TouchscreenHitbox_FindRectAtTouchHeld
	add r5, r0, #0
	mov r0, #0
	mvn r0, r0
	cmp r5, r0
	beq _021E6290
	add r0, r4, #0
	bl ov87_021E7490
	cmp r0, #3
	bhs _021E61B6
	mov r0, #0xe5
	mov r2, #1
	add r1, r4, r5
	lsl r0, r0, #2
	strb r2, [r1, r0]
_021E61B6:
	mov r0, #0xe5
	add r1, r4, r5
	lsl r0, r0, #2
	ldrb r0, [r1, r0]
	cmp r0, #1
	bne _021E61EA
	add r0, r4, #0
	bl ov87_021E7550
	lsl r1, r5, #0x18
	add r0, r4, #0
	lsr r1, r1, #0x18
	bl ov87_021E75E0
	add r0, r4, #0
	add r1, r5, #0
	bl ov87_021E74F4
	ldr r0, _021E63FC ; =0x0000055F
	bl IsSEPlaying
	cmp r0, #0
	bne _021E61EA
	ldr r0, _021E63FC ; =0x0000055F
	bl PlaySE
_021E61EA:
	add r0, r4, #0
	bl ov87_021E7734
	cmp r0, #1
	bne _021E6262
	ldr r0, _021E63FC ; =0x0000055F
	mov r1, #0
	bl StopSE
	mov r0, #0xd
	lsl r0, r0, #6
	ldr r0, [r4, r0]
	mov r1, #1
	bl ov87_021E7FD4
	ldr r1, _021E6400 ; =0x0000039F
	ldrb r0, [r4, r1]
	lsl r0, r0, #1
	add r2, r4, r0
	add r0, r1, #0
	sub r0, #0x35
	ldrh r3, [r2, r0]
	add r0, r1, #0
	sub r0, #0x27
	ldr r2, [r4, r0]
	ldrb r0, [r4, #0xe]
	lsl r0, r0, #1
	strh r3, [r2, r0]
	ldrb r0, [r4, r1]
	lsl r0, r0, #1
	add r2, r4, r0
	add r0, r1, #0
	sub r0, #0x35
	ldrh r0, [r2, r0]
	cmp r0, #0x5c
	bne _021E6236
	mov r2, #1
	b _021E6238
_021E6236:
	mov r2, #3
_021E6238:
	ldrb r0, [r4, #0xe]
	sub r1, #0x23
	ldr r1, [r4, r1]
	lsl r0, r0, #1
	strh r2, [r1, r0]
	add r0, r4, #0
	bl ov87_021E788C
	cmp r0, #1
	bne _021E6250
	mov r0, #0x1e
	b _021E6252
_021E6250:
	mov r0, #0
_021E6252:
	strb r0, [r4, #0xf]
	add r0, r4, #0
	bl ov87_021E70D0
	mov r0, #4
	strb r0, [r4, #8]
	mov r0, #0
	pop {r3, r4, r5, r6, r7, pc}
_021E6262:
	ldr r0, _021E6404 ; =0x000003A1
	ldrb r0, [r4, r0]
	cmp r0, #3
	blo _021E6298
	ldr r0, _021E63FC ; =0x0000055F
	mov r1, #0
	bl StopSE
	mov r0, #0x3c
	strb r0, [r4, #0xf]
	mov r0, #0xd
	lsl r0, r0, #6
	ldr r0, [r4, r0]
	mov r1, #1
	bl ov87_021E7FD4
	add r0, r4, #0
	bl ov87_021E7134
	mov r0, #7
	strb r0, [r4, #8]
	mov r0, #0
	pop {r3, r4, r5, r6, r7, pc}
_021E6290:
	ldr r0, _021E63FC ; =0x0000055F
	mov r1, #0
	bl StopSE
_021E6298:
	mov r0, #0x3a
	lsl r0, r0, #4
	ldrb r0, [r4, r0]
	lsl r0, r0, #0x1f
	lsr r0, r0, #0x1f
	cmp r0, #1
	bne _021E62AC
	add r0, r4, #0
	bl ov87_021E7698
_021E62AC:
	ldrb r0, [r4, #0xe]
	cmp r0, #2
	bne _021E62F6
	ldr r0, _021E6408 ; =ov87_021E8184
	bl TouchscreenHitbox_FindRectAtTouchNew
	mov r1, #0
	mvn r1, r1
	cmp r0, r1
	beq _021E6378
	mov r0, #0xcf
	lsl r0, r0, #2
	ldr r0, [r4, r0]
	mov r1, #1
	bl ov87_021E8084
	add r0, r4, #0
	bl ov87_021E7008
	ldr r0, _021E640C ; =0x000005E4
	bl PlaySE
	mov r0, #0xcf
	lsl r0, r0, #2
	ldr r0, [r4, r0]
	mov r1, #0
	bl ov87_021E803C
	add r0, r4, #0
	bl ov87_021E6AE0
	add r0, r4, #0
	bl ov87_021E6AF4
	mov r0, #0xa
	strb r0, [r4, #8]
	b _021E64F4
_021E62F6:
	ldr r0, _021E6410 ; =ov87_021E818C
	bl TouchscreenHitbox_FindRectAtTouchNew
	mov r1, #0
	mvn r1, r1
	cmp r0, r1
	beq _021E6378
	mov r0, #0xcf
	lsl r0, r0, #2
	ldr r0, [r4, r0]
	mov r1, #1
	bl ov87_021E8084
	add r0, r4, #0
	bl ov87_021E7008
	ldr r0, _021E640C ; =0x000005E4
	bl PlaySE
	mov r0, #0xcf
	lsl r0, r0, #2
	ldr r0, [r4, r0]
	mov r1, #1
	bl ov87_021E803C
	add r0, r4, #0
	bl ov87_021E6AE0
	add r0, r4, #0
	bl ov87_021E6AF4
	mov r0, #9
	strb r0, [r4, #8]
	b _021E64F4
_021E633A:
	ldrb r1, [r4, #0xf]
	cmp r1, #0
	bne _021E6350
	mov r1, #0
	bl ov87_021E78D8
	cmp r0, #0
	bne _021E6378
	mov r0, #5
	strb r0, [r4, #8]
	b _021E64F4
_021E6350:
	sub r0, r1, #1
	strb r0, [r4, #0xf]
	b _021E64F4
_021E6356:
	mov r1, #1
	bl ov87_021E78D8
	cmp r0, #0
	bne _021E6378
	mov r0, #0
	strb r0, [r4, #0xf]
	mov r0, #6
	strb r0, [r4, #8]
	b _021E64F4
_021E636A:
	mov r0, #0xd7
	lsl r0, r0, #2
	ldr r0, [r4, r0]
	bl ov87_021E80B4
	cmp r0, #1
	bne _021E637A
_021E6378:
	b _021E64F4
_021E637A:
	ldrb r0, [r4, #0xf]
	cmp r0, #0
	bne _021E63B4
	mov r7, #0x35
	mov r6, #0
	add r5, r4, #0
	lsl r7, r7, #4
_021E6388:
	ldr r0, [r5, r7]
	mov r1, #0
	bl ov87_021E7FD4
	add r6, r6, #1
	add r5, r5, #4
	cmp r6, #3
	blt _021E6388
	mov r0, #0xd7
	lsl r0, r0, #2
	ldr r0, [r4, r0]
	mov r1, #0
	bl ov87_021E7FD4
	add r0, r4, #0
	bl ov87_021E7998
	add r0, r4, #0
	bl ov87_021E79A0
	mov r0, #1
	pop {r3, r4, r5, r6, r7, pc}
_021E63B4:
	sub r0, r0, #1
	strb r0, [r4, #0xf]
	b _021E64F4
_021E63BA:
	ldrb r0, [r4, #0xf]
	cmp r0, #0
	bne _021E63DE
	mov r0, #1
	mov r1, #0
	bl GfGfx_EngineATogglePlanes
	add r0, r4, #0
	bl ov87_021E7998
	add r0, r4, #0
	bl ov87_021E79A0
	mov r0, #0x1e
	strb r0, [r4, #0xf]
	mov r0, #8
	strb r0, [r4, #8]
	b _021E64F4
_021E63DE:
	sub r0, r0, #1
	strb r0, [r4, #0xf]
	b _021E64F4
_021E63E4:
	ldrb r0, [r4, #0xf]
	cmp r0, #0
	bne _021E63EE
	mov r0, #1
	pop {r3, r4, r5, r6, r7, pc}
_021E63EE:
	sub r0, r0, #1
	strb r0, [r4, #0xf]
	b _021E64F4
	.balign 4, 0
_021E63F4: .word 0x00000562
_021E63F8: .word ov87_021E8308
_021E63FC: .word 0x0000055F
_021E6400: .word 0x0000039F
_021E6404: .word 0x000003A1
_021E6408: .word ov87_021E8184
_021E640C: .word 0x000005E4
_021E6410: .word ov87_021E818C
_021E6414:
	mov r0, #0x39
	lsl r0, r0, #4
	ldr r0, [r4, r0]
	bl YesNoPrompt_HandleInputForSave
	cmp r0, #1
	bne _021E6450
	mov r0, #0xcf
	lsl r0, r0, #2
	ldr r0, [r4, r0]
	mov r1, #0
	bl ov87_021E8084
	add r0, r4, #0
	add r0, #0x5c
	mov r1, #0
	bl sub_0200E5D4
	add r0, r4, #0
	add r0, #0x5c
	bl ClearWindowTilemapAndCopyToVram
	add r0, r4, #0
	bl ov87_021E7998
	add r0, r4, #0
	bl ov87_021E6B28
	mov r0, #1
	pop {r3, r4, r5, r6, r7, pc}
_021E6450:
	cmp r0, #2
	bne _021E64F4
	mov r0, #0xcf
	lsl r0, r0, #2
	ldr r0, [r4, r0]
	mov r1, #0
	bl ov87_021E8084
	add r0, r4, #0
	add r0, #0x5c
	mov r1, #0
	bl sub_0200E5D4
	add r0, r4, #0
	add r0, #0x5c
	bl ClearWindowTilemapAndCopyToVram
	add r0, r4, #0
	bl ov87_021E6B28
	mov r0, #0xb
	strb r0, [r4, #8]
	b _021E64F4
_021E647E:
	mov r0, #0x39
	lsl r0, r0, #4
	ldr r0, [r4, r0]
	bl YesNoPrompt_HandleInputForSave
	cmp r0, #1
	bne _021E64BA
	mov r0, #0xcf
	lsl r0, r0, #2
	ldr r0, [r4, r0]
	mov r1, #0
	bl ov87_021E8084
	add r0, r4, #0
	add r0, #0x5c
	mov r1, #0
	bl sub_0200E5D4
	add r0, r4, #0
	add r0, #0x5c
	bl ClearWindowTilemapAndCopyToVram
	add r0, r4, #0
	bl ov87_021E7990
	add r0, r4, #0
	bl ov87_021E6B28
	mov r0, #1
	pop {r3, r4, r5, r6, r7, pc}
_021E64BA:
	cmp r0, #2
	bne _021E64F4
	mov r0, #0xcf
	lsl r0, r0, #2
	ldr r0, [r4, r0]
	mov r1, #0
	bl ov87_021E8084
	add r0, r4, #0
	add r0, #0x5c
	mov r1, #0
	bl sub_0200E5D4
	add r0, r4, #0
	add r0, #0x5c
	bl ClearWindowTilemapAndCopyToVram
	add r0, r4, #0
	bl ov87_021E6B28
	mov r0, #0xb
	strb r0, [r4, #8]
	b _021E64F4
_021E64E8:
	bl System_GetTouchHeld
	cmp r0, #0
	bne _021E64F4
	mov r0, #3
	strb r0, [r4, #8]
_021E64F4:
	mov r0, #0
	pop {r3, r4, r5, r6, r7, pc}
	thumb_func_end ov87_021E6080
