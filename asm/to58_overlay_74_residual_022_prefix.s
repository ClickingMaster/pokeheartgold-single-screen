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
