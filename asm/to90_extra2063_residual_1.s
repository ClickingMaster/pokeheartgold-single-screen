	.extern GF_AssertFail
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
	.extern _u32_div_f
	.extern ov96_021E5E04
	.extern ov96_021E5E7C
	.extern ov96_021E5F24
	.extern ov96_021E8228
	.extern ov96_021E8A20
	.extern ov96_0220D014
	.extern ov96_0220D33C
	.include "asm/macros.inc"
	.include "overlay_96.inc"
	.include "global.inc"
	.include "overlay_96_exact_bank.inc"

.text
	thumb_func_start ov96_0220CF50
ov96_0220CF50: ; 0x0220CF50
	push {r3, r4, r5, r6, r7, lr}
	add r5, r0, #0
	add r0, r2, #0
	add r4, r1, #0
	bl PokeathlonCourse_GetDataCopyArea
	ldr r1, [r5, #0x18]
	add r0, #0x50
	lsr r2, r1, #0x1e
	mov r1, #0x28
	mul r1, r2
	add r0, r0, r1
	bl ov96_021E8A20
	ldr r7, [r5, #0x14]
	mov r6, #0xff
	lsr r1, r7, #0x1e
	lsl r2, r1, #2
	ldr r1, [r0]
	add r3, r5, #4
	bic r1, r6
	lsl r6, r7, #0x18
	lsr r6, r6, #0x18
	lsl r6, r6, #0x18
	lsr r6, r6, #0x18
	orr r6, r1
	str r6, [r0]
	ldr r2, [r3, r2]
	ldr r1, _0220CFFC ; =0xFFFF00FF
	lsl r2, r2, #6
	lsr r2, r2, #0x18
	lsl r2, r2, #0x18
	and r1, r6
	lsr r2, r2, #0x10
	orr r2, r1
	ldr r1, _0220D000 ; =0xFF00FFFF
	str r2, [r0]
	and r1, r2
	ldr r2, [r5, #0x14]
	lsl r2, r2, #8
	lsr r2, r2, #0x18
	lsl r2, r2, #0x18
	lsr r2, r2, #8
	orr r2, r1
	ldr r1, _0220D004 ; =0xFBFFFFFF
	str r2, [r0]
	and r1, r2
	ldr r2, [r5, #0x1c]
	lsl r2, r2, #0x16
	lsr r2, r2, #0x1f
	lsl r2, r2, #0x1f
	lsr r2, r2, #5
	orr r2, r1
	ldr r1, _0220D008 ; =0xF7FFFFFF
	and r2, r1
	mov r1, #2
	and r1, r4
	lsl r1, r1, #0x1f
	lsr r1, r1, #4
	orr r2, r1
	ldr r1, _0220D00C ; =0xFCFFFFFF
	str r2, [r0]
	and r2, r1
	ldr r1, [r5, #0x14]
	lsr r1, r1, #0x1e
	lsl r1, r1, #0x1e
	lsr r1, r1, #6
	orr r1, r2
	str r1, [r0]
	ldr r1, [r5, #0x14]
	lsl r1, r1, #2
	lsr r1, r1, #0x1a
	cmp r1, #1
	bne _0220CFE8
	mov r3, #1
	b _0220CFEA
_0220CFE8:
	mov r3, #0
_0220CFEA:
	ldr r2, [r0]
	ldr r1, _0220D010 ; =0xEFFFFFFF
	and r2, r1
	lsl r1, r3, #0x1f
	lsr r1, r1, #3
	orr r1, r2
	str r1, [r0]
	pop {r3, r4, r5, r6, r7, pc}
	nop
_0220CFFC: .word 0xFFFF00FF
_0220D000: .word 0xFF00FFFF
_0220D004: .word 0xFBFFFFFF
_0220D008: .word 0xF7FFFFFF
_0220D00C: .word 0xFCFFFFFF
_0220D010: .word 0xEFFFFFFF
	thumb_func_end ov96_0220CF50
