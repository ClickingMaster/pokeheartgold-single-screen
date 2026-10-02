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
	.extern ov96_021E8A20
	.include "asm/macros.inc"
	.include "overlay_96.inc"
	.include "global.inc"
	.include "overlay_96_exact_bank.inc"

.text
	thumb_func_start ov96_02216234
ov96_02216234: ; 0x02216234
	push {r3, r4, r5, r6, r7, lr}
	add r6, r0, #0
	bl PokeathlonCourse_GetHeapAllocPtr4
	add r4, r0, #0
	add r0, r6, #0
	bl PokeathlonCourse_GetDataCopyArea
	add r7, r0, #0
	bl ov96_021E8A20
	add r7, #0xf0
	add r5, r0, #0
	add r0, r7, #0
	bl ov96_021E8A20
	ldr r0, [r0, #0x20]
	lsr r0, r0, #0x1f
	cmp r0, #1
	bne _02216282
	ldr r0, _022162E0 ; =0x0000043C
	add r0, r4, r0
	bl ov96_02217820
	ldr r0, [r4, #0x1c]
	mov r1, #1
	bl ov96_021EB144
	mov r0, #6
	lsl r0, r0, #6
	ldr r0, [r4, r0]
	bl ov96_0221964C
	ldr r1, _022162E4 ; =ov96_02216200
	add r0, r6, #0
	bl ov96_021E8324
	mov r0, #1
	pop {r3, r4, r5, r6, r7, pc}
_02216282:
	bl System_GetTouchNew
	cmp r0, #0
	beq _022162A4
	add r0, r6, #0
	bl ov96_021E5F24
	add r1, r0, #0
	mov r0, #1
	lsl r1, r1, #0x18
	str r0, [sp]
	add r0, r6, #0
	lsr r1, r1, #0x18
	mov r2, #3
	mov r3, #0
	bl ov96_021E8228
_022162A4:
	bl System_GetTouchHeld
	cmp r0, #0
	beq _022162D4
	ldr r2, [r5]
	mov r0, #0xff
	ldr r1, _022162E8 ; =gSystem + 0x40
	bic r2, r0
	ldrh r0, [r1, #0x20]
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	orr r2, r0
	ldr r0, _022162EC ; =0xFFFF00FF
	str r2, [r5]
	and r2, r0
	ldrh r0, [r1, #0x22]
	add r1, r2, #0
	lsl r0, r0, #0x18
	lsr r0, r0, #0x10
	orr r1, r0
	mov r0, #1
	lsl r0, r0, #0x10
	orr r0, r1
	b _022162DA
_022162D4:
	ldr r1, [r5]
	ldr r0, _022162F0 ; =0xFFFEFFFF
	and r0, r1
_022162DA:
	str r0, [r5]
	mov r0, #0
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
_022162E0: .word 0x0000043C
_022162E4: .word ov96_02216200
_022162E8: .word gSystem + 0x40
_022162EC: .word 0xFFFF00FF
_022162F0: .word 0xFFFEFFFF
	thumb_func_end ov96_02216234
