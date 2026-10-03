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
	thumb_func_start ov96_021E9C0C
ov96_021E9C0C: ; 0x021E9C0C
	push {r4, r5, r6, lr}
	add r5, r0, #0
	ldr r1, [r5, #4]
	mov r0, #1
	tst r0, r1
	beq _021E9C1E
	ldr r0, [r5, #0x1c]
	bl SpriteTransfer_DeleteCharTransferTask
_021E9C1E:
	ldr r1, [r5, #4]
	mov r0, #4
	tst r0, r1
	beq _021E9C2C
	ldr r0, [r5, #0x20]
	bl SpriteTransfer_DeleteCharTransferTask
_021E9C2C:
	ldr r1, [r5, #4]
	mov r0, #2
	tst r0, r1
	beq _021E9C3A
	ldr r0, [r5, #0x24]
	bl SpriteTransfer_DeleteCharTransferTask
_021E9C3A:
	ldr r1, [r5, #4]
	mov r0, #8
	tst r0, r1
	beq _021E9C48
	ldr r0, [r5, #0x28]
	bl SpriteTransfer_DeleteCharTransferTask
_021E9C48:
	ldr r1, [r5, #4]
	mov r0, #0x10
	tst r0, r1
	beq _021E9C56
	ldr r0, [r5, #0x2c]
	bl SpriteTransfer_DeleteCharTransferTask
_021E9C56:
	ldr r1, [r5, #4]
	mov r0, #0x20
	tst r0, r1
	beq _021E9C64
	ldr r0, [r5, #0x30]
	bl SpriteTransfer_DeleteCharTransferTask
_021E9C64:
	ldr r1, [r5, #4]
	mov r0, #0x40
	tst r0, r1
	beq _021E9C72
	ldr r0, [r5, #0x34]
	bl SpriteTransfer_DeleteCharTransferTask
_021E9C72:
	ldr r1, [r5, #4]
	mov r0, #0x80
	tst r0, r1
	beq _021E9C80
	ldr r0, [r5, #0x38]
	bl SpriteTransfer_DeleteCharTransferTask
_021E9C80:
	mov r0, #1
	ldr r1, [r5, #4]
	lsl r0, r0, #8
	tst r0, r1
	beq _021E9C90
	ldr r0, [r5, #0x3c]
	bl SpriteTransfer_DeleteCharTransferTask
_021E9C90:
	mov r0, #2
	ldr r1, [r5, #4]
	lsl r0, r0, #8
	tst r0, r1
	beq _021E9CA0
	ldr r0, [r5, #0x40]
	bl SpriteTransfer_DeleteCharTransferTask
_021E9CA0:
	mov r0, #1
	ldr r1, [r5, #4]
	lsl r0, r0, #0xa
	tst r0, r1
	beq _021E9CB0
	ldr r0, [r5, #0x44]
	bl SpriteTransfer_DeleteCharTransferTask
_021E9CB0:
	mov r0, #2
	ldr r1, [r5, #4]
	lsl r0, r0, #0xa
	tst r0, r1
	beq _021E9CC0
	ldr r0, [r5, #0x48]
	bl SpriteTransfer_DeleteCharTransferTask
_021E9CC0:
	add r0, r5, #0
	add r0, #0xac
	ldr r0, [r0]
	bl SpriteTransfer_DeletePlttTransferTask
	add r0, r5, #0
	add r0, #0xb0
	ldr r0, [r0]
	cmp r0, #0
	beq _021E9CD8
	bl SpriteTransfer_DeletePlttTransferTask
_021E9CD8:
	mov r6, #0
	add r4, r5, #0
_021E9CDC:
	ldr r0, [r4, #0xc]
	bl Destroy2DGfxResObjMan
	add r6, r6, #1
	add r4, r4, #4
	cmp r6, #4
	blt _021E9CDC
	add r0, r5, #0
	bl Heap_Free
	pop {r4, r5, r6, pc}
	.balign 4, 0
	thumb_func_end ov96_021E9C0C
