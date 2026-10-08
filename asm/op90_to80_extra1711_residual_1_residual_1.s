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
	.extern ov96_021E60C0
	.extern ov96_021E8A20
	.extern ov96_021E8BAC
	.extern ov96_021EAA20
	.extern ov96_021EB564
	.extern ov96_021EB570
	.extern ov96_021EB57C
	.extern ov96_021EB588
	.extern ov96_021EB630
	.extern ov96_02211AF0
	.extern ov96_022123B0
	.extern ov96_022124F8
	.extern ov96_022127F4
	.extern ov96_02212B94
	.extern ov96_02213354
	.extern ov96_02214904
	.include "asm/macros.inc"
	.include "overlay_96.inc"
	.include "global.inc"
	.include "overlay_96_exact_bank.inc"

.text
	thumb_func_start ov96_02211DE4
ov96_02211DE4: ; 0x02211DE4
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x18
	str r0, [sp]
	mov r0, #0
	str r0, [sp, #0xc]
	str r0, [sp, #8]
	str r0, [sp, #4]
	add r4, r0, #0
	add r6, r0, #0
	ldr r0, [sp]
	add r5, r1, #0
	str r0, [sp, #0x10]
	add r0, #0x5c
	str r0, [sp, #0x10]
_02211E00:
	add r0, r4, #0
	mov r1, #3
	bl _s32_div_f
	add r7, r0, #0
	add r0, r4, #0
	mov r1, #3
	bl _s32_div_f
	mov r0, #0x5d
	lsl r0, r0, #2
	add r2, r7, #0
	mul r2, r0
	ldr r0, [sp, #0x10]
	add r2, r0, r2
	mov r0, #0x7c
	mul r0, r1
	add r0, r2, r0
	add r3, r0, #0
	add r3, #0x5c
	ldrh r3, [r3]
	ldr r2, [r0, #0x34]
	sub r3, r3, #1
	lsl r3, r3, #0x18
	asr r1, r2, #0xb
	lsr r7, r3, #0x18
	add r3, r5, r4
	lsr r1, r1, #0x14
	mov ip, r3
	add r1, r2, r1
	asr r2, r1, #0xc
	ldr r1, [r0, #0x78]
	ldr r3, [r0, #0x30]
	lsl r1, r1, #0x18
	asr r0, r3, #0xb
	lsr r0, r0, #0x14
	add r0, r3, r0
	asr r0, r0, #0xc
	strb r0, [r5, r4]
	mov r0, ip
	strb r2, [r0, #0xc]
	add r2, r7, #0
	lsr r1, r1, #0x18
	lsl r2, r6
	ldr r0, [sp, #0xc]
	lsl r1, r6
	add r0, r0, r2
	str r0, [sp, #0xc]
	ldr r0, [sp, #8]
	add r4, r4, #1
	add r0, r0, r1
	add r6, r6, #2
	str r0, [sp, #8]
	cmp r4, #0xc
	blt _02211E00
	ldr r1, _02211F2C ; =0x0000062C
	ldr r0, [sp]
	mov r6, #0
	add r4, r0, r1
	add r7, r6, #0
_02211E78:
	ldr r1, [r4, #8]
	asr r0, r1, #0xb
	lsr r0, r0, #0x14
	add r0, r1, r0
	asr r0, r0, #0xc
	str r0, [sp, #0x14]
	add r0, r4, #0
	bl ov96_02213EC4
	add r2, r5, r6
	ldr r1, [sp, #0x14]
	add r6, r6, #1
	strb r1, [r2, #0x18]
	strb r0, [r2, #0x1a]
	add r0, r4, #0
	add r0, #0x39
	ldrb r0, [r0]
	add r4, #0x4c
	add r1, r0, #0
	lsl r1, r7
	ldr r0, [sp, #4]
	add r7, r7, #2
	add r0, r0, r1
	str r0, [sp, #4]
	cmp r6, #2
	blt _02211E78
	ldr r0, _02211F30 ; =0x0000073C
	ldr r1, [sp]
	ldrb r2, [r1, r0]
	lsl r3, r2, #2
	add r3, r1, r3
	add r1, r0, #0
	sub r1, #0x48
	ldr r1, [r3, r1]
	lsl r1, r1, #0x18
	lsr r3, r1, #0x18
	add r1, r2, #1
	lsr r6, r1, #0x1f
	lsl r4, r1, #0x1e
	sub r4, r4, r6
	mov r1, #0x1e
	ror r4, r1
	ldr r1, [sp]
	add r4, r6, r4
	strb r4, [r1, r0]
	ldr r4, [sp, #4]
	lsl r2, r2, #0x1c
	lsl r6, r4, #0x18
	ldr r4, [sp, #0xc]
	add r1, r5, #0
	add r4, r4, r6
	add r2, r4, r2
	str r2, [r5, #0x1c]
	ldr r2, [sp]
	sub r0, #0xd8
	ldrb r0, [r2, r0]
	add r1, #0x1c
	cmp r0, #2
	bne _02211EF8
	mov r0, #1
	ldr r2, [r1]
	lsl r0, r0, #0x1e
	add r0, r2, r0
	str r0, [r1]
_02211EF8:
	mov r1, #0x6b
	ldr r0, [sp]
	lsl r1, r1, #4
	ldrb r0, [r0, r1]
	cmp r0, #0
	beq _02211F0E
	mov r0, #2
	ldr r1, [r5, #0x1c]
	lsl r0, r0, #0x1e
	sub r0, r1, r0
	str r0, [r5, #0x1c]
_02211F0E:
	ldr r0, [sp, #8]
	mov r1, #0
	add r2, r1, r0
	ldr r1, _02211F34 ; =0x0000073E
	ldr r0, [sp]
	str r2, [r5, #0x20]
	ldrb r0, [r0, r1]
	lsl r0, r0, #0x18
	add r1, r2, r0
	lsl r0, r3, #0x19
	add r0, r1, r0
	str r0, [r5, #0x20]
	add sp, #0x18
	pop {r3, r4, r5, r6, r7, pc}
	nop
_02211F2C: .word 0x0000062C
_02211F30: .word 0x0000073C
_02211F34: .word 0x0000073E
	thumb_func_end ov96_02211DE4
