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
	thumb_func_start ov96_021FF2A0
ov96_021FF2A0: ; 0x021FF2A0
	push {r4, r5, r6, r7, lr}
	sub sp, #0xa4
	str r0, [sp, #4]
	ldr r0, [sp, #0xb8]
	str r1, [sp, #8]
	str r0, [sp, #0xb8]
	add r0, r3, #0
	ldr r0, [r0]
	str r2, [sp, #0xc]
	str r3, [sp, #0x10]
	cmp r0, #3
	bne _021FF2BE
	add sp, #0xa4
	mov r0, #0
	pop {r4, r5, r6, r7, pc}
_021FF2BE:
	ldr r0, [sp, #4]
	add r7, r3, #0
	ldr r1, [r0]
	add r6, r3, #0
	asr r0, r1, #0xb
	lsr r0, r0, #0x14
	add r0, r1, r0
	asr r0, r0, #0xc
	str r0, [sp, #0x24]
	ldr r0, [sp, #4]
	add r5, r3, #0
	ldr r1, [r0, #4]
	ldr r2, _021FF56C ; =ov96_0221C5FC
	asr r0, r1, #0xb
	lsr r0, r0, #0x14
	add r0, r1, r0
	asr r0, r0, #0xc
	str r0, [sp, #0x28]
	ldr r0, [sp, #8]
	add r3, sp, #0x34
	ldr r1, [r0]
	mov r4, #0
	asr r0, r1, #0xb
	lsr r0, r0, #0x14
	add r0, r1, r0
	asr r0, r0, #0xc
	str r0, [sp, #0x1c]
	ldr r0, [sp, #8]
	add r7, #0x2c
	ldr r1, [r0, #4]
	add r6, #0x24
	asr r0, r1, #0xb
	lsr r0, r0, #0x14
	add r0, r1, r0
	asr r0, r0, #0xc
	str r0, [sp, #0x20]
	ldmia r2!, {r0, r1}
	stmia r3!, {r0, r1}
	ldmia r2!, {r0, r1}
	stmia r3!, {r0, r1}
_021FF30E:
	add r3, sp, #0x34
	add r2, sp, #0x94
	ldmia r3!, {r0, r1}
	stmia r2!, {r0, r1}
	ldmia r3!, {r0, r1}
	stmia r2!, {r0, r1}
	add r0, sp, #0x2c
	str r0, [sp]
	add r0, r6, #0
	add r1, r7, #0
	add r2, sp, #0x24
	add r3, sp, #0x1c
	bl sub_02020F4C
	cmp r0, #0
	beq _021FF34C
	ldr r0, [sp, #0x2c]
	lsl r1, r0, #0xc
	ldr r0, [sp, #0xb8]
	str r1, [r0]
	ldr r0, [sp, #0x30]
	lsl r1, r0, #0xc
	ldr r0, [sp, #0xb8]
	str r1, [r0, #4]
	mov r1, #0
	str r1, [r0, #8]
	add r0, sp, #0x94
	lsl r1, r4, #2
	add sp, #0xa4
	ldr r0, [r0, r1]
	pop {r4, r5, r6, r7, pc}
_021FF34C:
	mov r0, #0
	cmp r4, #0
	bne _021FF37C
	cmp r4, #2
	bne _021FF37C
	ldr r2, [sp, #0x28]
	ldr r1, [r5, #0x28]
	cmp r2, r1
	bne _021FF3A0
	ldr r2, [r5, #0x2c]
	ldr r3, [r5, #0x24]
	cmp r3, r2
	bge _021FF36A
	add r1, r3, #0
	b _021FF36E
_021FF36A:
	add r1, r2, #0
	add r2, r3, #0
_021FF36E:
	ldr r3, [sp, #0x24]
	cmp r1, r3
	bgt _021FF3A0
	cmp r3, r2
	bgt _021FF3A0
	mov r0, #1
	b _021FF3A0
_021FF37C:
	ldr r2, [sp, #0x24]
	ldr r1, [r5, #0x24]
	cmp r2, r1
	bne _021FF3A0
	ldr r2, [r5, #0x30]
	ldr r3, [r5, #0x28]
	cmp r3, r2
	bge _021FF390
	add r1, r3, #0
	b _021FF394
_021FF390:
	add r1, r2, #0
	add r2, r3, #0
_021FF394:
	ldr r3, [sp, #0x28]
	cmp r1, r3
	bgt _021FF3A0
	cmp r3, r2
	bgt _021FF3A0
	mov r0, #1
_021FF3A0:
	cmp r0, #0
	beq _021FF3C2
	ldr r2, [sp, #4]
	ldmia r2!, {r0, r1}
	str r2, [sp, #4]
	ldr r2, [sp, #0xb8]
	stmia r2!, {r0, r1}
	ldr r0, [sp, #4]
	str r2, [sp, #0xb8]
	ldr r1, [r0]
	add r0, r2, #0
	str r1, [r0]
	add r0, sp, #0x94
	lsl r1, r4, #2
	add sp, #0xa4
	ldr r0, [r0, r1]
	pop {r4, r5, r6, r7, pc}
_021FF3C2:
	add r4, r4, #1
	add r7, #0x10
	add r6, #0x10
	add r5, #0x10
	cmp r4, #4
	blt _021FF30E
	ldr r3, _021FF570 ; =ov96_0221C60C
	add r2, sp, #0x44
	ldmia r3!, {r0, r1}
	stmia r2!, {r0, r1}
	ldmia r3!, {r0, r1}
	stmia r2!, {r0, r1}
	ldr r0, [sp, #0xc]
	ldr r4, [sp, #0x10]
	mov r5, #0
	lsl r6, r0, #0xc
	add r7, sp, #0x6c
_021FF3E4:
	add r3, sp, #0x44
	add r2, sp, #0x84
	ldmia r3!, {r0, r1}
	stmia r2!, {r0, r1}
	ldmia r3!, {r0, r1}
	stmia r2!, {r0, r1}
	ldr r0, [r4, #4]
	add r1, sp, #0x78
	lsl r0, r0, #0xc
	str r0, [sp, #0x78]
	ldr r0, [r4, #8]
	add r2, r7, #0
	lsl r0, r0, #0xc
	str r0, [sp, #0x7c]
	mov r0, #0
	str r0, [sp, #0x80]
	ldr r0, [sp, #8]
	bl VEC_Subtract
	add r0, r7, #0
	bl VEC_Mag
	cmp r0, r6
	bgt _021FF452
	ldr r3, [sp, #0xc]
	ldr r0, [sp, #4]
	ldr r1, [sp, #8]
	add r2, sp, #0x78
	lsl r3, r3, #0xc
	bl ov96_021FF67C
	add r4, r0, #0
	ldr r0, [sp, #8]
	ldr r1, [sp, #4]
	add r2, sp, #0x60
	bl VEC_Subtract
	ldr r2, [sp, #4]
	add r0, r4, #0
	add r1, sp, #0x60
	add r3, sp, #0x54
	bl VEC_MultAdd
	ldr r1, [sp, #0x54]
	ldr r0, [sp, #0xb8]
	str r1, [r0]
	ldr r1, [sp, #0x58]
	str r1, [r0, #4]
	mov r1, #0
	str r1, [r0, #8]
	add r0, sp, #0x84
	lsl r1, r5, #2
	add sp, #0xa4
	ldr r0, [r0, r1]
	pop {r4, r5, r6, r7, pc}
_021FF452:
	add r5, r5, #1
	add r4, #8
	cmp r5, #4
	blt _021FF3E4
	ldr r0, [sp, #8]
	ldr r1, [r0]
	ldr r0, [sp, #0x10]
	ldr r0, [r0, #4]
	lsl r0, r0, #0xc
	cmp r0, r1
	bgt _021FF564
	ldr r0, [sp, #0x10]
	ldr r0, [r0, #0x14]
	lsl r0, r0, #0xc
	cmp r1, r0
	bgt _021FF564
	ldr r0, [sp, #8]
	ldr r1, [r0, #4]
	ldr r0, [sp, #0x10]
	ldr r0, [r0, #8]
	lsl r0, r0, #0xc
	cmp r0, r1
	bgt _021FF564
	ldr r0, [sp, #0x10]
	ldr r0, [r0, #0x18]
	lsl r0, r0, #0xc
	cmp r1, r0
	bgt _021FF564
	ldr r3, [sp, #8]
	ldr r2, [sp, #0xb8]
	ldmia r3!, {r0, r1}
	stmia r2!, {r0, r1}
	ldr r0, [r3]
	mov r1, #0
	str r0, [r2]
	ldr r0, [sp, #0x10]
	ldr r3, [sp, #0x10]
	ldr r0, [r0, #4]
	ldr r3, [r3, #0x14]
	str r0, [sp, #0x18]
	ldr r0, [sp, #8]
	str r3, [sp, #0x14]
	ldr r2, [r0]
	ldr r0, [sp, #0x18]
	lsl r3, r3, #0xc
	lsl r0, r0, #0xc
	sub r0, r0, r2
	sub r3, r3, r2
	mov ip, r1
	cmp r0, #0
	bge _021FF4BC
	sub r2, r1, #1
	mul r0, r2
_021FF4BC:
	cmp r3, #0
	bge _021FF4C6
	mov r2, #0
	mvn r2, r2
	mul r3, r2
_021FF4C6:
	cmp r0, r3
	bge _021FF4CE
	mov r6, #0xd
	b _021FF4D2
_021FF4CE:
	add r0, r3, #0
	mov r6, #0xb
_021FF4D2:
	ldr r2, [sp, #0x10]
	ldr r3, [sp, #8]
	ldr r4, [sp, #0x10]
	ldr r2, [r2, #8]
	ldr r5, [r3, #4]
	ldr r7, [r4, #0x18]
	lsl r3, r2, #0xc
	lsl r4, r7, #0xc
	sub r3, r3, r5
	sub r4, r4, r5
	cmp r3, #0
	bge _021FF4F0
	mov r5, #0
	mvn r5, r5
	mul r3, r5
_021FF4F0:
	cmp r4, #0
	bge _021FF4FA
	mov r5, #0
	mvn r5, r5
	mul r4, r5
_021FF4FA:
	cmp r3, r4
	bge _021FF502
	mov r4, #0xa
	b _021FF506
_021FF502:
	add r3, r4, #0
	mov r4, #0xc
_021FF506:
	cmp r0, r3
	bgt _021FF512
	mov r0, #1
	mov ip, r0
	add r0, r6, #0
	b _021FF516
_021FF512:
	mov r1, #1
	add r0, r4, #0
_021FF516:
	mov r3, ip
	cmp r3, #0
	beq _021FF540
	cmp r6, #0xd
	bne _021FF530
	ldr r2, [sp, #0x18]
	ldr r1, [sp, #0xc]
	sub r1, r2, r1
	lsl r2, r1, #0xc
	ldr r1, [sp, #0xb8]
	add sp, #0xa4
	str r2, [r1]
	pop {r4, r5, r6, r7, pc}
_021FF530:
	ldr r2, [sp, #0x14]
	ldr r1, [sp, #0xc]
	add r1, r2, r1
	lsl r2, r1, #0xc
	ldr r1, [sp, #0xb8]
	add sp, #0xa4
	str r2, [r1]
	pop {r4, r5, r6, r7, pc}
_021FF540:
	cmp r1, #0
	beq _021FF566
	cmp r4, #0xa
	bne _021FF556
	ldr r1, [sp, #0xc]
	sub r1, r2, r1
	lsl r2, r1, #0xc
	ldr r1, [sp, #0xb8]
	add sp, #0xa4
	str r2, [r1, #4]
	pop {r4, r5, r6, r7, pc}
_021FF556:
	ldr r1, [sp, #0xc]
	add r1, r7, r1
	lsl r2, r1, #0xc
	ldr r1, [sp, #0xb8]
	add sp, #0xa4
	str r2, [r1, #4]
	pop {r4, r5, r6, r7, pc}
_021FF564:
	mov r0, #0
_021FF566:
	add sp, #0xa4
	pop {r4, r5, r6, r7, pc}
	nop
_021FF56C: .word ov96_0221C5FC
_021FF570: .word ov96_0221C60C
	thumb_func_end ov96_021FF2A0
