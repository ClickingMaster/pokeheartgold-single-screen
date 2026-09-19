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



	thumb_func_start ov74_0222F6C4
ov74_0222F6C4: ; 0x0222F6C4
	push {r3, r4, r5, r6, r7, lr}
	ldr r1, _0222F7CC ; =0x000030E8
	mov r5, #0x15
	add r4, r0, r1
	mov r0, #0xc1
	lsl r0, r0, #4
	ldr r1, [r4, r0]
	lsl r5, r5, #0xe
	cmp r1, #0
	bne _0222F6DC
	mov r0, #1
	pop {r3, r4, r5, r6, r7, pc}
_0222F6DC:
	add r1, r0, #4
	ldr r1, [r4, r1]
	cmp r1, #1
	bne _0222F72A
	add r1, r0, #0
	sub r1, #8
	ldr r2, [r4, r1]
	sub r1, r0, #4
	ldr r1, [r4, r1]
	add r2, r2, r1
	add r1, r0, #0
	sub r1, #8
	str r2, [r4, r1]
	sub r1, r0, #4
	ldr r2, [r4, r1]
	add r1, r2, #0
	add r2, r2, r1
	sub r1, r0, #4
	str r2, [r4, r1]
	add r1, r0, #0
	sub r1, #8
	ldr r2, [r4, r1]
	asr r1, r2, #0xb
	lsr r1, r1, #0x14
	add r1, r2, r1
	asr r2, r1, #0xc
	mov r1, #0xfa
	lsl r1, r1, #2
	cmp r2, r1
	ble _0222F764
	mov r2, #0
	add r1, r0, #0
	str r2, [r4, r0]
	sub r1, #8
	str r2, [r4, r1]
	sub r0, r0, #4
	mov r5, #0xa8
	str r2, [r4, r0]
	b _0222F764
_0222F72A:
	add r1, r0, #0
	sub r1, #8
	ldr r2, [r4, r1]
	sub r1, r0, #4
	ldr r1, [r4, r1]
	sub r2, r2, r1
	add r1, r0, #0
	sub r1, #8
	str r2, [r4, r1]
	sub r1, r0, #4
	ldr r2, [r4, r1]
	lsr r1, r2, #0x1f
	add r1, r2, r1
	asr r2, r1, #1
	sub r1, r0, #4
	str r2, [r4, r1]
	mov r2, #1
	ldr r1, [r4, r1]
	lsl r2, r2, #0xa
	cmp r1, r2
	bge _0222F764
	mov r1, #0
	lsl r3, r2, #2
	add r2, r0, #0
	str r1, [r4, r0]
	sub r2, #8
	str r3, [r4, r2]
	sub r0, r0, #4
	str r1, [r4, r0]
_0222F764:
	mov r0, #3
	lsl r0, r0, #0xa
	ldr r0, [r4, r0]
	bl sub_02014A4C
	add r2, r0, #0
	mov r0, #0x54
	add r3, r0, #0
	add r3, #0xfc
_0222F776:
	asr r1, r5, #0xb
	lsr r1, r1, #0x14
	add r1, r5, r1
	asr r1, r1, #0xc
	bpl _0222F782
	mov r1, #0
_0222F782:
	cmp r1, #0xa8
	ble _0222F788
	mov r1, #0xa8
