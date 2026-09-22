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



	thumb_func_start ov74_02236AE0
ov74_02236AE0: ; 0x02236AE0
	push {r3, r4, r5, lr}
	add r5, r1, #0
	bl OverlayManager_GetData
	add r4, r0, #0
	ldr r0, _02236DE8 ; =0x0000267C
	ldr r0, [r4, r0]
	cmp r0, #0
	beq _02236B00
	blx r0
	cmp r0, #1
	bne _02236B00
	add r0, r4, #0
	bl ov74_022369D8
	str r0, [r5]
_02236B00:
	mov r3, #1
	ldr r0, [r5]
	lsl r3, r3, #0xc
	sub r0, r0, r3
	cmp r0, #0x13
	bls _02236B0E
	b _02236F0A
_02236B0E:
	add r0, r0, r0
	add r0, pc
	ldrh r0, [r0, #6]
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	add pc, r0
_02236B1A: ; jump table
	.short _02236B42 - _02236B1A - 2 ; case 0
	.short _02236B5A - _02236B1A - 2 ; case 1
	.short _02236B76 - _02236B1A - 2 ; case 2
	.short _02236B9E - _02236B1A - 2 ; case 3
	.short _02236BE8 - _02236B1A - 2 ; case 4
	.short _02236C00 - _02236B1A - 2 ; case 5
	.short _02236C46 - _02236B1A - 2 ; case 6
	.short _02236C6A - _02236B1A - 2 ; case 7
	.short _02236CA0 - _02236B1A - 2 ; case 8
	.short _02236CC2 - _02236B1A - 2 ; case 9
	.short _02236CFE - _02236B1A - 2 ; case 10
	.short _02236D24 - _02236B1A - 2 ; case 11
	.short _02236DAA - _02236B1A - 2 ; case 12
	.short _02236DC8 - _02236B1A - 2 ; case 13
	.short _02236F0A - _02236B1A - 2 ; case 14
	.short _02236DE2 - _02236B1A - 2 ; case 15
	.short _02236E94 - _02236B1A - 2 ; case 16
	.short _02236E8A - _02236B1A - 2 ; case 17
	.short _02236EB0 - _02236B1A - 2 ; case 18
	.short _02236EE6 - _02236B1A - 2 ; case 19
_02236B42:
	bl sub_02034D8C
	bl LoadDwcOverlay
	bl LoadOVY38
	mov r0, #3
	bl sub_02039FD8
	ldr r0, _02236DEC ; =0x00001001
	str r0, [r5]
	b _02236F0A
_02236B5A:
	bl sub_02034DB8
	cmp r0, #0
	beq _02236C3E
	ldr r0, _02236DF0 ; =ov74_022369A8
	ldr r1, _02236DF4 ; =ov74_022369C8
	bl ov00_021EC294
	ldr r0, [r4, #4]
	bl sub_020394A0
	ldr r0, _02236DF8 ; =0x00001002
	str r0, [r5]
	b _02236F0A
_02236B76:
	ldr r0, _02236DFC ; =0x000015E8
	mov r1, #2
	add r0, r4, r0
	mov r2, #1
	mov r3, #0x14
	bl ov00_021EC3F0
	mov r0, #2
	bl ov00_021EC454
	bl ov00_021EC4A4
	bl sub_0203A880
	ldr r0, _02236E00 ; =0x00001003
	mov r1, #0
	str r0, [r5]
	ldr r0, _02236E04 ; =0x00002678
	str r1, [r4, r0]
	b _02236F0A
_02236B9E:
	bl ov00_021EC60C
	bl ov00_021EC5B4
	cmp r0, #0
	beq _02236BD6
	add r0, r4, #0
	bl ov74_02236A2C
	cmp r0, #1
	bne _02236BCE
	ldr r0, _02236E04 ; =0x00002678
	ldr r0, [r4, r0]
	cmp r0, #1
	bne _02236BC8
	ldr r0, _02236E08 ; =ov74_0223E304
	mov r1, #3
	str r1, [r0, #8]
	ldr r0, _02236E0C ; =0x0000100D
	str r0, [r5]
	b _02236BD6
_02236BC8:
	ldr r0, _02236E10 ; =0x00001004
	str r0, [r5]
	b _02236BD6
_02236BCE:
	add r0, r4, #0
	bl ov74_022369D8
	str r0, [r5]
_02236BD6:
	ldr r0, _02236E14 ; =gSystem
	ldr r1, [r0, #0x48]
	mov r0, #2
	tst r0, r1
	beq _02236C3E
	ldr r0, _02236E04 ; =0x00002678
	mov r1, #1
	str r1, [r4, r0]
	b _02236F0A
_02236BE8:
	bl ov00_021ECD04
	cmp r0, #0
	bne _02236BFA
	add r0, r4, #0
	bl ov74_022369D8
	str r0, [r5]
	b _02236F0A
_02236BFA:
	ldr r0, _02236E18 ; =0x00001005
	str r0, [r5]
	b _02236F0A
_02236C00:
	bl ov00_021ECDC8
	cmp r0, #3
	bne _02236C14
	ldr r0, _02236E1C ; =0x00001006
	ldr r1, _02236E20 ; =ov00_021ECB40
	str r0, [r5]
	ldr r0, _02236DE8 ; =0x0000267C
	str r1, [r4, r0]
	b _02236C34
_02236C14:
	cmp r0, #4
	bne _02236C26
	add r0, r4, #0
	bl ov74_022369D8
	str r0, [r5]
	bl ov00_021EC8D8
	b _02236C34
_02236C26:
	cmp r0, #5
	bne _02236C34
	ldr r0, _02236E08 ; =ov74_0223E304
	mov r1, #3
	str r1, [r0, #8]
	ldr r0, _02236E0C ; =0x0000100D
	str r0, [r5]
_02236C34:
	ldr r0, _02236E14 ; =gSystem
	ldr r1, [r0, #0x48]
	mov r0, #2
	tst r0, r1
	bne _02236C40
_02236C3E:
	b _02236F0A
_02236C40:
	bl ov00_021ECEC0
	b _02236F0A
_02236C46:
	ldr r0, _02236E24 ; =ov74_02236AAC
	ldr r1, _02236E28 ; =ov74_0223D038
	ldr r2, _02236E2C ; =ov74_0223D040
	bl ov00_021ED1F0
	cmp r0, #0
	bne _02236C5E
	add r0, r4, #0
	bl ov74_022369D8
	str r0, [r5]
	b _02236F0A
_02236C5E:
	ldr r2, _02236E30 ; =0x00001007
	add r0, r4, #0
	add r1, r5, #0
	bl ov74_02236A54
	b _02236F0A
_02236C6A:
	ldr r0, _02236E04 ; =0x00002678
	ldr r0, [r4, r0]
	cmp r0, #1
	bne _02236C82
	add r3, #0xc
	add r0, r4, #0
	mov r1, #3
	add r2, r5, #0
	str r3, [sp]
	bl ov74_02236A78
	b _02236F0A
_02236C82:
	ldr r0, _02236E34 ; =ov74_0223D054
	add r1, r0, #0
	add r2, r0, #0
	bl ov00_021ED354
	cmp r0, #0
	bne _02236C9A
	add r0, r4, #0
	bl ov74_022369D8
	str r0, [r5]
	b _02236F0A
_02236C9A:
	ldr r0, _02236E38 ; =0x00001008
	str r0, [r5]
	b _02236F0A
_02236CA0:
	ldr r0, _02236E3C ; =0x00001654
	add r0, r4, r0
	bl ov00_021ED388
	cmp r0, #0
	bne _02236CB6
	add r0, r4, #0
	bl ov74_022369D8
	str r0, [r5]
	b _02236F0A
_02236CB6:
	ldr r2, _02236E40 ; =0x00001009
	add r0, r4, #0
	add r1, r5, #0
	bl ov74_02236A54
	b _02236F0A
_02236CC2:
	ldr r0, _02236E3C ; =0x00001654
	ldr r0, [r4, r0]
	cmp r0, #1
	beq _02236CDA
	add r3, #0xd
	add r0, r4, #0
	mov r1, #2
	add r2, r5, #0
	str r3, [sp]
	bl ov74_02236A78
	b _02236F0A
_02236CDA:
	ldr r0, _02236E44 ; =ov74_0223E318
	mov r1, #0
	mov r2, #0xa
	bl ov00_021ED3AC
	cmp r0, #0
	bne _02236CF2
	add r0, r4, #0
	bl ov74_022369D8
	str r0, [r5]
	b _02236F0A
_02236CF2:
	ldr r2, _02236E48 ; =0x0000100A
	add r0, r4, #0
	add r1, r5, #0
	bl ov74_02236A54
	b _02236F0A
_02236CFE:
	ldr r1, _02236E4C ; =0x00001658
	ldr r0, _02236E44 ; =ov74_0223E318
	add r1, r4, r1
	add r2, r3, #0
	bl ov00_021ED3F4
	cmp r0, #0
	bne _02236D18
	add r0, r4, #0
	bl ov74_022369D8
	str r0, [r5]
	b _02236F0A
_02236D18:
	ldr r0, _02236E50 ; =0x0000100B
	mov r1, #0
	str r0, [r5]
	ldr r0, _02236E54 ; =0x00002664
	str r1, [r4, r0]
	b _02236F0A
_02236D24:
	ldr r0, _02236E08 ; =ov74_0223E304
	ldr r1, [r0]
	cmp r1, #0
	bne _02236D72
	ldr r0, _02236E14 ; =gSystem
	ldr r1, [r0, #0x48]
	mov r0, #2
	tst r0, r1
	beq _02236D46
	add r3, #0xc
	add r0, r4, #0
	mov r1, #3
	add r2, r5, #0
	str r3, [sp]
	bl ov74_02236A78
	b _02236F0A
_02236D46:
	ldr r1, _02236E58 ; =0x0000265C
	add r0, r4, r1
	add r1, r1, #4
	add r1, r4, r1
	bl ov00_021ED444
	cmp r0, #1
	bne _02236DD0
	ldr r1, _02236E58 ; =0x0000265C
	mov r0, #0x64
	ldr r2, [r4, r1]
	add r1, r1, #4
	ldr r1, [r4, r1]
	mul r0, r2
	bl _u32_div_f
	ldr r1, _02236E54 ; =0x00002664
	ldr r2, [r4, r1]
	cmp r2, r0
	beq _02236DD0
	str r0, [r4, r1]
	b _02236F0A
_02236D72:
	ldr r0, [r0, #0xc]
	cmp r0, #0
	beq _02236D82
	add r0, r4, #0
	bl ov74_022369D8
	str r0, [r5]
	b _02236F0A
_02236D82:
	ldr r0, _02236E04 ; =0x00002678
	ldr r0, [r4, r0]
	cmp r0, #0
	bne _02236D9A
	add r3, #0xd
	add r0, r4, #0
	mov r1, #1
	add r2, r5, #0
	str r3, [sp]
	bl ov74_02236A78
	b _02236F0A
_02236D9A:
	add r3, #0xd
	add r0, r4, #0
	mov r1, #3
	add r2, r5, #0
	str r3, [sp]
	bl ov74_02236A78
	b _02236F0A
_02236DAA:
	bl ov00_021ED428
	cmp r0, #0
	bne _02236DB8
	ldr r0, _02236E0C ; =0x0000100D
	str r0, [r5]
	b _02236F0A
_02236DB8:
	ldr r0, _02236DE8 ; =0x0000267C
	mov r1, #0
	str r1, [r4, r0]
	bl ov74_02236AC8
	ldr r0, _02236E08 ; =ov74_0223E304
	ldr r0, [r0, #8]
	pop {r3, r4, r5, pc}
_02236DC8:
	bl ov00_021EC938
	cmp r0, #1
	beq _02236DD2
_02236DD0:
	b _02236F0A
_02236DD2:
	ldr r0, _02236DE8 ; =0x0000267C
	mov r1, #0
	str r1, [r4, r0]
	bl ov74_02236AC8
	ldr r0, _02236E08 ; =ov74_0223E304
	ldr r0, [r0, #8]
	pop {r3, r4, r5, pc}
_02236DE2:
	ldr r0, _02236E08 ; =ov74_0223E304
	b _02236E5C
	nop
_02236DE8: .word 0x0000267C
_02236DEC: .word 0x00001001
_02236DF0: .word ov74_022369A8
_02236DF4: .word ov74_022369C8
_02236DF8: .word 0x00001002
_02236DFC: .word 0x000015E8
_02236E00: .word 0x00001003
_02236E04: .word 0x00002678
_02236E08: .word ov74_0223E304
_02236E0C: .word 0x0000100D
_02236E10: .word 0x00001004
_02236E14: .word gSystem
_02236E18: .word 0x00001005
_02236E1C: .word 0x00001006
_02236E20: .word ov00_021ECB40
_02236E24: .word ov74_02236AAC
_02236E28: .word ov74_0223D038
_02236E2C: .word ov74_0223D040
_02236E30: .word 0x00001007
_02236E34: .word ov74_0223D054
_02236E38: .word 0x00001008
_02236E3C: .word 0x00001654
_02236E40: .word 0x00001009
_02236E44: .word ov74_0223E318
_02236E48: .word 0x0000100A
_02236E4C: .word 0x00001658
_02236E50: .word 0x0000100B
_02236E54: .word 0x00002664
_02236E58: .word 0x0000265C
_02236E5C:
	ldr r0, [r0, #0x10]
	cmp r0, #1
	bne _02236F0A
	add r0, r4, #0
	bl ov74_0222ACD8
	ldr r0, _02236F10 ; =0x00002674
	ldr r0, [r4, r0]
	sub r0, r0, #5
	cmp r0, #1
	bhi _02236E84
	ldr r3, _02236F14 ; =0x00001010
	add r0, r4, #0
	str r3, [sp]
	mov r1, #3
	add r2, r5, #0
	add r3, r3, #1
	bl ov74_02236A78
	b _02236F0A
_02236E84:
	ldr r0, _02236F18 ; =0x00001011
	str r0, [r5]
	b _02236F0A
_02236E8A:
	bl ov00_021EC8D8
	ldr r0, _02236F14 ; =0x00001010
	str r0, [r5]
	b _02236F0A
_02236E94:
	ldr r0, _02236F1C ; =gSystem
	ldr r1, [r0, #0x48]
	mov r0, #1
	tst r0, r1
	beq _02236F0A
	ldr r0, _02236F20 ; =0x0000267C
	mov r1, #0
	str r1, [r4, r0]
	bl ov00_021EC210
	bl ov74_02236AC8
	mov r0, #4
	pop {r3, r4, r5, pc}
_02236EB0:
	ldr r0, _02236F24 ; =ov74_0223E304
	ldr r1, [r0]
	cmp r1, #1
	bne _02236ED4
	mov r1, #0
	str r1, [r0]
	ldr r0, [r0, #0xc]
	cmp r0, #0
	beq _02236ECC
	add r0, r4, #0
	bl ov74_022369D8
	str r0, [r5]
	b _02236F0A
_02236ECC:
	ldr r0, _02236F28 ; =0x00001650
	ldr r0, [r4, r0]
	str r0, [r5]
	b _02236F0A
_02236ED4:
	ldr r0, _02236F1C ; =gSystem
	ldr r1, [r0, #0x48]
	mov r0, #2
	tst r0, r1
	beq _02236F0A
	ldr r0, _02236F2C ; =0x00002678
	mov r1, #1
	str r1, [r4, r0]
	b _02236F0A
_02236EE6:
	ldr r0, _02236F24 ; =ov74_0223E304
	ldr r1, [r0, #4]
	cmp r1, #1
	bne _02236EFA
	mov r1, #0
	str r1, [r0, #4]
	ldr r0, _02236F28 ; =0x00001650
	ldr r0, [r4, r0]
	str r0, [r5]
	b _02236F0A
_02236EFA:
	ldr r0, _02236F1C ; =gSystem
	ldr r1, [r0, #0x48]
	mov r0, #2
	tst r0, r1
	beq _02236F0A
	ldr r0, _02236F2C ; =0x00002678
	mov r1, #1
	str r1, [r4, r0]
_02236F0A:
	mov r0, #0
	pop {r3, r4, r5, pc}
	nop
_02236F10: .word 0x00002674
_02236F14: .word 0x00001010
_02236F18: .word 0x00001011
_02236F1C: .word gSystem
_02236F20: .word 0x0000267C
_02236F24: .word ov74_0223E304
_02236F28: .word 0x00001650
_02236F2C: .word 0x00002678
	thumb_func_end ov74_02236AE0
