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
	.extern ov96_02213354
	.extern ov96_02214904
	.include "asm/macros.inc"
	.include "overlay_96.inc"
	.include "global.inc"
	.include "overlay_96_exact_bank.inc"

.text
	thumb_func_start ov96_022124F8
ov96_022124F8: ; 0x022124F8
	push {r4, r5, r6, r7, lr}
	sub sp, #0x10c
	bl PokeathlonCourse_GetHeapAllocPtr4
	str r0, [sp, #8]
	add r0, sp, #0x70
	mov r4, #0
	str r4, [r0]
	str r4, [r0, #4]
	ldr r3, _022127F0 ; =ov96_0221D25C
	str r4, [r0, #8]
	ldmia r3!, {r0, r1}
	add r2, sp, #0x10
	stmia r2!, {r0, r1}
	ldr r0, [r3]
	add r7, sp, #0x7c
	str r0, [r2]
	ldr r0, [sp, #8]
	str r0, [sp, #0xc]
	add r0, #0x5c
	str r0, [sp, #0xc]
_02212522:
	add r0, r4, #0
	mov r1, #3
	bl _s32_div_f
	add r5, r0, #0
	add r0, r4, #0
	mov r1, #3
	bl _s32_div_f
	mov r0, #0x5d
	lsl r0, r0, #2
	add r2, r5, #0
	mul r2, r0
	ldr r0, [sp, #0xc]
	add r3, sp, #0x70
	add r2, r0, r2
	mov r0, #0x7c
	mul r0, r1
	add r5, r2, r0
	add r2, r5, #0
	ldmia r3!, {r0, r1}
	add r2, #0x3c
	stmia r2!, {r0, r1}
	ldr r0, [r3]
	add r3, r5, #0
	str r0, [r2]
	add r3, #0x30
	ldmia r3!, {r0, r1}
	add r2, r7, #0
	stmia r2!, {r0, r1}
	ldr r0, [r3]
	add r3, r5, #0
	str r0, [r2]
	add r3, #0x30
	ldmia r3!, {r0, r1}
	add r2, sp, #0x4c
	stmia r2!, {r0, r1}
	ldr r0, [r3]
	str r0, [r2]
	ldr r0, [r5, #0x48]
	cmp r0, #0
	beq _022125FC
	sub r0, r0, #2
	cmp r0, #1
	bhi _02212590
	mov r0, #0x5a
	ldrsh r0, [r5, r0]
	cmp r0, #0
	ble _02212590
	mov r0, #0x5a
	ldrsh r0, [r5, r0]
	sub r1, r0, #1
	add r0, r5, #0
	add r0, #0x5a
	strh r1, [r0]
_02212590:
	mov r0, #0x58
	ldrsh r1, [r5, r0]
	cmp r1, #0
	bne _022125AC
	mov r0, #0x5a
	ldrsh r0, [r5, r0]
	cmp r0, #0
	bgt _022125AC
	mov r0, #0
	add r1, r5, #0
	str r0, [r5, #0x48]
	add r1, #0x5a
	strh r0, [r1]
	b _022125FC
_022125AC:
	lsl r0, r1, #0xc
	add r1, r5, #0
	add r2, sp, #0x4c
	add r1, #0x4c
	add r3, r2, #0
	bl VEC_MultAdd
	add r6, sp, #0x4c
	add r3, r5, #0
	add r3, #0x30
	ldmia r6!, {r0, r1}
	add r2, r3, #0
	stmia r3!, {r0, r1}
	ldr r0, [r6]
	str r0, [r3]
	add r3, r5, #0
	ldmia r2!, {r0, r1}
	add r3, #0x24
	stmia r3!, {r0, r1}
	ldr r0, [r2]
	str r0, [r3]
	mov r0, #0x58
	ldrsh r0, [r5, r0]
	cmp r0, #0
	ble _022125EA
	mov r0, #0x58
	ldrsh r0, [r5, r0]
	sub r1, r0, #1
	add r0, r5, #0
	add r0, #0x58
	strh r1, [r0]
_022125EA:
	ldr r0, [r5, #0x78]
	cmp r0, #2
	beq _022125FC
	ldr r0, [r5, #0x48]
	cmp r0, #3
	bne _022125FC
	add r0, r5, #0
	bl ov96_02213FB4
_022125FC:
	ldr r0, [r5, #0x78]
	cmp r0, #2
	beq _0221260C
	add r0, r5, #0
	add r0, #0x60
	ldrb r0, [r0]
	cmp r0, #0
	beq _02212634
_0221260C:
	add r0, r5, #0
	add r0, #0x60
	ldrb r0, [r0]
	sub r1, r0, #1
	add r0, r5, #0
	add r0, #0x60
	strb r1, [r0]
	add r0, r5, #0
	add r0, #0x60
	ldrb r0, [r0]
	cmp r0, #0
	bne _0221263A
	mov r0, #0
	str r0, [r5, #0x78]
	add r0, r5, #0
	add r0, #0x70
	ldrb r0, [r0]
	add r5, #0x71
	strb r0, [r5]
	b _02212742
_02212634:
	ldr r0, [r5, #0x48]
	cmp r0, #0
	beq _0221263C
_0221263A:
	b _02212742
_0221263C:
	add r0, r5, #0
	add r0, #0x24
	add r1, sp, #0x4c
	add r2, sp, #0x64
	bl VEC_Subtract
	add r0, sp, #0x64
	bl VEC_Mag
	add r6, r0, #0
	cmp r6, #0
	bgt _0221265C
	add r0, r5, #0
	bl ov96_02213558
	b _02212742
_0221265C:
	add r0, sp, #0x64
	add r1, sp, #0x58
	bl VEC_Normalize
	add r3, sp, #0x58
	add r2, r5, #0
	ldmia r3!, {r0, r1}
	add r2, #0x3c
	stmia r2!, {r0, r1}
	ldr r0, [r3]
	str r0, [r2]
	ldr r0, [r5, #0x78]
	cmp r0, #1
	bne _0221267C
	ldr r0, [r5, #0x68]
	b _02212688
_0221267C:
	cmp r0, #3
	bne _02212686
	mov r0, #2
	lsl r0, r0, #0xc
	b _02212688
_02212686:
	ldr r0, [r5, #0x64]
_02212688:
	cmp r6, r0
	bgt _0221269C
	add r3, r5, #0
	add r3, #0x24
	ldmia r3!, {r0, r1}
	add r2, sp, #0x4c
	stmia r2!, {r0, r1}
	ldr r0, [r3]
	str r0, [r2]
	b _022126BA
_0221269C:
	mov r2, #0
	add r1, sp, #0x40
	str r2, [r1]
	str r2, [r1, #4]
	str r2, [r1, #8]
	add r1, sp, #0x58
	add r2, sp, #0x40
	add r3, sp, #0x64
	bl VEC_MultAdd
	add r1, sp, #0x4c
	add r0, sp, #0x64
	add r2, r1, #0
	bl VEC_Add
_022126BA:
	cmp r6, #0
	beq _02212736
	add r6, sp, #0x10
	ldmia r6!, {r0, r1}
	add r3, sp, #0x34
	add r2, r3, #0
	stmia r3!, {r0, r1}
	ldr r0, [r6]
	add r1, sp, #0x28
	str r0, [r3]
	ldr r0, [sp, #0x5c]
	str r0, [sp, #0x28]
	ldr r0, [sp, #0x58]
	str r0, [sp, #0x30]
	mov r0, #0
	str r0, [sp, #0x2c]
	add r0, r2, #0
	bl CalcAngleBetweenVecs
	mov r1, #2
	lsl r1, r1, #0xc
	cmp r0, r1
	bls _022126F0
	mov r1, #0xe
	lsl r1, r1, #0xc
	cmp r0, r1
	blo _022126FA
_022126F0:
	add r1, r5, #0
	add r1, #0x5c
	mov r0, #4
	strh r0, [r1]
	b _02212736
_022126FA:
	mov r1, #2
	lsl r1, r1, #0xc
	cmp r0, r1
	bls _02212714
	mov r1, #6
	lsl r1, r1, #0xc
	cmp r0, r1
	bhs _02212714
	add r1, r5, #0
	add r1, #0x5c
	mov r0, #2
	strh r0, [r1]
	b _02212736
_02212714:
	mov r1, #6
	lsl r1, r1, #0xc
	cmp r0, r1
	blo _0221272E
	mov r1, #0xa
	lsl r1, r1, #0xc
	cmp r0, r1
	bhi _0221272E
	add r1, r5, #0
	add r1, #0x5c
	mov r0, #3
	strh r0, [r1]
	b _02212736
_0221272E:
	add r1, r5, #0
	add r1, #0x5c
	mov r0, #1
	strh r0, [r1]
_02212736:
	add r2, sp, #0x4c
	ldmia r2!, {r0, r1}
	add r5, #0x30
	stmia r5!, {r0, r1}
	ldr r0, [r2]
	str r0, [r5]
_02212742:
	add r4, r4, #1
	add r7, #0xc
	cmp r4, #0xc
	bge _0221274C
	b _02212522
_0221274C:
	ldr r0, [sp, #8]
	add r5, sp, #0x7c
	add r0, #0x5c
	mov r6, #0
	str r5, [sp, #4]
	str r0, [sp, #8]
_02212758:
	add r0, r6, #0
	mov r1, #3
	bl _s32_div_f
	add r4, r0, #0
	add r0, r6, #0
	mov r1, #3
	bl _s32_div_f
	mov r0, #0x5d
	lsl r0, r0, #2
	add r2, r4, #0
	mul r2, r0
	ldr r0, [sp, #8]
	add r2, r0, r2
	mov r0, #0x7c
	mul r0, r1
	add r4, r2, r0
	ldr r1, [r4, #0x30]
	ldr r0, [r5]
	cmp r1, r0
	bne _0221278C
	ldr r1, [r4, #0x34]
	ldr r0, [r5, #4]
	cmp r1, r0
	beq _022127BE
_0221278C:
	add r0, sp, #0x1c
	str r0, [sp]
	add r0, r4, #0
	ldr r1, [sp, #4]
	ldr r2, [r4, #0x20]
	add r0, #0x30
	mov r3, #1
	bl ov96_02213728
	cmp r0, #0
	beq _022127BE
	add r7, sp, #0x1c
	add r3, r4, #0
	add r3, #0x30
	ldmia r7!, {r0, r1}
	add r2, r3, #0
	stmia r3!, {r0, r1}
	ldr r0, [r7]
	str r0, [r3]
	add r3, r4, #0
	ldmia r2!, {r0, r1}
	add r3, #0x24
	stmia r3!, {r0, r1}
	ldr r0, [r2]
	str r0, [r3]
_022127BE:
	ldr r1, [r4, #0x30]
	ldr r0, [r4, #0x24]
	cmp r1, r0
	bne _022127DC
	ldr r1, [r4, #0x34]
	ldr r0, [r4, #0x28]
	cmp r1, r0
	bne _022127DC
	ldr r0, [r4, #0x78]
	cmp r0, #3
	beq _022127DC
	cmp r0, #2
	beq _022127DC
	mov r0, #0
	str r0, [r4, #0x78]
_022127DC:
	ldr r0, [sp, #4]
	add r6, r6, #1
	add r0, #0xc
	add r5, #0xc
	str r0, [sp, #4]
	cmp r6, #0xc
	blt _02212758
	add sp, #0x10c
	pop {r4, r5, r6, r7, pc}
	nop
_022127F0: .word ov96_0221D25C
	thumb_func_end ov96_022124F8
