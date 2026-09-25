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
