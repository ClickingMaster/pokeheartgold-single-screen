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



	thumb_func_start ov74_0222962C
ov74_0222962C: ; 0x0222962C
	push {r3, r4, r5, lr}
	mov r5, #0x57
	lsl r5, r5, #2
	add r4, r0, #0
	sub r0, r5, #4
	ldr r0, [r4, r0]
	cmp r0, #1
	bne _02229652
	bl ov74_02230E7C
	mov r1, #3
	sub r0, r1, r0
	bl sub_0203A930
	bl ov74_02230D28
	mov r0, #1
	bl ov74_022310B8
_02229652:
	ldr r0, [r4, r5]
	cmp r0, #0x1e
	bls _0222965A
	b _02229B52
_0222965A:
	add r0, r0, r0
	add r0, pc
	ldrh r0, [r0, #6]
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	add pc, r0
_02229666: ; jump table
	.short _022296A4 - _02229666 - 2 ; case 0
	.short _022296BA - _02229666 - 2 ; case 1
	.short _022296E0 - _02229666 - 2 ; case 2
	.short _02229702 - _02229666 - 2 ; case 3
	.short _0222972C - _02229666 - 2 ; case 4
	.short _02229B52 - _02229666 - 2 ; case 5
	.short _02229B52 - _02229666 - 2 ; case 6
	.short _0222975E - _02229666 - 2 ; case 7
	.short _022297D4 - _02229666 - 2 ; case 8
	.short _022297F2 - _02229666 - 2 ; case 9
	.short _0222983C - _02229666 - 2 ; case 10
	.short _02229878 - _02229666 - 2 ; case 11
	.short _02229892 - _02229666 - 2 ; case 12
	.short _022298C6 - _02229666 - 2 ; case 13
	.short _02229908 - _02229666 - 2 ; case 14
	.short _02229928 - _02229666 - 2 ; case 15
	.short _02229940 - _02229666 - 2 ; case 16
	.short _0222995A - _02229666 - 2 ; case 17
	.short _022299B0 - _02229666 - 2 ; case 18
	.short _02229A38 - _02229666 - 2 ; case 19
	.short _02229A80 - _02229666 - 2 ; case 20
	.short _02229B52 - _02229666 - 2 ; case 21
	.short _02229AB4 - _02229666 - 2 ; case 22
	.short _02229AB8 - _02229666 - 2 ; case 23
	.short _02229B52 - _02229666 - 2 ; case 24
	.short _02229B52 - _02229666 - 2 ; case 25
	.short _02229B20 - _02229666 - 2 ; case 26
	.short _02229B52 - _02229666 - 2 ; case 27
	.short _02229AEC - _02229666 - 2 ; case 28
	.short _0222977A - _02229666 - 2 ; case 29
	.short _022297B8 - _02229666 - 2 ; case 30
_022296A4:
	add r0, r4, #0
	bl ov74_02228D20
	bl sub_02034D8C
	mov r0, #0x52
	mov r1, #1
	lsl r0, r0, #2
	str r1, [r4, r0]
	str r1, [r4, r5]
	b _02229B52
_022296BA:
	bl sub_02034DB8
	cmp r0, #0
	beq _02229772
	mov r1, #0x5b
	lsl r1, r1, #2
	ldr r0, _022299FC ; =ov74_0223BD5C
	add r1, r4, r1
	bl ov74_02230A84
	mov r0, #0x56
	mov r1, #1
	lsl r0, r0, #2
	str r1, [r4, r0]
	bl sub_0203A880
	mov r0, #2
	str r0, [r4, r5]
	b _02229B52
_022296E0:
	bl ov74_02230D6C
	mov r1, #0x19
	add r0, r4, #0
	lsl r1, r1, #6
	bl ov74_022295C8
	add r0, r4, #0
	add r0, #0x28
	mov r1, #0xa
	bl WaitingIcon_New
	ldr r1, _02229A00 ; =0x000034D8
	str r0, [r4, r1]
	mov r0, #3
	str r0, [r4, r5]
	b _02229B52
_02229702:
	bl ov74_02230E94
	cmp r0, #0
	beq _0222971E
	sub r1, r0, #1
	mov r0, #0x59
	lsl r0, r0, #2
	str r1, [r4, r0]
	ldr r1, _02229A04 ; =0x00000708
	add r0, r4, #0
	bl ov74_022295C8
	mov r0, #4
	str r0, [r4, r5]
_0222971E:
	add r0, r4, #0
	add r1, r4, r5
	mov r2, #0
	mov r3, #2
	bl ov74_022295D0
	b _02229B52
_0222972C:
	mov r0, #0x59
	lsl r0, r0, #2
	ldr r0, [r4, r0]
	bl ov74_02230DB8
	cmp r0, #0
	beq _02229750
	mov r0, #0x59
	lsl r0, r0, #2
	ldr r0, [r4, r0]
	bl ov74_02230DF4
	mov r0, #7
	str r0, [r4, r5]
	mov r0, #0x16
	ldr r1, _02229A04 ; =0x00000708
	lsl r0, r0, #4
	str r1, [r4, r0]
_02229750:
	add r0, r4, #0
	add r1, r4, r5
	mov r2, #0
	mov r3, #2
	bl ov74_022295D0
	b _02229B52
_0222975E:
	add r0, r4, #0
	add r1, r4, r5
	mov r2, #0
	mov r3, #2
	bl ov74_022295D0
	bl ov74_02230FD4
	cmp r0, #0
	bne _02229774
_02229772:
	b _02229B52
_02229774:
	mov r0, #0x1d
	str r0, [r4, r5]
	b _02229B52
_0222977A:
	mov r0, #0x1f
	str r0, [sp]
	add r1, r4, #0
	ldr r0, [r4, #4]
	add r1, #0x48
	mov r2, #2
	mov r3, #0x13
	bl ov74_02235568
	ldr r0, _02229A08 ; =0x00003170
	mov r1, #0
	ldr r0, [r4, r0]
	bl Sprite_SetDrawFlag
	ldr r0, _02229A00 ; =0x000034D8
	ldr r0, [r4, r0]
	bl sub_0200F450
	ldr r0, _02229A00 ; =0x000034D8
	mov r1, #0
	str r1, [r4, r0]
	ldr r0, _02229A0C ; =SEQ_SE_DP_SELECT
	bl PlaySE
	mov r0, #0x16
	ldr r1, _02229A04 ; =0x00000708
	lsl r0, r0, #4
	str r1, [r4, r0]
	mov r0, #0x1e
	str r0, [r4, r5]
	b _02229B52
_022297B8:
	ldr r0, _02229A10 ; =gSystem
	ldr r1, [r0, #0x48]
	mov r0, #1
	tst r0, r1
	beq _022297C6
	mov r0, #8
	str r0, [r4, r5]
_022297C6:
	add r0, r4, #0
	add r1, r4, r5
	mov r2, #1
	mov r3, #2
	bl ov74_022295D0
	b _02229B52
_022297D4:
	bl ov74_02230EB4
	mov r0, #9
	str r0, [r4, r5]
	mov r0, #0x16
	ldr r1, _02229A04 ; =0x00000708
	lsl r0, r0, #4
	str r1, [r4, r0]
	add r0, r4, #0
	add r1, r4, r5
	mov r2, #1
	mov r3, #2
	bl ov74_022295D0
	b _02229B52
_022297F2:
	bl ov74_02230F40
	cmp r0, #0
	beq _0222982E
	bl ov74_02230EE8
	mov r0, #0xa
	str r0, [r4, r5]
	mov r1, #0x32
	mov r0, #0x16
	lsl r1, r1, #6
	lsl r0, r0, #4
	str r1, [r4, r0]
	mov r0, #0x23
	str r0, [sp]
	add r1, r4, #0
	ldr r0, [r4, #4]
	add r1, #0x48
	mov r2, #2
	mov r3, #0x13
	bl ov74_02235568
	ldr r0, _02229A0C ; =SEQ_SE_DP_SELECT
	bl PlaySE
	ldr r0, _02229A00 ; =0x000034D8
	mov r1, #0
	str r1, [r4, r0]
	mov r0, #1
	pop {r3, r4, r5, pc}
_0222982E:
	add r0, r4, #0
	add r1, r4, r5
	mov r2, #1
	mov r3, #2
	bl ov74_022295D0
	b _02229B52
_0222983C:
	bl ov74_02230F40
	cmp r0, #0
	beq _0222986A
	mov r0, #0x49
	str r0, [sp]
	add r1, r4, #0
	ldr r0, [r4, #4]
	add r1, #0x48
	mov r2, #2
	mov r3, #0x13
	bl ov74_02235568
	ldr r0, _02229A08 ; =0x00003170
	mov r1, #1
	ldr r0, [r4, r0]
	bl Sprite_SetDrawFlag
	add r0, r4, #0
	bl ov74_02229084
	mov r0, #0xb
	str r0, [r4, r5]
_0222986A:
	add r0, r4, #0
	add r1, r4, r5
	mov r2, #1
	mov r3, #2
	bl ov74_022295D0
	b _02229B52
_02229878:
	ldr r0, _02229A14 ; =ov74_0223C1F4
	mov r1, #0x2a
	mov r2, #1
	bl ov74_02230F14
	mov r0, #0xc
	str r0, [r4, r5]
	mov r1, #0x32
	mov r0, #0x16
	lsl r1, r1, #6
	lsl r0, r0, #4
	str r1, [r4, r0]
	b _02229B52
_02229892:
	bl ov74_02230F6C
	cmp r0, #0
	beq _022298B8
	mov r0, #0xd
	str r0, [r4, r5]
	mov r0, #0x16
	mov r1, #0x3c
	lsl r0, r0, #4
	str r1, [r4, r0]
	mov r0, #0x49
	str r0, [sp]
	add r1, r4, #0
	ldr r0, [r4, #4]
	add r1, #0x48
	mov r2, #2
	mov r3, #0x13
	bl ov74_02235568
_022298B8:
	add r0, r4, #0
	add r1, r4, r5
	mov r2, #1
	mov r3, #2
	bl ov74_022295D0
	b _02229B52
_022298C6:
	mov r0, #0x16
	lsl r0, r0, #4
	ldr r1, [r4, r0]
	sub r1, r1, #1
	str r1, [r4, r0]
	ldr r0, [r4, r0]
	cmp r0, #0
	bne _02229938
	mov r0, #0x4a
	str r0, [sp]
	add r1, r4, #0
	ldr r0, [r4, #4]
	add r1, #0x48
	mov r2, #2
	mov r3, #0x13
	bl ov74_02235568
	add r0, r4, #0
	add r0, #0x28
	mov r1, #0xa
	bl WaitingIcon_New
	ldr r1, _02229A00 ; =0x000034D8
	str r0, [r4, r1]
	add r0, r4, #0
	bl ov74_0222947C
	ldr r0, [r4, #8]
	bl ov74_022360A0
	mov r0, #0xe
	str r0, [r4, r5]
	b _02229B52
_02229908:
	bl ov74_022360B0
	cmp r0, #1
	bne _02229938
	mov r0, #0xf
	str r0, [r4, r5]
	bl OS_GetTickLo
	mov r1, #0x36
	bl _s32_div_f
	mov r0, #0x16
	add r1, r1, #6
	lsl r0, r0, #4
	str r1, [r4, r0]
	b _02229B52
_02229928:
	mov r0, #0x16
	lsl r0, r0, #4
	ldr r1, [r4, r0]
	sub r1, r1, #1
	str r1, [r4, r0]
	ldr r0, [r4, r0]
	cmp r0, #0
	beq _0222993A
_02229938:
	b _02229B52
_0222993A:
	mov r0, #0x10
	str r0, [r4, r5]
	b _02229B52
_02229940:
	ldr r0, _02229A14 ; =ov74_0223C1F4
	mov r1, #0x2a
	mov r2, #2
	bl ov74_02230F14
	mov r0, #0x11
	str r0, [r4, r5]
	mov r1, #0x4b
	mov r0, #0x16
	lsl r1, r1, #4
	lsl r0, r0, #4
	str r1, [r4, r0]
	b _02229B52
_0222995A:
	bl ov74_02230F98
	cmp r0, #0
	beq _02229976
	bl ov74_02236128
	mov r0, #0x12
	str r0, [r4, r5]
	mov r1, #0x32
	mov r0, #0x16
	lsl r1, r1, #4
	lsl r0, r0, #4
	str r1, [r4, r0]
	b _02229B52
_02229976:
	mov r0, #0x16
	lsl r0, r0, #4
	ldr r1, [r4, r0]
	sub r1, r1, #1
	str r1, [r4, r0]
	ldr r0, [r4, r0]
	cmp r0, #0
	bne _02229996
	bl ov74_02231008
	mov r0, #0x17
	mov r1, #0x78
	str r0, [r4, r5]
	add r0, r1, #0
	add r0, #0xe8
	str r1, [r4, r0]
_02229996:
	bl ov74_02230FD4
	cmp r0, #0
	bne _02229A48
	bl ov74_02231008
	mov r0, #0x16
	mov r1, #0x78
	str r0, [r4, r5]
	add r0, r1, #0
	add r0, #0xe8
	str r1, [r4, r0]
	b _02229B52
_022299B0:
	bl ov74_0223615C
	cmp r0, #3
	bne _022299CA
	bl ov74_02231008
	mov r0, #0x16
	mov r1, #0x78
	str r0, [r4, r5]
	add r0, r1, #0
	add r0, #0xe8
	str r1, [r4, r0]
	b _02229A18
_022299CA:
	bl ov74_0223615C
	cmp r0, #2
	beq _022299D8
	bl ov74_022360B0
	b _02229A18
_022299D8:
	bl ov74_02230F6C
	cmp r0, #0
	bne _022299E8
	bl ov74_02230FD4
	cmp r0, #0
	bne _02229A18
_022299E8:
	mov r0, #0x13
	str r0, [r4, r5]
	mov r0, #0x16
	mov r1, #1
	lsl r0, r0, #4
	str r1, [r4, r0]
	bl ov74_02231008
	b _02229B52
	nop
_022299FC: .word ov74_0223BD5C
_02229A00: .word 0x000034D8
_02229A04: .word 0x00000708
_02229A08: .word 0x00003170
_02229A0C: .word SEQ_SE_DP_SELECT
_02229A10: .word gSystem
_02229A14: .word ov74_0223C1F4
_02229A18:
	mov r0, #0x16
	lsl r0, r0, #4
	ldr r1, [r4, r0]
	sub r1, r1, #1
	str r1, [r4, r0]
	ldr r0, [r4, r0]
	cmp r0, #0
	bne _02229A38
	bl ov74_02231008
	mov r0, #0x17
	str r0, [r4, r5]
	mov r0, #0x16
	mov r1, #0xa
	lsl r0, r0, #4
	str r1, [r4, r0]
_02229A38:
	mov r0, #0x16
	lsl r0, r0, #4
	ldr r1, [r4, r0]
	sub r1, r1, #1
	str r1, [r4, r0]
	ldr r0, [r4, r0]
	cmp r0, #0
	beq _02229A4A
_02229A48:
	b _02229B52
_02229A4A:
	mov r0, #0x14
	str r0, [r4, r5]
	mov r0, #0x4b
	str r0, [sp]
	add r1, r4, #0
	ldr r0, [r4, #4]
	add r1, #0x48
	mov r2, #2
	mov r3, #0x13
	bl ov74_02235568
	ldr r0, _02229B58 ; =0x00003170
	mov r1, #0
	ldr r0, [r4, r0]
	bl Sprite_SetDrawFlag
	ldr r0, _02229B5C ; =0x000034D8
	ldr r0, [r4, r0]
	bl sub_0200F450
	ldr r0, _02229B60 ; =SEQ_SE_DP_SELECT
	bl PlaySE
	ldr r0, _02229B5C ; =0x000034D8
	mov r1, #0
	str r1, [r4, r0]
	b _02229B52
_02229A80:
	bl ov74_02231048
	cmp r0, #0
	beq _02229B52
	mov r0, #0x52
	lsl r0, r0, #2
	ldr r0, [r4, r0]
	cmp r0, #1
	bne _02229AA2
	bl sub_02034DE0
	bl sub_0203A914
	mov r0, #0x52
	mov r1, #0
	lsl r0, r0, #2
	str r1, [r4, r0]
_02229AA2:
	ldr r0, _02229B64 ; =gSystem
	ldr r1, [r0, #0x48]
	mov r0, #1
	tst r0, r1
	beq _02229B52
	mov r0, #0x1b
	str r0, [r4, r5]
	mov r0, #4
	pop {r3, r4, r5, pc}
_02229AB4:
	mov r0, #0x17
	str r0, [r4, r5]
_02229AB8:
	mov r0, #0x4c
	str r0, [sp]
	add r1, r4, #0
	ldr r0, [r4, #4]
	add r1, #0x48
	mov r2, #2
	mov r3, #0x13
	bl ov74_02235568
	ldr r0, _02229B58 ; =0x00003170
	mov r1, #0
	ldr r0, [r4, r0]
	bl Sprite_SetDrawFlag
	ldr r0, _02229B5C ; =0x000034D8
	ldr r0, [r4, r0]
	cmp r0, #0
	beq _02229AE0
	bl sub_0200F450
_02229AE0:
	ldr r0, _02229B5C ; =0x000034D8
	mov r1, #0
	str r1, [r4, r0]
	mov r0, #0x1c
	str r0, [r4, r5]
	b _02229B52
_02229AEC:
	bl ov74_02231048
	cmp r0, #0
	beq _02229B52
	mov r0, #0x52
	lsl r0, r0, #2
	ldr r0, [r4, r0]
	cmp r0, #1
	bne _02229B0E
	bl sub_02034DE0
	bl sub_0203A914
	mov r0, #0x52
	mov r1, #0
	lsl r0, r0, #2
	str r1, [r4, r0]
_02229B0E:
	ldr r0, _02229B64 ; =gSystem
	ldr r1, [r0, #0x48]
	mov r0, #1
	tst r0, r1
	beq _02229B52
	mov r0, #0
	bl OS_ResetSystem
	b _02229B52
_02229B20:
	bl ov74_02231048
	cmp r0, #0
	beq _02229B52
	mov r0, #0x52
	lsl r0, r0, #2
	ldr r0, [r4, r0]
	cmp r0, #1
	bne _02229B52
	bl sub_02034DE0
	bl sub_0203A914
	mov r0, #0x52
	mov r1, #0
	lsl r0, r0, #2
	str r1, [r4, r0]
	ldr r0, _02229B5C ; =0x000034D8
	ldr r0, [r4, r0]
	cmp r0, #0
	beq _02229B4E
	bl sub_0200F450
_02229B4E:
	mov r0, #5
	pop {r3, r4, r5, pc}
_02229B52:
	mov r0, #0
	pop {r3, r4, r5, pc}
	nop
_02229B58: .word 0x00003170
_02229B5C: .word 0x000034D8
_02229B60: .word SEQ_SE_DP_SELECT
_02229B64: .word gSystem
	thumb_func_end ov74_0222962C
