#include "config.h"
#include "msgdata/msg/msg_0247.h"
#include "msgdata/msg/msg_0442.h"
#include "constants/sndseq.h"
	.include "asm/macros.inc"
	.include "overlay_74_thumb.inc"
	.include "global.inc"
	.public gApplication_TitleScreen
	.public sub_020342C0

	.public MainMenuApp_Init
	.public MainMenuApp_Main
	.public MainMenuApp_Exit
	.public ov74_0223B2C0
	.public ov74_0223B2CC
	.public ov74_0223B2D8
	.public ov74_0223B2E8
	.public gApp_MainMenu_SelectOption_ConnectToRanger
	.public gApp_MainMenu_SelectOption_MigrateFromAgb
	.public gApp_MainMenu_SelectOption_MysteryGift

	.text
	.public _0223B330
	.public _0223B340
	.public _0223B368
	.public _0223B390
	.public _0223B3A0
	.public _0223B3BC
	.public _0223B410
	.public _0223B420
	.public _0223B430
	.public _0223B440
	.public _0223B45C
	.public _0223B478
	.public _0223B494
	.public _0223B4B0
	.public ov74_022312C0
	.public ov74_022313F0
	.public ov74_02231424
	.public ov74_02231448
	.public ov74_0223144C
	.public ov74_02231450
	.public ov74_02231454
	.public ov74_02231458
	.public ov74_0223145C
	.public ov74_02231460
	.public ov74_022314A0
	.public ov74_022314BC
	.public ov74_022314DC
	.public ov74_02231508
	.public ov74_02231544
	.public ov74_02231560
	.public ov74_02231584
	.public ov74_0223161C
	.public ov74_02231638
	.public ov74_02231670
	.public ov74_022316E8
	.public ov74_02231704
	.public ov74_02231724
	.public ov74_02231744
	.public ov74_0223512C
	.public ov74_02235230
	.public ov74_02235258
	.public ov74_022352A0
	.public ov74_022352D0
	.public ov74_02235308
	.public ov74_02235390
	.public ov74_0223539C
	.public ov74_022353FC
	.public ov74_02235414
	.public ov74_02235464
	.public ov74_0223546C
	.public ov74_02235474
	.public ov74_02235568
	.public ov74_02235634
	.public ov74_0223563C
	.public ov74_0223567C
	.public ov74_02235690
	.public ov74_02235708
	.public ov74_02235728
	.public ov74_022358BC
	.public ov74_022358C8
	.public ov74_02235930
	.public ov74_022359BC
	.public ov74_02235A68
	.public ov74_02235DC4
	.public ov74_02235ED0
	.public ov74_02235F58
	.public ov74_02236034
	.public ov74_022360A0
	.public ov74_022360B0
	.public ov74_02236128
	.public ov74_02236140
	.public ov74_0223615C
	.public ov74_022365FC
	.public ov74_02236680
	.public ov74_022368D4
	.public ov74_02236980
	.public ov74_02236988
	.public ov74_02236AE0
	.public ov74_0223BD5C
	.public ov74_0223BD68
	.public ov74_0223C1F4
	.public ov74_0223C220
	.public ov74_0223C230
	.public ov74_0223C240
	.public ov74_0223C250
	.public ov74_0223C260
	.public ov74_0223C270
	.public ov74_0223C2A0
	.public ov74_0223C2B0
	.public ov74_0223C2D0
	.public ov74_0223C2E0
	.public ov74_0223C2F0
	.public ov74_0223C320
	.public ov74_0223C340
	.public ov74_0223C6D0
	.public ov74_0223C6E0
	.public ov74_0223C700
	.public ov74_0223D080
	.public ov74_0223D090
	.public ov74_0223D0A4
	.public ov74_0223D0A8
	.public ov74_0223D0BC
	.public ov74_0223D0C0
	.extern ov74_02229200
	.extern ov74_02229294
	.extern ov74_02229450
	.extern ov74_02229DA0
	.extern ov74_02229DCC
	.extern ov74_02229DD4
	.extern ov74_02229DE0
	.extern ov74_02229E60
	.extern ov74_02229F90
	.extern ov74_02229FF4
	.extern ov74_0222A2A4
	.extern ov74_0222A2E0
	.extern ov74_0222A35C
	.extern ov74_0222A3CC
	.extern ov74_0222A494
	.extern ov74_0222A4CC
	.extern ov74_0222A504
	.extern ov74_0222A538
	.extern ov74_0222A560
	.extern ov74_0222A5AC
	.extern ov74_0222A5CC
	.extern ov74_0222A650
	.extern ov74_0222A688
	.extern ov74_0222A6C0
	.extern ov74_0222ACD8
	.extern ov74_0222AE6C
	.extern ov74_0222C2EC
	.extern ov74_0222CD94
	.extern ov74_0222D2D4
	.extern ov74_0222D308
	.extern ov74_0222D358
	.extern ov74_0222D414
	.extern ov74_0222D490
	.extern ov74_0222D494
	.extern ov74_0222D4E4
	.extern ov74_0222D55C
	.extern ov74_0222D5C4
	.extern ov74_0222D614
	.extern ov74_0222D690
	.extern ov74_0222D774
	.extern ov74_0222D78C
	.extern ov74_0222D7D0
	.extern ov74_0222DDFC
	.extern ov74_0222E1F4
	.extern ov74_0222EB44
	.extern ov74_0222EE08
	.extern ov74_0222EE0C
	.extern ov74_0222EE58
	.extern ov74_0222EEB0
	.extern ov74_0222EF18
	.extern ov74_0222EF64
	.extern ov74_0222EF68
	.extern ov74_0222F598
	.extern ov74_0222F7D4
	.extern ov74_0222FB8C
	.extern ov74_02230018
	.extern ov74_02230030
	.extern ov74_02230070
	.extern ov74_022300A8
	.extern ov74_02230110
	.extern ov74_02230138
	.extern ov74_02230404
	.extern ov74_02230520
	.extern ov74_02230590
	.extern ov74_022305E4
	.extern ov74_022306C8
	.extern ov74_022308E0
	.extern ov74_02230964
	.extern ov74_02230988
	.extern ov74_022309AC
	.extern ov74_022309F0
	.extern ov74_02231054
	.extern ov74_022310AC
	.extern ov74_022310C4
	.extern ov74_022310D0
	.extern ov74_022310DC
	.extern ov74_022310E8
	.extern ov74_022310F4
	.extern ov74_02231100
	.extern ov74_02231154
	.extern ov74_0223115C
	.extern ov74_022311D8
	.extern ov74_022311DC
	.extern ov74_022311E8
	.extern ov74_02231260
	.public AGB_GetBoxMonAbility
	.public AGB_GetBoxMonData
	.public ConvertRSStringToDPStringInternational
	.public IdentifyPmAgbCartridge
	.public MigrateBoxMon
	.public PmAgbCartridgeGetOffsets
	.public PmAgbCartridgeHasFlash
	.public PmAgbCartridgeUnlinkSpec
	.public PmAgbCartridge_GetLanguage
	.public PmAgbCartridge_GetVersionInternal
	.public TranslateAgbSpecies
	.public _0223B330
	.public _0223B340
	.public _0223B368
	.public _0223B390
	.public _0223B3A0
	.public _0223B3BC
	.public _0223B410
	.public _0223B420
	.public _0223B430
	.public _0223B440
	.public _0223B45C
	.public _0223B478
	.public _0223B494
	.public _0223B4B0
	.public _0223B4E8
	.public _0223B4F8
	.public _0223B508
	.public _0223B524
	.public _0223B540
	.public _0223B55C
	.public _0223B578
	.public _0223B690
	.public _0223B710
	.public _0223B720
	.public _0223B73C
	.public _0223B760
	.public gApp_MainMenu
	.public gApp_MainMenu_SelectOption_ConnectToRanger
	.public gApp_MainMenu_SelectOption_MigrateFromAgb
	.public gApp_MainMenu_SelectOption_MysteryGift
	.public ov74_02231264
	.public ov74_0223127C
	.public ov74_022312C0
	.public ov74_022313F0
	.public ov74_02231424
	.public ov74_02231448
	.public ov74_0223144C
	.public ov74_02231450
	.public ov74_02231454
	.public ov74_02231458
	.public ov74_0223145C
	.public ov74_02231460
	.public ov74_022314A0
	.public ov74_022314BC
	.public ov74_022314DC
	.public ov74_02231508
	.public ov74_02231544
	.public ov74_02231560
	.public ov74_02231584
	.public ov74_0223161C
	.public ov74_02231638
	.public ov74_02231670
	.public ov74_022316E8
	.public ov74_02231704
	.public ov74_02231724
	.public ov74_02231744
	.public ov74_02231764
	.public ov74_02231790
	.public ov74_022317D8
	.public ov74_02231930
	.public ov74_0223195C
	.public ov74_022319F8
	.public ov74_02231A1C
	.public ov74_02231BC0
	.public ov74_02231BF0
	.public ov74_02231CFC
	.public ov74_02231D48
	.public ov74_02231D70
	.public ov74_02231D94
	.public ov74_02231DB8
	.public ov74_02231DDC
	.public ov74_02231E00
	.public ov74_02231E38
	.public ov74_02231E54
	.public ov74_02231EC4
	.public ov74_02231F30
	.public ov74_02231FB0
	.public ov74_02231FF4
	.public ov74_02232154
	.public ov74_02232284
	.public ov74_022322D8
	.public ov74_02232398
	.public ov74_022323D0
	.public ov74_02232424
	.public ov74_02232474
	.public ov74_022324A0
	.public ov74_0223262C
	.public ov74_02232678
	.public ov74_02232700
	.public ov74_02232758
	.public ov74_02232910
	.public ov74_02232940
	.public ov74_02232974
	.public ov74_02232A48
	.public ov74_02232AC8
	.public ov74_02232B18
	.public ov74_02232BD4
	.public ov74_02232DA4
	.public ov74_02232DC4
	.public ov74_02232E3C
	.public ov74_02232F5C
	.public ov74_02232F9C
	.public ov74_02233024
	.public ov74_02233060
	.public ov74_022330D0
	.public ov74_02233134
	.public ov74_0223319C
	.public ov74_02233230
	.public ov74_022332F4
	.public ov74_022338D4
	.public ov74_02233A88
	.public ov74_02233AB8
	.public ov74_02233ACC
	.public ov74_02233B04
	.public ov74_02233B58
	.public ov74_02233C08
	.public ov74_02233CE4
	.public ov74_02233DBC
	.public ov74_02233E50
	.public ov74_02233E8C
	.public ov74_02233ED4
	.public ov74_02233F14
	.public ov74_02233F4C
	.public ov74_02233F5C
	.public ov74_02233F68
	.public ov74_02233F84
	.public ov74_02233F8C
	.public ov74_02234468
	.public ov74_02234488
	.public ov74_022344A8
	.public ov74_022348B0
	.public ov74_02234A0C
	.public ov74_0223512C
	.public ov74_02235138
	.public ov74_0223514C
	.public ov74_022351FC
	.public ov74_02235230
	.public ov74_02235258
	.public ov74_022352A0
	.public ov74_022352D0
	.public ov74_02235308
	.public ov74_02235390
	.public ov74_0223539C
	.public ov74_022353FC
	.public ov74_02235414
	.public ov74_02235464
	.public ov74_0223546C
	.public ov74_02235474
	.public ov74_0223547C
	.public ov74_02235568
	.public ov74_02235634
	.public ov74_0223563C
	.public ov74_0223567C
	.public ov74_02235690
	.public ov74_02235708
	.public ov74_02235728
	.public ov74_0223589C
	.public ov74_022358BC
	.public ov74_022358C8
	.public ov74_02235930
	.public ov74_022359BC
	.public ov74_02235A68
	.public ov74_02235A74
	.public ov74_02235AC4
	.public ov74_02235B14
	.public ov74_02235BD0
	.public ov74_02235C10
	.public ov74_02235CE4
	.public ov74_02235DA4
	.public ov74_02235DC4
	.public ov74_02235ED0
	.public ov74_02235F58
	.public ov74_02235FF8
	.public ov74_02236010
	.public ov74_02236034
	.public ov74_02236070
	.public ov74_02236074
	.public ov74_022360A0
	.public ov74_022360B0
	.public ov74_02236128
	.public ov74_02236140
	.public ov74_0223615C
	.public ov74_02236168
	.public ov74_022361B8
	.public ov74_022361D4
	.public ov74_02236258
	.public ov74_02236354
	.public ov74_0223648C
	.public ov74_022364A8
	.public ov74_022364C0
	.public ov74_022364F0
	.public ov74_02236514
	.public ov74_0223653C
	.public ov74_02236570
	.public ov74_022365AC
	.public ov74_022365FC
	.public ov74_02236620
	.public ov74_02236644
	.public ov74_02236680
	.public ov74_022366E8
	.public ov74_02236768
	.public ov74_022368A4
	.public ov74_022368D4
	.public ov74_02236980
	.public ov74_02236988
	.public ov74_022369A8
	.public ov74_022369C8
	.public ov74_022369D8
	.public ov74_02236A2C
	.public ov74_02236A54
	.public ov74_02236A78
	.public ov74_02236AAC
	.public ov74_02236ABC
	.public ov74_02236AC8
	.public ov74_02236AE0
	.public ov74_02236F30
	.public ov74_02236F44
	.public ov74_02236F80
	.public ov74_0223BD5C
	.public ov74_0223BD68
	.public ov74_0223C1F4
	.public ov74_0223C220
	.public ov74_0223C230
	.public ov74_0223C240
	.public ov74_0223C250
	.public ov74_0223C260
	.public ov74_0223C270
	.public ov74_0223C2A0
	.public ov74_0223C2B0
	.public ov74_0223C2D0
	.public ov74_0223C2E0
	.public ov74_0223C2F0
	.public ov74_0223C320
	.public ov74_0223C340
	.public ov74_0223C6D0
	.public ov74_0223C6E0
	.public ov74_0223C700
	.public ov74_0223C920
	.public ov74_0223C960
	.public ov74_0223C968
	.public ov74_0223C980
	.public ov74_0223C998
	.public ov74_0223C9A8
	.public ov74_0223C9B8
	.public ov74_0223C9D8
	.public ov74_0223CBA0
	.public ov74_0223CC5C
	.public ov74_0223CC5E
	.public ov74_0223CE9C
	.public ov74_0223CEF0
	.public ov74_0223CF04
	.public ov74_0223CF18
	.public ov74_0223CF2C
	.public ov74_0223CF40
	.public ov74_0223CF58
	.public ov74_0223CF70
	.public ov74_0223CF8C
	.public ov74_0223CFA8
	.public ov74_0223CFC4
	.public ov74_0223CFE4
	.public ov74_0223D00C
	.public ov74_0223D014
	.public ov74_0223D018
	.public ov74_0223D038
	.public ov74_0223D040
	.public ov74_0223D054
	.public ov74_0223D080
	.public ov74_0223D090
	.public ov74_0223D0A4
	.public ov74_0223D0A8
	.public ov74_0223D0BC
	.public ov74_0223D0C0
	.public ov74_0223D0C4
	.public ov74_0223D338
	.public ov74_0223D33C
	.public ov74_0223D34C
	.public ov74_0223D450
	.public ov74_0223D454
	.public ov74_0223D488
	.public ov74_0223D48C
	.public ov74_0223D654
	.public ov74_0223E2D4
	.public ov74_0223E2FC
	.public ov74_0223E304
	.public ov74_0223E318
	.public sAgbCartNintendoLogo
	.public sPmAgbCartridgeSpec
	.public sPmAgbRomCodeMappings
	.public sPmAgbRomHeader



	thumb_func_start ov74_02232DC4
