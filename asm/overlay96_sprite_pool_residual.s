.set R86_GAP, 0
	.include "asm/macros.inc"
	.include "overlay_96.inc"
	.include "global.inc"
	.include "overlay_96_exact_bank.inc"

	.text
	; This file is a generated integration template. It is not linked directly.

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
	.public ov96_021E60C0
	.public ov96_021E6104
	.public ov96_021E6670
	.public ov96_021E679C
	.public ov96_021E6F18
	.public ov96_021E75B8
	.public ov96_021E7F98
	.public ov96_021E8318
	.public ov96_021E87B0
	.public ov96_021E8828
	.public ov96_021E8A20
	.public ov96_021E8A24
	.public ov96_021E8A2C
	.public ov96_021E8A30
	.public ov96_021E8B88
	.public ov96_021E8BAC
	.public ov96_021E8BB0
	.public ov96_021E90FC
	.public ov96_021E92AC
	.public ov96_021E92D0
	.public ov96_021E9510
	.public ov96_021E9524
	.public ov96_021E9528
	.public ov96_021E99F4
	.public ov96_021E99F8
	.public ov96_021E99FC
	.public ov96_021E9A04
	.public ov96_021E9A10
	.public ov96_021E9A14
	.public ov96_021E9A18
	.public ov96_021E9A1C
	.public ov96_021E9A24
	.public ov96_021E9A2C
	.public ov96_021E9A38
	.public ov96_021E9A48
	.public ov96_021E9A54
	.public ov96_021E9A68
	.public ov96_021E9CF4
	.public ov96_021E9D08
	.public ov96_021EA894
	.public ov96_021EAA00
	.public ov96_021EAA20
	.public ov96_021EAB94
	.public ov96_021EABDC
	.public ov96_021EABE0
	.public ov96_021EABF4
	.public ov96_021EAC08
	.public ov96_021EAD78
	.public ov96_021EAEC8
	.public ov96_021EAF6C
	.public ov96_021EAF70
	.public ov96_021EAF8C
	.public ov96_021EAF90
	.public ov96_021EB10C
	.public ov96_021EB138
	.public ov96_021EB564
	.public ov96_021EB570
	.public ov96_021EB57C
	.public ov96_021EB588
	.public ov96_021EB594
	.public ov96_021EB5A0
	.public ov96_021EB5AC
	.public ov96_021EB5B8
	.public ov96_021EB5BC
	.public ov96_021EB5E8
	.public ov96_021EB630
	.public ov96_021EB678
	.public ov96_021EB680
	.public ov96_021EB688
	.public ov96_021EB690
	.public ov96_021EB698
	.public ov96_021EB6A0
	.public ov96_021EC2E0
	.public ov96_021ECBF4
	.public ov96_021ECC38
	.public ov96_021ECC4C
	.public ov96_021ECDC4
	.public ov96_021ECDD4
	.public ov96_021ED47C
	.public ov96_021ED6E8
	.public ov96_021ED748
	.public ov96_021ED74C
	.public ov96_021ED750
	.public ov96_021EDF7C
	.public ov96_021EEA80
	.public ov96_021EED64
	.public ov96_021EEEBC
	.public ov96_021EEEC0
	.public ov96_021EEF98
	.public ov96_021EF18C
	.public ov96_021EF2A0
	.public ov96_021EF2AC
	.public ov96_021EF98C
	.public ov96_021EFA28
	.public ov96_021EFB50
	.public ov96_021F08E4
	.public ov96_021F27A8
	.public ov96_021F30BC
	.public ov96_021F3424
	.public ov96_021F3E58
	.public ov96_021F46B4
	.public ov96_021F54C0
	.public ov96_021F74C8
	.public ov96_021F75D4
	.public ov96_021F75E0
	.public ov96_021F7738
	.public ov96_021F8094
	.public ov96_021F81C0
	.public ov96_021F8354
	.public ov96_021F83BC
	.public ov96_021F83D0
	.public ov96_021F87D0
	.public ov96_021F893C
	.public ov96_021F8948
	.public ov96_021F8978
	.public ov96_021F9234
	.public ov96_021FA340
	.public ov96_021FBE3C
	.public ov96_021FBE44
	.public ov96_021FBE4C
	.public ov96_021FBE9C
	.public ov96_021FBEA0
	.public ov96_021FC0D0
	.public ov96_021FC0E4
	.public ov96_021FC0E8
	.public ov96_021FC0F4
	.public ov96_021FC314
	.public ov96_021FC690
	.public ov96_021FCD74
	.public ov96_021FDE6C
	.public ov96_021FE538
	.public ov96_02200BC8
	.public ov96_02200E78
	.public ov96_02200EEC
	.public ov96_02203BC0
	.public ov96_02203CC4
	.public ov96_02203CD4
	.public ov96_022043AC
	.public ov96_022043B8
	.public ov96_02204DE8
	.public ov96_02206368
	.public ov96_02207400
	.public ov96_022077F4
	.public ov96_02207BD4
	.public ov96_02207BD8
	.public ov96_02208AE8
	.public ov96_02208AF0
	.public ov96_02208B2C
	.public ov96_02209738
	.public ov96_0220AD4C
	.public ov96_0220B730
	.public ov96_0220B774
	.public ov96_0220B788
	.public ov96_0220C998
	.public ov96_0220D408
	.public ov96_0220D420
	.public ov96_0220E850
	.public ov96_0220E888
	.public ov96_0220E960
	.public ov96_0220F3B4
	.public ov96_0220FF64
	.public ov96_0221022C
	.public ov96_0221031C
	.public ov96_022104C4
	.public ov96_02210850
	.public ov96_02211650
	.public ov96_02213354
	.public ov96_02214904
	.public ov96_02214B74
	.public ov96_02214B7C
	.public ov96_02215864
	.public ov96_0221587C
	.public ov96_022158D4
	.public ov96_02215944
	.public ov96_02215DBC
	.public ov96_02215FA0
	.public ov96_02217890
	.public ov96_02217DBC
	.public ov96_022186B8
	.public ov96_02218A50
	.public ov96_022193E4
	.public ov96_0221978C
	.public ov96_02219FDC
	.public ov96_0221A670
	.public ov96_0221A680
	.public ov96_0221A690
	.public ov96_0221A69C
	.public ov96_0221A720

	.if 0
	thumb_func_start ov96_02204DF8
