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
	.extern ov96_021EB564
	.extern ov96_021EB570
	.extern ov96_021EB57C
	.extern ov96_021EB588
	.extern ov96_021EB630
	.extern ov96_02211AF0
	.extern ov96_02213354
	.extern ov96_02214904
	.include "asm/macros.inc"
	.include "overlay_96.inc"
	.include "global.inc"
	.include "overlay_96_exact_bank.inc"
	.extern ov96_022123B0
	.extern ov96_022124F8
	.extern ov96_022127F4
	.extern ov96_02212B94

.text
	thumb_func_start ov96_02211B94
ov96_02211B94: ; 0x02211B94
	push {r4, r5, r6, r7, lr}
	sub sp, #0x14
	str r0, [sp, #4]
	bl PokeathlonCourse_GetDataCopyArea
	str r0, [sp, #0xc]
	ldr r0, [sp, #4]
	bl PokeathlonCourse_GetHeapAllocPtr4
	add r4, r0, #0
	ldr r0, _02211DC8 ; =0x00000738
	ldr r1, [r4, r0]
	cmp r1, #0
	ble _02211BB4
	sub r1, r1, #1
	str r1, [r4, r0]
_02211BB4:
	ldr r0, [sp, #4]
	bl ov96_021E5F24
	cmp r0, #0
	beq _02211BC0
	b _02211DC4
_02211BC0:
	ldr r0, _02211DCC ; =0x0000073E
	ldrb r0, [r4, r0]
	cmp r0, #0
	beq _02211BDE
	ldr r0, [sp, #0xc]
	add r0, #0x28
	str r0, [sp, #0xc]
	bl ov96_021E8A20
	add r1, r0, #0
	add r0, r4, #0
	bl ov96_02211DE4
	add sp, #0x14
	pop {r4, r5, r6, r7, pc}
_02211BDE:
	ldr r0, [sp, #0xc]
	add r0, #0x28
	bl ov96_021E8A20
	str r0, [sp, #0x10]
	ldr r0, [sp, #0xc]
	add r0, #0x50
	bl ov96_021E8A20
	add r5, r0, #0
	ldr r0, [sp, #0xc]
	bl ov96_021E8A20
	add r3, r0, #0
	mov r2, #4
_02211BFC:
	ldmia r3!, {r0, r1}
	stmia r5!, {r0, r1}
	sub r2, r2, #1
	bne _02211BFC
	ldr r0, [r3]
	mov r6, #0
	str r0, [r5]
	ldr r0, [sp, #0xc]
	add r5, r4, #0
	add r0, #0x50
	str r0, [sp, #0xc]
	add r0, r4, #0
	str r0, [sp, #8]
	add r0, #0x5c
	str r0, [sp, #8]
_02211C1A:
	ldr r0, [sp, #0xc]
	bl ov96_021E8A20
	add r7, r0, #0
	ldr r0, [r7]
	cmp r0, #0
	ldr r0, _02211DD0 ; =0x00000704
	beq _02211C58
	ldr r1, [r5, r0]
	cmp r1, #0
	beq _02211C40
	add r0, r0, #4
	ldr r0, [r5, r0]
	cmp r0, #0
	beq _02211C40
	ldr r0, _02211DD0 ; =0x00000704
	mov r1, #0
	str r1, [r5, r0]
	b _02211C60
_02211C40:
	cmp r1, #0
	bne _02211C60
	ldr r0, _02211DD4 ; =0x00000708
	ldr r0, [r5, r0]
	cmp r0, #0
	bne _02211C60
	ldr r0, _02211DD0 ; =0x00000704
	mov r1, #1
	str r1, [r5, r0]
	add r0, r0, #4
	str r1, [r5, r0]
	b _02211C60
_02211C58:
	mov r1, #0
	str r1, [r5, r0]
	add r0, r0, #4
	str r1, [r5, r0]
_02211C60:
	ldr r0, _02211DD0 ; =0x00000704
	ldr r0, [r5, r0]
	cmp r0, #0
	beq _02211CAA
	ldrb r2, [r7, #4]
	lsl r1, r6, #0x18
	ldrb r3, [r7, #5]
	add r0, r4, #0
	lsr r1, r1, #0x18
	bl ov96_02213364
	add r7, r0, #0
	cmp r7, #0xc
	beq _02211D68
	mov r1, #3
	bl _s32_div_f
	mov r0, #0x7c
	add r2, r1, #0
	mul r2, r0
	ldr r0, [sp, #8]
	add r1, r0, r2
	ldr r0, [r1, #0x78]
	cmp r0, #2
	beq _02211D68
	cmp r0, #1
	beq _02211D68
	ldr r0, [r1, #0x48]
	cmp r0, #0
	bne _02211D68
	ldr r0, _02211DD8 ; =0x00000734
	add r1, r4, r6
	strb r7, [r1, r0]
	mov r1, #0
	add r0, #0xe
	strb r1, [r4, r0]
	b _02211D68
_02211CAA:
	ldr r0, _02211DD4 ; =0x00000708
	ldr r0, [r5, r0]
	cmp r0, #0
	beq _02211CF2
	ldr r1, _02211DD8 ; =0x00000734
	add r0, r4, r6
	ldrb r1, [r0, r1]
	cmp r1, #0xc
	beq _02211D68
	ldr r1, _02211DDC ; =0x00000742
	ldrb r1, [r4, r1]
	cmp r1, #0x14
	bhs _02211CCE
	ldr r1, _02211DDC ; =0x00000742
	ldrb r1, [r4, r1]
	add r2, r1, #1
	ldr r1, _02211DDC ; =0x00000742
	strb r2, [r4, r1]
_02211CCE:
	ldr r1, _02211DD8 ; =0x00000734
	ldrb r0, [r0, r1]
	mov r1, #3
	bl _s32_div_f
	add r2, r1, #0
	lsl r2, r2, #0x18
	lsr r2, r2, #0x18
	mov r3, #0x7c
	mul r3, r2
	ldr r2, [sp, #8]
	ldrb r0, [r7, #4]
	add r2, r2, r3
	ldrb r1, [r7, #5]
	add r2, #0x24
	bl ov96_02213354
	b _02211D68
_02211CF2:
	ldr r0, _02211DD8 ; =0x00000734
	add r7, r4, r6
	ldrb r0, [r7, r0]
	cmp r0, #0xc
	beq _02211D68
	mov r1, #3
	bl _s32_div_f
	lsl r0, r1, #0x18
	lsr r1, r0, #0x18
	mov r0, #0x7c
	add r2, r1, #0
	mul r2, r0
	ldr r0, [sp, #8]
	add r0, r0, r2
	ldr r1, [r0, #0x78]
	cmp r1, #3
	beq _02211D5C
	cmp r1, #2
	beq _02211D5C
	ldr r1, _02211DDC ; =0x00000742
	ldrb r1, [r4, r1]
	cmp r1, #8
	bhi _02211D58
	ldr r2, [r0, #0x24]
	ldr r1, [r0, #0x30]
	cmp r2, r1
	bne _02211D32
	ldr r2, [r0, #0x28]
	ldr r1, [r0, #0x34]
	cmp r2, r1
	beq _02211D5C
_02211D32:
	mov r1, #1
	str r1, [r0, #0x78]
	ldr r0, _02211DD8 ; =0x00000734
	mov r1, #3
	ldrb r0, [r7, r0]
	bl _s32_div_f
	add r2, r1, #0
	mov r0, #1
	str r0, [sp]
	lsl r1, r6, #0x18
	lsl r2, r2, #0x18
	ldr r0, [sp, #4]
	lsr r1, r1, #0x18
	lsr r2, r2, #0x18
	mov r3, #6
	bl ov96_021E8228
	b _02211D5C
_02211D58:
	mov r1, #0
	str r1, [r0, #0x78]
_02211D5C:
	ldr r0, _02211DDC ; =0x00000742
	mov r1, #0
	strb r1, [r4, r0]
	mov r1, #0xc
	sub r0, #0xe
	strb r1, [r7, r0]
_02211D68:
	ldr r0, [sp, #0xc]
	ldr r1, [sp, #8]
	add r0, #0x28
	str r0, [sp, #0xc]
	mov r0, #0x5d
	lsl r0, r0, #2
	add r0, r1, r0
	add r6, r6, #1
	add r5, #0xc
	str r0, [sp, #8]
	cmp r6, #4
	bge _02211D82
	b _02211C1A
_02211D82:
	ldr r1, _02211DE0 ; =0x0000074C
	ldr r0, [sp, #4]
	ldr r1, [r4, r1]
	bl ov96_02214B84
	ldr r0, [sp, #4]
	bl ov96_022124F8
	ldr r0, [sp, #4]
	bl ov96_02212B94
	ldr r0, [sp, #4]
	bl ov96_022127F4
	ldr r0, [sp, #4]
	bl ov96_022130EC
	ldr r0, [sp, #4]
	bl ov96_022132FC
	ldr r0, _02211DC8 ; =0x00000738
	ldr r1, [r4, r0]
	cmp r1, #0
	bgt _02211DB6
	mov r1, #1
	b _02211DB8
_02211DB6:
	mov r1, #0
_02211DB8:
	add r0, r0, #6
	strb r1, [r4, r0]
	ldr r1, [sp, #0x10]
	add r0, r4, #0
	bl ov96_02211DE4
_02211DC4:
	add sp, #0x14
	pop {r4, r5, r6, r7, pc}
	.balign 4, 0
_02211DC8: .word 0x00000738
_02211DCC: .word 0x0000073E
_02211DD0: .word 0x00000704
_02211DD4: .word 0x00000708
_02211DD8: .word 0x00000734
_02211DDC: .word 0x00000742
_02211DE0: .word 0x0000074C
	thumb_func_end ov96_02211B94

	thumb_func_start ov96_02211DE4
ov96_02211DE4: ; 0x02211DE4
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x18
	str r0, [sp]
	mov r0, #0
	str r0, [sp, #0xc]
	str r0, [sp, #8]
	str r0, [sp, #4]
	add r4, r0, #0
	add r6, r0, #0
	ldr r0, [sp]
	add r5, r1, #0
	str r0, [sp, #0x10]
	add r0, #0x5c
	str r0, [sp, #0x10]
_02211E00:
	add r0, r4, #0
	mov r1, #3
	bl _s32_div_f
	add r7, r0, #0
	add r0, r4, #0
	mov r1, #3
	bl _s32_div_f
	mov r0, #0x5d
	lsl r0, r0, #2
	add r2, r7, #0
	mul r2, r0
	ldr r0, [sp, #0x10]
	add r2, r0, r2
	mov r0, #0x7c
	mul r0, r1
	add r0, r2, r0
	add r3, r0, #0
	add r3, #0x5c
	ldrh r3, [r3]
	ldr r2, [r0, #0x34]
	sub r3, r3, #1
	lsl r3, r3, #0x18
	asr r1, r2, #0xb
	lsr r7, r3, #0x18
	add r3, r5, r4
	lsr r1, r1, #0x14
	mov ip, r3
	add r1, r2, r1
	asr r2, r1, #0xc
	ldr r1, [r0, #0x78]
	ldr r3, [r0, #0x30]
	lsl r1, r1, #0x18
	asr r0, r3, #0xb
	lsr r0, r0, #0x14
	add r0, r3, r0
	asr r0, r0, #0xc
	strb r0, [r5, r4]
	mov r0, ip
	strb r2, [r0, #0xc]
	add r2, r7, #0
	lsr r1, r1, #0x18
	lsl r2, r6
	ldr r0, [sp, #0xc]
	lsl r1, r6
	add r0, r0, r2
	str r0, [sp, #0xc]
	ldr r0, [sp, #8]
	add r4, r4, #1
	add r0, r0, r1
	add r6, r6, #2
	str r0, [sp, #8]
	cmp r4, #0xc
	blt _02211E00
	ldr r1, _02211F2C ; =0x0000062C
	ldr r0, [sp]
	mov r6, #0
	add r4, r0, r1
	add r7, r6, #0
_02211E78:
	ldr r1, [r4, #8]
	asr r0, r1, #0xb
	lsr r0, r0, #0x14
	add r0, r1, r0
	asr r0, r0, #0xc
	str r0, [sp, #0x14]
	add r0, r4, #0
	bl ov96_02213EC4
	add r2, r5, r6
	ldr r1, [sp, #0x14]
	add r6, r6, #1
	strb r1, [r2, #0x18]
	strb r0, [r2, #0x1a]
	add r0, r4, #0
	add r0, #0x39
	ldrb r0, [r0]
	add r4, #0x4c
	add r1, r0, #0
	lsl r1, r7
	ldr r0, [sp, #4]
	add r7, r7, #2
	add r0, r0, r1
	str r0, [sp, #4]
	cmp r6, #2
	blt _02211E78
	ldr r0, _02211F30 ; =0x0000073C
	ldr r1, [sp]
	ldrb r2, [r1, r0]
	lsl r3, r2, #2
	add r3, r1, r3
	add r1, r0, #0
	sub r1, #0x48
	ldr r1, [r3, r1]
	lsl r1, r1, #0x18
	lsr r3, r1, #0x18
	add r1, r2, #1
	lsr r6, r1, #0x1f
	lsl r4, r1, #0x1e
	sub r4, r4, r6
	mov r1, #0x1e
	ror r4, r1
	ldr r1, [sp]
	add r4, r6, r4
	strb r4, [r1, r0]
	ldr r4, [sp, #4]
	lsl r2, r2, #0x1c
	lsl r6, r4, #0x18
	ldr r4, [sp, #0xc]
	add r1, r5, #0
	add r4, r4, r6
	add r2, r4, r2
	str r2, [r5, #0x1c]
	ldr r2, [sp]
	sub r0, #0xd8
	ldrb r0, [r2, r0]
	add r1, #0x1c
	cmp r0, #2
	bne _02211EF8
	mov r0, #1
	ldr r2, [r1]
	lsl r0, r0, #0x1e
	add r0, r2, r0
	str r0, [r1]
_02211EF8:
	mov r1, #0x6b
	ldr r0, [sp]
	lsl r1, r1, #4
	ldrb r0, [r0, r1]
	cmp r0, #0
	beq _02211F0E
	mov r0, #2
	ldr r1, [r5, #0x1c]
	lsl r0, r0, #0x1e
	sub r0, r1, r0
	str r0, [r5, #0x1c]
_02211F0E:
	ldr r0, [sp, #8]
	mov r1, #0
	add r2, r1, r0
	ldr r1, _02211F34 ; =0x0000073E
	ldr r0, [sp]
	str r2, [r5, #0x20]
	ldrb r0, [r0, r1]
	lsl r0, r0, #0x18
	add r1, r2, r0
	lsl r0, r3, #0x19
	add r0, r1, r0
	str r0, [r5, #0x20]
	add sp, #0x18
	pop {r3, r4, r5, r6, r7, pc}
	nop
_02211F2C: .word 0x0000062C
_02211F30: .word 0x0000073C
_02211F34: .word 0x0000073E
	thumb_func_end ov96_02211DE4

	thumb_func_start ov96_02211F38
ov96_02211F38: ; 0x02211F38
	push {r4, r5, r6, r7, lr}
	sub sp, #0x74
	str r0, [sp, #8]
	bl PokeathlonCourse_GetHeapAllocPtr4
	str r0, [sp, #0x38]
	ldr r0, [sp, #8]
	bl ov96_021E5F24
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	str r0, [sp, #0x10]
	ldr r0, [sp, #8]
	bl PokeathlonCourse_GetDataCopyArea
	add r0, #0xf0
	bl ov96_021E8A20
	add r6, r0, #0
	ldr r0, [sp, #0x38]
	bl ov96_02214418
	ldr r0, [sp, #0x38]
	mov r4, #0
	str r0, [sp, #0x40]
	add r0, #0x5c
	add r7, r4, #0
	str r0, [sp, #0x40]
_02211F70:
	add r0, r4, #0
	mov r1, #3
	bl _s32_div_f
	str r0, [sp, #0x18]
	add r0, r4, #0
	mov r1, #3
	bl _s32_div_f
	ldr r2, [sp, #0x18]
	mov r0, #0x5d
	add r3, r2, #0
	lsl r0, r0, #2
	mul r3, r0
	ldr r0, [sp, #0x40]
	mov r2, #0x7c
	add r0, r0, r3
	mul r2, r1
	add r5, r0, r2
	ldr r0, [r0, r2]
	bl ov96_021EAA20
	bl ov96_021E8BAC
	ldr r0, [r6, #0x20]
	add r1, r0, #0
	asr r1, r7
	mov r0, #3
	and r0, r1
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	str r0, [sp, #0x28]
	cmp r0, #3
	bne _02211FC8
	mov r1, #1
	ldr r0, [r5, #4]
	add r2, r1, #0
	bl ov96_021EB52C
	ldr r0, [r5, #4]
	mov r1, #0xc
	bl ov96_021EB570
	b _02211FFE
_02211FC8:
	ldr r0, [sp, #0x28]
	cmp r0, #2
	ldr r0, [r5, #4]
	bne _02211FF6
	mov r1, #1
	add r2, r1, #0
	bl ov96_021EB52C
	ldr r0, [r5, #4]
	mov r1, #0x10
	bl ov96_021EB570
	mov r0, #0x89
	lsl r0, r0, #4
	bl IsSEPlaying
	cmp r0, #0
	bne _02211FFE
	mov r0, #0x89
	lsl r0, r0, #4
	bl PlaySE
	b _02211FFE
_02211FF6:
	mov r1, #1
	mov r2, #0
	bl ov96_021EB52C
_02211FFE:
	add r0, r6, r4
	str r0, [sp, #0xc]
	ldrb r0, [r6, r4]
	str r0, [sp, #0x34]
	ldr r0, [sp, #0xc]
	ldrb r0, [r0, #0xc]
	str r0, [sp, #0x30]
	ldr r0, [r6, #0x1c]
	add r1, r0, #0
	asr r1, r7
	mov r0, #3
	and r0, r1
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	str r0, [sp, #0x2c]
	mov r1, #0
	add r0, sp, #0x68
	str r1, [r0]
	str r1, [r0, #4]
	str r1, [r0, #8]
	ldr r0, [sp, #0x34]
	mov r1, #3
	lsl r0, r0, #0xc
	str r0, [sp, #0x68]
	ldr r0, [sp, #0x30]
	lsl r0, r0, #0xc
	str r0, [sp, #0x6c]
	add r0, r4, #0
	bl _s32_div_f
	str r0, [sp, #0x44]
	add r0, r4, #0
	mov r1, #3
	bl _s32_div_f
	add r2, r1, #0
	ldr r0, [sp, #8]
	ldr r1, [sp, #0x44]
	bl ov96_021E60C0
	ldrb r0, [r0, #7]
	cmp r0, #1
	beq _0221205E
	cmp r0, #2
	beq _0221206A
	cmp r0, #3
	beq _02212076
	b _02212082
_0221205E:
	mov r0, #2
	ldr r1, [sp, #0x6c]
	lsl r0, r0, #0xe
	sub r0, r1, r0
	str r0, [sp, #0x6c]
	b _02212086
_0221206A:
	mov r0, #2
	ldr r1, [sp, #0x6c]
	lsl r0, r0, #0xe
	sub r0, r1, r0
	str r0, [sp, #0x6c]
	b _02212086
_02212076:
	mov r0, #1
	ldr r1, [sp, #0x6c]
	lsl r0, r0, #0x10
	sub r0, r1, r0
	str r0, [sp, #0x6c]
	b _02212086
_02212082:
	bl GF_AssertFail
_02212086:
	ldr r0, [r5, #4]
	add r1, sp, #0x68
	bl ov96_021EB588
	ldr r0, [sp, #0x28]
	cmp r0, #1
	bne _02212138
	add r0, r5, #0
	add r0, #0x63
	ldrb r0, [r0]
	cmp r0, #0
	bne _0221212C
	mov r1, #0
	add r0, sp, #0x5c
	str r1, [r0]
	str r1, [r0, #4]
	str r1, [r0, #8]
	ldr r1, [sp, #0x38]
	ldr r0, _02212358 ; =0x00000748
	ldr r0, [r1, r0]
	lsl r1, r4, #0x18
	lsr r1, r1, #0x18
	bl ov96_021EAA04
	add r1, r5, #0
	add r1, #0x62
	ldrb r1, [r1]
	ldr r2, [sp, #0xc]
	add r3, sp, #0x4c
	str r1, [sp, #0x24]
	add r1, sp, #0x48
	str r1, [sp]
	ldr r1, [sp, #0xc]
	ldrb r2, [r2, #0xc]
	ldrb r1, [r1]
	bl ov96_021EB06C
	ldr r0, [sp, #0x4c]
	add r1, sp, #0x5c
	lsl r0, r0, #0xc
	str r0, [sp, #0x5c]
	ldr r0, [sp, #0x48]
	lsl r0, r0, #0xc
	str r0, [sp, #0x60]
	ldr r0, [sp, #0x24]
	lsl r0, r0, #2
	add r0, r5, r0
	str r0, [sp, #0x14]
	ldr r0, [r0, #8]
	bl ov96_021EB588
	ldr r0, [sp, #0x14]
	mov r1, #1
	ldr r0, [r0, #8]
	add r2, r1, #0
	bl ov96_021EB52C
	ldr r0, [sp, #0x14]
	mov r1, #0xe
	ldr r0, [r0, #8]
	bl ov96_021EB564
	add r0, r5, #0
	add r0, #0x62
	ldrb r0, [r0]
	mov r1, #3
	add r0, r0, #1
	bl _s32_div_f
	add r0, r5, #0
	add r0, #0x62
	strb r1, [r0]
	add r5, #0x63
	mov r0, #5
	strb r0, [r5]
	ldr r1, [sp, #0x18]
	ldr r0, [sp, #0x10]
	cmp r1, r0
	bne _02212138
	ldr r0, _0221235C ; =0x000008CC
	bl PlaySE
	b _02212138
_0221212C:
	add r0, r5, #0
	add r0, #0x63
	ldrb r0, [r0]
	add r5, #0x63
	sub r0, r0, #1
	strb r0, [r5]
_02212138:
	ldr r1, [sp, #0x38]
	ldr r0, _02212358 ; =0x00000748
	ldr r0, [r1, r0]
	lsl r1, r4, #0x18
	lsr r1, r1, #0x18
	bl ov96_021EAA04
	ldr r1, [sp, #0x28]
	add r5, r0, #0
	cmp r1, #2
	bne _02212156
	mov r1, #0x14
	bl ov96_021EAC5C
	b _02212162
_02212156:
	ldr r1, [sp, #0x2c]
	add r1, r1, #1
	lsl r1, r1, #0x18
	lsr r1, r1, #0x18
	bl ov96_021EAC0C
_02212162:
	ldr r1, [sp, #0x34]
	ldr r2, [sp, #0x30]
	add r0, r5, #0
	mov r3, #1
	bl ov96_021EB01C
	add r4, r4, #1
	add r7, r7, #2
	cmp r4, #0xc
	bge _02212178
	b _02211F70
_02212178:
	ldr r2, [sp, #0x38]
	mov r1, #0x5d
	ldr r0, [sp, #0x10]
	lsl r1, r1, #2
	add r2, #0x5c
	mul r1, r0
	add r0, r2, r1
	ldr r1, [sp, #0x10]
	add r3, r6, #0
	lsl r2, r1, #1
	add r3, #0xc
	add r1, r1, r2
	add r1, r3, r1
	bl ov96_022144C0
	ldr r0, [sp, #0x38]
	ldr r2, [sp, #0x10]
	add r1, r6, #0
	bl ov96_0221457C
	ldr r2, [r6, #0x1c]
	mov r5, #0
	asr r0, r2, #0x1e
	mov r1, #1
	and r0, r1
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	str r0, [sp, #0x1c]
	asr r0, r2, #0x1f
	and r0, r1
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	str r0, [sp, #0x3c]
	ldr r1, _02212360 ; =0x0000062C
	ldr r0, [sp, #0x38]
	str r5, [sp, #0x20]
	add r4, r0, r1
_022121C2:
	mov r1, #0
	add r0, sp, #0x50
	str r1, [r0]
	str r1, [r0, #4]
	str r1, [r0, #8]
	ldr r0, [r6, #0x1c]
	asr r0, r0, #0x18
	lsl r0, r0, #0x18
	lsr r1, r0, #0x18
	ldr r0, [sp, #0x20]
	asr r1, r0
	mov r0, #3
	and r0, r1
	lsl r0, r0, #0x18
	lsr r7, r0, #0x18
	cmp r7, #1
	bne _02212220
	ldr r0, [r4]
	mov r1, #1
	mov r2, #0
	bl ov96_021EB52C
	ldr r0, [r4, #4]
	mov r1, #1
	mov r2, #0
	bl ov96_021EB52C
	cmp r5, #0
	bne _02212206
	ldr r0, [sp, #0x1c]
	cmp r0, #0
	beq _02212206
	mov r2, #1
	b _02212208
_02212206:
	mov r2, #0
_02212208:
	add r3, r6, r5
	ldrb r0, [r3, #0x1a]
	lsl r1, r5, #0x18
	lsr r1, r1, #0x18
	str r0, [sp]
	ldr r0, [sp, #0x10]
	str r0, [sp, #4]
	ldrb r3, [r3, #0x18]
	ldr r0, [sp, #0x38]
	bl ov96_022123B0
	b _02212296
_02212220:
	add r1, r4, #0
	add r1, #0x3f
	mov r0, #0
	strb r0, [r1]
	cmp r5, #1
	bne _02212248
	ldr r0, [sp, #0x3c]
	cmp r0, #0
	ldr r0, [r4]
	beq _0221223E
	mov r1, #1
	add r2, r1, #0
	bl ov96_021EB52C
	b _02212252
_0221223E:
	mov r1, #1
	mov r2, #0
	bl ov96_021EB52C
	b _02212252
_02212248:
	mov r1, #1
	ldr r0, [r4]
	add r2, r1, #0
	bl ov96_021EB52C
_02212252:
	cmp r7, #2
	ldr r0, [r4, #4]
	bne _02212272
	mov r1, #1
	add r2, r1, #0
	bl ov96_021EB52C
	add r0, r4, #0
	add r0, #0x44
	ldrb r0, [r0]
	cmp r0, #2
	beq _0221227A
	ldr r0, _02212364 ; =0x000008C7
	bl PlaySE
	b _0221227A
_02212272:
	mov r1, #1
	mov r2, #0
	bl ov96_021EB52C
_0221227A:
	cmp r5, #0
	bne _0221228E
	ldr r0, [sp, #0x1c]
	cmp r0, #0
	beq _0221228E
	ldr r0, [r4]
	mov r1, #0xb
	bl ov96_021EB570
	b _02212296
_0221228E:
	ldr r0, [r4]
	mov r1, #0xa
	bl ov96_021EB570
_02212296:
	add r0, r4, #0
	add r0, #0x44
	strb r7, [r0]
	add r7, r6, r5
	ldrb r0, [r7, #0x18]
	lsl r0, r0, #0xc
	str r0, [sp, #0x50]
	ldrb r0, [r7, #0x1a]
	bl ov96_02213F5C
	str r0, [sp, #0x54]
	lsl r1, r5, #0x18
	ldrb r3, [r7, #0x1a]
	ldr r0, [sp, #0x38]
	ldr r2, [sp, #0x1c]
	lsr r1, r1, #0x18
	bl ov96_02213FF4
	ldr r0, [r4]
	add r1, sp, #0x50
	bl ov96_021EB588
	ldr r0, [r4, #4]
	add r1, sp, #0x50
	bl ov96_021EB588
	ldrb r0, [r7, #0x1a]
	cmp r0, #0xc0
	ldr r0, [r4]
	bhs _022122DA
	mov r1, #0x2e
	bl ov96_021EB630
	b _022122E0
_022122DA:
	mov r1, #4
	bl ov96_021EB630
_022122E0:
	ldr r0, [sp, #0x20]
	add r5, r5, #1
	add r0, r0, #2
	add r4, #0x4c
	str r0, [sp, #0x20]
	cmp r5, #2
	bge _022122F0
	b _022121C2
_022122F0:
	ldr r7, _02212368 ; =0x000006E4
	ldr r4, [sp, #0x38]
	add r6, r7, #0
	mov r5, #0
	sub r6, #0x10
_022122FA:
	ldr r0, [r4, r6]
	bl ov96_021EB57C
	cmp r0, #0
	bne _0221230E
	ldr r0, [r4, r7]
	mov r1, #1
	mov r2, #0
	bl ov96_021EB52C
_0221230E:
	add r5, r5, #1
	add r4, r4, #4
	cmp r5, #4
	blt _022122FA
	ldr r0, [sp, #8]
	bl ov96_0221236C
	mov r1, #0x75
	ldr r0, [sp, #0x38]
	lsl r1, r1, #4
	ldr r0, [r0, r1]
	bl ov96_02214904
	add r2, r0, #0
	mov r1, #0x75
	ldr r0, [sp, #0x38]
	lsl r1, r1, #4
	ldr r0, [r0, r1]
	lsl r1, r2, #0x18
	lsr r1, r1, #0x18
	bl ov96_0221497C
	mov r2, #0x75
	ldr r0, [sp, #0x38]
	lsl r2, r2, #4
	ldr r0, [r0, r2]
	ldr r1, [sp, #0x38]
	sub r2, #0x18
	ldr r1, [r1, r2]
	bl ov96_0221490C
	ldr r0, [sp, #0x38]
	bl ov96_02214490
	add sp, #0x74
	pop {r4, r5, r6, r7, pc}
	nop
_02212358: .word 0x00000748
_0221235C: .word 0x000008CC
_02212360: .word 0x0000062C
_02212364: .word 0x000008C7
_02212368: .word 0x000006E4
	thumb_func_end ov96_02211F38

	thumb_func_start ov96_0221236C
ov96_0221236C: ; 0x0221236C
	push {r3, r4, r5, lr}
	add r5, r0, #0
	bl PokeathlonCourse_GetHeapAllocPtr4
	add r4, r0, #0
	add r0, r5, #0
	bl PokeathlonCourse_GetDataCopyArea
	add r0, #0xf0
	bl ov96_021E8A20
	ldr r1, [r0, #0x1c]
	ldr r0, [r0, #0x20]
	asr r2, r1, #0x1c
	mov r1, #3
	and r1, r2
	lsl r1, r1, #0x18
	lsr r2, r1, #0x18
	asr r1, r0, #0x19
	mov r0, #0x7f
	and r0, r1
	lsl r0, r0, #0x18
	lsr r1, r0, #0x18
	ldr r0, _022123AC ; =0x00000814
	add r3, r4, r2
	strb r1, [r3, r0]
	sub r0, #0xc4
	ldr r0, [r4, r0]
	bl ov96_022148E8
	pop {r3, r4, r5, pc}
	nop
_022123AC: .word 0x00000814
	thumb_func_end ov96_0221236C
