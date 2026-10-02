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
	thumb_func_start ov96_02214278
ov96_02214278: ; 0x02214278
	push {r3, r4, r5, lr}
	sub sp, #0x10
	add r4, r1, #0
	ldrb r0, [r4]
	cmp r0, #0
	bne _02214286
	b _0221438C
_02214286:
	cmp r0, #1
	beq _02214292
	cmp r0, #2
	beq _022142CE
	add sp, #0x10
	pop {r3, r4, r5, pc}
_02214292:
	ldrb r0, [r4, #0xe]
	cmp r0, #0
	beq _022142B4
	ldrb r0, [r4, #0xf]
	cmp r0, #0
	ldr r0, [r4, #8]
	beq _022142AA
	ldrb r1, [r4, #1]
	mov r2, #1
	bl ov96_02214044
	b _022142BE
_022142AA:
	ldrb r1, [r4, #1]
	mov r2, #2
	bl ov96_02214044
	b _022142BE
_022142B4:
	ldrb r1, [r4, #1]
	ldr r0, [r4, #8]
	mov r2, #3
	bl ov96_02214044
_022142BE:
	mov r0, #0
	strb r0, [r4, #2]
	strb r0, [r4, #0xd]
	strb r0, [r4, #0xc]
	mov r0, #2
	add sp, #0x10
	strb r0, [r4]
	pop {r3, r4, r5, pc}
_022142CE:
	ldrb r0, [r4, #2]
	add r0, r0, #1
	strb r0, [r4, #2]
	ldrb r0, [r4, #2]
	cmp r0, #0x1e
	bls _02214318
	mov r0, #8
	str r0, [sp]
	mov r0, #2
	str r0, [sp, #4]
	mov r1, #1
	str r1, [sp, #8]
	ldrb r2, [r4, #1]
	ldr r0, [r4, #8]
	mov r3, #0
	lsl r2, r2, #0x1b
	ldr r0, [r0, #4]
	lsr r2, r2, #0x18
	bl BgTilemapRectChangePalette
	ldr r0, [r4, #8]
	mov r1, #1
	ldr r0, [r0, #4]
	bl ScheduleBgTilemapBufferTransfer
	ldrb r1, [r4, #1]
	ldr r0, [r4, #8]
	mov r2, #0
	bl ov96_02214044
	mov r0, #0
	mvn r0, r0
	strb r0, [r4, #3]
	mov r0, #0
	add sp, #0x10
	strb r0, [r4]
	pop {r3, r4, r5, pc}
_02214318:
	ldrb r0, [r4, #0xc]
	cmp r0, #0
	bne _02214388
	ldr r3, _02214390 ; =ov96_0221D1F4
	add r1, sp, #0xc
	ldrb r0, [r3, #2]
	add r2, sp, #0xc
	add r1, #2
	strb r0, [r2, #2]
	ldrb r0, [r3, #3]
	strb r0, [r2, #3]
	ldrb r5, [r3]
	add r0, sp, #0xc
	strb r5, [r2]
	ldrb r3, [r3, #1]
	strb r3, [r2, #1]
	ldrb r2, [r4, #0xd]
	add r2, r2, #1
	lsr r5, r2, #0x1f
	lsl r3, r2, #0x1f
	sub r3, r3, r5
	mov r2, #0x1f
	ror r3, r2
	add r2, r5, r3
	strb r2, [r4, #0xd]
	mov r2, #5
	strb r2, [r4, #0xc]
	ldrb r2, [r4, #0xe]
	cmp r2, #0
	beq _0221435A
	ldrb r0, [r4, #0xd]
	ldrb r1, [r1, r0]
	b _0221435E
_0221435A:
	ldrb r1, [r4, #0xd]
	ldrb r1, [r0, r1]
_0221435E:
	mov r0, #8
	str r0, [sp]
	mov r0, #2
	str r0, [sp, #4]
	str r1, [sp, #8]
	ldrb r2, [r4, #1]
	ldr r0, [r4, #8]
	mov r1, #1
	lsl r2, r2, #0x1b
	ldr r0, [r0, #4]
	lsr r2, r2, #0x18
	mov r3, #0
	bl BgTilemapRectChangePalette
	ldr r0, [r4, #8]
	mov r1, #1
	ldr r0, [r0, #4]
	bl ScheduleBgTilemapBufferTransfer
	add sp, #0x10
	pop {r3, r4, r5, pc}
_02214388:
	sub r0, r0, #1
	strb r0, [r4, #0xc]
_0221438C:
	add sp, #0x10
	pop {r3, r4, r5, pc}
	.balign 4, 0
_02214390: .word ov96_0221D1F4
	thumb_func_end ov96_02214278
