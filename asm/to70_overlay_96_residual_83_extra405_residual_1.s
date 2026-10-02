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
	.extern ov96_021EB570
	.include "asm/macros.inc"
	.include "overlay_96.inc"
	.include "global.inc"
	.include "overlay_96_exact_bank.inc"

.text
	thumb_func_start ov96_02203BD0
ov96_02203BD0: ; 0x02203BD0
	push {r4, r5, r6, r7, lr}
	sub sp, #0x24
	add r6, r1, #0
	add r5, r0, #0
	add r0, r6, #0
	mov r1, #0x64
	bl _s32_div_f
	lsl r0, r0, #0x10
	lsr r4, r0, #0x10
	add r0, r4, #0
	mov r1, #5
	bl _s32_div_f
	lsl r1, r1, #2
	add r0, sp, #0x1c
	strb r1, [r0, #3]
	add r0, r4, #0
	mov r1, #5
	bl _s32_div_f
	lsl r1, r0, #3
	add r0, sp, #0x1c
	strb r1, [r0]
	mov r0, #0x64
	mul r0, r4
	sub r0, r6, r0
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	mov r1, #0xa
	bl _u32_div_f
	lsl r0, r0, #0x10
	lsr r4, r0, #0x10
	add r0, r4, #0
	mov r1, #5
	bl _s32_div_f
	lsl r1, r1, #2
	add r0, sp, #0x1c
	strb r1, [r0, #4]
	add r0, r4, #0
	mov r1, #5
	bl _s32_div_f
	lsl r1, r0, #3
	add r0, sp, #0x1c
	strb r1, [r0, #1]
	add r0, r6, #0
	mov r1, #0xa
	bl _s32_div_f
	lsl r0, r1, #0x10
	lsr r4, r0, #0x10
	add r0, r4, #0
	mov r1, #5
	bl _s32_div_f
	lsl r1, r1, #2
	add r0, sp, #0x1c
	strb r1, [r0, #5]
	add r0, r4, #0
	mov r1, #5
	bl _s32_div_f
	lsl r1, r0, #3
	add r0, sp, #0x1c
	strb r1, [r0, #2]
	mov r4, #0
	mov r6, #4
	mov r7, #8
_02203C5E:
	str r6, [sp]
	str r7, [sp, #4]
	ldr r0, [r5, #0x20]
	lsl r2, r4, #2
	add r0, #0xc
	str r0, [sp, #8]
	add r0, sp, #0x1c
	add r0, #3
	ldrb r0, [r0, r4]
	add r2, #0x13
	lsl r2, r2, #0x18
	str r0, [sp, #0xc]
	add r0, sp, #0x1c
	ldrb r0, [r0, r4]
	mov r1, #5
	lsr r2, r2, #0x18
	str r0, [sp, #0x10]
	mov r0, #0x14
	str r0, [sp, #0x14]
	str r0, [sp, #0x18]
	ldr r0, [r5, #8]
	mov r3, #0x10
	bl CopyRectToBgTilemapRect
	add r0, r4, #1
	lsl r0, r0, #0x18
	lsr r4, r0, #0x18
	cmp r4, #3
	blo _02203C5E
	ldr r0, [r5, #8]
	mov r1, #5
	bl ScheduleBgTilemapBufferTransfer
	add sp, #0x24
	pop {r4, r5, r6, r7, pc}
	thumb_func_end ov96_02203BD0
