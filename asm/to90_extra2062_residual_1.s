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
	.include "asm/macros.inc"
	.include "overlay_96.inc"
	.include "global.inc"
	.include "overlay_96_exact_bank.inc"

.text
	thumb_func_start ov96_0220B634
ov96_0220B634: ; 0x0220B634
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #8
	add r5, r0, #0
	add r7, r1, #0
	ldr r0, _0220B6E4 ; =ov96_0221E5A4
	str r2, [sp, #4]
	mov r1, #0
	str r1, [r0]
	ldr r0, [r5, #0x40]
	lsl r0, r0, #0x16
	lsr r0, r0, #0x18
	cmp r0, #2
	bne _0220B652
	bl GF_AssertFail
_0220B652:
	ldr r0, [r5, #0x40]
	lsl r1, r0, #0x16
	lsr r1, r1, #0x18
	cmp r1, #1
	bne _0220B69C
	lsl r0, r0, #0x1e
	add r4, r5, #0
	lsr r0, r0, #0x1e
	ldr r1, [r5, #0x44]
	add r4, #0x34
	lsl r6, r0, #2
	ldr r0, [r4, r6]
	lsl r1, r1, #8
	lsr r1, r1, #0x18
	lsl r0, r0, #6
	lsl r1, r1, #0x10
	lsr r0, r0, #0x18
	lsr r1, r1, #0x10
	bl ov96_0220D33C
	lsl r0, r0, #0x18
	ldr r2, [r4, r6]
	ldr r1, _0220B6E8 ; =0xFC03FFFF
	lsr r0, r0, #6
	and r1, r2
	orr r0, r1
	str r0, [r4, r6]
	ldr r0, [r4, r6]
	lsl r0, r0, #6
	lsr r0, r0, #0x18
	bne _0220B69C
	add r0, r5, #0
	mov r1, #2
	bl ov96_0220C578
	add sp, #8
	pop {r3, r4, r5, r6, r7, pc}
_0220B69C:
	ldr r0, [r5, #0x44]
	lsl r0, r0, #0x17
	lsr r0, r0, #0x1f
	bne _0220B6AE
	ldr r2, [sp, #4]
	add r0, r5, #0
	add r1, r7, #0
	bl ov96_0220C714
_0220B6AE:
	ldr r0, [r5, #0x10]
	mov r1, #2
	bl ManagedSprite_SetAnim
	ldr r0, [r5, #0xc]
	bl ov96_021E5F24
	add r1, r0, #0
	mov r0, #1
	str r0, [sp]
	ldr r2, [r5, #0x40]
	lsl r1, r1, #0x18
	lsl r2, r2, #0x1e
	lsr r2, r2, #0x1e
	lsl r2, r2, #0x18
	ldr r0, [r5, #0xc]
	lsr r1, r1, #0x18
	lsr r2, r2, #0x18
	mov r3, #4
	bl ov96_021E8228
	add r0, r5, #0
	mov r1, #1
	bl ov96_0220C578
	add sp, #8
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
_0220B6E4: .word ov96_0221E5A4
_0220B6E8: .word 0xFC03FFFF
	thumb_func_end ov96_0220B634
