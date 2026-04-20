	.include "asm/macros.inc"
	.include "overlay_44.inc"
	.include "global.inc"

	.text

	thumb_func_start ov44_02229EE0
ov44_02229EE0: ; 0x02229EE0
	push {r3, lr}
	mov r1, #0x16
	lsl r1, r1, #4
	ldr r0, [r0, r1]
	bl Save_VarsFlags_Get
	mov r1, #2
	mov r2, #0x1b
	bl Save_VarsFlags_FlypointFlagAction
	pop {r3, pc}
	.balign 4, 0
	thumb_func_end ov44_02229EE0

	thumb_func_start ov44_02229EF8
ov44_02229EF8: ; 0x02229EF8
	mov r0, #0
	bx lr
	thumb_func_end ov44_02229EF8

	thumb_func_start ov44_02229EFC
ov44_02229EFC: ; 0x02229EFC
	mov r0, #1
	bx lr
	thumb_func_end ov44_02229EFC

	thumb_func_start ov44_02229F00
ov44_02229F00: ; 0x02229F00
	push {r3, r4, r5, lr}
	ldrb r4, [r1, #0x1b]
	add r5, r0, #0
	add r1, r4, #0
	sub r1, #0x14
	cmp r1, #1
	bhi _02229F18
	bl ov44_02229EE0
	cmp r0, #0
	bne _02229F18
	mov r4, #0x1d
_02229F18:
	add r0, r4, #0
	sub r0, #0x16
	cmp r0, #5
	bhi _02229F2C
	add r0, r5, #0
	bl ov44_02229EFC
	cmp r0, #0
	bne _02229F2C
	mov r4, #0x1d
_02229F2C:
	add r0, r4, #0
	sub r0, #0x12
	cmp r0, #1
	bhi _02229F40
	add r0, r5, #0
	bl ov44_02229EF8
	cmp r0, #0
	bne _02229F40
	mov r4, #0x1d
_02229F40:
	add r0, r4, #0
	pop {r3, r4, r5, pc}
	thumb_func_end ov44_02229F00

	thumb_func_start ov44_02229F44
ov44_02229F44: ; 0x02229F44
	sub r0, #0x12
	cmp r0, #9
	bhi _02229F6E
	add r0, r0, r0
	add r0, pc
	ldrh r0, [r0, #6]
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	add pc, r0
_02229F56: ; jump table
	.short _02229F6A - _02229F56 - 2 ; case 0
	.short _02229F6A - _02229F56 - 2 ; case 1
	.short _02229F6E - _02229F56 - 2 ; case 2
	.short _02229F6E - _02229F56 - 2 ; case 3
	.short _02229F6A - _02229F56 - 2 ; case 4
	.short _02229F6A - _02229F56 - 2 ; case 5
	.short _02229F6A - _02229F56 - 2 ; case 6
	.short _02229F6A - _02229F56 - 2 ; case 7
	.short _02229F6A - _02229F56 - 2 ; case 8
	.short _02229F6A - _02229F56 - 2 ; case 9
_02229F6A:
	mov r0, #1
	bx lr
_02229F6E:
	mov r0, #0
	bx lr
	.balign 4, 0
	thumb_func_end ov44_02229F44

	thumb_func_start ov44_02229F74
ov44_02229F74: ; 0x02229F74
	push {r3, lr}
	sub r0, #0x12
	cmp r0, #9
	bhi _02229FAC
	add r0, r0, r0
	add r0, pc
	ldrh r0, [r0, #6]
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	add pc, r0
_02229F88: ; jump table
	.short _02229F9C - _02229F88 - 2 ; case 0
	.short _02229F9C - _02229F88 - 2 ; case 1
	.short _02229FAC - _02229F88 - 2 ; case 2
	.short _02229FAC - _02229F88 - 2 ; case 3
	.short _02229FA0 - _02229F88 - 2 ; case 4
	.short _02229FA0 - _02229F88 - 2 ; case 5
	.short _02229FA8 - _02229F88 - 2 ; case 6
	.short _02229FA8 - _02229F88 - 2 ; case 7
	.short _02229FA4 - _02229F88 - 2 ; case 8
	.short _02229FA4 - _02229F88 - 2 ; case 9
_02229F9C:
	mov r0, #0xb
	pop {r3, pc}
_02229FA0:
	mov r0, #0xd
	pop {r3, pc}
_02229FA4:
	mov r0, #0xf
	pop {r3, pc}
_02229FA8:
	mov r0, #0xe
	pop {r3, pc}
_02229FAC:
	bl GF_AssertFail
	mov r0, #0
	pop {r3, pc}
	thumb_func_end ov44_02229F74

	thumb_func_start ov44_02229FB4
ov44_02229FB4: ; 0x02229FB4
	sub r0, #9
	cmp r0, #5
	bhi _02229FD6
	add r0, r0, r0
	add r0, pc
	ldrh r0, [r0, #6]
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	add pc, r0
_02229FC6: ; jump table
	.short _02229FD2 - _02229FC6 - 2 ; case 0
	.short _02229FD2 - _02229FC6 - 2 ; case 1
	.short _02229FD2 - _02229FC6 - 2 ; case 2
	.short _02229FD2 - _02229FC6 - 2 ; case 3
	.short _02229FD2 - _02229FC6 - 2 ; case 4
	.short _02229FD2 - _02229FC6 - 2 ; case 5
_02229FD2:
	mov r0, #1
	bx lr
_02229FD6:
	mov r0, #0
	bx lr
	.balign 4, 0
	thumb_func_end ov44_02229FB4

	thumb_func_start ov44_02229FDC
ov44_02229FDC: ; 0x02229FDC
	push {r4, lr}
	add r4, r0, #0
	bl ov44_02229FB4
	cmp r0, #0
	beq _02229FEC
	mov r0, #1
	pop {r4, pc}
_02229FEC:
	sub r4, #0xf
	cmp r4, #0xc
	bhi _0222A01C
	add r0, r4, r4
	add r0, pc
	ldrh r0, [r0, #6]
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	add pc, r0
_02229FFE: ; jump table
	.short _0222A018 - _02229FFE - 2 ; case 0
	.short _0222A01C - _02229FFE - 2 ; case 1
	.short _0222A01C - _02229FFE - 2 ; case 2
	.short _0222A01C - _02229FFE - 2 ; case 3
	.short _0222A018 - _02229FFE - 2 ; case 4
	.short _0222A01C - _02229FFE - 2 ; case 5
	.short _0222A018 - _02229FFE - 2 ; case 6
	.short _0222A01C - _02229FFE - 2 ; case 7
	.short _0222A018 - _02229FFE - 2 ; case 8
	.short _0222A01C - _02229FFE - 2 ; case 9
	.short _0222A018 - _02229FFE - 2 ; case 10
	.short _0222A01C - _02229FFE - 2 ; case 11
	.short _0222A018 - _02229FFE - 2 ; case 12
_0222A018:
	mov r0, #1
	pop {r4, pc}
_0222A01C:
	mov r0, #0
	pop {r4, pc}
	thumb_func_end ov44_02229FDC

	thumb_func_start ov44_0222A020
ov44_0222A020: ; 0x0222A020
	cmp r0, #7
	bhi _0222A044
	add r0, r0, r0
	add r0, pc
	ldrh r0, [r0, #6]
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	add pc, r0
_0222A030: ; jump table
	.short _0222A044 - _0222A030 - 2 ; case 0
	.short _0222A044 - _0222A030 - 2 ; case 1
	.short _0222A040 - _0222A030 - 2 ; case 2
	.short _0222A040 - _0222A030 - 2 ; case 3
	.short _0222A040 - _0222A030 - 2 ; case 4
	.short _0222A040 - _0222A030 - 2 ; case 5
	.short _0222A040 - _0222A030 - 2 ; case 6
	.short _0222A040 - _0222A030 - 2 ; case 7
_0222A040:
	mov r0, #1
	bx lr
_0222A044:
	mov r0, #0
	bx lr
	thumb_func_end ov44_0222A020

	thumb_func_start ov44_0222A048
ov44_0222A048: ; 0x0222A048
	push {r4, lr}
	add r4, r0, #0
	bl ov44_0222A020
	cmp r0, #0
	beq _0222A058
	mov r0, #1
	pop {r4, pc}
_0222A058:
	cmp r4, #8
	bgt _0222A064
	bge _0222A088
	cmp r4, #1
	beq _0222A088
	b _0222A08C
_0222A064:
	sub r4, #0x12
	cmp r4, #8
	bhi _0222A08C
	add r0, r4, r4
	add r0, pc
	ldrh r0, [r0, #6]
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	add pc, r0
_0222A076: ; jump table
	.short _0222A088 - _0222A076 - 2 ; case 0
	.short _0222A08C - _0222A076 - 2 ; case 1
	.short _0222A088 - _0222A076 - 2 ; case 2
	.short _0222A08C - _0222A076 - 2 ; case 3
	.short _0222A088 - _0222A076 - 2 ; case 4
	.short _0222A08C - _0222A076 - 2 ; case 5
	.short _0222A088 - _0222A076 - 2 ; case 6
	.short _0222A08C - _0222A076 - 2 ; case 7
	.short _0222A088 - _0222A076 - 2 ; case 8
_0222A088:
	mov r0, #1
	pop {r4, pc}
_0222A08C:
	mov r0, #0
	pop {r4, pc}
	thumb_func_end ov44_0222A048

	thumb_func_start ov44_0222A090
ov44_0222A090: ; 0x0222A090
	sub r0, #0xc
	cmp r0, #3
	bhi _0222A0AE
	add r0, r0, r0
	add r0, pc
	ldrh r0, [r0, #6]
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	add pc, r0
_0222A0A2: ; jump table
	.short _0222A0AA - _0222A0A2 - 2 ; case 0
	.short _0222A0AA - _0222A0A2 - 2 ; case 1
	.short _0222A0AA - _0222A0A2 - 2 ; case 2
	.short _0222A0AA - _0222A0A2 - 2 ; case 3
_0222A0AA:
	mov r0, #1
	bx lr
_0222A0AE:
	mov r0, #0
	bx lr
	.balign 4, 0
	thumb_func_end ov44_0222A090

	thumb_func_start ov44_0222A0B4
ov44_0222A0B4: ; 0x0222A0B4
	mov r1, #0x1d
	cmp r0, #0x1b
	bhi _0222A164
	add r0, r0, r0
	add r0, pc
	ldrh r0, [r0, #6]
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	add pc, r0
_0222A0C6: ; jump table
	.short _0222A164 - _0222A0C6 - 2 ; case 0
	.short _0222A162 - _0222A0C6 - 2 ; case 1
	.short _0222A14A - _0222A0C6 - 2 ; case 2
	.short _0222A14E - _0222A0C6 - 2 ; case 3
	.short _0222A152 - _0222A0C6 - 2 ; case 4
	.short _0222A156 - _0222A0C6 - 2 ; case 5
	.short _0222A15A - _0222A0C6 - 2 ; case 6
	.short _0222A15E - _0222A0C6 - 2 ; case 7
	.short _0222A146 - _0222A0C6 - 2 ; case 8
	.short _0222A116 - _0222A0C6 - 2 ; case 9
	.short _0222A11A - _0222A0C6 - 2 ; case 10
	.short _0222A11E - _0222A0C6 - 2 ; case 11
	.short _0222A122 - _0222A0C6 - 2 ; case 12
	.short _0222A126 - _0222A0C6 - 2 ; case 13
	.short _0222A12A - _0222A0C6 - 2 ; case 14
	.short _0222A112 - _0222A0C6 - 2 ; case 15
	.short _0222A12E - _0222A0C6 - 2 ; case 16
	.short _0222A164 - _0222A0C6 - 2 ; case 17
	.short _0222A132 - _0222A0C6 - 2 ; case 18
	.short _0222A0FE - _0222A0C6 - 2 ; case 19
	.short _0222A136 - _0222A0C6 - 2 ; case 20
	.short _0222A102 - _0222A0C6 - 2 ; case 21
	.short _0222A13A - _0222A0C6 - 2 ; case 22
	.short _0222A106 - _0222A0C6 - 2 ; case 23
	.short _0222A142 - _0222A0C6 - 2 ; case 24
	.short _0222A10E - _0222A0C6 - 2 ; case 25
	.short _0222A13E - _0222A0C6 - 2 ; case 26
	.short _0222A10A - _0222A0C6 - 2 ; case 27
_0222A0FE:
	mov r1, #0x12
	b _0222A164
_0222A102:
	mov r1, #0x14
	b _0222A164
_0222A106:
	mov r1, #0x16
	b _0222A164
_0222A10A:
	mov r1, #0x1a
	b _0222A164
_0222A10E:
	mov r1, #0x18
	b _0222A164
_0222A112:
	mov r1, #8
	b _0222A164
_0222A116:
	mov r1, #2
	b _0222A164
_0222A11A:
	mov r1, #3
	b _0222A164
_0222A11E:
	mov r1, #4
	b _0222A164
_0222A122:
	mov r1, #5
	b _0222A164
_0222A126:
	mov r1, #6
	b _0222A164
_0222A12A:
	mov r1, #7
	b _0222A164
_0222A12E:
	mov r1, #1
	b _0222A164
_0222A132:
	mov r1, #0x13
	b _0222A164
_0222A136:
	mov r1, #0x15
	b _0222A164
_0222A13A:
	mov r1, #0x17
	b _0222A164
_0222A13E:
	mov r1, #0x1b
	b _0222A164
_0222A142:
	mov r1, #0x19
	b _0222A164
_0222A146:
	mov r1, #0xf
	b _0222A164
_0222A14A:
	mov r1, #9
	b _0222A164
_0222A14E:
	mov r1, #0xa
	b _0222A164
_0222A152:
	mov r1, #0xb
	b _0222A164
_0222A156:
	mov r1, #0xc
	b _0222A164
_0222A15A:
	mov r1, #0xd
	b _0222A164
_0222A15E:
	mov r1, #0xe
	b _0222A164
_0222A162:
	mov r1, #0x10
_0222A164:
	add r0, r1, #0
	bx lr
	thumb_func_end ov44_0222A0B4

	thumb_func_start ov44_0222A168
ov44_0222A168: ; 0x0222A168
	push {r3, r4, r5, r6, r7, lr}
	mov r1, #0x16
	lsl r1, r1, #4
	ldr r0, [r0, r1]
	bl SaveArray_Party_Get
	str r0, [sp]
	bl Party_GetCount
	mov r5, #0
	add r7, r0, #0
	add r4, r5, #0
	cmp r7, #0
	ble _0222A1B0
_0222A184:
	ldr r0, [sp]
	add r1, r4, #0
	bl Party_GetMonByIndex
	mov r1, #0xa3
	mov r2, #0
	add r6, r0, #0
	bl GetMonData
	cmp r0, #0
	beq _0222A1AA
	add r0, r6, #0
	mov r1, #0x4c
	mov r2, #0
	bl GetMonData
	cmp r0, #0
	bne _0222A1AA
	add r5, r5, #1
_0222A1AA:
	add r4, r4, #1
	cmp r4, r7
	blt _0222A184
_0222A1B0:
	add r0, r5, #0
	pop {r3, r4, r5, r6, r7, pc}
	thumb_func_end ov44_0222A168

	thumb_func_start ov44_0222A1B4
ov44_0222A1B4: ; 0x0222A1B4
	push {r4, lr}
	add r4, r0, #0
	add r1, r4, #0
	sub r1, #0x12
	cmp r1, #1
	bhi _0222A1C6
	bl sub_02039AF8
	pop {r4, pc}
_0222A1C6:
	bl ov44_02229F44
	cmp r0, #0
	beq _0222A1D4
	bl sub_02039B18
	pop {r4, pc}
_0222A1D4:
	cmp r4, #0x10
	bne _0222A1DE
	bl sub_02039B58
	pop {r4, pc}
_0222A1DE:
	sub r4, #0x14
	cmp r4, #1
	bhi _0222A1F0
	mov r0, #0
	bl sub_020378E4
	bl sub_02039E9C
	pop {r4, pc}
_0222A1F0:
	mov r0, #1
	bl sub_020378E4
	bl sub_02039B7C
	pop {r4, pc}
	thumb_func_end ov44_0222A1B4

	thumb_func_start ov44_0222A1FC
ov44_0222A1FC: ; 0x0222A1FC
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x30
	str r0, [sp, #4]
	mov r0, #0
	add r1, r0, #0
	bl Main_SetVBlankIntrCB
	bl HBlankInterruptDisable
	bl GfGfx_DisableEngineAPlanes
	bl GfGfx_DisableEngineBPlanes
	mov r2, #1
	lsl r2, r2, #0x1a
	ldr r1, [r2]
	ldr r0, _0222A3E8 ; =0xFFFFE0FF
	and r1, r0
	str r1, [r2]
	ldr r2, _0222A3EC ; =0x04001000
	ldr r1, [r2]
	and r0, r1
	str r0, [r2]
	mov r0, #4
	mov r1, #8
	bl SetKeyRepeatTimers
	mov r0, #0x58
	mov r1, #0x35
	bl NARC_New
	str r0, [sp, #0x20]
	mov r0, #0x35
	bl BgConfig_Alloc
	mov r2, #0x57
	ldr r1, [sp, #4]
	lsl r2, r2, #2
	str r0, [r1, r2]
	mov r0, #0x35
	bl MessageFormat_New
	mov r2, #0x59
	ldr r1, [sp, #4]
	lsl r2, r2, #2
	str r0, [r1, r2]
	ldr r2, _0222A3F0 ; =0x0000030A
	mov r0, #0
	mov r1, #0x1b
	mov r3, #0x35
	bl NewMsgDataFromNarc
	mov r2, #0x5a
	ldr r1, [sp, #4]
	lsl r2, r2, #2
	str r0, [r1, r2]
	mov r2, #0x32
	mov r0, #0
	mov r1, #0x1b
	lsl r2, r2, #4
	mov r3, #0x35
	bl NewMsgDataFromNarc
	mov r2, #0x5b
	ldr r1, [sp, #4]
	lsl r2, r2, #2
	str r0, [r1, r2]
	bl ov44_0222A830
	mov r1, #0x57
	ldr r0, [sp, #4]
	lsl r1, r1, #2
	ldr r0, [r0, r1]
	bl ov44_0222A850
	ldr r0, [sp, #4]
	ldr r1, [sp, #0x20]
	bl ov44_0222AEC0
	mov r2, #0x33
	ldr r1, _0222A3F4 ; =0x00000D68
	ldr r0, [sp, #4]
	lsl r2, r2, #4
	add r0, r0, r1
	mov r1, #0
	bl MI_CpuFill8
	ldr r0, [sp, #0x20]
	mov r1, #5
	add r2, sp, #0x2c
	mov r3, #0x35
	bl GfGfxLoader_GetPlttDataFromOpenNarc
	add r4, r0, #0
	ldr r0, [sp, #0x2c]
	mov r2, #0xd7
	ldr r1, [sp, #4]
	lsl r2, r2, #4
	add r1, r1, r2
	ldr r0, [r0, #0xc]
	mov r2, #0x80
	bl MIi_CpuCopy16
	ldr r0, [sp, #0x2c]
	mov r2, #0xdf
	ldr r1, [sp, #4]
	lsl r2, r2, #4
	add r1, r1, r2
	ldr r0, [r0, #0xc]
	mov r2, #0x80
	bl MIi_CpuCopy16
	add r0, r4, #0
	bl Heap_Free
	mov r0, #0
	mov r1, #0xdf
	str r0, [sp, #0x18]
	str r0, [sp, #0x1c]
	ldr r0, [sp, #4]
	lsl r1, r1, #4
	add r0, r0, r1
	str r0, [sp, #0xc]
	ldr r0, [sp, #4]
	sub r1, #0x80
	add r0, r0, r1
	str r0, [sp, #8]
_0222A2FA:
	mov r0, #0
	str r0, [sp, #0x14]
	str r0, [sp, #0x10]
	ldr r0, [sp, #0x1c]
	add r0, r0, #1
	lsl r1, r0, #5
	ldr r0, [sp, #4]
	add r0, r0, r1
	str r0, [sp, #0x24]
_0222A30C:
	ldr r0, [sp, #0x18]
	cmp r0, #0x15
	blt _0222A316
	bl GF_AssertFail
_0222A316:
	ldr r0, [sp, #0x24]
	mov r7, #1
	add r4, r0, #2
	ldr r0, [sp, #0xc]
	add r6, r0, #2
	ldr r0, [sp, #8]
	add r5, r0, #2
	ldr r0, [sp, #0x14]
	asr r0, r0, #8
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	str r0, [sp, #0x28]
_0222A32E:
	mov r0, #0xd7
	lsl r0, r0, #4
	ldrh r0, [r4, r0]
	ldr r3, [sp, #0x28]
	add r1, r6, #0
	str r0, [sp]
	add r0, r5, #0
	mov r2, #1
	bl BlendPalette
	add r7, r7, #1
	add r4, r4, #2
	add r6, r6, #2
	add r5, r5, #2
	cmp r7, #0x10
	blt _0222A32E
	ldr r0, [sp, #0xc]
	add r0, #0x20
	str r0, [sp, #0xc]
	ldr r0, [sp, #0x18]
	add r0, r0, #1
	str r0, [sp, #0x18]
	ldr r0, [sp, #0x10]
	cmp r0, #1
	beq _0222A37A
	mov r0, #3
	ldr r1, [sp, #0x14]
	lsl r0, r0, #8
	add r1, r1, r0
	mov r0, #1
	lsl r0, r0, #0xc
	str r1, [sp, #0x14]
	cmp r1, r0
	blt _0222A30C
	str r0, [sp, #0x14]
	mov r0, #1
	str r0, [sp, #0x10]
	b _0222A30C
_0222A37A:
	ldr r0, [sp, #8]
	add r0, #0x20
	str r0, [sp, #8]
	ldr r0, [sp, #0x1c]
	add r0, r0, #1
	str r0, [sp, #0x1c]
	cmp r0, #3
	blt _0222A2FA
	mov r1, #0xdf
	ldr r0, [sp, #4]
	lsl r1, r1, #4
	add r0, r0, r1
	mov r1, #0x2a
	lsl r1, r1, #4
	bl DC_FlushRange
	ldr r1, _0222A3F8 ; =0x00000D6C
	ldr r0, [sp, #4]
	mov r2, #1
	str r2, [r0, r1]
	ldr r2, _0222A3FC ; =0x00001094
	mov r3, #0
	str r3, [r0, r2]
	sub r2, r1, #4
	ldr r1, [sp, #4]
	ldr r0, _0222A400 ; =ov44_0222A40C
	add r1, r1, r2
	mov r2, #0x14
	bl SysTask_CreateOnVBlankQueue
	ldr r2, _0222A3F4 ; =0x00000D68
	ldr r1, [sp, #4]
	str r0, [r1, r2]
	ldr r0, _0222A404 ; =ov44_0222A7F8
	bl Main_SetVBlankIntrCB
	bl ov44_0222AFE8
	ldr r0, [sp, #4]
	ldr r1, [sp, #0x20]
	bl ov44_0222B030
	ldr r0, [sp, #4]
	bl ov44_0222B0A4
	ldr r0, _0222A408 ; =gSystem + 0x60
	mov r1, #1
	strb r1, [r0, #9]
	bl GfGfx_SwapDisplay
	ldr r0, [sp, #0x20]
	bl NARC_Delete
	add sp, #0x30
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
_0222A3E8: .word 0xFFFFE0FF
_0222A3EC: .word 0x04001000
_0222A3F0: .word 0x0000030A
_0222A3F4: .word 0x00000D68
_0222A3F8: .word 0x00000D6C
_0222A3FC: .word 0x00001094
_0222A400: .word ov44_0222A40C
_0222A404: .word ov44_0222A7F8
_0222A408: .word gSystem + 0x60
	thumb_func_end ov44_0222A1FC

	thumb_func_start ov44_0222A40C
ov44_0222A40C: ; 0x0222A40C
	push {r4, lr}
	add r4, r1, #0
	ldr r0, [r4, #4]
	cmp r0, #0
	beq _0222A4AA
	ldr r1, _0222A4AC ; =0x0000032B
	mov r0, #1
	ldrb r2, [r4, r1]
	eor r2, r0
	strb r2, [r4, r1]
	ldrb r2, [r4, r1]
	tst r0, r2
	bne _0222A4AA
	add r0, r1, #1
	ldr r0, [r4, r0]
	cmp r0, #1
	bhi _0222A442
	sub r0, r1, #3
	ldrsh r0, [r4, r0]
	add r2, r4, #0
	add r2, #0x88
	lsl r0, r0, #5
	add r0, r2, r0
	mov r1, #0
	mov r2, #0x20
	bl GX_LoadBGPltt
_0222A442:
	mov r0, #0xcb
	lsl r0, r0, #2
	ldr r0, [r4, r0]
	cmp r0, #0
	beq _0222A450
	cmp r0, #2
	bne _0222A466
_0222A450:
	mov r0, #0xca
	lsl r0, r0, #2
	ldrsh r0, [r4, r0]
	add r1, r4, #0
	add r1, #0x88
	lsl r0, r0, #5
	add r0, r1, r0
	mov r1, #0
	mov r2, #0x20
	bl GXS_LoadBGPltt
_0222A466:
	ldr r0, _0222A4B0 ; =0x0000032A
	ldrsb r1, [r4, r0]
	cmp r1, #0
	bne _0222A48E
	sub r1, r0, #2
	ldrsh r1, [r4, r1]
	add r2, r1, #1
	sub r1, r0, #2
	strh r2, [r4, r1]
	ldrsh r1, [r4, r1]
	cmp r1, #0x15
	blt _0222A4AA
	mov r2, #0x13
	sub r1, r0, #2
	strh r2, [r4, r1]
	ldrsb r2, [r4, r0]
	mov r1, #1
	eor r1, r2
	strb r1, [r4, r0]
	pop {r4, pc}
_0222A48E:
	sub r1, r0, #2
	ldrsh r1, [r4, r1]
	sub r2, r1, #1
	sub r1, r0, #2
	strh r2, [r4, r1]
	ldrsh r1, [r4, r1]
	cmp r1, #0
	bge _0222A4AA
	mov r2, #1
	sub r1, r0, #2
	strh r2, [r4, r1]
	ldrsb r1, [r4, r0]
	eor r1, r2
	strb r1, [r4, r0]
_0222A4AA:
	pop {r4, pc}
	.balign 4, 0
_0222A4AC: .word 0x0000032B
_0222A4B0: .word 0x0000032A
	thumb_func_end ov44_0222A40C

	thumb_func_start ov44_0222A4B4
ov44_0222A4B4: ; 0x0222A4B4
	push {r3, r4, r5, r6, lr}
	sub sp, #0xc
	add r6, r1, #0
	ldr r1, [r6]
	add r5, r0, #0
	cmp r1, #0
	beq _0222A4CA
	cmp r1, #1
	bne _0222A4C8
	b _0222A5F2
_0222A4C8:
	b _0222A600
_0222A4CA:
	bl OverlayManager_GetArgs
	add r4, r0, #0
	bl sub_02039998
	cmp r0, #0
	beq _0222A510
	ldr r0, [r4, #4]
	cmp r0, #0xc
	bhi _0222A51A
	add r0, r0, r0
	add r0, pc
	ldrh r0, [r0, #6]
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	add pc, r0
_0222A4EA: ; jump table
	.short _0222A51A - _0222A4EA - 2 ; case 0
	.short _0222A504 - _0222A4EA - 2 ; case 1
	.short _0222A504 - _0222A4EA - 2 ; case 2
	.short _0222A504 - _0222A4EA - 2 ; case 3
	.short _0222A504 - _0222A4EA - 2 ; case 4
	.short _0222A504 - _0222A4EA - 2 ; case 5
	.short _0222A504 - _0222A4EA - 2 ; case 6
	.short _0222A504 - _0222A4EA - 2 ; case 7
	.short _0222A504 - _0222A4EA - 2 ; case 8
	.short _0222A504 - _0222A4EA - 2 ; case 9
	.short _0222A504 - _0222A4EA - 2 ; case 10
	.short _0222A51A - _0222A4EA - 2 ; case 11
	.short _0222A504 - _0222A4EA - 2 ; case 12
_0222A504:
	mov r1, #0x80
	mov r0, #3
	lsl r2, r1, #9
	bl Heap_Create
	b _0222A51A
_0222A510:
	mov r1, #0x80
	mov r0, #3
	lsl r2, r1, #9
	bl Heap_Create
_0222A51A:
	ldr r0, _0222A608 ; =FS_OVERLAY_ID(OVY_42)
	mov r1, #2
	bl HandleLoadOverlay
	ldr r0, [r4, #4]
	cmp r0, #1
	bne _0222A536
	mov r2, #7
	mov r0, #3
	mov r1, #0x35
	lsl r2, r2, #0x10
	bl Heap_Create
	b _0222A542
_0222A536:
	mov r2, #0xa
	mov r0, #3
	mov r1, #0x35
	lsl r2, r2, #0x10
	bl Heap_Create
_0222A542:
	mov r1, #0x43
	add r0, r5, #0
	lsl r1, r1, #6
	mov r2, #0x35
	bl OverlayManager_CreateAndGetData
	mov r2, #0x43
	mov r1, #0
	lsl r2, r2, #6
	add r5, r0, #0
	bl MI_CpuFill8
	mov r0, #0x20
	mov r1, #0x35
	bl GF_CreateVramTransferManager
	mov r1, #6
	mov r0, #8
	lsl r1, r1, #6
	str r0, [r5, r1]
	add r0, r1, #0
	ldr r2, [r4]
	sub r0, #0x20
	sub r1, #0x20
	str r2, [r5, r0]
	ldr r0, [r5, r1]
	bl sub_0202C6F4
	str r0, [r5]
	mov r1, #0xd5
	ldr r0, [r4, #4]
	lsl r1, r1, #2
	str r0, [r5, r1]
	add r0, r1, #0
	mov r2, #8
	sub r0, #8
	str r2, [r5, r0]
	sub r2, #9
	add r1, #0x1c
	str r2, [r5, r1]
	mov r0, #0xf
	mov r1, #0
	bl GfGfx_EngineATogglePlanes
	mov r0, #0xf
	mov r1, #0
	bl GfGfx_EngineBTogglePlanes
	add r0, r5, #0
	bl ov44_0222ADF8
	add r0, r5, #0
	bl ov44_0222A1FC
	mov r0, #6
	str r0, [sp]
	mov r1, #1
	str r1, [sp, #4]
	mov r0, #0x35
	str r0, [sp, #8]
	mov r0, #0
	add r2, r1, #0
	add r3, r0, #0
	bl BeginNormalPaletteFade
	bl sub_02039998
	cmp r0, #0
	beq _0222A5D2
	bl sub_0203A880
	b _0222A5DC
_0222A5D2:
	bl LoadDwcOverlay
	mov r0, #0x35
	bl sub_02039FD8
_0222A5DC:
	mov r0, #4
	bl sub_02021148
	cmp r0, #1
	beq _0222A5EA
	bl GF_AssertFail
_0222A5EA:
	ldr r0, [r6]
	add r0, r0, #1
	str r0, [r6]
	b _0222A600
_0222A5F2:
	bl OverlayManager_GetData
	mov r0, #0
	str r0, [r6]
	add sp, #0xc
	mov r0, #1
	pop {r3, r4, r5, r6, pc}
_0222A600:
	mov r0, #0
	add sp, #0xc
	pop {r3, r4, r5, r6, pc}
	nop
_0222A608: .word FS_OVERLAY_ID(OVY_42)
	thumb_func_end ov44_0222A4B4

	thumb_func_start ov44_0222A60C
ov44_0222A60C: ; 0x0222A60C
	push {r4, r5, r6, lr}
	add r5, r1, #0
	bl OverlayManager_GetData
	ldr r1, [r5]
	add r4, r0, #0
	cmp r1, #0
	beq _0222A626
	cmp r1, #1
	beq _0222A634
	cmp r1, #2
	beq _0222A64E
	b _0222A65A
_0222A626:
	bl IsPaletteFadeFinished
	cmp r0, #0
	beq _0222A65A
	mov r0, #1
	str r0, [r5]
	b _0222A65A
_0222A634:
	mov r2, #0xd2
	lsl r2, r2, #2
	ldr r2, [r4, r2]
	ldr r3, _0222A684 ; =ov44_0223676C
	lsl r6, r2, #2
	ldr r6, [r3, r6]
	cmp r6, #0
	beq _0222A65A
	ldr r3, _0222A688 ; =_022368A0
	str r2, [r3]
	blx r6
	str r0, [r5]
	b _0222A65A
_0222A64E:
	bl IsPaletteFadeFinished
	cmp r0, #0
	beq _0222A65A
	mov r0, #1
	pop {r4, r5, r6, pc}
_0222A65A:
	mov r0, #0x19
	lsl r0, r0, #4
	ldr r0, [r4, r0]
	cmp r0, #0
	beq _0222A668
	bl SpriteList_RenderAndAnimateSprites
_0222A668:
	mov r0, #0xe3
	lsl r0, r0, #2
	add r0, r4, r0
	bl ov44_02231BB0
	cmp r0, #1
	bne _0222A680
	mov r0, #0xe3
	lsl r0, r0, #2
	add r0, r4, r0
	bl ov44_02231C60
_0222A680:
	mov r0, #0
	pop {r4, r5, r6, pc}
	.balign 4, 0
_0222A684: .word ov44_0223676C
_0222A688: .word _022368A0
	thumb_func_end ov44_0222A60C

	thumb_func_start ov44_0222A68C
ov44_0222A68C: ; 0x0222A68C
	push {r3, r4, r5, r6, r7, lr}
	add r5, r0, #0
	mov r0, #0
	add r1, r0, #0
	bl Main_SetVBlankIntrCB
	ldr r0, _0222A74C ; =0x00000D68
	ldr r0, [r5, r0]
	cmp r0, #0
	beq _0222A6AE
	bl SysTask_Destroy
	ldr r0, _0222A74C ; =0x00000D68
	mov r1, #0
	str r1, [r5, r0]
	add r0, r0, #4
	str r1, [r5, r0]
_0222A6AE:
	mov r0, #0xe3
	lsl r0, r0, #2
	add r0, r5, r0
	bl ov44_02231BB0
	cmp r0, #1
	bne _0222A6C6
	mov r0, #0xe3
	lsl r0, r0, #2
	add r0, r5, r0
	bl ov44_02231B4C
_0222A6C6:
	add r0, r5, #0
	bl ov44_0222F98C
	cmp r0, #1
	bne _0222A6D6
	add r0, r5, #0
	bl ov44_0222FA28
_0222A6D6:
	ldr r0, _0222A750 ; =0x00000B0C
	add r0, r5, r0
	bl ov44_0222ABB8
	mov r0, #0xb3
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	bl sub_020135AC
	mov r7, #0xaf
	mov r6, #0
	add r4, r5, #0
	lsl r7, r7, #2
_0222A6F0:
	ldr r0, [r4, r7]
	bl Destroy2DGfxResObjMan
	add r6, r6, #1
	add r4, r4, #4
	cmp r6, #4
	blt _0222A6F0
	mov r0, #0x19
	lsl r0, r0, #4
	ldr r0, [r5, r0]
	bl SpriteList_Delete
	bl OamManager_Free
	bl ObjCharTransfer_Destroy
	bl ObjPlttTransfer_Destroy
	add r0, r5, #0
	bl ov44_0222B164
	mov r0, #0x57
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	bl ov44_0222AE74
	mov r0, #0x5b
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	bl DestroyMsgData
	mov r0, #0x5a
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	bl DestroyMsgData
	mov r0, #0x59
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	bl MessageFormat_Delete
	ldr r0, _0222A754 ; =FS_OVERLAY_ID(OVY_42)
	bl UnloadOverlayByID
	pop {r3, r4, r5, r6, r7, pc}
	nop
_0222A74C: .word 0x00000D68
_0222A750: .word 0x00000B0C
_0222A754: .word FS_OVERLAY_ID(OVY_42)
	thumb_func_end ov44_0222A68C

	thumb_func_start ov44_0222A758
ov44_0222A758: ; 0x0222A758
	push {r4, r5, r6, lr}
	add r5, r0, #0
	bl OverlayManager_GetData
	add r4, r0, #0
	bl ov44_0222A68C
	add r0, r5, #0
	bl OverlayManager_GetArgs
	add r6, r0, #0
	mov r0, #0xd3
	lsl r0, r0, #2
	ldr r1, [r4, r0]
	str r1, [r6, #4]
	ldr r0, [r4, r0]
	cmp r0, #8
	beq _0222A780
	cmp r0, #0xa
	bne _0222A78C
_0222A780:
	bl UnloadDwcOverlay
	mov r0, #0
	bl TextFlags_SetCanTouchSpeedUpPrint
	b _0222A792
_0222A78C:
	bl ov00_021E6EBC
	str r0, [r6, #8]
_0222A792:
	add r0, r4, #0
	bl ov44_0222AE44
	add r0, r5, #0
	bl OverlayManager_FreeData
	bl sub_02021238
	bl GF_DestroyVramTransferManager
	mov r0, #0x35
	bl Heap_Destroy
	mov r0, #0xd3
	lsl r0, r0, #2
	ldr r0, [r4, r0]
	cmp r0, #0xc
	bhi _0222A7E2
	add r0, r0, r0
	add r0, pc
	ldrh r0, [r0, #6]
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	add pc, r0
_0222A7C2: ; jump table
	.short _0222A7E2 - _0222A7C2 - 2 ; case 0
	.short _0222A7DC - _0222A7C2 - 2 ; case 1
	.short _0222A7DC - _0222A7C2 - 2 ; case 2
	.short _0222A7DC - _0222A7C2 - 2 ; case 3
	.short _0222A7DC - _0222A7C2 - 2 ; case 4
	.short _0222A7DC - _0222A7C2 - 2 ; case 5
	.short _0222A7DC - _0222A7C2 - 2 ; case 6
	.short _0222A7DC - _0222A7C2 - 2 ; case 7
	.short _0222A7DC - _0222A7C2 - 2 ; case 8
	.short _0222A7DC - _0222A7C2 - 2 ; case 9
	.short _0222A7DC - _0222A7C2 - 2 ; case 10
	.short _0222A7E2 - _0222A7C2 - 2 ; case 11
	.short _0222A7DC - _0222A7C2 - 2 ; case 12
_0222A7DC:
	mov r0, #0x80
	bl Heap_Destroy
_0222A7E2:
	bl ov44_0222E074
	cmp r0, #1
	bne _0222A7F2
	mov r0, #0
	mov r1, #0x78
	bl GF_SndHandleSetInitialVolume
_0222A7F2:
	mov r0, #1
	pop {r4, r5, r6, pc}
	.balign 4, 0
	thumb_func_end ov44_0222A758

	thumb_func_start ov44_0222A7F8
ov44_0222A7F8: ; 0x0222A7F8
	push {r4, lr}
	add r4, r0, #0
	mov r0, #0x57
	lsl r0, r0, #2
	ldr r0, [r4, r0]
	bl DoScheduledBgGpuUpdates
	bl GF_RunVramTransferTasks
	bl OamManager_ApplyAndResetBuffers
	ldr r0, _0222A82C ; =0x000010BC
	ldrh r0, [r4, r0]
	cmp r0, #1
	bne _0222A828
	mov r0, #0
	bl SetMasterBrightnessNeutral
	mov r0, #1
	bl SetMasterBrightnessNeutral
	ldr r0, _0222A82C ; =0x000010BC
	mov r1, #0
	strh r1, [r4, r0]
_0222A828:
	pop {r4, pc}
	nop
_0222A82C: .word 0x000010BC
	thumb_func_end ov44_0222A7F8

	thumb_func_start ov44_0222A830
ov44_0222A830: ; 0x0222A830
	push {r4, lr}
	sub sp, #0x28
	ldr r4, _0222A84C ; =ov44_02235548
	add r3, sp, #0
	mov r2, #5
_0222A83A:
	ldmia r4!, {r0, r1}
	stmia r3!, {r0, r1}
	sub r2, r2, #1
	bne _0222A83A
	add r0, sp, #0
	bl GfGfx_SetBanks
	add sp, #0x28
	pop {r4, pc}
	.balign 4, 0
_0222A84C: .word ov44_02235548
	thumb_func_end ov44_0222A830

	thumb_func_start ov44_0222A850
ov44_0222A850: ; 0x0222A850
	push {r3, r4, r5, lr}
	sub sp, #0xf0
	ldr r5, _0222AA68 ; =ov44_022353B0
	add r3, sp, #0xe0
	add r4, r0, #0
	add r2, r3, #0
	ldmia r5!, {r0, r1}
	stmia r3!, {r0, r1}
	ldmia r5!, {r0, r1}
	stmia r3!, {r0, r1}
	add r0, r2, #0
	bl SetBothScreensModesAndDisable
	ldr r5, _0222AA6C ; =ov44_022353E8
	add r3, sp, #0xc4
	ldmia r5!, {r0, r1}
	add r2, r3, #0
	stmia r3!, {r0, r1}
	ldmia r5!, {r0, r1}
	stmia r3!, {r0, r1}
	ldmia r5!, {r0, r1}
	stmia r3!, {r0, r1}
	ldr r0, [r5]
	mov r1, #0
	str r0, [r3]
	add r0, r4, #0
	add r3, r1, #0
	bl InitBgFromTemplate
	mov r0, #0
	mov r1, #0x20
	add r2, r0, #0
	mov r3, #0x35
	bl BG_ClearCharDataRange
	add r0, r4, #0
	mov r1, #0
	bl BgClearTilemapBufferAndCommit
	ldr r5, _0222AA70 ; =ov44_02235404
	add r3, sp, #0xa8
	ldmia r5!, {r0, r1}
	add r2, r3, #0
	stmia r3!, {r0, r1}
	ldmia r5!, {r0, r1}
	stmia r3!, {r0, r1}
	ldmia r5!, {r0, r1}
	stmia r3!, {r0, r1}
	ldr r0, [r5]
	mov r1, #1
	str r0, [r3]
	add r0, r4, #0
	mov r3, #0
	bl InitBgFromTemplate
	mov r0, #1
	mov r1, #0x20
	mov r2, #0
	mov r3, #0x35
	bl BG_ClearCharDataRange
	add r0, r4, #0
	mov r1, #1
	bl BgClearTilemapBufferAndCommit
	ldr r5, _0222AA74 ; =ov44_02235490
	add r3, sp, #0x8c
	ldmia r5!, {r0, r1}
	add r2, r3, #0
	stmia r3!, {r0, r1}
	ldmia r5!, {r0, r1}
	stmia r3!, {r0, r1}
	ldmia r5!, {r0, r1}
	stmia r3!, {r0, r1}
	ldr r0, [r5]
	mov r1, #2
	str r0, [r3]
	add r0, r4, #0
	mov r3, #0
	bl InitBgFromTemplate
	mov r0, #2
	mov r1, #0x20
	mov r2, #0
	mov r3, #0x35
	bl BG_ClearCharDataRange
	add r0, r4, #0
	mov r1, #2
	bl BgClearTilemapBufferAndCommit
	ldr r5, _0222AA78 ; =ov44_02235420
	add r3, sp, #0x70
	ldmia r5!, {r0, r1}
	add r2, r3, #0
	stmia r3!, {r0, r1}
	ldmia r5!, {r0, r1}
	stmia r3!, {r0, r1}
	ldmia r5!, {r0, r1}
	stmia r3!, {r0, r1}
	ldr r0, [r5]
	mov r1, #3
	str r0, [r3]
	add r0, r4, #0
	mov r3, #0
	bl InitBgFromTemplate
	mov r0, #3
	mov r1, #0x20
	mov r2, #0
	mov r3, #0x35
	bl BG_ClearCharDataRange
	add r0, r4, #0
	mov r1, #3
	bl BgClearTilemapBufferAndCommit
	ldr r5, _0222AA7C ; =ov44_0223543C
	add r3, sp, #0x54
	ldmia r5!, {r0, r1}
	add r2, r3, #0
	stmia r3!, {r0, r1}
	ldmia r5!, {r0, r1}
	stmia r3!, {r0, r1}
	ldmia r5!, {r0, r1}
	stmia r3!, {r0, r1}
	ldr r0, [r5]
	mov r1, #4
	str r0, [r3]
	add r0, r4, #0
	mov r3, #0
	bl InitBgFromTemplate
	add r0, r4, #0
	mov r1, #4
	bl BgClearTilemapBufferAndCommit
	ldr r5, _0222AA80 ; =ov44_02235458
	add r3, sp, #0x38
	ldmia r5!, {r0, r1}
	add r2, r3, #0
	stmia r3!, {r0, r1}
	ldmia r5!, {r0, r1}
	stmia r3!, {r0, r1}
	ldmia r5!, {r0, r1}
	stmia r3!, {r0, r1}
	ldr r0, [r5]
	mov r1, #5
	str r0, [r3]
	add r0, r4, #0
	mov r3, #0
	bl InitBgFromTemplate
	add r0, r4, #0
	mov r1, #5
	bl BgClearTilemapBufferAndCommit
	ldr r5, _0222AA84 ; =ov44_02235474
	add r3, sp, #0x1c
	ldmia r5!, {r0, r1}
	add r2, r3, #0
	stmia r3!, {r0, r1}
	ldmia r5!, {r0, r1}
	stmia r3!, {r0, r1}
	ldmia r5!, {r0, r1}
	stmia r3!, {r0, r1}
	ldr r0, [r5]
	mov r1, #6
	str r0, [r3]
	add r0, r4, #0
	mov r3, #0
	bl InitBgFromTemplate
	add r0, r4, #0
	mov r1, #6
	bl BgClearTilemapBufferAndCommit
	ldr r5, _0222AA88 ; =ov44_022354AC
	add r3, sp, #0
	ldmia r5!, {r0, r1}
	add r2, r3, #0
	stmia r3!, {r0, r1}
	ldmia r5!, {r0, r1}
	stmia r3!, {r0, r1}
	ldmia r5!, {r0, r1}
	stmia r3!, {r0, r1}
	ldr r0, [r5]
	mov r1, #7
	str r0, [r3]
	add r0, r4, #0
	mov r3, #0
	bl InitBgFromTemplate
	add r0, r4, #0
	mov r1, #7
	bl BgClearTilemapBufferAndCommit
	mov r0, #0
	mov r1, #0x20
	add r2, r0, #0
	mov r3, #0x35
	bl BG_ClearCharDataRange
	mov r0, #1
	mov r1, #0x20
	mov r2, #0
	mov r3, #0x35
	bl BG_ClearCharDataRange
	mov r0, #2
	mov r1, #0x20
	mov r2, #0
	mov r3, #0x35
	bl BG_ClearCharDataRange
	mov r0, #3
	mov r1, #0x20
	mov r2, #0
	mov r3, #0x35
	bl BG_ClearCharDataRange
	mov r0, #4
	mov r1, #0x20
	mov r2, #0
	mov r3, #0x35
	bl BG_ClearCharDataRange
	mov r0, #5
	mov r1, #0x20
	mov r2, #0
	mov r3, #0x35
	bl BG_ClearCharDataRange
	mov r0, #6
	mov r1, #0x20
	mov r2, #0
	mov r3, #0x35
	bl BG_ClearCharDataRange
	mov r0, #7
	mov r1, #0x20
	mov r2, #0
	mov r3, #0x35
	bl BG_ClearCharDataRange
	mov r0, #0
	mov r1, #3
	bl SetBgPriority
	mov r0, #1
	add r1, r0, #0
	bl SetBgPriority
	mov r0, #3
	mov r1, #0
	bl SetBgPriority
	mov r0, #2
	mov r1, #0
	bl SetBgPriority
	ldr r0, _0222AA8C ; =0x04000050
	mov r1, #0
	strh r1, [r0]
	ldr r0, _0222AA90 ; =0x04001050
	strh r1, [r0]
	add sp, #0xf0
	pop {r3, r4, r5, pc}
	.balign 4, 0
_0222AA68: .word ov44_022353B0
_0222AA6C: .word ov44_022353E8
_0222AA70: .word ov44_02235404
_0222AA74: .word ov44_02235490
_0222AA78: .word ov44_02235420
_0222AA7C: .word ov44_0223543C
_0222AA80: .word ov44_02235458
_0222AA84: .word ov44_02235474
_0222AA88: .word ov44_022354AC
_0222AA8C: .word 0x04000050
_0222AA90: .word 0x04001050
	thumb_func_end ov44_0222A850

	thumb_func_start ov44_0222AA94
ov44_0222AA94: ; 0x0222AA94
	push {r3, r4, r5, lr}
	ldr r2, _0222AABC ; =0x00000A88
	mov r4, #0
	add r5, r0, #0
_0222AA9C:
	ldr r3, [r5, r2]
	cmp r3, #0
	bne _0222AAAC
	lsl r2, r4, #2
	add r2, r0, r2
	ldr r0, _0222AABC ; =0x00000A88
	str r1, [r2, r0]
	pop {r3, r4, r5, pc}
_0222AAAC:
	add r4, r4, #1
	add r5, r5, #4
	cmp r4, #0x21
	blt _0222AA9C
	bl GF_AssertFail
	pop {r3, r4, r5, pc}
	nop
_0222AABC: .word 0x00000A88
	thumb_func_end ov44_0222AA94

	thumb_func_start ov44_0222AAC0
ov44_0222AAC0: ; 0x0222AAC0
	push {r3, r4, r5, lr}
	ldr r2, _0222AAE8 ; =0x00000A88
	mov r4, #0
	add r5, r0, #0
_0222AAC8:
	ldr r3, [r5, r2]
	cmp r3, r1
	bne _0222AADA
	lsl r1, r4, #2
	add r1, r0, r1
	ldr r0, _0222AAE8 ; =0x00000A88
	mov r2, #0
	str r2, [r1, r0]
	pop {r3, r4, r5, pc}
_0222AADA:
	add r4, r4, #1
	add r5, r5, #4
	cmp r4, #0x21
	blt _0222AAC8
	bl GF_AssertFail
	pop {r3, r4, r5, pc}
	.balign 4, 0
_0222AAE8: .word 0x00000A88
	thumb_func_end ov44_0222AAC0

	thumb_func_start ov44_0222AAEC
ov44_0222AAEC: ; 0x0222AAEC
	push {r3, r4, r5, r6, r7, lr}
	ldr r7, _0222AB20 ; =0x00000A88
	str r0, [sp]
	add r6, r1, #0
	mov r4, #0
	add r5, r0, #0
_0222AAF8:
	ldr r0, [r5, r7]
	cmp r0, #0
	beq _0222AB12
	bl ov44_02231EF4
	cmp r6, r0
	bne _0222AB12
	ldr r0, [sp]
	lsl r1, r4, #2
	add r1, r0, r1
	ldr r0, _0222AB20 ; =0x00000A88
	ldr r0, [r1, r0]
	pop {r3, r4, r5, r6, r7, pc}
_0222AB12:
	add r4, r4, #1
	add r5, r5, #4
	cmp r4, #0x21
	blt _0222AAF8
	mov r0, #0
	pop {r3, r4, r5, r6, r7, pc}
	nop
_0222AB20: .word 0x00000A88
	thumb_func_end ov44_0222AAEC

	thumb_func_start ov44_0222AB24
ov44_0222AB24: ; 0x0222AB24
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x10
	add r7, r0, #0
	add r6, r2, #0
	add r4, r3, #0
	mov r0, #0x40
	str r0, [sp]
	mov r3, #0xe
	add r5, r1, #0
	add r1, r3, #0
	add r0, r6, #0
	mov r2, #0
	add r3, #0xf2
	str r4, [sp, #4]
	bl GfGfxLoader_GXLoadPalFromOpenNarc
	mov r0, #0x40
	str r0, [sp]
	mov r3, #0x1a
	add r0, r6, #0
	mov r1, #0xe
	mov r2, #4
	lsl r3, r3, #4
	str r4, [sp, #4]
	bl GfGfxLoader_GXLoadPalFromOpenNarc
	mov r0, #0
	str r0, [sp]
	str r0, [sp, #4]
	str r0, [sp, #8]
	add r0, r6, #0
	mov r1, #0xd
	add r2, r7, #0
	mov r3, #1
	str r4, [sp, #0xc]
	bl GfGfxLoader_LoadCharDataFromOpenNarc
	mov r0, #0
	str r0, [sp]
	str r0, [sp, #4]
	str r0, [sp, #8]
	str r4, [sp, #0xc]
	add r0, r6, #0
	mov r1, #0xd
	add r2, r7, #0
	mov r3, #6
	bl GfGfxLoader_LoadCharDataFromOpenNarc
	ldr r0, [r5, #8]
	cmp r0, #0
	bne _0222AB9C
	add r3, r5, #0
	add r0, r6, #0
	mov r1, #0xd
	mov r2, #0
	add r3, #0xc
	str r4, [sp]
	bl GfGfxLoader_GetCharDataFromOpenNarc
	str r0, [r5, #8]
_0222AB9C:
	ldr r0, [r5]
	cmp r0, #0
	bne _0222ABB2
	add r0, r6, #0
	mov r1, #0xf
	mov r2, #0
	add r3, r5, #4
	str r4, [sp]
	bl GfGfxLoader_GetScrnDataFromOpenNarc
	str r0, [r5]
_0222ABB2:
	add sp, #0x10
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
	thumb_func_end ov44_0222AB24

	thumb_func_start ov44_0222ABB8
ov44_0222ABB8: ; 0x0222ABB8
	push {r4, lr}
	add r4, r0, #0
	ldr r0, [r4]
	cmp r0, #0
	beq _0222ABCA
	bl Heap_Free
	mov r0, #0
	str r0, [r4]
_0222ABCA:
	ldr r0, [r4, #8]
	cmp r0, #0
	beq _0222ABD8
	bl Heap_Free
	mov r0, #0
	str r0, [r4, #8]
_0222ABD8:
	pop {r4, pc}
	.balign 4, 0
	thumb_func_end ov44_0222ABB8

	thumb_func_start ov44_0222ABDC
ov44_0222ABDC: ; 0x0222ABDC
	push {r3, r4, r5, r6, lr}
	sub sp, #0x1c
	ldr r1, [r1, #4]
	add r4, r2, #0
	mov r2, #2
	str r2, [sp]
	str r2, [sp, #4]
	add r2, r1, #0
	add r2, #0xc
	str r2, [sp, #8]
	ldr r2, [sp, #0x34]
	add r6, r3, #0
	lsl r2, r2, #0x19
	lsr r2, r2, #0x18
	str r2, [sp, #0xc]
	mov r2, #0
	str r2, [sp, #0x10]
	ldrh r2, [r1]
	add r3, sp, #0x20
	add r5, r0, #0
	lsl r2, r2, #0x15
	lsr r2, r2, #0x18
	str r2, [sp, #0x14]
	ldrh r1, [r1, #2]
	add r2, r6, #0
	lsl r1, r1, #0x15
	lsr r1, r1, #0x18
	str r1, [sp, #0x18]
	lsl r1, r4, #0x18
	ldrb r3, [r3, #0x10]
	lsr r1, r1, #0x18
	bl CopyToBgTilemapRect
	add r0, r4, #0
	bl ov44_0222ADC4
	mov r1, #2
	str r1, [sp]
	str r1, [sp, #4]
	ldr r1, [sp, #0x38]
	add r3, sp, #0x20
	add r0, r0, r1
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	str r0, [sp, #8]
	lsl r1, r4, #0x18
	ldrb r3, [r3, #0x10]
	add r0, r5, #0
	lsr r1, r1, #0x18
	add r2, r6, #0
	bl BgTilemapRectChangePalette
	lsl r1, r4, #0x18
	add r0, r5, #0
	lsr r1, r1, #0x18
	bl ScheduleBgTilemapBufferTransfer
	add sp, #0x1c
	pop {r3, r4, r5, r6, pc}
	.balign 4, 0
	thumb_func_end ov44_0222ABDC

	thumb_func_start ov44_0222AC54
ov44_0222AC54: ; 0x0222AC54
	push {r3, r4, r5, r6, lr}
	sub sp, #0xc
	ldr r5, [sp, #0x24]
	mov r4, #0
	cmp r5, #0x1d
	bhi _0222ACD4
	add r5, r5, r5
	add r5, pc
	ldrh r5, [r5, #6]
	lsl r5, r5, #0x10
	asr r5, r5, #0x10
	add pc, r5
_0222AC6C: ; jump table
	.short _0222ACCC - _0222AC6C - 2 ; case 0
	.short _0222ACA8 - _0222AC6C - 2 ; case 1
	.short _0222ACAE - _0222AC6C - 2 ; case 2
	.short _0222ACAE - _0222AC6C - 2 ; case 3
	.short _0222ACAE - _0222AC6C - 2 ; case 4
	.short _0222ACAE - _0222AC6C - 2 ; case 5
	.short _0222ACAE - _0222AC6C - 2 ; case 6
	.short _0222ACAE - _0222AC6C - 2 ; case 7
	.short _0222ACB4 - _0222AC6C - 2 ; case 8
	.short _0222ACAC - _0222AC6C - 2 ; case 9
	.short _0222ACAC - _0222AC6C - 2 ; case 10
	.short _0222ACAC - _0222AC6C - 2 ; case 11
	.short _0222ACAC - _0222AC6C - 2 ; case 12
	.short _0222ACAC - _0222AC6C - 2 ; case 13
	.short _0222ACAC - _0222AC6C - 2 ; case 14
	.short _0222ACB2 - _0222AC6C - 2 ; case 15
	.short _0222ACC8 - _0222AC6C - 2 ; case 16
	.short _0222ACD4 - _0222AC6C - 2 ; case 17
	.short _0222ACBE - _0222AC6C - 2 ; case 18
	.short _0222ACBE - _0222AC6C - 2 ; case 19
	.short _0222ACC4 - _0222AC6C - 2 ; case 20
	.short _0222ACC2 - _0222AC6C - 2 ; case 21
	.short _0222ACBA - _0222AC6C - 2 ; case 22
	.short _0222ACB8 - _0222AC6C - 2 ; case 23
	.short _0222ACBA - _0222AC6C - 2 ; case 24
	.short _0222ACB8 - _0222AC6C - 2 ; case 25
	.short _0222ACBA - _0222AC6C - 2 ; case 26
	.short _0222ACB8 - _0222AC6C - 2 ; case 27
	.short _0222ACD0 - _0222AC6C - 2 ; case 28
	.short _0222ACD0 - _0222AC6C - 2 ; case 29
_0222ACA8:
	mov r5, #3
	b _0222ACD6
_0222ACAC:
	mov r4, #1
_0222ACAE:
	mov r5, #6
	b _0222ACD6
_0222ACB2:
	mov r4, #1
_0222ACB4:
	mov r5, #7
	b _0222ACD6
_0222ACB8:
	mov r4, #1
_0222ACBA:
	mov r5, #0xa
	b _0222ACD6
_0222ACBE:
	mov r5, #5
	b _0222ACD6
_0222ACC2:
	mov r4, #1
_0222ACC4:
	mov r5, #0xb
	b _0222ACD6
_0222ACC8:
	mov r5, #4
	b _0222ACD6
_0222ACCC:
	mov r5, #8
	b _0222ACD6
_0222ACD0:
	mov r5, #5
	b _0222ACD6
_0222ACD4:
	mov r5, #5
_0222ACD6:
	add r6, sp, #0x10
	ldrb r6, [r6, #0x10]
	str r6, [sp]
	str r5, [sp, #4]
	str r4, [sp, #8]
	bl ov44_0222ABDC
	add sp, #0xc
	pop {r3, r4, r5, r6, pc}
	thumb_func_end ov44_0222AC54

	thumb_func_start ov44_0222ACE8
ov44_0222ACE8: ; 0x0222ACE8
	push {r3, r4, r5, lr}
	sub sp, #0x18
	ldr r1, [r1, #0xc]
	add r4, r0, #0
	ldrh r5, [r1, #2]
	lsl r5, r5, #0x13
	lsr r5, r5, #0x10
	str r5, [sp]
	ldrh r5, [r1]
	lsl r5, r5, #0x13
	lsr r5, r5, #0x10
	str r5, [sp, #4]
	str r2, [sp, #8]
	str r3, [sp, #0xc]
	mov r2, #0x10
	str r2, [sp, #0x10]
	str r2, [sp, #0x14]
	ldr r2, [sp, #0x28]
	ldr r1, [r1, #0x14]
	lsl r2, r2, #0x14
	lsr r2, r2, #0x10
	mov r3, #0
	bl BlitBitmapRectToWindow
	ldrb r0, [r4, #4]
	bl ov44_0222ADC4
	add r2, r0, #0
	ldr r1, [sp, #0x2c]
	add r0, r4, #0
	add r1, r2, r1
	lsl r1, r1, #0x18
	lsr r1, r1, #0x18
	bl SetWindowPaletteNum
	add sp, #0x18
	pop {r3, r4, r5, pc}
	.balign 4, 0
	thumb_func_end ov44_0222ACE8

	thumb_func_start ov44_0222AD34
ov44_0222AD34: ; 0x0222AD34
	push {r3, r4, r5, lr}
	sub sp, #8
	ldr r5, [sp, #0x18]
	mov r4, #0
	cmp r5, #0x1d
	bhi _0222ADB4
	add r5, r5, r5
	add r5, pc
	ldrh r5, [r5, #6]
	lsl r5, r5, #0x10
	asr r5, r5, #0x10
	add pc, r5
_0222AD4C: ; jump table
	.short _0222ADAC - _0222AD4C - 2 ; case 0
	.short _0222AD88 - _0222AD4C - 2 ; case 1
	.short _0222AD8E - _0222AD4C - 2 ; case 2
	.short _0222AD8E - _0222AD4C - 2 ; case 3
	.short _0222AD8E - _0222AD4C - 2 ; case 4
	.short _0222AD8E - _0222AD4C - 2 ; case 5
	.short _0222AD8E - _0222AD4C - 2 ; case 6
	.short _0222AD8E - _0222AD4C - 2 ; case 7
	.short _0222AD94 - _0222AD4C - 2 ; case 8
	.short _0222AD8C - _0222AD4C - 2 ; case 9
	.short _0222AD8C - _0222AD4C - 2 ; case 10
	.short _0222AD8C - _0222AD4C - 2 ; case 11
	.short _0222AD8C - _0222AD4C - 2 ; case 12
	.short _0222AD8C - _0222AD4C - 2 ; case 13
	.short _0222AD8C - _0222AD4C - 2 ; case 14
	.short _0222AD92 - _0222AD4C - 2 ; case 15
	.short _0222ADA8 - _0222AD4C - 2 ; case 16
	.short _0222ADB4 - _0222AD4C - 2 ; case 17
	.short _0222AD9E - _0222AD4C - 2 ; case 18
	.short _0222AD9E - _0222AD4C - 2 ; case 19
	.short _0222ADA4 - _0222AD4C - 2 ; case 20
	.short _0222ADA2 - _0222AD4C - 2 ; case 21
	.short _0222AD9A - _0222AD4C - 2 ; case 22
	.short _0222AD98 - _0222AD4C - 2 ; case 23
	.short _0222AD9A - _0222AD4C - 2 ; case 24
	.short _0222AD98 - _0222AD4C - 2 ; case 25
	.short _0222AD9A - _0222AD4C - 2 ; case 26
	.short _0222AD98 - _0222AD4C - 2 ; case 27
	.short _0222ADB0 - _0222AD4C - 2 ; case 28
	.short _0222ADB0 - _0222AD4C - 2 ; case 29
_0222AD88:
	mov r5, #3
	b _0222ADB6
_0222AD8C:
	mov r4, #1
_0222AD8E:
	mov r5, #6
	b _0222ADB6
_0222AD92:
	mov r4, #1
_0222AD94:
	mov r5, #7
	b _0222ADB6
_0222AD98:
	mov r4, #1
_0222AD9A:
	mov r5, #0xa
	b _0222ADB6
_0222AD9E:
	mov r5, #5
	b _0222ADB6
_0222ADA2:
	mov r4, #1
_0222ADA4:
	mov r5, #0xb
	b _0222ADB6
_0222ADA8:
	mov r5, #4
	b _0222ADB6
_0222ADAC:
	mov r5, #8
	b _0222ADB6
_0222ADB0:
	mov r5, #5
	b _0222ADB6
_0222ADB4:
	mov r5, #5
_0222ADB6:
	str r5, [sp]
	str r4, [sp, #4]
	bl ov44_0222ACE8
	add sp, #8
	pop {r3, r4, r5, pc}
	.balign 4, 0
	thumb_func_end ov44_0222AD34

	thumb_func_start ov44_0222ADC4
ov44_0222ADC4: ; 0x0222ADC4
	cmp r0, #4
	bhs _0222ADCC
	mov r0, #8
	bx lr
_0222ADCC:
	mov r0, #0xd
	bx lr
	thumb_func_end ov44_0222ADC4

	thumb_func_start ov44_0222ADD0
ov44_0222ADD0: ; 0x0222ADD0
	push {lr}
	sub sp, #0xc
	mov r0, #6
	str r0, [sp]
	mov r0, #1
	str r0, [sp, #4]
	mov r0, #0x35
	str r0, [sp, #8]
	mov r0, #0
	add r1, r0, #0
	add r2, r0, #0
	add r3, r0, #0
	bl BeginNormalPaletteFade
	bl ov44_0222E030
	mov r0, #0x12
	add sp, #0xc
	pop {pc}
	.balign 4, 0
	thumb_func_end ov44_0222ADD0

	thumb_func_start ov44_0222ADF8
ov44_0222ADF8: ; 0x0222ADF8
	push {r4, lr}
	add r4, r0, #0
	mov r0, #0x5f
	lsl r0, r0, #2
	mov r1, #0x35
	bl String_New
	mov r1, #0x5d
	lsl r1, r1, #2
	str r0, [r4, r1]
	add r1, #8
	add r0, r1, #0
	mov r1, #0x35
	bl String_New
	mov r1, #0x5f
	lsl r1, r1, #2
	str r0, [r4, r1]
	add r0, r1, #0
	mov r1, #0x35
	bl String_New
	mov r1, #0x17
	lsl r1, r1, #4
	str r0, [r4, r1]
	mov r0, #0xb4
	mov r1, #0x35
	bl String_New
	mov r1, #0x5e
	lsl r1, r1, #2
	str r0, [r4, r1]
	mov r0, #0xd2
	mov r1, #0
	lsl r0, r0, #2
	str r1, [r4, r0]
	pop {r4, pc}
	.balign 4, 0
	thumb_func_end ov44_0222ADF8

	thumb_func_start ov44_0222AE44
ov44_0222AE44: ; 0x0222AE44
	push {r4, lr}
	add r4, r0, #0
	mov r0, #0x5e
	lsl r0, r0, #2
	ldr r0, [r4, r0]
	bl String_Delete
	mov r0, #0x5d
	lsl r0, r0, #2
	ldr r0, [r4, r0]
	bl String_Delete
	mov r0, #0x5f
	lsl r0, r0, #2
	ldr r0, [r4, r0]
	bl String_Delete
	mov r0, #0x17
	lsl r0, r0, #4
	ldr r0, [r4, r0]
	bl String_Delete
	pop {r4, pc}
	.balign 4, 0
	thumb_func_end ov44_0222AE44

	thumb_func_start ov44_0222AE74
ov44_0222AE74: ; 0x0222AE74
	push {r4, lr}
	add r4, r0, #0
	mov r1, #7
	bl FreeBgTilemapBuffer
	add r0, r4, #0
	mov r1, #6
	bl FreeBgTilemapBuffer
	add r0, r4, #0
	mov r1, #5
	bl FreeBgTilemapBuffer
	add r0, r4, #0
	mov r1, #4
	bl FreeBgTilemapBuffer
	add r0, r4, #0
	mov r1, #3
	bl FreeBgTilemapBuffer
	add r0, r4, #0
	mov r1, #2
	bl FreeBgTilemapBuffer
	add r0, r4, #0
	mov r1, #1
	bl FreeBgTilemapBuffer
	add r0, r4, #0
	mov r1, #0
	bl FreeBgTilemapBuffer
	add r0, r4, #0
	bl Heap_Free
	pop {r4, pc}
	.balign 4, 0
	thumb_func_end ov44_0222AE74

	thumb_func_start ov44_0222AEC0
ov44_0222AEC0: ; 0x0222AEC0
	push {r4, r5, r6, lr}
	sub sp, #0x10
	add r6, r0, #0
	mov r0, #0x57
	lsl r0, r0, #2
	ldr r4, [r6, r0]
	mov r0, #1
	add r5, r1, #0
	add r1, r0, #0
	bl GfGfx_EngineATogglePlanes
	mov r0, #2
	mov r1, #1
	bl GfGfx_EngineATogglePlanes
	mov r0, #4
	mov r1, #1
	bl GfGfx_EngineATogglePlanes
	mov r0, #8
	mov r1, #1
	bl GfGfx_EngineATogglePlanes
	mov r0, #1
	add r1, r0, #0
	bl GfGfx_EngineBTogglePlanes
	mov r2, #0
	str r2, [sp]
	mov r0, #0x35
	str r0, [sp, #4]
	add r0, r5, #0
	mov r1, #3
	add r3, r2, #0
	bl GfGfxLoader_GXLoadPalFromOpenNarc
	mov r3, #0
	str r3, [sp]
	mov r0, #0x35
	str r0, [sp, #4]
	add r0, r5, #0
	mov r1, #3
	mov r2, #4
	bl GfGfxLoader_GXLoadPalFromOpenNarc
	mov r1, #0x1a
	mov r0, #0
	lsl r1, r1, #4
	mov r2, #0x35
	bl LoadFontPal1
	mov r1, #6
	mov r0, #0
	lsl r1, r1, #6
	mov r2, #0x35
	bl LoadFontPal1
	mov r3, #0
	str r3, [sp]
	str r3, [sp, #4]
	str r3, [sp, #8]
	mov r0, #0x35
	str r0, [sp, #0xc]
	add r0, r5, #0
	mov r1, #2
	add r2, r4, #0
	bl GfGfxLoader_LoadCharDataFromOpenNarc
	mov r3, #0
	str r3, [sp]
	str r3, [sp, #4]
	str r3, [sp, #8]
	mov r0, #0x35
	str r0, [sp, #0xc]
	add r0, r5, #0
	mov r1, #6
	add r2, r4, #0
	bl GfGfxLoader_LoadScrnDataFromOpenNarc
	mov r0, #0
	str r0, [sp]
	str r0, [sp, #4]
	str r0, [sp, #8]
	mov r0, #0x35
	str r0, [sp, #0xc]
	add r0, r5, #0
	mov r1, #0xb
	add r2, r4, #0
	mov r3, #4
	bl GfGfxLoader_LoadCharDataFromOpenNarc
	mov r0, #0
	str r0, [sp]
	str r0, [sp, #4]
	str r0, [sp, #8]
	mov r0, #0x35
	str r0, [sp, #0xc]
	add r0, r5, #0
	mov r1, #0xc
	add r2, r4, #0
	mov r3, #4
	bl GfGfxLoader_LoadScrnDataFromOpenNarc
	mov r0, #0x16
	lsl r0, r0, #4
	ldr r0, [r6, r0]
	bl Save_PlayerData_GetOptionsAddr
	bl Options_GetFrame
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	str r0, [sp]
	mov r0, #0x35
	str r0, [sp, #4]
	ldr r2, _0222AFE0 ; =0x000001E2
	add r0, r4, #0
	mov r1, #2
	mov r3, #0xa
	bl LoadUserFrameGfx2
	mov r0, #0
	str r0, [sp]
	mov r0, #0x35
	str r0, [sp, #4]
	ldr r2, _0222AFE4 ; =0x000001D9
	add r0, r4, #0
	mov r1, #2
	mov r3, #0xb
	bl LoadUserFrameGfx1
	mov r0, #0
	str r0, [sp]
	mov r0, #0x35
	str r0, [sp, #4]
	ldr r2, _0222AFE4 ; =0x000001D9
	add r0, r4, #0
	mov r1, #1
	mov r3, #0xb
	bl LoadUserFrameGfx1
	add sp, #0x10
	pop {r4, r5, r6, pc}
	nop
_0222AFE0: .word 0x000001E2
_0222AFE4: .word 0x000001D9
	thumb_func_end ov44_0222AEC0

	thumb_func_start ov44_0222AFE8
ov44_0222AFE8: ; 0x0222AFE8
	push {r4, lr}
	sub sp, #0x10
	ldr r4, _0222B028 ; =ov44_022353C0
	add r3, sp, #0
	add r2, r3, #0
	ldmia r4!, {r0, r1}
	stmia r3!, {r0, r1}
	ldmia r4!, {r0, r1}
	stmia r3!, {r0, r1}
	add r0, r2, #0
	ldr r1, _0222B02C ; =0x00200010
	mov r2, #0x10
	bl ObjCharTransfer_InitEx
	mov r0, #0x14
	mov r1, #0x35
	bl ObjPlttTransfer_Init
	bl ObjCharTransfer_ClearBuffers
	bl ObjPlttTransfer_Reset
	ldr r1, _0222B02C ; =0x00200010
	mov r0, #1
	bl G2dRenderer_SetObjCharTransferReservedRegion
	mov r0, #1
	bl G2dRenderer_SetPlttTransferReservedRegion
	add sp, #0x10
	pop {r4, pc}
	nop
_0222B028: .word ov44_022353C0
_0222B02C: .word 0x00200010
	thumb_func_end ov44_0222AFE8

	thumb_func_start ov44_0222B030
ov44_0222B030: ; 0x0222B030
	push {r4, r5, r6, r7, lr}
	sub sp, #0x14
	add r6, r0, #0
	bl NNS_G2dInitOamManagerModule
	mov r0, #0
	str r0, [sp]
	mov r1, #0x7e
	str r1, [sp, #4]
	str r0, [sp, #8]
	mov r3, #0x20
	str r3, [sp, #0xc]
	mov r2, #0x35
	str r2, [sp, #0x10]
	add r2, r0, #0
	bl OamManager_Create
	mov r1, #0x65
	lsl r1, r1, #2
	mov r0, #8
	add r1, r6, r1
	mov r2, #0x35
	bl G2dRenderer_Init
	mov r1, #0x19
	lsl r1, r1, #4
	str r0, [r6, r1]
	add r0, r1, #4
	mov r2, #1
	add r0, r6, r0
	mov r1, #0
	lsl r2, r2, #0x14
	bl G2dRenderer_SetSubSurfaceCoords
	mov r7, #0xaf
	mov r4, #0
	add r5, r6, #0
	lsl r7, r7, #2
_0222B07C:
	mov r0, #1
	add r1, r4, #0
	mov r2, #0x35
	bl Create2DGfxResObjMan
	str r0, [r5, r7]
	add r4, r4, #1
	add r5, r5, #4
	cmp r4, #4
	blt _0222B07C
	mov r0, #1
	mov r1, #0x35
	bl FontSystem_NewInit
	mov r1, #0xb3
	lsl r1, r1, #2
	str r0, [r6, r1]
	add sp, #0x14
	pop {r4, r5, r6, r7, pc}
	.balign 4, 0
	thumb_func_end ov44_0222B030

	thumb_func_start ov44_0222B0A4
ov44_0222B0A4: ; 0x0222B0A4
	ldr r3, _0222B0AC ; =GfGfx_EngineATogglePlanes
	mov r0, #0x10
	mov r1, #1
	bx r3
	.balign 4, 0
_0222B0AC: .word GfGfx_EngineATogglePlanes
	thumb_func_end ov44_0222B0A4

	thumb_func_start ov44_0222B0B0
ov44_0222B0B0: ; 0x0222B0B0
	push {r3, r4, lr}
	sub sp, #0x14
	add r4, r0, #0
	mov r0, #0x2f
	lsl r0, r0, #4
	add r0, r4, r0
	bl WindowIsInUse
	cmp r0, #0
	beq _0222B0DA
	mov r0, #0x2f
	lsl r0, r0, #4
	add r0, r4, r0
	mov r1, #0
	bl sub_0200E5D4
	mov r0, #0x2f
	lsl r0, r0, #4
	add r0, r4, r0
	bl RemoveWindow
_0222B0DA:
	mov r0, #1
	str r0, [sp]
	mov r0, #0x16
	str r0, [sp, #4]
	mov r0, #2
	str r0, [sp, #8]
	mov r0, #0xd
	str r0, [sp, #0xc]
	ldr r0, _0222B15C ; =0x000001CA
	mov r1, #0x2f
	str r0, [sp, #0x10]
	sub r0, #0x6e
	lsl r1, r1, #4
	ldr r0, [r4, r0]
	add r1, r4, r1
	mov r2, #3
	mov r3, #5
	bl AddWindowParameterized
	mov r0, #0x2f
	lsl r0, r0, #4
	add r0, r4, r0
	mov r1, #0
	bl FillWindowPixelBuffer
	mov r2, #0x5a
	lsl r2, r2, #2
	ldr r0, [r4, r2]
	add r2, #0x10
	ldr r2, [r4, r2]
	mov r1, #0x15
	bl ReadMsgDataIntoString
	mov r1, #0x5e
	lsl r1, r1, #2
	mov r0, #0
	ldr r1, [r4, r1]
	add r2, r0, #0
	mov r3, #0xb0
	bl FontID_String_GetCenterAlignmentX
	mov r1, #0
	add r3, r0, #0
	mov r2, #0x2f
	str r1, [sp]
	mov r0, #0xff
	str r0, [sp, #4]
	ldr r0, _0222B160 ; =0x000F0E00
	lsl r2, r2, #4
	str r0, [sp, #8]
	str r1, [sp, #0xc]
	add r0, r4, r2
	lsr r2, r2, #1
	ldr r2, [r4, r2]
	mov r1, #1
	bl AddTextPrinterParameterizedWithColor
	mov r0, #0x2f
	lsl r0, r0, #4
	add r0, r4, r0
	bl ScheduleWindowCopyToVram
	add sp, #0x14
	pop {r3, r4, pc}
	nop
_0222B15C: .word 0x000001CA
_0222B160: .word 0x000F0E00
	thumb_func_end ov44_0222B0B0

	thumb_func_start ov44_0222B164
ov44_0222B164: ; 0x0222B164
	push {r4, lr}
	add r4, r0, #0
	bl ov44_0222F7BC
	mov r0, #0x2f
	lsl r0, r0, #4
	add r0, r4, r0
	bl WindowIsInUse
	cmp r0, #0
	beq _0222B184
	mov r0, #0x2f
	lsl r0, r0, #4
	add r0, r4, r0
	bl RemoveWindow
_0222B184:
	mov r0, #0x2d
	lsl r0, r0, #4
	add r0, r4, r0
	bl WindowIsInUse
	cmp r0, #0
	beq _0222B19C
	mov r0, #0x2d
	lsl r0, r0, #4
	add r0, r4, r0
	bl RemoveWindow
_0222B19C:
	mov r0, #0x33
	lsl r0, r0, #4
	add r0, r4, r0
	bl WindowIsInUse
	cmp r0, #0
	beq _0222B1B4
	mov r0, #0x33
	lsl r0, r0, #4
	add r0, r4, r0
	bl RemoveWindow
_0222B1B4:
	mov r0, #0x2e
	lsl r0, r0, #4
	add r0, r4, r0
	bl WindowIsInUse
	cmp r0, #0
	beq _0222B1CC
	mov r0, #0x2e
	lsl r0, r0, #4
	add r0, r4, r0
	bl RemoveWindow
_0222B1CC:
	mov r0, #0x31
	lsl r0, r0, #4
	add r0, r4, r0
	bl WindowIsInUse
	cmp r0, #0
	beq _0222B1E4
	mov r0, #0x31
	lsl r0, r0, #4
	add r0, r4, r0
	bl RemoveWindow
_0222B1E4:
	mov r0, #0x15
	lsl r0, r0, #4
	ldr r0, [r4, r0]
	cmp r0, #0
	beq _0222B1F6
	mov r1, #0
	add r2, r1, #0
	bl DestroyListMenu
_0222B1F6:
	mov r0, #0x53
	lsl r0, r0, #2
	ldr r0, [r4, r0]
	cmp r0, #0
	beq _0222B20C
	bl ListMenuItems_Delete
	mov r0, #0x53
	mov r1, #0
	lsl r0, r0, #2
	str r1, [r4, r0]
_0222B20C:
	mov r0, #3
	lsl r0, r0, #8
	add r0, r4, r0
	bl WindowIsInUse
	cmp r0, #0
	beq _0222B224
	mov r0, #3
	lsl r0, r0, #8
	add r0, r4, r0
	bl RemoveWindow
_0222B224:
	pop {r4, pc}
	.balign 4, 0
	thumb_func_end ov44_0222B164

	thumb_func_start ov44_0222B228
ov44_0222B228: ; 0x0222B228
	push {r3, r4, r5, lr}
	add r5, r0, #0
	add r4, r1, #0
	bl sub_02039998
	cmp r0, #0
	beq _0222B2CA
	bl ov00_021E6EBC
	add r1, r0, #0
	add r0, r5, #0
	bl ov44_0222F780
	bl sub_020398C8
	str r0, [r5, #4]
	ldr r1, [r5, #4]
	add r0, r5, #0
	bl ov44_02229F00
	cmp r0, #8
	beq _0222B278
	cmp r0, #0x12
	beq _0222B278
	cmp r0, #0x14
	beq _0222B278
	cmp r0, #0x16
	beq _0222B278
	cmp r0, #0x17
	beq _0222B278
	cmp r0, #0x18
	beq _0222B278
	cmp r0, #0x19
	beq _0222B278
	cmp r0, #0x1a
	beq _0222B278
	cmp r0, #0x1b
	beq _0222B278
	cmp r0, #0x13
	bne _0222B2A8
_0222B278:
	add r0, r5, #0
	mov r1, #0x10
	bl ov44_0222F818
	bl sub_020343E4
	bl sub_02039378
	bl sub_020398C8
	add r1, r0, #0
	str r0, [r5, #4]
	add r1, #0x22
	ldrb r1, [r1]
	add r0, #0x21
	strb r1, [r0]
	mov r0, #0xd7
	mov r1, #0x14
	lsl r0, r0, #2
	str r1, [r5, r0]
	mov r1, #0x41
	sub r0, #0x14
	str r1, [r5, r0]
	b _0222B360
_0222B2A8:
	add r0, r5, #0
	mov r1, #0x4d
	mov r2, #0
	bl ov44_0222F510
	mov r0, #1
	add r1, r0, #0
	bl sub_020398D4
	mov r0, #0x10
	bl sub_02037AC0
	mov r0, #0xd2
	mov r1, #0x3d
	lsl r0, r0, #2
	str r1, [r5, r0]
	b _0222B360
_0222B2CA:
	ldr r1, _0222B364 ; =0x0000047D
	mov r0, #0xb
	mov r2, #1
	bl Sound_SetSceneAndPlayBGM
	add r0, r5, #0
	bl ov44_0222B0B0
	ldr r0, [r5]
	bl sub_0202C08C
	bl DWC_CheckHasProfile
	cmp r0, #0
	bne _0222B32E
	mov r0, #0
	mov r1, #3
	bl SetBgPriority
	mov r0, #1
	mov r1, #0
	bl SetBgPriority
	mov r0, #3
	mov r1, #0
	bl SetBgPriority
	mov r0, #2
	mov r1, #1
	bl SetBgPriority
	add r0, r5, #0
	mov r1, #0x12
	bl ov44_0222F66C
	mov r0, #0x57
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	ldr r1, _0222B368 ; =ov44_02235384
	mov r2, #0
	bl ov44_02231A28
	mov r1, #0x62
	lsl r1, r1, #2
	str r0, [r5, r1]
	mov r0, #0xd2
	mov r1, #6
	lsl r0, r0, #2
	str r1, [r5, r0]
	b _0222B360
_0222B32E:
	ldr r0, [r5]
	bl sub_0202C08C
	bl DWC_CheckValidConsole
	cmp r0, #0
	bne _0222B34E
	add r0, r5, #0
	mov r1, #0x14
	bl ov44_0222F66C
	mov r0, #0xd2
	mov r1, #3
	lsl r0, r0, #2
	str r1, [r5, r0]
	b _0222B360
_0222B34E:
	add r0, r5, #0
	mov r1, #0x11
	mov r2, #1
	bl ov44_0222F510
	mov r0, #0xd2
	mov r1, #1
	lsl r0, r0, #2
	str r1, [r5, r0]
_0222B360:
	add r0, r4, #0
	pop {r3, r4, r5, pc}
	.balign 4, 0
_0222B364: .word 0x0000047D
_0222B368: .word ov44_02235384
	thumb_func_end ov44_0222B228

	thumb_func_start ov44_0222B36C
ov44_0222B36C: ; 0x0222B36C
	push {r3, r4, r5, lr}
	add r5, r0, #0
	mov r0, #6
	lsl r0, r0, #6
	ldr r0, [r5, r0]
	add r4, r1, #0
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	bl TextPrinterCheckActive
	cmp r0, #0
	bne _0222B3A0
	mov r0, #0x57
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	ldr r1, _0222B3A4 ; =ov44_0223537C
	mov r2, #0
	bl ov44_02231A28
	mov r1, #0x62
	lsl r1, r1, #2
	str r0, [r5, r1]
	mov r0, #0xd2
	mov r1, #2
	lsl r0, r0, #2
	str r1, [r5, r0]
_0222B3A0:
	add r0, r4, #0
	pop {r3, r4, r5, pc}
	.balign 4, 0
_0222B3A4: .word ov44_0223537C
	thumb_func_end ov44_0222B36C

	thumb_func_start ov44_0222B3A8
ov44_0222B3A8: ; 0x0222B3A8
	push {r3, r4, r5, lr}
	add r5, r0, #0
	mov r0, #0x62
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	add r4, r1, #0
	bl YesNoPrompt_HandleInput
	cmp r0, #1
	bne _0222B408
	mov r0, #0x62
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	bl YesNoPrompt_Destroy
	mov r0, #0x16
	lsl r0, r0, #4
	ldr r0, [r5, r0]
	ldr r1, _0222B428 ; =0x000004A4
	bl sub_020396FC
	str r0, [r5, #4]
	add r0, r5, #0
	mov r1, #0x17
	mov r2, #1
	bl ov44_0222F510
	mov r0, #0x63
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	cmp r0, #0
	beq _0222B3EC
	bl GF_AssertFail
_0222B3EC:
	mov r1, #0x2d
	lsl r1, r1, #4
	add r0, r5, r1
	sub r1, #0xee
	bl WaitingIcon_New
	mov r1, #0x63
	lsl r1, r1, #2
	str r0, [r5, r1]
	mov r0, #0xd2
	mov r1, #0xe
	lsl r0, r0, #2
	str r1, [r5, r0]
	b _0222B424
_0222B408:
	cmp r0, #2
	bne _0222B420
	mov r0, #0x62
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	bl YesNoPrompt_Destroy
	mov r0, #0xd2
	mov r1, #0x22
	lsl r0, r0, #2
	str r1, [r5, r0]
	b _0222B424
_0222B420:
	add r0, r4, #0
	pop {r3, r4, r5, pc}
_0222B424:
	add r0, r4, #0
	pop {r3, r4, r5, pc}
	.balign 4, 0
_0222B428: .word 0x000004A4
	thumb_func_end ov44_0222B3A8

	thumb_func_start ov44_0222B42C
ov44_0222B42C: ; 0x0222B42C
	push {r3, r4, r5, lr}
	add r5, r0, #0
	ldr r0, _0222B48C ; =gSystem
	add r4, r1, #0
	ldr r1, [r0, #0x48]
	mov r0, #3
	tst r0, r1
	bne _0222B444
	bl System_GetTouchNew
	cmp r0, #1
	bne _0222B488
_0222B444:
	add r0, r5, #0
	mov r1, #0x15
	bl ov44_0222F66C
	mov r0, #3
	mov r1, #0
	bl SetBgPriority
	mov r0, #2
	mov r1, #1
	bl SetBgPriority
	mov r0, #1
	mov r1, #0
	bl SetBgPriority
	mov r0, #0
	mov r1, #3
	bl SetBgPriority
	mov r0, #0x57
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	ldr r1, _0222B490 ; =ov44_02235384
	mov r2, #1
	bl ov44_02231A28
	mov r1, #0x62
	lsl r1, r1, #2
	str r0, [r5, r1]
	mov r0, #0xd2
	mov r1, #4
	lsl r0, r0, #2
	str r1, [r5, r0]
_0222B488:
	add r0, r4, #0
	pop {r3, r4, r5, pc}
	.balign 4, 0
_0222B48C: .word gSystem
_0222B490: .word ov44_02235384
	thumb_func_end ov44_0222B42C

	thumb_func_start ov44_0222B494
ov44_0222B494: ; 0x0222B494
	push {r3, r4, r5, lr}
	add r5, r0, #0
	mov r0, #0x62
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	add r4, r1, #0
	bl YesNoPrompt_HandleInput
	cmp r0, #1
	bne _0222B4EE
	mov r0, #0x62
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	bl YesNoPrompt_Destroy
	mov r0, #3
	lsl r0, r0, #8
	add r0, r5, r0
	mov r1, #0
	bl ClearFrameAndWindow2
	mov r0, #3
	lsl r0, r0, #8
	add r0, r5, r0
	bl RemoveWindow
	add r0, r5, #0
	mov r1, #0x16
	bl ov44_0222F66C
	mov r0, #0x57
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	ldr r1, _0222B524 ; =ov44_02235384
	mov r2, #1
	bl ov44_02231A28
	mov r1, #0x62
	lsl r1, r1, #2
	str r0, [r5, r1]
	mov r0, #0xd2
	mov r1, #5
	lsl r0, r0, #2
	str r1, [r5, r0]
	b _0222B520
_0222B4EE:
	cmp r0, #2
	bne _0222B51C
	mov r0, #0x62
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	bl YesNoPrompt_Destroy
	mov r0, #3
	lsl r0, r0, #8
	add r0, r5, r0
	mov r1, #0
	bl ClearFrameAndWindow2
	mov r0, #3
	lsl r0, r0, #8
	add r0, r5, r0
	bl RemoveWindow
	mov r0, #0xd2
	mov r1, #0x22
	lsl r0, r0, #2
	str r1, [r5, r0]
	b _0222B520
_0222B51C:
	add r0, r4, #0
	pop {r3, r4, r5, pc}
_0222B520:
	add r0, r4, #0
	pop {r3, r4, r5, pc}
	.balign 4, 0
_0222B524: .word ov44_02235384
	thumb_func_end ov44_0222B494

	thumb_func_start ov44_0222B528
ov44_0222B528: ; 0x0222B528
	push {r3, r4, r5, lr}
	add r5, r0, #0
	mov r0, #0x62
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	add r4, r1, #0
	bl YesNoPrompt_HandleInput
	cmp r0, #1
	bne _0222B5EA
	mov r0, #0x62
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	bl YesNoPrompt_Destroy
	mov r0, #3
	mov r1, #0
	bl SetBgPriority
	mov r0, #2
	mov r1, #0
	bl SetBgPriority
	mov r0, #1
	add r1, r0, #0
	bl SetBgPriority
	mov r0, #0
	mov r1, #3
	bl SetBgPriority
	mov r0, #3
	lsl r0, r0, #8
	add r0, r5, r0
	mov r1, #0
	bl ClearFrameAndWindow2
	mov r0, #3
	lsl r0, r0, #8
	add r0, r5, r0
	bl RemoveWindow
	ldr r0, [r5]
	bl sub_0202C03C
	mov r0, #0x16
	lsl r0, r0, #4
	ldr r0, [r5, r0]
	mov r1, #0x1f
	bl SaveArray_Get
	bl sub_02031710
	mov r0, #0x16
	lsl r0, r0, #4
	ldr r0, [r5, r0]
	bl Save_Frontier_GetStatic
	bl sub_02031188
	mov r0, #0xd2
	mov r1, #0xe
	lsl r0, r0, #2
	str r1, [r5, r0]
	mov r0, #0x16
	lsl r0, r0, #4
	ldr r0, [r5, r0]
	ldr r1, _0222B648 ; =0x000004A4
	bl sub_020396FC
	str r0, [r5, #4]
	add r0, r5, #0
	mov r1, #0x17
	mov r2, #1
	bl ov44_0222F510
	mov r0, #0x63
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	cmp r0, #0
	beq _0222B5CE
	bl GF_AssertFail
_0222B5CE:
	mov r1, #0x2d
	lsl r1, r1, #4
	add r0, r5, r1
	sub r1, #0xee
	bl WaitingIcon_New
	mov r1, #0x63
	lsl r1, r1, #2
	str r0, [r5, r1]
	mov r0, #0xda
	mov r1, #1
	lsl r0, r0, #2
	str r1, [r5, r0]
	b _0222B644
_0222B5EA:
	cmp r0, #2
	bne _0222B640
	mov r0, #0x62
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	bl YesNoPrompt_Destroy
	mov r0, #3
	mov r1, #0
	bl SetBgPriority
	mov r0, #2
	mov r1, #0
	bl SetBgPriority
	mov r0, #1
	add r1, r0, #0
	bl SetBgPriority
	mov r0, #0
	mov r1, #3
	bl SetBgPriority
	mov r0, #3
	lsl r0, r0, #8
	add r0, r5, r0
	mov r1, #0
	bl ClearFrameAndWindow2
	mov r0, #3
	lsl r0, r0, #8
	add r0, r5, r0
	bl RemoveWindow
	add r0, r5, #0
	mov r1, #0x14
	bl ov44_0222F66C
	mov r0, #0xd2
	mov r1, #3
	lsl r0, r0, #2
	str r1, [r5, r0]
	b _0222B644
_0222B640:
	add r0, r4, #0
	pop {r3, r4, r5, pc}
_0222B644:
	add r0, r4, #0
	pop {r3, r4, r5, pc}
	.balign 4, 0
_0222B648: .word 0x000004A4
	thumb_func_end ov44_0222B528

	thumb_func_start ov44_0222B64C
ov44_0222B64C: ; 0x0222B64C
	push {r3, r4, r5, lr}
	add r5, r0, #0
	mov r0, #0x62
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	add r4, r1, #0
	bl YesNoPrompt_HandleInput
	cmp r0, #1
	bne _0222B6EA
	mov r0, #0x62
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	bl YesNoPrompt_Destroy
	mov r0, #0
	mov r1, #3
	bl SetBgPriority
	mov r0, #1
	add r1, r0, #0
	bl SetBgPriority
	mov r0, #3
	mov r1, #0
	bl SetBgPriority
	mov r0, #2
	mov r1, #0
	bl SetBgPriority
	mov r0, #3
	lsl r0, r0, #8
	add r0, r5, r0
	mov r1, #0
	bl ClearFrameAndWindow2
	mov r0, #3
	lsl r0, r0, #8
	add r0, r5, r0
	bl RemoveWindow
	mov r0, #0xd2
	mov r1, #0xe
	lsl r0, r0, #2
	str r1, [r5, r0]
	mov r0, #0x16
	lsl r0, r0, #4
	ldr r0, [r5, r0]
	ldr r1, _0222B740 ; =0x000004A4
	bl sub_020396FC
	str r0, [r5, #4]
	add r0, r5, #0
	mov r1, #0x17
	mov r2, #1
	bl ov44_0222F510
	mov r0, #0x63
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	cmp r0, #0
	beq _0222B6CE
	bl GF_AssertFail
_0222B6CE:
	mov r1, #0x2d
	lsl r1, r1, #4
	add r0, r5, r1
	sub r1, #0xee
	bl WaitingIcon_New
	mov r1, #0x63
	lsl r1, r1, #2
	str r0, [r5, r1]
	mov r0, #0xda
	mov r1, #1
	lsl r0, r0, #2
	str r1, [r5, r0]
	b _0222B73C
_0222B6EA:
	cmp r0, #2
	bne _0222B738
	mov r0, #0x62
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	bl YesNoPrompt_Destroy
	mov r0, #0
	mov r1, #3
	bl SetBgPriority
	mov r0, #1
	add r1, r0, #0
	bl SetBgPriority
	mov r0, #3
	mov r1, #0
	bl SetBgPriority
	mov r0, #2
	mov r1, #0
	bl SetBgPriority
	mov r0, #3
	lsl r0, r0, #8
	add r0, r5, r0
	mov r1, #0
	bl ClearFrameAndWindow2
	mov r0, #3
	lsl r0, r0, #8
	add r0, r5, r0
	bl RemoveWindow
	mov r0, #0xd2
	mov r1, #0x22
	lsl r0, r0, #2
	str r1, [r5, r0]
	b _0222B73C
_0222B738:
	add r0, r4, #0
	pop {r3, r4, r5, pc}
_0222B73C:
	add r0, r4, #0
	pop {r3, r4, r5, pc}
	.balign 4, 0
_0222B740: .word 0x000004A4
	thumb_func_end ov44_0222B64C

	thumb_func_start ov44_0222B744
ov44_0222B744: ; 0x0222B744
	push {r3, r4, r5, lr}
	add r5, r0, #0
	add r4, r1, #0
	mov r0, #0
	mov r1, #3
	bl SetBgPriority
	mov r0, #1
	add r1, r0, #0
	bl SetBgPriority
	mov r0, #3
	mov r1, #0
	bl SetBgPriority
	mov r0, #2
	mov r1, #0
	bl SetBgPriority
	mov r0, #0xd2
	mov r1, #0xf
	lsl r0, r0, #2
	str r1, [r5, r0]
	add r0, r4, #0
	pop {r3, r4, r5, pc}
	.balign 4, 0
	thumb_func_end ov44_0222B744

	thumb_func_start ov44_0222B778
ov44_0222B778: ; 0x0222B778
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x18
	add r5, r0, #0
	mov r0, #0x16
	lsl r0, r0, #4
	ldr r0, [r5, r0]
	str r1, [sp]
	bl Save_PlayerData_GetProfile
	str r0, [sp, #0x14]
	mov r0, #0x16
	lsl r0, r0, #4
	ldr r0, [r5, r0]
	bl Save_Pokedex_Get
	str r0, [sp, #0x10]
	mov r0, #0x16
	lsl r0, r0, #4
	ldr r0, [r5, r0]
	bl SaveArray_Party_Get
	str r0, [sp, #0xc]
	mov r0, #0x16
	lsl r0, r0, #4
	ldr r0, [r5, r0]
	bl Save_WiFiHistory_Get
	str r0, [sp, #8]
	ldr r0, [sp, #0xc]
	bl Party_GetCount
	mov r6, #0
	str r0, [sp, #4]
	cmp r0, #0
	ble _0222B7F0
	add r4, r6, #0
_0222B7C0:
	ldr r0, [sp, #0xc]
	add r1, r6, #0
	bl Party_GetMonByIndex
	mov r1, #5
	mov r2, #0
	add r7, r0, #0
	bl GetMonData
	ldr r1, [r5, #4]
	mov r2, #0
	strh r0, [r1, r4]
	add r0, r7, #0
	mov r1, #6
	bl GetMonData
	ldr r1, [r5, #4]
	add r6, r6, #1
	add r1, r1, r4
	strh r0, [r1, #0xc]
	ldr r0, [sp, #4]
	add r4, r4, #2
	cmp r6, r0
	blt _0222B7C0
_0222B7F0:
	ldr r0, [sp, #0x14]
	bl PlayerProfile_GetVersion
	ldr r1, [r5, #4]
	strb r0, [r1, #0x18]
	ldr r0, [sp, #0x14]
	bl PlayerProfile_GetLanguage
	ldr r1, [r5, #4]
	strb r0, [r1, #0x19]
	ldr r0, [sp, #0x10]
	bl Pokedex_GetNatDexFlag
	ldr r1, [r5, #4]
	strb r0, [r1, #0x1a]
	ldr r0, [r5, #4]
	mov r1, #0x1d
	strb r1, [r0, #0x1b]
	ldr r1, [sp]
	add r0, r5, #0
	bl ov44_0222F830
	ldr r0, [r5, #4]
	mov r1, #0
	strb r1, [r0, #0x1c]
	ldr r0, [sp, #0x14]
	bl PlayerProfile_GetAvatar
	ldr r1, [r5, #4]
	strb r0, [r1, #0x1d]
	ldr r0, [sp, #0x14]
	bl PlayerProfile_GetTrainerGender
	ldr r1, [r5, #4]
	strb r0, [r1, #0x1e]
	ldr r0, [sp, #8]
	bl WifiHistory_GetPlayerCountry
	ldr r1, [r5, #4]
	strb r0, [r1, #0x1f]
	ldr r0, [sp, #8]
	bl WiFiHistory_GetPlayerRegion
	ldr r1, [r5, #4]
	add r1, #0x20
	strb r0, [r1]
	ldr r0, [r5, #4]
	mov r1, #1
	add r0, #0x21
	strb r1, [r0]
	ldr r0, [r5, #4]
	add r0, #0x22
	strb r1, [r0]
	ldr r0, [r5, #4]
	mov r1, #0x24
	bl ov00_021E6D60
	ldr r0, [r5, #4]
	mov r1, #0x24
	add r0, #0x24
	bl ov00_021E6CCC
	add sp, #0x18
	pop {r3, r4, r5, r6, r7, pc}
	thumb_func_end ov44_0222B778

	thumb_func_start ov44_0222B870
ov44_0222B870: ; 0x0222B870
	push {r3, r4, r5, r6, r7, lr}
	mov r6, #0
	add r5, r0, #0
	add r4, r6, #0
_0222B878:
	add r1, r5, r4
	mov r0, #0
	strb r0, [r1, #0xc]
	ldr r0, _0222B8DC ; =0x000010BE
	ldrh r0, [r5, r0]
	cmp r0, r4
	ble _0222B8D2
	add r0, r4, #0
	bl ov44_02231974
	cmp r0, #0
	beq _0222B8D2
	add r0, r5, #0
	add r1, r4, #0
	bl ov44_02231958
	add r7, r0, #0
	add r0, r5, #0
	add r1, r7, #0
	bl ov44_02229F00
	cmp r0, #0
	beq _0222B8BA
	cmp r0, #0x1c
	beq _0222B8BA
	ldrb r1, [r7, #0x1d]
	cmp r1, #0
	beq _0222B8BA
	add r0, r5, r6
	add r1, r4, #1
	strb r1, [r0, #0xc]
	add r6, r6, #1
	b _0222B8D2
_0222B8BA:
	cmp r0, #0x1c
	bne _0222B8D2
	add r0, r5, #0
	add r1, r4, #1
	bl ov44_02230048
	cmp r0, #0
	beq _0222B8D2
	add r0, r5, #0
	add r1, r4, #1
	bl ov44_02230028
_0222B8D2:
	add r4, r4, #1
	cmp r4, #0x20
	blt _0222B878
	add r0, r6, #0
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
_0222B8DC: .word 0x000010BE
	thumb_func_end ov44_0222B870

	thumb_func_start ov44_0222B8E0
ov44_0222B8E0: ; 0x0222B8E0
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x10
	add r5, r0, #0
	mov r4, #0
	str r4, [sp, #0xc]
	add r6, r5, #0
_0222B8EC:
	add r0, r5, #0
	add r1, r4, #0
	bl ov44_02231958
	add r7, r0, #0
	add r0, r5, #0
	add r1, r7, #0
	bl ov44_02229F00
	ldr r1, [r6, #0x4c]
	str r0, [sp]
	cmp r0, r1
	bne _0222B916
	add r0, r7, #0
	add r0, #0x21
	ldrb r1, [r0]
	add r0, r6, #0
	add r0, #0xcc
	ldr r0, [r0]
	cmp r1, r0
	beq _0222B992
_0222B916:
	add r0, r5, #0
	add r1, r4, #1
	bl ov44_0222AAEC
	str r0, [sp, #4]
	cmp r0, #0
	beq _0222B992
	ldr r0, [r6, #0x4c]
	bl ov44_02229FDC
	str r0, [sp, #8]
	ldr r0, [sp]
	bl ov44_02229FDC
	ldr r1, [sp, #8]
	cmp r1, #1
	bne _0222B94C
	cmp r0, #0
	bne _0222B94C
	mov r0, #0xe3
	lsl r0, r0, #2
	ldr r1, [sp, #4]
	add r0, r5, r0
	mov r2, #0
	bl ov44_02231FA8
	b _0222B964
_0222B94C:
	ldr r1, [sp, #8]
	cmp r1, #0
	bne _0222B964
	cmp r0, #1
	bne _0222B964
	mov r0, #0xe3
	lsl r0, r0, #2
	ldr r1, [sp, #4]
	add r0, r5, r0
	mov r2, #1
	bl ov44_02231FA8
_0222B964:
	ldr r0, [sp]
	mov r2, #7
	str r0, [r6, #0x4c]
	add r0, r7, #0
	add r0, #0x21
	ldrb r1, [r0]
	add r0, r6, #0
	add r0, #0xcc
	str r1, [r0]
	ldrb r3, [r7, #0x1d]
	ldr r0, [r5]
	add r1, r4, #0
	bl sub_0202C190
	ldrb r3, [r7, #0x1e]
	ldr r0, [r5]
	add r1, r4, #0
	mov r2, #8
	bl sub_0202C190
	ldr r0, [sp, #0xc]
	add r0, r0, #1
	str r0, [sp, #0xc]
_0222B992:
	add r4, r4, #1
	add r6, r6, #4
	cmp r4, #0x20
	blt _0222B8EC
	ldr r0, [sp, #0xc]
	add sp, #0x10
	pop {r3, r4, r5, r6, r7, pc}
	thumb_func_end ov44_0222B8E0

	thumb_func_start ov44_0222B9A0
ov44_0222B9A0: ; 0x0222B9A0
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #8
	add r4, r0, #0
	bl sub_020392D8
	add r7, r0, #0
	ldr r6, [r7]
	ldr r1, [r7, #4]
	add r0, r6, #0
	bl ov00_021E6A70
	add r5, r0, #0
	cmp r5, #0xb
	beq _0222B9C0
	cmp r6, #0x19
	bne _0222B9D0
_0222B9C0:
	ldr r0, [r7, #4]
	cmp r0, #2
	beq _0222B9CA
	mov r7, #0xe
	b _0222B9CC
_0222B9CA:
	mov r7, #0xb
_0222B9CC:
	mov r5, #0xb
	b _0222B9E4
_0222B9D0:
	cmp r6, #0x1a
	bne _0222B9DA
	mov r7, #0xd
	mov r5, #0xc
	b _0222B9E4
_0222B9DA:
	cmp r5, #0
	blt _0222B9E2
	add r7, r5, #0
	b _0222B9E4
_0222B9E2:
	mov r7, #0xb
_0222B9E4:
	add r0, r4, #0
	bl ov44_0222F7BC
	mov r0, #2
	str r0, [sp]
	mov r0, #1
	str r0, [sp, #4]
	mov r0, #0x59
	lsl r0, r0, #2
	ldr r0, [r4, r0]
	mov r1, #0
	add r2, r6, #0
	mov r3, #5
	bl BufferIntegerAsString
	mov r0, #0xd7
	mov r1, #0x1e
	lsl r0, r0, #2
	str r1, [r4, r0]
	add r0, r4, #0
	add r1, r7, #0
	bl ov44_0222F66C
	cmp r5, #0xb
	bhi _0222BA5E
	add r0, r5, r5
	add r0, pc
	ldrh r0, [r0, #6]
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	add pc, r0
_0222BA22: ; jump table
	.short _0222BA5E - _0222BA22 - 2 ; case 0
	.short _0222BA3A - _0222BA22 - 2 ; case 1
	.short _0222BA5E - _0222BA22 - 2 ; case 2
	.short _0222BA5E - _0222BA22 - 2 ; case 3
	.short _0222BA3A - _0222BA22 - 2 ; case 4
	.short _0222BA3A - _0222BA22 - 2 ; case 5
	.short _0222BA46 - _0222BA22 - 2 ; case 6
	.short _0222BA46 - _0222BA22 - 2 ; case 7
	.short _0222BA46 - _0222BA22 - 2 ; case 8
	.short _0222BA46 - _0222BA22 - 2 ; case 9
	.short _0222BA52 - _0222BA22 - 2 ; case 10
	.short _0222BA3A - _0222BA22 - 2 ; case 11
_0222BA3A:
	mov r0, #0xd2
	mov r1, #0xa
	lsl r0, r0, #2
	add sp, #8
	str r1, [r4, r0]
	pop {r3, r4, r5, r6, r7, pc}
_0222BA46:
	mov r0, #0xd2
	mov r1, #0xa
	lsl r0, r0, #2
	add sp, #8
	str r1, [r4, r0]
	pop {r3, r4, r5, r6, r7, pc}
_0222BA52:
	mov r0, #0xd2
	mov r1, #0xa
	lsl r0, r0, #2
	add sp, #8
	str r1, [r4, r0]
	pop {r3, r4, r5, r6, r7, pc}
_0222BA5E:
	mov r0, #0xd2
	mov r1, #0x23
	lsl r0, r0, #2
	str r1, [r4, r0]
	add sp, #8
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
	thumb_func_end ov44_0222B9A0

	thumb_func_start ov44_0222BA6C
ov44_0222BA6C: ; 0x0222BA6C
	push {r3, r4, r5, lr}
	add r5, r0, #0
	ldr r0, _0222BAB4 ; =gSystem
	add r4, r1, #0
	ldr r1, [r0, #0x48]
	mov r0, #3
	tst r0, r1
	bne _0222BA84
	bl System_GetTouchNew
	cmp r0, #1
	bne _0222BAB0
_0222BA84:
	mov r0, #3
	lsl r0, r0, #8
	add r0, r5, r0
	bl WindowIsInUse
	cmp r0, #0
	beq _0222BAA8
	mov r0, #3
	lsl r0, r0, #8
	add r0, r5, r0
	mov r1, #0
	bl ClearFrameAndWindow2
	mov r0, #3
	lsl r0, r0, #8
	add r0, r5, r0
	bl RemoveWindow
_0222BAA8:
	mov r0, #0xd2
	mov r1, #8
	lsl r0, r0, #2
	str r1, [r5, r0]
_0222BAB0:
	add r0, r4, #0
	pop {r3, r4, r5, pc}
	.balign 4, 0
_0222BAB4: .word gSystem
	thumb_func_end ov44_0222BA6C

	thumb_func_start ov44_0222BAB8
ov44_0222BAB8: ; 0x0222BAB8
	mov r2, #0xd2
	mov r3, #9
	lsl r2, r2, #2
	str r3, [r0, r2]
	add r0, r1, #0
	bx lr
	thumb_func_end ov44_0222BAB8

	thumb_func_start ov44_0222BAC4
ov44_0222BAC4: ; 0x0222BAC4
	mov r2, #0xd2
	mov r3, #0xb
	lsl r2, r2, #2
	str r3, [r0, r2]
	add r0, r1, #0
	bx lr
	thumb_func_end ov44_0222BAC4

	thumb_func_start ov44_0222BAD0
ov44_0222BAD0: ; 0x0222BAD0
	push {r3, r4, r5, lr}
	add r5, r0, #0
	mov r0, #0xd7
	lsl r0, r0, #2
	add r4, r1, #0
	ldr r1, [r5, r0]
	cmp r1, #0
	ble _0222BAE8
	sub r1, r1, #1
	str r1, [r5, r0]
	add r0, r4, #0
	pop {r3, r4, r5, pc}
_0222BAE8:
	ldr r0, _0222BB34 ; =gSystem
	ldr r1, [r0, #0x48]
	mov r0, #3
	tst r0, r1
	bne _0222BAFA
	bl System_GetTouchNew
	cmp r0, #1
	bne _0222BB30
_0222BAFA:
	mov r0, #3
	lsl r0, r0, #8
	add r0, r5, r0
	bl WindowIsInUse
	cmp r0, #0
	beq _0222BB1E
	mov r0, #3
	lsl r0, r0, #8
	add r0, r5, r0
	mov r1, #0
	bl ClearFrameAndWindow2
	mov r0, #3
	lsl r0, r0, #8
	add r0, r5, r0
	bl RemoveWindow
_0222BB1E:
	add r0, r5, #0
	mov r1, #0x58
	mov r2, #0
	bl ov44_0222F510
	mov r0, #0xd2
	mov r1, #0xb
	lsl r0, r0, #2
	str r1, [r5, r0]
_0222BB30:
	add r0, r4, #0
	pop {r3, r4, r5, pc}
	.balign 4, 0
_0222BB34: .word gSystem
	thumb_func_end ov44_0222BAD0

	thumb_func_start ov44_0222BB38
ov44_0222BB38: ; 0x0222BB38
	push {r3, r4, r5, lr}
	add r5, r0, #0
	mov r0, #6
	lsl r0, r0, #6
	ldr r0, [r5, r0]
	add r4, r1, #0
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	bl TextPrinterCheckActive
	cmp r0, #0
	bne _0222BB92
	ldr r0, _0222BB98 ; =0x04000304
	ldrh r1, [r0]
	lsr r0, r0, #0xb
	and r0, r1
	asr r0, r0, #0xf
	cmp r0, #1
	bne _0222BB76
	mov r2, #0x57
	mov r0, #0x35
	lsl r2, r2, #2
	str r0, [sp]
	ldr r0, [r5, r2]
	ldr r1, _0222BB9C ; =ov44_0223538C
	add r2, #0x7d
	mov r3, #0xb
	bl Std_CreateYesNoMenu
	mov r1, #0x61
	b _0222BB86
_0222BB76:
	mov r0, #0x57
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	ldr r1, _0222BBA0 ; =ov44_02235374
	mov r2, #0
	bl ov44_02231A28
	mov r1, #0x62
_0222BB86:
	lsl r1, r1, #2
	str r0, [r5, r1]
	mov r0, #0xd2
	mov r1, #0xc
	lsl r0, r0, #2
	str r1, [r5, r0]
_0222BB92:
	add r0, r4, #0
	pop {r3, r4, r5, pc}
	nop
_0222BB98: .word 0x04000304
_0222BB9C: .word ov44_0223538C
_0222BBA0: .word ov44_02235374
	thumb_func_end ov44_0222BB38

	thumb_func_start ov44_0222BBA4
ov44_0222BBA4: ; 0x0222BBA4
	push {r3, r4, r5, lr}
	add r5, r0, #0
	ldr r0, _0222BC74 ; =0x04000304
	add r4, r1, #0
	ldrh r1, [r0]
	lsr r0, r0, #0xb
	and r0, r1
	asr r0, r0, #0xf
	cmp r0, #1
	bne _0222BC0A
	mov r0, #0x61
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	mov r1, #0x35
	bl Handle2dMenuInput_DeleteOnFinish
	mov r1, #0
	mvn r1, r1
	cmp r0, r1
	bne _0222BBD0
	add r0, r4, #0
	pop {r3, r4, r5, pc}
_0222BBD0:
	cmp r0, #0
	bne _0222BBEA
	bl sub_02039330
	add r0, r5, #0
	mov r1, #0x10
	bl ov44_0222F818
	mov r0, #0xd2
	mov r1, #0xd
	lsl r0, r0, #2
	str r1, [r5, r0]
	b _0222BC6A
_0222BBEA:
	add r0, r5, #0
	mov r1, #0x10
	bl ov44_0222F818
	bl sub_02039330
	bl sub_020343E4
	mov r0, #0xd3
	mov r1, #8
	lsl r0, r0, #2
	str r1, [r5, r0]
	mov r1, #0x22
	sub r0, r0, #4
	str r1, [r5, r0]
	b _0222BC6A
_0222BC0A:
	mov r0, #0x62
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	bl YesNoPrompt_HandleInput
	cmp r0, #1
	bne _0222BC38
	mov r0, #0x62
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	bl YesNoPrompt_Destroy
	bl sub_02039330
	add r0, r5, #0
	mov r1, #0x10
	bl ov44_0222F818
	mov r0, #0xd2
	mov r1, #0xd
	lsl r0, r0, #2
	str r1, [r5, r0]
	b _0222BC6A
_0222BC38:
	cmp r0, #2
	bne _0222BC66
	mov r0, #0x62
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	bl YesNoPrompt_Destroy
	add r0, r5, #0
	mov r1, #0x10
	bl ov44_0222F818
	bl sub_02039330
	bl sub_020343E4
	mov r0, #0xd3
	mov r1, #8
	lsl r0, r0, #2
	str r1, [r5, r0]
	mov r1, #0x22
	sub r0, r0, #4
	str r1, [r5, r0]
	b _0222BC6A
_0222BC66:
	add r0, r4, #0
	pop {r3, r4, r5, pc}
_0222BC6A:
	add r0, r5, #0
	bl ov44_0222F7BC
	add r0, r4, #0
	pop {r3, r4, r5, pc}
	.balign 4, 0
_0222BC74: .word 0x04000304
	thumb_func_end ov44_0222BBA4

	thumb_func_start ov44_0222BC78
ov44_0222BC78: ; 0x0222BC78
	push {r4, r5, r6, lr}
	add r5, r0, #0
	add r4, r1, #0
	bl sub_02034DB8
	cmp r0, #0
	beq _0222BC88
	b _0222BE22
_0222BC88:
	mov r0, #0xe3
	lsl r0, r0, #2
	add r0, r5, r0
	bl ov44_02231BB0
	cmp r0, #1
	bne _0222BCCE
	mov r0, #0
	add r1, r0, #0
	bl sub_0200FBF4
	mov r0, #1
	mov r1, #0
	bl sub_0200FBF4
	mov r0, #0xe3
	lsl r0, r0, #2
	add r0, r5, r0
	bl ov44_02231B4C
	mov r2, #0
	ldr r0, _0222BE28 ; =0x00000A88
	add r3, r5, #0
	add r1, r2, #0
_0222BCB8:
	add r2, r2, #1
	str r1, [r3, r0]
	add r3, r3, #4
	cmp r2, #0x21
	blt _0222BCB8
	mov r0, #0x35
	bl thunk_ClearMainOAM
	mov r0, #0x35
	bl thunk_ClearSubOAM
_0222BCCE:
	add r0, r5, #0
	bl ov44_0222F98C
	cmp r0, #1
	bne _0222BCDE
	add r0, r5, #0
	bl ov44_0222FA28
_0222BCDE:
	mov r0, #0x2f
	lsl r0, r0, #4
	add r0, r5, r0
	bl WindowIsInUse
	cmp r0, #0
	beq _0222BD02
	mov r0, #0x2f
	lsl r0, r0, #4
	add r0, r5, r0
	mov r1, #0
	bl sub_0200E5D4
	mov r0, #0x2f
	lsl r0, r0, #4
	add r0, r5, r0
	bl RemoveWindow
_0222BD02:
	mov r0, #0x2e
	lsl r0, r0, #4
	add r0, r5, r0
	bl WindowIsInUse
	cmp r0, #0
	beq _0222BD1A
	mov r0, #0x2e
	lsl r0, r0, #4
	add r0, r5, r0
	bl RemoveWindow
_0222BD1A:
	mov r0, #0x57
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	mov r1, #3
	bl BgClearTilemapBufferAndCommit
	mov r0, #0x58
	mov r1, #0x35
	bl NARC_New
	add r6, r0, #0
	add r0, r5, #0
	add r1, r6, #0
	bl ov44_0222AEC0
	add r0, r6, #0
	bl NARC_Delete
	ldr r0, _0222BE2C ; =0x00000D6C
	mov r1, #1
	str r1, [r5, r0]
	ldr r0, _0222BE30 ; =gSystem + 0x60
	strb r1, [r0, #9]
	bl GfGfx_SwapDisplay
	mov r1, #0
	mov r0, #0x57
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	add r2, r1, #0
	add r3, r1, #0
	bl BgSetPosTextAndCommit
	mov r0, #0x57
	lsl r0, r0, #2
	mov r1, #0
	ldr r0, [r5, r0]
	mov r2, #3
	add r3, r1, #0
	bl BgSetPosTextAndCommit
	ldr r0, _0222BE34 ; =0x000010BC
	mov r1, #1
	strh r1, [r5, r0]
	mov r0, #0x53
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	cmp r0, #0
	beq _0222BDAC
	mov r0, #0x31
	lsl r0, r0, #4
	add r0, r5, r0
	bl RemoveWindow
	mov r0, #0x15
	lsl r0, r0, #4
	mov r1, #0
	ldr r0, [r5, r0]
	add r2, r1, #0
	bl DestroyListMenu
	mov r0, #0x15
	mov r1, #0
	lsl r0, r0, #4
	str r1, [r5, r0]
	sub r0, r0, #4
	ldr r0, [r5, r0]
	bl ListMenuItems_Delete
	mov r0, #0x53
	mov r1, #0
	lsl r0, r0, #2
	str r1, [r5, r0]
_0222BDAC:
	add r0, r5, #0
	add r0, #0xc
	mov r1, #0
	mov r2, #0x20
	bl MI_CpuFill8
	add r0, r5, #0
	add r0, #0x2c
	mov r1, #0
	mov r2, #0x20
	bl MI_CpuFill8
	add r0, r5, #0
	add r0, #0x4c
	mov r1, #0
	mov r2, #0x80
	bl MI_CpuFill8
	add r0, r5, #0
	add r0, #0xcc
	mov r1, #0
	mov r2, #0x80
	bl MI_CpuFill8
	add r0, r5, #0
	bl ov44_0222B0B0
	mov r0, #0x16
	lsl r0, r0, #4
	ldr r0, [r5, r0]
	ldr r1, _0222BE38 ; =0x000004A4
	bl sub_020396FC
	str r0, [r5, #4]
	add r0, r5, #0
	mov r1, #0x17
	mov r2, #1
	bl ov44_0222F510
	mov r0, #0x63
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	cmp r0, #0
	beq _0222BE08
	bl GF_AssertFail
_0222BE08:
	mov r1, #0x2d
	lsl r1, r1, #4
	add r0, r5, r1
	sub r1, #0xee
	bl WaitingIcon_New
	mov r1, #0x63
	lsl r1, r1, #2
	str r0, [r5, r1]
	mov r0, #0xd2
	mov r1, #0xe
	lsl r0, r0, #2
	str r1, [r5, r0]
_0222BE22:
	add r0, r4, #0
	pop {r4, r5, r6, pc}
	nop
_0222BE28: .word 0x00000A88
_0222BE2C: .word 0x00000D6C
_0222BE30: .word gSystem + 0x60
_0222BE34: .word 0x000010BC
_0222BE38: .word 0x000004A4
	thumb_func_end ov44_0222BC78

	thumb_func_start ov44_0222BE3C
ov44_0222BE3C: ; 0x0222BE3C
	push {r3, r4, r5, lr}
	add r5, r0, #0
	mov r0, #6
	lsl r0, r0, #6
	ldr r0, [r5, r0]
	add r4, r1, #0
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	bl TextPrinterCheckActive
	cmp r0, #0
	beq _0222BE58
	add r0, r4, #0
	pop {r3, r4, r5, pc}
_0222BE58:
	bl ov00_021E7314
	cmp r0, #0
	beq _0222BE92
	mov r0, #0xd2
	mov r1, #0x45
	lsl r0, r0, #2
	str r1, [r5, r0]
	add r0, r5, #0
	mov r1, #0x1e
	mov r2, #1
	bl ov44_0222F510
	mov r0, #0x63
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	cmp r0, #0
	beq _0222BE80
	bl GF_AssertFail
_0222BE80:
	mov r1, #0x2d
	lsl r1, r1, #4
	add r0, r5, r1
	sub r1, #0xee
	bl WaitingIcon_New
	mov r1, #0x63
	lsl r1, r1, #2
	str r0, [r5, r1]
_0222BE92:
	bl sub_02039274
	cmp r0, #0
	beq _0222BEC6
	mov r0, #0xda
	lsl r0, r0, #2
	ldr r1, [r5, r0]
	cmp r1, #0
	beq _0222BEAC
	mov r1, #0x10
	sub r0, #0x20
	str r1, [r5, r0]
	b _0222BEDC
_0222BEAC:
	add r0, r5, #0
	mov r1, #0x10
	bl ov44_0222B778
	add r0, r5, #0
	bl ov44_0222B870
	bl ov44_0222ADD0
	mov r1, #0xd2
	lsl r1, r1, #2
	str r0, [r5, r1]
	b _0222BEDC
_0222BEC6:
	bl sub_020393C8
	cmp r0, #0
	bne _0222BED6
	bl sub_020390C4
	cmp r0, #3
	bne _0222BEDC
_0222BED6:
	add r0, r5, #0
	bl ov44_0222B9A0
_0222BEDC:
	add r0, r4, #0
	pop {r3, r4, r5, pc}
	thumb_func_end ov44_0222BE3C

	thumb_func_start ov44_0222BEE0
ov44_0222BEE0: ; 0x0222BEE0
	push {r3, r4, r5, lr}
	add r5, r0, #0
	mov r0, #6
	lsl r0, r0, #6
	ldr r0, [r5, r0]
	add r4, r1, #0
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	bl TextPrinterCheckActive
	cmp r0, #0
	beq _0222BEFC
	add r0, r4, #0
	pop {r3, r4, r5, pc}
_0222BEFC:
	bl ov00_021E7314
	cmp r0, #0
	beq _0222BF1C
	mov r0, #0x16
	lsl r0, r0, #4
	ldr r0, [r5, r0]
	mov r1, #0
	bl Save_PrepareForAsyncWrite
	mov r0, #0xd2
	mov r1, #0x47
	lsl r0, r0, #2
	str r1, [r5, r0]
	add r0, r4, #0
	pop {r3, r4, r5, pc}
_0222BF1C:
	bl sub_02039274
	cmp r0, #0
	beq _0222BF50
	mov r0, #0xda
	lsl r0, r0, #2
	ldr r1, [r5, r0]
	cmp r1, #0
	beq _0222BF36
	mov r1, #0x10
	sub r0, #0x20
	str r1, [r5, r0]
	b _0222BF66
_0222BF36:
	add r0, r5, #0
	mov r1, #0x10
	bl ov44_0222B778
	add r0, r5, #0
	bl ov44_0222B870
	bl ov44_0222ADD0
	mov r1, #0xd2
	lsl r1, r1, #2
	str r0, [r5, r1]
	b _0222BF66
_0222BF50:
	bl sub_020393C8
	cmp r0, #0
	bne _0222BF60
	bl sub_020390C4
	cmp r0, #3
	bne _0222BF66
_0222BF60:
	add r0, r5, #0
	bl ov44_0222B9A0
_0222BF66:
	add r0, r4, #0
	pop {r3, r4, r5, pc}
	.balign 4, 0
	thumb_func_end ov44_0222BEE0

	thumb_func_start ov44_0222BF6C
ov44_0222BF6C: ; 0x0222BF6C
	push {r3, r4, r5, lr}
	add r5, r0, #0
	add r4, r1, #0
	bl ov00_021E7314
	cmp r0, #0
	beq _0222BFA0
	mov r0, #0x16
	lsl r0, r0, #4
	ldr r0, [r5, r0]
	bl Save_WriteFileAsync
	cmp r0, #0
	beq _0222BF9C
	cmp r0, #1
	beq _0222BF9C
	bl ov00_021E7328
	mov r0, #0x16
	lsl r0, r0, #4
	ldr r0, [r5, r0]
	bl Save_ClearStatusFlags
	b _0222BFA0
_0222BF9C:
	add r0, r4, #0
	pop {r3, r4, r5, pc}
_0222BFA0:
	bl sub_02039274
	cmp r0, #0
	beq _0222BFD4
	mov r0, #0xda
	lsl r0, r0, #2
	ldr r1, [r5, r0]
	cmp r1, #0
	beq _0222BFBA
	mov r1, #0x10
	sub r0, #0x20
	str r1, [r5, r0]
	b _0222BFEA
_0222BFBA:
	add r0, r5, #0
	mov r1, #0x10
	bl ov44_0222B778
	add r0, r5, #0
	bl ov44_0222B870
	bl ov44_0222ADD0
	mov r1, #0xd2
	lsl r1, r1, #2
	str r0, [r5, r1]
	b _0222BFEA
_0222BFD4:
	bl sub_020393C8
	cmp r0, #0
	bne _0222BFE4
	bl sub_020390C4
	cmp r0, #3
	bne _0222BFEA
_0222BFE4:
	add r0, r5, #0
	bl ov44_0222B9A0
_0222BFEA:
	add r0, r4, #0
	pop {r3, r4, r5, pc}
	.balign 4, 0
	thumb_func_end ov44_0222BF6C

	thumb_func_start ov44_0222BFF0
ov44_0222BFF0: ; 0x0222BFF0
	push {r3, r4, r5, lr}
	add r5, r0, #0
	mov r0, #0xd7
	lsl r0, r0, #2
	add r4, r1, #0
	ldr r1, [r5, r0]
	cmp r1, #0
	ble _0222C008
	sub r1, r1, #1
	str r1, [r5, r0]
	add r0, r4, #0
	pop {r3, r4, r5, pc}
_0222C008:
	bl sub_020392A0
	cmp r0, #0
	beq _0222C01C
	bl ov44_0222ADD0
	mov r1, #0xd2
	lsl r1, r1, #2
	str r0, [r5, r1]
	b _0222C05C
_0222C01C:
	bl sub_020393C8
	cmp r0, #0
	bne _0222C02C
	bl sub_020390C4
	cmp r0, #3
	bne _0222C034
_0222C02C:
	add r0, r5, #0
	bl ov44_0222B9A0
	b _0222C05C
_0222C034:
	bl sub_020390C4
	cmp r0, #4
	bge _0222C04E
	bl sub_02039264
	cmp r0, #0
	bne _0222C04E
	mov r0, #0
	bl sub_020373B4
	cmp r0, #0
	bne _0222C05C
_0222C04E:
	bl sub_02039358
	bl ov44_0222ADD0
	mov r1, #0xd2
	lsl r1, r1, #2
	str r0, [r5, r1]
_0222C05C:
	add r0, r4, #0
	pop {r3, r4, r5, pc}
	thumb_func_end ov44_0222BFF0

	thumb_func_start ov44_0222C060
ov44_0222C060: ; 0x0222C060
	push {r3, r4, r5, lr}
	add r5, r0, #0
	add r4, r1, #0
	bl ov44_0222F7BC
	add r0, r5, #0
	mov r1, #0x13
	bl ov44_0222F66C
	mov r0, #0xd2
	mov r1, #0x11
	lsl r0, r0, #2
	str r1, [r5, r0]
	mov r1, #0
	add r0, #0x20
	str r1, [r5, r0]
	add r0, r4, #0
	pop {r3, r4, r5, pc}
	thumb_func_end ov44_0222C060

	thumb_func_start ov44_0222C084
ov44_0222C084: ; 0x0222C084
	push {r3, r4, r5, lr}
	add r5, r0, #0
	ldr r0, _0222C11C ; =gSystem
	add r4, r1, #0
	ldr r1, [r0, #0x48]
	mov r0, #3
	tst r0, r1
	bne _0222C09C
	bl System_GetTouchNew
	cmp r0, #1
	bne _0222C118
_0222C09C:
	mov r1, #0xd5
	lsl r1, r1, #2
	ldr r0, [r5, r1]
	cmp r0, #1
	bne _0222C0BA
	add r0, r1, #0
	mov r2, #0xa
	sub r0, #8
	str r2, [r5, r0]
	mov r0, #0x22
	sub r1, #0xc
	str r0, [r5, r1]
	bl sub_02039330
	b _0222C118
_0222C0BA:
	ldr r0, [r5]
	bl sub_0202C2F8
	cmp r0, #0
	bne _0222C0DE
	add r0, r5, #0
	mov r1, #0x1a
	mov r2, #1
	bl ov44_0222F510
	mov r0, #0xd2
	mov r1, #0x3b
	lsl r0, r0, #2
	str r1, [r5, r0]
	mov r1, #1
	add r0, #0x14
	str r1, [r5, r0]
	b _0222C118
_0222C0DE:
	add r0, r5, #0
	mov r1, #0x10
	bl ov44_0222B778
	add r0, r5, #0
	bl ov44_0222B870
	bl ov44_0222ADD0
	mov r1, #0xd2
	lsl r1, r1, #2
	str r0, [r5, r1]
	sub r1, #0x48
	add r0, r5, r1
	bl WindowIsInUse
	cmp r0, #0
	beq _0222C118
	mov r0, #3
	lsl r0, r0, #8
	add r0, r5, r0
	mov r1, #0
	bl ClearFrameAndWindow2
	mov r0, #3
	lsl r0, r0, #8
	add r0, r5, r0
	bl RemoveWindow
_0222C118:
	add r0, r4, #0
	pop {r3, r4, r5, pc}
	.balign 4, 0
_0222C11C: .word gSystem
	thumb_func_end ov44_0222C084

	thumb_func_start ov44_0222C120
ov44_0222C120: ; 0x0222C120
	push {r3, r4, r5, r6, lr}
	sub sp, #0x14
	add r5, r0, #0
	mov r0, #0x16
	lsl r0, r0, #4
	ldr r0, [r5, r0]
	bl Save_PlayerData_GetProfile
	mov r1, #0x35
	add r6, r0, #0
	bl PlayerProfile_GetPlayerName_NewString
	add r4, r0, #0
	mov r0, #0x2e
	lsl r0, r0, #4
	add r0, r5, r0
	bl WindowIsInUse
	cmp r0, #0
	bne _0222C152
	add r0, r4, #0
	bl String_Delete
	add sp, #0x14
	pop {r3, r4, r5, r6, pc}
_0222C152:
	mov r0, #0x2e
	lsl r0, r0, #4
	add r0, r5, r0
	mov r1, #0
	bl FillWindowPixelBuffer
	add r0, r6, #0
	bl PlayerProfile_GetTrainerGender
	cmp r0, #0
	bne _0222C16C
	ldr r0, _0222C280 ; =0x00050600
	b _0222C170
_0222C16C:
	mov r0, #0xc1
	lsl r0, r0, #0xa
_0222C170:
	str r0, [sp, #0x10]
	mov r1, #0
	str r1, [sp]
	mov r0, #0xff
	str r0, [sp, #4]
	ldr r0, [sp, #0x10]
	add r2, r4, #0
	str r0, [sp, #8]
	mov r0, #0x2e
	lsl r0, r0, #4
	add r0, r5, r0
	mov r3, #0x20
	str r1, [sp, #0xc]
	bl AddTextPrinterParameterizedWithColor
	add r0, r4, #0
	bl String_Delete
	ldr r1, [r5, #4]
	add r0, r5, #0
	bl ov44_02229F00
	add r1, sp, #0x10
	add r4, r0, #0
	bl ov44_0223120C
	add r6, r0, #0
	cmp r4, #1
	bne _0222C204
	bl ov00_021E6EBC
	mov r1, #0
	mvn r1, r1
	cmp r0, r1
	bne _0222C1C8
	mov r2, #0x5a
	lsl r2, r2, #2
	ldr r0, [r5, r2]
	add r2, #8
	ldr r2, [r5, r2]
	add r1, r6, #0
	bl ReadMsgDataIntoString
	b _0222C214
_0222C1C8:
	bl ov00_021E6EBC
	add r1, r0, #0
	ldr r0, [r5]
	mov r2, #8
	bl sub_0202C090
	cmp r0, #0
	bne _0222C1E0
	ldr r0, _0222C280 ; =0x00050600
	str r0, [sp, #0x10]
	b _0222C1EA
_0222C1E0:
	cmp r0, #1
	bne _0222C1EA
	mov r0, #0xc1
	lsl r0, r0, #0xa
	str r0, [sp, #0x10]
_0222C1EA:
	bl ov00_021E6EBC
	add r1, r0, #0
	ldr r0, [r5]
	bl sub_0202C254
	add r1, r0, #0
	mov r0, #0x17
	lsl r0, r0, #4
	ldr r0, [r5, r0]
	bl CopyU16ArrayToString
	b _0222C214
_0222C204:
	mov r2, #0x5a
	lsl r2, r2, #2
	ldr r0, [r5, r2]
	add r2, #8
	ldr r2, [r5, r2]
	add r1, r6, #0
	bl ReadMsgDataIntoString
_0222C214:
	mov r1, #0
	str r1, [sp]
	mov r0, #0xff
	str r0, [sp, #4]
	ldr r0, [sp, #0x10]
	mov r2, #0x2e
	str r0, [sp, #8]
	lsl r2, r2, #4
	add r0, r5, r2
	str r1, [sp, #0xc]
	lsr r2, r2, #1
	ldr r2, [r5, r2]
	mov r3, #0x66
	bl AddTextPrinterParameterizedWithColor
	mov r2, #1
	str r2, [sp]
	mov r0, #0x57
	ldr r1, _0222C284 ; =0x00000B0C
	str r4, [sp, #4]
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	add r1, r5, r1
	mov r3, #2
	bl ov44_0222AC54
	ldr r0, [r5, #4]
	add r0, #0x21
	ldrb r0, [r0]
	cmp r0, #0
	beq _0222C256
	mov r0, #8
	b _0222C258
_0222C256:
	mov r0, #1
_0222C258:
	mov r2, #1
	str r2, [sp]
	str r0, [sp, #4]
	mov r0, #0
	str r0, [sp, #8]
	mov r0, #0x57
	lsl r0, r0, #2
	ldr r1, _0222C284 ; =0x00000B0C
	ldr r0, [r5, r0]
	add r1, r5, r1
	mov r3, #0x1a
	bl ov44_0222ABDC
	mov r0, #0x2e
	lsl r0, r0, #4
	add r0, r5, r0
	bl ScheduleWindowCopyToVram
	add sp, #0x14
	pop {r3, r4, r5, r6, pc}
	.balign 4, 0
_0222C280: .word 0x00050600
_0222C284: .word 0x00000B0C
	thumb_func_end ov44_0222C120

	thumb_func_start ov44_0222C288
ov44_0222C288: ; 0x0222C288
	push {r3, lr}
	cmp r2, #0
	bne _0222C294
	ldr r0, _0222C298 ; =0x000005DC
	bl PlaySE
_0222C294:
	pop {r3, pc}
	nop
_0222C298: .word 0x000005DC
	thumb_func_end ov44_0222C288

	thumb_func_start ov44_0222C29C
ov44_0222C29C: ; 0x0222C29C
	push {r3, r4, lr}
	sub sp, #0x14
	add r4, r0, #0
	mov r0, #0x2e
	lsl r0, r0, #4
	add r0, r4, r0
	bl WindowIsInUse
	cmp r0, #0
	beq _0222C2BA
	mov r0, #0x2e
	lsl r0, r0, #4
	add r0, r4, r0
	bl RemoveWindow
_0222C2BA:
	mov r0, #0x2f
	lsl r0, r0, #4
	add r0, r4, r0
	bl WindowIsInUse
	cmp r0, #0
	beq _0222C2DE
	mov r0, #0x2f
	lsl r0, r0, #4
	add r0, r4, r0
	mov r1, #0
	bl sub_0200E5D4
	mov r0, #0x2f
	lsl r0, r0, #4
	add r0, r4, r0
	bl RemoveWindow
_0222C2DE:
	mov r3, #1
	str r3, [sp]
	mov r0, #0x1c
	str r0, [sp, #4]
	mov r0, #2
	str r0, [sp, #8]
	mov r0, #0xd
	str r0, [sp, #0xc]
	ldr r0, _0222C350 ; =0x00000192
	mov r1, #0x2e
	str r0, [sp, #0x10]
	sub r0, #0x36
	lsl r1, r1, #4
	ldr r0, [r4, r0]
	add r1, r4, r1
	mov r2, #3
	bl AddWindowParameterized
	mov r2, #1
	mov r1, #0x2f
	lsl r1, r1, #4
	str r2, [sp]
	mov r0, #0x1c
	str r0, [sp, #4]
	mov r0, #2
	str r0, [sp, #8]
	mov r0, #0xd
	str r0, [sp, #0xc]
	ldr r0, _0222C354 ; =0x000001A1
	add r1, r4, r1
	str r0, [sp, #0x10]
	sub r0, #0x45
	ldr r0, [r4, r0]
	add r3, r2, #0
	bl AddWindowParameterized
	mov r0, #0x2f
	lsl r0, r0, #4
	add r0, r4, r0
	mov r1, #0xf
	bl FillWindowPixelBuffer
	mov r0, #0x2f
	lsl r0, r0, #4
	add r0, r4, r0
	bl ScheduleWindowCopyToVram
	mov r0, #0x2f
	lsl r0, r0, #4
	ldr r2, _0222C358 ; =0x000001D9
	add r0, r4, r0
	mov r1, #0
	mov r3, #0xb
	bl DrawFrameAndWindow1
	add sp, #0x14
	pop {r3, r4, pc}
	.balign 4, 0
_0222C350: .word 0x00000192
_0222C354: .word 0x000001A1
_0222C358: .word 0x000001D9
	thumb_func_end ov44_0222C29C

	thumb_func_start ov44_0222C35C
ov44_0222C35C: ; 0x0222C35C
	push {r4, r5, r6, r7, lr}
	sub sp, #0xc
	add r5, r0, #0
	add r7, r1, #0
	bl IsPaletteFadeFinished
	cmp r0, #0
	bne _0222C372
	add sp, #0xc
	add r0, r7, #0
	pop {r4, r5, r6, r7, pc}
_0222C372:
	bl sub_020393C8
	cmp r0, #0
	bne _0222C388
	bl sub_020392A0
	cmp r0, #0
	bne _0222C388
	add sp, #0xc
	add r0, r7, #0
	pop {r4, r5, r6, r7, pc}
_0222C388:
	ldr r0, _0222C4D8 ; =sub_02078D24
	bl ov00_021E5CBC
	ldr r0, _0222C4DC ; =0x00000D6C
	mov r1, #0
	str r1, [r5, r0]
	mov r0, #0x58
	mov r1, #0x35
	bl NARC_New
	add r6, r0, #0
	mov r0, #0x57
	lsl r0, r0, #2
	ldr r1, _0222C4E0 ; =0x00000B0C
	ldr r0, [r5, r0]
	add r1, r5, r1
	add r2, r6, #0
	mov r3, #0x35
	bl ov44_0222AB24
	add r0, r5, #0
	bl ov44_0222F8F0
	add r0, r5, #0
	bl ov44_0222F7BC
	mov r0, #0x57
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	mov r1, #3
	bl BgClearTilemapBufferAndCommit
	mov r0, #0x53
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	cmp r0, #0
	beq _0222C402
	mov r0, #0x31
	lsl r0, r0, #4
	add r0, r5, r0
	bl RemoveWindow
	mov r0, #0x15
	lsl r0, r0, #4
	mov r1, #0
	ldr r0, [r5, r0]
	add r2, r1, #0
	bl DestroyListMenu
	mov r0, #0x15
	mov r1, #0
	lsl r0, r0, #4
	str r1, [r5, r0]
	sub r0, r0, #4
	ldr r0, [r5, r0]
	bl ListMenuItems_Delete
	mov r0, #0x53
	mov r1, #0
	lsl r0, r0, #2
	str r1, [r5, r0]
_0222C402:
	mov r0, #0xe3
	lsl r0, r0, #2
	add r0, r5, r0
	bl ov44_02231BB0
	cmp r0, #0
	bne _0222C456
	ldr r0, [r5, #4]
	ldrb r0, [r0, #0x1e]
	cmp r0, #0
	bne _0222C41C
	mov r4, #0
	b _0222C41E
_0222C41C:
	mov r4, #0x61
_0222C41E:
	ldr r0, [r5]
	bl sub_0202C318
	ldr r1, _0222C4E4 ; =0x000010BE
	mov r3, #0x57
	strh r0, [r5, r1]
	str r4, [sp]
	ldrh r0, [r5, r1]
	lsl r3, r3, #2
	mov r1, #0x35
	str r0, [sp, #4]
	mov r0, #0xe3
	lsl r0, r0, #2
	ldr r3, [r5, r3]
	add r0, r5, r0
	add r2, r6, #0
	bl ov44_02231A7C
	mov r0, #0xe3
	lsl r0, r0, #2
	add r0, r5, r0
	add r1, r4, #0
	bl ov44_02231D9C
	add r1, r0, #0
	add r0, r5, #0
	bl ov44_0222AA94
_0222C456:
	add r0, r5, #0
	bl ov44_0222F98C
	cmp r0, #0
	bne _0222C46A
	add r0, r5, #0
	add r1, r6, #0
	mov r2, #0x35
	bl ov44_0222F9A0
_0222C46A:
	add r0, r5, #0
	mov r1, #0x35
	bl ov44_02230060
	add r0, r5, #0
	bl ov44_0222F7BC
	add r0, r5, #0
	bl ov44_0222B870
	add r0, r5, #0
	bl ov44_0222C29C
	add r0, r5, #0
	mov r1, #0x10
	bl ov44_0222F818
	bl sub_02039B58
	mov r1, #0
	mov r0, #0x37
	mvn r1, r1
	lsl r0, r0, #4
	str r1, [r5, r0]
	add r0, r6, #0
	bl NARC_Delete
	ldr r0, _0222C4E8 ; =gSystem + 0x60
	mov r1, #0
	strb r1, [r0, #9]
	bl GfGfx_SwapDisplay
	mov r0, #6
	str r0, [sp]
	mov r1, #1
	str r1, [sp, #4]
	mov r0, #0x35
	str r0, [sp, #8]
	mov r0, #0
	add r2, r1, #0
	add r3, r0, #0
	bl BeginNormalPaletteFade
	mov r0, #0xd2
	mov r1, #0x13
	lsl r0, r0, #2
	str r1, [r5, r0]
	mov r0, #0
	add r1, r0, #0
	bl sub_020398D4
	add r0, r7, #0
	add sp, #0xc
	pop {r4, r5, r6, r7, pc}
	nop
_0222C4D8: .word sub_02078D24
_0222C4DC: .word 0x00000D6C
_0222C4E0: .word 0x00000B0C
_0222C4E4: .word 0x000010BE
_0222C4E8: .word gSystem + 0x60
	thumb_func_end ov44_0222C35C

	thumb_func_start ov44_0222C4EC
ov44_0222C4EC: ; 0x0222C4EC
	push {r3, lr}
	bl sub_020390C4
	cmp r0, #1
	bne _0222C4FA
	mov r0, #1
	pop {r3, pc}
_0222C4FA:
	mov r0, #0
	pop {r3, pc}
	.balign 4, 0
	thumb_func_end ov44_0222C4EC

	thumb_func_start ov44_0222C500
ov44_0222C500: ; 0x0222C500
	push {r4, lr}
	add r4, r0, #0
	bl ov00_021E709C
	mov r1, #0
	mvn r1, r1
	cmp r0, r1
	beq _0222C51E
	ldr r0, [r4, #4]
	add r0, #0x21
	ldrb r0, [r0]
	bl ov00_021E70B8
	mov r0, #1
	pop {r4, pc}
_0222C51E:
	mov r0, #0
	pop {r4, pc}
	.balign 4, 0
	thumb_func_end ov44_0222C500

	thumb_func_start ov44_0222C524
ov44_0222C524: ; 0x0222C524
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x10
	add r5, r0, #0
	bl ov44_0222B870
	str r0, [sp, #8]
	mov r0, #0
	add r1, r5, #0
	str r0, [sp, #4]
	str r0, [sp]
	add r0, r5, #0
	add r0, #0xc
	add r1, #0x2c
	mov r2, #0x20
	bl memcmp
	cmp r0, #0
	bne _0222C54A
	b _0222C64C
_0222C54A:
	mov r4, #0
_0222C54C:
	add r7, r5, r4
	add r1, r7, #0
	add r1, #0x2c
	ldrb r1, [r1]
	mov r2, #0
	add r0, r2, #0
_0222C558:
	add r3, r5, r0
	ldrb r3, [r3, #0xc]
	cmp r3, r1
	bne _0222C564
	mov r2, #1
	b _0222C56A
_0222C564:
	add r0, r0, #1
	cmp r0, #0x20
	blt _0222C558
_0222C56A:
	cmp r2, #0
	bne _0222C5AE
	cmp r1, #0
	beq _0222C5AE
	add r0, r5, #0
	bl ov44_0222AAEC
	add r6, r0, #0
	beq _0222C5AE
	mov r0, #0xe3
	lsl r0, r0, #2
	add r0, r5, r0
	add r1, r6, #0
	bl ov44_02231E94
	add r0, r5, #0
	add r1, r6, #0
	bl ov44_0222AAC0
	add r1, r5, r4
	add r1, #0x2c
	ldrb r1, [r1]
	add r0, r5, #0
	bl ov44_02230028
	add r1, r5, r4
	add r1, #0x2c
	ldrb r1, [r1]
	add r0, r5, #0
	mov r2, #0x35
	bl ov44_0222F970
	mov r0, #1
	str r0, [sp]
_0222C5AE:
	ldrb r0, [r7, #0xc]
	mov r1, #0
	add r2, r1, #0
_0222C5B4:
	add r3, r5, r2
	add r3, #0x2c
	ldrb r3, [r3]
	cmp r0, r3
	bne _0222C5C2
	mov r1, #1
	b _0222C5C8
_0222C5C2:
	add r2, r2, #1
	cmp r2, #0x20
	blt _0222C5B4
_0222C5C8:
	cmp r1, #0
	bne _0222C618
	ldrb r1, [r7, #0xc]
	cmp r1, #0
	beq _0222C618
	add r0, r5, #0
	sub r1, r1, #1
	bl ov44_02231958
	add r1, r0, #0
	add r6, r5, r4
	mov r0, #0xe3
	lsl r0, r0, #2
	ldrb r1, [r1, #0x1d]
	ldrb r2, [r6, #0xc]
	add r0, r5, r0
	bl ov44_02231E08
	str r0, [sp, #0xc]
	cmp r0, #0
	beq _0222C60A
	ldrb r1, [r7, #0xc]
	add r0, r5, #0
	mov r2, #2
	bl ov44_0222FFF4
	ldr r1, [sp, #0xc]
	add r0, r5, #0
	bl ov44_0222AA94
	mov r0, #1
	str r0, [sp, #4]
	b _0222C618
_0222C60A:
	ldrb r1, [r6, #0xc]
	add r0, r5, #0
	mov r2, #1
	bl ov44_0222FFF4
	mov r0, #0
	strb r0, [r6, #0xc]
_0222C618:
	add r4, r4, #1
	cmp r4, #0x20
	blt _0222C54C
	add r0, r5, #0
	add r1, r5, #0
	add r0, #0x2c
	add r1, #0xc
	mov r2, #0x20
	bl memcpy
	add r0, r5, #0
	bl ov44_02230080
	ldr r0, [sp, #4]
	cmp r0, #1
	bne _0222C640
	ldr r0, _0222C654 ; =0x0000064E
	bl PlaySE
	b _0222C64C
_0222C640:
	ldr r0, [sp]
	cmp r0, #1
	bne _0222C64C
	ldr r0, _0222C654 ; =0x0000064E
	bl PlaySE
_0222C64C:
	ldr r0, [sp, #8]
	add sp, #0x10
	pop {r3, r4, r5, r6, r7, pc}
	nop
_0222C654: .word 0x0000064E
	thumb_func_end ov44_0222C524

	thumb_func_start ov44_0222C658
ov44_0222C658: ; 0x0222C658
	push {r3, r4, r5, lr}
	add r5, r0, #0
	add r4, r1, #0
	bl ov44_0222B8E0
	cmp r0, #0
	ble _0222C66E
	add r0, r5, #0
	add r1, r4, #0
	bl ov44_02230060
_0222C66E:
	pop {r3, r4, r5, pc}
	thumb_func_end ov44_0222C658

	thumb_func_start ov44_0222C670
ov44_0222C670: ; 0x0222C670
	push {r3, r4, r5, lr}
	add r5, r0, #0
	add r4, r1, #0
	bl ov44_0222C524
	add r0, r5, #0
	add r1, r4, #0
	bl ov44_0222C658
	pop {r3, r4, r5, pc}
	thumb_func_end ov44_0222C670

	thumb_func_start ov44_0222C684
ov44_0222C684: ; 0x0222C684
	push {r3, r4, r5, r6, r7, lr}
	add r4, r0, #0
	add r5, r1, #0
	mov r7, #0
	bl IsPaletteFadeFinished
	cmp r0, #0
	bne _0222C698
	add r0, r5, #0
	pop {r3, r4, r5, r6, r7, pc}
_0222C698:
	bl sub_020393C8
	cmp r0, #0
	beq _0222C6AA
	add r0, r4, #0
	bl ov44_0222B9A0
	add r0, r5, #0
	pop {r3, r4, r5, r6, r7, pc}
_0222C6AA:
	bl sub_020390C4
	cmp r0, #4
	bne _0222C6E0
	bl ov00_021E6EBC
	add r1, r0, #0
	add r0, r4, #0
	bl ov44_0222F780
	add r0, r4, #0
	mov r1, #0x12
	add r2, r7, #0
	bl ov44_0222F510
	mov r0, #0xd2
	mov r1, #0x1d
	lsl r0, r0, #2
	str r1, [r4, r0]
	add r0, r4, #0
	mov r1, #0x10
	bl ov44_0222F818
	bl sub_02039358
	add r0, r5, #0
	pop {r3, r4, r5, r6, r7, pc}
_0222C6E0:
	add r0, r4, #0
	mov r1, #0x35
	bl ov44_0222C670
	add r0, r4, #0
	bl ov44_0222FBA0
	mov r0, #0xe3
	lsl r0, r0, #2
	add r0, r4, r0
	bl ov44_02232008
	mov r0, #0xe3
	lsl r0, r0, #2
	add r0, r4, r0
	bl ov44_02231D98
	cmp r0, #0
	bne _0222C79A
	bl ov00_021E709C
	sub r1, r7, #1
	cmp r0, r1
	bne _0222C75C
	ldr r0, _0222C9A0 ; =gSystem
	ldr r1, [r0, #0x48]
	mov r0, #1
	lsl r0, r0, #0xa
	tst r0, r1
	beq _0222C75C
	add r0, r4, #0
	bl ov44_0222F8C0
	cmp r0, #0
	beq _0222C732
	add r0, r4, #0
	mov r1, #0x5a
	add r2, r7, #0
	bl ov44_0222F510
	b _0222C73C
_0222C732:
	add r0, r4, #0
	mov r1, #0x5b
	add r2, r7, #0
	bl ov44_0222F510
_0222C73C:
	ldr r0, _0222C9A4 ; =0x000005DD
	bl PlaySE
	add r0, r4, #0
	bl ov44_0222C120
	mov r0, #0xd2
	mov r1, #0x14
	lsl r0, r0, #2
	str r1, [r4, r0]
	add r0, r4, #0
	mov r1, #0x35
	bl ov44_0222F950
	add r0, r5, #0
	pop {r3, r4, r5, r6, r7, pc}
_0222C75C:
	add r0, r4, #0
	mov r1, #0x35
	bl ov44_0222FA80
	add r6, r0, #0
	add r0, r4, #0
	add r1, r6, #0
	bl ov44_0222AAEC
	add r1, r0, #0
	cmp r6, #0
	beq _0222C790
	cmp r1, #0
	beq _0222C784
	mov r0, #0xe3
	lsl r0, r0, #2
	add r0, r4, r0
	bl ov44_02231D48
	b _0222C79A
_0222C784:
	mov r0, #0xe3
	lsl r0, r0, #2
	add r0, r4, r0
	bl ov44_02231D8C
	b _0222C79A
_0222C790:
	mov r0, #0xe3
	lsl r0, r0, #2
	add r0, r4, r0
	bl ov44_02231D8C
_0222C79A:
	add r0, r4, #0
	bl ov44_0222C4EC
	cmp r0, #0
	beq _0222C7C4
	mov r0, #0x37
	lsl r0, r0, #4
	ldr r1, [r4, r0]
	mov r0, #0
	mvn r0, r0
	cmp r1, r0
	beq _0222C7C4
	ldr r0, _0222C9A8 ; =0x00000623
	bl PlaySE
	mov r0, #0xd2
	mov r1, #0x30
	lsl r0, r0, #2
	str r1, [r4, r0]
	add r0, r5, #0
	pop {r3, r4, r5, r6, r7, pc}
_0222C7C4:
	mov r0, #0x37
	lsl r0, r0, #4
	ldr r1, [r4, r0]
	mov r0, #0
	mvn r0, r0
	cmp r1, r0
	bne _0222C80E
	bl ov00_021E709C
	mov r1, #0
	mvn r1, r1
	cmp r0, r1
	beq _0222C80E
	bl ov00_021E709C
	mov r1, #0x37
	lsl r1, r1, #4
	str r0, [r4, r1]
	ldr r1, [r4, r1]
	add r0, r4, #0
	bl ov44_0222F780
	add r0, r4, #0
	mov r1, #0x4c
	mov r2, #0
	bl ov44_0222F510
	ldr r0, [r4, #4]
	add r0, #0x21
	ldrb r0, [r0]
	bl ov00_021E70B8
	mov r0, #0xd1
	mov r1, #0
	lsl r0, r0, #2
	str r1, [r4, r0]
	b _0222C854
_0222C80E:
	mov r0, #0x37
	lsl r0, r0, #4
	ldr r1, [r4, r0]
	mov r0, #0
	mvn r0, r0
	cmp r1, r0
	beq _0222C854
	bl ov00_021E709C
	mov r1, #0
	mvn r1, r1
	cmp r0, r1
	bne _0222C854
	mov r0, #0
	bl ov00_021E70B8
	mov r1, #0x37
	lsl r1, r1, #4
	ldr r1, [r4, r1]
	add r0, r4, #0
	bl ov44_0222F780
	add r0, r4, #0
	mov r1, #0x12
	mov r2, #0
	bl ov44_0222F510
	bl sub_02039358
	mov r0, #0xd2
	mov r1, #0x1b
	lsl r0, r0, #2
	str r1, [r4, r0]
	add r0, r5, #0
	pop {r3, r4, r5, r6, r7, pc}
_0222C854:
	bl ov00_021E7080
	cmp r0, #1
	bne _0222C86E
	bl ov00_021E709C
	mov r1, #0
	mvn r1, r1
	cmp r0, r1
	bne _0222C86E
	mov r0, #0
	bl ov00_021E70B8
_0222C86E:
	ldr r1, [r4, #4]
	add r0, r4, #0
	bl ov44_02229F00
	mov r1, #0x37
	lsl r1, r1, #4
	ldr r2, [r4, r1]
	mov r1, #0
	mvn r1, r1
	add r6, r0, #0
	cmp r2, r1
	bne _0222C8C8
	ldr r1, _0222C9A0 ; =gSystem
	ldr r2, [r1, #0x48]
	mov r1, #2
	tst r1, r2
	beq _0222C8B2
	bl ov44_02229FDC
	cmp r0, #0
	beq _0222C8B2
	ldr r0, _0222C9A4 ; =0x000005DD
	bl PlaySE
	mov r0, #0xd2
	mov r1, #0x27
	lsl r0, r0, #2
	str r1, [r4, r0]
	add r0, r4, #0
	mov r1, #0x35
	bl ov44_0222F950
	add r0, r5, #0
	pop {r3, r4, r5, r6, r7, pc}
_0222C8B2:
	mov r0, #0xe3
	lsl r0, r0, #2
	add r0, r4, r0
	bl ov44_02231BB4
	add r7, r0, #0
	mov r0, #0xe3
	lsl r0, r0, #2
	add r0, r4, r0
	bl ov44_02232018
_0222C8C8:
	cmp r7, #3
	bhi _0222C998
	add r0, r7, r7
	add r0, pc
	ldrh r0, [r0, #6]
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	add pc, r0
_0222C8D8: ; jump table
	.short _0222C8E0 - _0222C8D8 - 2 ; case 0
	.short _0222C8E4 - _0222C8D8 - 2 ; case 1
	.short _0222C970 - _0222C8D8 - 2 ; case 2
	.short _0222C922 - _0222C8D8 - 2 ; case 3
_0222C8E0:
	add r0, r5, #0
	pop {r3, r4, r5, r6, r7, pc}
_0222C8E4:
	ldr r0, _0222C9A4 ; =0x000005DD
	bl PlaySE
	add r0, r6, #0
	bl ov44_02229FDC
	cmp r0, #0
	beq _0222C8FE
	mov r0, #0xd2
	mov r1, #0x27
	lsl r0, r0, #2
	str r1, [r4, r0]
	b _0222C916
_0222C8FE:
	mov r0, #0xd3
	mov r1, #8
	lsl r0, r0, #2
	str r1, [r4, r0]
	mov r1, #0x39
	sub r0, r0, #4
	str r1, [r4, r0]
	add r0, r4, #0
	mov r1, #0x19
	mov r2, #1
	bl ov44_0222F510
_0222C916:
	add r0, r4, #0
	mov r1, #0x35
	bl ov44_0222F950
	add r0, r5, #0
	pop {r3, r4, r5, r6, r7, pc}
_0222C922:
	ldr r0, _0222C9A4 ; =0x000005DD
	bl PlaySE
	add r0, r6, #0
	bl ov44_02229FDC
	cmp r0, #0
	beq _0222C946
	mov r0, #0xe3
	lsl r0, r0, #2
	add r0, r4, r0
	bl ov44_02231FCC
	mov r0, #0xd2
	mov r1, #0x27
	lsl r0, r0, #2
	str r1, [r4, r0]
	b _0222C964
_0222C946:
	cmp r6, #0x10
	bne _0222C964
	bl sub_020392A0
	cmp r0, #1
	bne _0222C964
	mov r0, #0xe3
	lsl r0, r0, #2
	add r0, r4, r0
	bl ov44_02231FCC
	mov r0, #0xd2
	mov r1, #0x24
	lsl r0, r0, #2
	str r1, [r4, r0]
_0222C964:
	add r0, r4, #0
	mov r1, #0x35
	bl ov44_0222F950
	add r0, r5, #0
	pop {r3, r4, r5, r6, r7, pc}
_0222C970:
	ldr r0, _0222C9A4 ; =0x000005DD
	bl PlaySE
	add r0, r6, #0
	bl ov44_02229FDC
	cmp r0, #0
	beq _0222C984
	mov r1, #0x27
	b _0222C986
_0222C984:
	mov r1, #0x2a
_0222C986:
	mov r0, #0xd2
	lsl r0, r0, #2
	str r1, [r4, r0]
	add r0, r4, #0
	mov r1, #0x35
	bl ov44_0222F950
	add r0, r5, #0
	pop {r3, r4, r5, r6, r7, pc}
_0222C998:
	bl GF_AssertFail
	add r0, r5, #0
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
_0222C9A0: .word gSystem
_0222C9A4: .word 0x000005DD
_0222C9A8: .word 0x00000623
	thumb_func_end ov44_0222C684

	thumb_func_start ov44_0222C9AC
ov44_0222C9AC: ; 0x0222C9AC
	push {r3, r4, r5, lr}
	add r5, r0, #0
	add r4, r1, #0
	bl ov44_0222C500
	cmp r0, #0
	beq _0222C9CA
	mov r0, #0xd2
	mov r1, #0x13
	lsl r0, r0, #2
	str r1, [r5, r0]
	add r0, r5, #0
	bl ov44_022319EC
	b _0222C9F0
_0222C9CA:
	mov r0, #6
	lsl r0, r0, #6
	ldr r0, [r5, r0]
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	bl TextPrinterCheckActive
	cmp r0, #0
	bne _0222C9F0
	mov r0, #0xd2
	mov r1, #0x13
	lsl r0, r0, #2
	str r1, [r5, r0]
	add r0, r5, #0
	bl ov44_022319EC
	add r0, r5, #0
	bl ov44_0222F7BC
_0222C9F0:
	add r0, r4, #0
	pop {r3, r4, r5, pc}
	thumb_func_end ov44_0222C9AC

	thumb_func_start ov44_0222C9F4
ov44_0222C9F4: ; 0x0222C9F4
	push {r3, r4, r5, r6, r7, lr}
	add r7, r1, #0
	ldr r1, _0222CB30 ; =0x00000382
	add r5, r0, #0
	ldrh r1, [r5, r1]
	sub r1, r1, #1
	bl ov44_02231958
	add r1, r0, #0
	add r0, #0x21
	ldrb r4, [r0]
	add r0, r5, #0
	bl ov44_02229F00
	add r6, r0, #0
	mov r0, #0xd
	lsl r0, r0, #6
	ldr r1, [r5, r0]
	sub r1, r1, #1
	str r1, [r5, r0]
	ldr r1, [r5, r0]
	cmp r1, #0
	bge _0222CA2A
	mov r1, #0x46
	add r0, #8
	str r1, [r5, r0]
	b _0222CB2C
_0222CA2A:
	bl sub_020390C4
	cmp r0, #5
	bne _0222CA52
	bl ov00_021E6EBC
	add r1, r0, #0
	add r0, r5, #0
	bl ov44_0222F780
	add r0, r5, #0
	mov r1, #0xf
	mov r2, #0
	bl ov44_0222F510
	mov r0, #0xd2
	mov r1, #0x1b
	lsl r0, r0, #2
	str r1, [r5, r0]
	b _0222CB2C
_0222CA52:
	bl sub_020390C4
	cmp r0, #3
	bne _0222CA7A
	bl ov00_021E6EBC
	add r1, r0, #0
	add r0, r5, #0
	bl ov44_0222F780
	add r0, r5, #0
	mov r1, #0x12
	mov r2, #0
	bl ov44_0222F510
	mov r0, #0xd2
	mov r1, #0x1b
	lsl r0, r0, #2
	str r1, [r5, r0]
	b _0222CB2C
_0222CA7A:
	bl sub_020390C4
	cmp r0, #4
	beq _0222CA8A
	bl sub_02039264
	cmp r0, #0
	beq _0222CAAA
_0222CA8A:
	bl ov00_021E6EBC
	add r1, r0, #0
	add r0, r5, #0
	bl ov44_0222F780
	add r0, r5, #0
	mov r1, #0x13
	mov r2, #0
	bl ov44_0222F510
	mov r0, #0xd2
	mov r1, #0x1b
	lsl r0, r0, #2
	str r1, [r5, r0]
	b _0222CB2C
_0222CAAA:
	bl sub_020393C8
	cmp r0, #0
	beq _0222CABA
	add r0, r5, #0
	bl ov44_0222B9A0
	b _0222CB2C
_0222CABA:
	cmp r4, #0
	bne _0222CADE
	bl ov00_021E6EBC
	add r1, r0, #0
	add r0, r5, #0
	bl ov44_0222F780
	add r0, r5, #0
	mov r1, #0x10
	mov r2, #0
	bl ov44_0222F510
	mov r0, #0xd2
	mov r1, #0x1b
	lsl r0, r0, #2
	str r1, [r5, r0]
	b _0222CB2C
_0222CADE:
	cmp r6, #0x10
	beq _0222CB02
	bl ov00_021E6EBC
	add r1, r0, #0
	add r0, r5, #0
	bl ov44_0222F780
	add r0, r5, #0
	mov r1, #0x10
	mov r2, #0
	bl ov44_0222F510
	mov r0, #0xd2
	mov r1, #0x1b
	lsl r0, r0, #2
	str r1, [r5, r0]
	b _0222CB2C
_0222CB02:
	bl sub_020390C4
	cmp r0, #1
	bne _0222CB2C
	add r0, r5, #0
	bl ov44_0222F7BC
	add r0, r5, #0
	mov r1, #1
	bl ov44_0222F818
	mov r1, #0xd2
	mov r0, #0x18
	lsl r1, r1, #2
	str r0, [r5, r1]
	add r1, #0x3a
	ldrh r1, [r5, r1]
	ldr r0, [r5]
	sub r1, r1, #1
	bl sub_0202C4B0
_0222CB2C:
	add r0, r7, #0
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
_0222CB30: .word 0x00000382
	thumb_func_end ov44_0222C9F4

	thumb_func_start ov44_0222CB34
ov44_0222CB34: ; 0x0222CB34
	push {r3, r4, r5, lr}
	add r5, r0, #0
	add r4, r1, #0
	bl ov00_021E6EBC
	add r1, r0, #0
	add r0, r5, #0
	bl ov44_0222F780
	add r0, r5, #0
	mov r1, #0xe
	mov r2, #0
	bl ov44_0222F510
	add r0, r5, #0
	mov r1, #1
	bl ov44_0222F818
	mov r0, #0xd2
	mov r1, #0x17
	lsl r0, r0, #2
	str r1, [r5, r0]
	bl ov00_021E6EBC
	mov r1, #0
	mvn r1, r1
	cmp r0, r1
	beq _0222CB78
	bl ov00_021E6EBC
	add r1, r0, #0
	ldr r0, [r5]
	bl sub_0202C4B0
_0222CB78:
	add r0, r4, #0
	pop {r3, r4, r5, pc}
	thumb_func_end ov44_0222CB34

	thumb_func_start ov44_0222CB7C
ov44_0222CB7C: ; 0x0222CB7C
	push {r3, r4, r5, lr}
	add r5, r0, #0
	add r4, r1, #0
	bl sub_020393C8
	cmp r0, #0
	beq _0222CB94
	add r0, r5, #0
	bl ov44_0222B9A0
	add r0, r4, #0
	pop {r3, r4, r5, pc}
_0222CB94:
	bl ov00_021E6EBC
	cmp r0, #0
	bge _0222CBB2
	add r0, r5, #0
	mov r1, #0x13
	mov r2, #0
	bl ov44_0222F510
	mov r0, #0xd2
	mov r1, #0x1b
	lsl r0, r0, #2
	str r1, [r5, r0]
	add r0, r4, #0
	pop {r3, r4, r5, pc}
_0222CBB2:
	bl ov00_021E6EBC
	add r1, r0, #0
	add r0, r5, #0
	bl ov44_02231958
	add r1, r0, #0
	add r0, r5, #0
	bl ov44_02229F00
	cmp r0, #0x10
	beq _0222CBF0
	cmp r0, #1
	beq _0222CBF0
	bl ov00_021E6EBC
	add r1, r0, #0
	add r0, r5, #0
	bl ov44_0222F780
	add r0, r5, #0
	mov r1, #0x10
	mov r2, #0
	bl ov44_0222F510
	mov r0, #0xd2
	mov r1, #0x1b
	lsl r0, r0, #2
	str r1, [r5, r0]
	add r0, r4, #0
	pop {r3, r4, r5, pc}
_0222CBF0:
	mov r0, #6
	lsl r0, r0, #6
	ldr r0, [r5, r0]
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	bl TextPrinterCheckActive
	cmp r0, #0
	bne _0222CC2C
	ldr r0, _0222CC30 ; =gSystem
	ldr r1, [r0, #0x48]
	mov r0, #3
	tst r0, r1
	bne _0222CC14
	bl System_GetTouchNew
	cmp r0, #1
	bne _0222CC24
_0222CC14:
	add r0, r5, #0
	bl ov44_0222F7BC
	mov r0, #0xd2
	mov r1, #0x18
	lsl r0, r0, #2
	str r1, [r5, r0]
	b _0222CC2C
_0222CC24:
	add r0, r5, #0
	add r1, r4, #0
	bl ov44_0222CC34
_0222CC2C:
	add r0, r4, #0
	pop {r3, r4, r5, pc}
	.balign 4, 0
_0222CC30: .word gSystem
	thumb_func_end ov44_0222CB7C

	thumb_func_start ov44_0222CC34
ov44_0222CC34: ; 0x0222CC34
	push {r3, r4, r5, r6, lr}
	sub sp, #0xc
	add r5, r0, #0
	add r6, r1, #0
	bl ov00_021E6EBC
	cmp r0, #0
	bge _0222CC5C
	add r0, r5, #0
	mov r1, #0x13
	mov r2, #0
	bl ov44_0222F510
	mov r0, #0xd2
	mov r1, #0x1b
	lsl r0, r0, #2
	str r1, [r5, r0]
	add sp, #0xc
	add r0, r6, #0
	pop {r3, r4, r5, r6, pc}
_0222CC5C:
	bl ov00_021E6EBC
	add r1, r0, #0
	add r0, r5, #0
	bl ov44_02231958
	add r1, r0, #0
	add r0, r5, #0
	bl ov44_02229F00
	add r4, r0, #0
	add r0, r5, #0
	mov r1, #0x35
	bl ov44_0222C670
	add r0, r5, #0
	bl ov44_0222FBA0
	bl sub_020390C4
	cmp r0, #5
	bne _0222CCA8
	bl ov00_021E6EBC
	add r1, r0, #0
	add r0, r5, #0
	bl ov44_0222F780
	add r0, r5, #0
	mov r1, #0xf
	mov r2, #0
	bl ov44_0222F510
	mov r0, #0xd2
	mov r1, #0x1b
	lsl r0, r0, #2
	str r1, [r5, r0]
	b _0222CD9C
_0222CCA8:
	bl sub_020390C4
	cmp r0, #3
	bne _0222CCD0
	bl ov00_021E6EBC
	add r1, r0, #0
	add r0, r5, #0
	bl ov44_0222F780
	add r0, r5, #0
	mov r1, #0x12
	mov r2, #0
	bl ov44_0222F510
	mov r0, #0xd2
	mov r1, #0x1b
	lsl r0, r0, #2
	str r1, [r5, r0]
	b _0222CD9C
_0222CCD0:
	bl sub_020390C4
	cmp r0, #4
	beq _0222CCE0
	bl sub_02039264
	cmp r0, #0
	beq _0222CD00
_0222CCE0:
	bl ov00_021E6EBC
	add r1, r0, #0
	add r0, r5, #0
	bl ov44_0222F780
	add r0, r5, #0
	mov r1, #0x13
	mov r2, #0
	bl ov44_0222F510
	mov r0, #0xd2
	mov r1, #0x1b
	lsl r0, r0, #2
	str r1, [r5, r0]
	b _0222CD9C
_0222CD00:
	bl sub_020393C8
	cmp r0, #0
	beq _0222CD10
	add r0, r5, #0
	bl ov44_0222B9A0
	b _0222CD9C
_0222CD10:
	cmp r4, #0x10
	beq _0222CD38
	cmp r4, #1
	beq _0222CD38
	bl ov00_021E6EBC
	add r1, r0, #0
	add r0, r5, #0
	bl ov44_0222F780
	add r0, r5, #0
	mov r1, #0x10
	mov r2, #0
	bl ov44_0222F510
	mov r0, #0xd2
	mov r1, #0x1b
	lsl r0, r0, #2
	str r1, [r5, r0]
	b _0222CD9C
_0222CD38:
	ldr r0, _0222CDA4 ; =gSystem
	ldr r1, [r0, #0x48]
	mov r0, #3
	tst r0, r1
	bne _0222CD4A
	bl System_GetTouchNew
	cmp r0, #1
	bne _0222CD5E
_0222CD4A:
	add r0, r5, #0
	mov r1, #0x14
	mov r2, #0
	bl ov44_0222F510
	mov r0, #0xd2
	mov r1, #0x19
	lsl r0, r0, #2
	str r1, [r5, r0]
	b _0222CD9C
_0222CD5E:
	bl ov00_021E7078
	cmp r0, #0
	ldr r1, _0222CDA8 ; =0x00000B0C
	beq _0222CD82
	mov r2, #1
	str r2, [sp]
	mov r3, #2
	str r3, [sp, #4]
	mov r0, #0
	str r0, [sp, #8]
	mov r0, #0x57
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	add r1, r5, r1
	bl ov44_0222ABDC
	b _0222CD9C
_0222CD82:
	mov r2, #1
	str r2, [sp]
	mov r0, #3
	str r0, [sp, #4]
	mov r0, #0
	str r0, [sp, #8]
	mov r0, #0x57
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	add r1, r5, r1
	mov r3, #2
	bl ov44_0222ABDC
_0222CD9C:
	add r0, r6, #0
	add sp, #0xc
	pop {r3, r4, r5, r6, pc}
	nop
_0222CDA4: .word gSystem
_0222CDA8: .word 0x00000B0C
	thumb_func_end ov44_0222CC34

	thumb_func_start ov44_0222CDAC
ov44_0222CDAC: ; 0x0222CDAC
	push {r3, r4, r5, lr}
	add r5, r0, #0
	add r4, r1, #0
	bl sub_020393C8
	cmp r0, #0
	beq _0222CDC4
	add r0, r5, #0
	bl ov44_0222B9A0
	add r0, r4, #0
	pop {r3, r4, r5, pc}
_0222CDC4:
	mov r0, #6
	lsl r0, r0, #6
	ldr r0, [r5, r0]
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	bl TextPrinterCheckActive
	cmp r0, #0
	bne _0222CE30
	bl ov00_021E6EBC
	mov r1, #0
	mvn r1, r1
	cmp r0, r1
	beq _0222CDEE
	bl ov00_021E6EBC
	add r1, r0, #0
	ldr r0, [r5]
	bl sub_0202C4B0
_0222CDEE:
	ldr r0, _0222CE34 ; =0x04000304
	ldrh r1, [r0]
	lsr r0, r0, #0xb
	and r0, r1
	asr r0, r0, #0xf
	cmp r0, #1
	bne _0222CE14
	mov r2, #0x57
	mov r0, #0x35
	lsl r2, r2, #2
	str r0, [sp]
	ldr r0, [r5, r2]
	ldr r1, _0222CE38 ; =ov44_0223538C
	add r2, #0x7d
	mov r3, #0xb
	bl Std_CreateYesNoMenu
	mov r1, #0x61
	b _0222CE24
_0222CE14:
	mov r0, #0x57
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	ldr r1, _0222CE3C ; =ov44_02235374
	mov r2, #0
	bl ov44_02231A14
	mov r1, #0x62
_0222CE24:
	lsl r1, r1, #2
	str r0, [r5, r1]
	mov r0, #0xd2
	mov r1, #0x1a
	lsl r0, r0, #2
	str r1, [r5, r0]
_0222CE30:
	add r0, r4, #0
	pop {r3, r4, r5, pc}
	.balign 4, 0
_0222CE34: .word 0x04000304
_0222CE38: .word ov44_0223538C
_0222CE3C: .word ov44_02235374
	thumb_func_end ov44_0222CDAC




	.rodata
.public _0223535C
_0223535C:
	.byte 0x01, 0x03, 0x05, 0x00

.public ov44_02235360
ov44_02235360: ; 0x02235360
	.byte 0x00, 0x02, 0x04, 0x00

.public ov44_02235364
ov44_02235364: ; 0x02235364
	.byte 0x08, 0x07, 0x05, 0x07

.public ov44_02235368
ov44_02235368: ; 0x02235368
	.byte 0x00, 0x01, 0x02, 0x01

.public ov44_0223536C
ov44_0223536C: ; 0x0223536C
	.byte 0xF8, 0xF7, 0xF6, 0xF6
	.byte 0xF7, 0x00, 0x00, 0x00

.public ov44_02235374
ov44_02235374: ; 0x02235374
	.byte 0x02, 0x19, 0x0A, 0x06, 0x08, 0x0E, 0x51, 0x00

.public ov44_0223537C
ov44_0223537C: ; 0x0223537C
	.byte 0x02, 0x19, 0x0A, 0x06
	.byte 0x08, 0x0E, 0x51, 0x00

.public ov44_02235384
ov44_02235384: ; 0x02235384
	.byte 0x01, 0x19, 0x10, 0x06, 0x08, 0x0E, 0x89, 0x01

.public ov44_0223538C
ov44_0223538C: ; 0x0223538C
	.byte 0x02, 0x19, 0x0D, 0x06
	.byte 0x04, 0x0D, 0x51, 0x00

.public ov44_02235394
ov44_02235394: ; 0x02235394
	.byte 0xA0, 0xC0, 0x08, 0x40, 0xA0, 0xC0, 0x50, 0xB0, 0xA0, 0xC0, 0xC0, 0xF8

.public ov44_022353A0
ov44_022353A0: ; 0x022353A0
	.byte 0x27, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x29, 0x00, 0x00, 0x00, 0xFE, 0xFF, 0xFF, 0xFF

ov44_022353B0: ; 0x022353B0
	.byte 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

ov44_022353C0: ; 0x022353C0
	.byte 0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x40, 0x00, 0x00, 0x35, 0x00, 0x00, 0x00

.public ov44_022353D0
ov44_022353D0: ; 0x022353D0
	.byte 0x64, 0x00, 0x00, 0x00, 0x66, 0x00, 0x00, 0x00, 0x68, 0x00, 0x00, 0x00, 0x6C, 0x00, 0x00, 0x00
	.byte 0x6A, 0x00, 0x00, 0x00, 0x6E, 0x00, 0x00, 0x00

ov44_022353E8: ; 0x022353E8
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00

ov44_02235404: ; 0x02235404
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x1A, 0x04, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

ov44_02235420: ; 0x02235420
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x01, 0x00, 0x1E, 0x06, 0x01, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

ov44_0223543C: ; 0x0223543C
	.byte 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x1C, 0x00
	.byte 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

ov44_02235458: ; 0x02235458
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x1D, 0x02, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00

ov44_02235474: ; 0x02235474
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x1E, 0x04, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

ov44_02235490: ; 0x02235490
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x01, 0x00, 0x1B, 0x02, 0x01, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

ov44_022354AC: ; 0x022354AC
	.byte 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x1F, 0x06
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

.public ov44_022354C8
ov44_022354C8: ; 0x022354C8
	.byte 0x00, 0x00, 0x00, 0x00
	.word ov44_0222C288
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x02, 0x00, 0x00, 0x08, 0x00, 0x10
	.byte 0x2F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

.public ov44_022354E8
ov44_022354E8: ; 0x022354E8
	.byte 0x00, 0x2F, 0x00, 0x77, 0x30, 0x5F, 0x00, 0x77
	.byte 0x60, 0x8F, 0x00, 0x77, 0x90, 0xBF, 0x00, 0x77, 0x00, 0x2F, 0x80, 0xFF, 0x30, 0x5F, 0x80, 0xFF
	.byte 0x60, 0x8F, 0x80, 0xFF, 0x90, 0xBF, 0x80, 0xFF

.public ov44_02235508
ov44_02235508: ; 0x02235508
	.byte 0x00, 0x00, 0x00, 0x00
	.word ov44_0222C288
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x04, 0x00, 0x00, 0x08, 0x00, 0x10
	.byte 0x2F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

.public ov44_02235528
ov44_02235528: ; 0x02235528
	.byte 0x00, 0x00, 0x00, 0x00
	.word ov44_0222C288
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x04, 0x00, 0x00, 0x08, 0x00, 0x10
	.byte 0x2F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

ov44_02235548: ; 0x02235548
	.byte 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

.public ov44_02235570
ov44_02235570: ; 0x02235570
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x02, 0x00, 0x00, 0xC0, 0x1A, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0xC0, 0x1A, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0E, 0x00, 0x00, 0xC0, 0x1A, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

.public ov44_02235600
ov44_02235600: ; 0x02235600
	.byte 0x03, 0x00, 0x02, 0x00

.public ov44_02235604 
ov44_02235604: ; 0x02235604
	.byte 0x00, 0x00, 0x00, 0x1C, 0x00, 0x00, 0x03, 0x00, 0x58, 0x21, 0x00

.public ov44_0223560F
ov44_0223560F: ; 0x0223560F
	.byte 0x00

.public ov44_02235610 
ov44_02235610: ; 0x02235610
	.byte 0x00

.public ov44_02235611 
ov44_02235611: ; 0x02235611
	.byte 0x00

.public ov44_02235612
ov44_02235612: ; 0x02235612
	.byte 0x01, 0x01, 0x02, 0x01, 0x00, 0x01, 0x04, 0x01, 0x01, 0x01, 0x02, 0x01, 0x00, 0x00
	.byte 0x00, 0x00, 0x01, 0x01, 0x02, 0x01, 0x01, 0x01, 0x04, 0x01, 0x00, 0x01, 0x03, 0x01, 0x01, 0x00

.public ov44_02235630
ov44_02235630: ; 0x02235630
	.byte 0x10, 0x00, 0x0B, 0x00, 0x10, 0x00, 0x0B, 0x00, 0x16, 0x00, 0x0B, 0x00, 0x1C, 0x00, 0x0B, 0x00

ov44_02235640:
	.byte 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00
	.byte 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00
	.byte 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00
	.byte 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00
	.byte 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00
	.byte 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00
	.byte 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00
	.byte 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00
	.byte 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x16, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00
	.byte 0x06, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00
	.byte 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00
	.byte 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00
	.byte 0x06, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00
	.byte 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x1A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00
	.byte 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00
	.byte 0x06, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00
	.byte 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00
	.byte 0x06, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00
	.byte 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1E, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00

ov44_02235900:
	.byte 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00
	.byte 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00
	.byte 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00
	.byte 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00
	.byte 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00
	.byte 0x05, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00
	.byte 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00
	.byte 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00
	.byte 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x16, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x0E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x16, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00
	.byte 0x06, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x08, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00
	.byte 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00
	.byte 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00
	.byte 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00
	.byte 0x06, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x08, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00
	.byte 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00
	.byte 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x1A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00
	.byte 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00
	.byte 0x06, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x08, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00
	.byte 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00
	.byte 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x1C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00
	.byte 0x06, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x08, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00
	.byte 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00
	.byte 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1E, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00

ov44_02235BC0:
	.byte 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00
	.byte 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00
	.byte 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00
	.byte 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00
	.byte 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00
	.byte 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00
	.byte 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00
	.byte 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00
	.byte 0x05, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00
	.byte 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00
	.byte 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00
	.byte 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x16, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x0E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x16, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x16, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x0A, 0x00, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x00
	.byte 0x0A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00
	.byte 0x06, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x08, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00
	.byte 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00
	.byte 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x0A, 0x00, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x00
	.byte 0x0A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00
	.byte 0x06, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x08, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00
	.byte 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00
	.byte 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x1A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x0A, 0x00, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x00
	.byte 0x0A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00
	.byte 0x06, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x08, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00
	.byte 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00
	.byte 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x1C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x0A, 0x00, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x00
	.byte 0x0A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00
	.byte 0x06, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x08, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00
	.byte 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00
	.byte 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00

ov44_02235F88:
	.byte 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00
	.byte 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00
	.byte 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00
	.byte 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00
	.byte 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00
	.byte 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00
	.byte 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00
	.byte 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00
	.byte 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00
	.byte 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00
	.byte 0x05, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00
	.byte 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00
	.byte 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00
	.byte 0x05, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00
	.byte 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x0E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x16, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x16, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x0E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x16, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x16, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x0A, 0x00, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x00
	.byte 0x0A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00
	.byte 0x06, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x08, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00
	.byte 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00
	.byte 0x0C, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x0A, 0x00, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x00
	.byte 0x0A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00
	.byte 0x06, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x08, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00
	.byte 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00
	.byte 0x0C, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x1A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x1A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x0A, 0x00, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x00
	.byte 0x0A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00
	.byte 0x06, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x08, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00
	.byte 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00
	.byte 0x0C, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x1C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x1C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x0A, 0x00, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x00
	.byte 0x0A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00
	.byte 0x06, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x08, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00
	.byte 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00
	.byte 0x0C, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1E, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00

.public ov44_02236458
ov44_02236458: ; 0x02236458
	.byte 0x08, 0x00, 0x04, 0x0C

.public ov44_0223645C 
ov44_0223645C: ; 0x0223645C
	.byte 0x02, 0x19, 0x0D, 0x05
	.byte 0x04, 0x01, 0x39, 0x00

.public ov44_02236464
ov44_02236464: ; 0x02236464
	.word ov44_02234324
	.word ov44_02234328
	.word ov44_0223435C
	.word ov44_02234388

.public ov44_02236474
ov44_02236474: ; 0x02236474
	.byte 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00

.public ov44_02236484
ov44_02236484: ; 0x02236484
	.byte 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x40, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00

.public ov44_02236494
ov44_02236494: ; 0x02236494
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x1A, 0x04, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

.public ov44_022364B0
ov44_022364B0: ; 0x022364B0
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x01, 0x00, 0x1C, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

.public ov44_022364CC
ov44_022364CC: ; 0x022364CC
	.byte 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x1B, 0x02
	.byte 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

.public ov44_022364E8
ov44_022364E8: ; 0x022364E8
	.byte 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

.public ov44_02236510
ov44_02236510: ; 0x02236510
	.byte 0xD0, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0xE8, 0x00, 0x48, 0x00
	.byte 0x01, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0xD0, 0x00, 0x60, 0x00, 0x02, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0xB8, 0x00, 0x48, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00

.public ov44_02236540
ov44_02236540: ; 0x02236540
	.word ov44_02234BF0
	.word ov44_02234C10
	.word Wifi_PromptAwaitingResponse
	.word ov44_02234C88
	.word ov44_02234CE8
	.word ov44_02234D28
	.word ov44_02234D4C
	.word ov44_02234D88
	.word Wifi_PromptAwaitingMembers
	.word ov44_02234DE4
	.word ov44_02234E08
	.word ov44_02234EA4
	.word ov44_02234EF4
	.word ov44_02234F44
	.word ov44_02234F60
	.word ov44_02234F88
	.word Wifi_PromptMemberDrop
	.word ov44_02234FDC
	.word Wifi_PromptToggleVoiceChat_
	.word ov44_02235038
	.word ov44_02235090
	.word ov44_02235100
	.word ov44_02235158
	.word ov44_0223518C
	.word ov44_022351BC
	.word ov44_022351DC
	.word ov44_02235218
	.word ov44_02235268
	.word ov44_0223532C
	.word ov44_02235340

.public ov44_022365B8
ov44_022365B8: ; 0x022365B8
	.word ov44_02234474
	.word Wifi_PromptReadyMessage
	.word ov44_022344C4
	.word Wifi_PromptUserJoinRequest
	.word ov44_022345C8
	.word ov44_022345FC
	.word Wifi_PromptInsufficientMembers
	.word ov44_022346B4
	.word Wifi_PromptConfirmMembers
	.word ov44_022346E8
	.word ov44_0223471C
	.word ov44_02234764
	.word ov44_0223479C
	.word ov44_022347D4
	.word ov44_022347FC
	.word ov44_02234828
	.word ov44_02234858
	.word ov44_022348A8
	.word ov44_022348C4
	.word Wifi_PromptStopFindingMembers
	.word ov44_02234904
	.word ov44_02234944
	.word Wifi_PromptDropAsLeader
	.word ov44_022349B4
	.word ov44_022349F4
	.word Wifi_PromptCancelInvites
	.word ov44_02234A68
	.word ov44_02234AA8
	.word Wifi_PromptMemberDropped
	.word ov44_02234B18
	.word Wifi_PromptToggleVoiceChat
	.word ov44_02234B80
	.word ov44_02234BB4

.public ov44_0223663C
ov44_0223663C: ; 0x0223663C
	.byte 0x05, 0x04, 0x03, 0x02
	.byte 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x05, 0x05, 0x05, 0x00, 0x00

	.data

.public _02236660
_02236660:
	.byte 0x1E, 0x00, 0x00, 0x00, 0x13, 0x00, 0x00, 0x00, 0x22, 0x00, 0x00, 0x00, 0x1D, 0x00, 0x00, 0x00
	.byte 0x29, 0x00, 0x00, 0x00, 0xFE, 0xFF, 0xFF, 0xFF, 0x20, 0x00, 0x00, 0x00, 0x15, 0x00, 0x00, 0x00

.public ov44_02236680
ov44_02236680: ; 0x02236680
	.word ov44_02230300
	.word ov44_02230E5C
	.word ov44_022307E0
	.word ov44_022308B0
	.word ov44_02230B2C
	.word ov44_02230C68
	.word ov44_02230D8C

.public ov44_0223669C
ov44_0223669C: ; 0x0223669C
	.byte 0x47, 0x00, 0x00, 0x00
	.byte 0x17, 0x00, 0x00, 0x00, 0x47, 0x00, 0x00, 0x00, 0x19, 0x00, 0x00, 0x00, 0x47, 0x00, 0x00, 0x00
	.byte 0x1B, 0x00, 0x00, 0x00, 0x29, 0x00, 0x00, 0x00, 0xFE, 0xFF, 0xFF, 0xFF

.public ov44_022366BC
ov44_022366BC: ; 0x022366BC
	.byte 0x5F, 0x00, 0x00, 0x00
	.byte 0x0B, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x61, 0x00, 0x00, 0x00
	.byte 0x0A, 0x00, 0x00, 0x00, 0x29, 0x00, 0x00, 0x00, 0xFE, 0xFF, 0xFF, 0xFF

.public ov44_022366DC
ov44_022366DC: ; 0x022366DC
	.byte 0x62, 0x00, 0x00, 0x00
	.byte 0x0E, 0x00, 0x00, 0x00, 0x63, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x64, 0x00, 0x00, 0x00
	.byte 0x0D, 0x00, 0x00, 0x00, 0x29, 0x00, 0x00, 0x00, 0xFE, 0xFF, 0xFF, 0xFF

.public ov44_022366FC
ov44_022366FC: ; 0x022366FC
	.byte 0x00, 0x00, 0x00, 0x00
	.word ov44_0222C288
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x03, 0x00
	.byte 0x00, 0x08, 0x00, 0x10, 0x2F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

.public ov44_0223671C
ov44_0223671C: ; 0x0223671C
	.byte 0x5D, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x5E, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x1C, 0x00, 0x00, 0x00
	.byte 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

ov44_0223676C: ; 0x0223676C
	.word ov44_0222B228
	.word ov44_0222B36C
	.word ov44_0222B3A8
	.word ov44_0222B42C
	.word ov44_0222B494
	.word ov44_0222B528
	.word ov44_0222B64C
	.word ov44_0222BA6C
	.word ov44_0222BAB8
	.word ov44_0222BAC4
	.word ov44_0222BAD0
	.word ov44_0222BB38
	.word ov44_0222BBA4
	.word ov44_0222BC78
	.word ov44_0222B744
	.word ov44_0222BE3C
	.word ov44_0222C060
	.word ov44_0222C084
	.word ov44_0222C35C
	.word ov44_0222C684
	.word ov44_0222C9AC
	.word ov44_0222C9F4
	.word ov44_0222CB34
	.word ov44_0222CB7C
	.word ov44_0222CC34
	.word ov44_0222CDAC
	.word ov44_0222CE40
	.word ov44_0222CFE0
	.word ov44_0222D0A4
	.word ov44_0222D10C
	.word ov44_0222D1BC
	.word ov44_0222D1C0
	.word ov44_0222D1C4
	.word ov44_0222EB90
	.word ov44_0222F4E0
	.word ov44_0222D1C8
	.word ov44_0222D3DC
	.word ov44_0222D594
	.word ov44_0222DA64
	.word ov44_0222D214
	.word ov44_0222D23C
	.word ov44_0222D2B0
	.word ov44_0222DC18
	.word ov44_0222DD64
	.word ov44_0222E090
	.word ov44_0222E45C
	.word ov44_0222E62C
	.word ov44_0222E630
	.word ov44_0222E634
	.word ov44_0222E860
	.word ov44_0222E908
	.word ov44_0222E948
	.word ov44_0222E9C4
	.word ov44_0222EA2C
	.word ov44_0222EBB8
	.word ov44_0222EBC4
	.word ov44_0222EC14
	.word ov44_0222EC2C
	.word ov44_0222EC98
	.word ov44_0222EDB8
	.word ov44_0222EE10
	.word ov44_0222EE54
	.word ov44_0222EED4
	.word ov44_0222F0AC
	.word ov44_0222F194
	.word ov44_0222BFF0
	.word ov44_0222E5D8
	.word ov44_0222E5DC
	.word ov44_0222D824
	.word ov44_0222BEE0
	.word ov44_0222E5E0
	.word ov44_0222BF6C

.public ov44_0223688C 
ov44_0223688C: ; 0x0223688C
	.word ov44_02235640
	.word ov44_02235900
	.word ov44_02235BC0
	.word ov44_02235F88

.public ov44_0223689C
ov44_0223689C: ; 0x0223689C
	.byte 0x7E, 0x7F, 0x7F, 0x7F

	.bss

_022368A0:
	.space 0x20
