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



	thumb_func_start ov74_02236168
ov74_02236168: ; 0x02236168
	push {r4, lr}
	ldr r1, _022361A4 ; =ov74_0223E2FC
	add r4, r0, #0
	ldr r2, [r1, #4]
	cmp r2, #0
	beq _02236184
	ldr r3, [r1]
	ldr r1, _022361A8 ; =0x00001150
	ldr r0, _022361AC ; =ov74_0223D00C
	ldr r1, [r3, r1]
	lsl r3, r1, #2
	ldr r1, _022361B0 ; =ov74_0223CFE4
	ldr r1, [r1, r3]
	blx r2
_02236184:
	ldr r3, _022361A4 ; =ov74_0223E2FC
	ldr r1, _022361A8 ; =0x00001150
	ldr r0, [r3]
	str r4, [r0, r1]
	ldr r2, [r3, #4]
	cmp r2, #0
	beq _022361A0
	ldr r3, [r3]
	ldr r0, _022361B4 ; =ov74_0223D014
	ldr r1, [r3, r1]
	lsl r3, r1, #2
	ldr r1, _022361B0 ; =ov74_0223CFE4
	ldr r1, [r1, r3]
	blx r2
_022361A0:
	pop {r4, pc}
	nop
_022361A4: .word ov74_0223E2FC
_022361A8: .word 0x00001150
_022361AC: .word ov74_0223D00C
_022361B0: .word ov74_0223CFE4
_022361B4: .word ov74_0223D014
	thumb_func_end ov74_02236168




	thumb_func_start ov74_022361B8
ov74_022361B8: ; 0x022361B8
	ldr r1, _022361CC ; =ov74_0223E2FC
	ldr r3, [r1]
	ldr r1, _022361D0 ; =0x00001150
	ldr r2, [r3, r1]
	sub r2, #9
	cmp r2, #1
	bls _022361CA
	add r1, r1, #4
	str r0, [r3, r1]
_022361CA:
	bx lr
	.balign 4, 0
_022361CC: .word ov74_0223E2FC
_022361D0: .word 0x00001150
	thumb_func_end ov74_022361B8




	thumb_func_start ov74_022361D4
ov74_022361D4: ; 0x022361D4
	push {r4, lr}
	ldr r0, _02236240 ; =ov74_0223E2FC
	ldr r1, [r0]
	ldr r0, _02236244 ; =0x00001150
	ldr r0, [r1, r0]
	cmp r0, #4
	bne _022361E6
	bl OS_Terminate
_022361E6:
	mov r0, #2
	bl ov74_02236168
	ldr r3, _02236240 ; =ov74_0223E2FC
	ldr r0, _02236248 ; =0x0000FFFF
	ldr r2, [r3]
	ldr r1, _0223624C ; =0x00000FCC
	strh r0, [r2, r1]
	sub r2, r1, #2
	ldr r4, [r3]
	sub r1, r1, #4
	strh r0, [r4, r2]
	ldr r2, [r3]
	strh r0, [r2, r1]
	bl ov74_02236258
	cmp r0, #0
	bne _02236224
	mov r0, #9
	bl ov74_02236168
	ldr r0, _02236240 ; =ov74_0223E2FC
	ldr r1, [r0]
	ldr r0, _02236250 ; =0x0000117C
	ldr r1, [r1, r0]
	cmp r1, #0
	beq _02236220
	mov r0, #4
	blx r1
_02236220:
	mov r0, #0
	pop {r4, pc}
_02236224:
	ldr r1, _02236240 ; =ov74_0223E2FC
	ldr r0, _02236254 ; =0x00001158
	ldr r2, [r1]
	mov r3, #2
	strh r3, [r2, r0]
	ldr r1, [r1]
	add r0, #0x24
	ldr r1, [r1, r0]
	cmp r1, #0
	beq _0223623C
	mov r0, #0
	blx r1
_0223623C:
	mov r0, #1
	pop {r4, pc}
	.balign 4, 0
_02236240: .word ov74_0223E2FC
_02236244: .word 0x00001150
_02236248: .word 0x0000FFFF
_0223624C: .word 0x00000FCC
_02236250: .word 0x0000117C
_02236254: .word 0x00001158
	thumb_func_end ov74_022361D4




	thumb_func_start ov74_02236258
ov74_02236258: ; 0x02236258
	push {r3, r4, r5, r6, r7, lr}
	ldr r0, _02236330 ; =ov74_0223E2FC
	ldr r1, [r0]
	ldr r0, _02236334 ; =0x00001150
	ldr r0, [r1, r0]
	cmp r0, #2
	beq _0223626C
	bl OS_Terminate
	mov r0, #0
_0223626C:
	bl WM_GetAllowedChannel
	add r4, r0, #0
	mov r0, #2
	lsl r0, r0, #0xe
	cmp r4, r0
	bne _02236284
	mov r0, #3
	bl ov74_022361B8
	mov r0, #0
	pop {r3, r4, r5, r6, r7, pc}
_02236284:
	cmp r4, #0
	bne _02236292
	mov r0, #0x16
	bl ov74_022361B8
	mov r0, #0
	pop {r3, r4, r5, r6, r7, pc}
_02236292:
	ldr r0, _02236330 ; =ov74_0223E2FC
	ldr r2, [r0]
	ldr r0, _02236338 ; =0x00001176
	ldrh r1, [r2, r0]
	cmp r1, #0x10
	blo _022362D4
	sub r1, #0x10
	mov r5, #0
	mov r7, #0xd
	mov r6, #1
	b _022362BA
_022362A8:
	add r0, r1, #1
	add r1, r7, #0
	bl _s32_div_f
	add r0, r6, #0
	lsl r0, r1
	tst r0, r4
	bne _022362BE
	add r5, r5, #1
_022362BA:
	cmp r5, #0xd
	blt _022362A8
_022362BE:
	ldr r2, _02236330 ; =ov74_0223E2FC
	add r4, r1, #0
	ldr r3, [r2]
	ldr r0, _02236338 ; =0x00001176
	add r4, #0x10
	strh r4, [r3, r0]
	add r3, r1, #1
	ldr r1, [r2]
	ldr r0, _0223633C ; =0x00000FC4
	strh r3, [r1, r0]
	b _022362D8
_022362D4:
	ldr r0, _0223633C ; =0x00000FC4
	strh r1, [r2, r0]
_022362D8:
	ldr r2, _02236330 ; =ov74_0223E2FC
	ldr r0, _02236340 ; =0x00000FC6
	ldr r1, [r2]
	mov r3, #0xdc
	strh r3, [r1, r0]
	add r1, r0, #0
	ldr r3, [r2]
	sub r1, #0xc6
	add r1, r3, r1
	sub r0, r0, #6
	str r1, [r3, r0]
	ldr r1, [r2]
	ldr r0, _02236344 ; =0x00001158
	ldrh r0, [r1, r0]
	cmp r0, #3
	bne _0223630E
	mov r5, #0
	ldr r0, _02236348 ; =0x00001170
	ldr r1, _0223634C ; =0x00000FC8
	b _0223630A
_02236300:
	ldr r3, [r2]
	add r4, r3, r5
	ldrb r3, [r4, r0]
	add r5, r5, #1
	strb r3, [r4, r1]
_0223630A:
	cmp r5, #6
	blt _02236300
_0223630E:
	ldr r1, _02236330 ; =ov74_0223E2FC
	ldr r0, _02236350 ; =ov74_02236354
	ldr r2, [r1]
	mov r1, #0x3f
	lsl r1, r1, #6
	add r1, r2, r1
	bl WM_StartScan
	cmp r0, #2
	beq _0223632A
	bl ov74_022361B8
	mov r0, #0
	pop {r3, r4, r5, r6, r7, pc}
_0223632A:
	mov r0, #1
	pop {r3, r4, r5, r6, r7, pc}
	nop
_02236330: .word ov74_0223E2FC
_02236334: .word 0x00001150
_02236338: .word 0x00001176
_0223633C: .word 0x00000FC4
_02236340: .word 0x00000FC6
_02236344: .word 0x00001158
_02236348: .word 0x00001170
_0223634C: .word 0x00000FC8
_02236350: .word ov74_02236354
	thumb_func_end ov74_02236258




	thumb_func_start ov74_02236354
ov74_02236354: ; 0x02236354
	push {r3, r4, r5, lr}
	add r4, r0, #0
	ldrh r0, [r4, #2]
	cmp r0, #0
	beq _0223637A
	bl ov74_022361B8
	mov r0, #9
	bl ov74_02236168
	ldr r0, _02236468 ; =ov74_0223E2FC
	ldr r1, [r0]
	ldr r0, _0223646C ; =0x0000117C
	ldr r1, [r1, r0]
	cmp r1, #0
	beq _02236466
	mov r0, #4
	blx r1
	pop {r3, r4, r5, pc}
_0223637A:
	ldr r0, _02236468 ; =ov74_0223E2FC
	ldr r1, [r0]
	ldr r0, _02236470 ; =0x00001150
	ldr r0, [r1, r0]
	cmp r0, #2
	beq _022363A6
	bl ov74_0223648C
	cmp r0, #0
	bne _02236466
	mov r0, #9
	bl ov74_02236168
	ldr r0, _02236468 ; =ov74_0223E2FC
	ldr r1, [r0]
	ldr r0, _0223646C ; =0x0000117C
	ldr r1, [r1, r0]
	cmp r1, #0
	beq _02236466
	mov r0, #4
	blx r1
	pop {r3, r4, r5, pc}
_022363A6:
	ldrh r0, [r4, #8]
	cmp r0, #3
	beq _02236466
	cmp r0, #4
	beq _02236448
	cmp r0, #5
	bne _02236448
	mov r0, #0xf
	lsl r0, r0, #8
	add r0, r1, r0
	mov r1, #0xc0
	bl DC_InvalidateRange
	ldrh r0, [r4, #0x36]
	cmp r0, #8
	blo _022363CE
	ldr r1, [r4, #0x3c]
	ldr r0, _02236474 ; =0x00400318
	cmp r1, r0
	beq _022363E0
_022363CE:
	ldr r0, _02236468 ; =ov74_0223E2FC
	ldr r3, [r0, #4]
	cmp r3, #0
	beq _02236448
	ldr r0, _02236478 ; =ov74_0223D018
	ldr r1, [r4, #0x3c]
	ldr r2, _02236474 ; =0x00400318
	blx r3
	b _02236448
_022363E0:
	ldr r1, _02236468 ; =ov74_0223E2FC
	ldrh r3, [r4, #0x12]
	ldr r2, [r1]
	ldr r0, _0223647C ; =0x0000116C
	str r3, [r2, r0]
	ldr r1, [r1]
	sub r0, #0x14
	ldrh r0, [r1, r0]
	cmp r0, #2
	bne _0223642C
	add r0, r4, #0
	add r0, #0x48
	bl ov74_022366E8
	ldr r2, _02236468 ; =ov74_0223E2FC
	ldr r1, _02236480 ; =0x00001170
	mov r0, #0
_02236402:
	add r3, r4, r0
	ldrb r5, [r3, #0xa]
	ldr r3, [r2]
	add r3, r3, r0
	add r0, r0, #1
	strb r5, [r3, r1]
	cmp r0, #6
	blt _02236402
	ldr r1, _02236468 ; =ov74_0223E2FC
	ldr r0, _02236484 ; =0x00001176
	ldr r3, [r1]
	ldrh r2, [r3, r0]
	sub r2, #0xf
	strh r2, [r3, r0]
	ldr r1, [r1]
	add r0, r0, #6
	ldr r1, [r1, r0]
	cmp r1, #0
	beq _0223642C
	mov r0, #1
	blx r1
_0223642C:
	add r4, #0x48
	add r0, r4, #0
	bl ov74_02236768
	cmp r0, #0
	beq _02236448
	ldr r0, _02236468 ; =ov74_0223E2FC
	mov r2, #4
	ldr r1, [r0]
	ldr r0, _02236488 ; =0x00001158
	strh r2, [r1, r0]
	bl ov74_022365FC
	pop {r3, r4, r5, pc}
_02236448:
	bl ov74_02236258
	cmp r0, #0
	bne _02236466
	mov r0, #9
	bl ov74_02236168
	ldr r0, _02236468 ; =ov74_0223E2FC
	ldr r1, [r0]
	ldr r0, _0223646C ; =0x0000117C
	ldr r1, [r1, r0]
	cmp r1, #0
	beq _02236466
	mov r0, #4
	blx r1
_02236466:
	pop {r3, r4, r5, pc}
	.balign 4, 0
_02236468: .word ov74_0223E2FC
_0223646C: .word 0x0000117C
_02236470: .word 0x00001150
_02236474: .word 0x00400318
_02236478: .word ov74_0223D018
_0223647C: .word 0x0000116C
_02236480: .word 0x00001170
_02236484: .word 0x00001176
_02236488: .word 0x00001158
	thumb_func_end ov74_02236354




	thumb_func_start ov74_0223648C
ov74_0223648C: ; 0x0223648C
	push {r3, lr}
	ldr r0, _022364A4 ; =ov74_022364A8
	bl WM_EndScan
	cmp r0, #2
	beq _022364A0
	bl ov74_022361B8
	mov r0, #0
	pop {r3, pc}
_022364A0:
	mov r0, #1
	pop {r3, pc}
	.balign 4, 0
_022364A4: .word ov74_022364A8
	thumb_func_end ov74_0223648C




	thumb_func_start ov74_022364A8
ov74_022364A8: ; 0x022364A8
	push {r3, lr}
	ldrh r0, [r0, #2]
	cmp r0, #0
	beq _022364B6
	bl ov74_022361B8
	pop {r3, pc}
_022364B6:
	mov r0, #1
	bl ov74_02236168
	pop {r3, pc}
	.balign 4, 0
	thumb_func_end ov74_022364A8




	thumb_func_start ov74_022364C0
ov74_022364C0: ; 0x022364C0
	push {r3, lr}
	ldrh r0, [r0, #2]
	cmp r0, #0
	beq _022364D0
	mov r0, #0xa
	bl ov74_02236168
	pop {r3, pc}
_022364D0:
	mov r0, #0
	bl ov74_02236168
	ldr r0, _022364E8 ; =ov74_0223E2FC
	ldr r1, [r0]
	ldr r0, _022364EC ; =0x0000117C
	ldr r1, [r1, r0]
	cmp r1, #0
	beq _022364E6
	mov r0, #3
	blx r1
_022364E6:
	pop {r3, pc}
	.balign 4, 0
_022364E8: .word ov74_0223E2FC
_022364EC: .word 0x0000117C
	thumb_func_end ov74_022364C0




	thumb_func_start ov74_022364F0
ov74_022364F0: ; 0x022364F0
	push {r3, lr}
	ldr r0, _0223650C ; =ov74_0223E2FC
	mov r2, #0
	ldr r1, [r0]
	ldr r0, _02236510 ; =0x00001154
	str r2, [r1, r0]
	bl ov74_0223653C
	cmp r0, #0
	beq _02236508
	mov r0, #1
	pop {r3, pc}
_02236508:
	mov r0, #0
	pop {r3, pc}
	.balign 4, 0
_0223650C: .word ov74_0223E2FC
_02236510: .word 0x00001154
	thumb_func_end ov74_022364F0




	thumb_func_start ov74_02236514
ov74_02236514: ; 0x02236514
	push {r3, lr}
	ldrh r0, [r0, #2]
	cmp r0, #8
	bne _02236532
	mov r0, #9
	bl ov74_02236168
	ldr r0, _02236534 ; =ov74_0223E2FC
	ldr r1, [r0]
	ldr r0, _02236538 ; =0x0000117C
	ldr r1, [r1, r0]
	cmp r1, #0
	beq _02236532
	mov r0, #4
	blx r1
_02236532:
	pop {r3, pc}
	.balign 4, 0
_02236534: .word ov74_0223E2FC
_02236538: .word 0x0000117C
	thumb_func_end ov74_02236514




	thumb_func_start ov74_0223653C
ov74_0223653C: ; 0x0223653C
	push {r3, lr}
	mov r0, #3
	bl ov74_02236168
	ldr r0, _02236568 ; =ov74_0223E2FC
	ldr r1, _0223656C ; =ov74_02236570
	ldr r0, [r0]
	mov r2, #2
	bl WM_Initialize
	cmp r0, #2
	beq _02236562
	bl ov74_022361B8
	mov r0, #0xa
	bl ov74_02236168
	mov r0, #0
	pop {r3, pc}
_02236562:
	mov r0, #1
	pop {r3, pc}
	nop
_02236568: .word ov74_0223E2FC
_0223656C: .word ov74_02236570
	thumb_func_end ov74_0223653C




	thumb_func_start ov74_02236570
ov74_02236570: ; 0x02236570
	push {r3, lr}
	ldrh r0, [r0, #2]
	cmp r0, #0
	beq _02236584
	bl ov74_022361B8
	mov r0, #0xa
	bl ov74_02236168
	pop {r3, pc}
_02236584:
	ldr r0, _022365A8 ; =ov74_02236514
	bl WM_SetIndCallback
	cmp r0, #0
	beq _0223659A
	bl ov74_022361B8
	mov r0, #0xa
	bl ov74_02236168
	pop {r3, pc}
_0223659A:
	mov r0, #1
	bl ov74_02236168
	bl ov74_022361D4
	pop {r3, pc}
	nop
_022365A8: .word ov74_02236514
	thumb_func_end ov74_02236570




	thumb_func_start ov74_022365AC
ov74_022365AC: ; 0x022365AC
	push {r3, lr}
	ldr r0, _022365EC ; =ov74_0223E2FC
	ldr r1, [r0]
	ldr r0, _022365F0 ; =0x00001150
	ldr r0, [r1, r0]
	cmp r0, #1
	beq _022365BE
	bl OS_Terminate
_022365BE:
	mov r0, #3
	bl ov74_02236168
	ldr r0, _022365F4 ; =ov74_022364C0
	bl WM_End
	cmp r0, #2
	beq _022365E8
	mov r0, #9
	bl ov74_02236168
	ldr r0, _022365EC ; =ov74_0223E2FC
	ldr r1, [r0]
	ldr r0, _022365F8 ; =0x0000117C
	ldr r1, [r1, r0]
	cmp r1, #0
	beq _022365E4
	mov r0, #4
	blx r1
_022365E4:
	mov r0, #0
	pop {r3, pc}
_022365E8:
	mov r0, #1
	pop {r3, pc}
	.balign 4, 0
_022365EC: .word ov74_0223E2FC
_022365F0: .word 0x00001150
_022365F4: .word ov74_022364C0
_022365F8: .word 0x0000117C
	thumb_func_end ov74_022365AC




	thumb_func_start ov74_022365FC
ov74_022365FC: ; 0x022365FC
	push {r3, lr}
	ldr r0, _02236618 ; =ov74_0223E2FC
	ldr r1, [r0]
	ldr r0, _0223661C ; =0x00001150
	ldr r0, [r1, r0]
	cmp r0, #1
	bne _02236610
	bl ov74_022365AC
	pop {r3, pc}
_02236610:
	bl ov74_02236620
	pop {r3, pc}
	nop
_02236618: .word ov74_0223E2FC
_0223661C: .word 0x00001150
	thumb_func_end ov74_022365FC




	thumb_func_start ov74_02236620
ov74_02236620: ; 0x02236620
	push {r3, lr}
	mov r0, #3
	bl ov74_02236168
	ldr r0, _02236640 ; =ov74_02236644
	bl WM_Reset
	cmp r0, #2
	beq _0223663A
	bl ov74_022361B8
	mov r0, #0
	pop {r3, pc}
_0223663A:
	mov r0, #1
	pop {r3, pc}
	nop
_02236640: .word ov74_02236644
	thumb_func_end ov74_02236620




	thumb_func_start ov74_02236644
ov74_02236644: ; 0x02236644
	push {r4, lr}
	add r4, r0, #0
	ldrh r0, [r4, #2]
	cmp r0, #0
	beq _0223666C
	mov r0, #9
	bl ov74_02236168
	ldrh r0, [r4, #2]
	bl ov74_022361B8
	ldr r0, _02236678 ; =ov74_0223E2FC
	ldr r1, [r0]
	ldr r0, _0223667C ; =0x0000117C
	ldr r1, [r1, r0]
	cmp r1, #0
	beq _02236676
	mov r0, #4
	blx r1
	pop {r4, pc}
_0223666C:
	mov r0, #1
	bl ov74_02236168
	bl ov74_022365AC
_02236676:
	pop {r4, pc}
	.balign 4, 0
_02236678: .word ov74_0223E2FC
_0223667C: .word 0x0000117C
	thumb_func_end ov74_02236644




	thumb_func_start ov74_02236680
ov74_02236680: ; 0x02236680
	push {r3, r4, r5, r6, r7, lr}
	add r7, r0, #0
	add r2, #0x1f
	mov r0, #0x1f
	bic r2, r0
	ldr r4, _022366DC ; =ov74_0223E2FC
	ldr r3, _022366E0 ; =0x00001150
	str r2, [r4]
	mov r0, #0
	str r0, [r2, r3]
	add r2, r3, #0
	ldr r5, [r4]
	mov r6, #1
	add r2, #8
	strh r6, [r5, r2]
	add r2, r3, #0
	ldr r5, [r4]
	add r2, #0x28
	str r7, [r5, r2]
	add r2, r3, #0
	ldr r5, [r4]
	add r2, #0xc
	strh r0, [r5, r2]
	add r2, r3, #0
	ldr r5, [r4]
	add r2, #0xe
	strh r0, [r5, r2]
	add r2, r3, #0
	ldr r5, [r4]
	add r2, #0x2c
	str r1, [r5, r2]
	add r2, r3, #0
	ldr r5, [r4]
	mov r1, #0x10
	add r2, #0x26
	strh r1, [r5, r2]
	ldr r5, [r4]
	ldr r2, _022366E4 ; =0x00001048
	sub r1, #0x11
	str r0, [r5, r2]
	ldr r0, [r4]
	add r3, #0x1c
	str r1, [r0, r3]
	bl ov74_022364F0
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
_022366DC: .word ov74_0223E2FC
_022366E0: .word 0x00001150
_022366E4: .word 0x00001048
	thumb_func_end ov74_02236680




	thumb_func_start ov74_022366E8
ov74_022366E8: ; 0x022366E8
	push {r4, r5, r6, lr}
	ldr r3, _02236758 ; =ov74_0223E2FC
	ldr r1, _0223675C ; =0x00001048
	ldr r2, [r3]
	mov r4, #0
	str r4, [r2, r1]
	ldr r1, [r3]
	ldr r2, _02236760 ; =0x00001158
	mov r4, #3
	strh r4, [r1, r2]
	ldrh r5, [r0]
	ldr r4, [r3]
	add r1, r2, #2
	strh r5, [r4, r1]
	ldr r4, [r0, #4]
	add r0, r2, #0
	ldr r1, [r3]
	add r0, #8
	str r4, [r1, r0]
	ldr r4, [r3]
	add r2, #8
	ldr r0, [r4, r2]
	mov r1, #0x68
	sub r0, r0, #1
	bl _u32_div_f
	ldr r2, _02236764 ; =0x0000115C
	add r0, r0, #1
	strh r0, [r4, r2]
	ldr r4, _02236758 ; =ov74_0223E2FC
	mov r0, #0
	ldr r3, [r4]
	add r1, r2, #2
	strh r0, [r3, r1]
	add r1, r2, #0
	ldr r3, [r4]
	add r1, #8
	str r0, [r3, r1]
	ldr r1, [r4]
	ldrh r2, [r1, r2]
	cmp r2, #0
	ble _02236754
	mov r2, #0x46
	lsl r2, r2, #6
	add r3, r2, #0
	add r6, r0, #0
	sub r3, #0x24
_02236746:
	add r1, r1, r0
	strb r6, [r1, r2]
	ldr r1, [r4]
	add r0, r0, #1
	ldrh r5, [r1, r3]
	cmp r0, r5
	blt _02236746
_02236754:
	pop {r4, r5, r6, pc}
	nop
_02236758: .word ov74_0223E2FC
_0223675C: .word 0x00001048
_02236760: .word 0x00001158
_02236764: .word 0x0000115C
	thumb_func_end ov74_022366E8




	thumb_func_start ov74_02236768
ov74_02236768: ; 0x02236768
	push {r3, r4, r5, lr}
	add r4, r0, #0
	ldr r0, _02236884 ; =ov74_0223E2FC
	ldrh r1, [r4]
	ldr r3, [r0]
	ldr r0, _02236888 ; =0x0000115A
	ldrh r2, [r3, r0]
	cmp r2, r1
	bne _02236784
	add r0, r0, #6
	ldr r1, [r3, r0]
	ldr r0, [r4, #4]
	cmp r1, r0
	beq _022367B0
_02236784:
	ldr r0, _02236884 ; =ov74_0223E2FC
	ldr r3, [r0]
	ldr r0, _0223688C ; =0x00001164
	ldr r2, [r3, r0]
	add r1, r2, #1
	str r1, [r3, r0]
	cmp r2, #0x10
	blo _022367AC
	add r0, r4, #0
	bl ov74_022366E8
	ldr r0, _02236884 ; =ov74_0223E2FC
	ldr r1, [r0]
	ldr r0, _02236890 ; =0x0000117C
	ldr r1, [r1, r0]
	cmp r1, #0
	beq _022367B0
	mov r0, #5
	blx r1
	b _022367B0
_022367AC:
	mov r0, #0
	pop {r3, r4, r5, pc}
_022367B0:
	ldrh r3, [r4, #2]
	ldr r0, _02236894 ; =0x0000FFFF
	cmp r3, r0
	bne _022367D8
	ldr r1, _02236884 ; =ov74_0223E2FC
	add r4, #8
	ldr r2, [r1]
	mov r1, #0xfe
	lsl r1, r1, #4
	add r1, r2, r1
	add r0, r4, #0
	mov r2, #0x68
	bl MIi_CpuCopy32
	ldr r0, _02236884 ; =ov74_0223E2FC
	mov r2, #1
	ldr r1, [r0]
	ldr r0, _02236898 ; =0x00001048
	str r2, [r1, r0]
	b _02236880
_022367D8:
	ldr r1, _02236884 ; =ov74_0223E2FC
	ldr r0, _0223689C ; =0x00001168
	ldr r2, [r1]
	str r3, [r2, r0]
	ldr r3, [r1]
	add r1, r0, #0
	sub r1, #0xc
	ldrh r2, [r4, #2]
	ldrh r1, [r3, r1]
	cmp r2, r1
	blo _02236800
	add r0, #0x14
	ldr r1, [r3, r0]
	cmp r1, #0
	beq _022367FA
	mov r0, #4
	blx r1
_022367FA:
	mov r0, #9
	bl ov74_02236168
_02236800:
	ldr r0, _02236884 ; =ov74_0223E2FC
	mov r2, #0x46
	ldr r1, [r0]
	lsl r2, r2, #6
	ldrh r3, [r4, #2]
	add r5, r1, r2
	ldrb r1, [r5, r3]
	cmp r1, #0
	bne _02236880
	mov r1, #1
	strb r1, [r5, r3]
	ldr r5, [r0]
	add r0, r2, #0
	sub r0, #0x24
	ldrh r0, [r5, r0]
	ldrh r1, [r4, #2]
	sub r0, r0, #1
	cmp r1, r0
	bne _02236844
	add r3, r1, #0
	add r1, r2, #0
	sub r1, #8
	sub r2, #0x20
	mov r0, #0x68
	ldr r1, [r5, r1]
	mul r3, r0
	ldr r2, [r5, r2]
	add r4, #8
	add r0, r4, #0
	add r1, r1, r3
	sub r2, r2, r3
	bl MIi_CpuCopy32
	b _02236858
_02236844:
	add r4, #8
	sub r2, #8
	add r0, r4, #0
	ldr r4, [r5, r2]
	mov r2, #0x68
	add r3, r1, #0
	mul r3, r2
	add r1, r4, r3
	bl MIi_CpuCopy32
_02236858:
	ldr r1, _02236884 ; =ov74_0223E2FC
	ldr r0, _022368A0 ; =0x0000115E
	ldr r3, [r1]
	ldrh r2, [r3, r0]
	add r2, r2, #1
	strh r2, [r3, r0]
	ldr r3, [r1]
	sub r1, r0, #2
	ldrh r2, [r3, r0]
	ldrh r1, [r3, r1]
	cmp r2, r1
	bne _02236880
	add r0, #0x1e
	ldr r1, [r3, r0]
	cmp r1, #0
	beq _0223687C
	mov r0, #2
	blx r1
_0223687C:
	mov r0, #1
	pop {r3, r4, r5, pc}
_02236880:
	mov r0, #0
	pop {r3, r4, r5, pc}
	.balign 4, 0
_02236884: .word ov74_0223E2FC
_02236888: .word 0x0000115A
_0223688C: .word 0x00001164
_02236890: .word 0x0000117C
_02236894: .word 0x0000FFFF
_02236898: .word 0x00001048
_0223689C: .word 0x00001168
_022368A0: .word 0x0000115E
	thumb_func_end ov74_02236768




	thumb_func_start ov74_022368A4
ov74_022368A4: ; 0x022368A4
	push {r3, r4}
	mov r2, #0
	add r3, r2, #0
	lsr r1, r1, #1
	beq _022368CC
_022368AE:
	lsl r4, r3, #1
	ldrh r4, [r0, r4]
	add r3, r3, #1
	lsl r3, r3, #0x10
	add r2, r2, r4
	lsl r2, r2, #0x10
	lsr r2, r2, #0x10
	lsl r4, r2, #1
	asr r2, r2, #0xf
	orr r2, r4
	lsl r2, r2, #0x10
	lsr r3, r3, #0x10
	lsr r2, r2, #0x10
	cmp r3, r1
	blo _022368AE
_022368CC:
	add r0, r2, #0
	pop {r3, r4}
	bx lr
	.balign 4, 0
	thumb_func_end ov74_022368A4




	thumb_func_start ov74_022368D4
ov74_022368D4: ; 0x022368D4
	push {r4, lr}
	sub sp, #8
	ldr r0, _02236968 ; =ov74_0223E2FC
	add r1, sp, #0
	ldr r4, [r0]
	ldr r0, _0223696C ; =0x00001170
	add r3, r4, r0
	ldrh r2, [r3]
	sub r0, #0x16
	strh r2, [r1]
	ldrh r2, [r3, #2]
	strh r2, [r1, #6]
	ldrh r2, [r3, #4]
	add r3, sp, #0
	strh r2, [r1, #4]
	ldrh r0, [r4, r0]
	ldr r2, _02236970 ; =0x00003FA2
	strh r0, [r1, #2]
	mov r1, #0
_022368FA:
	ldrh r0, [r3]
	add r1, r1, #1
	eor r0, r2
	strh r0, [r3]
	ldrh r2, [r3]
	add r3, r3, #2
	cmp r1, #4
	blt _022368FA
	ldr r0, _02236968 ; =ov74_0223E2FC
	mov r2, #8
	ldr r1, [r0]
	ldr r0, _02236974 ; =0x0000104C
	add r0, r1, r0
	add r1, sp, #0
	bl CRYPTO_RC4Init
	ldr r0, _02236968 ; =ov74_0223E2FC
	ldr r2, _02236978 ; =0x00001178
	ldr r3, [r0]
	ldr r0, _02236974 ; =0x0000104C
	ldr r1, [r3, r2]
	sub r2, #0x18
	ldr r2, [r3, r2]
	add r0, r3, r0
	add r3, r1, #0
	bl CRYPTO_RC4Encrypt
	ldr r1, _02236968 ; =ov74_0223E2FC
	ldr r2, _02236974 ; =0x0000104C
	ldr r1, [r1]
	mov r0, #0
	add r1, r1, r2
	lsr r2, r2, #4
	bl MIi_CpuClear32
	ldr r0, _02236968 ; =ov74_0223E2FC
	ldr r1, _02236978 ; =0x00001178
	ldr r2, [r0]
	ldr r0, [r2, r1]
	sub r1, #0x18
	ldr r1, [r2, r1]
	bl ov74_022368A4
	ldr r1, _02236968 ; =ov74_0223E2FC
	ldr r2, [r1]
	ldr r1, _0223697C ; =0x0000115A
	ldrh r1, [r2, r1]
	cmp r1, r0
	bne _02236962
	add sp, #8
	mov r0, #1
	pop {r4, pc}
_02236962:
	mov r0, #0
	add sp, #8
	pop {r4, pc}
	.balign 4, 0
_02236968: .word ov74_0223E2FC
_0223696C: .word 0x00001170
_02236970: .word 0x00003FA2
_02236974: .word 0x0000104C
_02236978: .word 0x00001178
_0223697C: .word 0x0000115A
	thumb_func_end ov74_022368D4
