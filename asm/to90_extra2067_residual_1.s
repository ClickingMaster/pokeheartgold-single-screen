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
	.extern ov96_02205AFC

.text
	thumb_func_start ov96_022055AC
ov96_022055AC: ; 0x022055AC
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x78
	str r0, [sp]
	bl PokeathlonCourse_GetDataCopyArea
	str r0, [sp, #0x28]
	ldr r0, [sp]
	bl PokeathlonCourse_GetHeapAllocPtr4
	str r0, [sp, #0x2c]
	ldr r0, [sp]
	bl ov96_021E5F24
	cmp r0, #0
	beq _022055CC
	b _02205AE0
_022055CC:
	ldr r1, _022058B4 ; =0x0000050E
	ldr r0, [sp, #0x2c]
	ldrb r0, [r0, r1]
	cmp r0, #0
	beq _022055EC
	ldr r0, [sp, #0x28]
	add r0, #0x28
	str r0, [sp, #0x28]
	bl ov96_021E8A20
	add r1, r0, #0
	ldr r0, [sp, #0x2c]
	bl ov96_02205AFC
	add sp, #0x78
	pop {r3, r4, r5, r6, r7, pc}
_022055EC:
	ldr r0, [sp, #0x28]
	add r0, #0x50
	bl ov96_021E8A20
	add r4, r0, #0
	ldr r0, [sp, #0x28]
	bl ov96_021E8A20
	add r3, r0, #0
	mov r2, #4
_02205600:
	ldmia r3!, {r0, r1}
	stmia r4!, {r0, r1}
	sub r2, r2, #1
	bne _02205600
	ldr r0, [r3]
	ldr r1, _022058B8 ; =0x0000050C
	str r0, [r4]
	ldr r0, [sp, #0x2c]
	ldrh r0, [r0, r1]
	cmp r0, #0
	beq _0220561C
	sub r2, r0, #1
	ldr r0, [sp, #0x2c]
	strh r2, [r0, r1]
_0220561C:
	mov r1, #0xdb
	ldr r0, [sp, #0x2c]
	lsl r1, r1, #2
	ldr r0, [r0, r1]
	bl ov96_02208B34
	mov r0, #0
	ldr r5, [sp, #0x2c]
	str r0, [sp, #0x30]
	add r1, r0, #0
	ldr r2, _022058BC ; =0x0000051B
	ldr r0, [sp, #0x2c]
	add r4, r5, #0
	strb r1, [r0, r2]
	ldr r0, [sp, #0x28]
	add r6, r5, #0
	str r0, [sp, #0x18]
	add r0, #0x50
	str r0, [sp, #0x18]
	add r0, r5, #0
	str r0, [sp, #0x14]
	add r0, #0x24
	str r0, [sp, #0x14]
	add r0, r5, #0
	str r0, [sp, #0x10]
	add r0, #0xa0
	str r0, [sp, #0x10]
	add r0, r5, #0
	str r0, [sp, #0xc]
	add r0, #0xac
	str r0, [sp, #0xc]
	add r0, r5, #0
	str r0, [sp, #8]
	add r0, #0x88
	str r0, [sp, #8]
	str r0, [sp, #4]
_02205664:
	ldr r0, [sp, #0x18]
	bl ov96_021E8A20
	add r7, r0, #0
	ldr r0, [r7]
	cmp r0, #0
	beq _022056AA
	mov r0, #0xc1
	lsl r0, r0, #2
	ldr r1, [r5, r0]
	cmp r1, #0
	beq _0220568E
	add r0, r0, #4
	ldr r0, [r5, r0]
	cmp r0, #0
	beq _0220568E
	mov r0, #0xc1
	mov r1, #0
	lsl r0, r0, #2
	str r1, [r5, r0]
	b _022056B6
_0220568E:
	cmp r1, #0
	bne _022056B6
	mov r0, #0xc2
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	cmp r0, #0
	bne _022056B6
	mov r0, #0xc1
	mov r1, #1
	lsl r0, r0, #2
	str r1, [r5, r0]
	add r0, r0, #4
	str r1, [r5, r0]
	b _022056B6
_022056AA:
	mov r0, #0xc1
	mov r1, #0
	lsl r0, r0, #2
	str r1, [r5, r0]
	add r0, r0, #4
	str r1, [r5, r0]
_022056B6:
	add r0, r4, #0
	add r0, #0xc0
	ldrb r0, [r0]
	cmp r0, #0
	beq _02205750
	add r0, r4, #0
	add r0, #0xc1
	ldrb r1, [r0]
	cmp r1, #1
	bne _022056F6
	ldrb r0, [r7, #8]
	cmp r0, #1
	bne _022056F6
	add r0, r4, #0
	add r0, #0xd5
	ldrb r0, [r0]
	mov r1, #3
	add r0, r0, #1
	bl _s32_div_f
	add r0, r4, #0
	add r0, #0xd5
	strb r1, [r0]
	add r1, r4, #0
	add r1, #0xc1
	mov r0, #2
	strb r0, [r1]
	add r1, r4, #0
	add r1, #0xd4
	mov r0, #1
	strb r0, [r1]
	b _0220571C
_022056F6:
	cmp r1, #2
	bne _0220571C
	ldrb r0, [r7, #8]
	cmp r0, #2
	bne _0220571C
	add r1, r4, #0
	add r1, #0xc1
	mov r0, #0
	strb r0, [r1]
	add r1, r4, #0
	add r1, #0xc0
	strb r0, [r1]
	add r1, r4, #0
	add r1, #0xcf
	strb r0, [r1]
	add r1, r4, #0
	add r1, #0xd4
	mov r0, #1
	strb r0, [r1]
_0220571C:
	mov r0, #0xcd
	mov r1, #0
	lsl r0, r0, #2
	str r1, [r6, r0]
	add r1, r4, #0
	add r1, #0x9e
	mov r0, #0
	strh r0, [r1]
	add r1, r4, #0
	add r1, #0xa0
	str r0, [r1]
	add r1, r4, #0
	add r1, #0xa4
	str r0, [r1]
	add r1, r4, #0
	add r3, r4, #0
	add r1, #0xa8
	add r2, r4, #0
	add r3, #0xa0
	str r0, [r1]
	ldmia r3!, {r0, r1}
	add r2, #0xac
	stmia r2!, {r0, r1}
	ldr r0, [r3]
	str r0, [r2]
	b _02205A72
_02205750:
	add r0, r4, #0
	add r0, #0xca
	ldrb r0, [r0]
	cmp r0, #1
	bne _0220578E
	mov r0, #0xcd
	mov r1, #0
	lsl r0, r0, #2
	str r1, [r6, r0]
	add r1, r4, #0
	add r1, #0x9e
	mov r0, #0
	strh r0, [r1]
	add r1, r4, #0
	add r1, #0xa0
	str r0, [r1]
	add r1, r4, #0
	add r1, #0xa4
	str r0, [r1]
	add r1, r4, #0
	add r3, r4, #0
	add r1, #0xa8
	add r2, r4, #0
	add r3, #0xa0
	str r0, [r1]
	ldmia r3!, {r0, r1}
	add r2, #0xac
	stmia r2!, {r0, r1}
	ldr r0, [r3]
	str r0, [r2]
	b _02205A72
_0220578E:
	add r0, r4, #0
	add r0, #0xc8
	ldrb r0, [r0]
	cmp r0, #0
	beq _022057CC
	mov r0, #0xcd
	mov r1, #0
	lsl r0, r0, #2
	str r1, [r6, r0]
	add r1, r4, #0
	add r1, #0x9e
	mov r0, #0
	strh r0, [r1]
	add r1, r4, #0
	add r1, #0xa0
	str r0, [r1]
	add r1, r4, #0
	add r1, #0xa4
	str r0, [r1]
	add r1, r4, #0
	add r3, r4, #0
	add r1, #0xa8
	add r2, r4, #0
	add r3, #0xa0
	str r0, [r1]
	ldmia r3!, {r0, r1}
	add r2, #0xac
	stmia r2!, {r0, r1}
	ldr r0, [r3]
	str r0, [r2]
	b _02205A72
_022057CC:
	add r0, r4, #0
	add r0, #0xcf
	ldrb r0, [r0]
	cmp r0, #0
	beq _0220580A
	mov r0, #0xcd
	mov r1, #0
	lsl r0, r0, #2
	str r1, [r6, r0]
	add r1, r4, #0
	add r1, #0x9e
	mov r0, #0
	strh r0, [r1]
	add r1, r4, #0
	add r1, #0xa0
	str r0, [r1]
	add r1, r4, #0
	add r1, #0xa4
	str r0, [r1]
	add r1, r4, #0
	add r3, r4, #0
	add r1, #0xa8
	add r2, r4, #0
	add r3, #0xa0
	str r0, [r1]
	ldmia r3!, {r0, r1}
	add r2, #0xac
	stmia r2!, {r0, r1}
	ldr r0, [r3]
	str r0, [r2]
	b _02205A72
_0220580A:
	mov r0, #0
	str r0, [sp, #0x24]
	mov r0, #0xc1
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	cmp r0, #0
	beq _02205862
	ldr r1, [sp, #0x30]
	ldrb r2, [r7, #4]
	lsl r1, r1, #0x18
	ldrb r3, [r7, #5]
	ldr r0, [sp, #0x2c]
	lsr r1, r1, #0x18
	bl ov96_02205DD4
	cmp r0, #0
	beq _022058F2
	mov r0, #0xcd
	mov r1, #1
	lsl r0, r0, #2
	str r1, [r6, r0]
	ldrb r0, [r7, #4]
	add r3, r4, #0
	add r2, r4, #0
	lsl r1, r0, #0xc
	add r0, r4, #0
	add r0, #0xa0
	str r1, [r0]
	ldrb r0, [r7, #5]
	add r3, #0xa0
	add r2, #0xac
	lsl r1, r0, #0xc
	add r0, r4, #0
	add r0, #0xa4
	str r1, [r0]
	add r1, r4, #0
	add r1, #0xa8
	mov r0, #0
	str r0, [r1]
	ldmia r3!, {r0, r1}
	stmia r2!, {r0, r1}
	ldr r0, [r3]
	str r0, [r2]
	b _022058F2
_02205862:
	mov r0, #0xc2
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	cmp r0, #0
	beq _022058C0
	add r0, r4, #0
	add r0, #0x9e
	ldrh r0, [r0]
	cmp r0, #0xff
	bhs _02205886
	add r0, r4, #0
	add r0, #0x9e
	ldrh r0, [r0]
	add r1, r0, #1
	add r0, r4, #0
	add r0, #0x9e
	strh r1, [r0]
	b _02205890
_02205886:
	mov r0, #0x51
	ldr r1, [sp, #0x2c]
	mov r2, #1
	lsl r0, r0, #4
	strb r2, [r1, r0]
_02205890:
	mov r0, #0xcd
	lsl r0, r0, #2
	ldr r0, [r6, r0]
	cmp r0, #0
	bne _0220589C
	b _02205A72
_0220589C:
	ldrb r0, [r7, #4]
	lsl r1, r0, #0xc
	add r0, r4, #0
	add r0, #0xac
	str r1, [r0]
	ldrb r0, [r7, #5]
	lsl r1, r0, #0xc
	add r0, r4, #0
	add r0, #0xb0
	str r1, [r0]
	b _022058F2
	nop
_022058B4: .word 0x0000050E
_022058B8: .word 0x0000050C
_022058BC: .word 0x0000051B
_022058C0:
	mov r0, #0xcd
	lsl r0, r0, #2
	ldr r0, [r6, r0]
	cmp r0, #0
	beq _022058EE
	add r0, r4, #0
	add r0, #0x9e
	ldrh r0, [r0]
	cmp r0, #0x1e
	bhi _022058DC
	add r1, r4, #0
	add r1, #0xc9
	mov r0, #1
	strb r0, [r1]
_022058DC:
	mov r0, #0xcd
	mov r1, #0
	lsl r0, r0, #2
	str r1, [r6, r0]
	add r2, r1, #0
	mov r0, #0x51
	ldr r1, [sp, #0x2c]
	lsl r0, r0, #4
	strb r2, [r1, r0]
_022058EE:
	mov r0, #1
	str r0, [sp, #0x24]
_022058F2:
	add r0, r4, #0
	add r0, #0xc9
	ldrb r0, [r0]
	cmp r0, #0
	bne _022058FE
	b _02205A42
_022058FE:
	mov r1, #0
	add r0, sp, #0x54
	str r1, [r0]
	str r1, [r0, #4]
	str r1, [r0, #8]
	add r0, r4, #0
	add r0, #0xd5
	ldrb r1, [r0]
	mov r0, #0x14
	mul r0, r1
	add r2, r4, r0
	ldr r0, [r2, #0x30]
	str r0, [sp, #0x20]
	ldr r0, [r2, #0x34]
	str r0, [sp, #0x1c]
	ldr r0, [sp, #0x14]
	bl ov96_022073F0
	add r7, r0, #0
	ldr r0, [sp, #0x20]
	ldr r1, [sp, #0x1c]
	bl _fmul
	bl _f2d
	ldr r3, _02205AE4 ; =0x40200000
	mov r2, #0
	bl _ddiv
	add r3, r1, #0
	add r2, r0, #0
	ldr r1, _02205AE8 ; =0x40B00000
	mov r0, #0
	bl _dmul
	bl _dfix
	str r0, [sp, #0x34]
	ldr r0, [sp, #0xc]
	ldr r1, [sp, #0x10]
	add r2, sp, #0x60
	bl VEC_Subtract
	add r1, sp, #0x60
	ldr r0, [sp, #0x34]
	add r2, sp, #0x54
	add r3, r1, #0
	bl VEC_MultAdd
	add r0, sp, #0x60
	bl VEC_Mag
	asr r1, r0, #0xb
	lsr r1, r1, #0x14
	add r1, r0, r1
	asr r0, r1, #0xc
	bl _fflt
	add r2, r4, #0
	add r2, #0x88
	add r3, sp, #0x6c
	ldmia r2!, {r0, r1}
	mov ip, r3
	stmia r3!, {r0, r1}
	ldr r0, [r2]
	add r1, sp, #0x60
	str r0, [r3]
	mov r0, ip
	mov r2, ip
	bl ov96_02207400
	add r0, sp, #0x6c
	bl VEC_Mag
	bl _fflt
	str r0, [sp, #0x38]
	ldr r0, _02205AEC ; =0x45800000
	add r1, r7, #0
	bl _fmul
	add r1, r0, #0
	ldr r0, [sp, #0x38]
	bl _fgr
	bls _022059EA
	mov r1, #0
	add r0, sp, #0x3c
	str r1, [r0]
	str r1, [r0, #4]
	str r1, [r0, #8]
	add r0, sp, #0x6c
	add r1, r0, #0
	bl VEC_Normalize
	ldr r0, _02205AEC ; =0x45800000
	add r1, r7, #0
	bl _fmul
	bl _ffix
	ldr r3, [sp, #8]
	add r1, sp, #0x6c
	add r2, sp, #0x3c
	bl VEC_MultAdd
	ldr r0, _02205AEC ; =0x45800000
	add r1, r7, #0
	bl _fmul
	bl _ffix
	add r1, sp, #0x6c
	add r2, sp, #0x3c
	add r3, sp, #0x48
	bl VEC_MultAdd
	b _02205A00
_022059EA:
	ldr r0, [sp, #4]
	ldr r2, [sp, #8]
	add r1, sp, #0x60
	bl ov96_02207400
	add r3, sp, #0x60
	ldmia r3!, {r0, r1}
	add r2, sp, #0x48
	stmia r2!, {r0, r1}
	ldr r0, [r3]
	str r0, [r2]
_02205A00:
	ldr r0, [sp, #4]
	bl ov96_022079B8
	add r1, r4, #0
	add r1, #0xd4
	strb r0, [r1]
	ldr r1, [sp, #0x2c]
	ldr r0, _02205AF0 ; =0x0000051B
	mov r2, #1
	strb r2, [r1, r0]
	ldr r1, [r4, #0x7c]
	asr r0, r1, #0xb
	lsr r0, r0, #0x14
	add r0, r1, r0
	asr r2, r0, #0xc
	ldr r1, [sp, #0x2c]
	ldr r0, _02205AF4 ; =0x00000522
	strh r2, [r1, r0]
	add r0, r4, #0
	add r0, #0x80
	ldr r1, [r0]
	asr r0, r1, #0xb
	lsr r0, r0, #0x14
	add r0, r1, r0
	asr r2, r0, #0xc
	mov r0, #0x52
	ldr r1, [sp, #0x2c]
	lsl r0, r0, #4
	strh r2, [r1, r0]
	add r1, r4, #0
	add r1, #0xc9
	mov r0, #0
	strb r0, [r1]
_02205A42:
	ldr r0, [sp, #0x24]
	cmp r0, #0
	beq _02205A72
	add r1, r4, #0
	add r1, #0x9e
	mov r0, #0
	strh r0, [r1]
	add r1, r4, #0
	add r1, #0xa0
	str r0, [r1]
	add r1, r4, #0
	add r1, #0xa4
	str r0, [r1]
	add r1, r4, #0
	add r3, r4, #0
	add r1, #0xa8
	add r2, r4, #0
	add r3, #0xa0
	str r0, [r1]
	ldmia r3!, {r0, r1}
	add r2, #0xac
	stmia r2!, {r0, r1}
	ldr r0, [r3]
	str r0, [r2]
_02205A72:
	ldr r0, [sp, #0x18]
	add r5, #0xc
	add r0, #0x28
	str r0, [sp, #0x18]
	ldr r0, [sp, #0x14]
	add r4, #0xb8
	add r0, #0xb8
	str r0, [sp, #0x14]
	ldr r0, [sp, #0x10]
	add r6, r6, #4
	add r0, #0xb8
	str r0, [sp, #0x10]
	ldr r0, [sp, #0xc]
	add r0, #0xb8
	str r0, [sp, #0xc]
	ldr r0, [sp, #8]
	add r0, #0xb8
	str r0, [sp, #8]
	ldr r0, [sp, #4]
	add r0, #0xb8
	str r0, [sp, #4]
	ldr r0, [sp, #0x30]
	add r0, r0, #1
	str r0, [sp, #0x30]
	cmp r0, #4
	bge _02205AA8
	b _02205664
_02205AA8:
	ldr r0, [sp]
	bl ov96_02205E30
	ldr r0, [sp, #0x2c]
	bl ov96_02207870
	ldr r1, _02205AF8 ; =0x0000050C
	ldr r0, [sp, #0x2c]
	ldrh r0, [r0, r1]
	cmp r0, #0
	bne _02205AC6
	mov r2, #1
	add r1, r1, #2
	ldr r0, [sp, #0x2c]
	b _02205ACC
_02205AC6:
	ldr r0, [sp, #0x2c]
	mov r2, #0
	add r1, r1, #2
_02205ACC:
	strb r2, [r0, r1]
	ldr r0, [sp, #0x28]
	add r0, #0x28
	str r0, [sp, #0x28]
	bl ov96_021E8A20
	add r1, r0, #0
	ldr r0, [sp, #0x2c]
	bl ov96_02205AFC
_02205AE0:
	add sp, #0x78
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
_02205AE4: .word 0x40200000
_02205AE8: .word 0x40B00000
_02205AEC: .word 0x45800000
_02205AF0: .word 0x0000051B
_02205AF4: .word 0x00000522
_02205AF8: .word 0x0000050C
	thumb_func_end ov96_022055AC
