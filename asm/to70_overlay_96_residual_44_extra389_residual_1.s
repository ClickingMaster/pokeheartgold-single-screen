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
	.extern ov96_021EEB84
	.extern ov96_021EED64
	.include "asm/macros.inc"
	.include "overlay_96.inc"
	.include "global.inc"
	.include "overlay_96_exact_bank.inc"
	.public ov96_021EEA94

.text
	thumb_func_start ov96_021EEA94
ov96_021EEA94: ; 0x021EEA94
	push {r4, r5, r6, r7, lr}
	sub sp, #0x1c
	add r5, r1, #0
	str r3, [sp, #0xc]
	mov r1, #0
	add r6, r0, #0
	add r4, r2, #0
	bl thunk_Sprite_SetDrawFlag
	ldr r1, [sp, #0xc]
	mov r0, #0x51
	bl NARC_New
	ldr r2, [sp, #0xc]
	add r1, r5, #0
	str r0, [sp, #0x14]
	bl NARC_AllocAndReadWholeMember
	str r0, [sp, #0x10]
	bl NNS_G3dGetTex
	add r5, r0, #0
	beq _021EEAE8
	add r0, #0x3c
	beq _021EEAE4
	add r1, r5, #0
	add r1, #0x3d
	ldrb r1, [r1]
	cmp r4, r1
	bhs _021EEAE4
	add r1, r5, #0
	add r1, #0x42
	ldrh r1, [r1]
	add r2, r0, r1
	ldrh r0, [r0, r1]
	add r2, r2, #4
	add r1, r0, #0
	mul r1, r4
	add r0, r2, r1
	b _021EEAEA
_021EEAE4:
	mov r0, #0
	b _021EEAEA
_021EEAE8:
	mov r0, #0
_021EEAEA:
	ldr r0, [r0]
	mov r1, #2
	lsl r0, r0, #0x10
	lsr r4, r0, #0xd
	ldr r0, [r5, #0x14]
	lsl r1, r1, #8
	add r7, r5, r0
	ldr r0, [sp, #0xc]
	bl Heap_AllocAtEnd
	mov r1, #4
	str r1, [sp]
	str r1, [sp, #4]
	mov r2, #0
	str r0, [sp, #0x18]
	str r0, [sp, #8]
	add r0, r7, r4
	add r3, r2, #0
	bl sub_020145B4
	mov r2, #2
	ldr r1, [sp, #0x18]
	add r0, r6, #0
	lsl r2, r2, #8
	bl ov96_021EED64
	ldr r0, [sp, #0x18]
	bl Heap_Free
	add r0, r6, #0
	bl Sprite_GetVramType
	ldr r4, [r5, #0x38]
	add r7, r0, #0
	add r0, r5, r4
	mov r1, #0x20
	bl DC_FlushRange
	add r0, r6, #0
	bl Sprite_GetPaletteProxy
	add r1, r7, #0
	bl NNS_G2dGetImagePaletteLocation
	add r1, r0, #0
	cmp r7, #1
	bne _021EEB52
	add r0, r5, r4
	mov r2, #0x20
	bl GX_LoadOBJPltt
	b _021EEB5A
_021EEB52:
	add r0, r5, r4
	mov r2, #0x20
	bl GXS_LoadOBJPltt
_021EEB5A:
	ldr r0, [sp, #0x10]
	bl Heap_Free
	ldr r0, [sp, #0x14]
	bl NARC_Delete
	add r0, r6, #0
	mov r1, #1
	bl thunk_Sprite_SetDrawFlag
	add sp, #0x1c
	pop {r4, r5, r6, r7, pc}
	.balign 4, 0
	thumb_func_end ov96_021EEA94
