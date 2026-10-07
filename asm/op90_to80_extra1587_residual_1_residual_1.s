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
	.extern ov96_021EB588
	.extern ov96_021FEBF0
	.extern ov96_021FED3C
	.extern ov96_021FEE60
	.extern ov96_021FF67C
	.include "asm/macros.inc"
	.include "overlay_96.inc"
	.include "global.inc"
	.include "overlay_96_exact_bank.inc"

.text
	thumb_func_start ov96_021FEECC
ov96_021FEECC: ; 0x021FEECC
	push {r4, r5, r6, r7, lr}
	sub sp, #0x2c
	ldr r7, [sp, #0x40]
	str r0, [sp, #0x10]
	ldr r6, [sp, #0x44]
	mov r0, #0
	str r0, [sp, #0x14]
	add r4, r1, #0
	ldrb r0, [r3, #0x1d]
	lsl r1, r7, #1
	mov ip, r2
	asr r0, r1
	mov r1, #3
	and r0, r1
	add r1, r4, #0
	add r1, #0x9f
	lsl r0, r0, #0x18
	ldrb r1, [r1]
	lsr r0, r0, #0x18
	ldr r5, [sp, #0x50]
	cmp r1, r0
	beq _021FEF0A
	add r1, r4, #0
	add r1, #0x9f
	strb r0, [r1]
	add r1, r4, #0
	ldr r0, [sp, #0x14]
	add r1, #0xa0
	strb r0, [r1]
	mov r0, #1
	str r0, [sp, #0x14]
_021FEF0A:
	add r0, sp, #0x18
	str r0, [sp]
	mov r1, ip
	ldr r2, [r1]
	lsl r0, r6, #2
	asr r1, r2, #0xb
	lsr r1, r1, #0x14
	add r1, r2, r1
	mov r2, ip
	ldr r3, [r2, #4]
	ldr r0, [r4, r0]
	asr r2, r3, #0xb
	lsr r2, r2, #0x14
	add r2, r3, r2
	asr r1, r1, #0xc
	asr r2, r2, #0xc
	add r3, sp, #0x1c
	bl ov96_021EB0A4
	ldr r0, [sp, #0x1c]
	lsl r0, r0, #0xc
	str r0, [sp, #0x20]
	ldr r0, [sp, #0x18]
	lsl r0, r0, #0xc
	str r0, [sp, #0x24]
	mov r0, #0
	str r0, [sp, #0x28]
	add r0, r4, #0
	add r0, #0x9f
	ldrb r0, [r0]
	cmp r0, #0
	beq _021FEFB0
	cmp r0, #1
	beq _021FEF56
	cmp r0, #2
	beq _021FEF82
	add sp, #0x2c
	pop {r4, r5, r6, r7, pc}
_021FEF56:
	ldr r0, [sp, #0x4c]
	cmp r0, #0
	beq _021FEF66
	ldr r0, [sp, #0x54]
	cmp r0, #0
	beq _021FEF66
	mov r1, #1
	b _021FEF68
_021FEF66:
	mov r1, #0
_021FEF68:
	ldr r0, [sp, #0x48]
	str r6, [sp]
	str r0, [sp, #4]
	str r1, [sp, #8]
	ldr r0, [sp, #0x10]
	add r1, r4, #0
	add r2, sp, #0x20
	add r3, r7, #0
	str r5, [sp, #0xc]
	bl ov96_021FEBF0
	add sp, #0x2c
	pop {r4, r5, r6, r7, pc}
_021FEF82:
	ldr r0, [sp, #0x4c]
	cmp r0, #0
	beq _021FEF92
	ldr r0, [sp, #0x54]
	cmp r0, #0
	beq _021FEF92
	mov r1, #1
	b _021FEF94
_021FEF92:
	mov r1, #0
_021FEF94:
	ldr r0, [sp, #0x48]
	add r2, r7, #0
	str r0, [sp]
	add r0, r1, #0
	orr r0, r5
	str r0, [sp, #4]
	add r0, r4, #0
	add r1, sp, #0x20
	add r3, r6, #0
	str r5, [sp, #8]
	bl ov96_021FED3C
	add sp, #0x2c
	pop {r4, r5, r6, r7, pc}
_021FEFB0:
	ldr r0, [sp, #0x14]
	cmp r0, #0
	beq _021FEFDA
	ldr r0, [sp, #0x4c]
	cmp r0, #0
	beq _021FEFC6
	ldr r0, [sp, #0x54]
	cmp r0, #0
	beq _021FEFC6
	mov r0, #1
	b _021FEFC8
_021FEFC6:
	mov r0, #0
_021FEFC8:
	orr r0, r5
	str r0, [sp]
	ldr r3, [sp, #0x48]
	add r0, r4, #0
	add r1, r7, #0
	add r2, r6, #0
	str r5, [sp, #4]
	bl ov96_021FEE60
_021FEFDA:
	ldr r0, [r4, #0x70]
	add r1, sp, #0x20
	bl ov96_021EB588
	add sp, #0x2c
	pop {r4, r5, r6, r7, pc}
	.balign 4, 0
	thumb_func_end ov96_021FEECC
