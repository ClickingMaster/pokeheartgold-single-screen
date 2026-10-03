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
	.extern ov96_021EB588
	.extern ov96_021FEBF0
	.extern ov96_021FED3C
	.extern ov96_021FEE60
	.extern ov96_021FF67C
	.include "asm/macros.inc"
	.include "overlay_96.inc"
	.include "global.inc"
	.include "overlay_96_exact_bank.inc"

.text
	thumb_func_start ov96_021FEECC
ov96_021FEECC: ; 0x021FEECC
	push {r4, r5, r6, r7, lr}
	sub sp, #0x2c
	ldr r7, [sp, #0x40]
	str r0, [sp, #0x10]
	ldr r6, [sp, #0x44]
	mov r0, #0
	str r0, [sp, #0x14]
	add r4, r1, #0
	ldrb r0, [r3, #0x1d]
	lsl r1, r7, #1
	mov ip, r2
	asr r0, r1
	mov r1, #3
	and r0, r1
	add r1, r4, #0
	add r1, #0x9f
	lsl r0, r0, #0x18
	ldrb r1, [r1]
	lsr r0, r0, #0x18
	ldr r5, [sp, #0x50]
	cmp r1, r0
	beq _021FEF0A
	add r1, r4, #0
	add r1, #0x9f
	strb r0, [r1]
	add r1, r4, #0
	ldr r0, [sp, #0x14]
	add r1, #0xa0
	strb r0, [r1]
	mov r0, #1
	str r0, [sp, #0x14]
_021FEF0A:
	add r0, sp, #0x18
	str r0, [sp]
	mov r1, ip
	ldr r2, [r1]
	lsl r0, r6, #2
	asr r1, r2, #0xb
	lsr r1, r1, #0x14
	add r1, r2, r1
	mov r2, ip
	ldr r3, [r2, #4]
	ldr r0, [r4, r0]
	asr r2, r3, #0xb
	lsr r2, r2, #0x14
	add r2, r3, r2
	asr r1, r1, #0xc
	asr r2, r2, #0xc
	add r3, sp, #0x1c
	bl ov96_021EB0A4
	ldr r0, [sp, #0x1c]
	lsl r0, r0, #0xc
	str r0, [sp, #0x20]
	ldr r0, [sp, #0x18]
	lsl r0, r0, #0xc
	str r0, [sp, #0x24]
	mov r0, #0
	str r0, [sp, #0x28]
	add r0, r4, #0
	add r0, #0x9f
	ldrb r0, [r0]
	cmp r0, #0
	beq _021FEFB0
	cmp r0, #1
	beq _021FEF56
	cmp r0, #2
	beq _021FEF82
	add sp, #0x2c
	pop {r4, r5, r6, r7, pc}
_021FEF56:
	ldr r0, [sp, #0x4c]
	cmp r0, #0
	beq _021FEF66
	ldr r0, [sp, #0x54]
	cmp r0, #0
	beq _021FEF66
	mov r1, #1
	b _021FEF68
_021FEF66:
	mov r1, #0
_021FEF68:
	ldr r0, [sp, #0x48]
	str r6, [sp]
	str r0, [sp, #4]
	str r1, [sp, #8]
	ldr r0, [sp, #0x10]
	add r1, r4, #0
	add r2, sp, #0x20
	add r3, r7, #0
	str r5, [sp, #0xc]
	bl ov96_021FEBF0
	add sp, #0x2c
	pop {r4, r5, r6, r7, pc}
_021FEF82:
	ldr r0, [sp, #0x4c]
	cmp r0, #0
	beq _021FEF92
	ldr r0, [sp, #0x54]
	cmp r0, #0
	beq _021FEF92
	mov r1, #1
	b _021FEF94
_021FEF92:
	mov r1, #0
_021FEF94:
	ldr r0, [sp, #0x48]
	add r2, r7, #0
	str r0, [sp]
	add r0, r1, #0
	orr r0, r5
	str r0, [sp, #4]
	add r0, r4, #0
	add r1, sp, #0x20
	add r3, r6, #0
	str r5, [sp, #8]
	bl ov96_021FED3C
	add sp, #0x2c
	pop {r4, r5, r6, r7, pc}
_021FEFB0:
	ldr r0, [sp, #0x14]
	cmp r0, #0
	beq _021FEFDA
	ldr r0, [sp, #0x4c]
	cmp r0, #0
	beq _021FEFC6
	ldr r0, [sp, #0x54]
	cmp r0, #0
	beq _021FEFC6
	mov r0, #1
	b _021FEFC8
_021FEFC6:
	mov r0, #0
_021FEFC8:
	orr r0, r5
	str r0, [sp]
	ldr r3, [sp, #0x48]
	add r0, r4, #0
	add r1, r7, #0
	add r2, r6, #0
	str r5, [sp, #4]
	bl ov96_021FEE60
_021FEFDA:
	ldr r0, [r4, #0x70]
	add r1, sp, #0x20
	bl ov96_021EB588
	add sp, #0x2c
	pop {r4, r5, r6, r7, pc}
	.balign 4, 0
	thumb_func_end ov96_021FEECC

	thumb_func_start ov96_021FEFE8
ov96_021FEFE8: ; 0x021FEFE8
	push {r3, r4, r5, r6, r7, lr}
	str r3, [sp]
	add r5, r1, #0
	add r1, r2, #0
	ldr r2, [sp]
	ldr r6, [sp, #0x18]
	bl ov96_021E60D8
	add r4, r0, #0
	ldr r0, [sp]
	mov r1, #0x1c
	add r7, r0, #0
	ldrb r0, [r4]
	mul r7, r1
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	bl _fflt
	bl _f2d
	ldr r3, _021FF0B8 ; =0x40240000
	mov r2, #0
	bl _ddiv
	bl _d2f
	add r1, r6, r7
	str r0, [r1, #0x20]
	ldrb r0, [r4, #3]
	lsl r0, r0, #2
	add r0, r5, r0
	ldr r0, [r0, #0x14]
	bl _fflt
	add r1, r6, r7
	str r0, [r1, #0x28]
	ldrb r0, [r4, #3]
	lsl r0, r0, #2
	add r0, r5, r0
	ldr r0, [r0, #0x14]
	bl _fflt
	add r1, r6, r7
	str r0, [r1, #0x24]
	ldrb r0, [r4, #1]
	lsl r0, r0, #2
	add r0, r5, r0
	ldr r2, [r0, #0x64]
	add r0, r1, #0
	add r0, #0x2e
	strb r2, [r0]
	ldrb r0, [r4, #1]
	lsl r0, r0, #2
	add r0, r5, r0
	add r0, #0x8c
	ldr r2, [r0]
	add r0, r1, #0
	add r0, #0x2c
	strb r2, [r0]
	ldrb r0, [r4, #1]
	add r1, #0x2d
	lsl r0, r0, #2
	add r0, r5, r0
	ldr r0, [r0, #0x78]
	strb r0, [r1]
	ldrb r0, [r4, #4]
	lsl r0, r0, #2
	add r0, r5, r0
	ldr r0, [r0, #0x28]
	bl _fflt
	bl _f2d
	ldr r3, _021FF0B8 ; =0x40240000
	mov r2, #0
	bl _ddiv
	bl _d2f
	add r1, r6, r7
	str r0, [r1, #0x18]
	ldrb r0, [r4, #1]
	lsl r0, r0, #2
	add r0, r5, r0
	ldr r0, [r0, #0x3c]
	bl _fflt
	bl _f2d
	ldr r3, _021FF0B8 ; =0x40240000
	mov r2, #0
	bl _ddiv
	bl _d2f
	add r1, r6, r7
	str r0, [r1, #0x1c]
	ldrb r0, [r4, #3]
	add r1, #0x2f
	lsl r0, r0, #2
	add r0, r5, r0
	ldr r0, [r0, #0x50]
	strb r0, [r1]
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
_021FF0B8: .word 0x40240000
	thumb_func_end ov96_021FEFE8

	thumb_func_start ov96_021FF0BC
ov96_021FF0BC: ; 0x021FF0BC
	push {r4, r5, r6, r7, lr}
	sub sp, #0x14
	add r5, r0, #0
	ldr r0, [sp, #0x28]
	str r2, [sp]
	str r0, [sp, #0x28]
	ldr r0, [sp, #0x2c]
	str r3, [sp, #4]
	str r0, [sp, #0x2c]
	mov r0, #0
	add r7, r0, #0
_021FF0D2:
	stmia r2!, {r7}
	add r0, r0, #1
	stmia r3!, {r7}
	cmp r0, #0x1e
	blt _021FF0D2
	ldr r0, [sp, #0x28]
	str r5, [sp, #8]
	strb r7, [r0]
	ldr r0, [sp, #0x2c]
	strb r7, [r0]
	asr r0, r1, #0xb
	lsr r0, r0, #0x14
	add r0, r1, r0
	asr r0, r0, #0xc
	str r0, [sp, #0x10]
	ldr r0, _021FF1C4 ; =0x0000045C
	add r6, r5, r0
_021FF0F4:
	ldr r0, _021FF1C8 ; =0x000004D4
	ldr r0, [r5, r0]
	cmp r0, #0
	beq _021FF1C0
	ldr r1, [sp, #8]
	ldr r0, _021FF1C4 ; =0x0000045C
	ldr r0, [r1, r0]
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	str r0, [sp, #0xc]
	ldr r0, _021FF1CC ; =0x000004DC
	ldrh r1, [r5, r0]
	ldr r0, [sp, #0x10]
	sub r4, r0, r1
	add r0, r4, #0
	bl _dflt
	ldr r3, _021FF1D0 ; =0x40880000
	mov r2, #0
	bl _dgeq
	blo _021FF144
	add r0, r4, #0
	bl _dflt
	add r3, r1, #0
	add r2, r0, #0
	ldr r1, _021FF1D4 ; =0x40900000
	mov r0, #0
	bl _dsub
	add r3, r1, #0
	add r2, r0, #0
	ldr r1, _021FF1D8 ; =0x40540000
	mov r0, #0
	bl _dadd
	bl _dfix
	b _021FF17C
_021FF144:
	add r0, r4, #0
	bl _dflt
	ldr r3, _021FF1DC ; =0xC0880000
	mov r2, #0
	bl _dleq
	bhi _021FF178
	add r0, r4, #0
	bl _dflt
	add r3, r1, #0
	add r2, r0, #0
	ldr r1, _021FF1D4 ; =0x40900000
	mov r0, #0
	bl _dadd
	add r3, r1, #0
	add r2, r0, #0
	ldr r1, _021FF1D8 ; =0x40540000
	mov r0, #0
	bl _dsub
	bl _dfix
	b _021FF17C
_021FF178:
	mov r0, #0x50
	sub r0, r0, r4
_021FF17C:
	mov r1, #0x1f
	mvn r1, r1
	cmp r0, r1
	blt _021FF1B0
	mov r1, #0x12
	lsl r1, r1, #4
	cmp r0, r1
	bgt _021FF1B0
	ldr r0, [sp, #0xc]
	cmp r0, #3
	bne _021FF1A2
	ldr r0, [sp, #0x2c]
	ldrb r2, [r0]
	add r1, r2, #1
	strb r1, [r0]
	ldr r0, [sp, #4]
	lsl r1, r2, #2
	str r6, [r0, r1]
	b _021FF1B0
_021FF1A2:
	ldr r0, [sp, #0x28]
	ldrb r2, [r0]
	add r1, r2, #1
	strb r1, [r0]
	ldr r0, [sp]
	lsl r1, r2, #2
	str r6, [r0, r1]
_021FF1B0:
	ldr r0, [sp, #8]
	add r7, r7, #1
	add r0, r0, #4
	add r5, #0xc
	add r6, r6, #4
	str r0, [sp, #8]
	cmp r7, #0x1e
	blt _021FF0F4
_021FF1C0:
	add sp, #0x14
	pop {r4, r5, r6, r7, pc}
	.balign 4, 0
_021FF1C4: .word 0x0000045C
_021FF1C8: .word 0x000004D4
_021FF1CC: .word 0x000004DC
_021FF1D0: .word 0x40880000
_021FF1D4: .word 0x40900000
_021FF1D8: .word 0x40540000
_021FF1DC: .word 0xC0880000
	thumb_func_end ov96_021FF0BC