ov74_02232DC4: ; 0x02232DC4
	push {r3, r4, r5, r6, r7, lr}
	add r6, r0, #0
	ldr r7, _02232E34 ; =0x00000414
	mov r4, #0
	add r5, r6, #0
_02232DCE:
	ldr r0, [r5, r7]
	bl Sprite_Delete
	add r4, r4, #1
	add r5, r5, #4
	cmp r4, #6
	blt _02232DCE
	mov r0, #2
	mov r1, #1
	bl GfGfx_EngineATogglePlanes
	ldr r0, _02232E38 ; =0x00000498
	add r0, r6, r0
	bl ov74_02232DA4
	mov r0, #0xe6
	lsl r0, r0, #2
	ldr r0, [r6, r0]
	mov r1, #1
	bl Sprite_SetDrawFlag
	mov r0, #0xee
	lsl r0, r0, #2
	ldr r0, [r6, r0]
	mov r1, #1
	bl Sprite_SetDrawFlag
	mov r0, #0xea
	lsl r0, r0, #2
	ldr r0, [r6, r0]
	mov r1, #1
	bl Sprite_SetDrawFlag
	mov r0, #0xf3
	mov r5, #0
	lsl r0, r0, #2
	add r4, r6, #0
	sub r3, r5, #1
	mov r2, #0xe
	add r1, r0, #4
