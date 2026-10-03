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
	.include "asm/macros.inc"
	.include "overlay_96.inc"
	.include "global.inc"
	.include "overlay_96_exact_bank.inc"

.text
	thumb_func_start ov96_021FD2E0
ov96_021FD2E0: ; 0x021FD2E0
	push {r4, r5, r6, r7, lr}
	sub sp, #0x2c
	str r1, [sp, #0x10]
	str r0, [sp, #0xc]
	bl PokeathlonCourse_GetHeapAllocPtr4
	str r0, [sp, #0x18]
	ldr r0, [sp, #0x10]
	ldrb r0, [r0]
	cmp r0, #0
	beq _021FD300
	cmp r0, #1
	beq _021FD3A0
	cmp r0, #2
	beq _021FD3C6
	b _021FD3D6
_021FD300:
	mov r0, #0
	str r0, [sp, #0x1c]
	ldr r0, [sp, #0x18]
	ldr r1, _021FD3DC ; =0x000004D4
	str r0, [sp, #0x14]
	add r5, r0, #0
	add r7, r0, r1
_021FD30E:
	ldr r1, [sp, #0x14]
	ldr r0, _021FD3E0 ; =0x0000045C
	ldr r0, [r1, r0]
	lsl r1, r0, #0x18
	asr r2, r0, #8
	asr r0, r0, #0x10
	lsl r2, r2, #0x18
	lsl r0, r0, #0x10
	lsr r1, r1, #0x18
	lsr r6, r2, #0x18
	lsr r4, r0, #0x10
	cmp r1, #0
	beq _021FD370
	ldr r0, _021FD3DC ; =0x000004D4
	mov r2, #1
	str r2, [r5, r0]
	add r0, #0xb
	strb r1, [r5, r0]
	ldr r0, _021FD3E4 ; =0x000004DC
	sub r1, r1, #1
	strh r4, [r5, r0]
	add r0, r0, #2
	strb r6, [r5, r0]
	ldr r0, _021FD3E8 ; =0x000004D8
	ldr r0, [r5, r0]
	bl ov96_021EB564
	ldr r0, _021FD3E8 ; =0x000004D8
	mov r1, #1
	ldr r0, [r5, r0]
	add r2, r1, #0
	bl ov96_021EB52C
	mov r0, #0
	add r4, #0x50
	str r0, [sp, #0x28]
	lsl r0, r4, #0xc
	add r6, #0x20
	str r0, [sp, #0x20]
	lsl r0, r6, #0xc
	str r0, [sp, #0x24]
	ldr r0, _021FD3E8 ; =0x000004D8
	add r1, sp, #0x20
	ldr r0, [r5, r0]
	bl ov96_021EB588
	add r0, r7, #0
	bl ov96_021FFB44
_021FD370:
	ldr r0, [sp, #0x14]
	add r5, #0xc
	add r0, r0, #4
	str r0, [sp, #0x14]
	ldr r0, [sp, #0x1c]
	add r7, #0xc
	add r0, r0, #1
	str r0, [sp, #0x1c]
	cmp r0, #0x1e
	blt _021FD30E
	ldr r0, [sp, #0x18]
	mov r1, #0
	bl ov96_021FEAEC
	ldr r0, [sp, #0x10]
	ldrb r0, [r0]
	add r1, r0, #1
	ldr r0, [sp, #0x10]
	strb r1, [r0]
	ldr r0, [sp, #0xc]
	mov r1, #0x12
	bl PokeathlonCourse_SetStateTransitionType
	b _021FD3D6
_021FD3A0:
	mov r0, #6
	str r0, [sp]
	mov r0, #1
	str r0, [sp, #4]
	ldr r0, [sp, #0x18]
	mov r1, #3
	ldr r0, [r0, #0x14]
	add r2, r1, #0
	str r0, [sp, #8]
	mov r0, #2
	mov r3, #0
	bl BeginNormalPaletteFade
	ldr r0, [sp, #0x10]
	ldrb r0, [r0]
	add r1, r0, #1
	ldr r0, [sp, #0x10]
	strb r1, [r0]
	b _021FD3D6
_021FD3C6:
	bl IsPaletteFadeFinished
	cmp r0, #0
	beq _021FD3D6
	ldr r0, [sp, #0xc]
	mov r1, #1
	bl PokeathlonCourse_SetStateField07
_021FD3D6:
	mov r0, #0
	add sp, #0x2c
	pop {r4, r5, r6, r7, pc}
	.balign 4, 0
_021FD3DC: .word 0x000004D4
_021FD3E0: .word 0x0000045C
_021FD3E4: .word 0x000004DC
_021FD3E8: .word 0x000004D8
	thumb_func_end ov96_021FD2E0
