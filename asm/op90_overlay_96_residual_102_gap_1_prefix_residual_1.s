	.extern ov96_021E60C0
	.extern ov96_021E6104
	.extern ov96_021E8A20
	.extern ov96_021E8BB0
	.extern ov96_021EAA20
	.extern ov96_021EAF6C
	.extern ov96_021EAF70
	.extern ov96_021EB10C
	.include "asm/macros.inc"
	.include "overlay_96.inc"
	.include "global.inc"
	.include "overlay_96_exact_bank.inc"

.text
.extern PokeathlonCourse_RunSubStateLoop
.extern PokeathlonCourse_InitStateInfo
.extern PokeathlonCourse_InitPlayerProfiles
.extern PokeathlonCourse_GetPlayerProfile
.extern PokeathlonCourse_GetParticipantCount
.extern PokeathlonCourse_GetParticipantData
.extern PokeathlonCourse_GetParticipantUnk04
.extern PokeathlonCourse_GetSaveData
.extern PokeathlonCourse_GetFieldData
.extern PokeathlonCourse_GetFieldData_AtIndex
.extern PokeathlonCourse_GetFieldBA4
.extern PokeathlonCourse_AllocPtr4FromHeap
.extern PokeathlonCourse_FreePtr4HeapAlloc
.extern PokeathlonCourse_GetHeapAllocPtr4
.extern PokeathlonCourse_GetHeapID
.extern PokeathlonCourse_GetField1ED
.extern PokeathlonCourse_SetField1ED
.extern PokeathlonCourse_IncrementField1ED
.extern PokeathlonCourse_SetField1F4
.extern ov96_021E5E04
.extern PokeathlonCourse_GetField3D8_ForCurrentParticipant
.extern PokeathlonCourse_GetField3D8_AtIndex
.extern ov96_021E5E7C
.extern PokeathlonCourse_GetCurrentParticipantIndex
.extern PokeathlonCourse_GetMode
.extern PokeathlonCourse_GetField1EF
.extern PokeathlonCourse_IncrementField1EF
.extern PokeathlonCourse_ResetField1EF
.extern PokeathlonCourse_GetSystem
.extern ov96_021E5F24
.extern PokeathlonCourse_GetPlayerProfileFromData
.extern PokeathlonCourse_GetField974_AtIndex
.extern PokeathlonCourse_GetDataCopyArea
.extern PokeathlonCourse_ResetDataCopyArea
.extern PokeathlonCourse_SetField3A4
.extern PokeathlonCourse_ResetField3A4
.extern PokeathlonCourse_GetField3A4
.extern PokeathlonCourse_GetUnkConstant4
.extern PokeathlonCourse_SetField5E0_AtIndex
.extern PokeathlonCourse_GetField5F0_AtIndex
.extern PokeathlonCourse_SetStateField07
.extern PokeathlonCourse_SetStateField07_IfDifferent
.extern PokeathlonCourse_SetStateTransitionType
.extern PokeathlonCourse_SetVBlankIntrCB
.extern PokeathlonCourse_GetGraphicsSystem
	thumb_func_start ov96_0220F4A0