_02232E1E:
	str r3, [r4, r0]
	str r2, [r4, r1]
	add r5, r5, #1
	add r4, #0xc
	cmp r5, #6
	blt _02232E1E
	mov r0, #0x41
	mov r1, #0
	lsl r0, r0, #4
	str r1, [r6, r0]
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
_02232E34: .word 0x00000414
_02232E38: .word 0x00000498
	thumb_func_end ov74_02232DC4




	thumb_func_start ov74_02232E3C
ov74_02232E3C: ; 0x02232E3C
	push {r3, r4, r5, r6, r7, lr}
	add r6, r0, #0
	mov r0, #0
	add r1, r0, #0
	bl Main_SetVBlankIntrCB
	mov r7, #0x6a
	mov r4, #0
	add r5, r6, #0
	lsl r7, r7, #2
_02232E50:
	ldr r0, [r5, r7]
	cmp r0, #0
	beq _02232E5A
	bl Sprite_Delete
_02232E5A:
	mov r0, #0x6b
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	cmp r0, #0
	beq _02232E68
	bl Sprite_Delete
_02232E68:
	add r4, r4, #1
	add r5, #0xc
	cmp r4, #0x1e
	blt _02232E50
	mov r7, #0xf2
	mov r5, #0
	add r4, r6, #0
	lsl r7, r7, #2
_02232E78:
	ldr r0, [r4, r7]
	cmp r0, #0
	beq _02232E82
	bl Sprite_Delete
_02232E82:
	add r5, r5, #1
	add r4, #0xc
	cmp r5, #6
	blt _02232E78
	mov r0, #0xe6
	lsl r0, r0, #2
	ldr r0, [r6, r0]
	cmp r0, #0
	beq _02232E98
	bl Sprite_Delete
_02232E98:
	mov r0, #0xee
	lsl r0, r0, #2
	ldr r0, [r6, r0]
	cmp r0, #0
	beq _02232EA6
	bl Sprite_Delete
_02232EA6:
	mov r0, #0xea
	lsl r0, r0, #2
	ldr r0, [r6, r0]
	cmp r0, #0
	beq _02232EB4
	bl Sprite_Delete
_02232EB4:
	ldr r0, _02232F48 ; =0x00000478
	add r0, r6, r0
	bl ov74_02232DA4
	ldr r0, _02232F4C ; =0x00000488
	add r0, r6, r0
	bl ov74_02232DA4
	ldr r0, _02232F50 ; =0x00000498
	add r0, r6, r0
	bl ov74_02232DA4
	mov r0, #2
	bl FontID_Release
	mov r0, #0x5b
	lsl r0, r0, #2
	ldr r0, [r6, r0]
	bl SpriteTransfer_DeleteCharTransferTask
	mov r0, #0x17
	lsl r0, r0, #4
	ldr r0, [r6, r0]
	bl SpriteTransfer_DeletePlttTransferTask
	mov r7, #0x55
	mov r5, #0
	add r4, r6, #0
	lsl r7, r7, #2