_0222F788:
	lsl r6, r3, #1
	add r7, r2, r6
	sub r6, r1, r0
	strh r6, [r7, #6]
	ldrh r6, [r7, #6]
	sub r1, r0, r1
	add r3, r3, #4
	strh r6, [r7, #2]
	mov r6, #0xa8
	sub r6, r6, r0
	lsl r6, r6, #3
	add r6, r2, r6
	strh r1, [r6, #6]
	ldrh r1, [r6, #6]
	add r0, r0, #1
	strh r1, [r6, #2]
	ldr r1, _0222F7D0 ; =0x00000C08
	ldr r1, [r4, r1]
	add r5, r5, r1
	cmp r0, #0xa8
	blt _0222F776
	mov r1, #6
	add r0, r2, #0
	lsl r1, r1, #8
	bl DC_FlushRange
	mov r0, #3
	lsl r0, r0, #0xa
	ldr r0, [r4, r0]
	bl sub_02014A8C
	mov r0, #0
	pop {r3, r4, r5, r6, r7, pc}
	nop
_0222F7CC: .word 0x000030E8
_0222F7D0: .word 0x00000C08
	thumb_func_end ov74_0222F6C4


	thumb_func_start ov74_0222F7D4
ov74_0222F7D4: ; 0x0222F7D4
	push {r4, r5, r6, lr}
	sub sp, #8
	add r5, r1, #0
	add r6, r0, #0
	bl OverlayManager_GetData
	ldr r1, [r5]
	add r4, r0, #0
	cmp r1, #0x1e
	bls _0222F7EA
	b _0222FB2C
_0222F7EA:
	add r1, r1, r1
	add r1, pc
	ldrh r1, [r1, #6]
	lsl r1, r1, #0x10
	asr r1, r1, #0x10
	add pc, r1
_0222F7F6: ; jump table
	.short _0222F834 - _0222F7F6 - 2 ; case 0
	.short _0222F874 - _0222F7F6 - 2 ; case 1
	.short _0222F886 - _0222F7F6 - 2 ; case 2
	.short _0222F8FC - _0222F7F6 - 2 ; case 3
	.short _0222F92E - _0222F7F6 - 2 ; case 4
	.short _0222F956 - _0222F7F6 - 2 ; case 5
	.short _0222F976 - _0222F7F6 - 2 ; case 6
	.short _0222F9AA - _0222F7F6 - 2 ; case 7
	.short _0222F9CE - _0222F7F6 - 2 ; case 8
	.short _0222F9F4 - _0222F7F6 - 2 ; case 9
	.short _0222FA20 - _0222F7F6 - 2 ; case 10
	.short _0222FA5A - _0222F7F6 - 2 ; case 11
	.short _0222FB2C - _0222F7F6 - 2 ; case 12
	.short _0222FAD6 - _0222F7F6 - 2 ; case 13
	.short _0222FB2C - _0222F7F6 - 2 ; case 14
	.short _0222FB2C - _0222F7F6 - 2 ; case 15
	.short _0222FB2C - _0222F7F6 - 2 ; case 16
	.short _0222FB2C - _0222F7F6 - 2 ; case 17
	.short _0222FB2C - _0222F7F6 - 2 ; case 18
	.short _0222FB2C - _0222F7F6 - 2 ; case 19
	.short _0222FA7E - _0222F7F6 - 2 ; case 20
	.short _0222FA8A - _0222F7F6 - 2 ; case 21
	.short _0222FB2C - _0222F7F6 - 2 ; case 22
	.short _0222FAC6 - _0222F7F6 - 2 ; case 23
	.short _0222FB2C - _0222F7F6 - 2 ; case 24
	.short _0222FB2C - _0222F7F6 - 2 ; case 25
	.short _0222FB2C - _0222F7F6 - 2 ; case 26
	.short _0222FB00 - _0222F7F6 - 2 ; case 27
	.short _0222FB0C - _0222F7F6 - 2 ; case 28
	.short _0222FB1C - _0222F7F6 - 2 ; case 29
	.short _0222FB2C - _0222F7F6 - 2 ; case 30
_0222F834:
	add r0, r6, #0
	bl OverlayManager_GetArgs
	ldr r1, [r0, #8]
	ldr r0, _0222FB50 ; =0x00002BA4
	str r1, [r4, r0]
	ldr r0, [r4, r0]
	bl Save_MysteryGift_Get
	ldr r1, _0222FB54 ; =0x00002BA0
	str r0, [r4, r1]
	add r0, r1, #4
	ldr r0, [r4, r0]
	bl Save_PlayerData_GetOptionsAddr
	ldr r1, _0222FB58 ; =0x00002BA8
	str r0, [r4, r1]
	ldr r0, [r4, r1]
	bl Options_GetFrame
	ldr r1, _0222FB5C ; =0x00002BAC
	str r0, [r4, r1]
	sub r1, #0xc
	ldr r0, [r4, r1]
	mov r1, #4
	bl SaveMysteryGift_CardGetByIdx
	ldr r1, _0222FB60 ; =0x00002BB4
	str r0, [r4, r1]
	mov r0, #1
	str r0, [r5]
	b _0222FB2C
_0222F874:
	bl ov74_0222FCA4
	ldr r0, _0222FB64 ; =0x000029FC
	ldr r0, [r4, r0]
	bl ov74_0222FCC4
	mov r0, #2
	str r0, [r5]
	b _0222FB2C
_0222F886:
	bl ResetAllTextPrinters
	add r0, r4, #0
	mov r1, #0
	bl ov74_0222F314
	mov r1, #0x1e
	mov r0, #0
	lsl r1, r1, #4
	mov r2, #0x55
	bl LoadFontPal0
	mov r1, #0
	str r1, [sp]
	mov r0, #0x55
	str r0, [sp, #4]
	ldr r0, _0222FB64 ; =0x000029FC
	mov r2, #1
	ldr r0, [r4, r0]
	mov r3, #0xd
	bl LoadUserFrameGfx1
	mov r0, #1
	str r0, [sp]
	mov r0, #0x55
	str r0, [sp, #4]
	ldr r0, _0222FB64 ; =0x000029FC
	mov r1, #0
	ldr r0, [r4, r0]
	mov r2, #0xa
	mov r3, #0xe
	bl LoadUserFrameGfx1
	ldr r0, _0222FB5C ; =0x00002BAC
	mov r1, #0
	ldr r0, [r4, r0]
	mov r2, #0x13
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	str r0, [sp]
	mov r0, #0x55
	str r0, [sp, #4]
	ldr r0, _0222FB64 ; =0x000029FC
	mov r3, #0xa
	ldr r0, [r4, r0]
	bl LoadUserFrameGfx2
	add r0, r4, #0
	mov r1, #1
	mov r2, #0
	bl ov74_0222F024
	add r0, r4, #0
	mov r1, #1
	mov r2, #3
	add r3, r5, #0
	bl ov74_0222EFF0
	b _0222FB2C
_0222F8FC:
	ldr r0, _0222FB68 ; =gSystem
	ldr r1, [r0, #0x48]
	mov r0, #2
	tst r0, r1
	beq _0222F91A
	ldr r0, _0222FB6C ; =SEQ_SE_DP_SELECT
	bl PlaySE
	add r0, r4, #0
	mov r1, #0
	mov r2, #0x1d
	add r3, r5, #0
	bl ov74_0222EFF0
	b _0222FB2C
_0222F91A:
	mov r0, #1
	tst r0, r1
	bne _0222F922
	b _0222FB2C
_0222F922:
	ldr r0, _0222FB6C ; =SEQ_SE_DP_SELECT
	bl PlaySE
	mov r0, #4
	str r0, [r5]
	b _0222FB2C
_0222F92E:
	ldr r1, _0222FB70 ; =0x00002BC4
	mov r3, #0xa
	add r1, r4, r1
	mov r2, #7
	lsl r3, r3, #6
	bl ov74_0222F1BC
	ldr r3, _0222FB74 ; =0x00002BE4
	mov r2, #8
	add r1, r3, #0
	str r0, [r4, r3]
	sub r1, #0x10
	ldr r3, [r4, r3]
	add r0, r4, #0
	add r1, r4, r1
	bl ov74_0222F1BC
	mov r0, #5
	str r0, [r5]
	b _0222FB2C
_0222F956:
	add r0, r6, #0
	add r1, r5, #0
	mov r2, #0
	bl ov74_0222F404
	ldr r0, _0222FB68 ; =gSystem
	ldr r1, [r0, #0x48]
	mov r0, #2
	tst r0, r1
	beq _0222FA28
	ldr r0, _0222FB6C ; =SEQ_SE_DP_SELECT
	bl PlaySE
	mov r0, #0xb
	str r0, [r5]
	b _0222FB2C
_0222F976:
	ldr r0, _0222FB78 ; =SEQ_SE_DP_CARD2
	bl PlaySE
	mov r0, #0x10
	mov r1, #0
	bl GfGfx_EngineATogglePlanes
	ldr r0, _0222FB70 ; =0x00002BC4
	mov r1, #0
	add r0, r4, r0
	bl ov74_0222ECEC
	ldr r0, _0222FB7C ; =0x00002BD4
	mov r1, #0
	add r0, r4, r0
	bl ov74_0222ECD4
	mov r1, #1
	add r0, r4, #0
	lsl r2, r1, #0xc
	mov r3, #0x66
	bl ov74_0222F624
	mov r0, #7
	str r0, [r5]
	b _0222FB2C
_0222F9AA:
	bl ov74_0222F6C4
	cmp r0, #0
	beq _0222FA28
	mov r1, #1
	add r0, r4, #0
	add r2, r1, #0
	bl ov74_0222F024
	ldr r2, _0222FB80 ; =0x00708000
	add r0, r4, #0
	mov r1, #0
	lsr r3, r2, #1
	bl ov74_0222F624
	mov r0, #8
	str r0, [r5]
	b _0222FB2C
_0222F9CE:
	bl ov74_0222F6C4
	ldr r0, _0222FB68 ; =gSystem
	ldr r1, [r0, #0x48]
	mov r0, #3
	tst r0, r1
	beq _0222FA28
	ldr r0, _0222FB78 ; =SEQ_SE_DP_CARD2
	bl PlaySE
	mov r1, #1
	add r0, r4, #0
	lsl r2, r1, #0xc
	mov r3, #0x66
	bl ov74_0222F624
	mov r0, #9
	str r0, [r5]
	b _0222FB2C
_0222F9F4:
	bl ov74_0222F6C4
	cmp r0, #0
	beq _0222FA28
	add r0, r4, #0
	mov r1, #1
	mov r2, #0
	bl ov74_0222F024
	ldr r2, _0222FB80 ; =0x00708000
	add r0, r4, #0
	mov r1, #0
	lsr r3, r2, #1
	bl ov74_0222F624
	mov r0, #0x10
	mov r1, #0
	bl GfGfx_EngineATogglePlanes
	mov r0, #0xa
	str r0, [r5]
	b _0222FB2C
_0222FA20:
	bl ov74_0222F6C4
	cmp r0, #0
	bne _0222FA2A
_0222FA28:
	b _0222FB2C
_0222FA2A:
	ldr r0, _0222FB70 ; =0x00002BC4
	mov r1, #0
	add r0, r4, r0
	mov r2, #0x13
	mov r3, #0xa
	bl DrawFrameAndWindow2
	ldr r0, _0222FB7C ; =0x00002BD4
	mov r1, #0
	add r0, r4, r0
	mov r2, #0xa
	mov r3, #0xe
	bl DrawFrameAndWindow1
	mov r0, #0x10
	mov r1, #1
	bl GfGfx_EngineATogglePlanes
	add r0, r4, #0
	bl ov74_0222F688
	mov r0, #5
	str r0, [r5]
	b _0222FB2C
_0222FA5A:
	bl ov74_0222EDC0
	ldr r0, _0222FB70 ; =0x00002BC4
	mov r1, #0
	add r0, r4, r0
	bl ov74_0222ECEC
	ldr r0, _0222FB70 ; =0x00002BC4
	add r0, r4, r0
	bl ClearWindowTilemapAndCopyToVram
	ldr r0, _0222FB70 ; =0x00002BC4
	add r0, r4, r0
	bl RemoveWindow
	mov r0, #3
	str r0, [r5]
	b _0222FB2C
_0222FA7E:
	mov r1, #0
	mov r2, #0x15
	add r3, r5, #0
	bl ov74_0222EFF0
	b _0222FB2C
_0222FA8A:
	bl ov74_0222EC60
	ldr r0, _0222FB70 ; =0x00002BC4
	mov r1, #0
	add r0, r4, r0
	bl ov74_0222ECEC
	ldr r0, _0222FB7C ; =0x00002BD4
	mov r1, #0
	add r0, r4, r0
	bl ov74_0222ECD4
	add r0, r4, #0
	mov r1, #1
	mov r2, #0
	bl ov74_0222F024
	mov r1, #0x1e
	mov r0, #0
	lsl r1, r1, #4
	mov r2, #0x55
	bl LoadFontPal0
	add r0, r4, #0
	mov r1, #1
	mov r2, #4
	add r3, r5, #0
	bl ov74_0222EFF0
	b _0222FB2C
_0222FAC6:
	bl sub_02037D78
	cmp r0, #0
	bne _0222FB2C
	ldr r0, _0222FB84 ; =0x00002BF8
	ldr r0, [r4, r0]
	str r0, [r5]
	b _0222FB2C
_0222FAD6:
	ldr r0, _0222FB68 ; =gSystem
	ldr r1, [r0, #0x48]
	mov r0, #3
	tst r0, r1
	beq _0222FB2C
	ldr r0, _0222FB70 ; =0x00002BC4
	mov r1, #0
	add r0, r4, r0
	bl ov74_0222ECEC
	ldr r0, _0222FB70 ; =0x00002BC4
	add r0, r4, r0
	bl ClearWindowTilemapAndCopyToVram
	ldr r0, _0222FB70 ; =0x00002BC4
	add r0, r4, r0
	bl RemoveWindow
	mov r0, #3
	str r0, [r5]
	b _0222FB2C
_0222FB00:
	mov r1, #0
	mov r2, #0x1d
	add r3, r5, #0
	bl ov74_0222EFF0
	b _0222FB2C
_0222FB0C:
	bl IsPaletteFadeFinished
	cmp r0, #0
	beq _0222FB2C
	ldr r0, _0222FB84 ; =0x00002BF8
	ldr r0, [r4, r0]
	str r0, [r5]
	b _0222FB2C
_0222FB1C:
	bl ov74_0222EC60
	add r0, r4, #0
	bl ov74_0222F688
	add sp, #8
	mov r0, #1
	pop {r4, r5, r6, pc}
_0222FB2C:
	ldr r0, _0222FB88 ; =0x00002BFC
	ldr r0, [r4, r0]
	cmp r0, #0
	beq _0222FB38
	bl SpriteList_RenderAndAnimateSprites
_0222FB38:
	bl ov74_022358BC
	mov r0, #0x3d
	lsl r0, r0, #8
	ldr r1, [r4, r0]
	cmp r1, #0
	beq _0222FB4A
	add r0, r4, #0
	blx r1
_0222FB4A:
	mov r0, #0
	add sp, #8
	pop {r4, r5, r6, pc}
	.balign 4, 0
_0222FB50: .word 0x00002BA4
_0222FB54: .word 0x00002BA0
_0222FB58: .word 0x00002BA8
_0222FB5C: .word 0x00002BAC
_0222FB60: .word 0x00002BB4
_0222FB64: .word 0x000029FC
_0222FB68: .word gSystem
_0222FB6C: .word SEQ_SE_DP_SELECT
_0222FB70: .word 0x00002BC4
_0222FB74: .word 0x00002BE4
_0222FB78: .word SEQ_SE_DP_CARD2
_0222FB7C: .word 0x00002BD4
_0222FB80: .word 0x00708000
_0222FB84: .word 0x00002BF8
_0222FB88: .word 0x00002BFC
	thumb_func_end ov74_0222F7D4


	thumb_func_start ov74_0222FB8C
ov74_0222FB8C: ; 0x0222FB8C
	push {r3, r4, r5, r6, r7, lr}
	str r0, [sp]
	bl OverlayManager_GetData
	add r7, r0, #0
	ldr r0, _0222FC38 ; =0x00002A08
	mov r6, #0
	add r4, r7, #0
	add r5, r7, r0
_0222FB9E:
	ldr r0, _0222FC38 ; =0x00002A08
	ldr r0, [r4, r0]
	cmp r0, #0
	beq _0222FBB2
	add r0, r5, #0
	bl ClearWindowTilemapAndCopyToVram
	add r0, r5, #0
	bl RemoveWindow
_0222FBB2:
	add r6, r6, #1
	add r4, #0x10
	add r5, #0x10
	cmp r6, #0xb
	blo _0222FB9E
	ldr r0, _0222FC3C ; =0x00002BC4
	ldr r1, [r7, r0]
	cmp r1, #0
	beq _0222FBD2
	add r0, r7, r0
	bl ClearWindowTilemapAndCopyToVram
	ldr r0, _0222FC3C ; =0x00002BC4
	add r0, r7, r0
	bl RemoveWindow
_0222FBD2:
	ldr r0, _0222FC40 ; =0x00002BD4
	ldr r1, [r7, r0]
	cmp r1, #0
	beq _0222FBE8
	add r0, r7, r0
	bl ClearWindowTilemapAndCopyToVram
	ldr r0, _0222FC40 ; =0x00002BD4
	add r0, r7, r0
	bl RemoveWindow
_0222FBE8:
	ldr r0, _0222FC44 ; =0x000029FC
	mov r1, #0
	ldr r0, [r7, r0]
	bl FreeBgTilemapBuffer
	ldr r0, _0222FC44 ; =0x000029FC
	mov r1, #1
	ldr r0, [r7, r0]
	bl FreeBgTilemapBuffer
	ldr r0, _0222FC44 ; =0x000029FC
	mov r1, #2
	ldr r0, [r7, r0]
	bl FreeBgTilemapBuffer
	ldr r0, _0222FC44 ; =0x000029FC
	mov r1, #3
	ldr r0, [r7, r0]
	bl FreeBgTilemapBuffer
	ldr r0, _0222FC44 ; =0x000029FC
	ldr r0, [r7, r0]
	bl Heap_Free
	ldr r0, _0222FC48 ; =FS_OVERLAY_ID(OVY_74)
	ldr r1, _0222FC4C ; =gApp_MainMenu_SelectOption_MysteryGift
	bl RegisterMainOverlay
	mov r0, #0x59
	bl Heap_Destroy
	ldr r0, [sp]
	bl OverlayManager_FreeData
	mov r0, #0x55
	bl Heap_Destroy
	mov r0, #1
	pop {r3, r4, r5, r6, r7, pc}
	nop
_0222FC38: .word 0x00002A08
_0222FC3C: .word 0x00002BC4
_0222FC40: .word 0x00002BD4
_0222FC44: .word 0x000029FC
_0222FC48: .word FS_OVERLAY_ID(OVY_74)
_0222FC4C: .word gApp_MainMenu_SelectOption_MysteryGift
	thumb_func_end ov74_0222FB8C


	thumb_func_start ov74_0222FC50
ov74_0222FC50: ; 0x0222FC50
	push {r3, r4, r5, r6, r7, lr}
	add r6, r0, #0
	add r7, r1, #0
	add r5, r2, #0
	bl ov74_0222FCC4
	ldr r1, _0222FC98 ; =0x00003D0C
	add r0, r5, #0
	bl Heap_AllocAtEnd
	ldr r2, _0222FC98 ; =0x00003D0C
	mov r1, #0
	add r4, r0, #0
	bl memset
	ldr r0, _0222FC9C ; =0x000029FC
	mov r1, #0x1e
	str r6, [r4, r0]
	mov r0, #0
	lsl r1, r1, #4
	add r2, r5, #0
	str r5, [r4]
	bl LoadFontPal0
	ldr r0, _0222FCA0 ; =0x00002BB4
	mov r1, #1
	str r7, [r4, r0]
	add r0, r4, #0
	mov r2, #0
	bl ov74_0222F024
	add r0, r4, #0
	bl Heap_Free
	pop {r3, r4, r5, r6, r7, pc}
	nop
_0222FC98: .word 0x00003D0C
_0222FC9C: .word 0x000029FC
_0222FCA0: .word 0x00002BB4
	thumb_func_end ov74_0222FC50


	thumb_func_start ov74_0222FCA4
ov74_0222FCA4: ; 0x0222FCA4
	push {r4, lr}
	sub sp, #0x28
	ldr r4, _0222FCC0 ; =_0223B4B0
	add r3, sp, #0
	mov r2, #5
_0222FCAE:
	ldmia r4!, {r0, r1}
	stmia r3!, {r0, r1}
	sub r2, r2, #1
	bne _0222FCAE
	add r0, sp, #0
	bl GfGfx_SetBanks
	add sp, #0x28
	pop {r4, pc}
	.balign 4, 0
_0222FCC0: .word _0223B4B0
	thumb_func_end ov74_0222FCA4


	thumb_func_start ov74_0222FCC4
ov74_0222FCC4: ; 0x0222FCC4
	push {r3, r4, r5, lr}
	sub sp, #0x80
	ldr r5, _0222FD84 ; =_0223B430
	add r3, sp, #0x70
	add r4, r0, #0
	add r2, r3, #0
	ldmia r5!, {r0, r1}
	stmia r3!, {r0, r1}
	ldmia r5!, {r0, r1}
	stmia r3!, {r0, r1}
	add r0, r2, #0
	bl SetBothScreensModesAndDisable
	ldr r5, _0222FD88 ; =_0223B45C
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
	ldr r5, _0222FD8C ; =_0223B494
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
	ldr r5, _0222FD90 ; =_0223B478
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
	ldr r5, _0222FD94 ; =_0223B440
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
_0222FD84: .word _0223B430
_0222FD88: .word _0223B45C
_0222FD8C: .word _0223B494
_0222FD90: .word _0223B478
_0222FD94: .word _0223B440
	thumb_func_end ov74_0222FCC4


	thumb_func_start ov74_0222FD98
ov74_0222FD98: ; 0x0222FD98
	push {r3, r4, r5, r6, r7, lr}
	add r5, r1, #0
	add r7, r0, #0
	bl Save_MysteryGift_Get
	mov r1, #0
	ldr r4, [r5, #0x48]
	mvn r1, r1
	add r6, r0, #0
	cmp r4, r1
	bne _0222FDCC
	add r2, r5, #0
	add r2, #0x4c
	ldrh r2, [r2]
	lsr r1, r1, #0x10
	cmp r2, r1
	bne _0222FDCC
	bl Save_MysteryGift_Init
	add r0, r7, #0
	mov r1, #0
	bl Save_NowWriteFile_AfterMGInit
	mov r0, #0
	bl OS_ResetSystem
_0222FDCC:
	add r0, r5, #0
	add r0, #0x4c
	ldrh r1, [r0]
	cmp r1, #0x64
	blo _0222FDE0
	cmp r1, #0x98
	bhi _0222FDE0
	mov r0, #0x46
	lsl r0, r0, #6
	orr r4, r0
_0222FDE0:
	cmp r4, #0
	bne _0222FDE8
	mov r4, #0
	mvn r4, r4
_0222FDE8:
#ifdef HEARTGOLD
	mov r0, #0x80
#else
	mov r0, #1
	lsl r0, r0, #8
#endif
	tst r0, r4
	bne _0222FDF2
	mov r0, #1
	pop {r3, r4, r5, r6, r7, pc}
_0222FDF2:
	add r0, r5, #0
	add r0, #0x4e
	ldrb r0, [r0]
	lsl r0, r0, #0x1f
	lsr r0, r0, #0x1f
	cmp r0, #1
	bne _0222FE0E
	add r0, r6, #0
	bl SaveMysteryGift_ReceivedFlagTest
	cmp r0, #1
	bne _0222FE0E
	mov r0, #2
	pop {r3, r4, r5, r6, r7, pc}
_0222FE0E:
	add r0, r5, #0
	add r0, #0x4e
	ldrb r0, [r0]
	lsl r0, r0, #0x1d
	lsr r0, r0, #0x1f
	cmp r0, #1
	bne _0222FE2A
	add r0, r6, #0
	bl SaveMysteryGift_CardFindAvailable
	cmp r0, #0
	bne _0222FE2A
	mov r0, #4
	pop {r3, r4, r5, r6, r7, pc}
_0222FE2A:
	add r0, r6, #0
	bl SaveMysteryGift_FindAvailable
	cmp r0, #0
	bne _0222FE38
	mov r0, #3
	pop {r3, r4, r5, r6, r7, pc}
_0222FE38:
	add r5, #0x4e
	ldrb r0, [r5]
	lsl r0, r0, #0x1a
	lsr r0, r0, #0x1f
	cmp r0, #1
	bne _0222FE48
	mov r0, #5
	pop {r3, r4, r5, r6, r7, pc}
_0222FE48:
	mov r0, #0
	pop {r3, r4, r5, r6, r7, pc}
	thumb_func_end ov74_0222FD98
