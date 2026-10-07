	.extern PokeathlonCourse_AllocPtr4FromHeap
	.extern PokeathlonCourse_FreePtr4HeapAlloc
	.extern PokeathlonCourse_GetCurrentParticipantIndex
	.extern PokeathlonCourse_GetDataCopyArea
	.extern PokeathlonCourse_GetField1ED
	.extern PokeathlonCourse_GetField1EF
	.extern PokeathlonCourse_GetField3A4
	.extern PokeathlonCourse_GetField3D8_AtIndex
	.extern PokeathlonCourse_GetField3D8_ForCurrentParticipant
	.extern PokeathlonCourse_GetField5F0_AtIndex
	.extern PokeathlonCourse_GetField974_AtIndex
	.extern PokeathlonCourse_GetFieldBA4
	.extern PokeathlonCourse_GetFieldData
	.extern PokeathlonCourse_GetFieldData_AtIndex
	.extern PokeathlonCourse_GetGraphicsSystem
	.extern PokeathlonCourse_GetHeapAllocPtr4
	.extern PokeathlonCourse_GetHeapID
	.extern PokeathlonCourse_GetMode
	.extern PokeathlonCourse_GetParticipantCount
	.extern PokeathlonCourse_GetParticipantData
	.extern PokeathlonCourse_GetParticipantUnk04
	.extern PokeathlonCourse_GetPlayerProfile
	.extern PokeathlonCourse_GetPlayerProfileFromData
	.extern PokeathlonCourse_GetSaveData
	.extern PokeathlonCourse_GetSystem
	.extern PokeathlonCourse_GetUnkConstant4
	.extern PokeathlonCourse_IncrementField1ED
	.extern PokeathlonCourse_IncrementField1EF
	.extern PokeathlonCourse_InitPlayerProfiles
	.extern PokeathlonCourse_InitStateInfo
	.extern PokeathlonCourse_ResetDataCopyArea
	.extern PokeathlonCourse_ResetField1EF
	.extern PokeathlonCourse_ResetField3A4
	.extern PokeathlonCourse_RunSubStateLoop
	.extern PokeathlonCourse_SetField1ED
	.extern PokeathlonCourse_SetField1F4
	.extern PokeathlonCourse_SetField3A4
	.extern PokeathlonCourse_SetField5E0_AtIndex
	.extern PokeathlonCourse_SetStateField07
	.extern PokeathlonCourse_SetStateField07_IfDifferent
	.extern PokeathlonCourse_SetStateTransitionType
	.extern PokeathlonCourse_SetVBlankIntrCB
	.extern ov96_021E5E04
	.extern ov96_021E5E7C
	.extern ov96_021E5F24
	.extern ov96_021E60C0
	.extern ov96_021E8A20
	.extern ov96_021E8BAC
	.extern ov96_021EAA20
	.extern ov96_021EABE0
	.extern ov96_021EB5B8
	.include "asm/macros.inc"
	.include "overlay_96.inc"
	.include "global.inc"
	.include "overlay_96_exact_bank.inc"

.text
	thumb_func_start ov96_021F1614