ov96_02204DF8: ; 0x02204DF8
	push {r4, r5, r6, r7, lr}
	sub sp, #0xc
	add r5, r0, #0
	bl PokeathlonCourse_GetGraphicsSystem
	add r6, r0, #0
	bl ov96_021E9510
	mov r4, #0
	add r7, r4, #0
_02204E0C:
	lsl r1, r4, #0x18
	add r0, r5, #0
	lsr r1, r1, #0x18
	bl PokeathlonCourse_GetField5F0_AtIndex
	add r3, r0, #0
	str r7, [sp]
	mov r0, #3
	str r0, [sp, #4]
	mov r0, #0x49
	lsl r0, r0, #2
	str r0, [sp, #8]
	add r0, r5, #0
	add r1, r6, #0
	add r2, r4, #0
	bl ov96_021E95F8
	add r4, r4, #1
	cmp r4, #4
	blt _02204E0C
	add r0, r6, #0
	mov r1, #1
	bl ov96_021E93B4
	add r0, r6, #0
	mov r1, #1
	bl ov96_0221A56C
	add r0, r6, #0
	bl ov96_021E952C
	add r0, r6, #0
	mov r1, #1
	bl ov96_021E9570
	mov r0, #1
	add sp, #0xc
	pop {r4, r5, r6, r7, pc}
	thumb_func_end ov96_02204DF8


	thumb_func_start ov96_02204E58
ov96_02204E58: ; 0x02204E58
	push {r3, r4, r5, lr}
	add r5, r0, #0
	bl PokeathlonCourse_GetHeapAllocPtr4
	add r4, r0, #0
	bl sub_0203A914
	mov r0, #0
	add r1, r0, #0
	bl Main_SetVBlankIntrCB
	mov r0, #0
	add r1, r0, #0
	bl Main_SetHBlankIntrCB
	add r0, r5, #0
	bl PokeathlonCourse_ResetField3A4
	ldr r0, [r4, #0x18]
	bl Heap_Free
	ldr r0, [r4]
	mov r1, #0
	bl FreeBgTilemapBuffer
	ldr r0, [r4]
	mov r1, #3
	bl FreeBgTilemapBuffer
	ldr r0, [r4]
	mov r1, #4
	bl FreeBgTilemapBuffer
	ldr r0, [r4]
	mov r1, #5
	bl FreeBgTilemapBuffer
	ldr r0, [r4]
	mov r1, #6
	bl FreeBgTilemapBuffer
	add r0, r4, #4
	bl RemoveWindow
	ldr r0, [r4]
	bl Heap_Free
	ldr r0, [r4, #0x20]
	bl ov96_021EB21C
	mov r0, #0xd2
	lsl r0, r0, #2
	ldr r0, [r4, r0]
	bl ov96_021EA894
	mov r0, #0xd1
	lsl r0, r0, #2
	ldr r0, [r4, r0]
	bl ov96_021E9C0C
	bl OamManager_Free
	bl ObjCharTransfer_Destroy
	bl ObjPlttTransfer_Destroy
	mov r0, #0xdb
	lsl r0, r0, #2
	ldr r0, [r4, r0]
	bl ov96_02208B2C
	mov r0, #0x37
	lsl r0, r0, #4
	ldr r0, [r4, r0]
	bl ov96_02207D64
	mov r0, #4
	bl FontID_Release
	add r0, r5, #0
	bl PokeathlonCourse_FreePtr4HeapAlloc
	ldr r0, _02204F18 ; =gSystem + 0x60
	mov r1, #0
	strb r1, [r0, #9]
	bl GfGfx_SwapDisplay
	ldr r0, _02204F1C ; =0x04000050
	mov r1, #0
	strh r1, [r0]
	mov r0, #0x8b
	bl Heap_Destroy
	mov r0, #1
	pop {r3, r4, r5, pc}
	nop
_02204F18: .word gSystem + 0x60
_02204F1C: .word 0x04000050
	thumb_func_end ov96_02204E58


	thumb_func_start ov96_02204F20
ov96_02204F20: ; 0x02204F20
	push {r4, lr}
	sub sp, #0x28
	ldr r4, _02204F3C ; =ov96_0221CB9C
	add r3, sp, #0
	mov r2, #5
_02204F2A:
	ldmia r4!, {r0, r1}
	stmia r3!, {r0, r1}
	sub r2, r2, #1
	bne _02204F2A
	add r0, sp, #0
	bl GfGfx_SetBanks
	add sp, #0x28
	pop {r4, pc}
	.balign 4, 0
_02204F3C: .word ov96_0221CB9C
	thumb_func_end ov96_02204F20
	.endif

	.if R86_GAP == 1

	thumb_func_start ov96_02204F40
ov96_02204F40: ; 0x02204F40
	push {r4, r5, lr}
	sub sp, #0x9c
	ldr r5, _02205030 ; =ov96_0221CAF0
	add r3, sp, #0x8c
	add r4, r0, #0
	add r2, r3, #0
	ldmia r5!, {r0, r1}
	stmia r3!, {r0, r1}
	ldmia r5!, {r0, r1}
	stmia r3!, {r0, r1}
	add r0, r2, #0
	bl SetBothScreensModesAndDisable
	ldr r5, _02205034 ; =ov96_0221CB64
	add r3, sp, #0x70
	ldmia r5!, {r0, r1}
	add r2, r3, #0
	stmia r3!, {r0, r1}
	ldmia r5!, {r0, r1}
	stmia r3!, {r0, r1}
	ldmia r5!, {r0, r1}
	stmia r3!, {r0, r1}
	ldr r0, [r5]
	mov r1, #0
	str r0, [r3]
	add r0, r4, #0
	add r3, r1, #0
	bl InitBgFromTemplate
	add r0, r4, #0
	mov r1, #0
	bl BgClearTilemapBufferAndCommit
	ldr r5, _02205038 ; =ov96_0221CB80
	add r3, sp, #0x54
	ldmia r5!, {r0, r1}
	add r2, r3, #0
	stmia r3!, {r0, r1}
	ldmia r5!, {r0, r1}
	stmia r3!, {r0, r1}
	ldmia r5!, {r0, r1}
	stmia r3!, {r0, r1}
	ldr r0, [r5]
	mov r1, #3
	str r0, [r3]
	add r0, r4, #0
	mov r3, #0
	bl InitBgFromTemplate
	add r0, r4, #0
	mov r1, #3
	bl BgClearTilemapBufferAndCommit
	ldr r5, _0220503C ; =ov96_0221CB10
	add r3, sp, #0x38
	ldmia r5!, {r0, r1}
	add r2, r3, #0
	stmia r3!, {r0, r1}
	ldmia r5!, {r0, r1}
	stmia r3!, {r0, r1}
	ldmia r5!, {r0, r1}
	stmia r3!, {r0, r1}
	ldr r0, [r5]
	mov r1, #4
	str r0, [r3]
	add r0, r4, #0
	mov r3, #0
	bl InitBgFromTemplate
	add r0, r4, #0
	mov r1, #4
	bl BgClearTilemapBufferAndCommit
	ldr r5, _02205040 ; =ov96_0221CB2C
	add r3, sp, #0x1c
	ldmia r5!, {r0, r1}
	add r2, r3, #0
	stmia r3!, {r0, r1}
	ldmia r5!, {r0, r1}
	stmia r3!, {r0, r1}
	ldmia r5!, {r0, r1}
	stmia r3!, {r0, r1}
	ldr r0, [r5]
	mov r1, #5
	str r0, [r3]
	add r0, r4, #0
	mov r3, #0
	bl InitBgFromTemplate
	add r0, r4, #0
	mov r1, #5
	bl BgClearTilemapBufferAndCommit
	ldr r5, _02205044 ; =ov96_0221CB48
	add r3, sp, #0
	ldmia r5!, {r0, r1}
	add r2, r3, #0
	stmia r3!, {r0, r1}
	ldmia r5!, {r0, r1}
	stmia r3!, {r0, r1}
	ldmia r5!, {r0, r1}
	stmia r3!, {r0, r1}
	ldr r0, [r5]
	mov r1, #6
	str r0, [r3]
	add r0, r4, #0
	mov r3, #0
	bl InitBgFromTemplate
	add r0, r4, #0
	mov r1, #6
	bl BgClearTilemapBufferAndCommit
	mov r0, #8
	mov r1, #0
	bl GfGfx_EngineATogglePlanes
	add sp, #0x9c
	pop {r4, r5, pc}
	nop
_02205030: .word ov96_0221CAF0
_02205034: .word ov96_0221CB64
_02205038: .word ov96_0221CB80
_0220503C: .word ov96_0221CB10
_02205040: .word ov96_0221CB2C
_02205044: .word ov96_0221CB48
	thumb_func_end ov96_02204F40


	thumb_func_start ov96_02205048
ov96_02205048: ; 0x02205048
	push {r4, lr}
	sub sp, #0x10
	mov r3, #0
	str r3, [sp]
	str r3, [sp, #4]
	add r4, r0, #0
	str r3, [sp, #8]
	ldr r0, [r4, #0x14]
	mov r1, #1
	str r0, [sp, #0xc]
	ldr r2, [r4]
	mov r0, #0xe2
	bl GfGfxLoader_LoadCharData
	mov r2, #0
	str r2, [sp]
	ldr r3, [r4, #0x14]
	mov r0, #0xe2
	mov r1, #3
	bl GfGfxLoader_LoadFromNarc
	add r1, r4, #0
	add r1, #0x1c
	str r0, [r4, #0x18]
	bl NNS_G2dGetUnpackedBGCharacterData
	mov r3, #0
	str r3, [sp]
	str r3, [sp, #4]
	str r3, [sp, #8]
	ldr r0, [r4, #0x14]
	mov r1, #2
	str r0, [sp, #0xc]
	ldr r2, [r4]
	mov r0, #0xe2
	bl GfGfxLoader_LoadScrnData
	mov r1, #0
	mov r0, #0x40
	str r0, [sp]
	ldr r0, [r4, #0x14]
	add r2, r1, #0
	str r0, [sp, #4]
	mov r0, #0xe2
	add r3, r1, #0
	bl GfGfxLoader_GXLoadPal
	mov r0, #0x37
	lsl r0, r0, #4
	ldr r0, [r4, r0]
	bl ov96_02207DDC
	add sp, #0x10
	pop {r4, pc}
	thumb_func_end ov96_02205048
	.endif


	.if 0
	thumb_func_start ov96_022050B4
ov96_022050B4: ; 0x022050B4
	push {r4, lr}
	sub sp, #8
	mov r1, #1
	str r1, [sp]
	mov r1, #0xe2
	mov r2, #0x11
	mov r3, #0x68
	add r4, r0, #0
	bl ov96_021EB2BC
	mov r0, #1
	str r0, [sp]
	mov r0, #2
	str r0, [sp, #4]
	add r0, r4, #0
	mov r1, #0xe2
	mov r2, #0xe
	mov r3, #0x68
	bl ov96_021EB2F4
	add r0, r4, #0
	mov r1, #0xe2
	mov r2, #0x10
	mov r3, #0x68
	bl ov96_021EB334
	add r0, r4, #0
	mov r1, #0xe2
	mov r2, #0xf
	mov r3, #0x68
	bl ov96_021EB36C
	add sp, #8
	pop {r4, pc}
	thumb_func_end ov96_022050B4
	.endif

	thumb_func_start ov96_022052B0
ov96_022052B0: ; 0x022052B0
	push {r4, r5, r6, r7, lr}
	sub sp, #0x84
	str r1, [sp, #0x10]
	str r0, [sp, #0xc]
	bl PokeathlonCourse_GetHeapAllocPtr4
	str r0, [sp, #0x18]
	ldr r0, [sp, #0x10]
	ldrb r0, [r0]
	cmp r0, #0
	beq _022052D4
	cmp r0, #1
	bne _022052CC
	b _02205430
_022052CC:
	cmp r0, #2
	bne _022052D2
	b _02205456
_022052D2:
	b _02205466
_022052D4:
	ldr r0, [sp, #0xc]
	bl PokeathlonCourse_GetDataCopyArea
	add r0, #0xf0
	bl ov96_021E8A20
	mov r3, #0xde
	ldr r2, [sp, #0x18]
	lsl r3, r3, #2
	ldr r2, [r2, r3]
	add r0, sp, #0x20
	mov r1, #0xe3
	bl ReadWholeNarcMemberByIdPair
	add r0, sp, #0x5c
	ldr r7, [sp, #0x18]
	str r0, [sp, #0x14]
	mov r0, #0
	add r6, sp, #0x20
	str r0, [sp, #0x1c]
	add r4, sp, #0x34
	add r5, r7, #0
_02205300:
	ldrb r0, [r4, #1]
	ldr r2, _02205474 ; =0x0000061C
	lsl r0, r0, #0x13
	lsr r1, r0, #0x10
	ldrb r0, [r4]
	lsl r0, r0, #0x13
	lsr r0, r0, #0x10
	strh r0, [r7, r2]
	ldrb r2, [r4, #1]
	lsl r3, r2, #3
	ldr r2, _02205478 ; =0x0000061E
	cmp r0, #0
	strh r3, [r7, r2]
	beq _0220533C
	cmp r1, #0
	beq _0220533C
	mov r2, #0xdf
	mov r3, #1
	lsl r2, r2, #2
	str r3, [r5, r2]
	add r2, #8
	strh r0, [r5, r2]
	ldr r0, _0220547C ; =0x00000386
	add r2, r3, #0
	strh r1, [r5, r0]
	sub r0, r0, #6
	ldr r0, [r5, r0]
	add r1, r3, #0
	bl ov96_021EB52C
_0220533C:
	ldr r0, [sp, #0x1c]
	add r4, r4, #2
	add r0, r0, #1
	add r7, r7, #4
	add r5, #0xc
	str r0, [sp, #0x1c]
	cmp r0, #0x14
	blt _02205300
	ldr r5, [sp, #0x18]
	mov r7, #0
	add r4, r5, #0
_02205352:
	ldrb r0, [r6, #1]
	ldr r2, _02205480 ; =0x0000066C
	lsl r0, r0, #0x13
	lsr r1, r0, #0x10
	ldrb r0, [r6]
	lsl r0, r0, #0x13
	lsr r0, r0, #0x10
	strh r0, [r5, r2]
	ldrb r2, [r6, #1]
	lsl r3, r2, #3
	ldr r2, _02205484 ; =0x0000066E
	cmp r0, #0
	strh r3, [r5, r2]
	beq _02205398
	cmp r1, #0
	beq _02205398
	ldr r2, _02205488 ; =0x0000046C
	mov r3, #1
	str r3, [r4, r2]
	add r2, #0xc
	strh r0, [r4, r2]
	ldr r0, _0220548C ; =0x0000047A
	add r2, r3, #0
	strh r1, [r4, r0]
	sub r0, #0xa
	ldr r0, [r4, r0]
	add r1, r3, #0
	bl ov96_021EB52C
	ldr r0, _02205490 ; =0x00000474
	mov r1, #1
	ldr r0, [r4, r0]
	add r2, r1, #0
	bl ov96_021EB52C
_02205398:
	add r7, r7, #1
	add r6, r6, #2
	add r5, r5, #4
	add r4, #0x10
	cmp r7, #0xa
	blt _02205352
	ldr r0, _02205494 ; =0x000005C8
	ldr r2, [sp, #0x18]
	mov r3, #0
	add r1, r0, #2
_022053AC:
	ldr r4, [sp, #0x14]
	add r3, r3, #1
	ldrb r4, [r4]
	lsl r4, r4, #3
	strh r4, [r2, r0]
	ldr r4, [sp, #0x14]
	ldrb r4, [r4, #1]
	lsl r4, r4, #3
	strh r4, [r2, r1]
	ldr r4, [sp, #0x14]
	add r2, r2, #4
	add r4, r4, #2
	str r4, [sp, #0x14]
	cmp r3, #0x14
	blt _022053AC
	ldr r0, [sp, #0xc]
	bl ov96_021E5F24
	cmp r0, #0
	bne _02205408
	ldr r0, [sp, #0x18]
	bl ov96_02207870
	mov r1, #0xdb
	ldr r0, [sp, #0x18]
	lsl r1, r1, #2
	ldr r0, [r0, r1]
	add r1, sp, #0x34
	bl ov96_02208AF8
	mov r1, #0xdb
	ldr r0, [sp, #0x18]
	lsl r1, r1, #2
	ldr r0, [r0, r1]
	add r1, sp, #0x20
	bl ov96_02208AF8
	mov r1, #0xdb
	ldr r0, [sp, #0x18]
	lsl r1, r1, #2
	ldr r0, [r0, r1]
	ldr r2, _02205498 ; =0x00000564
	ldr r1, [sp, #0x18]
	add r1, r1, r2
	bl ov96_02208AF0
_02205408:
	ldr r0, [sp, #0x18]
	bl ov96_0220764C
	mov r0, #0x5c
	bl GF_heap_c_dummy_return_true
	cmp r0, #0
	bne _0220541C
	bl GF_AssertFail
_0220541C:
	ldr r0, [sp, #0x10]
	ldrb r0, [r0]
	add r1, r0, #1
	ldr r0, [sp, #0x10]
	strb r1, [r0]
	ldr r0, [sp, #0xc]
	mov r1, #0x13
	bl PokeathlonCourse_SetStateTransitionType
	b _02205466
_02205430:
	mov r0, #6
	str r0, [sp]
	mov r0, #1
	str r0, [sp, #4]
	ldr r0, [sp, #0x18]
	mov r1, #3
	ldr r0, [r0, #0x14]
	add r2, r1, #0
	str r0, [sp, #8]
	mov r0, #2
	mov r3, #0
	bl BeginNormalPaletteFade
	ldr r0, [sp, #0x10]
	ldrb r0, [r0]
	add r1, r0, #1
	ldr r0, [sp, #0x10]
	strb r1, [r0]
	b _02205466
_02205456:
	bl IsPaletteFadeFinished
	cmp r0, #0
	beq _02205466
	ldr r0, [sp, #0xc]
	mov r1, #1
	bl PokeathlonCourse_SetStateField07
_02205466:
	ldr r0, [sp, #0xc]
	bl ov96_02205D30
	mov r0, #0
	add sp, #0x84
	pop {r4, r5, r6, r7, pc}
	nop
_02205474: .word 0x0000061C
_02205478: .word 0x0000061E
_0220547C: .word 0x00000386
_02205480: .word 0x0000066C
_02205484: .word 0x0000066E
_02205488: .word 0x0000046C
_0220548C: .word 0x0000047A
_02205490: .word 0x00000474
_02205494: .word 0x000005C8
_02205498: .word 0x00000564
	thumb_func_end ov96_022052B0