ov96_0220F4A0: ; 0x0220F4A0
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x58
	str r0, [sp, #0xc]
	add r0, sp, #0x40
	mov r1, #0
	str r1, [r0]
	str r1, [r0, #4]
	str r1, [r0, #8]
	str r1, [r0, #0xc]
	str r1, [r0, #0x10]
	str r1, [r0, #0x14]
	mov r1, #0xaa
	mov r2, #0x11
	bl ReadWholeNarcMemberByIdPair
	ldr r0, [sp, #0xc]
	cmp r0, #0
	bne _0220F4C8
	bl GF_AssertFail
_0220F4C8:
	ldr r0, [sp, #0xc]
	ldr r0, [r0, #4]
	cmp r0, #0
	bne _0220F4D4
	bl GF_AssertFail
_0220F4D4:
	ldr r0, [sp, #0xc]
	ldr r7, [r0, #4]
	add r0, r7, #0
	bl ov96_0220F280
	mov r2, #0x8f
	ldr r1, [sp, #0xc]
	lsl r2, r2, #2
	add r1, r1, r2
	mov r0, #0
	mov r2, #3
	mov r3, #3
_0220F4EC:
	ldr r5, [r1]
	add r4, r0, #0
	bic r5, r2
	and r4, r3
	orr r4, r5
	str r4, [r1]
	add r0, r0, #1
	add r1, #0xe4
	cmp r0, #4
	blt _0220F4EC
	mov r1, #0x59
	ldr r0, [sp, #0xc]
	lsl r1, r1, #2
	add r0, r0, r1
	mov r6, #0
	str r0, [sp, #0x20]
_0220F50C:
	add r0, r6, #0
	mov r1, #3
	bl _s32_div_f
	str r1, [sp, #0x10]
	add r0, r6, #0
	mov r1, #3
	bl _s32_div_f
	mov r1, #0xe4
	str r0, [sp, #0x1c]
	mul r1, r0
	ldr r0, [sp, #0x20]
	add r2, r0, r1
	ldr r1, [sp, #0x10]
	mov r0, #0x48
	mul r0, r1
	add r5, r2, r0
	ldr r0, [sp, #0xc]
	lsl r1, r6, #0x18
	ldr r0, [r0, #0x20]
	lsr r1, r1, #0x18
	bl ov96_021EAA04
	mov r1, #1
	str r0, [sp, #0x24]
	bl ov96_021EAB38
	add r0, r6, #0
	mov r1, #3
	bl _s32_div_f
	add r4, r0, #0
	add r0, r6, #0
	mov r1, #3
	bl _s32_div_f
	add r2, r1, #0
	add r0, r7, #0
	add r1, r4, #0
	bl ov96_021E60C0
	bl ov96_021E6138
	lsl r1, r0, #3
	add r0, sp, #0x40
	add r2, r0, r1
	add r1, r2, #0
	sub r1, #8
	sub r2, r2, #4
	ldr r0, [sp, #0x24]
	ldr r1, [r1]
	ldr r2, [r2]
	bl ov96_021EAF70
	ldr r0, [sp, #0xc]
	lsl r1, r6, #0x18
	ldr r0, [r0, #0x20]
	lsr r1, r1, #0x18
	bl ov96_021EAA04
	add r4, r0, #0
	bl ov96_021EAA20
	bl ov96_021E8BB0
	str r0, [sp, #0x28]
	add r0, r4, #0
	mov r1, #0
	bl ov96_0220D554
	mov r1, #0xfe
	lsl r1, r1, #0x16
	add r0, r4, #0
	add r2, r1, #0
	bl ov96_021EB10C
	ldr r1, [sp, #0x1c]
	mov r0, #0xc
	add r2, r1, #0
	mul r2, r0
	ldr r0, _0220F700 ; =ov96_0221CFBC
	add r1, r0, r2
	ldr r0, [sp, #0x10]
	lsl r0, r0, #2
	add r0, r1, r0
	mov r1, #0
	ldrsh r1, [r0, r1]
	str r1, [sp, #0x18]
	mov r1, #2
	ldrsh r0, [r0, r1]
	str r0, [sp, #0x14]
	add r0, r4, #0
	bl ov96_021EAC0C
	ldr r1, [sp, #0x18]
	ldr r2, [sp, #0x14]
	add r0, r4, #0
	bl ov96_021EAF94
	bl ov96_021E6104
	add r1, r0, #0
	add r0, r4, #0
	bl ov96_021EAF6C
	add r0, sp, #0x2c
	str r0, [sp]
	ldr r1, [sp, #0x18]
	ldr r2, [sp, #0x14]
	add r0, r4, #0
	add r3, sp, #0x30
	bl ov96_021EB0A4
	str r7, [r5]
	ldr r1, [r5, #0x40]
	ldr r0, _0220F704 ; =0xFFF0FFFF
	and r1, r0
	lsl r0, r6, #0x1c
	lsr r0, r0, #0xc
	orr r0, r1
	str r0, [r5, #0x40]
	ldr r0, [sp, #0x1c]
	lsl r0, r0, #0x18
	lsr r1, r0, #0x18
	add r0, r5, #0
	add r0, #0x45
	strb r1, [r0]
	ldr r0, [sp, #0x10]
	lsl r0, r0, #0x18
	lsr r3, r0, #0x18
	add r0, r5, #0
	add r0, #0x46
	strb r3, [r0]
	str r4, [r5, #4]
	ldr r2, [r5, #0x40]
	ldr r0, _0220F708 ; =0xFCFFFFFF
	and r0, r2
	ldr r2, [sp, #0x28]
	ldrh r2, [r2, #4]
	lsl r2, r2, #0x1e
	lsr r2, r2, #6
	orr r0, r2
	str r0, [r5, #0x40]
	add r0, r7, #0
	add r2, r3, #0
	bl ov96_0220F378
	str r0, [r5, #8]
	mov r0, #2
	str r0, [r5, #0xc]
	ldr r1, [r5, #0x40]
	ldr r0, _0220F70C ; =0xFF0FFFFF
	and r1, r0
	mov r0, #2
	lsl r0, r0, #0x14
	orr r0, r1
	str r0, [r5, #0x40]
	ldr r0, [sp, #0x30]
	lsl r0, r0, #0xc
	str r0, [r5, #0x1c]
	ldr r0, [sp, #0x2c]
	lsl r0, r0, #0xc
	str r0, [r5, #0x20]
	ldr r0, [sp, #0x30]
	lsl r0, r0, #0xc
	str r0, [r5, #0x10]
	ldr r0, [sp, #0x2c]
	lsl r0, r0, #0xc
	str r0, [r5, #0x14]
	add r0, r7, #0
	bl ov96_021E5F24
	ldr r1, [sp, #0x1c]
	cmp r1, r0
	bne _0220F6A8
	ldr r0, [sp, #0x10]
	ldr r3, [sp, #0x18]
	lsl r0, r0, #2
	add r1, sp, #0x34
	add r2, sp, #0x34
	add r1, r1, r0
	strh r3, [r2, r0]
	ldr r0, [sp, #0x14]
	strh r0, [r1, #2]
	ldr r0, [sp, #0xc]
	ldr r0, [r0, #0x10]
	bl SpriteManager_GetSpriteList
	add r3, r0, #0
	mov r0, #1
	str r0, [sp]
	ldr r2, [sp, #0xc]
	add r0, r7, #0
	ldr r2, [r2, #0x1c]
	add r1, r4, #0
	bl ov96_021E64F8
	mov r1, #4
	bl Sprite_SetDrawPriority
	add r0, r4, #0
	mov r1, #6
	bl ov96_021EABA8
	b _0220F6B0
_0220F6A8:
	add r0, r4, #0
	mov r1, #7
	bl ov96_021EABA8
_0220F6B0:
	add r0, r7, #0
	bl ov96_021E5F24
	cmp r0, #0
	bne _0220F6D0
	add r0, r7, #0
	bl PokeathlonCourse_GetDataCopyArea
	add r0, #0x28
	bl ov96_021E8A20
	add r1, r0, r6
	ldr r0, [sp, #0x30]
	strb r0, [r1, #4]
	ldr r0, [sp, #0x2c]
	strb r0, [r1, #0x10]
_0220F6D0:
	add r6, r6, #1
	cmp r6, #0xc
	bge _0220F6D8
	b _0220F50C
_0220F6D8:
	ldr r0, [sp, #0xc]
	ldr r0, [r0, #0x10]
	bl SpriteManager_GetSpriteList
	add r3, r0, #0
	mov r0, #1
	str r0, [sp]
	mov r0, #3
	str r0, [sp, #4]
	add r0, sp, #0x34
	str r0, [sp, #8]
	ldr r2, [sp, #0xc]
	add r0, r7, #0
	ldr r2, [r2, #0x1c]
	mov r1, #0
	bl ov96_021E62AC
	add sp, #0x58
	pop {r3, r4, r5, r6, r7, pc}
	nop
_0220F700: .word ov96_0221CFBC
_0220F704: .word 0xFFF0FFFF
_0220F708: .word 0xFCFFFFFF
_0220F70C: .word 0xFF0FFFFF
	thumb_func_end ov96_0220F4A0
