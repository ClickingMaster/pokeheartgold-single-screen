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



	thumb_func_start ov74_02231764
ov74_02231764: ; 0x02231764
	cmp r0, #0
	bgt _0223176C
	mov r0, #1
	bx lr
_0223176C:
	cmp r0, #0xfb
	bgt _02231774
	mov r0, #0
	bx lr
_02231774:
	ldr r1, _0223178C ; =0x00000115
	cmp r0, r1
	bge _0223177E
	mov r0, #1
	bx lr
_0223177E:
	add r1, #0x86
	cmp r0, r1
	ble _02231788
	mov r0, #1
	bx lr
_02231788:
	mov r0, #0
	bx lr
	.balign 4, 0
_0223178C: .word 0x00000115
	thumb_func_end ov74_02231764




	thumb_func_start ov74_02231790
ov74_02231790: ; 0x02231790
	push {r4, lr}
	add r4, r0, #0
	bl PmAgbCartridge_GetVersionInternal
	cmp r0, #4
	bhi _022317D0
	add r0, r0, r0
	add r0, pc
	ldrh r0, [r0, #6]
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	add pc, r0
_022317A8: ; jump table
	.short _022317B2 - _022317A8 - 2 ; case 0
	.short _022317B8 - _022317A8 - 2 ; case 1
	.short _022317BE - _022317A8 - 2 ; case 2
	.short _022317C4 - _022317A8 - 2 ; case 3
	.short _022317CA - _022317A8 - 2 ; case 4
_022317B2:
	mov r0, #VERSION_RUBY
	str r0, [r4]
	pop {r4, pc}
_022317B8:
	mov r0, #VERSION_SAPPHIRE
	str r0, [r4]
	pop {r4, pc}
_022317BE:
	mov r0, #VERSION_LEAFGREEN
	str r0, [r4]
	pop {r4, pc}
_022317C4:
	mov r0, #VERSION_FIRERED
	str r0, [r4]
	pop {r4, pc}
_022317CA:
	mov r0, #VERSION_EMERALD
	str r0, [r4]
	pop {r4, pc}
_022317D0:
	mov r0, #0
	mvn r0, r0
	str r0, [r4]
	pop {r4, pc}
	thumb_func_end ov74_02231790




	thumb_func_start ov74_022317D8
ov74_022317D8: ; 0x022317D8
	push {r3, r4, r5, lr}
	sub sp, #0x10
	ldr r1, _0223192C ; =0x0000E890
	add r5, r0, #0
	add r4, r5, r1
	ldr r1, [r4]
	cmp r1, #9
	bls _022317EA
	b _02231924
_022317EA:
	add r1, r1, r1
	add r1, pc
	ldrh r1, [r1, #6]
	lsl r1, r1, #0x10
	asr r1, r1, #0x10
	add pc, r1
_022317F6: ; jump table
	.short _0223180A - _022317F6 - 2 ; case 0
	.short _0223182A - _022317F6 - 2 ; case 1
	.short _02231842 - _022317F6 - 2 ; case 2
	.short _02231852 - _022317F6 - 2 ; case 3
	.short _02231862 - _022317F6 - 2 ; case 4
	.short _0223187E - _022317F6 - 2 ; case 5
	.short _02231892 - _022317F6 - 2 ; case 6
	.short _022318AE - _022317F6 - 2 ; case 7
	.short _022318DA - _022317F6 - 2 ; case 8
	.short _02231900 - _022317F6 - 2 ; case 9
_0223180A:
	bl ov74_0223195C
	ldr r0, [r5, #0x10]
	bl Save_MigratedPokemon_Get
	add r5, r0, #0
	bl ov74_02233F68
	add r1, r0, #0
	add r0, r5, #0
	bl MigratedPokemon_RecordMigration
	ldr r0, [r4]
	add r0, r0, #1
	str r0, [r4]
	b _02231924
_0223182A:
	bl OS_GetTick
	mov r2, #0x78
	mov r3, #0
	bl _ull_mod
	add r0, r0, #1
	str r0, [r4, #4]
	ldr r0, [r4]
	add r0, r0, #1
	str r0, [r4]
	b _02231924
_02231842:
	ldr r0, [r4, #4]
	sub r0, r0, #1
	str r0, [r4, #4]
	bne _02231924
	ldr r0, [r4]
	add r0, r0, #1
	str r0, [r4]
	b _02231924
_02231852:
	ldr r0, [r5, #0x10]
	mov r1, #2
	bl Save_PrepareForAsyncWrite
	ldr r0, [r4]
	add r0, r0, #1
	str r0, [r4]
	b _02231924
_02231862:
	ldr r0, [r5, #0x10]
	bl Save_WriteFileAsync
	cmp r0, #3
	bne _02231872
	add sp, #0x10
	mov r0, #0xc
	pop {r3, r4, r5, pc}
_02231872:
	cmp r0, #1
	bne _02231924
	ldr r0, [r4]
	add r0, r0, #1
	str r0, [r4]
	b _02231924
_0223187E:
	mov r0, #0
	add r1, r0, #0
	add r2, sp, #0
	mov r3, #0x10
	bl CTRDG_ReadAgbFlash
	ldr r0, [r4]
	add r0, r0, #1
	str r0, [r4]
	b _02231924
_02231892:
	bl ov74_02233E8C
	cmp r0, #0
	bne _022318A6
	mov r0, #1
	bl Sys_ClearSleepDisableFlag
	add sp, #0x10
	mov r0, #0xc
	pop {r3, r4, r5, pc}
_022318A6:
	ldr r0, [r4]
	add r0, r0, #1
	str r0, [r4]
	b _02231924
_022318AE:
	bl ov74_02233F14
	cmp r0, #9
	beq _02231924
	bl ov74_02233F14
	cmp r0, #0xb
	bne _022318C6
	ldr r0, [r4]
	add r0, r0, #1
	str r0, [r4]
	b _02231924
_022318C6:
	bl ov74_02233ED4
	cmp r0, #8
	bne _02231924
	mov r0, #1
	bl Sys_ClearSleepDisableFlag
	add sp, #0x10
	mov r0, #0xc
	pop {r3, r4, r5, pc}
_022318DA:
	ldr r0, [r5, #0x10]
	bl Save_WriteFileAsync
	cmp r0, #3
	bne _022318EA
	add sp, #0x10
	mov r0, #0xc
	pop {r3, r4, r5, pc}
_022318EA:
	cmp r0, #2
	bne _022318DA
	bl ov74_02233ED4
	mov r0, #1
	bl Sys_SetSleepDisableFlag
	ldr r0, [r4]
	add r0, r0, #1
	str r0, [r4]
	b _02231924
_02231900:
	bl ov74_02233ED4
	cmp r0, #8
	bne _02231914
	mov r0, #1
	bl Sys_ClearSleepDisableFlag
	add sp, #0x10
	mov r0, #0xc
	pop {r3, r4, r5, pc}
_02231914:
	cmp r0, #0
	bne _02231924
	mov r0, #1
	bl Sys_ClearSleepDisableFlag
	add sp, #0x10
	mov r0, #0xb
	pop {r3, r4, r5, pc}
_02231924:
	mov r0, #0xa
	add sp, #0x10
	pop {r3, r4, r5, pc}
	nop
_0223192C: .word 0x0000E890
	thumb_func_end ov74_022317D8




	thumb_func_start ov74_02231930
ov74_02231930: ; 0x02231930
	push {r3, r4, r5, lr}
	add r5, r0, #0
	ldr r0, [r5, #0x10]
	bl Save_MigratedPokemon_Get
	add r4, r0, #0
	bl ov74_02233F68
	add r1, r0, #0
	add r0, r4, #0
	bl MigratedPokemon_RecordMigration
	mov r0, #4
	bl sub_0201A728
	ldr r0, [r5, #0x10]
	bl SaveGameNormal
	mov r0, #4
	bl sub_0201A738
	pop {r3, r4, r5, pc}
	thumb_func_end ov74_02231930




	thumb_func_start ov74_0223195C
ov74_0223195C: ; 0x0223195C
	push {r4, r5, r6, r7, lr}
	sub sp, #0xf4
	add r7, r0, #0
	ldr r0, [r7, #0x10]
	bl Save_MigratedPokemon_Get
	str r0, [sp]
	add r0, sp, #8
	bl Mon_GetBoxMon
	add r6, r0, #0
	mov r4, #0
	add r5, r7, #0
_02231976:
	ldr r0, _022319F4 ; =0x0000E880
	mov r1, #0x3d
	lsl r1, r1, #4
	ldr r0, [r7, r0]
	ldr r2, [r5, r1]
	mov r1, #0x96
	lsl r1, r1, #4
	add r0, r0, #4
	mul r1, r2
	add r0, r0, r1
	mov r1, #0xf3
	lsl r1, r1, #2
	ldr r2, [r5, r1]
	mov r1, #0x50
	mul r1, r2
	add r0, r0, r1
	add r1, r6, #0
	bl MigrateBoxMon
	ldr r0, [sp]
	add r1, r6, #0
	add r2, r4, #0
	bl MigratedPokemon_CopyBoxPokemonToSlot
	add r4, r4, #1
	add r5, #0xc
	cmp r4, #6
	blt _02231976
	mov r5, #0
	add r0, sp, #4
	strh r5, [r0]
	add r4, r7, #0
	sub r6, r5, #1
_022319B8:
	mov r0, #0xf3
	lsl r0, r0, #2
	ldr r2, [r4, r0]
	add r0, r0, #4
	ldr r1, [r4, r0]
	cmp r2, r6
	beq _022319E6
	cmp r1, #0xe
	beq _022319E6
	ldr r0, _022319F4 ; =0x0000E880
	mov r3, #0x96
	ldr r0, [r7, r0]
	lsl r3, r3, #4
	mul r3, r1
	add r0, r0, #4
	mov r1, #0x50
	mul r1, r2
	add r0, r0, r3
	add r0, r0, r1
	mov r1, #0xb
	add r2, sp, #4
	bl ov74_022348B0
_022319E6:
	add r5, r5, #1
	add r4, #0xc
	cmp r5, #6
	blt _022319B8
	add sp, #0xf4
	pop {r4, r5, r6, r7, pc}
	nop
_022319F4: .word 0x0000E880
	thumb_func_end ov74_0223195C




	thumb_func_start ov74_022319F8
ov74_022319F8: ; 0x022319F8
	push {r4, lr}
	add r4, r0, #0
	mov r0, #1
	tst r2, r0
	beq _02231A16
	mov r2, #0
	bl FontID_String_GetWidth
	ldr r1, [r4, #0x10]
	lsl r1, r1, #3
	sub r1, r1, r0
	lsr r0, r1, #0x1f
	add r0, r1, r0
	asr r0, r0, #1
	pop {r4, pc}
_02231A16:
	ldr r0, [r4, #0x18]
	pop {r4, pc}
	.balign 4, 0
	thumb_func_end ov74_022319F8




	thumb_func_start ov74_02231A1C
ov74_02231A1C: ; 0x02231A1C
	push {r4, r5, r6, r7, lr}
	sub sp, #0x24
	add r5, r1, #0
	ldr r7, [r5, #0x44]
	add r6, r0, #0
	add r4, r2, #0
	cmp r7, #0
	bne _02231A2E
	mov r7, #0xff
_02231A2E:
	ldr r1, [r5]
	ldr r0, [r1]
	cmp r0, #0
	bne _02231A6C
	ldr r0, [r5, #0xc]
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	str r0, [sp]
	ldr r0, [r5, #0x10]
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	str r0, [sp, #4]
	ldr r0, [r5, #0x14]
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	str r0, [sp, #8]
	mov r0, #0xf
	str r0, [sp, #0xc]
	ldr r0, [r5, #0x20]
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	str r0, [sp, #0x10]
	ldr r2, [r5, #0x24]
	ldr r3, [r5, #8]
	lsl r2, r2, #0x18
	lsl r3, r3, #0x18
	ldr r0, [r6, #0x20]
	lsr r2, r2, #0x18
	lsr r3, r3, #0x18
	bl AddWindowParameterized
_02231A6C:
	mov r0, #2
	tst r0, r4
	bne _02231A7E
	ldr r1, [r5, #0x30]
	ldr r0, [r5]
	lsl r1, r1, #0x18
	lsr r1, r1, #0x18
	bl FillWindowPixelBuffer
_02231A7E:
	mov r0, #0
	ldr r1, [r5, #0x34]
	mvn r0, r0
	cmp r1, r0
	beq _02231B0E
	ldr r2, _02231BB0 ; =0x0000012D
	mov r0, #1
	mov r1, #0x1b
	mov r3, #0x4c
	bl NewMsgDataFromNarc
	str r0, [sp, #0x18]
	ldr r0, [r5, #0x40]
	str r0, [sp, #0x14]
	cmp r0, #0
	bne _02231AA6
	mov r0, #0x4c
	bl MessageFormat_New
	str r0, [sp, #0x14]
_02231AA6:
	ldr r0, _02231BB4 ; =0x00012608
	ldr r0, [r6, r0]
	bl String_SetEmpty
	ldr r0, _02231BB4 ; =0x00012608
	ldr r1, [r5, #0x34]
	ldr r0, [r6, r0]
	str r0, [sp, #0x1c]
	ldr r0, [sp, #0x18]
	bl NewString_ReadMsgData
	str r0, [sp, #0x20]
	ldr r1, _02231BB4 ; =0x00012608
	ldr r0, [sp, #0x14]
	ldr r1, [r6, r1]
	ldr r2, [sp, #0x20]
	bl StringExpandPlaceholders
	ldr r0, [sp, #0x20]
	bl String_Delete
	ldr r1, [sp, #0x1c]
	add r0, r5, #0
	add r2, r4, #0
	bl ov74_022319F8
	add r3, r0, #0
	ldr r0, [r5, #0x1c]
	ldr r2, [sp, #0x1c]
	str r0, [sp]
	str r7, [sp, #4]
	ldr r0, [r5, #0x2c]
	str r0, [sp, #8]
	mov r0, #0
	str r0, [sp, #0xc]
	ldr r0, [r5]
	ldr r1, [r5, #0x28]
	bl AddTextPrinterParameterizedWithColor
	str r0, [r5, #0x48]
	ldr r0, [r5, #0x40]
	cmp r0, #0
	bne _02231B02
	ldr r0, [sp, #0x14]
	bl MessageFormat_Delete
_02231B02:
	ldr r0, [sp, #0x18]
	bl DestroyMsgData
	mov r0, #0
	mvn r0, r0
	str r0, [r5, #0x34]
_02231B0E:
	ldr r1, [r5, #0x38]
	cmp r1, #0
	beq _02231B4A
	ldr r0, _02231BB8 ; =0x0001260C
	mov r2, #0x40
	ldr r6, [r6, r0]
	add r0, r6, #0
	bl CopyU16ArrayToStringN
	add r0, r5, #0
	add r1, r6, #0
	add r2, r4, #0
	bl ov74_022319F8
	add r3, r0, #0
	ldr r0, [r5, #0x1c]
	add r2, r6, #0
	str r0, [sp]
	str r7, [sp, #4]
	ldr r0, [r5, #0x2c]
	str r0, [sp, #8]
	mov r0, #0
	str r0, [sp, #0xc]
	ldr r0, [r5]
	ldr r1, [r5, #0x28]
	bl AddTextPrinterParameterizedWithColor
	str r0, [r5, #0x48]
	mov r0, #0
	str r0, [r5, #0x38]
_02231B4A:
	ldr r1, [r5, #0x3c]
	cmp r1, #0
	beq _02231B78
	add r0, r5, #0
	add r2, r4, #0
	bl ov74_022319F8
	add r3, r0, #0
	ldr r0, [r5, #0x1c]
	str r0, [sp]
	str r7, [sp, #4]
	ldr r0, [r5, #0x2c]
	str r0, [sp, #8]
	mov r0, #0
	str r0, [sp, #0xc]
	ldr r0, [r5]
	ldr r1, [r5, #0x28]
	ldr r2, [r5, #0x3c]
	bl AddTextPrinterParameterizedWithColor
	str r0, [r5, #0x48]
	mov r0, #0
	str r0, [r5, #0x3c]
_02231B78:
	mov r0, #4
	tst r0, r4
	bne _02231B84
	ldr r0, [r5]
	bl CopyWindowToVram
_02231B84:
	mov r0, #8
	tst r0, r4
	beq _02231BAC
	mov r0, #0x10
	tst r0, r4
	ldr r0, [r5]
	beq _02231BA0
	ldr r2, _02231BBC ; =0x000003D2
	mov r1, #0
	mov r3, #0xd
	bl DrawFrameAndWindow2
	add sp, #0x24
	pop {r4, r5, r6, r7, pc}
_02231BA0:
	mov r2, #0x3f
	mov r1, #0
	lsl r2, r2, #4
	mov r3, #0xe
	bl DrawFrameAndWindow1
_02231BAC:
	add sp, #0x24
	pop {r4, r5, r6, r7, pc}
	.balign 4, 0
_02231BB0: .word 0x0000012D
_02231BB4: .word 0x00012608
_02231BB8: .word 0x0001260C
_02231BBC: .word 0x000003D2
	thumb_func_end ov74_02231A1C




	thumb_func_start ov74_02231BC0
ov74_02231BC0: ; 0x02231BC0
	push {r4, lr}
	sub sp, #0x10
	ldr r4, _02231BEC ; =_0223B4E8
	add r3, sp, #0
	add r2, r3, #0
	ldmia r4!, {r0, r1}
	stmia r3!, {r0, r1}
	ldmia r4!, {r0, r1}
	stmia r3!, {r0, r1}
	add r0, r2, #0
	bl ObjCharTransfer_Init
	mov r0, #0x14
	mov r1, #0x4c
	bl ObjPlttTransfer_Init
	bl ObjCharTransfer_ClearBuffers
	bl ObjPlttTransfer_Reset
	add sp, #0x10
	pop {r4, pc}
	.balign 4, 0
_02231BEC: .word _0223B4E8
	thumb_func_end ov74_02231BC0




	thumb_func_start ov74_02231BF0
ov74_02231BF0: ; 0x02231BF0
	push {r4, r5, r6, r7, lr}
	sub sp, #0x14
	add r6, r0, #0
	bl NNS_G2dInitOamManagerModule
	mov r0, #0
	str r0, [sp]
	mov r1, #0x7e
	str r1, [sp, #4]
	str r0, [sp, #8]
	mov r3, #0x20
	str r3, [sp, #0xc]
	mov r2, #0x4c
	str r2, [sp, #0x10]
	add r2, r0, #0
	bl OamManager_Create
	add r1, r6, #0
	mov r0, #0x50
	add r1, #0x2c
	mov r2, #0x4c
	bl G2dRenderer_Init
	str r0, [r6, #0x28]
	add r0, r6, #0
	mov r2, #1
	add r0, #0x2c
	mov r1, #0
	lsl r2, r2, #0x14
	bl G2dRenderer_SetSubSurfaceCoords
	mov r7, #0x55
	mov r4, #0
	add r5, r6, #0
	lsl r7, r7, #2
_02231C36:
	mov r0, #3
	add r1, r4, #0
	mov r2, #0x4c
	bl Create2DGfxResObjMan
	str r0, [r5, r7]
	add r4, r4, #1
	add r5, r5, #4
	cmp r4, #6
	blt _02231C36
	mov r0, #0
	str r0, [sp]
	mov r3, #1
	str r3, [sp, #4]
	mov r0, #0x4c
	str r0, [sp, #8]
	mov r0, #0x55
	lsl r0, r0, #2
	ldr r0, [r6, r0]
	mov r1, #0x71
	mov r2, #0x1e
	bl AddCharResObjFromNarc
	mov r1, #0x5b
	lsl r1, r1, #2
	str r0, [r6, r1]
	mov r3, #0
	str r3, [sp]
	mov r0, #1
	str r0, [sp, #4]
	mov r0, #4
	str r0, [sp, #8]
	mov r0, #0x4c
	sub r1, #0x14
	str r0, [sp, #0xc]
	ldr r0, [r6, r1]
	mov r1, #0x71
	mov r2, #0x1b
	bl AddPlttResObjFromNarc
	mov r1, #0x17
	lsl r1, r1, #4
	str r0, [r6, r1]
	mov r0, #0
	str r0, [sp]
	mov r0, #2
	str r0, [sp, #4]
	mov r0, #0x4c
	sub r1, #0x14
	str r0, [sp, #8]
	ldr r0, [r6, r1]
	mov r1, #0x71
	mov r2, #0x1d
	mov r3, #1
	bl AddCellOrAnimResObjFromNarc
	mov r1, #0x5d
	lsl r1, r1, #2
	str r0, [r6, r1]
	mov r0, #0
	str r0, [sp]
	mov r0, #3
	str r0, [sp, #4]
	mov r0, #0x4c
	sub r1, #0x14
	str r0, [sp, #8]
	ldr r0, [r6, r1]
	mov r1, #0x71
	mov r2, #0x1c
	mov r3, #1
	bl AddCellOrAnimResObjFromNarc
	mov r1, #0x5e
	lsl r1, r1, #2
	str r0, [r6, r1]
	sub r1, #0xc
	ldr r0, [r6, r1]
	bl SpriteTransfer_CreateCharTransferTask
	mov r0, #0x17
	lsl r0, r0, #4
	ldr r0, [r6, r0]
	bl SpriteTransfer_CreateExtPlttTransferTask
	bl sub_02074490
	add r1, r0, #0
	mov r0, #0
	mov r2, #1
	str r0, [sp]
	mov r0, #0x4c
	add r3, r2, #0
	str r0, [sp, #4]
	mov r0, #0x14
	add r3, #0xff
	bl GfGfxLoader_GXLoadPal
	add sp, #0x14
	pop {r4, r5, r6, r7, pc}
	thumb_func_end ov74_02231BF0




	thumb_func_start ov74_02231CFC
ov74_02231CFC: ; 0x02231CFC
	push {lr}
	sub sp, #0x2c
	mov r1, #0
	str r1, [sp]
	sub r2, r1, #1
	str r2, [sp, #4]
	str r2, [sp, #8]
	str r1, [sp, #0xc]
	mov r3, #0x55
	str r1, [sp, #0x10]
	lsl r3, r3, #2
	ldr r2, [r0, r3]
	str r2, [sp, #0x14]
	add r2, r3, #4
	ldr r2, [r0, r2]
	str r2, [sp, #0x18]
	add r2, r3, #0
	add r2, #8
	ldr r2, [r0, r2]
	str r2, [sp, #0x1c]
	add r2, r3, #0
	add r2, #0xc
	ldr r2, [r0, r2]
	add r3, #0x30
	str r2, [sp, #0x20]
	str r1, [sp, #0x24]
	add r0, r0, r3
	add r2, r1, #0
	add r3, r1, #0
	str r1, [sp, #0x28]
	bl CreateSpriteResourcesHeader
	mov r0, #0x10
	mov r1, #1
	bl GfGfx_EngineATogglePlanes
	add sp, #0x2c
	pop {pc}
	thumb_func_end ov74_02231CFC




	thumb_func_start ov74_02231D48
ov74_02231D48: ; 0x02231D48
	ldr r2, [r1, #0x28]
	str r2, [r0]
	mov r2, #0x61
	lsl r2, r2, #2
	add r1, r1, r2
	str r1, [r0, #4]
	mov r2, #0
	mov r1, #1
	str r2, [r0, #0x10]
	lsl r1, r1, #0xc
	str r1, [r0, #0x14]
	str r1, [r0, #0x18]
	str r1, [r0, #0x1c]
	strh r2, [r0, #0x20]
	mov r1, #0x14
	str r1, [r0, #0x24]
	str r3, [r0, #0x28]
	mov r1, #0x4c
	str r1, [r0, #0x2c]
	bx lr
	thumb_func_end ov74_02231D48




	thumb_func_start ov74_02231D70
ov74_02231D70: ; 0x02231D70
	ldr r3, _02231D8C ; =0x0000E880
	ldr r0, [r0, r3]
	add r3, r0, #4
	mov r0, #0x96
	lsl r0, r0, #4
	mul r0, r1
	add r1, r3, r0
	mov r0, #0x50
	mul r0, r2
	add r0, r1, r0
	ldr r3, _02231D90 ; =AGB_GetBoxMonData
	mov r1, #0xb
	mov r2, #0
	bx r3
	.balign 4, 0
_02231D8C: .word 0x0000E880
_02231D90: .word AGB_GetBoxMonData
	thumb_func_end ov74_02231D70




	thumb_func_start ov74_02231D94
ov74_02231D94: ; 0x02231D94
	ldr r3, _02231DB0 ; =0x0000E880
	ldr r0, [r0, r3]
	add r3, r0, #4
	mov r0, #0x96
	lsl r0, r0, #4
	mul r0, r1
	add r1, r3, r0
	mov r0, #0x50
	mul r0, r2
	add r0, r1, r0
	ldr r3, _02231DB4 ; =AGB_GetBoxMonData
	mov r1, #0x2d
	mov r2, #0
	bx r3
	.balign 4, 0
_02231DB0: .word 0x0000E880
_02231DB4: .word AGB_GetBoxMonData
	thumb_func_end ov74_02231D94




	thumb_func_start ov74_02231DB8
ov74_02231DB8: ; 0x02231DB8
	ldr r3, _02231DD4 ; =0x0000E880
	ldr r0, [r0, r3]
	add r3, r0, #4
	mov r0, #0x96
	lsl r0, r0, #4
	mul r0, r1
	add r1, r3, r0
	mov r0, #0x50
	mul r0, r2
	add r0, r1, r0
	mov r1, #0
	ldr r3, _02231DD8 ; =AGB_GetBoxMonData
	add r2, r1, #0
	bx r3
	.balign 4, 0
_02231DD4: .word 0x0000E880
_02231DD8: .word AGB_GetBoxMonData
	thumb_func_end ov74_02231DB8




	thumb_func_start ov74_02231DDC
ov74_02231DDC: ; 0x02231DDC
	push {r4, r5}
	ldr r5, [sp, #8]
	lsr r4, r5, #0x1f
	add r4, r5, r4
	asr r4, r4, #1
	sub r5, r2, r4
	strb r5, [r0]
	lsr r5, r3, #0x1f
	add r5, r3, r5
	asr r5, r5, #1
	sub r3, r1, r5
	strb r3, [r0, #2]
	add r2, r2, r4
	strb r2, [r0, #1]
	add r1, r1, r5
	strb r1, [r0, #3]
	pop {r4, r5}
	bx lr
	thumb_func_end ov74_02231DDC




	thumb_func_start ov74_02231E00
ov74_02231E00: ; 0x02231E00
	push {r3, r4, r5, r6, r7, lr}
	add r6, r1, #0
	mov r1, #1
	add r5, r0, #0
	add r0, r3, #0
	lsl r1, r1, #0xc
	add r7, r2, #0
	bl Heap_AllocAtEnd
	add r4, r0, #0
	beq _02231E34
	add r1, r5, #0
	add r2, r6, #0
	bl ReadWholeNarcMemberByIdPair
	add r0, r4, #0
	add r1, r7, #0
	bl NNS_G2dGetUnpackedBGCharacterData
	cmp r0, #0
	bne _02231E34
	add r0, r4, #0
	bl Heap_Free
	mov r0, #0
	pop {r3, r4, r5, r6, r7, pc}
_02231E34:
	add r0, r4, #0
	pop {r3, r4, r5, r6, r7, pc}
	thumb_func_end ov74_02231E00




	thumb_func_start ov74_02231E38
ov74_02231E38: ; 0x02231E38
	push {r4, r5, r6, lr}
	add r6, r0, #0
	add r5, r1, #0
	add r4, r2, #0
	add r0, r3, #0
	add r1, r6, #0
	bl NARC_ReadWholeMember
	add r0, r4, #0
	add r1, r5, #0
	bl NNS_G2dGetUnpackedBGCharacterData
	pop {r4, r5, r6, pc}
	.balign 4, 0
	thumb_func_end ov74_02231E38




	thumb_func_start ov74_02231E54
ov74_02231E54: ; 0x02231E54
	push {r4, lr}
	add r3, r1, #0
	mov r4, #0
	cmp r0, #0xc9
	beq _02231E66
	ldr r1, _02231EC0 ; =0x00000182
	cmp r0, r1
	beq _02231E90
	b _02231EBA
_02231E66:
	mov r4, #3
	lsl r0, r4, #8
	add r2, r3, #0
	and r0, r3
	lsr r1, r0, #6
	lsl r0, r4, #0x18
	and r2, r4
	and r0, r3
	lsl r4, r4, #0x10
	and r3, r4
	lsr r0, r0, #0x12
	lsr r3, r3, #0xc
	orr r0, r3
	orr r0, r1
	orr r0, r2
	mov r1, #0x1c
	bl _u32_div_f
	lsl r0, r1, #0x18
	lsr r4, r0, #0x18
	b _02231EBA
_02231E90:
	cmp r2, #5
	bhi _02231EAC
	add r0, r2, r2
	add r0, pc
	ldrh r0, [r0, #6]
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	add pc, r0
_02231EA0: ; jump table
	.short _02231EAC - _02231EA0 - 2 ; case 0
	.short _02231EAC - _02231EA0 - 2 ; case 1
	.short _02231EAC - _02231EA0 - 2 ; case 2
	.short _02231EB8 - _02231EA0 - 2 ; case 3
	.short _02231EB0 - _02231EA0 - 2 ; case 4
	.short _02231EB4 - _02231EA0 - 2 ; case 5
_02231EAC:
	mov r4, #0
	b _02231EBA
_02231EB0:
	mov r4, #1
	b _02231EBA
_02231EB4:
	mov r4, #2
	b _02231EBA
_02231EB8:
	mov r4, #3
_02231EBA:
	add r0, r4, #0
	pop {r4, pc}
	nop
_02231EC0: .word 0x00000182
	thumb_func_end ov74_02231E54




	thumb_func_start ov74_02231EC4
ov74_02231EC4: ; 0x02231EC4
	push {r3, r4, r5, r6, r7, lr}
	add r6, r2, #0
	add r7, r3, #0
	add r5, r1, #0
	bl TranslateAgbSpecies
	add r1, r6, #0
	add r2, r7, #0
	add r4, r0, #0
	bl ov74_02231E54
	add r7, r0, #0
	add r0, r4, #0
	add r1, r5, #0
	add r2, r7, #0
	bl GetMonIconNaixEx
	add r1, r0, #0
	mov r0, #0x14
	add r2, sp, #0
	mov r3, #0x4c
	bl ov74_02231E00
	add r6, r0, #0
	ldr r0, [sp]
	mov r1, #2
	ldr r0, [r0, #0x14]
	lsl r1, r1, #8
	bl DC_FlushRange
	ldr r1, [sp, #0x18]
	ldr r0, [sp]
	lsl r1, r1, #4
	add r1, #0x64
	mov r2, #2
	ldr r0, [r0, #0x14]
	lsl r1, r1, #5
	lsl r2, r2, #8
	bl GX_LoadOBJ
	add r0, r4, #0
	add r1, r7, #0
	add r2, r5, #0
	bl GetMonIconPaletteEx
	add r1, r0, #0
	ldr r0, [sp, #0x1c]
	add r1, #8
	bl Sprite_SetPaletteOverride
	add r0, r6, #0
	bl Heap_Free
	pop {r3, r4, r5, r6, r7, pc}
	thumb_func_end ov74_02231EC4




	thumb_func_start ov74_02231F30
ov74_02231F30: ; 0x02231F30
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #8
	add r7, r1, #0
	ldr r1, _02231FAC ; =ov74_0223D338
	str r2, [sp]
	ldr r2, [r1]
	mov r1, #0x83
	add r5, r3, #0
	lsl r1, r1, #2
	mul r1, r5
	add r4, r2, r1
	ldr r1, [sp, #0x20]
	add r6, r0, #0
	cmp r1, #0
	beq _02231FA4
	bl ov74_02231764
	cmp r0, #0
	bne _02231F60
	add r0, r6, #0
	bl TranslateAgbSpecies
	add r6, r0, #0
	b _02231F62
_02231F60:
	mov r6, #0
_02231F62:
	ldr r2, [sp]
	add r0, r6, #0
	add r1, r7, #0
	bl GetMonIconNaixEx
	ldr r2, [sp, #0x24]
	ldr r3, [sp, #0x28]
	add r1, sp, #4
	bl ov74_02231E38
	ldr r0, [sp, #4]
	add r1, r4, #0
	mov r2, #2
	ldr r0, [r0, #0x14]
	add r1, #0xc
	lsl r2, r2, #8
	bl MIi_CpuCopyFast
	lsl r0, r5, #4
	add r0, #0x64
	lsl r0, r0, #5
	str r0, [r4]
	ldr r0, [sp, #0x20]
	ldr r1, [sp]
	str r0, [r4, #8]
	add r0, r6, #0
	add r2, r7, #0
	bl GetMonIconPaletteEx
	add r0, #8
	add sp, #8
	str r0, [r4, #4]
	pop {r3, r4, r5, r6, r7, pc}
_02231FA4:
	mov r0, #0
	str r0, [r4, #8]
	add sp, #8
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
_02231FAC: .word ov74_0223D338
	thumb_func_end ov74_02231F30




	thumb_func_start ov74_02231FB0
ov74_02231FB0: ; 0x02231FB0
	push {r3, r4, r5, r6, r7, lr}
	ldr r0, _02231FF0 ; =ov74_0223D338
	mov r6, #2
	lsl r6, r6, #8
	add r7, r6, #0
	ldr r5, [r0]
	mov r4, #0
	add r7, #0xc
_02231FC0:
	ldr r0, [r5, #8]
	cmp r0, #0
	beq _02231FE4
	add r0, r5, #0
	add r0, #0xc
	add r1, r6, #0
	bl DC_FlushRange
	add r0, r5, #0
	ldr r1, [r5]
	add r0, #0xc
	add r2, r6, #0
	bl GX_LoadOBJ
	ldr r0, [r5, #8]
	ldr r1, [r5, #4]
	bl Sprite_SetPaletteOverride
_02231FE4:
	add r4, r4, #1
	add r5, r5, r7
	cmp r4, #0x1e
	blt _02231FC0
	pop {r3, r4, r5, r6, r7, pc}
	nop
_02231FF0: .word ov74_0223D338
	thumb_func_end ov74_02231FB0




	thumb_func_start ov74_02231FF4
ov74_02231FF4: ; 0x02231FF4
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x20
	add r6, r0, #0
	mov r0, #0x14
	mov r1, #0x4c
	bl NARC_New
	mov r1, #1
	str r0, [sp, #0x10]
	mov r0, #0x4c
	lsl r1, r1, #0xc
	bl Heap_AllocAtEnd
	mov r7, #0
	str r0, [sp, #0x14]
	str r7, [sp, #0xc]
	add r5, r6, #0
_02232016:
	ldr r0, _02232140 ; =0x0000E880
	ldr r1, _02232144 ; =0x0000E884
	ldr r0, [r6, r0]
	ldr r2, [r6, r1]
	mov r1, #0x96
	lsl r1, r1, #4
	mul r1, r2
	add r0, r0, #4
	add r1, r0, r1
	ldr r0, [sp, #0xc]
	mov r2, #0
	add r0, r1, r0
	mov r1, #5
	bl AGB_GetBoxMonData
	cmp r0, #0
	beq _022320DA
	ldr r1, _02232144 ; =0x0000E884
	add r0, r6, #0
	ldr r1, [r6, r1]
	add r2, r7, #0
	bl ov74_02231D70
	ldr r1, _02232144 ; =0x0000E884
	str r0, [sp, #0x1c]
	ldr r1, [r6, r1]
	add r0, r6, #0
	add r2, r7, #0
	bl ov74_02231D94
	ldr r1, _02232144 ; =0x0000E884
	str r0, [sp, #0x18]
	ldr r1, [r6, r1]
	add r0, r6, #0
	add r2, r7, #0
	bl ov74_02231DB8
	add r4, r0, #0
	ldr r0, [sp, #0x1c]
	bl TranslateAgbSpecies
	ldr r2, _02232148 ; =gSystem + 0x60
	add r1, r4, #0
	ldrb r2, [r2, #0xa]
	bl ov74_02231E54
	add r4, r0, #0
	mov r0, #0x6a
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	ldr r1, [sp, #0x18]
	str r0, [sp]
	ldr r0, [sp, #0x14]
	add r2, r4, #0
	str r0, [sp, #4]
	ldr r0, [sp, #0x10]
	add r3, r7, #0
	str r0, [sp, #8]
	ldr r0, [sp, #0x1c]
	bl ov74_02231F30
	mov r0, #0x6a
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	mov r1, #1
	bl Sprite_SetDrawFlag
	ldr r0, _02232140 ; =0x0000E880
	ldr r1, _02232144 ; =0x0000E884
	ldr r0, [r6, r0]
	ldr r2, [r6, r1]
	mov r1, #0x96
	lsl r1, r1, #4
	mul r1, r2
	add r0, r0, #4
	add r1, r0, r1
	ldr r0, [sp, #0xc]
	mov r2, #0
	add r0, r1, r0
	mov r1, #0xc
	bl AGB_GetBoxMonData
	cmp r0, #0
	beq _022320CC
	mov r0, #0x6b
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	mov r1, #1
	bl Sprite_SetDrawFlag
	b _0223210A
_022320CC:
	mov r0, #0x6b
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	mov r1, #0
	bl Sprite_SetDrawFlag
	b _0223210A
_022320DA:
	mov r0, #0
	str r0, [sp]
	ldr r0, [sp, #0x14]
	ldr r1, [sp, #0x18]
	str r0, [sp, #4]
	ldr r0, [sp, #0x10]
	add r2, r4, #0
	str r0, [sp, #8]
	ldr r0, [sp, #0x1c]
	add r3, r7, #0
	bl ov74_02231F30
	mov r0, #0x6a
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	mov r1, #0
	bl Sprite_SetDrawFlag
	mov r0, #0x6b
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	mov r1, #0
	bl Sprite_SetDrawFlag
_0223210A:
	ldr r0, [sp, #0xc]
	add r7, r7, #1
	add r0, #0x50
	add r5, #0xc
	str r0, [sp, #0xc]
	cmp r7, #0x1e
	bge _0223211A
	b _02232016
_0223211A:
	ldr r0, [sp, #0x14]
	bl Heap_Free
	ldr r0, [sp, #0x10]
	bl NARC_Delete
	ldr r1, _0223214C ; =ov74_02231FB0
	ldr r0, _02232150 ; =0x00012604
	str r1, [r6, r0]
	ldr r1, _02232144 ; =0x0000E884
	add r0, r6, #0
	ldr r1, [r6, r1]
	bl ov74_02232678
	add r0, r6, #0
	bl ov74_0223262C
	add sp, #0x20
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
_02232140: .word 0x0000E880
_02232144: .word 0x0000E884
_02232148: .word gSystem + 0x60
_0223214C: .word ov74_02231FB0
_02232150: .word 0x00012604
	thumb_func_end ov74_02231FF4




	thumb_func_start ov74_02232154
ov74_02232154: ; 0x02232154
	push {r4, r5, r6, r7, lr}
	sub sp, #0x44
	mov r2, #0x61
	add r5, r0, #0
	lsl r2, r2, #2
	add r0, sp, #0x14
	add r1, r5, #0
	add r2, r5, r2
	mov r3, #1
	bl ov74_02231D48
	mov r0, #0x28
	str r0, [sp, #8]
	mov r0, #0x31
	mov r4, #0
	lsl r0, r0, #4
	str r4, [sp, #0xc]
	add r7, r5, r0
_02232178:
	mov r0, #0
	str r0, [sp, #0x10]
	ldr r0, [sp, #8]
	mov r6, #0x1c
	lsl r0, r0, #0xc
	str r0, [sp, #4]
_02232184:
	mov r0, #0x1c
	str r0, [sp]
	ldr r2, [sp, #8]
	add r0, r7, #0
	add r1, r6, #0
	mov r3, #0x1c
	bl ov74_02231DDC
	lsl r0, r6, #0xc
	str r0, [sp, #0x1c]
	ldr r0, [sp, #4]
	str r0, [sp, #0x20]
	mov r0, #0x6a
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	cmp r0, #0
	bne _022321B2
	add r0, sp, #0x14
	bl Sprite_CreateAffine
	mov r1, #0x6a
	lsl r1, r1, #2
	str r0, [r5, r1]
_022321B2:
	mov r0, #0x6a
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	mov r1, #1
	bl Sprite_SetAnimActiveFlag
	mov r0, #0x6a
	lsl r0, r0, #2
	add r1, r4, #0
	ldr r0, [r5, r0]
	add r1, #0xa
	bl Sprite_SetAnimCtrlSeq
	mov r0, #0x6a
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	mov r1, #1
	bl Sprite_SetPriority
	mov r0, #0x6a
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	mov r1, #1
	bl Sprite_SetDrawFlag
	mov r0, #0x6a
	lsl r0, r0, #2
	add r1, r4, #0
	ldr r0, [r5, r0]
	add r1, #0x64
	bl Sprite_SetDrawPriority
	mov r0, #6
	ldr r1, [sp, #0x1c]
	lsl r0, r0, #0xc
	add r0, r1, r0
	str r0, [sp, #0x1c]
	mov r0, #3
	ldr r1, [sp, #0x20]
	lsl r0, r0, #0xe
	add r0, r1, r0
	str r0, [sp, #0x20]
	mov r0, #0x6b
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	cmp r0, #0
	bne _0223221C
	add r0, sp, #0x14
	bl Sprite_CreateAffine
	mov r1, #0x6b
	lsl r1, r1, #2
	str r0, [r5, r1]
_0223221C:
	mov r0, #0x6b
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	mov r1, #1
	bl Sprite_SetAnimActiveFlag
	mov r0, #0x6b
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	mov r1, #0x28
	bl Sprite_SetAnimCtrlSeq
	mov r0, #0x6b
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	mov r1, #1
	bl Sprite_SetPriority
	mov r0, #0x6b
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	mov r1, #0
	bl Sprite_SetDrawFlag
	mov r0, #0x6b
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	add r1, r4, #0
	bl Sprite_SetDrawPriority
	add r4, r4, #1
	add r7, r7, #4
	add r5, #0xc
	cmp r4, #0x1e
	beq _02232280
	ldr r0, [sp, #0x10]
	add r6, #0x28
	add r0, r0, #1
	str r0, [sp, #0x10]
	cmp r0, #6
	blt _02232184
	ldr r0, [sp, #8]
	add r0, #0x18
	str r0, [sp, #8]
	ldr r0, [sp, #0xc]
	add r0, r0, #1
	str r0, [sp, #0xc]
	cmp r0, #5
	bge _02232280
	b _02232178
_02232280:
	add sp, #0x44
	pop {r4, r5, r6, r7, pc}
	thumb_func_end ov74_02232154




	thumb_func_start ov74_02232284
ov74_02232284: ; 0x02232284
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x30
	add r4, r2, #0
	mov r2, #0x61
	add r6, r0, #0
	lsl r2, r2, #2
	add r5, r1, #0
	add r7, r3, #0
	add r0, sp, #0
	add r1, r6, #0
	add r2, r6, r2
	mov r3, #1
	bl ov74_02231D48
	lsl r0, r5, #0xc
	str r0, [sp, #8]
	lsl r0, r4, #0xc
	str r0, [sp, #0xc]
	mov r0, #0xa
	str r0, [sp, #0x24]
	add r0, sp, #0
	bl Sprite_CreateAffine
	add r4, r0, #0
	mov r1, #1
	bl Sprite_SetAnimActiveFlag
	add r0, r4, #0
	add r1, r7, #0
	bl Sprite_SetAnimCtrlSeq
	add r0, r4, #0
	mov r1, #1
	bl Sprite_SetPriority
	ldr r1, [sp, #0x48]
	add r0, r4, #0
	bl Sprite_SetDrawFlag
	add r0, r4, #0
	add sp, #0x30
	pop {r3, r4, r5, r6, r7, pc}
	thumb_func_end ov74_02232284




	thumb_func_start ov74_022322D8
ov74_022322D8: ; 0x022322D8
	push {r3, r4, r5, r6, r7, lr}
	add r6, r0, #0
	mov r0, #0x20
	str r0, [sp]
	mov r0, #0xe2
	lsl r0, r0, #2
	add r0, r6, r0
	mov r1, #0xe4
	mov r2, #0xb0
	mov r3, #0x32
	bl ov74_02231DDC
	mov r0, #1
	str r0, [sp]
	add r0, r6, #0
	mov r1, #0xe4
	mov r2, #0xb0
	mov r3, #6
	bl ov74_02232284
	mov r1, #0xe6
	lsl r1, r1, #2
	str r0, [r6, r1]
	sub r1, #8
	add r0, r6, r1
	mov r1, #0xb0
	mov r3, #0x20
	add r2, r1, #0
	str r3, [sp]
	bl ov74_02231DDC
	mov r0, #1
	mov r1, #0xb0
	str r0, [sp]
	add r0, r6, #0
	add r2, r1, #0
	mov r3, #3
	bl ov74_02232284
	mov r1, #0xea
	lsl r1, r1, #2
	str r0, [r6, r1]
	sub r1, #0x1c
	mov r3, #0x20
	add r0, r6, r1
	mov r1, #0x18
	mov r2, #0xb0
	str r3, [sp]
	bl ov74_02231DDC
	mov r0, #1
	str r0, [sp]
	add r0, r6, #0
	mov r1, #0x18
	mov r2, #0xb0
	mov r3, #0
	bl ov74_02232284
	mov r1, #0xee
	lsl r1, r1, #2
	str r0, [r6, r1]
	mov r7, #0
	mov r0, #0xff
	sub r1, #0x24
	strb r0, [r6, r1]
	add r4, r7, #0
	add r5, r6, #0
_0223235E:
	mov r0, #0
	str r0, [sp]
	add r0, r6, #0
	add r1, r4, #0
	mov r2, #0x40
	mov r3, #9
	bl ov74_02232284
	mov r1, #0xf2
	lsl r1, r1, #2
	str r0, [r5, r1]
	mov r1, #0
	mov r0, #0xf3
	mvn r1, r1
	lsl r0, r0, #2
	str r1, [r5, r0]
	mov r1, #0xe
	add r0, r0, #4
	str r1, [r5, r0]
	add r7, r7, #1
	add r4, #0x24
	add r5, #0xc
	cmp r7, #6
	blt _0223235E
	mov r0, #0x41
	mov r1, #0
	lsl r0, r0, #4
	str r1, [r6, r0]
	pop {r3, r4, r5, r6, r7, pc}
	thumb_func_end ov74_022322D8




	thumb_func_start ov74_02232398
ov74_02232398: ; 0x02232398
	push {r3, lr}
	ldr r3, _022323CC ; =0x0000E880
	ldr r2, [r0, r3]
	add r3, r3, #4
	ldr r3, [r0, r3]
	mov r0, #0x96
	lsl r0, r0, #4
	add r2, r2, #4
	mul r0, r3
	add r2, r2, r0
	mov r0, #0x50
	mul r0, r1
	add r0, r2, r0
	mov r1, #0x41
	mov r2, #0
	bl AGB_GetBoxMonData
	mov r1, #0x67
	lsl r1, r1, #2
	cmp r0, r1
	bne _022323C6
	mov r0, #1
	pop {r3, pc}
_022323C6:
	mov r0, #0
	pop {r3, pc}
	nop
_022323CC: .word 0x0000E880
	thumb_func_end ov74_02232398




	thumb_func_start ov74_022323D0
ov74_022323D0: ; 0x022323D0
	push {r3, r4, r5, r6, r7, lr}
	ldr r3, _0223241C ; =0x0000E880
	mov r5, #0
	ldr r2, [r0, r3]
	add r3, r3, #4
	ldr r3, [r0, r3]
	mov r0, #0x96
	lsl r0, r0, #4
	add r2, r2, #4
	mul r0, r3
	add r4, r2, r0
	mov r0, #0x50
	add r6, r1, #0
	mul r6, r0
	add r7, r5, #0
_022323EE:
	add r1, r5, #0
	add r0, r4, r6
	add r1, #0xd
	add r2, r7, #0
	bl AGB_GetBoxMonData
	ldr r3, _02232420 ; =ov74_0223C998
	add r2, r0, #0
	mov r1, #0
_02232400:
	ldr r0, [r3]
	cmp r2, r0
	bne _0223240A
	mov r0, #1
	pop {r3, r4, r5, r6, r7, pc}
_0223240A:
	add r1, r1, #1
	add r3, r3, #4
	cmp r1, #8
	blo _02232400
	add r5, r5, #1
	cmp r5, #4
	blt _022323EE
	mov r0, #0
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
_0223241C: .word 0x0000E880
_02232420: .word ov74_0223C998
	thumb_func_end ov74_022323D0




	thumb_func_start ov74_02232424
ov74_02232424: ; 0x02232424
	push {r3, lr}
	ldr r3, _02232468 ; =0x0000E880
	ldr r2, [r0, r3]
	add r3, r3, #4
	ldr r3, [r0, r3]
	mov r0, #0x96
	lsl r0, r0, #4
	add r2, r2, #4
	mul r0, r3
	add r2, r2, r0
	mov r0, #0x50
	mul r0, r1
	add r0, r2, r0
	mov r1, #0xc
	mov r2, #0
	bl AGB_GetBoxMonData
	ldr r1, _0223246C ; =ov74_0223C9A8
	ldrh r1, [r1, #0x30]
	cmp r1, #0
	beq _02232462
	ldr r2, _02232470 ; =ov74_0223C9D8
_02232450:
	ldrh r1, [r2]
	cmp r0, r1
	bne _0223245A
	mov r0, #1
	pop {r3, pc}
_0223245A:
	add r2, r2, #2
	ldrh r1, [r2]
	cmp r1, #0
	bne _02232450
_02232462:
	mov r0, #0
	pop {r3, pc}
	nop
_02232468: .word 0x0000E880
_0223246C: .word ov74_0223C9A8
_02232470: .word ov74_0223C9D8
	thumb_func_end ov74_02232424




	thumb_func_start ov74_02232474
ov74_02232474: ; 0x02232474
	push {r3, lr}
	ldr r3, _0223249C ; =0x0000E880
	ldr r2, [r0, r3]
	add r3, r3, #4
	ldr r3, [r0, r3]
	mov r0, #0x96
	lsl r0, r0, #4
	add r2, r2, #4
	mul r0, r3
	add r2, r2, r0
	mov r0, #0x50
	mul r0, r1
	add r0, r2, r0
	mov r1, #0xb
	mov r2, #0
	bl AGB_GetBoxMonData
	bl ov74_02231764
	pop {r3, pc}
	.balign 4, 0
_0223249C: .word 0x0000E880
	thumb_func_end ov74_02232474




	thumb_func_start ov74_022324A0
ov74_022324A0: ; 0x022324A0
	push {r4, r5, r6, r7, lr}
	sub sp, #0x14
	add r4, r0, #0
	mov r0, #0x6a
	lsl r0, r0, #2
	add r0, r4, r0
	str r0, [sp]
	add r6, r1, #0
	mov r0, #0xc
	mul r0, r6
	ldr r1, [sp]
	str r0, [sp, #4]
	ldr r0, [r1, r0]
	cmp r0, #0
	beq _022324CC
	bl Sprite_GetDrawFlag
	cmp r0, #0
	bne _022324CC
	add sp, #0x14
	mov r0, #3
	pop {r4, r5, r6, r7, pc}
_022324CC:
	mov r5, #0
	mov r7, #0x3d
	mvn r5, r5
	mov r2, #0
	add r3, r4, #0
	lsl r7, r7, #4
	b _022324F4
_022324DA:
	ldr r0, _02232628 ; =0x0000E884
	ldr r1, [r4, r0]
	ldr r0, [r3, r7]
	cmp r1, r0
	bne _022324F0
	mov r0, #0xf3
	lsl r0, r0, #2
	ldr r0, [r3, r0]
	cmp r6, r0
	bne _022324F0
	add r5, r2, #0
_022324F0:
	add r3, #0xc
	add r2, r2, #1
_022324F4:
	mov r0, #0
	mvn r0, r0
	cmp r5, r0
	bne _02232500
	cmp r2, #6
	blt _022324DA
_02232500:
	add r0, r4, #0
	add r1, r6, #0
	bl ov74_02232398
	cmp r0, #1
	bne _02232512
	add sp, #0x14
	mov r0, #4
	pop {r4, r5, r6, r7, pc}
_02232512:
	add r0, r4, #0
	add r1, r6, #0
	bl ov74_022323D0
	cmp r0, #1
	bne _02232524
	add sp, #0x14
	mov r0, #5
	pop {r4, r5, r6, r7, pc}
_02232524:
	add r0, r4, #0
	add r1, r6, #0
	bl ov74_02232424
	cmp r0, #1
	bne _02232536
	add sp, #0x14
	mov r0, #6
	pop {r4, r5, r6, r7, pc}
_02232536:
	add r0, r4, #0
	add r1, r6, #0
	bl ov74_02232474
	cmp r0, #1
	bne _02232548
	add sp, #0x14
	mov r0, #7
	pop {r4, r5, r6, r7, pc}
_02232548:
	mov r0, #0
	mvn r0, r0
	cmp r5, r0
	bne _022325F0
	mov r0, #0x41
	lsl r0, r0, #4
	ldr r0, [r4, r0]
	cmp r0, #6
	bne _02232560
	add sp, #0x14
	mov r0, #0
	pop {r4, r5, r6, r7, pc}
_02232560:
	mov r5, #0
	mov r0, #0xf3
	add r3, r4, #0
	lsl r0, r0, #2
	sub r1, r5, #1
_0223256A:
	ldr r2, [r3, r0]
	cmp r2, r1
	bne _022325E6
	ldr r1, [sp]
	ldr r0, [sp, #4]
	ldr r0, [r1, r0]
	bl Sprite_GetMatrixPtr
	add r1, r0, #0
	mov r0, #0xc
	add r7, r5, #0
	mul r7, r0
	mov r0, #0xf2
	lsl r0, r0, #2
	add r5, r4, r0
	ldr r0, [r5, r7]
	bl Sprite_SetMatrix
	ldr r0, [r5, r7]
	bl Sprite_GetMatrixPtr
	add r3, r0, #0
	add r2, sp, #8
	ldmia r3!, {r0, r1}
	mov ip, r2
	stmia r2!, {r0, r1}
	ldr r0, [r3]
	mov r1, #2
	str r0, [r2]
	ldr r0, [sp, #8]
	lsl r1, r1, #0xe
	sub r0, r0, r1
	str r0, [sp, #8]
	ldr r0, [sp, #0xc]
	lsr r1, r1, #1
	sub r0, r0, r1
	str r0, [sp, #0xc]
	ldr r0, [r5, r7]
	mov r1, ip
	bl Sprite_SetMatrix
	ldr r0, [r5, r7]
	mov r1, #1
	bl Sprite_SetDrawFlag
	mov r1, #0xf3
	add r3, r4, r7
	lsl r1, r1, #2
	ldr r0, _02232628 ; =0x0000E884
	str r6, [r3, r1]
	ldr r2, [r4, r0]
	add r0, r1, #4
	str r2, [r3, r0]
	add r0, r1, #0
	add r0, #0x44
	ldr r0, [r4, r0]
	add r1, #0x44
	add r0, r0, #1
	str r0, [r4, r1]
	add sp, #0x14
	mov r0, #1
	pop {r4, r5, r6, r7, pc}
_022325E6:
	add r5, r5, #1
	add r3, #0xc
	cmp r5, #6
	blt _0223256A
	b _02232622
_022325F0:
	mov r0, #0xc
	add r6, r5, #0
	mul r6, r0
	mov r0, #0xf2
	add r1, r4, r6
	lsl r0, r0, #2
	ldr r0, [r1, r0]
	mov r1, #0
	bl Sprite_SetDrawFlag
	mov r2, #0
	mov r1, #0xf3
	mvn r2, r2
	add r0, r4, r6
	lsl r1, r1, #2
	str r2, [r0, r1]
	add r0, r1, #0
	add r0, #0x44
	ldr r0, [r4, r0]
	add r1, #0x44
	sub r0, r0, #1
	str r0, [r4, r1]
	add sp, #0x14
	mov r0, #2
	pop {r4, r5, r6, r7, pc}
_02232622:
	mov r0, #0
	add sp, #0x14
	pop {r4, r5, r6, r7, pc}
	.balign 4, 0
_02232628: .word 0x0000E884
	thumb_func_end ov74_022324A0




	thumb_func_start ov74_0223262C
ov74_0223262C: ; 0x0223262C
	push {r3, r4, r5, r6, r7, lr}
	add r6, r0, #0
	mov r4, #0
	add r5, r6, #0
	sub r7, r4, #1
_02232636:
	mov r0, #0xf3
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	cmp r0, r7
	beq _0223265C
	ldr r0, _02232674 ; =0x0000E884
	ldr r1, [r6, r0]
	mov r0, #0x3d
	lsl r0, r0, #4
	ldr r0, [r5, r0]
	cmp r1, r0
	bne _0223265C
	mov r0, #0xf2
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	mov r1, #1
	bl Sprite_SetDrawFlag
	b _02232668
_0223265C:
	mov r0, #0xf2
	lsl r0, r0, #2
	ldr r0, [r5, r0]
	mov r1, #0
	bl Sprite_SetDrawFlag
_02232668:
	add r4, r4, #1
	add r5, #0xc
	cmp r4, #6
	blt _02232636
	pop {r3, r4, r5, r6, r7, pc}
	nop
_02232674: .word 0x0000E884
	thumb_func_end ov74_0223262C




	thumb_func_start ov74_02232678
ov74_02232678: ; 0x02232678
	push {r3, r4, r5, lr}
	sub sp, #0x60
	add r5, r0, #0
	mov r0, #0
	add r4, r1, #0
	add r3, sp, #0x14
	add r1, r0, #0
	mov r2, #4
_02232688:
	stmia r3!, {r0, r1}
	stmia r3!, {r0, r1}
	sub r2, r2, #1
	bne _02232688
	stmia r3!, {r0, r1}
	str r0, [r3]
	ldr r0, _022326F0 ; =0x00000478
	add r0, r5, r0
	str r0, [sp, #0x14]
	mov r0, #6
	str r0, [sp, #0x1c]
	mov r0, #0x15
	str r0, [sp, #0x20]
	mov r0, #0xd
	str r0, [sp, #0x24]
	mov r0, #2
	str r0, [sp, #0x28]
	sub r0, r0, #3
	str r0, [sp, #0x48]
	mov r0, #0
	str r0, [sp, #0x2c]
	str r0, [sp, #0x30]
	mov r0, #1
	str r0, [sp, #0x38]
	str r0, [sp, #0x3c]
	ldr r0, _022326F4 ; =0x00010200
	str r0, [sp, #0x40]
	mov r0, #0xa0
	str r0, [sp, #0x34]
	bl PmAgbCartridge_GetLanguage
	add r3, r0, #0
	ldr r0, _022326F8 ; =0x0000E880
	mov r2, #9
	ldr r1, [r5, r0]
	ldr r0, _022326FC ; =0x00008344
	add r1, r1, r0
	lsl r0, r4, #3
	add r0, r4, r0
	add r0, r1, r0
	add r1, sp, #0
	bl ConvertRSStringToDPStringInternational
	add r0, sp, #0
	str r0, [sp, #0x4c]
	add r0, r5, #0
	add r1, sp, #0x14
	mov r2, #1
	bl ov74_02231A1C
	add sp, #0x60
	pop {r3, r4, r5, pc}
	.balign 4, 0
_022326F0: .word 0x00000478
_022326F4: .word 0x00010200
_022326F8: .word 0x0000E880
_022326FC: .word 0x00008344
	thumb_func_end ov74_02232678




	thumb_func_start ov74_02232700
ov74_02232700: ; 0x02232700
	push {r4, r5, r6, lr}
	add r5, r0, #0
	ldr r0, [r5, #0x20]
	add r6, r1, #0
	mov r1, #2
	bl GetBgTilemapBuffer
	add r4, r0, #0
	cmp r6, #0
	beq _02232724
	add r0, r6, #0
	mov r1, #8
	mov r2, #0
	bl AGB_GetBoxMonData
	lsl r0, r0, #0x18
	lsr r1, r0, #0x18
	b _02232726
_02232724:
	mov r1, #0
_02232726:
	mov r2, #0
	mov r0, #1
_0223272A:
	add r3, r1, #0
	tst r3, r0
	beq _02232736
	add r6, r2, #0
	add r6, #0x80
	b _0223273A
_02232736:
	add r6, r2, #0
	add r6, #0x60
_0223273A:
	add r3, r4, #0
	add r3, #0xd6
	lsl r1, r1, #0x17
	add r2, r2, #1
	strh r6, [r3]
	lsr r1, r1, #0x18
	add r4, r4, #2
	cmp r2, #4
	blt _0223272A
	ldr r0, [r5, #0x20]
	mov r1, #2
	bl BgCommitTilemapBufferToVram
	pop {r4, r5, r6, pc}
	.balign 4, 0
	thumb_func_end ov74_02232700




	thumb_func_start ov74_02232758
ov74_02232758: ; 0x02232758
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x78
	add r5, r0, #0
	mov r0, #0
	add r4, r1, #0
	add r3, sp, #0x2c
	add r1, r0, #0
	mov r2, #4
_02232768:
	stmia r3!, {r0, r1}
	stmia r3!, {r0, r1}
	sub r2, r2, #1
	bne _02232768
	stmia r3!, {r0, r1}
	str r0, [r3]
	mov r1, #0x20
	str r1, [sp, #0x3c]
	mov r1, #1
	ldr r0, _02232908 ; =0x00000488
	str r1, [sp, #0x50]
	str r1, [sp, #0x54]
	mov r1, #0xbc
	str r1, [sp, #0x4c]
	ldr r1, _0223290C ; =0x000F0200
	add r0, r5, r0
	str r1, [sp, #0x58]
	mov r1, #0x2c
	str r0, [sp, #0x2c]
	mov r0, #0
	str r1, [sp, #0x60]
	mov r1, #0x90
	mov r2, #4
	str r1, [sp, #0x44]
	str r0, [sp, #0x34]
	str r0, [sp, #0x38]
	str r0, [sp, #0x64]
	str r0, [sp, #0x48]
	add r0, r5, #0
	add r1, sp, #0x2c
	str r2, [sp, #0x40]
	bl ov74_02231A1C
	mov r0, #0x2b
	str r0, [sp, #0x60]
	mov r0, #0x50
	str r0, [sp, #0x44]
	mov r0, #8
	str r0, [sp, #0x48]
	add r0, r5, #0
	add r1, sp, #0x2c
	mov r2, #6
	bl ov74_02231A1C
	add r0, r5, #0
	add r1, r4, #0
	bl ov74_02232700
	cmp r4, #0
	bne _022327D6
	ldr r0, [sp, #0x2c]
	bl CopyWindowToVram
	add sp, #0x78
	pop {r3, r4, r5, r6, r7, pc}
_022327D6:
	add r2, sp, #0x1c
	add r0, r4, #0
	mov r1, #2
	add r2, #2
	bl AGB_GetBoxMonData
	add r0, r4, #0
	mov r1, #3
	mov r2, #0
	bl AGB_GetBoxMonData
	add r3, r0, #0
	add r0, sp, #0x1c
	add r0, #2
	add r1, sp, #8
	mov r2, #0xb
	bl ConvertRSStringToDPStringInternational
	add r0, sp, #8
	str r0, [sp, #0x64]
	mov r0, #0
	mvn r0, r0
	str r0, [sp, #0x60]
	mov r0, #8
	str r0, [sp, #0x44]
	mov r0, #0
	str r0, [sp, #0x48]
	add r0, r5, #0
	add r1, sp, #0x2c
	mov r2, #6
	bl ov74_02231A1C
	mov r0, #0x40
	mov r1, #0x4c
	bl String_New
	add r6, r0, #0
	mov r0, #1
	mov r1, #0x1b
	mov r2, #0xed
	mov r3, #0x4c
	bl NewMsgDataFromNarc
	add r7, r0, #0
	add r0, r4, #0
	mov r1, #0xb
	mov r2, #0
	bl AGB_GetBoxMonData
	bl TranslateAgbSpecies
	str r0, [sp, #4]
	ldr r1, [sp, #4]
	add r0, r7, #0
	add r2, r6, #0
	bl ReadMsgDataIntoString
	mov r0, #0
	mvn r0, r0
	str r0, [sp, #0x60]
	mov r0, #0x10
	str r0, [sp, #0x44]
	str r0, [sp, #0x48]
	add r0, r5, #0
	add r1, sp, #0x2c
	mov r2, #6
	str r6, [sp, #0x68]
	bl ov74_02231A1C
	add r0, r7, #0
	bl DestroyMsgData
	add r0, r6, #0
	bl String_Delete
	add r0, r4, #0
	mov r1, #0xc
	mov r2, #0
	bl AGB_GetBoxMonData
	cmp r0, #0
	beq _022328B8
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	bl UpConvertItemId_Gen3to4
	add r7, r0, #0
	mov r0, #0x40
	mov r1, #0x4c
	bl String_New
	lsl r1, r7, #0x10
	lsr r1, r1, #0x10
	mov r2, #0x4c
	add r6, r0, #0
	bl GetItemNameIntoString
	mov r0, #0
	mvn r0, r0
	str r0, [sp, #0x60]
	mov r0, #0x98
	str r0, [sp, #0x44]
	mov r0, #0x10
	str r0, [sp, #0x48]
	add r0, r5, #0
	add r1, sp, #0x2c
	mov r2, #6
	str r6, [sp, #0x68]
	bl ov74_02231A1C
	add r0, r6, #0
	bl String_Delete
_022328B8:
	add r0, r4, #0
	bl ov74_02234A0C
	add r6, r0, #0
	mov r0, #0xa
	mov r1, #0x4c
	bl String_New
	mov r3, #1
	add r1, r6, #0
	mov r2, #3
	add r4, r0, #0
	str r3, [sp]
	bl String16_FormatInteger
	mov r0, #0
	mvn r0, r0
	str r0, [sp, #0x60]
	mov r0, #0x64
	str r0, [sp, #0x44]
	mov r0, #8
	str r0, [sp, #0x48]
	add r0, r5, #0
	add r1, sp, #0x2c
	mov r2, #2
	str r4, [sp, #0x68]
	bl ov74_02231A1C
	add r0, r4, #0
	bl String_Delete
	ldr r0, [sp, #4]
	mov r1, #0
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	bl PlayCry
	add sp, #0x78
	pop {r3, r4, r5, r6, r7, pc}
	nop
_02232908: .word 0x00000488
_0223290C: .word 0x000F0200
	thumb_func_end ov74_02232758




	thumb_func_start ov74_02232910
ov74_02232910: ; 0x02232910
	push {r3, r4, r5, r6, lr}
	sub sp, #0xc
	add r5, r0, #0
	mov r0, #6
	str r0, [sp]
	mov r0, #1
	str r0, [sp, #4]
	mov r0, #0x4c
	str r0, [sp, #8]
	add r6, r2, #0
	mov r0, #0
	add r4, r3, #0
	add r2, r1, #0
	add r3, r0, #0
	bl BeginNormalPaletteFade
	cmp r4, #0
	beq _02232938
	mov r0, #0x17
	str r0, [r4]
_02232938:
	str r6, [r5, #0x24]
	add sp, #0xc
	pop {r3, r4, r5, r6, pc}
	.balign 4, 0
	thumb_func_end ov74_02232910




	thumb_func_start ov74_02232940
ov74_02232940: ; 0x02232940
	push {r3, r4, r5, r6, lr}
	sub sp, #0xc
	add r5, r0, #0
	mov r0, #6
	str r0, [sp]
	mov r0, #1
	add r4, r3, #0
	str r0, [sp, #4]
	mov r0, #0x4c
	add r6, r2, #0
	str r0, [sp, #8]
	ldr r3, _02232970 ; =0x00007FFF
	mov r0, #0
	add r2, r1, #0
	bl BeginNormalPaletteFade
	cmp r4, #0
	beq _02232968
	mov r0, #0x17
	str r0, [r4]
_02232968:
	str r6, [r5, #0x24]
	add sp, #0xc
	pop {r3, r4, r5, r6, pc}
	nop
_02232970: .word 0x00007FFF
	thumb_func_end ov74_02232940




	thumb_func_start ov74_02232974
ov74_02232974: ; 0x02232974
	push {r3, r4, r5, lr}
	sub sp, #0x80
	ldr r5, _02232A34 ; =_0223B4F8
	add r3, sp, #0x70
	add r4, r0, #0
	add r2, r3, #0
	ldmia r5!, {r0, r1}
	stmia r3!, {r0, r1}
	ldmia r5!, {r0, r1}
	stmia r3!, {r0, r1}
	add r0, r2, #0
	bl SetBothScreensModesAndDisable
	ldr r5, _02232A38 ; =_0223B508
	add r3, sp, #0x54
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
	ldr r5, _02232A3C ; =_0223B524
	add r3, sp, #0x38
	ldmia r5!, {r0, r1}
	add r2, r3, #0
	stmia r3!, {r0, r1}
	ldmia r5!, {r0, r1}
	stmia r3!, {r0, r1}
	ldmia r5!, {r0, r1}
	stmia r3!, {r0, r1}
	ldr r0, [r5]
	mov r1, #1
	str r0, [r3]
	add r0, r4, #0
	mov r3, #0
	bl InitBgFromTemplate
	add r0, r4, #0
	mov r1, #1
	bl BgClearTilemapBufferAndCommit
	ldr r5, _02232A40 ; =_0223B540
	add r3, sp, #0x1c
	ldmia r5!, {r0, r1}
	add r2, r3, #0
	stmia r3!, {r0, r1}
	ldmia r5!, {r0, r1}
	stmia r3!, {r0, r1}
	ldmia r5!, {r0, r1}
	stmia r3!, {r0, r1}
	ldr r0, [r5]
	mov r1, #2
	str r0, [r3]
	add r0, r4, #0
	mov r3, #0
	bl InitBgFromTemplate
	add r0, r4, #0
	mov r1, #2
	bl BgClearTilemapBufferAndCommit
	ldr r5, _02232A44 ; =_0223B55C
	add r3, sp, #0
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
	add sp, #0x80
	pop {r3, r4, r5, pc}
	nop
_02232A34: .word _0223B4F8
_02232A38: .word _0223B508
_02232A3C: .word _0223B524
_02232A40: .word _0223B540
_02232A44: .word _0223B55C
	thumb_func_end ov74_02232974




	thumb_func_start ov74_02232A48
ov74_02232A48: ; 0x02232A48
	push {r3, r4, r5, lr}
	sub sp, #0x38
	ldr r5, _02232AC0 ; =_0223B578
	add r4, r0, #0
	add r3, sp, #0x10
	mov r2, #5
_02232A54:
	ldmia r5!, {r0, r1}
	stmia r3!, {r0, r1}
	sub r2, r2, #1
	bne _02232A54
	add r0, sp, #0x10
	bl GfGfx_SetBanks
	ldr r0, [r4, #0x20]
	bl ov74_02232974
	ldr r0, _02232AC4 ; =gSystem + 0x60
	mov r1, #1
	strb r1, [r0, #9]
	bl GfGfx_SwapDisplay
	bl ResetAllTextPrinters
	mov r1, #0x1e
	mov r0, #0
	lsl r1, r1, #4
	mov r2, #0x4c
	bl LoadFontPal0
	mov r0, #0xc0
	str r0, [sp]
	mov r0, #0x4c
	mov r2, #0
	str r0, [sp, #4]
	mov r0, #0x71
	mov r1, #0x17
	add r3, r2, #0
	bl GfGfxLoader_GXLoadPal
	mov r0, #0
	str r0, [sp]
	mov r0, #5
	lsl r0, r0, #0xa
	str r0, [sp, #4]
	mov r0, #1
	str r0, [sp, #8]
	mov r0, #0x4c
	str r0, [sp, #0xc]
	ldr r2, [r4, #0x20]
	mov r0, #0x71
	mov r1, #0x1a
	mov r3, #2
	bl GfGfxLoader_LoadCharData
	mov r0, #2
	mov r1, #0x4c
	bl FontID_Alloc
	add sp, #0x38
	pop {r3, r4, r5, pc}
	.balign 4, 0
_02232AC0: .word _0223B578
_02232AC4: .word gSystem + 0x60
	thumb_func_end ov74_02232A48




	thumb_func_start ov74_02232AC8
ov74_02232AC8: ; 0x02232AC8
	push {r4, lr}
	sub sp, #0x10
	add r4, r0, #0
	mov r0, #0
	str r0, [sp]
	mov r0, #6
	lsl r0, r0, #8
	str r0, [sp, #4]
	mov r0, #1
	str r0, [sp, #8]
	mov r0, #0x4c
	str r0, [sp, #0xc]
	ldr r2, [r4, #0x20]
	mov r0, #0x71
	mov r1, #0x18
	mov r3, #2
	bl GfGfxLoader_LoadScrnData
	mov r0, #0x20
	str r0, [sp]
	mov r0, #0x18
	str r0, [sp, #4]
	mov r2, #0
	ldr r1, [r4]
	ldr r0, _02232B14 ; =ov74_0223C960
	add r3, r2, #0
	ldrb r0, [r0, r1]
	mov r1, #2
	str r0, [sp, #8]
	ldr r0, [r4, #0x20]
	bl BgTilemapRectChangePalette
	ldr r0, [r4, #0x20]
	mov r1, #2
	bl BgCommitTilemapBufferToVram
	add sp, #0x10
	pop {r4, pc}
	.balign 4, 0
_02232B14: .word ov74_0223C960
	thumb_func_end ov74_02232AC8




	thumb_func_start ov74_02232B18
ov74_02232B18: ; 0x02232B18
	push {r4, lr}
	sub sp, #8
	mov r1, #7
	add r4, r0, #0
	mov r0, #0
	lsl r1, r1, #6
	mov r2, #0x4c
	bl LoadFontPal0
	mov r1, #0
	mov r2, #0x3f
	str r1, [sp]
	mov r0, #0x4c
	str r0, [sp, #4]
	ldr r0, [r4, #0x20]
	lsl r2, r2, #4
	mov r3, #0xe
	bl LoadUserFrameGfx1
	ldr r0, [r4, #0x1c]
	ldr r2, _02232BC0 ; =0x000003D2
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	str r0, [sp]
	mov r0, #0x4c
	str r0, [sp, #4]
	ldr r0, [r4, #0x20]
	mov r1, #0
	mov r3, #0xd
	bl LoadUserFrameGfx2
	ldr r0, _02232BC4 ; =0x0000042C
	mov r1, #0
	add r0, r4, r0
	mov r2, #0x4c
	bl memset
	ldr r0, _02232BC8 ; =0x00000498
	mov r3, #0x1b
	add r1, r0, #0
	add r2, r4, r0
	sub r1, #0x6c
	str r2, [r4, r1]
	add r1, r0, #0
	mov r2, #2
	sub r1, #0x64
	str r2, [r4, r1]
	add r1, r0, #0
	mov r2, #1
	sub r1, #0x60
	str r2, [r4, r1]
	add r1, r0, #0
	sub r1, #0x5c
	str r3, [r4, r1]
	add r1, r0, #0
	mov r3, #4
	sub r1, #0x58
	str r3, [r4, r1]
	add r3, r0, #0
	mov r1, #0
	sub r3, #0x48
	str r1, [r4, r3]
	add r3, r0, #0
	sub r3, #0x44
	str r2, [r4, r3]
	add r3, r0, #0
	sub r3, #0x4c
	str r2, [r4, r3]
	add r2, r0, #0
	ldr r3, _02232BCC ; =0x0001020F
	sub r2, #0x40
	str r3, [r4, r2]
	add r2, r0, #0
	ldr r3, _02232BD0 ; =0x000F020F
	sub r2, #0x3c
	str r3, [r4, r2]
	add r2, r0, #0
	sub r2, #0x34
	str r1, [r4, r2]
	sub r0, #0x30
	str r1, [r4, r0]
	add sp, #8
	pop {r4, pc}
	nop
_02232BC0: .word 0x000003D2
_02232BC4: .word 0x0000042C
_02232BC8: .word 0x00000498
_02232BCC: .word 0x0001020F
_02232BD0: .word 0x000F020F
	thumb_func_end ov74_02232B18




	thumb_func_start ov74_02232BD4
ov74_02232BD4: ; 0x02232BD4
	push {r4, r5, r6, r7, lr}
	sub sp, #0x4c
	add r6, r0, #0
	mov r7, #0x6b
	mov r5, #0
	add r4, r6, #0
	lsl r7, r7, #2
_02232BE2:
	mov r0, #0x6a
	lsl r0, r0, #2
	ldr r0, [r4, r0]
	mov r1, #0
	bl Sprite_SetDrawFlag
	ldr r0, [r4, r7]
	mov r1, #0
	bl Sprite_SetDrawFlag
	add r5, r5, #1
	add r4, #0xc
	cmp r5, #0x1e
	blt _02232BE2
	mov r7, #0xf2
	mov r5, #0
	add r4, r6, #0
	lsl r7, r7, #2
_02232C06:
	ldr r0, [r4, r7]
	mov r1, #0
	bl Sprite_SetDrawFlag
	add r5, r5, #1
	add r4, #0xc
	cmp r5, #6
	blt _02232C06
	mov r0, #0xe6
	lsl r0, r0, #2
	ldr r0, [r6, r0]
	mov r1, #0
	bl Sprite_SetDrawFlag
	mov r0, #0xee
	lsl r0, r0, #2
	ldr r0, [r6, r0]
	mov r1, #0
	bl Sprite_SetDrawFlag
	mov r0, #0xea
	lsl r0, r0, #2
	ldr r0, [r6, r0]
	mov r1, #0
	bl Sprite_SetDrawFlag
	mov r2, #0x61
	lsl r2, r2, #2
	add r0, sp, #0x1c
	add r1, r6, #0
	add r2, r6, r2
	mov r3, #1
	bl ov74_02231D48
	mov r0, #0x1c
	mov r7, #0
	str r0, [sp, #0x10]
	add r4, r6, #0
	add r5, r6, #0
_02232C54:
	ldr r0, [sp, #0x10]
	lsl r0, r0, #0xc
	str r0, [sp, #0x24]
	mov r0, #0x8e
	lsl r0, r0, #0xc
	str r0, [sp, #0x28]
	add r0, sp, #0x1c
	bl Sprite_CreateAffine
	ldr r1, _02232D94 ; =0x00000414
	str r0, [r4, r1]
	add r0, r1, #0
	ldr r0, [r4, r0]
	mov r1, #1
	bl Sprite_SetAnimActiveFlag
	ldr r0, _02232D94 ; =0x00000414
	add r1, r7, #0
	ldr r0, [r4, r0]
	add r1, #0xa
	bl Sprite_SetAnimCtrlSeq
	ldr r0, _02232D94 ; =0x00000414
	mov r1, #1
	ldr r0, [r4, r0]
	bl Sprite_SetPriority
	ldr r0, _02232D94 ; =0x00000414
	mov r1, #1
	ldr r0, [r4, r0]
	bl Sprite_SetDrawFlag
	mov r1, #0x3d
	mov r2, #0xf3
	lsl r1, r1, #4
	lsl r2, r2, #2
	ldr r1, [r5, r1]
	ldr r2, [r5, r2]
	add r0, r6, #0
	bl ov74_02231D70
	mov r1, #0x3d
	mov r2, #0xf3
	lsl r1, r1, #4
	lsl r2, r2, #2
	str r0, [sp, #0x14]
	ldr r1, [r5, r1]
	ldr r2, [r5, r2]
	add r0, r6, #0
	bl ov74_02231D94
	mov r1, #0x3d
	mov r2, #0xf3
	lsl r1, r1, #4
	lsl r2, r2, #2
	str r0, [sp, #0x18]
	ldr r1, [r5, r1]
	ldr r2, [r5, r2]
	add r0, r6, #0
	bl ov74_02231DB8
	add r2, r0, #0
	ldr r0, _02232D94 ; =0x00000414
	str r7, [sp]
	ldr r0, [r4, r0]
	ldr r3, _02232D98 ; =gSystem + 0x60
	str r0, [sp, #4]
	ldrb r3, [r3, #0xa]
	ldr r0, [sp, #0x14]
	ldr r1, [sp, #0x18]
	bl ov74_02231EC4
	ldr r0, [sp, #0x10]
	add r7, r7, #1
	add r0, #0x28
	str r0, [sp, #0x10]
	add r4, r4, #4
	add r5, #0xc
	cmp r7, #6
	blt _02232C54
	mov r0, #0
	str r0, [sp]
	mov r0, #6
	lsl r0, r0, #8
	str r0, [sp, #4]
	mov r0, #1
	str r0, [sp, #8]
	mov r0, #0x4c
	str r0, [sp, #0xc]
	ldr r2, [r6, #0x20]
	mov r0, #0x71
	mov r1, #0x19
	mov r3, #2
	bl GfGfxLoader_LoadScrnData
	mov r0, #0x20
	str r0, [sp]
	mov r0, #0x18
	str r0, [sp, #4]
	mov r2, #0
	ldr r1, [r6]
	ldr r0, _02232D9C ; =ov74_0223C960
	add r3, r2, #0
	ldrb r0, [r0, r1]
	mov r1, #2
	str r0, [sp, #8]
	ldr r0, [r6, #0x20]
	bl BgTilemapRectChangePalette
	ldr r0, [r6, #0x20]
	mov r1, #2
	bl BgCommitTilemapBufferToVram
	mov r0, #2
	mov r1, #0
	bl GfGfx_EngineATogglePlanes
	mov r1, #7
	mov r0, #0
	lsl r1, r1, #6
	mov r2, #0x4c
	bl LoadFontPal0
	mov r1, #0
	mov r2, #0x3f
	str r1, [sp]
	mov r0, #0x4c
	str r0, [sp, #4]
	ldr r0, [r6, #0x20]
	lsl r2, r2, #4
	mov r3, #0xe
	bl LoadUserFrameGfx1
	ldr r0, [r6, #0x1c]
	ldr r2, _02232DA0 ; =0x000003D2
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	str r0, [sp]
	mov r0, #0x4c
	str r0, [sp, #4]
	ldr r0, [r6, #0x20]
	mov r1, #0
	mov r3, #0xd
	bl LoadUserFrameGfx2
	add r0, r6, #0
	bl ov74_02232B18
	mov r1, #0x46
	mov r0, #0xa
	lsl r1, r1, #4
	str r0, [r6, r1]
	sub r1, #0x34
	add r0, r6, #0
	add r1, r6, r1
	mov r2, #0x18
	bl ov74_02231A1C
	add sp, #0x4c
	pop {r4, r5, r6, r7, pc}
	.balign 4, 0
_02232D94: .word 0x00000414
_02232D98: .word gSystem + 0x60
_02232D9C: .word ov74_0223C960
_02232DA0: .word 0x000003D2
	thumb_func_end ov74_02232BD4




	thumb_func_start ov74_02232DA4
ov74_02232DA4: ; 0x02232DA4
	push {r4, lr}
	add r4, r0, #0
	ldr r1, [r4]
	cmp r1, #0
	beq _02232DC0
	mov r1, #0
	bl ClearFrameAndWindow2
	add r0, r4, #0
	bl ClearWindowTilemapAndCopyToVram
	add r0, r4, #0
	bl RemoveWindow
_02232DC0:
	pop {r4, pc}
	.balign 4, 0
	thumb_func_end ov74_02232DA4




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
