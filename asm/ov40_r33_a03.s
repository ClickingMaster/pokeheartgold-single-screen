	.include "asm/macros.inc"
	.include "overlay_40.inc"
	.include "global.inc"
	.public BgClearTilemapBufferAndCommit
	.public GfGfxLoader_LoadCharDataFromOpenNarc
	.public GfGfxLoader_LoadScrnDataFromOpenNarc
	.public GfGfx_EngineATogglePlanes
	.public GfGfx_EngineBTogglePlanes
	.public Heap_Free
	.public ManagedSprite_SetAnim
	.public PaletteData_BlendPalettes
	.public PlaySE
	.public StopSE
	.public System_GetTouchNew
	.public TouchHitboxController_Destroy
	.public ov39_0222774C
	.public GlobalTerminalNet_GetError
	.public VsRecorder_SetScreenIfDone
	.public VsRecorder_SetSubState
	.public VsRecorder_SetTextSpriteMsg
	.public VsRecorder_RebuildMenuRowsParent
	.public VsRecorder_StepBlend
	.public VsRecorder_StepFade
	.public VsRecorder_ResetFade
	.public VsRecorder_ClearPaletteBlend
	.public VsRecorder_RemoveTopMessage
	.public VsRecorder_CreateProfile
	.public VsRecorder_FreeProfile
	.public VsRecorder_SetProfileDrawFlag
	.public VsRecorder_ClearListWindow
	.public VsRecorder_StartFlipScreens
	.public VsRecorder_IsFlipScreensDone
	.public ov40_0222FC14
	.public VsRecorder_LoadSavedVideos
	.public VsRecorder_FreeSavedVideos
	.public ov40_02230410
	.public VsRecorder_CreateVideoInfo
	.public VsRecorder_FreeVideoInfo
	.public VsRecorder_SetVideoPartyIconsDrawFlag
	.public ov40_022306E0
	.public VsRecorder_PlaceOrbitCursor
	.public VsRecorder_LoadScreen
	.public VsRecorder_SetVBlankPaused
	.public VsRecorder_PrintVideoListMessage
	.public VsRecorder_FreeVideoDetails
	.public VsRecorder_VideoListOverwriteNo
	.public VsRecorder_VideoListSave
	.public VsRecorder_VideoListConfirmOverwrite
	.public VsRecorder_VideoListReturnToDetails
	.public VsRecorder_FreeSlotButtons
	.public VsRecorder_RemoveDetailsButtonWindow
	.public VsRecorder_CreateDetailsButtonWindow
	.public VsRecorder_CreateOverwriteWindows
	.public VsRecorder_RemoveOverwriteWindows
	.public VsRecorder_GetNet
	.public VsRecorder_IsNetIdle
	.public BattleRecord_Delete
	.public BattleRecord_Exists
	.public BattleRecord_GetHeader
	.public BattleRecordHeader_GetField
	.public WirelessStatusIcon_ShowOnBothScreens
	.public OrbitCursor_SetVisible
	.public OrbitCursor_SetRadius
	.text

	thumb_func_start VsRecorder_VideoListOverwriteNo