ov96_021F1614: ; 0x021F1614
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x138
	str r0, [sp, #0xc]
	bl PokeathlonCourse_GetHeapAllocPtr4
	add r6, r0, #0
	ldr r0, [sp, #0xc]
	bl PokeathlonCourse_GetDataCopyArea
	add r0, #0xf0
	bl ov96_021E8A20
	str r0, [sp, #0x4c]
	ldr r0, [sp, #0xc]
	bl ov96_021E5F24
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	str r0, [sp, #0x2c]
	ldr r0, [sp, #0x4c]
	mov r1, #0xf
	ldr r2, [r0, #0x1c]
	asr r0, r2, #0x1c
	and r0, r1
	str r0, [sp, #0x64]
	asr r0, r2, #0x18
	add r5, r0, #0
	ldr r0, [sp, #0x4c]
	and r5, r1
	ldr r0, [r0, #0x18]
	asr r0, r0, #0x18
	lsl r0, r0, #0x18
	lsr r4, r0, #0x18
	ldr r0, [sp, #0x4c]
	ldr r2, [r0, #0x20]
	mov r0, #1
	asr r3, r2, #0xd
	and r3, r0
	lsl r3, r3, #0x18
	lsr r3, r3, #0x18
	str r3, [sp, #0x60]
	asr r3, r2, #0xe
	and r0, r3
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	str r0, [sp, #0x5c]
	asr r3, r2, #0xf
	mov r0, #7
	and r0, r3
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	str r0, [sp, #0x28]
	asr r3, r2, #0x1a
	mov r0, #3
	and r0, r3
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	str r0, [sp, #0x58]
	asr r0, r2, #0x12
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	str r0, [sp, #0x54]
	asr r0, r2, #0x1c
	and r0, r1
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	add r0, r0, #1
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	str r0, [sp, #0x50]
	ldr r0, [sp, #0x5c]
	cmp r0, #0
	bne _021F16B4
	ldr r0, [sp, #0xc]
	bl PokeathlonCourse_GetDataCopyArea
	bl ov96_021E8A20
	mov r1, #0
	strb r1, [r0, #8]
_021F16B4:
	mov r0, #0x73
	lsl r0, r0, #4
	ldrh r1, [r6, r0]
	lsl r2, r5, #0x18
	lsr r7, r2, #0x18
	lsl r1, r1, #0x18
	lsr r1, r1, #0x18
	cmp r1, r7
	beq _021F16E4
	mov r5, #0
	strh r7, [r6, r0]
	add r3, r5, #0
	add r0, #0xc
_021F16CE:
	add r2, r6, r5
	add r5, r5, #1
	strb r3, [r2, r0]
	cmp r5, #0xc
	blt _021F16CE
	ldr r0, _021F19E8 ; =0x000007AC
	ldr r3, [sp, #0xc]
	add r0, r6, r0
	add r2, r7, #0
	bl ov96_021F2B68
_021F16E4:
	cmp r4, #0
	bne _021F170A
	ldr r0, _021F19EC ; =0x00000738
	mov r1, #1
	ldr r3, [r6, r0]
	ldr r0, [r6]
	add r2, r3, #0
	ldr r3, [r3, #8]
	add r2, #0xc
	bl BG_LoadScreenTilemapData
	ldr r0, [r6]
	mov r1, #1
	bl ScheduleBgTilemapBufferTransfer
	ldr r0, _021F19F0 ; =0x000007A8
	mov r1, #0
	str r1, [r6, r0]
	b _021F1796
_021F170A:
	cmp r4, #0x2b
	bhs _021F1766
	ldr r0, _021F19F0 ; =0x000007A8
	ldr r1, [r6, r0]
	cmp r1, #0
	bne _021F175C
	ldr r1, [sp, #0x50]
	cmp r1, #1
	bhi _021F172E
	cmp r4, #0x20
	blo _021F172E
	sub r0, #0xd1
	bl PlaySE
	ldr r0, _021F19F0 ; =0x000007A8
	mov r1, #1
	str r1, [r6, r0]
	b _021F175C
_021F172E:
	ldr r0, [sp, #0x50]
	cmp r0, #1
	bls _021F1746
	cmp r4, #0x1c
	blo _021F1746
	ldr r0, _021F19F4 ; =0x000006D7
	bl PlaySE
	ldr r0, _021F19F0 ; =0x000007A8
	mov r1, #1
	str r1, [r6, r0]
	b _021F175C
_021F1746:
	ldr r0, [sp, #0x50]
	cmp r0, #3
	bls _021F175C
	cmp r4, #0x15
	blo _021F175C
	ldr r0, _021F19F4 ; =0x000006D7
	bl PlaySE
	ldr r0, _021F19F0 ; =0x000007A8
	mov r1, #1
	str r1, [r6, r0]
_021F175C:
	ldr r1, [r6]
	add r0, r4, #0
	bl ov96_021F295C
	b _021F1796
_021F1766:
	ldr r3, _021F19F8 ; =ov96_0221BC8C
	add r2, sp, #0x11c
	mov r1, #8
_021F176C:
	ldrh r0, [r3]
	add r3, r3, #2
	strh r0, [r2]
	add r2, r2, #2
	sub r1, r1, #1
	bne _021F176C
	mov r1, #2
	str r1, [sp]
	mov r0, #4
	str r0, [sp, #4]
	str r1, [sp, #8]
	ldr r0, [r6]
	mov r1, #1
	add r2, sp, #0x11c
	mov r3, #0xe
	bl LoadRectToBgTilemapRect
	ldr r1, [r6]
	mov r0, #0x2a
	bl ov96_021F295C
_021F1796:
	ldr r0, [sp, #0x60]
	cmp r0, #0
	beq _021F17A2
	ldr r0, _021F19FC ; =0x000008CF
	bl PlaySE
_021F17A2:
	ldr r0, _021F1A00 ; =0x00000774
	ldr r1, [sp, #0x50]
	ldr r0, [r6, r0]
	bl ov96_021F4364
	add r0, sp, #0x12c
	ldr r3, _021F1A04 ; =ov96_0221BC9C
	str r0, [sp, #0x30]
	add r2, sp, #0xa4
	ldmia r3!, {r0, r1}
	stmia r2!, {r0, r1}
	ldmia r3!, {r0, r1}
	stmia r2!, {r0, r1}
	ldmia r3!, {r0, r1}
	stmia r2!, {r0, r1}
	ldr r3, _021F1A08 ; =ov96_0221BCB4
	mov r5, #0
	add r2, sp, #0x8c
	ldmia r3!, {r0, r1}
	stmia r2!, {r0, r1}
	ldmia r3!, {r0, r1}
	stmia r2!, {r0, r1}
	ldmia r3!, {r0, r1}
	stmia r2!, {r0, r1}
	add r0, r6, #0
	str r0, [sp, #0x68]
	add r0, #0x20
	str r5, [sp, #0x34]
	add r7, r6, #0
	str r0, [sp, #0x68]
_021F17DE:
	add r0, r5, #0
	mov r1, #3
	bl _s32_div_f
	str r1, [sp, #0x14]
	add r0, r5, #0
	mov r1, #3
	bl _s32_div_f
	mov r1, #0x1b
	lsl r1, r1, #4
	str r0, [sp, #0x24]
	mul r1, r0
	ldr r0, [sp, #0x68]
	ldr r2, [sp, #0x14]
	add r0, r0, r1
	mov r1, #0x90
	mul r1, r2
	add r4, r0, r1
	ldr r0, [r0, r1]
	bl ov96_021EAA20
	bl ov96_021E8BAC
	str r0, [sp, #0x3c]
	ldr r0, [sp, #0x4c]
	ldr r1, [r0, #0x1c]
	ldr r0, [sp, #0x34]
	asr r1, r0
	mov r0, #3
	and r0, r1
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	str r0, [sp, #0x44]
	ldr r0, [sp, #0x4c]
	ldr r0, [r0, #0x20]
	add r1, r0, #0
	asr r1, r5
	mov r0, #1
	and r0, r1
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	str r0, [sp, #0x40]
	ldr r0, [sp, #0x44]
	cmp r0, #0
	beq _021F183C
	b _021F194A
_021F183C:
	mov r0, #0
	str r0, [sp, #0x38]
	ldr r0, _021F1A0C ; =0x00000778
	ldrh r0, [r7, r0]
	cmp r0, #0
	bne _021F1850
	ldr r0, _021F1A0C ; =0x00000778
	mov r1, #1
	strh r1, [r7, r0]
	b _021F191A
_021F1850:
	cmp r0, #1
	bne _021F1912
	add r2, sp, #0xa4
	add r3, sp, #0xf8
	mov ip, r3
	ldmia r2!, {r0, r1}
	stmia r3!, {r0, r1}
	ldmia r2!, {r0, r1}
	stmia r3!, {r0, r1}
	ldmia r2!, {r0, r1}
	stmia r3!, {r0, r1}
	add r2, sp, #0x8c
	add r3, sp, #0xe0
	ldmia r2!, {r0, r1}
	stmia r3!, {r0, r1}
	ldmia r2!, {r0, r1}
	stmia r3!, {r0, r1}
	ldmia r2!, {r0, r1}
	stmia r3!, {r0, r1}
	ldr r1, _021F1A10 ; =0x0000077A
	mov r2, ip
	ldrh r1, [r7, r1]
	ldr r0, _021F1A14 ; =0x45800000
	lsr r1, r1, #1
	lsl r1, r1, #2
	ldr r1, [r2, r1]
	bl _fmul
	bl _ffix
	str r0, [sp, #0x110]
	ldr r1, _021F1A10 ; =0x0000077A
	ldr r0, _021F1A14 ; =0x45800000
	ldrh r1, [r7, r1]
	lsr r1, r1, #1
	lsl r2, r1, #2
	add r1, sp, #0xf8
	ldr r1, [r1, r2]
	bl _fmul
	bl _ffix
	str r0, [sp, #0x114]
	ldr r1, _021F1A10 ; =0x0000077A
	ldr r0, _021F1A14 ; =0x45800000
	ldrh r1, [r7, r1]
	lsr r1, r1, #1
	lsl r2, r1, #2
	add r1, sp, #0xf8
	ldr r1, [r1, r2]
	bl _fmul
	bl _ffix
	str r0, [sp, #0x118]
	ldr r0, [sp, #0x4c]
	ldr r1, [sp, #0x4c]
	add r2, r0, r5
	add r0, sp, #0x84
	str r0, [sp]
	ldrb r1, [r1, r5]
	ldrb r2, [r2, #0xc]
	ldr r0, [r4]
	add r3, sp, #0x88
	bl ov96_021EB06C
	ldr r0, [sp, #0x84]
	cmp r0, #0xb0
	blt _021F18E8
	ldr r0, _021F1A10 ; =0x0000077A
	ldrh r0, [r7, r0]
	lsr r0, r0, #1
	lsl r1, r0, #2
	add r0, sp, #0xe0
	ldr r0, [r0, r1]
	str r0, [sp, #0x38]
_021F18E8:
	ldr r0, _021F1A10 ; =0x0000077A
	ldrh r0, [r7, r0]
	add r1, r0, #1
	ldr r0, _021F1A10 ; =0x0000077A
	strh r1, [r7, r0]
	ldr r0, [r4]
	mov r1, #1
	bl ov96_021EABE0
	ldr r0, [sp, #0x3c]
	add r1, sp, #0x110
	bl Sprite_SetAffineScale
	ldr r0, _021F1A10 ; =0x0000077A
	ldrh r0, [r7, r0]
	cmp r0, #0xc
	blo _021F191A
	ldr r0, _021F1A0C ; =0x00000778
	mov r1, #2
	strh r1, [r7, r0]
	b _021F191A
_021F1912:
	ldr r0, [r4]
	mov r1, #0
	bl ov96_021EAB38
_021F191A:
	ldr r0, [r4, #8]
	mov r1, #1
	mov r2, #0
	bl ov96_021EB52C
	ldr r0, [r4]
	mov r1, #0
	bl ov96_021EAB74
	ldr r0, [sp, #0x4c]
	ldr r1, [sp, #0x4c]
	add r2, r0, r5
	ldrb r3, [r2, #0xc]
	ldr r2, [sp, #0x38]
	ldrb r1, [r1, r5]
	add r2, r2, r3
	ldr r0, [r4]
	mov r3, #1
	bl ov96_021EB01C
	ldr r0, [sp, #0x30]
	mov r1, #1
	strb r1, [r0]
	b _021F1C52
_021F194A:
	ldr r0, _021F1A0C ; =0x00000778
	mov r1, #0
	strh r1, [r7, r0]
	add r0, r0, #2
	strh r1, [r7, r0]
	mov r0, #1
	lsl r0, r0, #0xc
	str r0, [sp, #0xd4]
	str r0, [sp, #0xd8]
	str r0, [sp, #0xdc]
	ldr r0, [r4]
	bl ov96_021EABE0
	ldr r0, [sp, #0x40]
	cmp r0, #0
	ldr r0, [r4, #8]
	beq _021F1992
	mov r1, #1
	add r2, r1, #0
	bl ov96_021EB52C
	ldr r1, [sp, #0x24]
	ldr r0, [sp, #0x2c]
	cmp r1, r0
	bne _021F199A
	mov r0, #0x89
	lsl r0, r0, #4
	bl IsSEPlaying
	cmp r0, #0
	bne _021F199A
	mov r0, #0x89
	lsl r0, r0, #4
	bl PlaySE
	b _021F199A
_021F1992:
	mov r1, #1
	mov r2, #0
	bl ov96_021EB52C
_021F199A:
	ldr r0, [sp, #0x3c]
	add r1, sp, #0xd4
	bl Sprite_SetAffineScale
	ldr r0, [r4]
	mov r1, #1
	bl ov96_021EAB38
	ldr r0, [sp, #0x44]
	cmp r0, #2
	bne _021F19D4
	ldr r1, _021F1A18 ; =0x0000073C
	add r0, r6, r5
	ldrb r1, [r0, r1]
	cmp r1, #0
	bne _021F19D4
	ldr r1, _021F1A18 ; =0x0000073C
	mov r2, #1
	strb r2, [r0, r1]
	add r0, r1, #0
	add r0, #0x30
	lsl r1, r5, #0x18
	add r3, r4, #0
	ldr r0, [r6, r0]
	lsr r1, r1, #0x18
	add r3, #0x68
	bl ov96_021EAF60
	b _021F1A34
_021F19D4:
	ldr r0, [sp, #0x44]
	cmp r0, #1
	bne _021F1A34
	ldr r1, _021F1A18 ; =0x0000073C
	add r0, r6, r5
	ldrb r1, [r0, r1]
	cmp r1, #1
	bne _021F1A34
	ldr r1, _021F1A18 ; =0x0000073C
	b _021F1A1C
	.balign 4, 0
_021F19E8: .word 0x000007AC
_021F19EC: .word 0x00000738
_021F19F0: .word 0x000007A8
_021F19F4: .word 0x000006D7
_021F19F8: .word ov96_0221BC8C
_021F19FC: .word 0x000008CF
_021F1A00: .word 0x00000774
_021F1A04: .word ov96_0221BC9C
_021F1A08: .word ov96_0221BCB4
_021F1A0C: .word 0x00000778
_021F1A10: .word 0x0000077A
_021F1A14: .word 0x45800000
_021F1A18: .word 0x0000073C
_021F1A1C:
	mov r2, #0
	strb r2, [r0, r1]
	add r0, r1, #0
	add r0, #0x30
	lsl r1, r5, #0x18
	add r3, r4, #0
	ldr r0, [r6, r0]
	lsr r1, r1, #0x18
	mov r2, #1
	add r3, #0x48
	bl ov96_021EAF60
_021F1A34:
	ldr r0, [sp, #0x30]
	mov r1, #0
	strb r1, [r0]
	ldr r0, [sp, #0x4c]
	add r1, r0, r5
	ldrb r0, [r0, r5]
	str r0, [sp, #0x18]
	ldrb r0, [r1, #0xc]
	str r0, [sp, #0x1c]
	ldr r0, [sp, #0x4c]
	ldr r1, [r0, #0x18]
	ldr r0, [sp, #0x34]
	asr r1, r0
	mov r0, #3
	and r0, r1
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	str r0, [sp, #0x48]
	ldr r0, [sp, #0x18]
	mov r1, #0
	lsl r0, r0, #0xc
	str r0, [r4, #0xc]
	ldr r0, [sp, #0x1c]
	lsl r0, r0, #0xc
	str r0, [r4, #0x10]
	add r0, sp, #0xc8
	str r1, [r0]
	str r1, [r0, #4]
	str r1, [r0, #8]
	ldr r0, [r4, #0xc]
	str r0, [sp, #0xc8]
	ldr r0, [r4, #0x10]
	str r0, [sp, #0xcc]
	ldr r0, [r4, #4]
	bl ov96_021EB5B8
	add r1, sp, #0xc8
	bl Sprite_SetMatrix
	mov r1, #0
	add r0, sp, #0xbc
	str r1, [r0]
	str r1, [r0, #4]
	str r1, [r0, #8]
	ldr r0, [r4, #0xc]
	mov r1, #3
	str r0, [sp, #0xbc]
	ldr r0, [r4, #0x10]
	str r0, [sp, #0xc0]
	add r0, r5, #0
	bl _s32_div_f
	str r1, [sp, #0x10]
	add r0, r5, #0
	mov r1, #3
	bl _s32_div_f
	add r1, r0, #0
	ldr r0, [sp, #0xc]
	ldr r2, [sp, #0x10]
	bl ov96_021E60C0
	ldrb r0, [r0, #7]
	cmp r0, #1
	beq _021F1AC0
	cmp r0, #2
	beq _021F1ACC
	cmp r0, #3
	beq _021F1AD8
	b _021F1AE4
_021F1AC0:
	mov r0, #2
	ldr r1, [sp, #0xc0]
	lsl r0, r0, #0xe
	sub r0, r1, r0
	str r0, [sp, #0xc0]
	b _021F1AE8
_021F1ACC:
	mov r0, #2
	ldr r1, [sp, #0xc0]
	lsl r0, r0, #0xe
	sub r0, r1, r0
	str r0, [sp, #0xc0]
	b _021F1AE8
_021F1AD8:
	mov r0, #1
	ldr r1, [sp, #0xc0]
	lsl r0, r0, #0x10
	sub r0, r1, r0
	str r0, [sp, #0xc0]
	b _021F1AE8
_021F1AE4:
	bl GF_AssertFail
_021F1AE8:
	ldr r0, [r4, #8]
	bl ov96_021EB5B8
	add r1, sp, #0xbc
	bl Sprite_SetMatrix
	ldr r0, _021F1CA4 ; =0x0000076C
	lsl r1, r5, #0x18
	ldr r0, [r6, r0]
	lsr r1, r1, #0x18
	bl ov96_021EAA04
	ldr r1, [sp, #0x48]
	str r0, [sp, #0x6c]
	add r1, r1, #1
	lsl r1, r1, #0x18
	lsr r1, r1, #0x18
	str r1, [sp, #0x20]
	bl ov96_021EAC0C
	ldr r0, [sp, #0x6c]
	ldr r1, [sp, #0x18]
	ldr r2, [sp, #0x1c]
	mov r3, #1
	bl ov96_021EB01C
	ldr r1, [sp, #0x24]
	ldr r0, [sp, #0x2c]
	cmp r1, r0
	bne _021F1B48
	ldr r0, [sp, #0xc]
	ldr r1, [sp, #0x2c]
	ldr r2, [sp, #0x10]
	bl ov96_021E60D8
	ldrb r0, [r0, #2]
	cmp r0, #0
	beq _021F1B48
	mov r0, #0
	mov r2, #0x7d
	str r0, [sp]
	lsl r2, r2, #4
	ldrsh r2, [r6, r2]
	ldr r0, [sp, #0x6c]
	ldr r3, [sp, #0x20]
	mov r1, #0
	bl ov96_021EAED4
_021F1B48:
	ldr r0, [sp, #0x5c]
	cmp r0, #0
	beq _021F1B80
	ldr r0, _021F1CA4 ; =0x0000076C
	lsl r1, r5, #0x18
	ldr r0, [r6, r0]
	lsr r1, r1, #0x18
	bl ov96_021EAA04
	str r0, [sp, #0x70]
	ldr r0, [sp, #0x3c]
	mov r1, #0
	bl Sprite_SetAnimActiveFlag
	lsl r3, r5, #0x18
	ldr r0, _021F1CA8 ; =0x000007AF
	mov r1, #1
	strb r1, [r6, r0]
	ldr r0, [sp, #0x20]
	ldr r1, _021F1CAC ; =0x000007AC
	str r0, [sp]
	ldr r0, [sp, #0xc]
	ldr r2, [sp, #0x70]
	add r1, r6, r1
	lsr r3, r3, #0x18
	bl ov96_021F2E80
	b _021F1B88
_021F1B80:
	ldr r0, [sp, #0x3c]
	mov r1, #1
	bl Sprite_SetAnimActiveFlag
_021F1B88:
	ldr r0, [sp, #0x64]
	cmp r0, r5
	bne _021F1BF6
	ldr r0, [sp, #0x28]
	cmp r0, #1
	blo _021F1BF2
	cmp r0, #4
	bhi _021F1BF2
	ldr r0, [r4, #4]
	bl ov96_021EB5B8
	str r0, [sp, #0x74]
	mov r1, #1
	ldr r0, [r4, #4]
	add r2, r1, #0
	bl ov96_021EB52C
	ldr r1, [sp, #0x28]
	ldr r0, [sp, #0x74]
	add r1, r1, #2
	bl Sprite_SetAnimCtrlSeq
	ldr r0, [sp, #0x28]
	cmp r0, #2
	bhi _021F1BC2
	ldr r0, _021F1CB0 ; =0x0000088D
	bl PlaySE
	b _021F1BD4
_021F1BC2:
	cmp r0, #3
	bne _021F1BCE
	ldr r0, _021F1CB4 ; =0x0000088E
	bl PlaySE
	b _021F1BD4
_021F1BCE:
	ldr r0, _021F1CB8 ; =0x0000088F
	bl PlaySE
_021F1BD4:
	ldr r0, _021F1CBC ; =0x00000774
	ldr r1, [sp, #0x24]
	ldr r0, [r6, r0]
	lsl r1, r1, #0x18
	ldr r2, [sp, #0x28]
	lsr r1, r1, #0x18
	bl ov96_021F4390
	ldr r0, _021F1CAC ; =0x000007AC
	lsl r1, r5, #0x18
	add r0, r6, r0
	lsr r1, r1, #0x18
	bl ov96_021F2E2C
	b _021F1BF6
_021F1BF2:
	bl GF_AssertFail
_021F1BF6:
	ldr r1, [sp, #0x24]
	ldr r0, [sp, #0x2c]
	cmp r1, r0
	bne _021F1C52
	add r0, sp, #0x7c
	str r0, [sp]
	ldr r0, [r4]
	ldr r1, [sp, #0x18]
	ldr r2, [sp, #0x1c]
	add r3, sp, #0x80
	bl ov96_021EB06C
	ldr r0, [sp, #0x44]
	cmp r0, #1
	bne _021F1C44
	mov r0, #0x73
	lsl r0, r0, #4
	ldrh r0, [r6, r0]
	ldr r1, [sp, #0x80]
	ldr r2, [sp, #0x7c]
	lsl r0, r0, #0x18
	lsl r1, r1, #0x10
	lsl r2, r2, #0x10
	lsr r0, r0, #0x18
	lsr r1, r1, #0x10
	lsr r2, r2, #0x10
	add r3, sp, #0x78
	bl ov96_021F3180
	cmp r0, #0
	beq _021F1C44
	ldr r1, [sp, #0x14]
	ldr r0, [sp, #0xc]
	lsl r1, r1, #0x18
	lsr r1, r1, #0x18
	mov r2, #1
	bl ov96_021E658C
	b _021F1C52
_021F1C44:
	ldr r1, [sp, #0x14]
	ldr r0, [sp, #0xc]
	lsl r1, r1, #0x18
	lsr r1, r1, #0x18
	mov r2, #0
	bl ov96_021E658C
_021F1C52:
	ldr r0, [sp, #0x34]
	add r5, r5, #1
	add r0, r0, #2
	str r0, [sp, #0x34]
	ldr r0, [sp, #0x30]
	add r7, r7, #4
	add r0, r0, #1
	str r0, [sp, #0x30]
	cmp r5, #0xc
	bge _021F1C68
	b _021F17DE
_021F1C68:
	ldr r1, [sp, #0x4c]
	add r0, r6, #0
	bl ov96_021F2FEC
	ldr r1, _021F1CA8 ; =0x000007AF
	ldrb r0, [r6, r1]
	cmp r0, #0
	beq _021F1C86
	ldr r0, [sp, #0x5c]
	cmp r0, #0
	bne _021F1C86
	sub r0, r1, #3
	add r0, r6, r0
	bl ov96_021F2D98
_021F1C86:
	ldr r0, _021F1CBC ; =0x00000774
	ldr r1, [sp, #0x54]
	ldr r0, [r6, r0]
	ldr r2, [sp, #0x58]
	bl ov96_021F4688
	ldr r0, _021F1CBC ; =0x00000774
	ldr r1, [sp, #0x5c]
	ldr r0, [r6, r0]
	add r2, sp, #0x12c
	bl ov96_021F45F4
	add sp, #0x138
	pop {r3, r4, r5, r6, r7, pc}
	nop
_021F1CA4: .word 0x0000076C
_021F1CA8: .word 0x000007AF
_021F1CAC: .word 0x000007AC
_021F1CB0: .word 0x0000088D
_021F1CB4: .word 0x0000088E
_021F1CB8: .word 0x0000088F
_021F1CBC: .word 0x00000774
	thumb_func_end ov96_021F1614
