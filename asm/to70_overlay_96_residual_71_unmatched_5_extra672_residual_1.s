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
	.extern ov96_021E8A20
	.include "asm/macros.inc"
	.include "overlay_96.inc"
	.include "global.inc"
	.include "overlay_96_exact_bank.inc"

.text
	thumb_func_start ov96_021FB630
ov96_021FB630: ; 0x021FB630
	push {r4, r5, r6, r7, lr}
	sub sp, #0x1c
	str r0, [sp, #8]
	bl ov96_021E5F24
	cmp r0, #0
	bne _021FB6B0
	ldr r0, [sp, #8]
	bl PokeathlonCourse_GetDataCopyArea
	add r4, r0, #0
	ldr r0, [sp, #8]
	bl PokeathlonCourse_GetHeapAllocPtr4
	str r0, [sp, #0x18]
	ldr r0, [sp, #8]
	bl PokeathlonCourse_GetParticipantCount
	str r0, [sp, #0x14]
	str r0, [sp, #0x10]
	cmp r0, #4
	bge _021FB6B0
	mov r1, #0x28
	add r4, #0x50
	mul r1, r0
	add r0, r4, r1
	str r0, [sp, #0xc]
_021FB666:
	ldr r0, [sp, #0xc]
	bl ov96_021E8A20
	add r7, r0, #0
	ldr r1, [sp, #0x10]
	ldr r0, [sp, #0x14]
	mov r4, #0
	sub r1, r1, r0
	lsl r0, r1, #1
	add r5, r1, r0
	ldr r0, [sp, #0x10]
	lsl r0, r0, #0x18
	lsr r6, r0, #0x18
_021FB680:
	lsl r0, r4, #0x18
	lsr r0, r0, #0x18
	str r0, [sp]
	lsl r0, r5, #0x18
	lsr r0, r0, #0x18
	str r0, [sp, #4]
	ldr r0, [sp, #8]
	ldr r1, [sp, #0x18]
	add r2, r7, #0
	add r3, r6, #0
	bl ov96_021FBA3C
	add r4, r4, #1
	add r5, r5, #1
	cmp r4, #3
	blt _021FB680
	ldr r0, [sp, #0xc]
	add r0, #0x28
	str r0, [sp, #0xc]
	ldr r0, [sp, #0x10]
	add r0, r0, #1
	str r0, [sp, #0x10]
	cmp r0, #4
	blt _021FB666
_021FB6B0:
	add sp, #0x1c
	pop {r4, r5, r6, r7, pc}
	thumb_func_end ov96_021FB630
