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
	.extern ov96_021E8318
	.extern ov96_021E8A20
	.include "asm/macros.inc"
	.include "overlay_96.inc"
	.include "global.inc"
	.include "overlay_96_exact_bank.inc"

.text
	thumb_func_start ov96_02207CCC
ov96_02207CCC: ; 0x02207CCC
	push {r3, r4, r5, r6, r7, lr}
	add r4, r1, #0
	mov r1, #0x72
	lsl r1, r1, #2
	add r6, r0, #0
	str r2, [sp]
	bl Heap_Alloc
	mov r2, #0x72
	add r7, r0, #0
	mov r1, #0
	lsl r2, r2, #2
	bl MI_CpuFill8
	str r6, [r7]
	ldr r0, [sp]
	str r4, [r7, #8]
	str r0, [r7, #4]
	add r0, r7, #0
	bl ov96_02208A4C
	ldr r2, _02207D60 ; =0x00000135
	mov r0, #1
	mov r1, #0x1b
	add r3, r6, #0
	bl NewMsgDataFromNarc
	str r0, [r7, #0x1c]
	add r0, r6, #0
	bl MessageFormat_New
	str r0, [r7, #0x20]
	mov r4, #0
	add r5, r7, #0
_02207D10:
	mov r0, #0xb
	add r1, r6, #0
	bl String_New
	mov r1, #0x6a
	lsl r1, r1, #2
	str r0, [r5, r1]
	add r4, r4, #1
	add r5, r5, #4
	cmp r4, #3
	blt _02207D10
	ldr r0, [sp]
	bl ov96_021E5F24
	add r1, r0, #0
	ldr r0, [sp]
	bl PokeathlonCourse_GetParticipantUnk04
	add r4, r0, #0
	mov r6, #0
	add r5, r7, #0
_02207D3A:
	mov r0, #0x6a
	lsl r0, r0, #2
	add r1, r4, #0
	ldr r0, [r5, r0]
	add r1, #0x12
	bl CopyU16ArrayToString
	add r6, r6, #1
	add r4, #0x28
	add r5, r5, #4
	cmp r6, #3
	blt _02207D3A
	add r0, r7, #0
	mov r1, #1
	bl ov96_022088AC
	add r0, r7, #0
	pop {r3, r4, r5, r6, r7, pc}
	nop
_02207D60: .word 0x00000135
	thumb_func_end ov96_02207CCC
