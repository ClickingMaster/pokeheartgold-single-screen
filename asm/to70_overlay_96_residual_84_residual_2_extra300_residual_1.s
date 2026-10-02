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
	.extern ov96_021EB564
	.extern ov96_021EB588
	.extern ov96_021EB630
	.include "asm/macros.inc"
	.include "overlay_96.inc"
	.include "global.inc"
	.include "overlay_96_exact_bank.inc"
	.extern ov96_02203F0C

.text
	thumb_func_start ov96_02203E30
ov96_02203E30: ; 0x02203E30
	push {r4, r5, r6, r7, lr}
	sub sp, #0x1c
	add r7, r0, #0
	str r1, [sp, #4]
	str r2, [sp, #8]
	mov r4, #0
	mov r5, #2
	mov r6, #3
_02203E40:
	add r0, r7, #0
	add r1, r6, #0
	add r2, r5, #0
	mov r3, #0x65
	str r5, [sp]
	bl ov96_021EB408
	add r4, r4, #1
	cmp r4, #3
	blt _02203E40
	mov r0, #0x10
	ldr r6, [sp, #8]
	ldr r4, [sp, #4]
	mov r5, #0
	str r0, [sp, #0xc]
_02203E5E:
	add r0, r7, #0
	mov r1, #0x65
	mov r2, #2
	bl ov96_021EB4F4
	str r0, [r4, #0x54]
	mov r0, #0
	str r0, [sp, #0x18]
	ldr r0, [sp, #0xc]
	mov r2, #0
	lsl r0, r0, #0xc
	str r0, [sp, #0x10]
	ldr r0, [r6, #0xc]
	str r0, [sp]
	ldrh r3, [r6, #2]
	ldrh r0, [r6]
	ldrb r1, [r6, #7]
	lsl r3, r3, #0x18
	lsr r3, r3, #0x18
	bl GetMonPicHeightBySpeciesGenderForm
	mov r1, #0x5e
	lsl r1, r1, #2
	add r0, r0, r1
	lsl r0, r0, #0xc
	str r0, [sp, #0x14]
	ldr r0, [r4, #0x54]
	add r1, sp, #0x10
	bl ov96_021EB588
	ldr r0, [r4, #0x54]
	add r1, r5, #1
	bl ov96_021EB564
	mov r1, #1
	ldr r0, [r4, #0x54]
	add r2, r1, #0
	bl ov96_021EB52C
	mov r1, #0x13
	ldr r0, [r4, #0x54]
	sub r1, r1, r5
	bl ov96_021EB630
	add r0, r7, #0
	mov r1, #0x65
	mov r2, #5
	bl ov96_021EB4F4
	str r0, [r4, #0x60]
	mov r1, #0
	bl ov96_021EB564
	mov r0, #1
	ldr r1, [sp, #0x14]
	lsl r0, r0, #0x10
	sub r0, r1, r0
	str r0, [sp, #0x14]
	ldr r0, [r4, #0x60]
	add r1, sp, #0x10
	bl ov96_021EB588
	ldr r0, [r4, #0x60]
	mov r1, #4
	bl ov96_021EB630
	ldr r0, [sp, #0xc]
	add r5, r5, #1
	add r0, #0x32
	add r6, #0x10
	add r4, r4, #4
	str r0, [sp, #0xc]
	cmp r5, #3
	blt _02203E5E
	ldr r0, [sp, #4]
	ldr r1, [sp, #8]
	bl ov96_02203DCC
	ldr r1, [sp, #4]
	add r0, r7, #0
	bl ov96_02203F50
	ldr r0, [sp, #4]
	bl ov96_02203F0C
	add sp, #0x1c
	pop {r4, r5, r6, r7, pc}
	thumb_func_end ov96_02203E30
