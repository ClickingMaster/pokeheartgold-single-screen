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

.text
	thumb_func_start ov96_021F3EC0
ov96_021F3EC0: ; 0x021F3EC0
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x30
	str r0, [sp, #4]
	ldr r0, _021F3F7C ; =ov96_0221BFF4
	str r1, [sp, #8]
	ldr r2, [r0, #0x10]
	ldr r1, [r0, #0x14]
	str r2, [sp, #0x1c]
	str r1, [sp, #0x20]
	ldr r1, [r0, #0x18]
	ldr r0, [r0, #0x1c]
	add r5, sp, #0x1c
	add r4, sp, #0x14
	str r1, [sp, #0x14]
	str r0, [sp, #0x18]
	mov r6, #0
	mov r7, #3
_021F3EE2:
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
	blt _021F3EE2
	mov r0, #0
	ldr r4, [sp, #8]
	str r0, [sp, #0xc]
	add r7, sp, #0x14
	add r6, sp, #0x1c
	mov r5, #0x40
_021F3F10:
	ldr r1, [r6]
	ldr r2, [r7]
	lsl r1, r1, #0x18
	lsl r2, r2, #0x18
	ldr r0, [sp, #4]
	lsr r1, r1, #0x18
	lsr r2, r2, #0x18
	bl ov96_021EB4F4
	add r1, r4, #0
	add r1, #0xf8
	str r0, [r1]
	add r0, r4, #0
	add r0, #0xf8
	ldr r0, [r0]
	bl ov96_021EB5B8
	mov r1, #2
	str r0, [sp, #0x10]
	bl Sprite_SetDrawPriority
	mov r0, #0
	str r0, [sp, #0x2c]
	lsl r0, r5, #0xc
	str r0, [sp, #0x24]
	mov r0, #0x26
	lsl r0, r0, #0x10
	str r0, [sp, #0x28]
	ldr r0, [sp, #0x10]
	add r1, sp, #0x24
	bl Sprite_SetMatrix
	add r0, r4, #0
	add r0, #0xf8
	ldr r0, [r0]
	mov r1, #1
	mov r2, #0
	bl ov96_021EB52C
	ldr r0, [sp, #0xc]
	add r7, r7, #4
	add r0, r0, #1
	add r6, r6, #4
	add r4, r4, #4
	add r5, #0x80
	str r0, [sp, #0xc]
	cmp r0, #2
	blt _021F3F10
	ldr r0, [sp, #4]
	ldr r1, [sp, #8]
	bl ov96_021F440C
	add sp, #0x30
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
_021F3F7C: .word ov96_0221BFF4
	thumb_func_end ov96_021F3EC0
