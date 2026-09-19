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
	.public ov74_02229200
	.public ov74_02229294
	.public ov74_02229450
	.public ov74_02229DA0
	.public ov74_02229DCC
	.public ov74_02229DD4
	.public ov74_02229DE0
	.public ov74_02229E60
	.public ov74_02229F90
	.public ov74_02229FF4
	.public ov74_0222A2A4
	.public ov74_0222A2E0
	.public ov74_0222A35C
	.public ov74_0222A3CC
	.public ov74_0222A494
	.public ov74_0222A4CC
	.public ov74_0222A504
	.public ov74_0222A538
	.public ov74_0222A560
	.public ov74_0222A5AC
	.public ov74_0222A5CC
	.public ov74_0222A650
	.public ov74_0222A688
	.public ov74_0222A6C0
	.public ov74_0222ACD8
	.public ov74_0222AE6C
	.public ov74_0222C2EC
	.public ov74_0222CD94
	.public ov74_0222D2D4
	.public ov74_0222D308
	.public ov74_0222D358
	.public ov74_0222D414
	.public ov74_0222D490
	.public ov74_0222D494
	.public ov74_0222D4E4
	.public ov74_0222D55C
	.public ov74_0222D5C4
	.public ov74_0222D614
	.public ov74_0222D690
	.public ov74_0222D774
	.public ov74_0222D78C
	.public ov74_0222D7D0
	.public ov74_0222DDFC
	.public ov74_0222E1F4
	.public ov74_0222EB44
	.public ov74_0222EE08
	.public ov74_0222EE0C
	.public ov74_0222EE58
	.public ov74_0222EEB0
	.public ov74_0222EF18
	.public ov74_0222EF64
	.public ov74_0222EF68
	.public ov74_0222F598
	.public ov74_0222F7D4
	.public ov74_0222FB8C
	.public ov74_02230018
	.public ov74_02230030
	.public ov74_02230070
	.public ov74_022300A8
	.public ov74_02230110
	.public ov74_02230138
	.public ov74_02230404
	.public ov74_02230520
	.public ov74_02230590
	.public ov74_022305E4
	.public ov74_022306C8
	.public ov74_022308E0
	.public ov74_02230964
	.public ov74_02230988
	.public ov74_022309AC
	.public ov74_022309F0
	.extern _0223B330
	.extern _0223B340
	.extern _0223B368
	.extern _0223B390
	.extern _0223B3A0
	.extern _0223B3BC
	.extern _0223B410
	.extern _0223B420
	.extern _0223B430
	.extern _0223B440
	.extern _0223B45C
	.extern _0223B478
	.extern _0223B494
	.extern _0223B4B0
	.extern gApp_MainMenu_SelectOption_ConnectToRanger
	.extern gApp_MainMenu_SelectOption_MigrateFromAgb
	.extern gApp_MainMenu_SelectOption_MysteryGift
	.extern ov74_02231048
	.extern ov74_02231054
	.extern ov74_0223105C
	.extern ov74_02231064
	.extern ov74_02231070
	.extern ov74_0223107C
	.extern ov74_02231088
	.extern ov74_02231094
	.extern ov74_022310A0
	.extern ov74_022310B8
	.extern ov74_022310C4
	.extern ov74_02231100
	.extern ov74_0223110C
	.extern ov74_02231118
	.extern ov74_02231124
	.extern ov74_02231130
	.extern ov74_0223113C
	.extern ov74_02231154
	.extern ov74_0223115C
	.extern ov74_02231164
	.extern ov74_02231184
	.extern ov74_02231194
	.extern ov74_022311A0
	.extern ov74_022311AC
	.extern ov74_022311BC
	.extern ov74_022311CC
	.extern ov74_022311DC
	.extern ov74_022311F4
	.extern ov74_02231214
	.extern ov74_02231238
	.extern ov74_02231260
	.extern ov74_022312C0
	.extern ov74_022313F0
	.extern ov74_02231424
	.extern ov74_02231448
	.extern ov74_0223144C
	.extern ov74_02231450
	.extern ov74_02231454
	.extern ov74_02231458
	.extern ov74_0223145C
	.extern ov74_02231460
	.extern ov74_022314A0
	.extern ov74_022314BC
	.extern ov74_022314DC
	.extern ov74_02231508
	.extern ov74_02231544
	.extern ov74_02231560
	.extern ov74_02231584
	.extern ov74_0223161C
	.extern ov74_02231638
	.extern ov74_02231670
	.extern ov74_022316E8
	.extern ov74_02231704
	.extern ov74_02231724
	.extern ov74_02231744
	.extern ov74_0223512C
	.extern ov74_02235230
	.extern ov74_02235258
	.extern ov74_022352A0
	.extern ov74_022352D0
	.extern ov74_02235308
	.extern ov74_02235390
	.extern ov74_0223539C
	.extern ov74_022353FC
	.extern ov74_02235414
	.extern ov74_02235464
	.extern ov74_0223546C
	.extern ov74_02235474
	.extern ov74_02235568
	.extern ov74_02235634
	.extern ov74_0223563C
	.extern ov74_0223567C
	.extern ov74_02235690
	.extern ov74_02235708
	.extern ov74_02235728
	.extern ov74_022358BC
	.extern ov74_022358C8
	.extern ov74_02235930
	.extern ov74_022359BC
	.extern ov74_02235A68
	.extern ov74_02235DC4
	.extern ov74_02235ED0
	.extern ov74_02235F58
	.extern ov74_02236034
	.extern ov74_022360A0
	.extern ov74_022360B0
	.extern ov74_02236128
	.extern ov74_02236140
	.extern ov74_0223615C
	.extern ov74_022365FC
	.extern ov74_02236680
	.extern ov74_022368D4
	.extern ov74_02236980
	.extern ov74_02236988
	.extern ov74_02236AE0
	.extern ov74_0223BD5C
	.extern ov74_0223BD68
	.extern ov74_0223C1F4
	.extern ov74_0223C220
	.extern ov74_0223C230
	.extern ov74_0223C240
	.extern ov74_0223C250
	.extern ov74_0223C260
	.extern ov74_0223C270
	.extern ov74_0223C2A0
	.extern ov74_0223C2B0
	.extern ov74_0223C2D0
	.extern ov74_0223C2E0
	.extern ov74_0223C2F0
	.extern ov74_0223C320
	.extern ov74_0223C340
	.extern ov74_0223C6D0
	.extern ov74_0223C6E0
	.extern ov74_0223C700
	.extern ov74_0223D080
	.extern ov74_0223D090
	.extern ov74_0223D0A4
	.extern ov74_0223D0A8
	.extern ov74_0223D0BC
	.extern ov74_0223D0C0

	.extern ov74_02228C84
	.extern ov74_IntToWstring5
	.extern ov74_02228D20
	.extern ov74_02228D64
	.extern ov74_02228E98
	.extern ov74_02228F14
	.extern ov74_02228F8C
	.extern ov74_02229084
	.extern ov74_02229190
	.extern ov74_02229200
	.extern ov74_02229294
	.extern ov74_02229450
	.extern ov74_0222947C
	.extern ov74_022295C8
	.extern ov74_022295D0
	.extern ov74_0222962C
	.extern ov74_02229B68
	.extern ov74_02229BC0
	.extern ov74_02229BD0
	.extern ov74_02229C0C
	.extern ov74_02229C44
	.extern ov74_02229C5C
	.extern ov74_02229C60
	.extern ov74_02229C80
	.extern ov74_02229CB4
	.extern ov74_02229CB8
	.extern ov74_02229CE0
	.extern ov74_02229D0C
	.extern ov74_02229D1C
	.extern ov74_02229D50
	.extern ov74_02229D6C
	.extern ov74_02229DA0
	.extern ov74_02229DBC
	.extern ov74_02229DCC
	.extern ov74_02229DD4
	.extern ov74_02229DE0
	.extern ov74_02229DF8
	.extern ov74_02229E14
	.extern ov74_02229E28
	.extern ov74_02229E60
	.extern ov74_02229E68
	.extern ov74_02229F04
	.extern ov74_02229F28
	.extern ov74_02229F60
	.extern ov74_02229F78
	.extern ov74_02229F90
	.extern ov74_02229FF4
	.extern ov74_0222A078
	.extern ov74_0222A0E4
	.extern ov74_0222A130
	.extern ov74_0222A174
	.extern ov74_0222A240
	.extern ov74_0222A2A4
	.extern ov74_0222A2E0
	.extern ov74_0222A35C
	.extern ov74_0222A3CC
	.extern ov74_0222A43C
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
	.extern ov74_0222A6F8
	.extern ov74_0222A744
	.extern ov74_0222A7A0
	.extern ov74_0222A81C
	.extern ov74_0222A89C
	.extern ov74_0222A94C
	.extern ov74_0222AA18
	.extern ov74_0222AAAC
	.extern ov74_0222AB0C
	.extern ov74_0222AB70
	.extern ov74_0222AC1C
	.extern ov74_0222ACD8
	.extern ov74_0222AD6C
	.extern ov74_0222ADBC
	.extern ov74_0222AE3C
	.extern ov74_0222AE6C
	.extern ov74_0222AF28
	.extern ov74_0222B0C8
	.extern ov74_0222B144
	.extern ov74_0222B19C
	.extern ov74_0222B1F4
	.extern ov74_0222B20C
	.extern ov74_0222B224
	.extern ov74_0222B288
	.extern ov74_0222B2C4
	.extern ov74_0222B30C
	.extern ov74_0222B344
	.extern ov74_0222B374
	.extern ov74_0222B3A4
	.extern ov74_0222B5F8
	.extern ov74_0222B760
	.extern ov74_0222B950
	.extern ov74_0222BA48
	.extern ov74_0222BED8
	.extern ov74_0222BEF0
	.extern ov74_0222BF08
	.extern ov74_0222BF18
	.extern ov74_0222BF2C
	.extern ov74_0222BFA0
	.extern ov74_0222C014
	.extern ov74_0222C04C
	.extern ov74_0222C2EC
	.extern ov74_0222CD7C
	.extern ov74_0222CD88
	.extern ov74_0222CD94
	.extern ov74_0222CE10
	.extern ov74_0222CE6C
	.extern ov74_0222CEC0
	.extern ov74_0222CEE0
	.extern ov74_0222CFFC
	.extern ov74_0222D024
	.extern ov74_0222D098
	.extern ov74_0222D0EC
	.extern ov74_0222D104
	.extern ov74_0222D11C
	.extern ov74_0222D1D4
	.extern ov74_0222D248
	.extern ov74_0222D2D4
	.extern ov74_0222D308
	.extern ov74_0222D358
	.extern ov74_0222D414
	.extern ov74_0222D448
	.extern ov74_0222D490
	.extern ov74_0222D494
	.extern ov74_0222D4E4
	.extern ov74_0222D55C
	.extern ov74_0222D5C4
	.extern ov74_0222D614
	.extern ov74_0222D690
	.extern ov74_0222D774
	.extern ov74_0222D78C
	.extern ov74_0222D7A4
	.extern ov74_0222D7D0
	.extern ov74_0222D7F0
	.extern ov74_0222D824
	.extern ov74_0222D9E0
	.extern ov74_0222DAF8
	.extern ov74_0222DB30
	.extern ov74_0222DB70
	.extern ov74_0222DC60
	.extern ov74_0222DCD4
	.extern ov74_0222DDFC
	.extern ov74_0222DE68
	.extern ov74_0222DE8C
	.extern ov74_0222DEF0
	.extern ov74_0222DF2C
	.extern ov74_0222E03C
	.extern ov74_0222E060
	.extern ov74_0222E0D4
	.extern ov74_0222E1F4
	.extern ov74_0222E7EC
	.extern ov74_0222E85C
	.extern ov74_0222E898
	.extern ov74_0222E8B4
	.extern ov74_0222EA88
	.extern ov74_0222EB28
	.extern ov74_0222EB44
	.extern ov74_0222EC08
	.extern ov74_0222EC60
	.extern ov74_0222ECD4
	.extern ov74_0222ECEC
	.extern ov74_0222ED04
	.extern ov74_0222EDC0
	.extern ov74_0222EE08
	.extern ov74_0222EE0C
	.extern ov74_0222EE58
	.extern ov74_0222EEB0
	.extern ov74_0222EF18
	.extern ov74_0222EF64
	.extern ov74_0222EF68
	.extern ov74_0222EFF0
	.extern ov74_0222F024
	.extern ov74_0222F1BC
	.extern ov74_0222F2D4
	.extern ov74_0222F314
	.extern ov74_0222F404
	.extern ov74_0222F478
	.extern ov74_0222F598
	.extern ov74_0222F600
	.extern ov74_0222F624
	.extern ov74_0222F688
	.extern ov74_0222F6C4
	.extern ov74_0222F7D4
	.extern ov74_0222FB8C
	.extern ov74_0222FC50
	.extern ov74_0222FCA4
	.extern ov74_0222FCC4
	.extern ov74_0222FD98
	.extern ov74_0222FE4C
	.extern ov74_0222FE5C
	.extern ov74_0222FE68
	.extern ov74_0222FE78
	.extern ov74_0222FEA0
	.extern ov74_0222FEC8
	.extern ov74_0222FF28
	.extern ov74_0222FF68
	.extern ov74_0222FF80
	.extern ov74_0222FFAC
	.extern ov74_0222FFFC
	.extern ov74_02230018
	.extern ov74_02230030
	.extern ov74_02230070
	.extern ov74_022300A8
	.extern ov74_02230110
	.extern ov74_02230138
	.extern ov74_0223026C
	.extern ov74_0223030C
	.extern ov74_02230320
	.extern ov74_02230404
	.extern ov74_02230478
	.extern ov74_02230520
	.extern ov74_02230590
	.extern ov74_022305E4
	.extern ov74_02230674
	.extern ov74_022306C8
	.extern ov74_02230714
	.extern ov74_0223078C
	.extern ov74_02230818
	.extern ov74_022308E0
	.extern ov74_02230964
	.extern ov74_02230988
	.extern ov74_022309AC
	.extern ov74_022309F0
	.extern ov74_02230A14
	.extern ov74_02230A34
	.extern ov74_02230A4C
	.extern ov74_02230A74
	.extern ov74_02230A7C
	.extern ov74_02230A84
	.extern ov74_02230BB4
	.extern ov74_02230C10
	.extern ov74_02230CCC
	.extern ov74_02230CEC
	.extern ov74_02230D18
	.extern ov74_02230D28
	.extern ov74_02230D6C
	.extern ov74_02230D80
	.extern ov74_02230DB8
	.extern ov74_02230DF4
	.extern ov74_02230E44
	.extern ov74_02230E7C
	.extern ov74_02230E94
	.extern ov74_02230EB4
	.extern ov74_02230EE8
	.extern ov74_02230F14
	.extern ov74_02230F40
	.extern ov74_02230F6C
	.extern ov74_02230F98
	.extern ov74_02230FD4
	.extern ov74_02231008



	thumb_func_start ov74_0222E060
