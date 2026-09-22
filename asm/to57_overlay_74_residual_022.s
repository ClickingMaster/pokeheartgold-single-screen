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



	thumb_func_start ov74_02233F8C
ov74_02233F8C: ; 0x02233F8C
	push {r4, r5, r6, lr}
	add r4, r0, #0
	add r0, r1, #0
	mov r1, #0x18
	add r5, r2, #0
	mov r6, #0
	bl _u32_div_f
	cmp r1, #0x17
	bhi _02233FE2
	add r0, r1, r1
	add r0, pc
	ldrh r0, [r0, #6]
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	add pc, r0
_02233FAC: ; jump table
	.short _02233FDC - _02233FAC - 2 ; case 0
	.short _0223400E - _02233FAC - 2 ; case 1
	.short _0223403E - _02233FAC - 2 ; case 2
	.short _0223406E - _02233FAC - 2 ; case 3
	.short _0223409E - _02233FAC - 2 ; case 4
	.short _022340CE - _02233FAC - 2 ; case 5
	.short _022340FE - _02233FAC - 2 ; case 6
	.short _02234130 - _02233FAC - 2 ; case 7
	.short _02234160 - _02233FAC - 2 ; case 8
	.short _02234190 - _02233FAC - 2 ; case 9
	.short _022341C0 - _02233FAC - 2 ; case 10
	.short _022341F0 - _02233FAC - 2 ; case 11
	.short _02234220 - _02233FAC - 2 ; case 12
	.short _02234252 - _02233FAC - 2 ; case 13
	.short _02234282 - _02233FAC - 2 ; case 14
	.short _022342B2 - _02233FAC - 2 ; case 15
	.short _022342E2 - _02233FAC - 2 ; case 16
	.short _02234312 - _02233FAC - 2 ; case 17
	.short _02234342 - _02233FAC - 2 ; case 18
	.short _02234374 - _02233FAC - 2 ; case 19
	.short _022343A4 - _02233FAC - 2 ; case 20
	.short _022343D4 - _02233FAC - 2 ; case 21
	.short _02234404 - _02233FAC - 2 ; case 22
	.short _02234434 - _02233FAC - 2 ; case 23
_02233FDC:
	add r4, #0x20
	cmp r5, #3
	bls _02233FE4
_02233FE2:
	b _02234462
_02233FE4:
	add r0, r5, r5
	add r0, pc
	ldrh r0, [r0, #6]
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	add pc, r0
_02233FF0: ; jump table
	.short _02233FF8 - _02233FF0 - 2 ; case 0
	.short _02233FFC - _02233FF0 - 2 ; case 1
	.short _02234002 - _02233FF0 - 2 ; case 2
	.short _02234008 - _02233FF0 - 2 ; case 3
_02233FF8:
	add r6, r4, #0
	b _02234462
_02233FFC:
	add r6, r4, #0
	add r6, #0xc
	b _02234462
_02234002:
	add r6, r4, #0
	add r6, #0x18
	b _02234462
_02234008:
	add r6, r4, #0
	add r6, #0x24
	b _02234462
_0223400E:
	add r4, #0x20
	cmp r5, #3
	bhi _02234104
	add r0, r5, r5
	add r0, pc
	ldrh r0, [r0, #6]
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	add pc, r0
_02234020: ; jump table
	.short _02234028 - _02234020 - 2 ; case 0
	.short _0223402C - _02234020 - 2 ; case 1
	.short _02234032 - _02234020 - 2 ; case 2
	.short _02234038 - _02234020 - 2 ; case 3
_02234028:
	add r6, r4, #0
	b _02234462
_0223402C:
	add r6, r4, #0
	add r6, #0xc
	b _02234462
_02234032:
	add r6, r4, #0
	add r6, #0x24
	b _02234462
_02234038:
	add r6, r4, #0
	add r6, #0x18
	b _02234462
_0223403E:
	add r4, #0x20
	cmp r5, #3
	bhi _02234104
	add r0, r5, r5
	add r0, pc
	ldrh r0, [r0, #6]
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	add pc, r0
_02234050: ; jump table
	.short _02234058 - _02234050 - 2 ; case 0
	.short _0223405C - _02234050 - 2 ; case 1
	.short _02234062 - _02234050 - 2 ; case 2
	.short _02234068 - _02234050 - 2 ; case 3
_02234058:
	add r6, r4, #0
	b _02234462
_0223405C:
	add r6, r4, #0
	add r6, #0x18
	b _02234462
_02234062:
	add r6, r4, #0
	add r6, #0xc
	b _02234462
_02234068:
	add r6, r4, #0
	add r6, #0x24
	b _02234462
_0223406E:
	add r4, #0x20
	cmp r5, #3
	bhi _02234104
	add r0, r5, r5
	add r0, pc
	ldrh r0, [r0, #6]
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	add pc, r0
_02234080: ; jump table
	.short _02234088 - _02234080 - 2 ; case 0
	.short _0223408C - _02234080 - 2 ; case 1
	.short _02234092 - _02234080 - 2 ; case 2
	.short _02234098 - _02234080 - 2 ; case 3
_02234088:
	add r6, r4, #0
	b _02234462
_0223408C:
	add r6, r4, #0
	add r6, #0x24
	b _02234462
_02234092:
	add r6, r4, #0
	add r6, #0xc
	b _02234462
_02234098:
	add r6, r4, #0
	add r6, #0x18
	b _02234462
_0223409E:
	add r4, #0x20
	cmp r5, #3
	bhi _02234104
	add r0, r5, r5
	add r0, pc
	ldrh r0, [r0, #6]
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	add pc, r0
_022340B0: ; jump table
	.short _022340B8 - _022340B0 - 2 ; case 0
	.short _022340BC - _022340B0 - 2 ; case 1
	.short _022340C2 - _022340B0 - 2 ; case 2
	.short _022340C8 - _022340B0 - 2 ; case 3
_022340B8:
	add r6, r4, #0
	b _02234462
_022340BC:
	add r6, r4, #0
	add r6, #0x18
	b _02234462
_022340C2:
	add r6, r4, #0
	add r6, #0x24
	b _02234462
_022340C8:
	add r6, r4, #0
	add r6, #0xc
	b _02234462
_022340CE:
	add r4, #0x20
	cmp r5, #3
	bhi _02234104
	add r0, r5, r5
	add r0, pc
	ldrh r0, [r0, #6]
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	add pc, r0
_022340E0: ; jump table
	.short _022340E8 - _022340E0 - 2 ; case 0
	.short _022340EC - _022340E0 - 2 ; case 1
	.short _022340F2 - _022340E0 - 2 ; case 2
	.short _022340F8 - _022340E0 - 2 ; case 3
_022340E8:
	add r6, r4, #0
	b _02234462
_022340EC:
	add r6, r4, #0
	add r6, #0x24
	b _02234462
_022340F2:
	add r6, r4, #0
	add r6, #0x18
	b _02234462
_022340F8:
	add r6, r4, #0
	add r6, #0xc
	b _02234462
_022340FE:
	add r4, #0x20
	cmp r5, #3
	bls _02234106
_02234104:
	b _02234462
_02234106:
	add r0, r5, r5
	add r0, pc
	ldrh r0, [r0, #6]
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	add pc, r0
_02234112: ; jump table
	.short _0223411A - _02234112 - 2 ; case 0
	.short _02234120 - _02234112 - 2 ; case 1
	.short _02234124 - _02234112 - 2 ; case 2
	.short _0223412A - _02234112 - 2 ; case 3
_0223411A:
	add r6, r4, #0
	add r6, #0xc
	b _02234462
_02234120:
	add r6, r4, #0
	b _02234462
_02234124:
	add r6, r4, #0
	add r6, #0x18
	b _02234462
_0223412A:
	add r6, r4, #0
	add r6, #0x24
	b _02234462
_02234130:
	add r4, #0x20
	cmp r5, #3
	bhi _02234226
	add r0, r5, r5
	add r0, pc
	ldrh r0, [r0, #6]
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	add pc, r0
_02234142: ; jump table
	.short _0223414A - _02234142 - 2 ; case 0
	.short _02234150 - _02234142 - 2 ; case 1
	.short _02234154 - _02234142 - 2 ; case 2
	.short _0223415A - _02234142 - 2 ; case 3
_0223414A:
	add r6, r4, #0
	add r6, #0xc
	b _02234462
_02234150:
	add r6, r4, #0
	b _02234462
_02234154:
	add r6, r4, #0
	add r6, #0x24
	b _02234462
_0223415A:
	add r6, r4, #0
	add r6, #0x18
	b _02234462
_02234160:
	add r4, #0x20
	cmp r5, #3
	bhi _02234226
	add r0, r5, r5
	add r0, pc
	ldrh r0, [r0, #6]
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	add pc, r0
_02234172: ; jump table
	.short _0223417A - _02234172 - 2 ; case 0
	.short _02234180 - _02234172 - 2 ; case 1
	.short _02234184 - _02234172 - 2 ; case 2
	.short _0223418A - _02234172 - 2 ; case 3
_0223417A:
	add r6, r4, #0
	add r6, #0x18
	b _02234462
_02234180:
	add r6, r4, #0
	b _02234462
_02234184:
	add r6, r4, #0
	add r6, #0xc
	b _02234462
_0223418A:
	add r6, r4, #0
	add r6, #0x24
	b _02234462
_02234190:
	add r4, #0x20
	cmp r5, #3
	bhi _02234226
	add r0, r5, r5
	add r0, pc
	ldrh r0, [r0, #6]
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	add pc, r0
_022341A2: ; jump table
	.short _022341AA - _022341A2 - 2 ; case 0
	.short _022341B0 - _022341A2 - 2 ; case 1
	.short _022341B4 - _022341A2 - 2 ; case 2
	.short _022341BA - _022341A2 - 2 ; case 3
_022341AA:
	add r6, r4, #0
	add r6, #0x24
	b _02234462
_022341B0:
	add r6, r4, #0
	b _02234462
_022341B4:
	add r6, r4, #0
	add r6, #0xc
	b _02234462
_022341BA:
	add r6, r4, #0
	add r6, #0x18
	b _02234462
_022341C0:
	add r4, #0x20
	cmp r5, #3
	bhi _02234226
	add r0, r5, r5
	add r0, pc
	ldrh r0, [r0, #6]
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	add pc, r0
_022341D2: ; jump table
	.short _022341DA - _022341D2 - 2 ; case 0
	.short _022341E0 - _022341D2 - 2 ; case 1
	.short _022341E4 - _022341D2 - 2 ; case 2
	.short _022341EA - _022341D2 - 2 ; case 3
_022341DA:
	add r6, r4, #0
	add r6, #0x18
	b _02234462
_022341E0:
	add r6, r4, #0
	b _02234462
_022341E4:
	add r6, r4, #0
	add r6, #0x24
	b _02234462
_022341EA:
	add r6, r4, #0
	add r6, #0xc
	b _02234462
_022341F0:
	add r4, #0x20
	cmp r5, #3
	bhi _02234226
	add r0, r5, r5
	add r0, pc
	ldrh r0, [r0, #6]
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	add pc, r0
_02234202: ; jump table
	.short _0223420A - _02234202 - 2 ; case 0
	.short _02234210 - _02234202 - 2 ; case 1
	.short _02234214 - _02234202 - 2 ; case 2
	.short _0223421A - _02234202 - 2 ; case 3
_0223420A:
	add r6, r4, #0
	add r6, #0x24
	b _02234462
_02234210:
	add r6, r4, #0
	b _02234462
_02234214:
	add r6, r4, #0
	add r6, #0x18
	b _02234462
_0223421A:
	add r6, r4, #0
	add r6, #0xc
	b _02234462
_02234220:
	add r4, #0x20
	cmp r5, #3
	bls _02234228
_02234226:
	b _02234462
_02234228:
	add r0, r5, r5
	add r0, pc
	ldrh r0, [r0, #6]
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	add pc, r0
_02234234: ; jump table
	.short _0223423C - _02234234 - 2 ; case 0
	.short _02234242 - _02234234 - 2 ; case 1
	.short _02234248 - _02234234 - 2 ; case 2
	.short _0223424C - _02234234 - 2 ; case 3
_0223423C:
	add r6, r4, #0
	add r6, #0xc
	b _02234462
_02234242:
	add r6, r4, #0
	add r6, #0x18
	b _02234462
_02234248:
	add r6, r4, #0
	b _02234462
_0223424C:
	add r6, r4, #0
	add r6, #0x24
	b _02234462
_02234252:
	add r4, #0x20
	cmp r5, #3
	bhi _02234348
	add r0, r5, r5
	add r0, pc
	ldrh r0, [r0, #6]
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	add pc, r0
_02234264: ; jump table
	.short _0223426C - _02234264 - 2 ; case 0
	.short _02234272 - _02234264 - 2 ; case 1
	.short _02234278 - _02234264 - 2 ; case 2
	.short _0223427C - _02234264 - 2 ; case 3
_0223426C:
	add r6, r4, #0
	add r6, #0xc
	b _02234462
_02234272:
	add r6, r4, #0
	add r6, #0x24
	b _02234462
_02234278:
	add r6, r4, #0
	b _02234462
_0223427C:
	add r6, r4, #0
	add r6, #0x18
	b _02234462
_02234282:
	add r4, #0x20
	cmp r5, #3
	bhi _02234348
	add r0, r5, r5
	add r0, pc
	ldrh r0, [r0, #6]
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	add pc, r0
_02234294: ; jump table
	.short _0223429C - _02234294 - 2 ; case 0
	.short _022342A2 - _02234294 - 2 ; case 1
	.short _022342A8 - _02234294 - 2 ; case 2
	.short _022342AC - _02234294 - 2 ; case 3
_0223429C:
	add r6, r4, #0
	add r6, #0x18
	b _02234462
_022342A2:
	add r6, r4, #0
	add r6, #0xc
	b _02234462
_022342A8:
	add r6, r4, #0
	b _02234462
_022342AC:
	add r6, r4, #0
	add r6, #0x24
	b _02234462
_022342B2:
	add r4, #0x20
	cmp r5, #3
	bhi _02234348
	add r0, r5, r5
	add r0, pc
	ldrh r0, [r0, #6]
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	add pc, r0
_022342C4: ; jump table
	.short _022342CC - _022342C4 - 2 ; case 0
	.short _022342D2 - _022342C4 - 2 ; case 1
	.short _022342D8 - _022342C4 - 2 ; case 2
	.short _022342DC - _022342C4 - 2 ; case 3
_022342CC:
	add r6, r4, #0
	add r6, #0x24
	b _02234462
_022342D2:
	add r6, r4, #0
	add r6, #0xc
	b _02234462
_022342D8:
	add r6, r4, #0
	b _02234462
_022342DC:
	add r6, r4, #0
	add r6, #0x18
	b _02234462
_022342E2:
	add r4, #0x20
	cmp r5, #3
	bhi _02234348
	add r0, r5, r5
	add r0, pc
	ldrh r0, [r0, #6]
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	add pc, r0
_022342F4: ; jump table
	.short _022342FC - _022342F4 - 2 ; case 0
	.short _02234302 - _022342F4 - 2 ; case 1
	.short _02234308 - _022342F4 - 2 ; case 2
	.short _0223430C - _022342F4 - 2 ; case 3
_022342FC:
	add r6, r4, #0
	add r6, #0x18
	b _02234462
_02234302:
	add r6, r4, #0
	add r6, #0x24
	b _02234462
_02234308:
	add r6, r4, #0
	b _02234462
_0223430C:
	add r6, r4, #0
	add r6, #0xc
	b _02234462
_02234312:
	add r4, #0x20
	cmp r5, #3
	bhi _02234348
	add r0, r5, r5
	add r0, pc
	ldrh r0, [r0, #6]
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	add pc, r0
_02234324: ; jump table
	.short _0223432C - _02234324 - 2 ; case 0
	.short _02234332 - _02234324 - 2 ; case 1
	.short _02234338 - _02234324 - 2 ; case 2
	.short _0223433C - _02234324 - 2 ; case 3
_0223432C:
	add r6, r4, #0
	add r6, #0x24
	b _02234462
_02234332:
	add r6, r4, #0
	add r6, #0x18
	b _02234462
_02234338:
	add r6, r4, #0
	b _02234462
_0223433C:
	add r6, r4, #0
	add r6, #0xc
	b _02234462
_02234342:
	add r4, #0x20
	cmp r5, #3
	bls _0223434A
_02234348:
	b _02234462
_0223434A:
	add r0, r5, r5
	add r0, pc
	ldrh r0, [r0, #6]
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	add pc, r0
_02234356: ; jump table
	.short _0223435E - _02234356 - 2 ; case 0
	.short _02234364 - _02234356 - 2 ; case 1
	.short _0223436A - _02234356 - 2 ; case 2
	.short _02234370 - _02234356 - 2 ; case 3
_0223435E:
	add r6, r4, #0
	add r6, #0xc
	b _02234462
_02234364:
	add r6, r4, #0
	add r6, #0x18
	b _02234462
_0223436A:
	add r6, r4, #0
	add r6, #0x24
	b _02234462
_02234370:
	add r6, r4, #0
	b _02234462
_02234374:
	add r4, #0x20
	cmp r5, #3
	bhi _02234462
	add r0, r5, r5
	add r0, pc
	ldrh r0, [r0, #6]
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	add pc, r0
_02234386: ; jump table
	.short _0223438E - _02234386 - 2 ; case 0
	.short _02234394 - _02234386 - 2 ; case 1
	.short _0223439A - _02234386 - 2 ; case 2
	.short _022343A0 - _02234386 - 2 ; case 3
_0223438E:
	add r6, r4, #0
	add r6, #0xc
	b _02234462
_02234394:
	add r6, r4, #0
	add r6, #0x24
	b _02234462
_0223439A:
	add r6, r4, #0
	add r6, #0x18
	b _02234462
_022343A0:
	add r6, r4, #0
	b _02234462
_022343A4:
	add r4, #0x20
	cmp r5, #3
	bhi _02234462
	add r0, r5, r5
	add r0, pc
	ldrh r0, [r0, #6]
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	add pc, r0
_022343B6: ; jump table
	.short _022343BE - _022343B6 - 2 ; case 0
	.short _022343C4 - _022343B6 - 2 ; case 1
	.short _022343CA - _022343B6 - 2 ; case 2
	.short _022343D0 - _022343B6 - 2 ; case 3
_022343BE:
	add r6, r4, #0
	add r6, #0x18
	b _02234462
_022343C4:
	add r6, r4, #0
	add r6, #0xc
	b _02234462
_022343CA:
	add r6, r4, #0
	add r6, #0x24
	b _02234462
_022343D0:
	add r6, r4, #0
	b _02234462
_022343D4:
	add r4, #0x20
	cmp r5, #3
	bhi _02234462
	add r0, r5, r5
	add r0, pc
	ldrh r0, [r0, #6]
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	add pc, r0
_022343E6: ; jump table
	.short _022343EE - _022343E6 - 2 ; case 0
	.short _022343F4 - _022343E6 - 2 ; case 1
	.short _022343FA - _022343E6 - 2 ; case 2
	.short _02234400 - _022343E6 - 2 ; case 3
_022343EE:
	add r6, r4, #0
	add r6, #0x24
	b _02234462
_022343F4:
	add r6, r4, #0
	add r6, #0xc
	b _02234462
_022343FA:
	add r6, r4, #0
	add r6, #0x18
	b _02234462
_02234400:
	add r6, r4, #0
	b _02234462
_02234404:
	add r4, #0x20
	cmp r5, #3
	bhi _02234462
	add r0, r5, r5
	add r0, pc
	ldrh r0, [r0, #6]
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	add pc, r0
_02234416: ; jump table
	.short _0223441E - _02234416 - 2 ; case 0
	.short _02234424 - _02234416 - 2 ; case 1
	.short _0223442A - _02234416 - 2 ; case 2
	.short _02234430 - _02234416 - 2 ; case 3
_0223441E:
	add r6, r4, #0
	add r6, #0x18
	b _02234462
_02234424:
	add r6, r4, #0
	add r6, #0x24
	b _02234462
_0223442A:
	add r6, r4, #0
	add r6, #0xc
	b _02234462
_02234430:
	add r6, r4, #0
	b _02234462
_02234434:
	add r4, #0x20
	cmp r5, #3
	bhi _02234462
	add r0, r5, r5
	add r0, pc
	ldrh r0, [r0, #6]
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	add pc, r0
_02234446: ; jump table
	.short _0223444E - _02234446 - 2 ; case 0
	.short _02234454 - _02234446 - 2 ; case 1
	.short _0223445A - _02234446 - 2 ; case 2
	.short _02234460 - _02234446 - 2 ; case 3
_0223444E:
	add r6, r4, #0
	add r6, #0x24
	b _02234462
_02234454:
	add r6, r4, #0
	add r6, #0x18
	b _02234462
_0223445A:
	add r6, r4, #0
	add r6, #0xc
	b _02234462
_02234460:
	add r6, r4, #0
_02234462:
	add r0, r6, #0
	pop {r4, r5, r6, pc}
	.balign 4, 0
	thumb_func_end ov74_02233F8C




	thumb_func_start ov74_02234468
ov74_02234468: ; 0x02234468
	push {r3, r4}
	add r4, r0, #0
	mov r3, #0
	add r4, #0x20
_02234470:
	ldr r2, [r4]
	ldr r1, [r0, #4]
	add r3, r3, #1
	eor r2, r1
	str r2, [r4]
	ldr r1, [r0]
	eor r1, r2
	stmia r4!, {r1}
	cmp r3, #0xc
	blo _02234470
	pop {r3, r4}
	bx lr
	thumb_func_end ov74_02234468




	thumb_func_start ov74_02234488
ov74_02234488: ; 0x02234488
	push {r3, r4}
	add r4, r0, #0
	mov r3, #0
	add r4, #0x20
_02234490:
	ldr r2, [r4]
	ldr r1, [r0]
	add r3, r3, #1
	eor r2, r1
	str r2, [r4]
	ldr r1, [r0, #4]
	eor r1, r2
	stmia r4!, {r1}
	cmp r3, #0xc
	blo _02234490
	pop {r3, r4}
	bx lr
	thumb_func_end ov74_02234488




	thumb_func_start ov74_022344A8
ov74_022344A8: ; 0x022344A8
	push {r3, r4, r5, r6, r7, lr}
	add r1, r0, #0
	mov r4, #0
	ldr r1, [r1]
	str r0, [sp]
	add r2, r4, #0
	bl ov74_02233F8C
	add r5, r0, #0
	ldr r0, [sp]
	mov r2, #1
	add r1, r0, #0
	ldr r1, [r1]
	bl ov74_02233F8C
	add r6, r0, #0
	ldr r0, [sp]
	mov r2, #2
	add r1, r0, #0
	ldr r1, [r1]
	bl ov74_02233F8C
	add r7, r0, #0
	ldr r0, [sp]
	mov r2, #3
	add r1, r0, #0
	ldr r1, [r1]
	bl ov74_02233F8C
	add r1, r4, #0
_022344E4:
	ldrh r2, [r5]
	add r1, r1, #1
	add r5, r5, #2
	add r2, r4, r2
	lsl r2, r2, #0x10
	lsr r4, r2, #0x10
	cmp r1, #6
	blt _022344E4
	mov r2, #0
_022344F6:
	ldrh r1, [r6]
	add r2, r2, #1
	add r6, r6, #2
	add r1, r4, r1
	lsl r1, r1, #0x10
	lsr r4, r1, #0x10
	cmp r2, #6
	blt _022344F6
	mov r2, #0
_02234508:
	ldrh r1, [r7]
	add r2, r2, #1
	add r7, r7, #2
	add r1, r4, r1
	lsl r1, r1, #0x10
	lsr r4, r1, #0x10
	cmp r2, #6
	blt _02234508
	mov r2, #0
_0223451A:
	ldrh r1, [r0]
	add r2, r2, #1
	add r0, r0, #2
	add r1, r4, r1
	lsl r1, r1, #0x10
	lsr r4, r1, #0x10
	cmp r2, #6
	blt _0223451A
	add r0, r4, #0
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
	thumb_func_end ov74_022344A8




	thumb_func_start AGB_GetBoxMonData
AGB_GetBoxMonData: ; 0x02234530
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x10
	mov r4, #0
	add r6, r0, #0
	str r1, [sp]
	add r7, r2, #0
	add r5, r4, #0
	str r4, [sp, #0xc]
	str r4, [sp, #8]
	str r4, [sp, #4]
	cmp r1, #0xa
	ble _022345A6
	ldr r1, [r6]
	add r2, r4, #0
	bl ov74_02233F8C
	str r0, [sp, #0xc]
	ldr r1, [r6]
	add r0, r6, #0
	mov r2, #1
	bl ov74_02233F8C
	str r0, [sp, #8]
	ldr r1, [r6]
	add r0, r6, #0
	mov r2, #2
	bl ov74_02233F8C
	str r0, [sp, #4]
	ldr r1, [r6]
	add r0, r6, #0
	mov r2, #3
	bl ov74_02233F8C
	add r5, r0, #0
	add r0, r6, #0
	bl ov74_02234468
	add r0, r6, #0
	bl ov74_022344A8
	ldrh r1, [r6, #0x1c]
	cmp r0, r1
	beq _022345A6
	ldrb r0, [r6, #0x13]
	mov r1, #1
	bic r0, r1
	mov r1, #1
	orr r0, r1
	strb r0, [r6, #0x13]
	ldrb r1, [r6, #0x13]
	mov r0, #4
	orr r0, r1
	strb r0, [r6, #0x13]
	mov r0, #1
	ldr r1, [r5, #4]
	lsl r0, r0, #0x1e
	orr r0, r1
	str r0, [r5, #4]
_022345A6:
	ldr r0, [sp]
	cmp r0, #0x50
	bls _022345AE
	b _0223489C
_022345AE:
	add r0, r0, r0
	add r0, pc
	ldrh r0, [r0, #6]
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	add pc, r0
_022345BA: ; jump table
	.short _0223465C - _022345BA - 2 ; case 0
	.short _02234660 - _022345BA - 2 ; case 1
	.short _02234664 - _022345BA - 2 ; case 2
	.short _02234684 - _022345BA - 2 ; case 3
	.short _02234688 - _022345BA - 2 ; case 4
	.short _02234690 - _022345BA - 2 ; case 5
	.short _02234698 - _022345BA - 2 ; case 6
	.short _022346A0 - _022345BA - 2 ; case 7
	.short _022346B4 - _022345BA - 2 ; case 8
	.short _022346B8 - _022345BA - 2 ; case 9
	.short _022346BC - _022345BA - 2 ; case 10
	.short _022346C0 - _022345BA - 2 ; case 11
	.short _022346D4 - _022345BA - 2 ; case 12
	.short _022346EC - _022345BA - 2 ; case 13
	.short _022346EC - _022345BA - 2 ; case 14
	.short _022346EC - _022345BA - 2 ; case 15
	.short _022346EC - _022345BA - 2 ; case 16
	.short _022346F8 - _022345BA - 2 ; case 17
	.short _022346F8 - _022345BA - 2 ; case 18
	.short _022346F8 - _022345BA - 2 ; case 19
	.short _022346F8 - _022345BA - 2 ; case 20
	.short _022346E0 - _022345BA - 2 ; case 21
	.short _02234728 - _022345BA - 2 ; case 22
	.short _0223472E - _022345BA - 2 ; case 23
	.short _02234734 - _022345BA - 2 ; case 24
	.short _022346DA - _022345BA - 2 ; case 25
	.short _02234704 - _022345BA - 2 ; case 26
	.short _0223470A - _022345BA - 2 ; case 27
	.short _02234710 - _022345BA - 2 ; case 28
	.short _02234716 - _022345BA - 2 ; case 29
	.short _0223471C - _022345BA - 2 ; case 30
	.short _02234722 - _022345BA - 2 ; case 31
	.short _022346E6 - _022345BA - 2 ; case 32
	.short _0223473A - _022345BA - 2 ; case 33
	.short _0223474C - _022345BA - 2 ; case 34
	.short _02234754 - _022345BA - 2 ; case 35
	.short _0223475C - _022345BA - 2 ; case 36
	.short _02234764 - _022345BA - 2 ; case 37
	.short _0223476C - _022345BA - 2 ; case 38
	.short _0223477A - _022345BA - 2 ; case 39
	.short _02234782 - _022345BA - 2 ; case 40
	.short _0223478A - _022345BA - 2 ; case 41
	.short _02234792 - _022345BA - 2 ; case 42
	.short _0223479A - _022345BA - 2 ; case 43
	.short _022347A2 - _022345BA - 2 ; case 44
	.short _022347AA - _022345BA - 2 ; case 45
	.short _022347B2 - _022345BA - 2 ; case 46
	.short _02234740 - _022345BA - 2 ; case 47
	.short _02234746 - _022345BA - 2 ; case 48
	.short _02234774 - _022345BA - 2 ; case 49
	.short _022347B8 - _022345BA - 2 ; case 50
	.short _022347C0 - _022345BA - 2 ; case 51
	.short _022347C8 - _022345BA - 2 ; case 52
	.short _022347D0 - _022345BA - 2 ; case 53
	.short _022347D8 - _022345BA - 2 ; case 54
	.short _0223489C - _022345BA - 2 ; case 55
	.short _0223489C - _022345BA - 2 ; case 56
	.short _0223489C - _022345BA - 2 ; case 57
	.short _0223489C - _022345BA - 2 ; case 58
	.short _0223489C - _022345BA - 2 ; case 59
	.short _0223489C - _022345BA - 2 ; case 60
	.short _0223489C - _022345BA - 2 ; case 61
	.short _0223489C - _022345BA - 2 ; case 62
	.short _0223489C - _022345BA - 2 ; case 63
	.short _0223489C - _022345BA - 2 ; case 64
	.short _0223484E - _022345BA - 2 ; case 65
	.short _0223486C - _022345BA - 2 ; case 66
	.short _022347E0 - _022345BA - 2 ; case 67
	.short _022347E8 - _022345BA - 2 ; case 68
	.short _022347F0 - _022345BA - 2 ; case 69
	.short _022347F8 - _022345BA - 2 ; case 70
	.short _02234800 - _022345BA - 2 ; case 71
	.short _02234808 - _022345BA - 2 ; case 72
	.short _02234810 - _022345BA - 2 ; case 73
	.short _02234818 - _022345BA - 2 ; case 74
	.short _02234820 - _022345BA - 2 ; case 75
	.short _02234828 - _022345BA - 2 ; case 76
	.short _02234830 - _022345BA - 2 ; case 77
	.short _02234838 - _022345BA - 2 ; case 78
	.short _02234840 - _022345BA - 2 ; case 79
	.short _02234848 - _022345BA - 2 ; case 80
_0223465C:
	ldr r4, [r6]
	b _0223489C
_02234660:
	ldr r4, [r6, #4]
	b _0223489C
_02234664:
	ldrb r0, [r6, #0x13]
	lsl r0, r0, #0x1f
	lsr r0, r0, #0x1f
	beq _02234670
	mov r4, #0
	b _0223467E
_02234670:
	mov r4, #0
_02234672:
	add r0, r6, r4
	ldrb r0, [r0, #8]
	strb r0, [r7, r4]
	add r4, r4, #1
	cmp r4, #0xa
	blo _02234672
_0223467E:
	mov r0, #0xff
	strb r0, [r7, r4]
	b _0223489C
_02234684:
	ldrb r4, [r6, #0x12]
	b _0223489C
_02234688:
	ldrb r0, [r6, #0x13]
	lsl r0, r0, #0x1f
	lsr r4, r0, #0x1f
	b _0223489C
_02234690:
	ldrb r0, [r6, #0x13]
	lsl r0, r0, #0x1e
	lsr r4, r0, #0x1f
	b _0223489C
_02234698:
	ldrb r0, [r6, #0x13]
	lsl r0, r0, #0x1d
	lsr r4, r0, #0x1f
	b _0223489C
_022346A0:
	mov r4, #0
_022346A2:
	add r0, r6, r4
	ldrb r0, [r0, #0x14]
	strb r0, [r7, r4]
	add r4, r4, #1
	cmp r4, #7
	blo _022346A2
	mov r0, #0xff
	strb r0, [r7, r4]
	b _0223489C
_022346B4:
	ldrb r4, [r6, #0x1b]
	b _0223489C
_022346B8:
	ldrh r4, [r6, #0x1c]
	b _0223489C
_022346BC:
	ldrh r4, [r6, #0x1e]
	b _0223489C
_022346C0:
	ldrb r0, [r6, #0x13]
	lsl r0, r0, #0x1f
	lsr r0, r0, #0x1f
	beq _022346CE
	mov r4, #0x67
	lsl r4, r4, #2
	b _0223489C
_022346CE:
	ldr r0, [sp, #0xc]
	ldrh r4, [r0]
	b _0223489C
_022346D4:
	ldr r0, [sp, #0xc]
	ldrh r4, [r0, #2]
	b _0223489C
_022346DA:
	ldr r0, [sp, #0xc]
	ldr r4, [r0, #4]
	b _0223489C
_022346E0:
	ldr r0, [sp, #0xc]
	ldrb r4, [r0, #8]
	b _0223489C
_022346E6:
	ldr r0, [sp, #0xc]
	ldrb r4, [r0, #9]
	b _0223489C
_022346EC:
	ldr r0, [sp]
	sub r0, #0xd
	lsl r1, r0, #1
	ldr r0, [sp, #8]
	ldrh r4, [r0, r1]
	b _0223489C
_022346F8:
	ldr r1, [sp]
	ldr r0, [sp, #8]
	sub r1, #0x11
	add r0, r0, r1
	ldrb r4, [r0, #8]
	b _0223489C
_02234704:
	ldr r0, [sp, #4]
	ldrb r4, [r0]
	b _0223489C
_0223470A:
	ldr r0, [sp, #4]
	ldrb r4, [r0, #1]
	b _0223489C
_02234710:
	ldr r0, [sp, #4]
	ldrb r4, [r0, #2]
	b _0223489C
_02234716:
	ldr r0, [sp, #4]
	ldrb r4, [r0, #3]
	b _0223489C
_0223471C:
	ldr r0, [sp, #4]
	ldrb r4, [r0, #4]
	b _0223489C
_02234722:
	ldr r0, [sp, #4]
	ldrb r4, [r0, #5]
	b _0223489C
_02234728:
	ldr r0, [sp, #4]
	ldrb r4, [r0, #6]
	b _0223489C
_0223472E:
	ldr r0, [sp, #4]
	ldrb r4, [r0, #7]
	b _0223489C
_02234734:
	ldr r0, [sp, #4]
	ldrb r4, [r0, #8]
	b _0223489C
_0223473A:
	ldr r0, [sp, #4]
	ldrb r4, [r0, #9]
	b _0223489C
_02234740:
	ldr r0, [sp, #4]
	ldrb r4, [r0, #0xa]
	b _0223489C
_02234746:
	ldr r0, [sp, #4]
	ldrb r4, [r0, #0xb]
	b _0223489C
_0223474C:
	ldr r0, [r5]
	lsl r0, r0, #0x18
	lsr r4, r0, #0x18
	b _0223489C
_02234754:
	ldr r0, [r5]
	lsl r0, r0, #0x10
	lsr r4, r0, #0x18
	b _0223489C
_0223475C:
	ldr r0, [r5]
	lsl r0, r0, #9
	lsr r4, r0, #0x19
	b _0223489C
_02234764:
	ldr r0, [r5]
	lsl r0, r0, #5
	lsr r4, r0, #0x1c
	b _0223489C
_0223476C:
	ldr r0, [r5]
	lsl r0, r0, #1
	lsr r4, r0, #0x1c
	b _0223489C
_02234774:
	ldr r0, [r5]
	lsr r4, r0, #0x1f
	b _0223489C
_0223477A:
	ldr r0, [r5, #4]
	lsl r0, r0, #0x1b
	lsr r4, r0, #0x1b
	b _0223489C
_02234782:
	ldr r0, [r5, #4]
	lsl r0, r0, #0x16
	lsr r4, r0, #0x1b
	b _0223489C
_0223478A:
	ldr r0, [r5, #4]
	lsl r0, r0, #0x11
	lsr r4, r0, #0x1b
	b _0223489C
_02234792:
	ldr r0, [r5, #4]
	lsl r0, r0, #0xc
	lsr r4, r0, #0x1b
	b _0223489C
_0223479A:
	ldr r0, [r5, #4]
	lsl r0, r0, #7
	lsr r4, r0, #0x1b
	b _0223489C
_022347A2:
	ldr r0, [r5, #4]
	lsl r0, r0, #2
	lsr r4, r0, #0x1b
	b _0223489C
_022347AA:
	ldr r0, [r5, #4]
	lsl r0, r0, #1
	lsr r4, r0, #0x1f
	b _0223489C
_022347B2:
	ldr r0, [r5, #4]
	lsr r4, r0, #0x1f
	b _0223489C
_022347B8:
	ldr r0, [r5, #8]
	lsl r0, r0, #0x1d
	lsr r4, r0, #0x1d
	b _0223489C
_022347C0:
	ldr r0, [r5, #8]
	lsl r0, r0, #0x1a
	lsr r4, r0, #0x1d
	b _0223489C
_022347C8:
	ldr r0, [r5, #8]
	lsl r0, r0, #0x17
	lsr r4, r0, #0x1d
	b _0223489C
_022347D0:
	ldr r0, [r5, #8]
	lsl r0, r0, #0x14
	lsr r4, r0, #0x1d
	b _0223489C
_022347D8:
	ldr r0, [r5, #8]
	lsl r0, r0, #0x11
	lsr r4, r0, #0x1d
	b _0223489C
_022347E0:
	ldr r0, [r5, #8]
	lsl r0, r0, #0x10
	lsr r4, r0, #0x1f
	b _0223489C
_022347E8:
	ldr r0, [r5, #8]
	lsl r0, r0, #0xf
	lsr r4, r0, #0x1f
	b _0223489C
_022347F0:
	ldr r0, [r5, #8]
	lsl r0, r0, #0xe
	lsr r4, r0, #0x1f
	b _0223489C
_022347F8:
	ldr r0, [r5, #8]
	lsl r0, r0, #0xd
	lsr r4, r0, #0x1f
	b _0223489C
_02234800:
	ldr r0, [r5, #8]
	lsl r0, r0, #0xc
	lsr r4, r0, #0x1f
	b _0223489C
_02234808:
	ldr r0, [r5, #8]
	lsl r0, r0, #0xb
	lsr r4, r0, #0x1f
	b _0223489C
_02234810:
	ldr r0, [r5, #8]
	lsl r0, r0, #0xa
	lsr r4, r0, #0x1f
	b _0223489C
_02234818:
	ldr r0, [r5, #8]
	lsl r0, r0, #9
	lsr r4, r0, #0x1f
	b _0223489C
_02234820:
	ldr r0, [r5, #8]
	lsl r0, r0, #8
	lsr r4, r0, #0x1f
	b _0223489C
_02234828:
	ldr r0, [r5, #8]
	lsl r0, r0, #7
	lsr r4, r0, #0x1f
	b _0223489C
_02234830:
	ldr r0, [r5, #8]
	lsl r0, r0, #6
	lsr r4, r0, #0x1f
	b _0223489C
_02234838:
	ldr r0, [r5, #8]
	lsl r0, r0, #5
	lsr r4, r0, #0x1f
	b _0223489C
_02234840:
	ldr r0, [r5, #8]
	lsl r0, r0, #1
	lsr r4, r0, #0x1c
	b _0223489C
_02234848:
	ldr r0, [r5, #8]
	lsr r4, r0, #0x1f
	b _0223489C
_0223484E:
	ldr r0, [sp, #0xc]
	ldrh r4, [r0]
	cmp r4, #0
	beq _0223489C
	ldr r0, [r5, #4]
	lsl r0, r0, #1
	lsr r0, r0, #0x1f
	bne _02234866
	ldrb r0, [r6, #0x13]
	lsl r0, r0, #0x1f
	lsr r0, r0, #0x1f
	beq _0223489C
_02234866:
	mov r4, #0x67
	lsl r4, r4, #2
	b _0223489C
_0223486C:
	ldr r4, [r5, #4]
	lsl r0, r4, #2
	lsr r0, r0, #0x1b
	lsl r5, r0, #0x19
	lsl r0, r4, #7
	lsr r0, r0, #0x1b
	lsl r3, r0, #0x14
	lsl r0, r4, #0xc
	lsr r0, r0, #0x1b
	lsl r2, r0, #0xf
	lsl r0, r4, #0x11
	lsr r0, r0, #0x1b
	lsl r1, r0, #0xa
	lsl r0, r4, #0x1b
	lsl r4, r4, #0x16
	lsr r4, r4, #0x1b
	lsr r0, r0, #0x1b
	lsl r4, r4, #5
	orr r0, r4
	orr r0, r1
	orr r0, r2
	orr r0, r3
	add r4, r5, #0
	orr r4, r0
_0223489C:
	ldr r0, [sp]
	cmp r0, #0xa
	ble _022348A8
	add r0, r6, #0
	bl ov74_02234488
_022348A8:
	add r0, r4, #0
	add sp, #0x10
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
	thumb_func_end AGB_GetBoxMonData




	thumb_func_start ov74_022348B0
ov74_022348B0: ; 0x022348B0
	push {r3, r4, r5, r6, r7, lr}
	add r6, r1, #0
	add r4, r0, #0
	add r5, r2, #0
	mov r7, #0
	cmp r6, #0xa
	ble _02234924
	ldr r1, [r4]
	add r2, r7, #0
	bl ov74_02233F8C
	add r7, r0, #0
	ldr r1, [r4]
	add r0, r4, #0
	mov r2, #1
	bl ov74_02233F8C
	ldr r1, [r4]
	add r0, r4, #0
	mov r2, #2
	bl ov74_02233F8C
	ldr r1, [r4]
	add r0, r4, #0
	mov r2, #3
	bl ov74_02233F8C
	str r0, [sp]
	add r0, r4, #0
	bl ov74_02234468
	add r0, r4, #0
	bl ov74_022344A8
	ldrh r1, [r4, #0x1c]
	cmp r0, r1
	beq _02234924
	ldrb r1, [r4, #0x13]
	mov r0, #1
	bic r1, r0
	mov r0, #1
	orr r0, r1
	strb r0, [r4, #0x13]
	ldrb r1, [r4, #0x13]
	mov r0, #4
	orr r0, r1
	strb r0, [r4, #0x13]
	ldr r0, [sp]
	ldr r1, [r0, #4]
	mov r0, #1
	lsl r0, r0, #0x1e
	orr r1, r0
	ldr r0, [sp]
	str r1, [r0, #4]
	add r0, r4, #0
	bl ov74_02234488
	pop {r3, r4, r5, r6, r7, pc}
_02234924:
	cmp r6, #0xb
	bhi _022349C8
	add r0, r6, r6
	add r0, pc
	ldrh r0, [r0, #6]
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	add pc, r0
_02234934: ; jump table
	.short _022349C8 - _02234934 - 2 ; case 0
	.short _022349C8 - _02234934 - 2 ; case 1
	.short _0223494C - _02234934 - 2 ; case 2
	.short _0223495C - _02234934 - 2 ; case 3
	.short _02234962 - _02234934 - 2 ; case 4
	.short _02234974 - _02234934 - 2 ; case 5
	.short _02234986 - _02234934 - 2 ; case 6
	.short _02234998 - _02234934 - 2 ; case 7
	.short _022349C8 - _02234934 - 2 ; case 8
	.short _022349C8 - _02234934 - 2 ; case 9
	.short _022349C8 - _02234934 - 2 ; case 10
	.short _022349A8 - _02234934 - 2 ; case 11
_0223494C:
	mov r2, #0
_0223494E:
	ldrb r1, [r5, r2]
	add r0, r4, r2
	add r2, r2, #1
	strb r1, [r0, #8]
	cmp r2, #0xa
	blt _0223494E
	b _022349C8
_0223495C:
	ldrb r0, [r5]
	strb r0, [r4, #0x12]
	b _022349C8
_02234962:
	ldrb r0, [r4, #0x13]
	ldrb r2, [r5]
	mov r1, #1
	bic r0, r1
	mov r1, #1
	and r1, r2
	orr r0, r1
	strb r0, [r4, #0x13]
	b _022349C8
_02234974:
	ldrb r0, [r4, #0x13]
	mov r1, #2
	bic r0, r1
	ldrb r1, [r5]
	lsl r1, r1, #0x1f
	lsr r1, r1, #0x1e
	orr r0, r1
	strb r0, [r4, #0x13]
	b _022349C8
_02234986:
	ldrb r0, [r4, #0x13]
	mov r1, #4
	bic r0, r1
	ldrb r1, [r5]
	lsl r1, r1, #0x1f
	lsr r1, r1, #0x1d
	orr r0, r1
	strb r0, [r4, #0x13]
	b _022349C8
_02234998:
	mov r2, #0
_0223499A:
	ldrb r1, [r5, r2]
	add r0, r4, r2
	add r2, r2, #1
	strb r1, [r0, #0x14]
	cmp r2, #7
	blt _0223499A
	b _022349C8
_022349A8:
	ldrb r0, [r5, #1]
	ldrb r1, [r5]
	lsl r0, r0, #8
	add r0, r1, r0
	strh r0, [r7]
	ldrh r0, [r7]
	ldrb r1, [r4, #0x13]
	cmp r0, #0
	beq _022349C2
	mov r0, #2
	orr r0, r1
	strb r0, [r4, #0x13]
	b _022349C8
_022349C2:
	mov r0, #2
	bic r1, r0
	strb r1, [r4, #0x13]
_022349C8:
	cmp r6, #0xa
	ble _022349DA
	add r0, r4, #0
	bl ov74_022344A8
	strh r0, [r4, #0x1c]
	add r0, r4, #0
	bl ov74_02234488
_022349DA:
	pop {r3, r4, r5, r6, r7, pc}
	thumb_func_end ov74_022348B0




	thumb_func_start TranslateAgbSpecies
TranslateAgbSpecies: ; 0x022349DC
	ldr r1, _02234A00 ; =0x00000115
	cmp r0, r1
	blt _022349FC
	ldr r3, _02234A04 ; =ov74_0223CC5C
	mov r2, #0
_022349E6:
	ldrh r1, [r3]
	cmp r0, r1
	bne _022349F4
	ldr r0, _02234A08 ; =ov74_0223CC5E
	lsl r1, r2, #2
	ldrh r0, [r0, r1]
	bx lr
_022349F4:
	add r2, r2, #1
	add r3, r3, #4
	cmp r2, #0x90
	blo _022349E6
_022349FC:
	bx lr
	nop
_02234A00: .word 0x00000115
_02234A04: .word ov74_0223CC5C
_02234A08: .word ov74_0223CC5E
	thumb_func_end TranslateAgbSpecies
