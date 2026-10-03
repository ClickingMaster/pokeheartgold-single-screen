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
	.extern ov96_021EB5E8
	.extern ov96_022004B4
	.include "asm/macros.inc"
	.include "overlay_96.inc"
	.include "global.inc"
	.include "overlay_96_exact_bank.inc"

.text
	thumb_func_start ov96_022002F8
ov96_022002F8: ; 0x022002F8
	push {r4, r5, r6, r7, lr}
	sub sp, #0x24
	add r6, r0, #0
	add r5, r1, #0
	ldr r1, [r6]
	mov r0, #0x14
	bl NARC_New
	str r0, [sp, #0x14]
	bl sub_02074490
	add r1, r0, #0
	mov r0, #0x60
	str r0, [sp]
	ldr r0, [r6]
	mov r2, #5
	str r0, [sp, #4]
	mov r0, #0x14
	mov r3, #0x20
	bl GfGfxLoader_GXLoadPal
	add r0, r6, #0
	str r0, [sp, #0x10]
	add r0, #0xec
	mov r7, #0
	add r4, r6, #0
	str r0, [sp, #0x10]
_0220032E:
	mov r1, #1
	ldr r0, [r6]
	lsl r1, r1, #0xc
	bl Heap_AllocAtEnd
	add r1, r4, #0
	add r1, #0xbc
	str r0, [r1]
	ldrh r0, [r5, #2]
	mov r1, #0
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	str r0, [sp, #0xc]
	ldrh r0, [r5]
	ldr r2, [sp, #0xc]
	str r0, [sp, #8]
	bl GetMonIconNaixEx
	add r2, r4, #0
	add r2, #0xbc
	add r1, r0, #0
	ldr r0, [sp, #0x14]
	ldr r2, [r2]
	bl NARC_ReadWholeMember
	add r0, r4, #0
	add r0, #0xbc
	ldr r0, [r0]
	ldr r1, [sp, #0x10]
	bl NNS_G2dGetUnpackedBGCharacterData
	ldr r0, [sp, #8]
	ldr r1, [sp, #0xc]
	mov r2, #0
	bl GetMonIconPaletteEx
	add r2, r0, #1
	mov r0, #0x47
	add r1, r6, r7
	lsl r0, r0, #2
	strb r2, [r1, r0]
	ldr r0, [sp, #0x10]
	add r7, r7, #1
	add r0, r0, #4
	add r4, r4, #4
	add r5, r5, #4
	str r0, [sp, #0x10]
	cmp r7, #0xc
	blt _0220032E
	ldr r0, [sp, #0x14]
	bl NARC_Delete
	mov r5, #0
	add r4, r6, #0
	add r7, sp, #0x18
_0220039C:
	lsl r1, r5, #0x18
	add r0, r6, #0
	lsr r1, r1, #0x18
	mov r2, #0
	bl ov96_022004B4
	mov r1, #1
	ldr r0, [r4, #0x48]
	add r2, r1, #0
	bl ov96_021EB52C
	mov r1, #1
	ldr r0, [r4, #0x54]
	add r2, r1, #0
	bl ov96_021EB52C
	mov r0, #0
	str r0, [sp, #0x20]
	mov r0, #0xa
	lsl r0, r0, #0x10
	str r0, [sp, #0x18]
	mov r0, #0x2b
	lsl r0, r0, #0x10
	str r0, [sp, #0x1c]
	ldr r0, [r4, #0x48]
	add r1, r7, #0
	bl ov96_021EB588
	ldr r0, [r4, #0x54]
	add r1, r7, #0
	bl ov96_021EB588
	add r5, r5, #1
	add r4, #0x20
	cmp r5, #4
	blt _0220039C
	add sp, #0x24
	pop {r4, r5, r6, r7, pc}
	thumb_func_end ov96_022002F8
