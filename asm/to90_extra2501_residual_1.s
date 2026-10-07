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
	.extern ov96_021FDE08
	.extern ov96_021FDE6C
	.include "asm/macros.inc"
	.include "overlay_96.inc"
	.include "global.inc"
	.include "overlay_96_exact_bank.inc"
	.public ov96_021FDA30

.text
	thumb_func_start ov96_021FDA30
ov96_021FDA30: ; 0x021FDA30
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x18
	mov r2, #0
	str r0, [sp]
	mov lr, r2
	add r0, r2, #0
	str r0, [sp, #0x14]
	ldr r3, [sp]
	mov r0, lr
	str r0, [sp, #0x10]
	add r0, r1, #0
	str r1, [sp, #4]
	add r7, r2, #0
	add r3, #0x30
	str r0, [sp, #0xc]
	mov r6, lr
_021FDA50:
	add r0, r3, #0
	add r0, #0x8b
	add r4, r3, #0
	add r4, #0xa2
	ldrb r4, [r4]
	ldrb r0, [r0]
	mov r1, #0
	lsl r4, r4, #2
	add r0, r1, r0
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	add r0, r0, r4
	mov ip, r0
	add r0, r3, #0
	add r0, #0x9e
	ldrb r4, [r0]
	ldr r0, [sp, #0x10]
	lsl r4, r0
	mov r0, lr
	add r0, r0, r4
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	mov lr, r0
	add r0, r3, #0
	add r0, #0x9d
	ldrb r0, [r0]
	lsl r4, r0, #7
	add r0, r3, #0
	add r0, #0x9c
	ldrb r0, [r0]
	add r5, r4, r0
	ldr r0, [sp, #4]
	add r0, r0, r7
	str r0, [sp, #8]
	add r0, r3, #0
	add r0, #0x80
	ldr r4, [r0]
	asr r0, r4, #0xb
	lsr r0, r0, #0x14
	add r0, r4, r0
	asr r4, r0, #0xc
	ldr r0, [sp, #8]
	strb r4, [r0, #8]
	ldr r4, [r3, #0x7c]
	asr r0, r4, #0xb
	lsr r0, r0, #0x14
	add r0, r4, r0
	asr r4, r0, #0xc
	ldr r0, [sp, #0xc]
	strh r4, [r0]
	ldr r0, [sp, #8]
	mov r4, ip
	strb r4, [r0, #0xc]
	strb r5, [r0, #0x10]
	add r4, r3, #0
_021FDABE:
	add r0, r4, #0
	add r0, #0x30
	ldrb r0, [r0]
	add r5, r1, r6
	lsl r5, r5, #1
	lsl r0, r5
	add r1, r1, #1
	add r2, r2, r0
	add r4, #0x1c
	cmp r1, #3
	blt _021FDABE
	ldr r0, [sp]
	ldr r4, _021FDB60 ; =0x00000644
	add r0, r0, r7
	ldrb r1, [r0, r4]
	ldr r0, [sp, #8]
	add r6, r6, #3
	strb r1, [r0, #0x1e]
	add r0, r3, #0
	add r0, #0xd1
	ldrb r0, [r0]
	add r3, #0xd4
	add r1, r0, #0
	lsl r1, r7
	ldr r0, [sp, #0x14]
	add r7, r7, #1
	add r0, r0, r1
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	str r0, [sp, #0x14]
	ldr r0, [sp, #0x10]
	add r0, r0, #2
	str r0, [sp, #0x10]
	ldr r0, [sp, #0xc]
	add r0, r0, #2
	str r0, [sp, #0xc]
	cmp r7, #4
	blt _021FDA50
	ldr r0, [sp, #4]
	mov r1, lr
	strb r1, [r0, #0x1d]
	str r2, [r0, #0x14]
	mov r1, #0xf
	ldr r0, [sp]
	lsl r1, r1, #6
	ldrb r0, [r0, r1]
	add r1, r4, #0
	sub r1, #8
	lsl r0, r0, #0x18
	add r2, r2, r0
	ldr r0, [sp, #4]
	str r2, [r0, #0x14]
	ldr r0, [sp]
	ldrb r0, [r0, r1]
	lsl r0, r0, #0x19
	add r1, r2, r0
	ldr r0, [sp, #4]
	str r1, [r0, #0x14]
	ldr r0, [sp, #0x14]
	lsl r0, r0, #0x1a
	add r1, r1, r0
	ldr r0, [sp, #4]
	str r1, [r0, #0x14]
	ldr r0, [sp]
	sub r1, r4, #6
	ldrh r1, [r0, r1]
	ldr r0, [sp, #4]
	strh r1, [r0, #0x1a]
	ldr r0, [sp]
	sub r1, r4, #7
	ldrb r1, [r0, r1]
	ldr r0, [sp, #4]
	strb r1, [r0, #0x1c]
	ldr r0, [sp]
	sub r1, r4, #4
	ldrh r1, [r0, r1]
	ldr r0, [sp, #4]
	strh r1, [r0, #0x18]
	add sp, #0x18
	pop {r3, r4, r5, r6, r7, pc}
	nop
_021FDB60: .word 0x00000644
	thumb_func_end ov96_021FDA30
