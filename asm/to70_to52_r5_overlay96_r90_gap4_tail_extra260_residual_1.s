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
	.extern ov96_021EB5B8
	.include "asm/macros.inc"
	.include "overlay_96.inc"
	.include "global.inc"
	.include "overlay_96_exact_bank.inc"
	.extern ov96_022089D0

.text
	thumb_func_start ov96_02208914
ov96_02208914: ; 0x02208914
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x28
	str r0, [sp, #4]
	ldr r0, _022089CC ; =ov96_0221CBCC
	str r1, [sp, #8]
	ldr r2, [r0]
	ldr r1, [r0, #4]
	str r2, [sp, #0x14]
	str r1, [sp, #0x18]
	ldr r1, [r0, #0x10]
	ldr r0, [r0, #0x14]
	add r5, sp, #0x14
	add r4, sp, #0xc
	str r1, [sp, #0xc]
	str r0, [sp, #0x10]
	mov r6, #0
	mov r7, #3
_02208936:
	ldr r0, [r4]
	add r1, r7, #0
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	str r0, [sp]
	ldr r3, [r5]
	ldr r0, [sp, #4]
	lsl r3, r3, #0x18
	mov r2, #2
	lsr r3, r3, #0x18
	bl ov96_021EB408
	add r6, r6, #1
	add r4, r4, #4
	add r5, r5, #4
	cmp r6, #2
	blt _02208936
	ldr r4, [sp, #8]
	mov r5, #0
	add r7, sp, #0xc
	add r6, sp, #0x14
_02208960:
	ldr r1, [r6]
	ldr r2, [r7]
	lsl r1, r1, #0x18
	lsl r2, r2, #0x18
	ldr r0, [sp, #4]
	lsr r1, r1, #0x18
	lsr r2, r2, #0x18
	bl ov96_021EB4F4
	mov r1, #0x13
	lsl r1, r1, #4
	str r0, [r4, r1]
	add r0, r1, #0
	ldr r0, [r4, r0]
	bl ov96_021EB5B8
	mov r1, #0
	str r1, [sp, #0x24]
	mov r1, #0xa
	lsl r1, r1, #0xe
	str r1, [sp, #0x1c]
	mov r1, #1
	sub r2, r1, r5
	mov r1, #0x58
	mul r1, r2
	add r1, #0x30
	lsl r2, r1, #0xc
	mov r1, #0xaf
	lsl r1, r1, #0xe
	add r1, r2, r1
	str r1, [sp, #0x20]
	add r1, sp, #0x1c
	bl Sprite_SetMatrix
	mov r0, #0x13
	lsl r0, r0, #4
	mov r1, #1
	ldr r0, [r4, r0]
	add r2, r1, #0
	bl ov96_021EB52C
	add r5, r5, #1
	add r7, r7, #4
	add r6, r6, #4
	add r4, r4, #4
	cmp r5, #2
	blt _02208960
	ldr r0, [sp, #4]
	ldr r1, [sp, #8]
	bl ov96_022089D0
	add sp, #0x28
	pop {r3, r4, r5, r6, r7, pc}
	nop
_022089CC: .word ov96_0221CBCC
	thumb_func_end ov96_02208914
