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
	.extern ov96_02215944
	.include "asm/macros.inc"
	.include "overlay_96.inc"
	.include "global.inc"
	.include "overlay_96_exact_bank.inc"

.text
	thumb_func_start ov96_02218934
ov96_02218934: ; 0x02218934
	push {r4, r5, r6, r7, lr}
	sub sp, #0x3c
	ldr r3, [r0, #0x2c]
	cmp r3, #0
	ble _02218944
	ldr r2, [r0, #0x30]
	cmp r2, #0
	bgt _0221894A
_02218944:
	add sp, #0x3c
	mov r0, #1
	pop {r4, r5, r6, r7, pc}
_0221894A:
	add r1, sp, #4
	str r1, [sp]
	ldr r0, [r0, #4]
	asr r1, r3, #0xc
	ldr r0, [r0]
	asr r2, r2, #0xc
	add r3, sp, #8
	bl ov96_021EB06C
	ldr r4, [sp, #4]
	cmp r4, #0x40
	blt _02218A46
	cmp r4, #0xa0
	bgt _02218A46
	ldr r0, [sp, #8]
	cmp r0, #0
	ble _0221897E
	lsl r0, r0, #0xc
	bl _fflt
	add r1, r0, #0
	mov r0, #0x3f
	lsl r0, r0, #0x18
	bl _fadd
	b _0221898C
_0221897E:
	lsl r0, r0, #0xc
	bl _fflt
	mov r1, #0x3f
	lsl r1, r1, #0x18
	bl _fsub
_0221898C:
	bl _ffix
	add r7, r0, #0
	cmp r4, #0
	ble _022189A8
	lsl r0, r4, #0xc
	bl _fflt
	add r1, r0, #0
	mov r0, #0x3f
	lsl r0, r0, #0x18
	bl _fadd
	b _022189B6
_022189A8:
	lsl r0, r4, #0xc
	bl _fflt
	mov r1, #0x3f
	lsl r1, r1, #0x18
	bl _fsub
_022189B6:
	bl _ffix
	add r4, r0, #0
	mov r0, #0
	ldr r1, _02218A4C ; =ov96_0221D6D4
	add r2, sp, #0xc
	add r3, r0, #0
	mov r5, #2
_022189C6:
	ldrsh r6, [r1, r3]
	add r0, r0, #1
	lsl r6, r6, #0xc
	str r6, [r2]
	ldrsh r6, [r1, r5]
	add r1, r1, #4
	lsl r6, r6, #0xc
	str r6, [r2, #4]
	add r2, #0xc
	cmp r0, #4
	blt _022189C6
	ldr r5, [sp, #0xc]
	ldr r1, [sp, #0x1c]
	ldr r0, [sp, #0x10]
	sub r2, r7, r5
	sub r0, r1, r0
	asr r1, r0, #0x1f
	asr r3, r2, #0x1f
	bl _ll_mul
	mov r2, #2
	mov r3, #0
	lsl r2, r2, #0xa
	add r0, r0, r2
	adc r1, r3
	lsl r1, r1, #0x14
	lsr r0, r0, #0xc
	orr r0, r1
	ldr r1, [sp, #0x18]
	sub r1, r1, r5
	bl FX_Div
	ldr r1, [sp, #0x10]
	ldr r5, [sp, #0x24]
	add r6, r0, r1
	sub r2, r7, r5
	ldr r1, [sp, #0x34]
	ldr r0, [sp, #0x28]
	asr r3, r2, #0x1f
	sub r0, r1, r0
	asr r1, r0, #0x1f
	bl _ll_mul
	mov r2, #2
	mov r3, #0
	lsl r2, r2, #0xa
	add r0, r0, r2
	adc r1, r3
	lsl r1, r1, #0x14
	lsr r0, r0, #0xc
	orr r0, r1
	ldr r1, [sp, #0x30]
	sub r1, r1, r5
	bl FX_Div
	ldr r1, [sp, #0x28]
	add r0, r0, r1
	cmp r4, r6
	blt _02218A46
	cmp r4, r0
	blt _02218A46
	add sp, #0x3c
	mov r0, #0
	pop {r4, r5, r6, r7, pc}
_02218A46:
	mov r0, #1
	add sp, #0x3c
	pop {r4, r5, r6, r7, pc}
	.balign 4, 0
_02218A4C: .word ov96_0221D6D4
	thumb_func_end ov96_02218934