ov74_0222E060: ; 0x0222E060
	push {r4, lr}
	add r4, r0, #0
	mov r0, #0xaf
	lsl r0, r0, #6
	ldr r0, [r4, r0]
	cmp r0, #3
	blt _0222E072
	bl GF_AssertFail
_0222E072:
	mov r0, #0x1b
	bl SaveSubstruct_AssertCRC
	mov r0, #0xaf
	lsl r0, r0, #6
	ldr r1, [r4, r0]
	sub r0, #0xc
	lsl r1, r1, #2
	add r1, r4, r1
	ldr r2, [r1, r0]
	mov r0, #0x35
	lsl r0, r0, #4
	ldrb r1, [r2, r0]
	cmp r1, #0xff
	beq _0222E094
	add r1, r1, #1
	strb r1, [r2, r0]
_0222E094:
	mov r0, #0xaf
	lsl r0, r0, #6
	ldr r1, [r4, r0]
	sub r0, #0xc
	lsl r1, r1, #2
	add r1, r4, r1
	ldr r2, [r1, r0]
	mov r0, #0xd2
	lsl r0, r0, #2
	ldrb r1, [r2, r0]
	cmp r1, #0xff
	beq _0222E0B0
	sub r1, r1, #1
	strb r1, [r2, r0]
