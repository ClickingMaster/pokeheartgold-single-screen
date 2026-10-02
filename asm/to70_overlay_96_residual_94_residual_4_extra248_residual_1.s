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
	thumb_func_start ov96_0220B1D8
ov96_0220B1D8: ; 0x0220B1D8
	push {r4, r5, r6, r7, lr}
	sub sp, #0xc
	str r1, [sp]
	mov r4, #0
	add r5, r0, #0
	add r7, r2, #0
	str r3, [sp, #4]
	add r6, r4, #0
_0220B1E8:
	ldr r0, [r5, #4]
	cmp r0, #0
	bne _0220B2C0
	mov r0, #1
	str r0, [r5, #4]
	strh r7, [r5, #0x10]
	ldr r0, [sp, #4]
	strh r0, [r5, #0x12]
	bl MTRandom
	mov r1, #7
	and r0, r1
	add r0, r0, #4
	beq _0220B220
	bl MTRandom
	mov r1, #7
	and r0, r1
	add r0, r0, #4
	lsl r0, r0, #0xc
	bl _ffltu
	add r1, r0, #0
	mov r0, #0x3f
	lsl r0, r0, #0x18
	bl _fadd
	b _0220B238
_0220B220:
	bl MTRandom
	mov r1, #7
	and r0, r1
	add r0, r0, #4
	lsl r0, r0, #0xc
	bl _ffltu
	mov r1, #0x3f
	lsl r1, r1, #0x18
	bl _fsub
_0220B238:
	lsr r2, r6, #0x1f
	lsl r1, r6, #0x1f
	add r3, r0, #0
	sub r1, r1, r2
	mov r0, #0x1f
	ror r1, r0
	add r0, r2, r1
	bne _0220B24E
	mov r0, #1
	str r0, [sp, #8]
	b _0220B254
_0220B24E:
	mov r0, #0
	mvn r0, r0
	str r0, [sp, #8]
_0220B254:
	add r0, r3, #0
	bl _ffix
	ldr r1, [sp, #8]
	mul r0, r1
	str r0, [r5, #8]
	bl MTRandom
	mov r1, #7
	and r0, r1
	add r0, #8
	beq _0220B288
	bl MTRandom
	mov r1, #7
	and r0, r1
	add r0, #8
	lsl r0, r0, #0xc
	bl _ffltu
	add r1, r0, #0
	mov r0, #0x3f
	lsl r0, r0, #0x18
	bl _fadd
	b _0220B2A0
_0220B288:
	bl MTRandom
	mov r1, #7
	and r0, r1
	add r0, #8
	lsl r0, r0, #0xc
	bl _ffltu
	mov r1, #0x3f
	lsl r1, r1, #0x18
	bl _fsub
_0220B2A0:
	bl _ffix
	str r0, [r5, #0xc]
	ldr r0, [r5]
	ldr r2, [sp, #4]
	add r1, r7, #0
	bl ManagedSprite_SetPositionXY
	ldr r0, [r5]
	bl ManagedSprite_ResetSpriteAnimCtrlState
	ldr r0, [r5]
	mov r1, #1
	bl ManagedSprite_SetDrawFlag
	add r4, r4, #1
_0220B2C0:
	ldr r0, [sp]
	cmp r4, r0
	bge _0220B2CE
	add r6, r6, #1
	add r5, #0x14
	cmp r6, #0x10
	blt _0220B1E8
_0220B2CE:
	add sp, #0xc
	pop {r4, r5, r6, r7, pc}
	.balign 4, 0
	thumb_func_end ov96_0220B1D8