VsRecorder_VideoListOverwriteNo: ; 0x0223A874
	push {r3, r4, r5, lr}
	mov r1, #0x86
	add r5, r0, #0
	lsl r1, r1, #4
	ldr r4, [r5, r1]
	ldr r1, [r5, #8]
	cmp r1, #0
	beq _0223A88E
	cmp r1, #1
	beq _0223A89E
	cmp r1, #2
	beq _0223A8CC
	b _0223A918
_0223A88E:
	mov r0, #4
	mov r1, #0
	bl GfGfx_EngineBTogglePlanes
	ldr r0, [r5, #8]
	add r0, r0, #1
	str r0, [r5, #8]
	b _0223A91E
_0223A89E:
	add r0, r4, #0
	add r1, r4, #4
	mov r2, #1
	mov r3, #2
	bl VsRecorder_StepBlend
	cmp r0, #0
	beq _0223A91E
	add r0, r5, #0
	bl VsRecorder_RemoveOverwriteWindows
	ldr r0, [r5, #0x24]
	mov r1, #6
	bl BgClearTilemapBufferAndCommit
	ldr r0, [r5, #0x24]
	mov r1, #7
	bl BgClearTilemapBufferAndCommit
	ldr r0, [r5, #8]
	add r0, r0, #1
	str r0, [r5, #8]
	b _0223A91E
_0223A8CC:
	add r0, r4, #0
	add r0, #8
	mov r1, #0
	bl VsRecorder_StepFade
	add r0, r4, #0
	add r1, r4, #4
	mov r2, #0
	mov r3, #2
	bl VsRecorder_StepBlend
	cmp r0, #0
	beq _0223A8FE
	add r0, r5, #0
	mov r1, #0x64
	mov r2, #0
	bl VsRecorder_PrintVideoListMessage
	mov r0, #4
	mov r1, #1
	bl GfGfx_EngineBTogglePlanes
	ldr r0, [r5, #8]
	add r0, r0, #1
	str r0, [r5, #8]
_0223A8FE:
	ldr r0, [r5, #0x58]
	mov r1, #3
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	str r0, [sp]
	ldr r3, [r4, #8]
	ldr r0, [r5, #0x28]
	lsl r3, r3, #0x18
	mov r2, #0xc
	lsr r3, r3, #0x18
	bl PaletteData_BlendPalettes
	b _0223A91E
_0223A918:
	mov r1, #0xc
	bl VsRecorder_SetSubState
_0223A91E:
	mov r0, #0
	pop {r3, r4, r5, pc}
	.balign 4, 0
	thumb_func_end VsRecorder_VideoListOverwriteNo


	thumb_func_start VsRecorder_VideoListSave
VsRecorder_VideoListSave: ; 0x0223A924
	push {r4, r5, r6, lr}
	sub sp, #8
	mov r1, #0x86
	add r4, r0, #0
	lsl r1, r1, #4
	ldr r5, [r4, r1]
	ldr r1, [r4, #8]
	cmp r1, #7
	bls _0223A938
	b _0223AB4A
_0223A938:
	add r1, r1, r1
	add r1, pc
	ldrh r1, [r1, #6]
	lsl r1, r1, #0x10
	asr r1, r1, #0x10
	add pc, r1
_0223A944: ; jump table
	.short _0223A954 - _0223A944 - 2 ; case 0
	.short _0223A964 - _0223A944 - 2 ; case 1
	.short _0223A9B6 - _0223A944 - 2 ; case 2
	.short _0223A9D4 - _0223A944 - 2 ; case 3
	.short _0223AA0A - _0223A944 - 2 ; case 4
	.short _0223AA4E - _0223A944 - 2 ; case 5
	.short _0223AA94 - _0223A944 - 2 ; case 6
	.short _0223AAF0 - _0223A944 - 2 ; case 7
_0223A954:
	mov r0, #4
	mov r1, #0
	bl GfGfx_EngineBTogglePlanes
	ldr r0, [r4, #8]
	add r0, r0, #1
	str r0, [r4, #8]
	b _0223ABFC
_0223A964:
	add r0, r5, #0
	add r0, #8
	mov r1, #1
	bl VsRecorder_StepFade
	add r0, r5, #0
	add r1, r5, #4
	mov r2, #1
	mov r3, #2
	bl VsRecorder_StepBlend
	cmp r0, #0
	beq _0223A99C
	ldr r0, [r4, #0x24]
	mov r1, #6
	bl BgClearTilemapBufferAndCommit
	ldr r1, _0223AC04 ; =0x00000116
	add r0, r4, #0
	mov r2, #0
	bl VsRecorder_PrintVideoListMessage
	add r0, r4, #0
	bl ov40_022306E0
	ldr r0, [r4, #8]
	add r0, r0, #1
	str r0, [r4, #8]
_0223A99C:
	ldr r0, [r4, #0x58]
	mov r1, #3
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	str r0, [sp]
	ldr r3, [r5, #8]
	ldr r0, [r4, #0x28]
	lsl r3, r3, #0x18
	mov r2, #0xc
	lsr r3, r3, #0x18
	bl PaletteData_BlendPalettes
	b _0223ABFC
_0223A9B6:
	ldr r2, _0223AC08 ; =0x000004D4
	ldr r1, _0223AC0C ; =0x00002028
	ldr r2, [r4, r2]
	ldr r1, [r5, r1]
	add r3, r4, r2
	ldr r2, _0223AC10 ; =0x0000413C
	ldrb r2, [r3, r2]
	bl ov40_0222FC14
	cmp r0, #0
	beq _0223AA9C
	ldr r0, [r4, #8]
	add r0, r0, #1
	str r0, [r4, #8]
	b _0223ABFC
_0223A9D4:
	mov r1, #0x6f
	lsl r1, r1, #4
	ldr r1, [r4, r1]
	mov r2, #0x80
	mov r3, #0x60
	bl VsRecorder_PlaceOrbitCursor
	mov r0, #0x6f
	lsl r0, r0, #4
	ldr r0, [r4, r0]
	mov r1, #1
	bl OrbitCursor_SetVisible
	mov r0, #0x6f
	lsl r0, r0, #4
	mov r1, #0x18
	ldr r0, [r4, r0]
	add r2, r1, #0
	bl OrbitCursor_SetRadius
	ldr r0, [r4, #8]
	add r0, r0, #1
	str r0, [r4, #8]
	ldr r0, _0223AC14 ; =0x0000057D
	bl PlaySE
	b _0223ABFC
_0223AA0A:
	bl VsRecorder_IsNetIdle
	cmp r0, #0
	bne _0223AA18
	add sp, #8
	mov r0, #0
	pop {r4, r5, r6, pc}
_0223AA18:
	mov r1, #0x46
	add r0, r4, #0
	lsl r1, r1, #2
	mov r2, #0
	bl VsRecorder_PrintVideoListMessage
	bl BattleRecord_GetHeader
	mov r1, #4
	mov r2, #0
	bl BattleRecordHeader_GetField
	add r6, r0, #0
	add r5, r1, #0
	add r0, r4, #0
	bl VsRecorder_GetNet
	add r1, r6, #0
	add r2, r5, #0
	bl ov39_0222774C
	cmp r0, #1
	bne _0223AA9C
	ldr r0, [r4, #8]
	add r0, r0, #1
	str r0, [r4, #8]
	b _0223ABFC
_0223AA4E:
	bl VsRecorder_IsNetIdle
	cmp r0, #0
	bne _0223AA5C
	add sp, #8
	mov r0, #0
	pop {r4, r5, r6, pc}
_0223AA5C:
	add r0, r4, #0
	bl VsRecorder_GetNet
	add r1, sp, #4
	bl GlobalTerminalNet_GetError
	cmp r0, #1
	ldr r0, _0223AC14 ; =0x0000057D
	bne _0223AA76
	mov r1, #0
	bl StopSE
	b _0223AA82
_0223AA76:
	mov r1, #0
	bl StopSE
	ldr r0, _0223AC18 ; =0x00000577
	bl PlaySE
_0223AA82:
	ldr r0, [r4, #8]
	ldr r1, _0223AC1C ; =0x00000119
	add r0, r0, #1
	str r0, [r4, #8]
	add r0, r4, #0
	mov r2, #0
	bl VsRecorder_PrintVideoListMessage
	b _0223ABFC
_0223AA94:
	bl System_GetTouchNew
	cmp r0, #0
	bne _0223AA9E
_0223AA9C:
	b _0223ABFC
_0223AA9E:
	add r0, r4, #0
	bl VsRecorder_RemoveTopMessage
	add r0, r4, #0
	mov r1, #1
	bl VsRecorder_SetVBlankPaused
	add r0, r4, #0
	bl VsRecorder_FreeSlotButtons
	add r0, r4, #0
	mov r1, #0
	bl VsRecorder_SetVBlankPaused
	mov r0, #0x6f
	lsl r0, r0, #4
	ldr r0, [r4, r0]
	mov r1, #0
	bl OrbitCursor_SetVisible
	mov r0, #0x6f
	lsl r0, r0, #4
	mov r1, #0
	ldr r0, [r4, r0]
	add r2, r1, #0
	bl OrbitCursor_SetRadius
	add r0, r4, #0
	bl VsRecorder_FreeSavedVideos
	add r0, r4, #0
	bl VsRecorder_LoadSavedVideos
	ldr r0, _0223AC20 ; =0x0000049C
	add r0, r4, r0
	bl VsRecorder_ClearListWindow
	ldr r0, [r4, #8]
	add r0, r0, #1
	str r0, [r4, #8]
	b _0223ABFC
_0223AAF0:
	mov r0, #0x62
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	bl TouchHitboxController_Destroy
	mov r0, #0x63
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	bl TouchHitboxController_Destroy
	mov r0, #0x19
	lsl r0, r0, #4
	ldr r0, [r5, r0]
	bl TouchHitboxController_Destroy
	add r0, r4, #0
	bl VsRecorder_FreeVideoDetails
	add r5, #8
	add r0, r5, #0
	bl VsRecorder_ResetFade
	add r0, r4, #0
	mov r1, #1
	bl VsRecorder_SetVBlankPaused
	add r0, r4, #0
	bl VsRecorder_RebuildMenuRowsParent
	add r0, r4, #0
	mov r1, #0
	bl VsRecorder_SetVBlankPaused
	mov r0, #1
	mov r1, #0x6d
	bl WirelessStatusIcon_ShowOnBothScreens
	add r0, r4, #0
	mov r1, #1
	bl VsRecorder_StartFlipScreens
	ldr r0, [r4, #8]
	add r0, r0, #1
	str r0, [r4, #8]
	b _0223ABFC
_0223AB4A:
	add r0, r4, #0
	bl VsRecorder_IsFlipScreensDone
	cmp r0, #0
	beq _0223ABFC
	add r0, r5, #0
	add r0, #8
	mov r1, #0
	bl VsRecorder_StepFade
	cmp r0, #0
	beq _0223ABCC
	add r0, r4, #0
	bl VsRecorder_ClearPaletteBlend
	add r0, r5, #0
	add r0, #8
	bl VsRecorder_ResetFade
	ldr r0, [r4, #0x58]
	mov r1, #2
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	str r0, [sp]
	ldr r0, [r4, #0x28]
	mov r2, #0xc
	mov r3, #0x10
	bl PaletteData_BlendPalettes
	mov r1, #1
	ldr r3, [r4, #0x10]
	add r0, r4, #0
	add r2, r1, #0
	bl VsRecorder_SetScreenIfDone
	add r0, r4, #0
	mov r1, #5
	bl VsRecorder_SetSubState
	ldr r0, [r4, #0x24]
	mov r1, #2
	bl BgClearTilemapBufferAndCommit
	ldr r0, [r4, #0x24]
	mov r1, #6
	bl BgClearTilemapBufferAndCommit
	ldr r0, [r4, #0x24]
	mov r1, #3
	bl BgClearTilemapBufferAndCommit
	ldr r0, [r4, #0x24]
	mov r1, #7
	bl BgClearTilemapBufferAndCommit
	add r0, r5, #0
	bl Heap_Free
	bl BattleRecord_Exists
	cmp r0, #1
	bne _0223ABFC
	bl BattleRecord_Delete
	b _0223ABFC
_0223ABCC:
	ldr r0, [r4, #0x58]
	mov r1, #1
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	str r0, [sp]
	ldr r3, [r5, #8]
	ldr r0, [r4, #0x28]
	lsl r3, r3, #0x18
	mov r2, #2
	lsr r3, r3, #0x18
	bl PaletteData_BlendPalettes
	ldr r0, [r4, #0x58]
	mov r1, #3
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	str r0, [sp]
	ldr r3, [r5, #8]
	ldr r0, [r4, #0x28]
	lsl r3, r3, #0x18
	mov r2, #0xc
	lsr r3, r3, #0x18
	bl PaletteData_BlendPalettes
_0223ABFC:
	mov r0, #0
	add sp, #8
	pop {r4, r5, r6, pc}
	nop
_0223AC04: .word 0x00000116
_0223AC08: .word 0x000004D4
_0223AC0C: .word 0x00002028
_0223AC10: .word 0x0000413C
_0223AC14: .word 0x0000057D
_0223AC18: .word 0x00000577
_0223AC1C: .word 0x00000119
_0223AC20: .word 0x0000049C
	thumb_func_end VsRecorder_VideoListSave


	thumb_func_start VsRecorder_VideoListConfirmOverwrite
VsRecorder_VideoListConfirmOverwrite: ; 0x0223AC24
	push {r3, r4, r5, lr}
	sub sp, #0x10
	mov r1, #0x86
	add r5, r0, #0
	lsl r1, r1, #4
	ldr r4, [r5, r1]
	ldr r1, [r5, #8]
	cmp r1, #0
	beq _0223AC3C
	cmp r1, #1
	beq _0223AC9A
	b _0223ACC0
_0223AC3C:
	add r0, r4, #0
	add r0, #8
	mov r1, #1
	bl VsRecorder_StepFade
	add r0, r4, #0
	add r1, r4, #4
	mov r2, #1
	mov r3, #2
	bl VsRecorder_StepBlend
	cmp r0, #0
	beq _0223AC80
	mov r0, #0
	str r0, [sp]
	str r0, [sp, #4]
	str r0, [sp, #8]
	mov r0, #0x6d
	str r0, [sp, #0xc]
	ldr r0, [r5, #0x14]
	ldr r2, [r5, #0x24]
	mov r1, #0x54
	mov r3, #7
	bl GfGfxLoader_LoadScrnDataFromOpenNarc
	ldr r2, _0223ACCC ; =0x00002028
	add r0, r5, #0
	ldr r2, [r4, r2]
	mov r1, #0x66
	bl VsRecorder_PrintVideoListMessage
	ldr r0, [r5, #8]
	add r0, r0, #1
	str r0, [r5, #8]
_0223AC80:
	ldr r0, [r5, #0x58]
	mov r1, #3
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	str r0, [sp]
	ldr r3, [r4, #8]
	ldr r0, [r5, #0x28]
	lsl r3, r3, #0x18
	mov r2, #0xc
	lsr r3, r3, #0x18
	bl PaletteData_BlendPalettes
	b _0223ACC6
_0223AC9A:
	add r0, r4, #0
	add r1, r4, #4
	mov r2, #0
	mov r3, #2
	bl VsRecorder_StepBlend
	cmp r0, #0
	beq _0223ACC6
	add r0, r5, #0
	bl VsRecorder_CreateOverwriteWindows
	mov r0, #4
	mov r1, #1
	bl GfGfx_EngineBTogglePlanes
	ldr r0, [r5, #8]
	add r0, r0, #1
	str r0, [r5, #8]
	b _0223ACC6
_0223ACC0:
	mov r1, #0xf
	bl VsRecorder_SetSubState
_0223ACC6:
	mov r0, #0
	add sp, #0x10
	pop {r3, r4, r5, pc}
	.balign 4, 0
_0223ACCC: .word 0x00002028
	thumb_func_end VsRecorder_VideoListConfirmOverwrite


	thumb_func_start VsRecorder_VideoListReturnToDetails
VsRecorder_VideoListReturnToDetails: ; 0x0223ACD0
	push {r4, r5, r6, lr}
	sub sp, #0x10
	add r4, r0, #0
	mov r0, #0x86
	lsl r0, r0, #4
	ldr r1, [r4, #8]
	ldr r5, [r4, r0]
	cmp r1, #3
	bls _0223ACE4
	b _0223AEEC
_0223ACE4:
	add r1, r1, r1
	add r1, pc
	ldrh r1, [r1, #6]
	lsl r1, r1, #0x10
	asr r1, r1, #0x10
	add pc, r1
_0223ACF0: ; jump table
	.short _0223ACF8 - _0223ACF0 - 2 ; case 0
	.short _0223AD20 - _0223ACF0 - 2 ; case 1
	.short _0223AD5A - _0223ACF0 - 2 ; case 2
	.short _0223AE82 - _0223ACF0 - 2 ; case 3
_0223ACF8:
	ldr r0, [r4, #0x24]
	mov r1, #2
	bl BgClearTilemapBufferAndCommit
	ldr r0, [r4, #0x24]
	mov r1, #6
	bl BgClearTilemapBufferAndCommit
	mov r0, #4
	mov r1, #0
	bl GfGfx_EngineBTogglePlanes
	mov r0, #4
	mov r1, #0
	bl GfGfx_EngineATogglePlanes
	ldr r0, [r4, #8]
	add r0, r0, #1
	str r0, [r4, #8]
	b _0223AF18
_0223AD20:
	add r0, r5, #0
	add r0, #8
	mov r1, #1
	bl VsRecorder_StepFade
	add r0, r5, #0
	add r1, r5, #4
	mov r2, #1
	mov r3, #0
	bl VsRecorder_StepBlend
	cmp r0, #0
	beq _0223AD40
	ldr r0, [r4, #8]
	add r0, r0, #1
	str r0, [r4, #8]
_0223AD40:
	ldr r0, [r4, #0x58]
	mov r1, #3
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	str r0, [sp]
	ldr r3, [r5, #8]
	ldr r0, [r4, #0x28]
	lsl r3, r3, #0x18
	mov r2, #0xc
	lsr r3, r3, #0x18
	bl PaletteData_BlendPalettes
	b _0223AF18
_0223AD5A:
	add r0, #0xc
	ldr r0, [r4, r0]
	cmp r0, #0xd2
	bne _0223AD9A
	mov r0, #0x45
	lsl r0, r0, #2
	add r1, r4, #0
	add r0, r5, r0
	add r1, #0x14
	mov r2, #3
	bl VsRecorder_SetTextSpriteMsg
	mov r0, #0x13
	lsl r0, r0, #4
	add r1, r4, #0
	add r0, r5, r0
	add r1, #0x14
	mov r2, #0x5e
	bl VsRecorder_SetTextSpriteMsg
	mov r0, #0x46
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	mov r1, #0
	bl ManagedSprite_SetAnim
	mov r0, #0x4d
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	mov r1, #3
	bl ManagedSprite_SetAnim
_0223AD9A:
	add r0, r4, #0
	bl VsRecorder_RemoveDetailsButtonWindow
	add r0, r4, #0
	bl VsRecorder_CreateDetailsButtonWindow
	mov r0, #0
	str r0, [sp]
	str r0, [sp, #4]
	str r0, [sp, #8]
	mov r0, #0x6d
	str r0, [sp, #0xc]
	ldr r0, [r4, #0x14]
	ldr r2, [r4, #0x24]
	mov r1, #0x3e
	mov r3, #3
	bl GfGfxLoader_LoadCharDataFromOpenNarc
	mov r0, #0
	str r0, [sp]
	str r0, [sp, #4]
	str r0, [sp, #8]
	mov r0, #0x6d
	str r0, [sp, #0xc]
	ldr r0, [r4, #0x14]
	ldr r2, [r4, #0x24]
	mov r1, #0x3e
	mov r3, #7
	bl GfGfxLoader_LoadCharDataFromOpenNarc
	add r0, r4, #0
	mov r1, #1
	bl VsRecorder_SetVBlankPaused
	ldr r0, [r5, #0x10]
	cmp r0, #0
	bne _0223AE1E
	mov r0, #0x22
	lsl r0, r0, #4
	add r0, r5, r0
	add r1, r4, #0
	bl VsRecorder_FreeVideoInfo
	mov r0, #0x22
	lsl r0, r0, #4
	add r0, r5, r0
	add r1, r4, #0
	bl VsRecorder_CreateVideoInfo
	mov r0, #0x22
	lsl r0, r0, #4
	add r0, r5, r0
	mov r1, #0
	bl VsRecorder_SetVideoPartyIconsDrawFlag
	mov r0, #0x22
	lsl r0, r0, #4
	add r0, r5, r0
	bl ov40_02230410
	add r1, r0, #0
	add r0, r4, #0
	mov r2, #3
	bl VsRecorder_LoadScreen
	b _0223AE5A
_0223AE1E:
	mov r0, #0x65
	lsl r0, r0, #2
	add r0, r5, r0
	add r1, r4, #0
	bl VsRecorder_FreeProfile
	mov r0, #0x65
	lsl r0, r0, #2
	add r0, r5, r0
	add r1, r4, #0
	bl VsRecorder_CreateProfile
	mov r0, #0x65
	lsl r0, r0, #2
	add r0, r5, r0
	mov r1, #0
	bl VsRecorder_SetProfileDrawFlag
	mov r0, #0
	str r0, [sp]
	str r0, [sp, #4]
	str r0, [sp, #8]
	mov r0, #0x6d
	str r0, [sp, #0xc]
	ldr r0, [r4, #0x14]
	ldr r2, [r4, #0x24]
	mov r1, #0x50
	mov r3, #3
	bl GfGfxLoader_LoadScrnDataFromOpenNarc
_0223AE5A:
	add r0, r4, #0
	mov r1, #0
	bl VsRecorder_SetVBlankPaused
	mov r0, #0
	str r0, [sp]
	str r0, [sp, #4]
	str r0, [sp, #8]
	mov r0, #0x6d
	str r0, [sp, #0xc]
	ldr r0, [r4, #0x14]
	ldr r2, [r4, #0x24]
	mov r1, #6
	mov r3, #7
	bl GfGfxLoader_LoadScrnDataFromOpenNarc
	ldr r0, [r4, #8]
	add r0, r0, #1
	str r0, [r4, #8]
	b _0223AF18
_0223AE82:
	add r0, r5, #0
	add r0, #8
	mov r1, #0
	bl VsRecorder_StepFade
	mov r2, #0
	add r0, r5, #0
	add r1, r5, #4
	add r3, r2, #0
	bl VsRecorder_StepBlend
	cmp r0, #0
	beq _0223AED2
	ldr r0, [r5, #0x10]
	cmp r0, #0
	bne _0223AEB0
	mov r0, #0x22
	lsl r0, r0, #4
	add r0, r5, r0
	mov r1, #1
	bl VsRecorder_SetVideoPartyIconsDrawFlag
	b _0223AEBC
_0223AEB0:
	mov r0, #0x65
	lsl r0, r0, #2
	add r0, r5, r0
	mov r1, #1
	bl VsRecorder_SetProfileDrawFlag
_0223AEBC:
	mov r0, #4
	mov r1, #1
	bl GfGfx_EngineBTogglePlanes
	mov r0, #4
	mov r1, #1
	bl GfGfx_EngineATogglePlanes
	ldr r0, [r4, #8]
	add r0, r0, #1
	str r0, [r4, #8]
_0223AED2:
	ldr r0, [r4, #0x58]
	mov r1, #3
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	str r0, [sp]
	ldr r3, [r5, #8]
	ldr r0, [r4, #0x28]
	lsl r3, r3, #0x18
	mov r2, #0xc
	lsr r3, r3, #0x18
	bl PaletteData_BlendPalettes
	b _0223AF18
_0223AEEC:
	ldr r1, _0223AF20 ; =0x00004138
	mov r0, #0
	ldr r1, [r4, r1]
	cmp r1, #0
	ble _0223AF10
	mov r1, #0x9a
	lsl r1, r1, #6
	add r5, r4, r1
	ldr r2, _0223AF20 ; =0x00004138
	add r6, r4, #0
	sub r1, #0x78
_0223AF02:
	str r5, [r6, r1]
	ldr r3, [r4, r2]
	add r0, r0, #1
	add r5, #0xe4
	add r6, r6, #4
	cmp r0, r3
	blt _0223AF02
_0223AF10:
	add r0, r4, #0
	mov r1, #7
	bl VsRecorder_SetSubState
_0223AF18:
	mov r0, #0
	add sp, #0x10
	pop {r4, r5, r6, pc}
	nop
_0223AF20: .word 0x00004138
	thumb_func_end VsRecorder_VideoListReturnToDetails
