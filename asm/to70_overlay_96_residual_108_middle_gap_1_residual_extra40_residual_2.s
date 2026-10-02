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
	.public ov96_022124F8
	.public ov96_022127F4
	.public ov96_02212B94

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

	thumb_func_start ov96_022127F4
ov96_022127F4: ; 0x022127F4
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x60
	str r0, [sp, #8]
	bl PokeathlonCourse_GetHeapAllocPtr4
	str r0, [sp, #0x1c]
	mov r0, #0
	str r0, [sp, #0x20]
	ldr r1, _02212B08 ; =0x0000062C
	ldr r0, [sp, #0x1c]
	add r5, r0, r1
_0221280A:
	add r0, r5, #0
	add r0, #0x38
	ldrb r1, [r0]
	cmp r1, #0
	beq _02212822
	add r0, r5, #0
	add r0, #0x39
	ldrb r0, [r0]
	cmp r0, #0
	beq _02212824
	cmp r0, #2
	beq _02212824
_02212822:
	b _02212B82
_02212824:
	cmp r1, #1
	bne _0221282E
	mov r0, #8
	str r0, [sp, #0x18]
	b _02212836
_0221282E:
	cmp r1, #2
	bne _02212836
	mov r0, #0xc
	str r0, [sp, #0x18]
_02212836:
	ldr r0, [sp, #0x18]
	mov r6, #0
	lsl r0, r0, #0xc
	str r0, [sp, #0xc]
	add r0, r5, #0
	str r0, [sp, #0x28]
	add r0, #8
	str r0, [sp, #0x28]
	ldr r0, [sp, #0x1c]
	str r0, [sp, #0x24]
	add r0, #0x5c
	str r0, [sp, #0x24]
_0221284E:
	mov r1, #0
	add r0, sp, #0x54
	str r1, [r0]
	str r1, [r0, #4]
	str r1, [r0, #8]
	add r0, r6, #0
	mov r1, #3
	bl _s32_div_f
	str r1, [sp, #0x34]
	add r0, r6, #0
	mov r1, #3
	bl _s32_div_f
	mov r1, #0x5d
	lsl r1, r1, #2
	str r0, [sp, #0x10]
	mul r1, r0
	ldr r0, [sp, #0x24]
	ldr r2, [sp, #0x34]
	add r1, r0, r1
	mov r0, #0x7c
	mul r0, r2
	add r2, sp, #0x58
	str r2, [sp]
	add r2, sp, #0x38
	add r4, r1, r0
	str r2, [sp, #4]
	ldr r0, [r1, r0]
	ldr r1, [r4, #0x30]
	ldr r2, [r4, #0x34]
	add r3, sp, #0x54
	bl ov96_021EAF78
	ldr r1, [sp, #0x38]
	ldr r2, [sp, #0x28]
	ldr r3, [sp, #0xc]
	add r0, sp, #0x54
	lsl r1, r1, #0xc
	bl ov96_0221341C
	cmp r0, #0
	bne _022128A6
	b _02212ADC
_022128A6:
	add r0, r5, r6
	add r0, #0x2c
	ldrb r0, [r0]
	cmp r0, #0
	beq _022128B6
	ldr r0, [r5, #0x48]
	cmp r0, #0
	bne _022128E6
_022128B6:
	ldr r0, [r4, #0x48]
	cmp r0, #3
	beq _022128E6
	cmp r0, #2
	beq _022128E6
	ldr r0, [sp, #0x28]
	add r1, sp, #0x54
	add r2, sp, #0x48
	bl VEC_Subtract
	add r0, sp, #0x48
	add r1, r0, #0
	bl VEC_Normalize
	add r0, r5, #0
	add r0, #0x39
	ldrb r0, [r0]
	cmp r0, #2
	bne _022129A6
	add r0, r5, #0
	add r0, #0x3b
	ldrb r0, [r0]
	cmp r0, r6
	bne _022128E8
_022128E6:
	b _02212AE4
_022128E8:
	add r0, r4, #0
	add r0, #0x71
	ldrb r1, [r0]
	add r0, r5, #0
	add r0, #0x45
	ldrb r0, [r0]
	sub r1, r1, r0
	cmp r1, #0
	bgt _02212966
	add r1, r4, #0
	add r1, #0x71
	mov r0, #0
	strb r0, [r1]
	mov r0, #2
	add r1, r4, #0
	str r0, [r4, #0x78]
	add r3, r4, #0
	add r3, #0x4c
	add r1, #0x60
	mov r0, #0x5a
	strb r0, [r1]
	mov r0, #3
	add r1, r4, #0
	str r0, [r4, #0x48]
	add r1, #0x5a
	mov r0, #0x1e
	strh r0, [r1]
	add r0, sp, #0x48
	add r2, r0, #0
	str r0, [sp, #0x2c]
	ldmia r2!, {r0, r1}
	add r7, r3, #0
	stmia r3!, {r0, r1}
	add r0, r2, #0
	ldr r0, [r0]
	str r2, [sp, #0x2c]
	str r0, [r3]
	mov r0, #0
	ldr r2, [sp, #0x34]
	ldr r1, [r7]
	mvn r0, r0
	mul r0, r1
	str r0, [r7]
	mov r0, #0
	lsl r2, r2, #0x18
	ldr r1, [r4, #0x50]
	mvn r0, r0
	mul r0, r1
	str r0, [r4, #0x50]
	ldr r1, [sp, #0x10]
	add r4, #0x58
	mov r0, #7
	strh r0, [r4]
	mov r0, #1
	str r0, [sp]
	lsl r1, r1, #0x18
	ldr r0, [sp, #8]
	lsr r1, r1, #0x18
	lsr r2, r2, #0x18
	mov r3, #1
	bl ov96_021E8228
	b _02212AD2
_02212966:
	add r0, r4, #0
	add r0, #0x71
	strb r1, [r0]
	mov r0, #3
	add r1, r4, #0
	str r0, [r4, #0x48]
	add r3, r4, #0
	add r3, #0x4c
	add r1, #0x5a
	mov r0, #0x1e
	strh r0, [r1]
	add r1, r4, #0
	add r1, #0x58
	mov r0, #7
	add r7, sp, #0x48
	strh r0, [r1]
	ldmia r7!, {r0, r1}
	add r2, r3, #0
	stmia r3!, {r0, r1}
	ldr r0, [r7]
	str r0, [r3]
	mov r0, #0
	ldr r1, [r2]
	mvn r0, r0
	mul r0, r1
	str r0, [r2]
	mov r0, #0
	ldr r1, [r4, #0x50]
	mvn r0, r0
	mul r0, r1
	str r0, [r4, #0x50]
	b _02212AD2
_022129A6:
	add r0, r4, #0
	add r0, #0x3c
	bl VEC_Mag
	cmp r0, #0
	bne _022129B4
	b _02212AC2
_022129B4:
	add r0, r4, #0
	add r0, #0x6c
	ldrh r0, [r0]
	mov r1, #0x5a
	lsl r1, r1, #2
	lsl r0, r0, #0x10
	bl _s32_div_f
	add r2, r0, #0
	add r0, r4, #0
	lsl r2, r2, #0x10
	add r0, #0x3c
	add r1, sp, #0x48
	lsr r2, r2, #0x10
	bl ov96_02213534
	ldr r1, [r4, #0x78]
	cmp r1, #1
	bne _02212A86
	cmp r0, #0
	beq _02212A86
	add r1, r5, #0
	add r1, #0x39
	mov r0, #2
	strb r0, [r1]
	add r1, r5, #0
	add r1, #0x3e
	strb r0, [r1]
	add r0, r4, #0
	add r0, #0x73
	ldrb r1, [r0]
	add r0, r5, #0
	add r0, #0x41
	strb r1, [r0]
	add r0, r4, #0
	add r0, #0x72
	ldrb r1, [r0]
	add r0, r5, #0
	add r0, #0x45
	strb r1, [r0]
	add r0, r4, #0
	add r0, #0x75
	ldrb r1, [r0]
	add r0, r5, #0
	add r0, #0x46
	strh r1, [r0]
	add r0, r5, #0
	add r0, #0x41
	ldrb r1, [r0]
	mov r0, #0x1e
	add r2, r1, #0
	mul r2, r0
	ldr r0, _02212B0C ; =ov96_0221D514
	ldrb r0, [r0, r2]
	add r2, r4, #0
	add r2, #0x4c
	str r0, [sp, #0x14]
	mov r0, #1
	str r0, [r4, #0x48]
	add r0, sp, #0x48
	mov ip, r0
	mov r3, ip
	ldmia r3!, {r0, r1}
	add r7, r2, #0
	stmia r2!, {r0, r1}
	add r0, r3, #0
	ldr r0, [r0]
	mov r3, #4
	str r0, [r2]
	mov r0, #0
	ldr r2, [sp, #0x34]
	ldr r1, [r7]
	mvn r0, r0
	mul r0, r1
	str r0, [r7]
	mov r0, #0
	lsl r2, r2, #0x18
	ldr r1, [r4, #0x50]
	mvn r0, r0
	mul r0, r1
	add r1, r4, #0
	str r0, [r4, #0x50]
	add r1, #0x58
	mov r0, #3
	strh r0, [r1]
	mov r0, #0
	add r1, r5, #0
	str r0, [r4, #0x78]
	add r1, #0x3c
	mov r0, #1
	strb r0, [r1]
	add r1, r5, #0
	add r1, #0x3d
	mov r0, #0x1e
	strb r0, [r1]
	ldr r1, [sp, #0x10]
	mov r0, #1
	str r0, [sp]
	lsl r1, r1, #0x18
	ldr r0, [sp, #8]
	lsr r1, r1, #0x18
	lsr r2, r2, #0x18
	bl ov96_021E8228
	b _02212ABA
_02212A86:
	add r1, r5, #0
	add r1, #0x3e
	mov r0, #1
	strb r0, [r1]
	add r4, #0x73
	add r0, r5, #0
	ldrb r1, [r4]
	add r0, #0x41
	strb r1, [r0]
	add r0, r5, #0
	add r0, #0x41
	ldrb r1, [r0]
	mov r0, #0x1e
	add r2, r1, #0
	mul r2, r0
	ldr r0, _02212B10 ; =ov96_0221D5AA
	add r1, r5, #0
	ldrb r0, [r0, r2]
	add r1, #0x3c
	str r0, [sp, #0x14]
	mov r0, #1
	strb r0, [r1]
	add r1, r5, #0
	add r1, #0x3d
	mov r0, #0x1e
	strb r0, [r1]
_02212ABA:
	add r1, r5, #0
	ldr r0, [sp, #0x14]
	add r1, #0x40
	strb r0, [r1]
_02212AC2:
	ldr r0, [sp, #0x1c]
	add r1, r5, #0
	add r2, sp, #0x48
	bl ov96_022134D4
	add r0, r5, #0
	add r0, #0x3b
	strb r6, [r0]
_02212AD2:
	add r1, r5, r6
	add r1, #0x2c
	mov r0, #1
	strb r0, [r1]
	b _02212AE4
_02212ADC:
	add r1, r5, r6
	add r1, #0x2c
	mov r0, #0
	strb r0, [r1]
_02212AE4:
	add r6, r6, #1
	cmp r6, #0xc
	bge _02212AEC
	b _0221284E
_02212AEC:
	add r7, r5, #0
	mov r4, #0
	add r7, #8
_02212AF2:
	lsr r2, r4, #0x1f
	lsl r1, r4, #0x1e
	sub r1, r1, r2
	mov r0, #0x1e
	ror r1, r0
	add r1, r2, r1
	mov r0, #0x18
	add r2, r1, #0
	mul r2, r0
	ldr r0, _02212B14 ; =ov96_0221D4B4
	b _02212B18
	.balign 4, 0
_02212B08: .word 0x0000062C
_02212B0C: .word ov96_0221D514
_02212B10: .word ov96_0221D5AA
_02212B14: .word ov96_0221D4B4
_02212B18:
	ldr r3, [sp, #0xc]
	add r6, r0, r2
	asr r0, r4, #1
	lsr r0, r0, #0x1e
	add r0, r4, r0
	asr r1, r0, #2
	mov r0, #0xc
	mul r0, r1
	mov r1, #2
	str r0, [sp, #0x30]
	add r0, r6, r0
	lsl r1, r1, #0xe
	add r2, r7, #0
	bl ov96_0221341C
	cmp r0, #0
	beq _02212B7C
	ldr r0, [sp, #0x30]
	add r1, r5, #0
	add r0, r6, r0
	add r1, #8
	add r2, sp, #0x3c
	bl VEC_Subtract
	add r0, sp, #0x3c
	add r1, r0, #0
	bl VEC_Normalize
	add r0, r5, #0
	add r0, #0x14
	add r1, sp, #0x3c
	bl VEC_DotProduct
	cmp r0, #0
	ble _02212B82
	add r2, r5, #0
	lsl r0, r0, #1
	add r2, #0x14
	neg r0, r0
	add r1, sp, #0x3c
	add r3, r2, #0
	bl VEC_MultAdd
	add r0, r5, #0
	add r1, r5, #0
	add r0, #0x14
	add r1, #0x20
	bl VEC_Normalize
	b _02212B82
_02212B7C:
	add r4, r4, #1
	cmp r4, #8
	blt _02212AF2
_02212B82:
	ldr r0, [sp, #0x20]
	add r5, #0x4c
	add r0, r0, #1
	str r0, [sp, #0x20]
	cmp r0, #2
	bge _02212B90
	b _0221280A
_02212B90:
	add sp, #0x60
	pop {r3, r4, r5, r6, r7, pc}
	thumb_func_end ov96_022127F4

	thumb_func_start ov96_02212B94
ov96_02212B94: ; 0x02212B94
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x28
	bl PokeathlonCourse_GetHeapAllocPtr4
	mov r1, #0x6b
	add r5, r0, #0
	lsl r1, r1, #4
	ldrb r0, [r5, r1]
	cmp r0, #0
	bne _02212BC0
	add r0, r1, #0
	add r0, #0x88
	ldr r2, [r5, r0]
	mov r0, #0xe1
	lsl r0, r0, #2
	cmp r2, r0
	bgt _02212BC0
	mov r2, #3
	add r0, r1, #1
	strb r2, [r5, r0]
	mov r0, #1
	strb r0, [r5, r1]
_02212BC0:
	ldr r0, _02212EC4 ; =0x0000062C
	mov r7, #0
	add r4, r5, r0
_02212BC6:
	add r0, r4, #0
	add r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	beq _02212CB4
	add r0, r4, #0
	add r0, #0x39
	ldrb r0, [r0]
	cmp r0, #3
	bne _02212C02
	add r0, r4, #0
	add r0, #0x43
	ldrb r0, [r0]
	add r1, r0, #1
	add r0, r4, #0
	add r0, #0x43
	strb r1, [r0]
	add r0, r4, #0
	add r0, #0x43
	ldrb r0, [r0]
	cmp r0, #8
	blo _02212CB4
	add r1, r4, #0
	add r1, #0x43
	mov r0, #0
	strb r0, [r1]
	add r1, r4, #0
	add r1, #0x39
	strb r0, [r1]
	b _02212E70
_02212C02:
	cmp r0, #1
	bne _02212CAC
	add r0, r4, #0
	add r0, #0x3a
	ldrb r0, [r0]
	sub r1, r0, #1
	add r0, r4, #0
	add r0, #0x3a
	strb r1, [r0]
	add r0, r4, #0
	add r0, #0x3a
	ldrb r0, [r0]
	cmp r0, #0
	bne _02212CB4
	add r0, r4, #0
	bl ov96_02213444
	add r0, r5, #0
	add r1, r4, #0
	bl ov96_02213E60
	cmp r7, #0
	bne _02212CA2
	ldr r0, _02212EC8 ; =0x00000738
	mov r1, #0x4b
	ldr r0, [r5, r0]
	lsl r1, r1, #4
	cmp r0, r1
	blt _02212C40
	mov r6, #0
	b _02212C58
_02212C40:
	lsr r1, r1, #1
	cmp r0, r1
	blt _02212C4A
	mov r6, #5
	b _02212C58
_02212C4A:
	mov r1, #0x4b
	lsl r1, r1, #2
	cmp r0, r1
	blt _02212C56
	mov r6, #0xa
	b _02212C58
_02212C56:
	mov r6, #0x19
_02212C58:
	ldr r0, _02212ECC ; =0x00000743
	ldrb r0, [r5, r0]
	add r0, r6, r0
	lsl r0, r0, #0x18
	lsr r6, r0, #0x18
	cmp r6, #0x64
	bls _02212C68
	mov r6, #0x64
_02212C68:
	bl LCRandom
	mov r1, #0x64
	bl _s32_div_f
	cmp r1, r6
	bge _02212C86
	add r1, r4, #0
	add r1, #0x38
	mov r0, #2
	strb r0, [r1]
	ldr r0, _02212ECC ; =0x00000743
	mov r1, #0
	strb r1, [r5, r0]
	b _02212E70
_02212C86:
	add r1, r4, #0
	add r1, #0x38
	mov r0, #1
	strb r0, [r1]
	ldr r0, _02212ECC ; =0x00000743
	ldrb r0, [r5, r0]
	cmp r0, #0x64
	bhs _02212CB4
	ldr r0, _02212ECC ; =0x00000743
	ldrb r0, [r5, r0]
	add r1, r0, #1
	ldr r0, _02212ECC ; =0x00000743
	strb r1, [r5, r0]
	b _02212E70
_02212CA2:
	add r1, r4, #0
	add r1, #0x38
	mov r0, #1
	strb r0, [r1]
	b _02212E70
_02212CAC:
	cmp r0, #0
	beq _02212CB6
	cmp r0, #2
	beq _02212CB6
_02212CB4:
	b _02212E70
_02212CB6:
	add r0, r4, #0
	add r0, #0x40
	ldrb r0, [r0]
	lsl r6, r0, #0xc
	bne _02212CC2
	b _02212E56
_02212CC2:
	add r3, r4, #0
	add r3, #8
	ldmia r3!, {r0, r1}
	add r2, sp, #0x1c
	stmia r2!, {r0, r1}
	ldr r0, [r3]
	str r0, [r2]
	ldr r1, [r4, #8]
	ldr r0, [r4, #0x14]
	add r0, r1, r0
	str r0, [r4, #8]
	ldr r1, [r4, #0xc]
	ldr r0, [r4, #0x18]
	add r0, r1, r0
	str r0, [r4, #0xc]
	add r0, r4, #0
	add r0, #0x46
	ldrh r1, [r0]
	ldr r0, _02212ED0 ; =ov96_0221D640
	ldrb r0, [r0, r1]
	lsl r0, r0, #0xc
	cmp r6, r0
	blt _02212D0E
	add r0, r4, #0
	add r0, #0x3e
	ldrb r0, [r0]
	cmp r0, #2
	bne _02212D04
	add r1, r4, #0
	add r1, #0x39
	mov r0, #2
	strb r0, [r1]
	b _02212D1C
_02212D04:
	add r1, r4, #0
	add r1, #0x39
	mov r0, #0
	strb r0, [r1]
	b _02212D1C
_02212D0E:
	add r1, r4, #0
	add r1, #0x39
	mov r0, #0
	strb r0, [r1]
	add r1, r4, #0
	add r1, #0x45
	strb r0, [r1]
_02212D1C:
	mov r1, #0
	add r0, sp, #0x10
	str r1, [r0]
	str r1, [r0, #4]
	str r1, [r0, #8]
	add r1, r4, #0
	add r1, #0x3e
	add r0, r4, #0
	add r0, #0x3c
	ldrb r1, [r1]
	ldrb r0, [r0]
	cmp r1, #2
	bne _02212D48
	add r1, r4, #0
	add r1, #0x41
	ldrb r1, [r1]
	mov r2, #0x1e
	mul r2, r1
	ldr r1, _02212ED4 ; =ov96_0221D514
	add r1, r1, r2
	ldrb r1, [r0, r1]
	b _02212D64
_02212D48:
	cmp r1, #1
	bne _02212D5E
	add r1, r4, #0
	add r1, #0x41
	ldrb r1, [r1]
	mov r2, #0x1e
	mul r2, r1
	ldr r1, _02212ED8 ; =ov96_0221D5AA
	add r1, r1, r2
	ldrb r1, [r0, r1]
	b _02212D64
_02212D5E:
	bl GF_AssertFail
	mov r1, #0
_02212D64:
	add r0, r4, #0
	add r0, #0x40
	strb r1, [r0]
	add r2, r4, #0
	add r0, r5, #0
	add r1, r4, #0
	add r2, #0x20
	bl ov96_022134D4
	add r0, r4, #0
	add r0, #0x3c
	ldrb r0, [r0]
	add r1, r0, #1
	add r0, r4, #0
	add r0, #0x3c
	strb r1, [r0]
	add r0, r4, #0
	add r0, #0x3c
	ldrb r1, [r0]
	add r0, r4, #0
	add r0, #0x3d
	ldrb r0, [r0]
	cmp r1, r0
	blo _02212DB8
	mov r0, #0
	str r0, [r4, #0x14]
	add r2, r4, #0
	add r3, sp, #0x10
	str r0, [r4, #0x18]
	ldmia r3!, {r0, r1}
	add r2, #0x20
	stmia r2!, {r0, r1}
	ldr r0, [r3]
	add r1, r4, #0
	str r0, [r2]
	add r1, #0x3e
	mov r0, #0
	strb r0, [r1]
	add r1, r4, #0
	add r1, #0x40
	strb r0, [r1]
	b _02212DE4
_02212DB8:
	add r0, r4, #0
	add r0, #0x40
	ldrb r0, [r0]
	cmp r0, #0
	bne _02212DE4
	add r3, sp, #0x10
	add r2, r4, #0
	ldmia r3!, {r0, r1}
	add r2, #0x20
	stmia r2!, {r0, r1}
	ldr r0, [r3]
	add r1, r4, #0
	str r0, [r2]
	add r1, #0x3c
	mov r0, #0
	strb r0, [r1]
	add r1, r4, #0
	add r1, #0x3d
	strb r0, [r1]
	add r1, r4, #0
	add r1, #0x3e
	strb r0, [r1]
_02212DE4:
	mov r0, #0
	str r0, [r4, #0x48]
	add r0, sp, #4
	str r0, [sp]
	add r0, r4, #0
	add r0, #8
	add r1, sp, #0x1c
	mov r2, #8
	mov r3, #0
	bl ov96_02213728
	add r3, r0, #0
	beq _02212E10
	add r6, sp, #4
	add r2, r4, #0
	ldmia r6!, {r0, r1}
	add r2, #8
	stmia r2!, {r0, r1}
	ldr r0, [r6]
	str r0, [r2]
	mov r0, #1
	str r0, [r4, #0x48]
_02212E10:
	cmp r3, #4
	bhi _02212E70
	add r0, r3, r3
	add r0, pc
	ldrh r0, [r0, #6]
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	add pc, r0
_02212E20: ; jump table
	.short _02212E70 - _02212E20 - 2 ; case 0
	.short _02212E40 - _02212E20 - 2 ; case 1
	.short _02212E40 - _02212E20 - 2 ; case 2
	.short _02212E2A - _02212E20 - 2 ; case 3
	.short _02212E2A - _02212E20 - 2 ; case 4
_02212E2A:
	mov r0, #0
	ldr r1, [r4, #0x24]
	mvn r0, r0
	mul r0, r1
	str r0, [r4, #0x24]
	mov r0, #0
	ldr r1, [r4, #0x18]
	mvn r0, r0
	mul r0, r1
	str r0, [r4, #0x18]
	b _02212E70
_02212E40:
	mov r0, #0
	ldr r1, [r4, #0x20]
	mvn r0, r0
	mul r0, r1
	str r0, [r4, #0x20]
	mov r0, #0
	ldr r1, [r4, #0x14]
	mvn r0, r0
	mul r0, r1
	str r0, [r4, #0x14]
	b _02212E70
_02212E56:
	add r1, r4, #0
	add r1, #0x39
	mov r0, #0
	strb r0, [r1]
	add r1, r4, #0
	add r1, #0x45
	strb r0, [r1]
	add r1, r4, #0
	add r1, #0x46
	strh r0, [r1]
	add r1, r4, #0
	add r1, #0x3e
	strb r0, [r1]
_02212E70:
	add r7, r7, #1
	add r4, #0x4c
	cmp r7, #2
	bge _02212E7A
	b _02212BC6
_02212E7A:
	ldr r4, _02212EDC ; =0x00000664
	ldrb r0, [r5, r4]
	cmp r0, #0
	beq _02212F06
	add r0, r4, #0
	add r0, #0x4c
	ldrb r0, [r5, r0]
	cmp r0, #0
	beq _02212F06
	add r6, r4, #0
	sub r6, #0x38
	add r0, r5, r6
	add r0, #0x39
	ldrb r0, [r0]
	add r4, #0x14
	cmp r0, #1
	beq _02212F06
	cmp r0, #3
	beq _02212F06
	add r0, r5, r4
	add r0, #0x39
	ldrb r0, [r0]
	cmp r0, #1
	beq _02212F06
	cmp r0, #3
	beq _02212F06
	add r0, r5, r6
	add r0, #0x38
	ldrb r0, [r0]
	cmp r0, #1
	bne _02212EBC
	mov r1, #8
	b _02212EE8
_02212EBC:
	cmp r0, #2
	bne _02212EE0
	mov r1, #0xc
	b _02212EE8
	.balign 4, 0
_02212EC4: .word 0x0000062C
_02212EC8: .word 0x00000738
_02212ECC: .word 0x00000743
_02212ED0: .word ov96_0221D640
_02212ED4: .word ov96_0221D514
_02212ED8: .word ov96_0221D5AA
_02212EDC: .word 0x00000664
_02212EE0:
	bl GF_AssertFail
	add sp, #0x28
	pop {r3, r4, r5, r6, r7, pc}
_02212EE8:
	add r0, r5, r6
	add r2, r5, r4
	mov r3, #2
	add r0, #8
	lsl r1, r1, #0xc
	add r2, #8
	lsl r3, r3, #0xe
	bl ov96_0221341C
	cmp r0, #0
	beq _02212F06
	add r0, r5, r6
	add r1, r5, r4
	bl ov96_02212F0C
_02212F06:
	add sp, #0x28
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
	thumb_func_end ov96_02212B94