_0222E0B0:
	mov r0, #0x1b
	bl SaveSubstruct_UpdateCRC
	ldr r0, _0222E0C8 ; =0x00002BA4
	ldr r0, [r4, r0]
	bl ov74_022360A0
	ldr r1, _0222E0CC ; =ov74_0222E03C
	ldr r0, _0222E0D0 ; =0x00003D48
	str r1, [r4, r0]
	pop {r4, pc}
	nop
_0222E0C8: .word 0x00002BA4
_0222E0CC: .word ov74_0222E03C
_0222E0D0: .word 0x00003D48
	thumb_func_end ov74_0222E060


	thumb_func_start ov74_0222E0D4
ov74_0222E0D4: ; 0x0222E0D4
	push {r3, r4, r5, r6, r7, lr}
	add r7, r2, #0
	ldr r2, _0222E1D4 ; =gSystem
	add r6, r1, #0
	ldr r3, [r2, #0x48]
	ldr r1, _0222E1D8 ; =0x00003D4C
	add r5, r0, #0
	mov r2, #0x10
	ldr r0, [r5, r1]
	tst r2, r3
	beq _0222E0F2
	cmp r0, #1
	beq _0222E0F2
	mov r2, #1
	str r2, [r5, r1]
_0222E0F2:
	ldr r1, _0222E1D4 ; =gSystem
	ldr r2, [r1, #0x48]
	mov r1, #0x20
	tst r1, r2
	beq _0222E108
	ldr r1, _0222E1D8 ; =0x00003D4C
	ldr r2, [r5, r1]
	cmp r2, #0
	beq _0222E108
	mov r2, #0
	str r2, [r5, r1]
_0222E108:
	ldr r1, _0222E1D8 ; =0x00003D4C
	ldr r1, [r5, r1]
	cmp r0, r1
	beq _0222E138
	cmp r1, #0
	bne _0222E118
	mov r1, #1
	b _0222E11A
_0222E118:
	mov r1, #0
_0222E11A:
	ldr r0, _0222E1DC ; =0x00002DC4
	ldr r0, [r5, r0]
	bl Sprite_SetAnimCtrlSeq
	ldr r0, _0222E1D8 ; =0x00003D4C
	ldr r0, [r5, r0]
	cmp r0, #0
	beq _0222E12E
	mov r1, #1
	b _0222E130
_0222E12E:
	mov r1, #0
_0222E130:
	ldr r0, _0222E1E0 ; =0x00002DC8
	ldr r0, [r5, r0]
	bl Sprite_SetAnimCtrlSeq
_0222E138:
	ldr r0, _0222E1D4 ; =gSystem
	mov r4, #0
	ldr r1, [r0, #0x48]
	mov r0, #2
	add r2, r1, #0
	tst r2, r0
	beq _0222E14A
	add r4, r0, #0
	b _0222E17A
_0222E14A:
	mov r0, #1
	and r1, r0
	beq _0222E160
	cmp r6, #0
	beq _0222E160
	ldr r2, _0222E1D8 ; =0x00003D4C
	ldr r2, [r5, r2]
	cmp r2, #0
	bne _0222E160
	add r4, r0, #0
	b _0222E17A
_0222E160:
	cmp r1, #0
	beq _0222E170
	ldr r0, _0222E1D8 ; =0x00003D4C
	ldr r0, [r5, r0]
	cmp r0, #1
	bne _0222E170
	mov r4, #2
	b _0222E17A
_0222E170:
	cmp r1, #0
	beq _0222E17A
	cmp r6, #0
	bne _0222E17A
	mov r4, #3
_0222E17A:
	cmp r4, #1
	bne _0222E1B2
	ldr r0, _0222E1E4 ; =SEQ_SE_DP_SELECT
	bl PlaySE
	mov r0, #0xab
	bl sub_02037AC0
	mov r3, #0xa
	ldr r1, _0222E1E8 ; =0x00002C34
	mov r0, #1
	str r0, [r5, r1]
	mov r0, #0x16
	sub r1, #0x64
	str r0, [r7]
	add r0, r5, #0
	add r1, r5, r1
	mov r2, #0x11
	lsl r3, r3, #6
	bl ov74_0222D9E0
	ldr r0, _0222E1EC ; =0x00002BD0
	mov r1, #0x13
	add r0, r5, r0
	bl WaitingIcon_New
	ldr r1, _0222E1F0 ; =0x00003D50
	str r0, [r5, r1]
_0222E1B2:
	cmp r4, #2
	bne _0222E1C6
	ldr r0, _0222E1E4 ; =SEQ_SE_DP_SELECT
	bl PlaySE
	add r0, r5, #0
	add r1, r7, #0
	mov r2, #0x14
	bl ov74_0222EB28
_0222E1C6:
	cmp r4, #3
	bne _0222E1D0
	ldr r0, _0222E1E4 ; =SEQ_SE_DP_SELECT
	bl PlaySE
_0222E1D0:
	pop {r3, r4, r5, r6, r7, pc}
	nop
_0222E1D4: .word gSystem
_0222E1D8: .word 0x00003D4C
_0222E1DC: .word 0x00002DC4
_0222E1E0: .word 0x00002DC8
_0222E1E4: .word SEQ_SE_DP_SELECT
_0222E1E8: .word 0x00002C34
_0222E1EC: .word 0x00002BD0
_0222E1F0: .word 0x00003D50
	thumb_func_end ov74_0222E0D4


	thumb_func_start ov74_0222E1F4
ov74_0222E1F4: ; 0x0222E1F4
	push {r4, r5, r6, lr}
	sub sp, #8
	add r5, r1, #0
	add r6, r0, #0
	bl OverlayManager_GetData
	ldr r1, [r5]
	add r4, r0, #0
	cmp r1, #0x1e
	bls _0222E20A
	b _0222E79C
_0222E20A:
	add r1, r1, r1
	add r1, pc
	ldrh r1, [r1, #6]
	lsl r1, r1, #0x10
	asr r1, r1, #0x10
	add pc, r1
_0222E216: ; jump table
	.short _0222E254 - _0222E216 - 2 ; case 0
	.short _0222E2C2 - _0222E216 - 2 ; case 1
	.short _0222E2D4 - _0222E216 - 2 ; case 2
	.short _0222E34A - _0222E216 - 2 ; case 3
	.short _0222E3C8 - _0222E216 - 2 ; case 4
	.short _0222E3F0 - _0222E216 - 2 ; case 5
	.short _0222E410 - _0222E216 - 2 ; case 6
	.short _0222E444 - _0222E216 - 2 ; case 7
	.short _0222E468 - _0222E216 - 2 ; case 8
	.short _0222E48E - _0222E216 - 2 ; case 9
	.short _0222E4BA - _0222E216 - 2 ; case 10
	.short _0222E4F4 - _0222E216 - 2 ; case 11
	.short _0222E518 - _0222E216 - 2 ; case 12
	.short _0222E744 - _0222E216 - 2 ; case 13
	.short _0222E524 - _0222E216 - 2 ; case 14
	.short _0222E530 - _0222E216 - 2 ; case 15
	.short _0222E540 - _0222E216 - 2 ; case 16
	.short _0222E54C - _0222E216 - 2 ; case 17
	.short _0222E58E - _0222E216 - 2 ; case 18
	.short _0222E5E8 - _0222E216 - 2 ; case 19
	.short _0222E646 - _0222E216 - 2 ; case 20
	.short _0222E652 - _0222E216 - 2 ; case 21
	.short _0222E6A2 - _0222E216 - 2 ; case 22
	.short _0222E68E - _0222E216 - 2 ; case 23
	.short _0222E6D6 - _0222E216 - 2 ; case 24
	.short _0222E6F2 - _0222E216 - 2 ; case 25
	.short _0222E734 - _0222E216 - 2 ; case 26
	.short _0222E76E - _0222E216 - 2 ; case 27
	.short _0222E77A - _0222E216 - 2 ; case 28
	.short _0222E78C - _0222E216 - 2 ; case 29
	.short _0222E79C - _0222E216 - 2 ; case 30
_0222E254:
	add r0, r6, #0
	bl OverlayManager_GetArgs
	ldr r1, [r0, #8]
	ldr r0, _0222E5A4 ; =0x00002BA4
	str r1, [r4, r0]
	ldr r0, [r4, r0]
	bl Save_MysteryGift_Get
	ldr r1, _0222E5A8 ; =0x00002BA0
	str r0, [r4, r1]
	add r0, r1, #4
	ldr r0, [r4, r0]
	bl Save_PlayerData_GetOptionsAddr
	ldr r1, _0222E5AC ; =0x00002BA8
	str r0, [r4, r1]
	ldr r0, [r4, r1]
	bl Options_GetFrame
	ldr r1, _0222E5B0 ; =0x00002BAC
	str r0, [r4, r1]
	sub r1, #0xc
	ldr r0, [r4, r1]
	mov r1, #0
	bl SaveMysteryGift_CardGetByIdx
	ldr r1, _0222E5B4 ; =0x00002BB4
	str r0, [r4, r1]
	sub r1, #0x14
	ldr r0, [r4, r1]
	mov r1, #1
	bl SaveMysteryGift_CardGetByIdx
	ldr r1, _0222E5B8 ; =0x00002BB8
	str r0, [r4, r1]
	sub r1, #0x18
	ldr r0, [r4, r1]
	mov r1, #2
	bl SaveMysteryGift_CardGetByIdx
	ldr r1, _0222E5BC ; =0x00002BBC
	mov r2, #1
	str r0, [r4, r1]
	add r1, r1, #4
	ldr r1, [r4, r1]
	add r0, r4, #0
	bl ov74_0222DAF8
	mov r1, #0xaf
	lsl r1, r1, #6
	str r0, [r4, r1]
	mov r0, #1
	str r0, [r5]
	b _0222E79C
_0222E2C2:
	bl ov74_0222FCA4
	ldr r0, _0222E5C0 ; =0x000029FC
	ldr r0, [r4, r0]
	bl ov74_0222FCC4
	mov r0, #2
	str r0, [r5]
	b _0222E79C
_0222E2D4:
	bl ResetAllTextPrinters
	add r0, r4, #0
	mov r1, #0
	bl ov74_0222DB70
	mov r1, #0x1e
	mov r0, #0
	lsl r1, r1, #4
	mov r2, #0x55
	bl LoadFontPal0
	mov r1, #0
	str r1, [sp]
	mov r0, #0x55
	str r0, [sp, #4]
	ldr r0, _0222E5C0 ; =0x000029FC
	mov r2, #1
	ldr r0, [r4, r0]
	mov r3, #0xd
	bl LoadUserFrameGfx1
	mov r0, #1
	str r0, [sp]
	mov r0, #0x55
	str r0, [sp, #4]
	ldr r0, _0222E5C0 ; =0x000029FC
	mov r1, #0
	ldr r0, [r4, r0]
	mov r2, #0xa
	mov r3, #0xe
	bl LoadUserFrameGfx1
	ldr r0, _0222E5B0 ; =0x00002BAC
	mov r1, #0
	ldr r0, [r4, r0]
	mov r2, #0x13
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	str r0, [sp]
	mov r0, #0x55
	str r0, [sp, #4]
	ldr r0, _0222E5C0 ; =0x000029FC
	mov r3, #0xa
	ldr r0, [r4, r0]
	bl LoadUserFrameGfx2
	add r0, r4, #0
	mov r1, #1
	mov r2, #0
	bl ov74_0222D824
	add r0, r4, #0
	mov r1, #1
	mov r2, #3
	add r3, r5, #0
	bl ov74_0222D7F0
	b _0222E79C
_0222E34A:
	mov r1, #0xaf
	lsl r1, r1, #6
	ldr r6, [r4, r1]
	ldr r1, _0222E5C4 ; =gSystem
	mov r2, #0x40
	ldr r1, [r1, #0x48]
	add r3, r1, #0
	tst r3, r2
	beq _0222E368
	add r1, r6, #0
	sub r2, #0x41
	bl ov74_0222DAF8
	add r6, r0, #0
	b _0222E3A4
_0222E368:
	mov r2, #0x80
	tst r2, r1
	beq _0222E37A
	add r1, r6, #0
	mov r2, #1
	bl ov74_0222DAF8
	add r6, r0, #0
	b _0222E3A4
_0222E37A:
	mov r0, #2
	tst r0, r1
	beq _0222E394
	ldr r0, _0222E5C8 ; =SEQ_SE_DP_SELECT
	bl PlaySE
	add r0, r4, #0
	mov r1, #0
	mov r2, #0x1d
	add r3, r5, #0
	bl ov74_0222D7F0
	b _0222E3A4
_0222E394:
	mov r0, #1
	tst r0, r1
	beq _0222E3A4
	ldr r0, _0222E5C8 ; =SEQ_SE_DP_SELECT
	bl PlaySE
	mov r0, #4
	str r0, [r5]
_0222E3A4:
	mov r0, #0xaf
	lsl r0, r0, #6
	ldr r0, [r4, r0]
	cmp r0, r6
	bne _0222E3B0
	b _0222E79C
_0222E3B0:
	ldr r0, _0222E5CC ; =SEQ_SE_DP_CARD2
	bl PlaySE
	mov r0, #0xaf
	lsl r0, r0, #6
	str r6, [r4, r0]
	add r0, r4, #0
	mov r1, #1
	mov r2, #0
	bl ov74_0222D824
	b _0222E79C
_0222E3C8:
	ldr r1, _0222E5D0 ; =0x00002BD0
	mov r3, #0xa
	add r1, r4, r1
	mov r2, #7
	lsl r3, r3, #6
	bl ov74_0222D9E0
	ldr r3, _0222E5D4 ; =0x00002BF0
	mov r2, #8
	add r1, r3, #0
	str r0, [r4, r3]
	sub r1, #0x10
	ldr r3, [r4, r3]
	add r0, r4, #0
	add r1, r4, r1
	bl ov74_0222D9E0
	mov r0, #5
	str r0, [r5]
	b _0222E79C
_0222E3F0:
	add r0, r6, #0
	add r1, r5, #0
	mov r2, #0
	bl ov74_0222DC60
	ldr r0, _0222E5C4 ; =gSystem
	ldr r1, [r0, #0x48]
	mov r0, #2
	tst r0, r1
	beq _0222E4C2
	ldr r0, _0222E5C8 ; =SEQ_SE_DP_SELECT
	bl PlaySE
	mov r0, #0xb
	str r0, [r5]
	b _0222E79C
_0222E410:
	ldr r0, _0222E5CC ; =SEQ_SE_DP_CARD2
	bl PlaySE
	mov r0, #0x10
	mov r1, #0
	bl GfGfx_EngineATogglePlanes
	ldr r0, _0222E5D0 ; =0x00002BD0
	mov r1, #0
	add r0, r4, r0
	bl ov74_0222D104
	ldr r0, _0222E5D8 ; =0x00002BE0
	mov r1, #0
	add r0, r4, r0
	bl ov74_0222D0EC
	mov r1, #1
	add r0, r4, #0
	lsl r2, r1, #0xc
	mov r3, #0x66
	bl ov74_0222DE8C
	mov r0, #7
	str r0, [r5]
	b _0222E79C
_0222E444:
	bl ov74_0222DF2C
	cmp r0, #0
	beq _0222E4C2
	mov r1, #1
	add r0, r4, #0
	add r2, r1, #0
	bl ov74_0222D824
	ldr r2, _0222E5DC ; =0x00708000
	add r0, r4, #0
	mov r1, #0
	lsr r3, r2, #1
	bl ov74_0222DE8C
	mov r0, #8
	str r0, [r5]
	b _0222E79C
_0222E468:
	bl ov74_0222DF2C
	ldr r0, _0222E5C4 ; =gSystem
	ldr r1, [r0, #0x48]
	mov r0, #3
	tst r0, r1
	beq _0222E4C2
	ldr r0, _0222E5CC ; =SEQ_SE_DP_CARD2
	bl PlaySE
	mov r1, #1
	add r0, r4, #0
	lsl r2, r1, #0xc
	mov r3, #0x66
	bl ov74_0222DE8C
	mov r0, #9
	str r0, [r5]
	b _0222E79C
_0222E48E:
	bl ov74_0222DF2C
	cmp r0, #0
	beq _0222E4C2
	add r0, r4, #0
	mov r1, #1
	mov r2, #0
	bl ov74_0222D824
	ldr r2, _0222E5DC ; =0x00708000
	add r0, r4, #0
	mov r1, #0
	lsr r3, r2, #1
	bl ov74_0222DE8C
	mov r0, #0x10
	mov r1, #0
	bl GfGfx_EngineATogglePlanes
	mov r0, #0xa
	str r0, [r5]
	b _0222E79C
_0222E4BA:
	bl ov74_0222DF2C
	cmp r0, #0
	bne _0222E4C4
_0222E4C2:
	b _0222E79C
_0222E4C4:
	ldr r0, _0222E5D0 ; =0x00002BD0
	mov r1, #0
	add r0, r4, r0
	mov r2, #0x13
	mov r3, #0xa
	bl DrawFrameAndWindow2
	ldr r0, _0222E5D8 ; =0x00002BE0
	mov r1, #0
	add r0, r4, r0
	mov r2, #0xa
	mov r3, #0xe
	bl DrawFrameAndWindow1
	mov r0, #0x10
	mov r1, #1
	bl GfGfx_EngineATogglePlanes
	add r0, r4, #0
	bl ov74_0222DEF0
	mov r0, #5
	str r0, [r5]
	b _0222E79C
_0222E4F4:
	bl ov74_0222D448
	ldr r0, _0222E5D0 ; =0x00002BD0
	mov r1, #0
	add r0, r4, r0
	bl ov74_0222D104
	ldr r0, _0222E5D0 ; =0x00002BD0
	add r0, r4, r0
	bl ClearWindowTilemapAndCopyToVram
	ldr r0, _0222E5D0 ; =0x00002BD0
	add r0, r4, r0
	bl RemoveWindow
	mov r0, #3
	str r0, [r5]
	b _0222E79C
_0222E518:
	ldr r2, _0222E5E0 ; =ov74_0222D414
	add r0, r6, #0
	add r1, r5, #0
	bl ov74_0222DC60
	b _0222E79C
_0222E524:
	ldr r2, _0222E5E0 ; =ov74_0222D414
	add r0, r6, #0
	add r1, r5, #0
	bl ov74_0222DC60
	b _0222E79C
_0222E530:
	ldr r1, _0222E5D8 ; =0x00002BE0
	ldr r2, _0222E5E4 ; =0x00010200
	add r1, r4, r1
	bl ov74_0222D7A4
	mov r0, #0xe
	str r0, [r5]
	b _0222E79C
_0222E540:
	mov r1, #0
	mov r2, #0x11
	add r3, r5, #0
	bl ov74_0222D7F0
	b _0222E79C
_0222E54C:
	bl ov74_0222D448
	ldr r0, _0222E5D0 ; =0x00002BD0
	mov r1, #0
	add r0, r4, r0
	bl ov74_0222D104
	ldr r0, _0222E5D0 ; =0x00002BD0
	add r0, r4, r0
	bl ClearWindowTilemapAndCopyToVram
	ldr r0, _0222E5D0 ; =0x00002BD0
	add r0, r4, r0
	bl RemoveWindow
	add r0, r4, #0
	mov r1, #0
	mov r2, #3
	bl ov74_0222D824
	ldr r0, _0222E5C0 ; =0x000029FC
	ldr r0, [r4, r0]
	bl ov74_0222D248
	add r0, r4, #0
	bl ov74_0222CFFC
	add r0, r4, #0
	bl ov74_0222D098
	mov r0, #0x12
	str r0, [r5]
	b _0222E79C
_0222E58E:
	bl ov74_0222E7EC
	bl sub_0203A880
	add r0, r4, #0
	mov r1, #1
	mov r2, #0x13
	add r3, r5, #0
	bl ov74_0222D7F0
	b _0222E79C
	.balign 4, 0
_0222E5A4: .word 0x00002BA4
_0222E5A8: .word 0x00002BA0
_0222E5AC: .word 0x00002BA8
_0222E5B0: .word 0x00002BAC
_0222E5B4: .word 0x00002BB4
_0222E5B8: .word 0x00002BB8
_0222E5BC: .word 0x00002BBC
_0222E5C0: .word 0x000029FC
_0222E5C4: .word gSystem
_0222E5C8: .word SEQ_SE_DP_SELECT
_0222E5CC: .word SEQ_SE_DP_CARD2
_0222E5D0: .word 0x00002BD0
_0222E5D4: .word 0x00002BF0
_0222E5D8: .word 0x00002BE0
_0222E5DC: .word 0x00708000
_0222E5E0: .word ov74_0222D414
_0222E5E4: .word 0x00010200
_0222E5E8:
	mov r6, #0
	add r0, r6, #0
	bl sub_020373B4
	cmp r0, #0
	beq _0222E63A
	add r0, r4, #0
	add r0, #0x90
	bl sub_020358B8
	ldr r1, _0222E7C0 ; =0x00002B08
	add r0, r4, #0
	add r1, r4, r1
	bl ov74_0222E8B4
	add r6, r0, #0
	bne _0222E62A
	ldr r0, _0222E7C4 ; =0x00002C30
	ldr r0, [r4, r0]
	cmp r0, #0
	beq _0222E62A
	ldr r0, _0222E7C0 ; =0x00002B08
	mov r1, #0
	add r0, r4, r0
	bl FillWindowPixelBuffer
	ldr r0, _0222E7C0 ; =0x00002B08
	add r0, r4, r0
	bl CopyWindowToVram
	ldr r0, _0222E7C4 ; =0x00002C30
	mov r1, #0
	str r1, [r4, r0]
_0222E62A:
	ldr r1, _0222E7C8 ; =0x00002AF8
	add r0, r4, #0
	add r1, r4, r1
	add r2, r6, #0
	bl ov74_0222EA88
	ldr r0, _0222E7C4 ; =0x00002C30
	str r6, [r4, r0]
_0222E63A:
	add r0, r4, #0
	add r1, r6, #0
	add r2, r5, #0
	bl ov74_0222E0D4
	b _0222E79C
_0222E646:
	mov r1, #0
	mov r2, #0x15
	add r3, r5, #0
	bl ov74_0222D7F0
	b _0222E79C
_0222E652:
	bl ov74_0222D024
	ldr r0, _0222E7CC ; =0x00002BD0
	mov r1, #0
	add r0, r4, r0
	bl ov74_0222D104
	ldr r0, _0222E7D0 ; =0x00002BE0
	mov r1, #0
	add r0, r4, r0
	bl ov74_0222D0EC
	add r0, r4, #0
	mov r1, #1
	mov r2, #0
	bl ov74_0222D824
	mov r1, #0x1e
	mov r0, #0
	lsl r1, r1, #4
	mov r2, #0x55
	bl LoadFontPal0
	add r0, r4, #0
	mov r1, #1
	mov r2, #4
	add r3, r5, #0
	bl ov74_0222D7F0
	b _0222E79C
_0222E68E:
	bl sub_02037D78
	cmp r0, #0
	beq _0222E698
	b _0222E79C
_0222E698:
	mov r0, #0xb1
	lsl r0, r0, #6
	ldr r0, [r4, r0]
	str r0, [r5]
	b _0222E79C
_0222E6A2:
	ldr r0, _0222E7D4 ; =0x00002C34
	ldr r1, [r4, r0]
	sub r1, r1, #1
	str r1, [r4, r0]
	ldr r0, [r4, r0]
	cmp r0, #0
	bne _0222E79C
	ldr r1, _0222E7D8 ; =0x00002DD8
	add r0, r4, #0
	ldr r2, [r4]
	add r0, #0x90
	add r1, r4, r1
	bl ov74_02235ED0
	ldr r0, _0222E7D8 ; =0x00002DD8
	mov r1, #0xd6
	add r0, r4, r0
	lsl r1, r1, #2
	bl ov74_02229D0C
	add r0, r4, #0
	bl ov74_0222E060
	mov r0, #0x18
	str r0, [r5]
	b _0222E79C
_0222E6D6:
	bl ov74_0223615C
	cmp r0, #4
	bne _0222E79C
	mov r0, #0x93
	bl sub_02037AC0
	mov r0, #1
	add r1, r0, #0
	bl sub_020398D4
	mov r0, #0x19
	str r0, [r5]
	b _0222E79C
_0222E6F2:
	bl ov74_0222E898
	cmp r0, #0
	beq _0222E704
	mov r0, #0x93
	bl sub_02037B38
	cmp r0, #1
	bne _0222E79C
_0222E704:
	mov r0, #0
	add r1, r0, #0
	bl sub_020398D4
	bl ov74_02236128
	ldr r1, _0222E7CC ; =0x00002BD0
	mov r3, #0xa
	add r0, r4, #0
	add r1, r4, r1
	mov r2, #0x12
	lsl r3, r3, #6
	bl ov74_0222D9E0
	ldr r0, _0222E7DC ; =0x00003D50
	ldr r0, [r4, r0]
	bl sub_0200F450
	add r0, r4, #0
	add r1, r5, #0
	mov r2, #0x1a
	bl ov74_0222EB28
	b _0222E79C
_0222E734:
	ldr r0, _0222E7E0 ; =gSystem
	ldr r1, [r0, #0x48]
	mov r0, #3
	tst r0, r1
	beq _0222E79C
	mov r0, #0x14
	str r0, [r5]
	b _0222E79C
_0222E744:
	ldr r0, _0222E7E0 ; =gSystem
	ldr r1, [r0, #0x48]
	mov r0, #3
	tst r0, r1
	beq _0222E79C
	ldr r0, _0222E7CC ; =0x00002BD0
	mov r1, #0
	add r0, r4, r0
	bl ov74_0222D104
	ldr r0, _0222E7CC ; =0x00002BD0
	add r0, r4, r0
	bl ClearWindowTilemapAndCopyToVram
	ldr r0, _0222E7CC ; =0x00002BD0
	add r0, r4, r0
	bl RemoveWindow
	mov r0, #3
	str r0, [r5]
	b _0222E79C
_0222E76E:
	mov r1, #0
	mov r2, #0x1d
	add r3, r5, #0
	bl ov74_0222D7F0
	b _0222E79C
_0222E77A:
	bl IsPaletteFadeFinished
	cmp r0, #0
	beq _0222E79C
	mov r0, #0xb1
	lsl r0, r0, #6
	ldr r0, [r4, r0]
	str r0, [r5]
	b _0222E79C
_0222E78C:
	bl ov74_0222D024
	add r0, r4, #0
	bl ov74_0222DEF0
	add sp, #8
	mov r0, #1
	pop {r4, r5, r6, pc}
_0222E79C:
	ldr r0, _0222E7E4 ; =0x00002C44
	ldr r0, [r4, r0]
	cmp r0, #0
	beq _0222E7A8
	bl SpriteList_RenderAndAnimateSprites
_0222E7A8:
	bl ov74_022358BC
	ldr r0, _0222E7E8 ; =0x00003D48
	ldr r1, [r4, r0]
	cmp r1, #0
	beq _0222E7B8
	add r0, r4, #0
	blx r1
_0222E7B8:
	mov r0, #0
	add sp, #8
	pop {r4, r5, r6, pc}
	nop
_0222E7C0: .word 0x00002B08
_0222E7C4: .word 0x00002C30
_0222E7C8: .word 0x00002AF8
_0222E7CC: .word 0x00002BD0
_0222E7D0: .word 0x00002BE0
_0222E7D4: .word 0x00002C34
_0222E7D8: .word 0x00002DD8
_0222E7DC: .word 0x00003D50
_0222E7E0: .word gSystem
_0222E7E4: .word 0x00002C44
_0222E7E8: .word 0x00003D48
	thumb_func_end ov74_0222E1F4


	thumb_func_start ov74_0222E7EC
ov74_0222E7EC: ; 0x0222E7EC
	push {r3, r4, r5, lr}
	add r5, r0, #0
	mov r0, #0xaf
	lsl r0, r0, #6
	ldr r1, [r5, r0]
	mov r2, #0xd6
	lsl r1, r1, #2
	add r1, r5, r1
	sub r0, #0xc
	ldr r4, [r1, r0]
	add r0, r5, #0
	add r0, #0xe0
	add r1, r4, #0
	lsl r2, r2, #2
	bl memcpy
	mov r1, #0x41
	add r0, r5, #0
	lsl r1, r1, #2
	add r0, #0x90
	add r1, r4, r1
	mov r2, #0x50
	bl memcpy
	ldr r0, _0222E854 ; =0x00000428
	mov r1, #0
	strb r1, [r5, r0]
	add r0, r5, #0
	add r0, #0xde
	ldrb r1, [r0]
	mov r0, #0x10
	mov r2, #0xf
	bic r1, r0
	add r0, r5, #0
	add r0, #0xde
	strb r1, [r0]
	add r0, r5, #0
	add r0, #0xde
	ldrb r1, [r0]
	mov r0, #0x20
	orr r1, r0
	add r0, r5, #0
	add r0, #0xde
	strb r1, [r0]
	ldr r1, _0222E858 ; =0x00002BA4
	add r0, r5, #4
	ldr r1, [r5, r1]
	bl ov74_02229CE0
	mov r0, #0
	pop {r3, r4, r5, pc}
	nop
_0222E854: .word 0x00000428
_0222E858: .word 0x00002BA4
	thumb_func_end ov74_0222E7EC