_02232EEE:
	ldr r0, [r4, r7]
	bl Destroy2DGfxResObjMan
	add r5, r5, #1
	add r4, r4, #4
	cmp r5, #6
	blt _02232EEE
	ldr r0, [r6, #0x28]
	bl SpriteList_Delete
	mov r0, #0
	str r0, [r6, #0x28]
	bl OamManager_Free
	bl ObjCharTransfer_Destroy
	bl ObjPlttTransfer_Destroy
	ldr r0, _02232F54 ; =0x0000E88C
	ldr r0, [r6, r0]
	bl YesNoPrompt_Destroy
	ldr r0, _02232F58 ; =gSystem + 0x60
	mov r1, #0
	strb r1, [r0, #9]
	bl GfGfx_SwapDisplay
	ldr r0, [r6, #0x20]
	mov r1, #0
	bl FreeBgTilemapBuffer
	ldr r0, [r6, #0x20]
	mov r1, #1
	bl FreeBgTilemapBuffer
	ldr r0, [r6, #0x20]
	mov r1, #2
	bl FreeBgTilemapBuffer
	ldr r0, [r6, #0x20]
	mov r1, #3
	bl FreeBgTilemapBuffer
	pop {r3, r4, r5, r6, r7, pc}
	nop
_02232F48: .word 0x00000478
_02232F4C: .word 0x00000488
_02232F50: .word 0x00000498
_02232F54: .word 0x0000E88C
_02232F58: .word gSystem + 0x60
	thumb_func_end ov74_02232E3C




	thumb_func_start ov74_02232F5C
ov74_02232F5C: ; 0x02232F5C
	push {lr}
	sub sp, #0x14
	ldr r1, [r0, #0x20]
	mov r3, #0
	str r1, [sp]
	mov r1, #2
	lsl r1, r1, #8
	str r1, [sp, #8]
	mov r1, #8
	str r1, [sp, #0xc]
	str r3, [sp, #4]
	mov r1, #0x19
	add r2, sp, #0
	strb r1, [r2, #0x10]
	mov r1, #7
	strb r1, [r2, #0x11]
	strb r3, [r2, #0x13]
	ldrb r3, [r2, #0x12]
	mov r1, #0xf0
	bic r3, r1
	mov r1, #0x10
	orr r1, r3
	strb r1, [r2, #0x12]
	ldr r1, _02232F98 ; =0x0000E88C
	ldr r0, [r0, r1]
	add r1, sp, #0
	bl YesNoPrompt_InitFromTemplate
	add sp, #0x14
	pop {pc}
	.balign 4, 0
_02232F98: .word 0x0000E88C
	thumb_func_end ov74_02232F5C




	thumb_func_start ov74_02232F9C
ov74_02232F9C: ; 0x02232F9C
	push {r4, r5, r6, lr}
	sub sp, #0x18
	add r5, r0, #0
	bl ov74_02233F84
	add r4, r0, #0
	bl PmAgbCartridge_GetLanguage
	add r3, r0, #0
	add r0, r4, #0
	add r1, sp, #8
	mov r2, #8
	bl ConvertRSStringToDPStringInternational
	mov r0, #0x4c
	bl MessageFormat_New
	add r4, r0, #0
	mov r0, #8
	mov r1, #0x4c
	bl String_New
	add r1, sp, #8
	add r6, r0, #0
	bl CopyU16ArrayToString
	mov r1, #1
	str r1, [sp]
	mov r0, #2
	str r0, [sp, #4]
	add r0, r4, #0
	add r2, r6, #0
	mov r3, #0
	bl BufferString
	add r0, r5, #0
	bl ov74_02232B18
	ldr r0, [r5]
	mov r2, #0x18
	lsl r1, r0, #2
	ldr r0, _02233020 ; =ov74_0223C980
	ldr r0, [r0, r1]
	mov r1, #0x46
	lsl r1, r1, #4
	str r0, [r5, r1]
	add r0, r1, #0
	add r0, #0xc
	sub r1, #0x34
	str r4, [r5, r0]
	add r0, r5, #0
	add r1, r5, r1
	bl ov74_02231A1C
	add r0, r6, #0
	bl String_Delete
	add r0, r4, #0
	bl MessageFormat_Delete
	add r0, r5, #0
	bl ov74_02232F5C
	add sp, #0x18
	pop {r4, r5, r6, pc}
	nop
_02233020: .word ov74_0223C980
	thumb_func_end ov74_02232F9C




	thumb_func_start ov74_02233024
ov74_02233024: ; 0x02233024
	push {r4, lr}
	add r4, r0, #0
	ldr r0, _02233054 ; =0x00012604
	ldr r0, [r4, r0]
	cmp r0, #0
	beq _02233038
	blx r0
	ldr r0, _02233054 ; =0x00012604
	mov r1, #0
	str r1, [r4, r0]
_02233038:
	bl GF_RunVramTransferTasks
	bl OamManager_ApplyAndResetBuffers
	ldr r0, [r4, #0x20]
	bl DoScheduledBgGpuUpdates
	ldr r3, _02233058 ; =0x027E0000
	ldr r1, _0223305C ; =0x00003FF8
	mov r0, #1
	ldr r2, [r3, r1]
	orr r0, r2
	str r0, [r3, r1]
	pop {r4, pc}
	.balign 4, 0
_02233054: .word 0x00012604
_02233058: .word 0x027E0000
_0223305C: .word 0x00003FF8
	thumb_func_end ov74_02233024




	thumb_func_start ov74_02233060
ov74_02233060: ; 0x02233060
	push {r4, r5, r6, r7, lr}
	sub sp, #0xc
	str r0, [sp]
	ldr r0, [r0, #0x10]
	bl Save_MigratedPokemon_Get
	mov r6, #0
	str r0, [sp, #8]
	str r6, [sp, #4]
	add r7, r6, #0
_02233074:
	mov r4, #0
	add r5, r4, #0
_02233078:
	ldr r1, [sp]
	ldr r0, _022330CC ; =0x0000E880
	mov r2, #0
	ldr r0, [r1, r0]
	mov r1, #5
	add r0, r0, #4
	add r0, r0, r7
	add r0, r0, r5
	bl AGB_GetBoxMonData
	cmp r0, #0
	beq _02233092
	add r6, r6, #1
_02233092:
	add r4, r4, #1
	add r5, #0x50
	cmp r4, #0x1e
	blt _02233078
	mov r0, #0x96
	lsl r0, r0, #4
	add r7, r7, r0
	ldr r0, [sp, #4]
	add r0, r0, #1
	str r0, [sp, #4]
	cmp r0, #0xe
	blt _02233074
	cmp r6, #6
	bge _022330B4
	add sp, #0xc
	mov r0, #5
	pop {r4, r5, r6, r7, pc}
_022330B4:
	ldr r0, [sp, #8]
	bl MigratedPokemon_CountPokemon
	cmp r0, #0
	beq _022330C4
	add sp, #0xc
	mov r0, #6
	pop {r4, r5, r6, r7, pc}
_022330C4:
	mov r0, #0
	add sp, #0xc
	pop {r4, r5, r6, r7, pc}
	nop
_022330CC: .word 0x0000E880
	thumb_func_end ov74_02233060




	thumb_func_start ov74_022330D0
ov74_022330D0: ; 0x022330D0
	push {r3, r4, r5, lr}
	add r5, r0, #0
	add r4, r1, #0
	ldr r1, [r5, #4]
	cmp r1, #0
	beq _0223310C
	bl ov74_02232B18
	mov r0, #1
	bl TextFlags_SetCanTouchSpeedUpPrint
	ldr r0, _0223312C ; =ov74_0223C9B8
	lsl r1, r4, #2
	ldr r0, [r0, r1]
	mov r1, #0x46
	lsl r1, r1, #4
	str r0, [r5, r1]
	add r0, r1, #0
	sub r1, #0x34
	mov r2, #1
	add r0, #0x10
	str r2, [r5, r0]
	add r0, r5, #0
	add r1, r5, r1
	mov r2, #0x18
	bl ov74_02231A1C
	mov r0, #0
	str r0, [r5, #4]
	b _02233126
_0223310C:
	ldr r0, _02233130 ; =0x00000474
	ldr r0, [r5, r0]
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	bl TextPrinterCheckActive
	cmp r0, #0
	bne _02233126
	mov r0, #0
	bl TextFlags_SetCanTouchSpeedUpPrint
	mov r0, #1
	pop {r3, r4, r5, pc}
_02233126:
	mov r0, #0
	pop {r3, r4, r5, pc}
	nop
_0223312C: .word ov74_0223C9B8
_02233130: .word 0x00000474
	thumb_func_end ov74_022330D0




	thumb_func_start ov74_02233134
ov74_02233134: ; 0x02233134
	push {r3, r4, r5, lr}
	add r5, r0, #0
	add r4, r1, #0
	ldr r1, [r5, #4]
	cmp r1, #0
	beq _02233170
	bl ov74_02232B18
	mov r0, #1
	bl TextFlags_SetCanTouchSpeedUpPrint
	ldr r0, _02233194 ; =ov74_0223C9B8
	lsl r1, r4, #2
	ldr r0, [r0, r1]
	mov r1, #0x46
	lsl r1, r1, #4
	str r0, [r5, r1]
	add r0, r1, #0
	sub r1, #0x34
	mov r2, #1
	add r0, #0x10
	str r2, [r5, r0]
	add r0, r5, #0
	add r1, r5, r1
	mov r2, #0x18
	bl ov74_02231A1C
	mov r0, #0
	str r0, [r5, #4]
	b _02233190
_02233170:
	ldr r0, _02233198 ; =0x00000474
	ldr r0, [r5, r0]
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	bl TextPrinterCheckActive
	cmp r0, #0
	bne _02233190
	add r0, r5, #0
	bl ov74_02232F5C
	mov r0, #0
	bl TextFlags_SetCanTouchSpeedUpPrint
	mov r0, #1
	pop {r3, r4, r5, pc}
_02233190:
	mov r0, #0
	pop {r3, r4, r5, pc}
	.balign 4, 0
_02233194: .word ov74_0223C9B8
_02233198: .word 0x00000474
	thumb_func_end ov74_02233134




	thumb_func_start ov74_0223319C
ov74_0223319C: ; 0x0223319C
	push {r3, r4, r5, lr}
	add r5, r0, #0
	add r4, r1, #0
	mov r1, #0
	ldr r2, [r5, #8]
	mvn r1, r1
	cmp r2, r1
	beq _022331DE
	bl ov74_02232B18
	mov r0, #1
	bl TextFlags_SetCanTouchSpeedUpPrint
	mov r1, #0x46
	ldr r0, [r5, #8]
	lsl r1, r1, #4
	str r0, [r5, r1]
	add r0, r1, #0
	add r0, #0x10
	sub r1, #0x34
	str r4, [r5, r0]
	add r0, r5, #0
	add r1, r5, r1
	mov r2, #0x18
	bl ov74_02231A1C
	mov r0, #0
	mvn r0, r0
	str r0, [r5, #8]
	ldr r0, _02233220 ; =SEQ_SE_DP_SELECT
	bl PlaySE
	b _0223321C
_022331DE:
	cmp r4, #0
	beq _022331FC
	ldr r0, _02233224 ; =0x00000474
	ldr r0, [r5, r0]
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	bl TextPrinterCheckActive
	cmp r0, #0
	bne _0223321C
	mov r0, #0
	bl TextFlags_SetCanTouchSpeedUpPrint
	mov r0, #1
	pop {r3, r4, r5, pc}
_022331FC:
	ldr r0, _02233228 ; =gSystem + 0x40
	ldrh r0, [r0, #0x24]
	cmp r0, #0
	bne _0223320C
	ldr r0, _0223322C ; =gSystem
	ldr r0, [r0, #0x48]
	cmp r0, #0
	beq _0223321C
_0223320C:
	mov r0, #0
	bl TextFlags_SetCanTouchSpeedUpPrint
	ldr r0, _02233220 ; =SEQ_SE_DP_SELECT
	bl PlaySE
	mov r0, #1
	pop {r3, r4, r5, pc}
_0223321C:
	mov r0, #0
	pop {r3, r4, r5, pc}
	.balign 4, 0
_02233220: .word SEQ_SE_DP_SELECT
_02233224: .word 0x00000474
_02233228: .word gSystem + 0x40
_0223322C: .word gSystem
	thumb_func_end ov74_0223319C




	thumb_func_start ov74_02233230
ov74_02233230: ; 0x02233230
	push {r3, r4, r5, lr}
	mov r2, #0xe
	add r5, r0, #0
	mov r0, #3
	mov r1, #0x4c
	lsl r2, r2, #0xe
	bl Heap_Create
	ldr r1, _022332D8 ; =0x00012610
	add r0, r5, #0
	mov r2, #0x4c
	bl OverlayManager_CreateAndGetData
	ldr r2, _022332D8 ; =0x00012610
	mov r1, #0
	add r4, r0, #0
	bl memset
	mov r0, #0x4c
	bl BgConfig_Alloc
	str r0, [r4, #0x20]
	mov r0, #0x4c
	bl YesNoPrompt_Create
	ldr r1, _022332DC ; =0x0000E88C
	str r0, [r4, r1]
	mov r0, #0
	add r1, r0, #0
	bl sub_0200FBF4
	mov r0, #1
	mov r1, #0
	bl sub_0200FBF4
	add r0, r5, #0
	bl OverlayManager_GetArgs
	ldr r0, [r0, #8]
	str r0, [r4, #0x10]
	bl Save_PlayerData_GetProfile
	str r0, [r4, #0x14]
	ldr r0, [r4, #0x10]
	bl Save_PlayerData_GetOptionsAddr
	str r0, [r4, #0x18]
	bl Options_GetFrame
	str r0, [r4, #0x1c]
	mov r0, #6
	lsl r0, r0, #6
	mov r1, #0x4c
	bl String_New
	ldr r1, _022332E0 ; =0x00012608
	str r0, [r4, r1]
	mov r0, #6
	lsl r0, r0, #6
	mov r1, #0x4c
	bl String_New
	ldr r1, _022332E4 ; =0x0001260C
	mov r2, #1
	str r0, [r4, r1]
	ldr r1, _022332E8 ; =0x0000047E
	mov r0, #9
	bl Sound_SetSceneAndPlayBGM
	bl OS_IsTickAvailable
	cmp r0, #0
	bne _022332C6
	bl OS_InitTick
_022332C6:
	bl ov74_02236074
	ldr r0, _022332EC ; =0x0000E89C
	add r1, r4, r0
	ldr r0, _022332F0 ; =ov74_0223D338
	str r1, [r0]
	mov r0, #1
	pop {r3, r4, r5, pc}
	nop
_022332D8: .word 0x00012610
_022332DC: .word 0x0000E88C
_022332E0: .word 0x00012608
_022332E4: .word 0x0001260C
_022332E8: .word 0x0000047E
_022332EC: .word 0x0000E89C
_022332F0: .word ov74_0223D338
	thumb_func_end ov74_02233230




	thumb_func_start ov74_022332F4
ov74_022332F4: ; 0x022332F4
	push {r4, r5, r6, lr}
	add r5, r1, #0
	bl OverlayManager_GetData
	add r4, r0, #0
	bl CTRDG_IsExisting
	bl CTRDG_IsPulledOut
	cmp r0, #1
	beq _02233312
	ldr r0, _02233618 ; =_02111864
	ldr r0, [r0]
	cmp r0, #0
	beq _02233328
_02233312:
	ldr r0, _0223361C ; =0x0000E890
	ldr r0, [r4, r0]
	sub r0, r0, #3
	cmp r0, #1
	bhi _02233322
	ldr r0, [r4, #0x10]
	bl Save_Cancel
_02233322:
	mov r0, #0x4c
	bl ShowGBACartRemovedError
_02233328:
	ldr r0, [r4, #0xc]
	add r0, r0, #1
	str r0, [r4, #0xc]
	ldr r0, [r5]
	cmp r0, #0x18
	bhi _02233424
	add r0, r0, r0
	add r0, pc
	ldrh r0, [r0, #6]
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	add pc, r0
_02233340: ; jump table
	.short _02233372 - _02233340 - 2 ; case 0
	.short _022333BA - _02233340 - 2 ; case 1
	.short _02233400 - _02233340 - 2 ; case 2
	.short _02233414 - _02233340 - 2 ; case 3
	.short _0223344E - _02233340 - 2 ; case 4
	.short _02233466 - _02233340 - 2 ; case 5
	.short _022334BC - _02233340 - 2 ; case 6
	.short _022334CE - _02233340 - 2 ; case 7
	.short _022334EC - _02233340 - 2 ; case 8
	.short _022334FE - _02233340 - 2 ; case 9
	.short _0223355C - _02233340 - 2 ; case 10
	.short _02233574 - _02233340 - 2 ; case 11
	.short _02233588 - _02233340 - 2 ; case 12
	.short _022335C8 - _02233340 - 2 ; case 13
	.short _022336F6 - _02233340 - 2 ; case 14
	.short _02233710 - _02233340 - 2 ; case 15
	.short _0223372C - _02233340 - 2 ; case 16
	.short _02233746 - _02233340 - 2 ; case 17
	.short _0223377C - _02233340 - 2 ; case 18
	.short _022337A2 - _02233340 - 2 ; case 19
	.short _0223380C - _02233340 - 2 ; case 20
	.short _02233818 - _02233340 - 2 ; case 21
	.short _0223385A - _02233340 - 2 ; case 22
	.short _0223387E - _02233340 - 2 ; case 23
	.short _0223388C - _02233340 - 2 ; case 24
_02233372:
	bl PmAgbCartridgeUnlinkSpec
	mov r0, #0x4b
	lsl r0, r0, #4
	add r0, r4, r0
	bl PmAgbCartridgeGetOffsets
	ldr r1, _02233620 ; =0x000004A8
	str r0, [r4, r1]
	ldr r0, [r4, r1]
	cmp r0, #0
	bne _022333AE
	bl ov74_02233CE4
	ldr r1, _02233624 ; =0x000004AC
	str r0, [r4, r1]
	ldr r0, [r4, r1]
	cmp r0, #0
	bne _022333AA
	bl ov74_02233F4C
	ldr r1, _02233628 ; =0x0000E880
	str r0, [r4, r1]
	ldr r0, [r4, r1]
	ldrb r2, [r0]
	add r0, r1, #4
	str r2, [r4, r0]
	b _022333AE
_022333AA:
	mov r0, #1
	str r0, [r4, #4]
_022333AE:
	add r0, r4, #0
	bl ov74_02231790
	mov r0, #1
	str r0, [r5]
	b _02233896
_022333BA:
	add r0, r4, #0
	bl ov74_02232A48
	bl ov74_02231BC0
	add r0, r4, #0
	bl ov74_02231BF0
	add r0, r4, #0
	bl ov74_02231CFC
	add r0, r4, #0
	bl ov74_022322D8
	ldr r0, _0223362C ; =ov74_02233024
	add r1, r4, #0
	bl Main_SetVBlankIntrCB
	mov r0, #0x10
	mov r1, #0
	bl GfGfx_EngineATogglePlanes
	ldr r0, [r4, #4]
	cmp r0, #1
	bne _022333FA
	add r0, r4, #0
	mov r1, #1
	mov r2, #0xb
	add r3, r5, #0
	bl ov74_02232910
	b _02233896
_022333FA:
	mov r0, #2
	str r0, [r5]
	b _02233896
_02233400:
	add r0, r4, #0
	bl ov74_02232F9C
	add r0, r4, #0
	mov r1, #1
	mov r2, #3
	add r3, r5, #0
	bl ov74_02232910
	b _02233896
_02233414:
	ldr r0, _02233630 ; =0x0000E88C
	ldr r0, [r4, r0]
	bl YesNoPrompt_HandleInput
	cmp r0, #1
	beq _02233426
	cmp r0, #2
	beq _02233438
_02233424:
	b _02233896
_02233426:
	ldr r0, _02233630 ; =0x0000E88C
	ldr r0, [r4, r0]
	bl YesNoPrompt_Reset
#ifdef HEARTGOLD
	mov r0, #0x22
#else
	mov r0, #0x23
#endif
	str r0, [r4, #8]
	mov r0, #4
	str r0, [r5]
	b _02233896
_02233438:
	ldr r0, _02233630 ; =0x0000E88C
	ldr r0, [r4, r0]
	bl YesNoPrompt_Reset
	add r0, r4, #0
	mov r1, #0
	mov r2, #0x18
	add r3, r5, #0
	bl ov74_02232940
	b _02233896
_0223344E:
	add r0, r4, #0
	mov r1, #1
	bl ov74_0223319C
	cmp r0, #0
	beq _02233476
	add r0, r4, #0
	bl ov74_02232F5C
	mov r0, #5
	str r0, [r5]
	b _02233896
_02233466:
	ldr r0, _02233630 ; =0x0000E88C
	ldr r0, [r4, r0]
	bl YesNoPrompt_HandleInput
	cmp r0, #1
	beq _02233478
	cmp r0, #2
	beq _022334A6
_02233476:
	b _02233896
_02233478:
	ldr r0, _02233630 ; =0x0000E88C
	ldr r0, [r4, r0]
	bl YesNoPrompt_Reset
	add r0, r4, #0
	bl ov74_02233060
	str r0, [r4, #4]
	cmp r0, #0
	beq _0223349E
	sub r0, r0, #3
	cmp r0, #1
	bhi _02233498
	mov r0, #8
	str r0, [r5]
	b _02233896
_02233498:
	mov r0, #0xb
	str r0, [r5]
	b _02233896
_0223349E:
	mov r0, #6
	str r0, [r4, #8]
	str r0, [r5]
	b _02233896
_022334A6:
	ldr r0, _02233630 ; =0x0000E88C
	ldr r0, [r4, r0]
	bl YesNoPrompt_Reset
	add r0, r4, #0
	mov r1, #0
	mov r2, #0x18
	add r3, r5, #0
	bl ov74_02232940
	b _02233896
_022334BC:
	add r0, r4, #0
	mov r1, #1
	bl ov74_0223319C
	cmp r0, #0
	beq _0223350E
	mov r0, #7
	str r0, [r5]
	b _02233896
_022334CE:
	ldr r0, _02233634 ; =gSystem + 0x40
	ldrh r0, [r0, #0x24]
	cmp r0, #0
	bne _022334DE
	ldr r0, _02233638 ; =gSystem
	ldr r0, [r0, #0x48]
	cmp r0, #0
	beq _0223350E
_022334DE:
	add r0, r4, #0
	mov r1, #0
	mov r2, #0xc
	add r3, r5, #0
	bl ov74_02232910
	b _02233896
_022334EC:
	ldr r1, [r4, #4]
	add r0, r4, #0
	bl ov74_02233134
	cmp r0, #0
	beq _0223350E
	mov r0, #9
	str r0, [r5]
	b _02233896
_022334FE:
	ldr r0, _02233630 ; =0x0000E88C
	ldr r0, [r4, r0]
	bl YesNoPrompt_HandleInput
	cmp r0, #1
	beq _02233510
	cmp r0, #2
	beq _02233546
_0223350E:
	b _02233896
_02233510:
	ldr r0, _02233630 ; =0x0000E88C
	ldr r0, [r4, r0]
	bl YesNoPrompt_Reset
	mov r1, #0x47
	lsl r1, r1, #4
	mov r2, #0
	add r0, r1, #0
	str r2, [r4, r1]
	sub r1, #0x44
	mov r3, #0x28
	sub r0, #0x10
	str r3, [r4, r0]
	add r0, r4, #0
	add r1, r4, r1
	bl ov74_02231A1C
	ldr r1, _0223363C ; =0x00000498
	add r0, r4, r1
	sub r1, #0xc6
	bl WaitingIcon_New
	ldr r1, _02233640 ; =0x0000E898
	str r0, [r4, r1]
	mov r0, #0xa
	str r0, [r5]
	b _02233896
_02233546:
	ldr r0, _02233630 ; =0x0000E88C
	ldr r0, [r4, r0]
	bl YesNoPrompt_Reset
	add r0, r4, #0
	mov r1, #0
	mov r2, #0x18
	add r3, r5, #0
	bl ov74_02232940
	b _02233896
_0223355C:
	add r0, r4, #0
	bl ov74_02231930
	ldr r0, _02233640 ; =0x0000E898
	ldr r0, [r4, r0]
	bl sub_0200F450
	mov r0, #7
	str r0, [r4, #4]
	mov r0, #0xb
	str r0, [r5]
	b _02233896
_02233574:
	ldr r1, [r4, #4]
	add r0, r4, #0
	bl ov74_022330D0
	cmp r0, #0
	bne _02233582
	b _02233896
_02233582:
	mov r0, #0x16
	str r0, [r5]
	b _02233896
_02233588:
	ldr r0, _0223363C ; =0x00000498
	add r0, r4, r0
	bl ov74_02232DA4
	add r0, r4, #0
	bl ov74_02232AC8
	add r0, r4, #0
	bl ov74_02232154
	add r0, r4, #0
	bl ov74_02231FF4
	ldr r0, _0223362C ; =ov74_02233024
	add r1, r4, #0
	bl Main_SetVBlankIntrCB
	mov r0, #0x10
	mov r1, #1
	bl GfGfx_EngineATogglePlanes
	add r0, r4, #0
	mov r1, #0
	bl ov74_02232758
	add r0, r4, #0
	mov r1, #1
	mov r2, #0xd
	add r3, r5, #0
	bl ov74_02232910
	b _02233896
_022335C8:
	mov r0, #0x31
	lsl r0, r0, #4
	add r0, r4, r0
	bl TouchscreenHitbox_FindRectAtTouchNew
	add r6, r0, #0
	mov r0, #0
	mvn r0, r0
	cmp r6, r0
	beq _022336A2
	cmp r6, #0x1e
	bge _02233698
	add r0, r4, #0
	add r1, r6, #0
	bl ov74_022324A0
	cmp r0, #1
	bne _02233652
	ldr r2, _02233628 ; =0x0000E880
	add r0, r4, #0
	ldr r1, [r4, r2]
	add r2, r2, #4
	ldr r3, [r4, r2]
	mov r2, #0x96
	lsl r2, r2, #4
	add r1, r1, #4
	mul r2, r3
	add r2, r1, r2
	mov r1, #0x50
	mul r1, r6
	add r1, r2, r1
	bl ov74_02232758
	mov r0, #0x41
	lsl r0, r0, #4
	ldr r0, [r4, r0]
	cmp r0, #6
	bne _022336A2
	ldr r0, _02233644 ; =0x0000E888
	b _02233648
	.balign 4, 0
_02233618: .word _02111864
_0223361C: .word 0x0000E890
_02233620: .word 0x000004A8
_02233624: .word 0x000004AC
_02233628: .word 0x0000E880
_0223362C: .word ov74_02233024
_02233630: .word 0x0000E88C
_02233634: .word gSystem + 0x40
_02233638: .word gSystem
_0223363C: .word 0x00000498
_02233640: .word 0x0000E898
_02233644: .word 0x0000E888
_02233648:
	mov r1, #0x2d
	str r1, [r4, r0]
	mov r0, #0xf
	str r0, [r5]
	b _02233896
_02233652:
	cmp r0, #2
	bne _02233660
	add r0, r4, #0
	mov r1, #0
	bl ov74_02232758
	b _02233896
_02233660:
	cmp r0, #4
	bne _0223366E
	mov r0, #8
	str r0, [r4, #8]
	mov r0, #0xe
	str r0, [r5]
	b _02233896
_0223366E:
	cmp r0, #5
	bne _0223367C
	mov r0, #9
	str r0, [r4, #8]
	mov r0, #0xe
	str r0, [r5]
	b _02233896
_0223367C:
	cmp r0, #6
	bne _0223368A
	mov r0, #0x26
	str r0, [r4, #8]
	mov r0, #0xe
	str r0, [r5]
	b _02233896
_0223368A:
	cmp r0, #7
	bne _022336A2
	mov r0, #0x26
	str r0, [r4, #8]
	mov r0, #0xe
	str r0, [r5]
	b _02233896
_02233698:
	beq _022336E2
	cmp r6, #0x1f
	beq _022336A4
	cmp r6, #0x20
	beq _022336C2
_022336A2:
	b _02233896
_022336A4:
	ldr r0, _022338A4 ; =0x0000E884
	ldr r1, [r4, r0]
	cmp r1, #0
	bne _022336B0
	mov r1, #0xd
	b _022336B2
_022336B0:
	sub r1, r1, #1
_022336B2:
	str r1, [r4, r0]
	add r0, r4, #0
	bl ov74_02231FF4
	ldr r0, _022338A8 ; =SEQ_SE_DP_SELECT
	bl PlaySE
	b _02233896
_022336C2:
	ldr r0, _022338A4 ; =0x0000E884
	ldr r1, [r4, r0]
	add r1, r1, #1
	str r1, [r4, r0]
	ldr r1, [r4, r0]
	cmp r1, #0xe
	bne _022336D4
	mov r1, #0
	str r1, [r4, r0]
_022336D4:
	add r0, r4, #0
	bl ov74_02231FF4
	ldr r0, _022338A8 ; =SEQ_SE_DP_SELECT
	bl PlaySE
	b _02233896
_022336E2:
	add r0, r4, #0
	mov r1, #0
	mov r2, #0x18
	add r3, r5, #0
	bl ov74_02232940
	ldr r0, _022338A8 ; =SEQ_SE_DP_SELECT
	bl PlaySE
	b _02233896
_022336F6:
	add r0, r4, #0
	mov r1, #0
	bl ov74_0223319C
	cmp r0, #0
	beq _02233756
	ldr r0, _022338AC ; =0x00000498
	add r0, r4, r0
	bl ov74_02232DA4
	mov r0, #0xd
	str r0, [r5]
	b _02233896
_02233710:
	ldr r0, _022338B0 ; =0x0000E888
	ldr r1, [r4, r0]
	sub r1, r1, #1
	str r1, [r4, r0]
	ldr r0, [r4, r0]
	cmp r0, #0
	bne _02233756
	add r0, r4, #0
	mov r1, #0
	mov r2, #0x10
	add r3, r5, #0
	bl ov74_02232910
	b _02233896
_0223372C:
	add r0, r4, #0
	bl ov74_02232BD4
	add r0, r4, #0
	mov r1, #1
	mov r2, #0x11
	add r3, r5, #0
	bl ov74_02232910
	add r0, r4, #0
	bl ov74_02232F5C
	b _02233896
_02233746:
	ldr r0, _022338B4 ; =0x0000E88C
	ldr r0, [r4, r0]
	bl YesNoPrompt_HandleInput
	cmp r0, #1
	beq _02233758
	cmp r0, #2
	beq _02233766
_02233756:
	b _02233896
_02233758:
	ldr r0, _022338B4 ; =0x0000E88C
	ldr r0, [r4, r0]
	bl YesNoPrompt_Reset
	mov r0, #0x12
	str r0, [r5]
	b _02233896
_02233766:
	ldr r0, _022338B4 ; =0x0000E88C
	ldr r0, [r4, r0]
	bl YesNoPrompt_Reset
	add r0, r4, #0
	mov r1, #0
	mov r2, #0x14
	add r3, r5, #0
	bl ov74_02232910
	b _02233896
_0223377C:
	mov r1, #0x46
	mov r0, #0xb
	lsl r1, r1, #4
	str r0, [r4, r1]
	sub r1, #0x34
	add r0, r4, #0
	add r1, r4, r1
	mov r2, #0
	bl ov74_02231A1C
	add r0, r4, #0
	bl ov74_02232F5C
	ldr r0, _022338A8 ; =SEQ_SE_DP_SELECT
	bl PlaySE
	mov r0, #0x13
	str r0, [r5]
	b _02233896
_022337A2:
	ldr r0, _022338B4 ; =0x0000E88C
	ldr r0, [r4, r0]
	bl YesNoPrompt_HandleInput
	cmp r0, #1
	beq _022337B4
	cmp r0, #2
	beq _022337F6
	b _02233896
_022337B4:
	ldr r0, [r4]
	mov r2, #0
	lsl r1, r0, #2
	ldr r0, _022338B8 ; =ov74_0223C968
	ldr r0, [r0, r1]
	mov r1, #0x46
	lsl r1, r1, #4
	str r0, [r4, r1]
	sub r1, #0x34
	add r0, r4, #0
	add r1, r4, r1
	bl ov74_02231A1C
	ldr r0, _022338B4 ; =0x0000E88C
	ldr r0, [r4, r0]
	bl YesNoPrompt_Reset
	ldr r0, _022338BC ; =0x0000E890
	mov r1, #0
	str r1, [r4, r0]
	ldr r1, _022338AC ; =0x00000498
	add r0, r4, r1
	sub r1, #0xc6
	bl WaitingIcon_New
	ldr r1, _022338C0 ; =0x0000E898
	str r0, [r4, r1]
	mov r0, #0x15
	str r0, [r5]
	mov r0, #4
	bl sub_0201A728
	b _02233896
_022337F6:
	ldr r0, _022338B4 ; =0x0000E88C
	ldr r0, [r4, r0]
	bl YesNoPrompt_Reset
	add r0, r4, #0
	mov r1, #0
	mov r2, #0x14
	add r3, r5, #0
	bl ov74_02232910
	b _02233896
_0223380C:
	add r0, r4, #0
	bl ov74_02232DC4
	mov r0, #0xc
	str r0, [r5]
	b _02233896
_02233818:
	add r0, r4, #0
	bl ov74_022317D8
	add r6, r0, #0
	cmp r6, #0xa
	beq _02233896
	ldr r0, _022338C0 ; =0x0000E898
	ldr r0, [r4, r0]
	bl sub_0200F450
	ldr r0, _022338C4 ; =SEQ_SE_DP_SAVE
	bl PlaySE
	cmp r6, #0xb
	bne _0223383A
#ifdef HEARTGOLD
	mov r1, #0x1d
#else
	mov r1, #0x1e
#endif
	b _0223383C
_0223383A:
	mov r1, #0x24
_0223383C:
	mov r0, #0x46
	lsl r0, r0, #4
	str r1, [r4, r0]
	ldr r1, _022338C8 ; =0x0000042C
	add r0, r4, #0
	add r1, r4, r1
	mov r2, #0
	bl ov74_02231A1C
	mov r0, #0x16
	str r0, [r5]
	mov r0, #4
	bl sub_0201A738
	b _02233896
_0223385A:
	ldr r0, _022338CC ; =gSystem + 0x40
	ldrh r0, [r0, #0x24]
	cmp r0, #0
	bne _0223386A
	ldr r0, _022338D0 ; =gSystem
	ldr r0, [r0, #0x48]
	cmp r0, #0
	beq _02233896
_0223386A:
	add r0, r4, #0
	mov r1, #0
	mov r2, #0x18
	add r3, r5, #0
	bl ov74_02232940
	ldr r0, _022338A8 ; =SEQ_SE_DP_SELECT
	bl PlaySE
	b _02233896
_0223387E:
	bl IsPaletteFadeFinished
	cmp r0, #0
	beq _02233896
	ldr r0, [r4, #0x24]
	str r0, [r5]
	b _02233896
_0223388C:
	add r0, r4, #0
	bl ov74_02232E3C
	mov r0, #1
	pop {r4, r5, r6, pc}
_02233896:
	ldr r0, [r4, #0x28]
	cmp r0, #0
	beq _022338A0
	bl SpriteList_RenderAndAnimateSprites
_022338A0:
	mov r0, #0
	pop {r4, r5, r6, pc}
	.balign 4, 0
_022338A4: .word 0x0000E884
_022338A8: .word SEQ_SE_DP_SELECT
_022338AC: .word 0x00000498
_022338B0: .word 0x0000E888
_022338B4: .word 0x0000E88C
_022338B8: .word ov74_0223C968
_022338BC: .word 0x0000E890
_022338C0: .word 0x0000E898
_022338C4: .word SEQ_SE_DP_SAVE
_022338C8: .word 0x0000042C
_022338CC: .word gSystem + 0x40
_022338D0: .word gSystem
	thumb_func_end ov74_022332F4




	thumb_func_start ov74_022338D4
ov74_022338D4: ; 0x022338D4
	push {r3, r4, r5, lr}
	add r5, r0, #0
	bl OverlayManager_GetData
	add r4, r0, #0
	ldr r0, _02233914 ; =0x00012608
	ldr r0, [r4, r0]
	bl String_Delete
	ldr r0, _02233918 ; =0x0001260C
	ldr r0, [r4, r0]
	bl String_Delete
	ldr r0, [r4, #0x20]
	bl Heap_Free
	ldr r0, _0223391C ; =FS_OVERLAY_ID(intro_title)
	ldr r1, _02233920 ; =gApplication_TitleScreen
	bl RegisterMainOverlay
	add r0, r5, #0
	bl OverlayManager_FreeData
	mov r0, #0x4c
	bl Heap_Destroy
	mov r0, #0
	bl ov74_02236034
	mov r0, #1
	pop {r3, r4, r5, pc}
	nop
_02233914: .word 0x00012608
_02233918: .word 0x0001260C
_0223391C: .word FS_OVERLAY_ID(intro_title)
_02233920: .word gApplication_TitleScreen
	thumb_func_end ov74_022338D4
