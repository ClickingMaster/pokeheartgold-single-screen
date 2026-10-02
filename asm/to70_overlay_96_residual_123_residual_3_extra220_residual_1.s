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
	.extern ov96_02215DBC
	.include "asm/macros.inc"
	.include "overlay_96.inc"
	.include "global.inc"
	.include "overlay_96_exact_bank.inc"

.text
	thumb_func_start ov96_0221A400
ov96_0221A400: ; 0x0221A400
	push {r3, r4, r5, lr}
	add r4, r0, #0
	ldr r1, [r4, #8]
	ldr r0, [r4]
	lsl r1, r1, #0x1e
	lsr r1, r1, #0x1e
	lsl r1, r1, #0x18
	lsr r1, r1, #0x18
	bl ov96_02215E94
	cmp r0, #0
	beq _0221A41A
	b _0221A568
_0221A41A:
	ldr r1, [r4, #8]
	lsl r0, r1, #0x1a
	lsr r0, r0, #0x1f
	beq _0221A4C4
	lsl r0, r1, #0x1b
	lsr r0, r0, #0x1f
	bne _0221A4C4
	lsl r1, r1, #0x1e
	lsr r1, r1, #0x1e
	lsl r1, r1, #0x18
	ldr r0, [r4]
	lsr r1, r1, #0x18
	bl ov96_02215DBC
	ldrb r1, [r4, #7]
	ldr r0, [r4]
	bl ov96_02215DBC
	cmp r0, #3
	beq _0221A462
	ldrb r1, [r4, #7]
	ldr r0, [r4]
	bl ov96_02215E94
	cmp r0, #0
	bne _0221A462
	ldr r1, [r4, #8]
	ldr r0, [r4]
	lsl r1, r1, #0x1e
	lsr r1, r1, #0x1e
	lsl r1, r1, #0x18
	lsr r1, r1, #0x18
	bl ov96_02215EB0
	cmp r0, #0
	beq _0221A49C
_0221A462:
	ldr r1, [r4, #8]
	ldr r0, [r4]
	lsl r1, r1, #0x1e
	lsr r1, r1, #0x1e
	lsl r1, r1, #0x18
	lsr r1, r1, #0x18
	bl ov96_02215DD4
	ldr r1, [r0]
	asr r2, r1, #0xc
	add r1, sp, #0
	strh r2, [r1]
	ldr r0, [r0, #4]
	add r2, sp, #0
	asr r0, r0, #0xc
	strh r0, [r1, #2]
	ldr r1, [r4, #8]
	ldr r0, [r4]
	lsl r1, r1, #0x1e
	lsr r1, r1, #0x1e
	lsl r1, r1, #0x18
	lsr r1, r1, #0x18
	bl ov96_02215F80
	ldr r1, [r4, #8]
	mov r0, #0x20
	bic r1, r0
	str r1, [r4, #8]
	b _0221A4C4
_0221A49C:
	ldrb r1, [r4, #7]
	ldr r0, [r4]
	bl ov96_02215DD4
	ldr r1, [r0]
	asr r2, r1, #0xc
	add r1, sp, #0
	strh r2, [r1]
	ldr r0, [r0, #4]
	add r2, sp, #0
	asr r0, r0, #0xc
	strh r0, [r1, #2]
	ldr r1, [r4, #8]
	ldr r0, [r4]
	lsl r1, r1, #0x1e
	lsr r1, r1, #0x1e
	lsl r1, r1, #0x18
	lsr r1, r1, #0x18
	bl ov96_02215F80
_0221A4C4:
	ldr r1, [r4, #8]
	lsl r0, r1, #0x1b
	lsr r0, r0, #0x1f
	beq _0221A504
	lsl r1, r1, #0x1e
	lsr r1, r1, #0x1e
	lsl r1, r1, #0x18
	ldr r0, [r4]
	lsr r1, r1, #0x18
	bl ov96_02215ECC
	cmp r0, #0
	beq _0221A568
	ldr r1, [r4, #8]
	ldr r0, [r4]
	lsl r1, r1, #0x1e
	lsr r1, r1, #0x1e
	lsl r1, r1, #0x18
	lsr r1, r1, #0x18
	bl ov96_02215DBC
	add r5, r0, #0
	add r0, r4, #0
	bl ov96_0221A314
	cmp r5, #3
	bne _0221A568
	ldr r1, [r4, #8]
	mov r0, #0x10
	bic r1, r0
	str r1, [r4, #8]
	pop {r3, r4, r5, pc}
_0221A504:
	mov r0, #4
	ldrsb r1, [r4, r0]
	sub r1, r1, #1
	strb r1, [r4, #4]
	ldrsb r0, [r4, r0]
	cmp r0, #0
	bgt _0221A568
	bge _0221A518
	mov r0, #0
	strb r0, [r4, #4]
_0221A518:
	mov r0, #6
	ldrsb r1, [r4, r0]
	sub r1, r1, #1
	strb r1, [r4, #6]
	ldrsb r0, [r4, r0]
	cmp r0, #0
	bgt _0221A568
	mov r0, #3
	strb r0, [r4, #6]
	ldr r1, [r4, #8]
	ldr r0, [r4]
	lsl r1, r1, #0x1e
	lsr r1, r1, #0x1e
	lsl r1, r1, #0x18
	lsr r1, r1, #0x18
	bl ov96_02215ECC
	cmp r0, #0
	beq _0221A568
	add r0, r4, #0
	bl ov96_0221A08C
	mov r0, #5
	ldrsb r1, [r4, r0]
	sub r1, r1, #1
	strb r1, [r4, #5]
	ldrsb r0, [r4, r0]
	cmp r0, #0
	bgt _0221A568
	add r0, r4, #0
	bl ov96_0221A3AC
	cmp r0, #0
	beq _0221A564
	ldr r1, [r4, #8]
	mov r0, #0x10
	orr r0, r1
	str r0, [r4, #8]
_0221A564:
	mov r0, #0
	strb r0, [r4, #5]
_0221A568:
	pop {r3, r4, r5, pc}
	.balign 4, 0
	thumb_func_end ov96_0221A400
