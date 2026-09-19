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



	thumb_func_start ov74_0222A6F8
ov74_0222A6F8: ; 0x0222A6F8
	push {r4, r5, r6, r7, lr}
	sub sp, #0x1c
	add r5, r3, #0
	ldr r3, _0222A740 ; =_0223B3A0
	add r7, r0, #0
	add r4, r1, #0
	add r6, r2, #0
	add r2, sp, #0
	ldmia r3!, {r0, r1}
	mov ip, r2
	stmia r2!, {r0, r1}
	ldmia r3!, {r0, r1}
	stmia r2!, {r0, r1}
	ldmia r3!, {r0, r1}
	stmia r2!, {r0, r1}
	ldr r0, [r3]
	lsr r1, r6, #0xb
	str r0, [r2]
	add r0, sp, #0
	strb r1, [r0, #0x12]
	lsr r1, r5, #0xe
	strb r1, [r0, #0x13]
	lsl r1, r4, #0x18
	add r0, r7, #0
	lsr r1, r1, #0x18
	mov r2, ip
	mov r3, #0
	bl InitBgFromTemplate
	lsl r1, r4, #0x18
	add r0, r7, #0
	lsr r1, r1, #0x18
	bl BgClearTilemapBufferAndCommit
	add sp, #0x1c
	pop {r4, r5, r6, r7, pc}
	.balign 4, 0
_0222A740: .word _0223B3A0
	thumb_func_end ov74_0222A6F8


	thumb_func_start ov74_0222A744
ov74_0222A744: ; 0x0222A744
	push {r3, r4, r5, lr}
	sub sp, #0x10
	ldr r5, _0222A79C ; =_0223B390
	add r3, sp, #0
	add r4, r0, #0
	add r2, r3, #0
	ldmia r5!, {r0, r1}
	stmia r3!, {r0, r1}
	ldmia r5!, {r0, r1}
	stmia r3!, {r0, r1}
	add r0, r2, #0
	bl SetBothScreensModesAndDisable
	mov r1, #0
	mov r2, #0xf
	add r0, r4, #0
	lsl r2, r2, #0xc
	add r3, r1, #0
	bl ov74_0222A6F8
	mov r1, #1
	mov r2, #0x3e
	add r0, r4, #0
	lsl r2, r2, #0xa
	lsl r3, r1, #0xf
	bl ov74_0222A6F8
	mov r2, #0xe
	add r0, r4, #0
	mov r1, #4
	lsl r2, r2, #0xa
	mov r3, #0
	bl ov74_0222A6F8
	mov r2, #0x1e
	mov r3, #1
	add r0, r4, #0
	mov r1, #5
	lsl r2, r2, #0xa
	lsl r3, r3, #0xe
	bl ov74_0222A6F8
	add sp, #0x10
	pop {r3, r4, r5, pc}
	.balign 4, 0
_0222A79C: .word _0223B390
	thumb_func_end ov74_0222A744


	thumb_func_start ov74_0222A7A0
ov74_0222A7A0: ; 0x0222A7A0
	push {r4, lr}
	sub sp, #0x10
	add r4, r0, #0
	mov r0, #0x20
	mov r2, #4
	str r0, [sp]
	mov r0, #0x54
	add r3, r2, #0
	str r0, [sp, #4]
	mov r0, #0x71
	mov r1, #0
	add r3, #0xfc
	bl GfGfxLoader_GXLoadPal
	mov r0, #0
	str r0, [sp]
	mov r0, #5
	lsl r0, r0, #0xa
	str r0, [sp, #4]
	mov r1, #1
	str r1, [sp, #8]
	mov r0, #0x54
	str r0, [sp, #0xc]
	mov r0, #0x71
	add r2, r4, #0
	mov r3, #5
	bl GfGfxLoader_LoadCharData
	mov r0, #0
	str r0, [sp]
	mov r0, #6
	lsl r0, r0, #8
	str r0, [sp, #4]
	mov r0, #1
	str r0, [sp, #8]
	mov r0, #0x54
	str r0, [sp, #0xc]
	mov r0, #0x71
	mov r1, #2
	add r2, r4, #0
	mov r3, #5
	bl GfGfxLoader_LoadScrnData
	mov r0, #0x20
	str r0, [sp]
	mov r0, #0x18
	str r0, [sp, #4]
	mov r0, #8
	mov r2, #0
	str r0, [sp, #8]
	add r0, r4, #0
	mov r1, #5
	add r3, r2, #0
	bl BgTilemapRectChangePalette
	add r0, r4, #0
	mov r1, #5
	bl BgCommitTilemapBufferToVram
	add sp, #0x10
	pop {r4, pc}
	.balign 4, 0
	thumb_func_end ov74_0222A7A0


	thumb_func_start ov74_0222A81C
ov74_0222A81C: ; 0x0222A81C
	push {r4, lr}
	sub sp, #0x10
	add r4, r0, #0
	mov r0, #0x20
	str r0, [sp]
	mov r0, #0x54
	str r0, [sp, #4]
	mov r0, #0x71
	mov r1, #0
	add r3, r0, #0
	add r2, r1, #0
	add r3, #0x8f
	bl GfGfxLoader_GXLoadPal
	mov r0, #0
	str r0, [sp]
	mov r0, #5
	lsl r0, r0, #0xa
	str r0, [sp, #4]
	mov r1, #1
	str r1, [sp, #8]
	mov r0, #0x54
	str r0, [sp, #0xc]
	mov r0, #0x71
	add r2, r4, #0
	add r3, r1, #0
	bl GfGfxLoader_LoadCharData
	mov r0, #0
	str r0, [sp]
	mov r0, #6
	lsl r0, r0, #8
	str r0, [sp, #4]
	mov r3, #1
	str r3, [sp, #8]
	mov r0, #0x54
	str r0, [sp, #0xc]
	mov r0, #0x71
	mov r1, #2
	add r2, r4, #0
	bl GfGfxLoader_LoadScrnData
	mov r0, #0x20
	str r0, [sp]
	mov r0, #0x18
	str r0, [sp, #4]
	mov r0, #8
	mov r2, #0
	str r0, [sp, #8]
	add r0, r4, #0
	mov r1, #1
	add r3, r2, #0
	bl BgTilemapRectChangePalette
	add r0, r4, #0
	mov r1, #1
	bl BgCommitTilemapBufferToVram
	add r0, r4, #0
	bl ov74_0222A7A0
	add sp, #0x10
	pop {r4, pc}
	.balign 4, 0
	thumb_func_end ov74_0222A81C


	thumb_func_start ov74_0222A89C
ov74_0222A89C: ; 0x0222A89C
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x28
	str r0, [sp]
	add r5, r1, #0
	add r7, r2, #0
	str r3, [sp, #4]
	bl OverlayManager_GetData
	add r4, r0, #0
	ldr r0, [r4, #0x7c]
	cmp r0, #0
	beq _0222A8B8
	bl ListMenuItems_Delete
_0222A8B8:
	add r0, r7, #0
	mov r1, #0x54
	bl ListMenuItems_New
	str r0, [r4, #0x7c]
	mov r0, #0
	mov r1, #0x1b
	mov r2, #0xf7
	mov r3, #0x54
	bl NewMsgDataFromNarc
	mov r6, #0
	str r0, [r4, #0x10]
	cmp r7, #0
	ble _0222A8EA
_0222A8D6:
	ldr r0, [r4, #0x7c]
	ldr r1, [r4, #0x10]
	ldr r2, [r5]
	ldr r3, [r5, #4]
	bl ListMenuItems_AppendFromMsgData
	add r6, r6, #1
	add r5, #8
	cmp r6, r7
	blt _0222A8D6
_0222A8EA:
	ldr r0, [r4, #0x10]
	bl DestroyMsgData
	ldr r3, _0222A948 ; =ov74_0223C270
	add r2, sp, #8
	ldmia r3!, {r0, r1}
	stmia r2!, {r0, r1}
	ldmia r3!, {r0, r1}
	stmia r2!, {r0, r1}
	ldmia r3!, {r0, r1}
	stmia r2!, {r0, r1}
	ldmia r3!, {r0, r1}
	stmia r2!, {r0, r1}
	ldr r0, [r4, #0x7c]
	str r0, [sp, #8]
	add r0, sp, #8
	strh r7, [r0, #0x12]
	ldrh r1, [r0, #0x12]
	strh r1, [r0, #0x10]
	ldr r0, [sp, #4]
	str r0, [sp, #0x14]
	ldr r0, [r4, #0x78]
	cmp r0, #0
	beq _0222A922
	mov r1, #0
	add r2, r1, #0
	bl DestroyListMenu
_0222A922:
	mov r1, #0
	add r0, sp, #8
	add r2, r1, #0
	mov r3, #0x54
	bl ListMenuInit
	str r0, [r4, #0x78]
	mov r0, #0
	ldr r2, [sp, #0x40]
	mvn r0, r0
	cmp r2, r0
	beq _0222A944
	add r4, #0x18
	ldr r0, [sp]
	add r1, r4, #0
	bl ov74_0222AA18
_0222A944:
	add sp, #0x28
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
_0222A948: .word ov74_0223C270
	thumb_func_end ov74_0222A89C


	thumb_func_start ov74_0222A94C
ov74_0222A94C: ; 0x0222A94C
	push {r4, r5, r6, r7, lr}
	sub sp, #0x44
	str r0, [sp, #0x14]
	add r7, r1, #0
	str r2, [sp, #0x20]
	bl OverlayManager_GetData
	str r0, [sp, #0x1c]
	ldr r0, [r0, #4]
	mov r5, #0
	bl Save_MysteryGift_Get
	ldr r1, _0222AA10 ; =ov74_0223C220
	ldr r4, [sp, #0x1c]
	ldr r2, [r1, #0x70]
	ldr r1, [r1, #0x74]
	str r0, [sp, #0x18]
	add r4, #0x28
	add r5, r5, #1
	str r2, [sp, #0x24]
	str r1, [sp, #0x28]
	bl SaveMysteryGift_HasAnyCard
	cmp r0, #0
	beq _0222A992
	lsl r6, r5, #3
	add r0, sp, #0x24
	add r1, r0, r6
	ldr r0, _0222AA10 ; =ov74_0223C220
	add r5, r5, #1
	ldr r3, [r0, #0x78]
	ldr r2, [r0, #0x7c]
	add r0, sp, #0x24
	str r3, [r0, r6]
	str r2, [r1, #4]
_0222A992:
	ldr r0, [sp, #0x18]
	bl SaveMysteryGift_SpecialCardTagIsValid
	cmp r0, #0
	beq _0222A9B0
	lsl r6, r5, #3
	add r0, sp, #0x24
	add r1, r0, r6
	ldr r0, _0222AA14 ; =ov74_0223C2A0
	add r5, r5, #1
	ldr r3, [r0]
	ldr r2, [r0, #4]
	add r0, sp, #0x24
	str r3, [r0, r6]
	str r2, [r1, #4]
_0222A9B0:
	lsl r6, r5, #3
	add r0, sp, #0x24
	add r1, r0, r6
	ldr r0, _0222AA14 ; =ov74_0223C2A0
	ldr r3, [r0, #8]
	ldr r2, [r0, #0xc]
	add r0, sp, #0x24
	str r3, [r0, r6]
	str r2, [r1, #4]
	ldr r0, [r4]
	cmp r0, #0
	bne _0222A9EE
	mov r0, #7
	str r0, [sp]
	mov r0, #0x14
	str r0, [sp, #4]
	add r0, r5, #1
	lsl r0, r0, #0x19
	lsr r0, r0, #0x18
	str r0, [sp, #8]
	mov r2, #0
	lsl r0, r7, #0x10
	str r2, [sp, #0xc]
	lsr r0, r0, #0x10
	str r0, [sp, #0x10]
	ldr r0, [sp, #0x1c]
	add r1, r4, #0
	ldr r0, [r0]
	mov r3, #6
	bl AddWindowParameterized
_0222A9EE:
	add r0, r4, #0
	mov r1, #1
	mov r2, #0x1f
	mov r3, #3
	bl DrawFrameAndWindow1
	ldr r0, [sp, #0x20]
	add r1, sp, #0x24
	str r0, [sp]
	ldr r0, [sp, #0x14]
	add r2, r5, #1
	add r3, r4, #0
	bl ov74_0222A89C
	add sp, #0x44
	pop {r4, r5, r6, r7, pc}
	nop
_0222AA10: .word ov74_0223C220
_0222AA14: .word ov74_0223C2A0
	thumb_func_end ov74_0222A94C


	thumb_func_start ov74_0222AA18
ov74_0222AA18: ; 0x0222AA18
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x10
	add r5, r1, #0
	add r7, r2, #0
	bl OverlayManager_GetData
	add r4, r0, #0
	mov r0, #1
	mov r1, #0x1b
	mov r2, #0xf7
	mov r3, #0x54
	bl NewMsgDataFromNarc
	str r0, [r4, #0x10]
	mov r0, #0x54
	bl MessageFormat_New
	str r0, [r4, #0xc]
	mov r0, #1
	mov r1, #6
	bl GetFontAttribute
	add r1, r0, #0
	add r0, r5, #0
	bl FillWindowPixelBuffer
	ldr r6, [r4, #0x14]
	cmp r6, #0
	bne _0222AA60
	ldr r0, [r4, #0xc]
	ldr r1, [r4, #0x10]
	add r2, r7, #0
	mov r3, #0x54
	bl ReadMsgData_ExpandPlaceholders
	add r6, r0, #0
_0222AA60:
	mov r3, #0
	str r3, [sp]
	ldr r0, [r4, #0x68]
	mov r1, #1
	str r0, [sp, #4]
	ldr r0, _0222AAA8 ; =0x0001020F
	add r2, r6, #0
	str r0, [sp, #8]
	str r3, [sp, #0xc]
	add r0, r5, #0
	bl AddTextPrinterParameterizedWithColor
	str r0, [r4, #0x6c]
	ldr r0, [r4, #0x14]
	cmp r0, #0
	bne _0222AA86
	add r0, r6, #0
	bl String_Delete
_0222AA86:
	add r0, r5, #0
	mov r1, #0
	mov r2, #1
	mov r3, #2
	bl DrawFrameAndWindow2
	ldr r0, [r4, #0x10]
	bl DestroyMsgData
	ldr r0, [r4, #0xc]
	bl MessageFormat_Delete
	mov r0, #0xff
	str r0, [r4, #0x68]
	add sp, #0x10
	pop {r3, r4, r5, r6, r7, pc}
	nop
_0222AAA8: .word 0x0001020F
	thumb_func_end ov74_0222AA18


	thumb_func_start ov74_0222AAAC
ov74_0222AAAC: ; 0x0222AAAC
	push {r4, r5, r6, lr}
	sub sp, #0x10
	add r5, r1, #0
	add r6, r2, #0
	bl OverlayManager_GetData
	mov r0, #0x25
	mov r1, #0x54
	bl String_New
	add r1, r6, #0
	mov r2, #0x24
	add r4, r0, #0
	bl CopyU16ArrayToStringN
	mov r0, #0
	mov r1, #6
	bl GetFontAttribute
	add r1, r0, #0
	add r0, r5, #0
	bl FillWindowPixelBuffer
	mov r1, #0
	str r1, [sp]
	mov r0, #0xff
	str r0, [sp, #4]
	ldr r0, _0222AB08 ; =0x0001020F
	add r2, r4, #0
	str r0, [sp, #8]
	add r0, r5, #0
	add r3, r1, #0
	str r1, [sp, #0xc]
	bl AddTextPrinterParameterizedWithColor
	add r0, r5, #0
	mov r1, #0
	mov r2, #0x1f
	mov r3, #3
	bl DrawFrameAndWindow1
	add r0, r4, #0
	bl String_Delete
	add sp, #0x10
	pop {r4, r5, r6, pc}
	.balign 4, 0
_0222AB08: .word 0x0001020F
	thumb_func_end ov74_0222AAAC


	thumb_func_start ov74_0222AB0C
ov74_0222AB0C: ; 0x0222AB0C
	push {r3, r4, r5, r6, r7, lr}
	add r7, r0, #0
	add r5, r1, #0
	add r6, r2, #0
	bl OverlayManager_GetData
	ldr r0, [r0, #0x78]
	bl ListMenu_ProcessInput
	add r4, r0, #0
	mov r0, #1
	mvn r0, r0
	cmp r4, r0
	beq _0222AB30
	add r0, r0, #1
	cmp r4, r0
	beq _0222AB66
	b _0222AB4A
_0222AB30:
	ldr r0, _0222AB68 ; =SEQ_SE_DP_SELECT
	bl PlaySE
	cmp r6, #0
	beq _0222AB66
	add r0, r7, #0
	blx r6
	mov r1, #0
	mvn r1, r1
	cmp r0, r1
	beq _0222AB66
	str r0, [r5]
	pop {r3, r4, r5, r6, r7, pc}
_0222AB4A:
	ldr r0, _0222AB68 ; =SEQ_SE_DP_SELECT
	bl PlaySE
	cmp r4, #0
	beq _0222AB66
	ldr r0, _0222AB6C ; =ov74_0223D0A8
	str r4, [r0, #8]
	add r0, r7, #0
	blx r4
	mov r1, #0
	mvn r1, r1
	cmp r0, r1
	beq _0222AB66
	str r0, [r5]
_0222AB66:
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
_0222AB68: .word SEQ_SE_DP_SELECT
_0222AB6C: .word ov74_0223D0A8
	thumb_func_end ov74_0222AB0C


	thumb_func_start ov74_0222AB70
ov74_0222AB70: ; 0x0222AB70
	push {r4, r5, lr}
	sub sp, #0x14
	add r5, r0, #0
	add r4, r1, #0
	bl ResetAllTextPrinters
	mov r0, #0
	add r1, r0, #0
	mov r2, #0x54
	bl LoadFontPal0
	mov r0, #0
	mov r1, #0x20
	mov r2, #0x54
	bl LoadFontPal0
	ldr r0, [r4, #8]
	bl Options_GetFrame
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	str r0, [sp]
	mov r0, #0x54
	str r0, [sp, #4]
	ldr r0, [r4]
	mov r1, #0
	mov r2, #1
	mov r3, #2
	bl LoadUserFrameGfx2
	mov r0, #1
	str r0, [sp]
	mov r0, #0x54
	str r0, [sp, #4]
	ldr r0, [r4]
	mov r1, #0
	mov r2, #0x1f
	mov r3, #3
	bl LoadUserFrameGfx1
	mov r0, #5
	ldr r1, _0222AC18 ; =0x00007D8C
	lsl r0, r0, #0x18
	strh r1, [r0]
	add r0, r4, #0
	add r0, #0x18
	bl WindowIsInUse
	cmp r0, #0
	bne _0222ABF4
	mov r0, #0x13
	str r0, [sp]
	mov r0, #0x1b
	str r0, [sp, #4]
	mov r0, #4
	str r0, [sp, #8]
	mov r2, #0
	add r1, r4, #0
	str r2, [sp, #0xc]
	mov r0, #0x28
	str r0, [sp, #0x10]
	ldr r0, [r4]
	add r1, #0x18
	mov r3, #2
	bl AddWindowParameterized
_0222ABF4:
	add r1, r4, #0
	add r0, r5, #0
	add r1, #0x18
	mov r2, #0
	bl ov74_0222AA18
	add r0, r5, #0
	mov r1, #0xc4
	mov r2, #0
	bl ov74_0222A94C
	ldr r0, [r4]
	bl ov74_0222A81C
	mov r0, #1
	add sp, #0x14
	pop {r4, r5, pc}
	nop
_0222AC18: .word 0x00007D8C
	thumb_func_end ov74_0222AB70


	thumb_func_start ov74_0222AC1C
ov74_0222AC1C: ; 0x0222AC1C
	push {r4, r5, r6, lr}
	add r6, r0, #0
	add r5, r1, #0
	bl OverlayManager_GetData
	add r4, r0, #0
	ldr r1, _0222ACC8 ; =0x000015E0
	add r0, r6, #0
	add r1, r4, r1
	bl ov74_02236AE0
	cmp r0, #4
	bhi _0222ACC4
	add r0, r0, r0
	add r0, pc
	ldrh r0, [r0, #6]
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	add pc, r0
_0222AC42: ; jump table
	.short _0222ACC4 - _0222AC42 - 2 ; case 0
	.short _0222AC4C - _0222AC42 - 2 ; case 1
	.short _0222AC78 - _0222AC42 - 2 ; case 2
	.short _0222AC78 - _0222AC42 - 2 ; case 3
	.short _0222AC8C - _0222AC42 - 2 ; case 4
_0222AC4C:
	ldr r1, _0222ACCC ; =0x00001658
	add r0, r4, #0
	mov r2, #0xea
	add r0, #0x8c
	add r1, r4, r1
	lsl r2, r2, #2
	bl memcpy
	add r0, r4, #0
	mov r1, #0
	bl ov74_02229E28
	add r0, r4, #0
	mov r1, #0x1b
	add r0, #0x84
	str r1, [r0]
	mov r0, #0x13
	str r0, [r5]
	ldr r0, _0222ACD0 ; =0x000015DC
	mov r1, #0
	str r1, [r4, r0]
	pop {r4, r5, r6, pc}
_0222AC78:
	add r0, r4, #0
	mov r1, #0
	bl ov74_02229E28
	ldr r0, _0222ACD0 ; =0x000015DC
	mov r1, #0
	str r1, [r4, r0]
	mov r0, #0x11
	str r0, [r5]
	pop {r4, r5, r6, pc}
_0222AC8C:
	mov r0, #0x9a
	lsl r0, r0, #6
	add r0, r4, r0
	bl ClearWindowTilemapAndCopyToVram
	mov r0, #0x9a
	lsl r0, r0, #6
	add r0, r4, r0
	bl RemoveWindow
	ldr r0, [r4]
	mov r1, #0
	bl BgClearTilemapBufferAndCommit
	ldr r0, _0222ACD4 ; =SEQ_SE_DP_SELECT
	bl PlaySE
	add r0, r6, #0
	mov r1, #0xc4
	mov r2, #0
	bl ov74_0222A94C
	mov r0, #0x10
	mov r1, #1
	bl GfGfx_EngineATogglePlanes
	mov r0, #3
	str r0, [r5]
_0222ACC4:
	pop {r4, r5, r6, pc}
	nop
_0222ACC8: .word 0x000015E0
_0222ACCC: .word 0x00001658
_0222ACD0: .word 0x000015DC
_0222ACD4: .word SEQ_SE_DP_SELECT
	thumb_func_end ov74_0222AC1C


	thumb_func_start ov74_0222ACD8
ov74_0222ACD8: ; 0x0222ACD8
	push {r3, r4, r5, r6, lr}
	sub sp, #0x5c
	add r5, r0, #0
	ldr r0, _0222AD64 ; =0x0000266C
	ldr r4, [r5, r0]
	mov r0, #0
	mvn r0, r0
	cmp r4, r0
	bne _0222ACEC
	mov r4, #0xb
_0222ACEC:
	mov r0, #0x10
	mov r1, #0
	bl GfGfx_EngineATogglePlanes
	ldr r0, [r5]
	mov r1, #0
	bl BgClearTilemapBufferAndCommit
	mov r0, #0x54
	bl MessageFormat_New
	mov r1, #2
	str r1, [sp]
	mov r1, #1
	str r1, [sp, #4]
	ldr r2, _0222AD68 ; =0x00002670
	mov r1, #0
	ldr r2, [r5, r2]
	mov r3, #5
	add r6, r0, #0
	bl BufferIntegerAsString
	mov r0, #0x1f
	mov r1, #0x9a
	str r0, [sp]
	mov r0, #3
	lsl r1, r1, #6
	mov r3, #0x32
	str r0, [sp, #4]
	add r0, sp, #8
	add r1, r5, r1
	mov r2, #1
	lsl r3, r3, #4
	bl ov74_02235414
	add r0, sp, #8
	mov r1, #0x18
	mov r2, #0x10
	mov r3, #0x64
	bl ov74_02235464
	add r0, sp, #8
	mov r1, #0
	mov r2, #1
	bl ov74_0223546C
	str r6, [sp, #0x1c]
	str r4, [sp]
	mov r2, #4
	ldr r0, [r5]
	add r1, sp, #8
	add r3, r2, #0
	bl ov74_02235568
	add r0, r6, #0
	bl MessageFormat_Delete
	add sp, #0x5c
	pop {r3, r4, r5, r6, pc}
	nop
_0222AD64: .word 0x0000266C
_0222AD68: .word 0x00002670
	thumb_func_end ov74_0222ACD8


	thumb_func_start ov74_0222AD6C
ov74_0222AD6C: ; 0x0222AD6C
	push {r3, r4, r5, lr}
	add r5, r0, #0
	bl OverlayManager_GetData
	add r1, r0, #0
	add r0, #0x80
	ldr r0, [r0]
	cmp r0, #3
	bne _0222AD82
	mov r4, #0x16
	b _0222ADAC
_0222AD82:
	cmp r0, #4
	bne _0222AD8A
	mov r4, #0x1a
	b _0222ADAC
_0222AD8A:
	cmp r0, #2
	bne _0222AD92
	mov r4, #0x17
	b _0222ADAC
_0222AD92:
	cmp r0, #1
	bne _0222AD9A
	mov r4, #0x1c
	b _0222ADAC
_0222AD9A:
	cmp r0, #5
	bne _0222ADAC
	add r0, r5, #0
	add r1, #0x18
	mov r2, #9
	mov r3, #0x34
	bl ov74_0222ADBC
	pop {r3, r4, r5, pc}
_0222ADAC:
	add r0, r5, #0
	add r1, #0x18
	add r2, r4, #0
	mov r3, #0x33
	bl ov74_0222ADBC
	pop {r3, r4, r5, pc}
	.balign 4, 0
	thumb_func_end ov74_0222AD6C


	thumb_func_start ov74_0222ADBC
ov74_0222ADBC: ; 0x0222ADBC
	push {r4, r5, r6, r7, lr}
	sub sp, #0xc
	str r0, [sp]
	add r6, r1, #0
	add r5, r2, #0
	add r7, r3, #0
	bl OverlayManager_GetData
	add r4, r0, #0
	cmp r6, #0
	beq _0222AE16
	cmp r5, #0
	beq _0222AE16
	mov r0, #1
	mov r1, #0x1b
	mov r2, #0xf7
	mov r3, #0x54
	bl NewMsgDataFromNarc
	str r0, [sp, #4]
	mov r0, #0x54
	bl MessageFormat_New
	ldr r1, [sp, #4]
	add r2, r5, #0
	mov r3, #0x54
	str r0, [sp, #8]
	bl ReadMsgData_ExpandPlaceholders
	str r0, [r4, #0x14]
	mov r0, #1
	str r0, [r4, #0x68]
	ldr r0, [sp]
	add r1, r6, #0
	add r2, r5, #0
	bl ov74_0222AA18
	ldr r0, [sp, #8]
	str r7, [r4, #0x74]
	bl MessageFormat_Delete
	ldr r0, [sp, #4]
	bl DestroyMsgData
	b _0222AE36
_0222AE16:
	ldr r0, [r4, #0x6c]
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	bl TextPrinterCheckActive
	cmp r0, #0
	bne _0222AE36
	ldr r0, [r4, #0x14]
	bl String_Delete
	mov r0, #0
	str r0, [r4, #0x14]
	str r0, [r4, #0x68]
	add sp, #0xc
	ldr r0, [r4, #0x74]
	pop {r4, r5, r6, r7, pc}
_0222AE36:
	mov r0, #0x32
	add sp, #0xc
	pop {r4, r5, r6, r7, pc}
	thumb_func_end ov74_0222ADBC


	thumb_func_start ov74_0222AE3C
ov74_0222AE3C: ; 0x0222AE3C
	push {r3, lr}
	ldr r1, _0222AE68 ; =0x000015D4
	ldr r0, [r0, r1]
	cmp r0, #4
	bhi _0222AE64
	add r0, r0, r0
	add r0, pc
	ldrh r0, [r0, #6]
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	add pc, r0
_0222AE52: ; jump table
	.short _0222AE64 - _0222AE52 - 2 ; case 0
	.short _0222AE5C - _0222AE52 - 2 ; case 1
	.short _0222AE64 - _0222AE52 - 2 ; case 2
	.short _0222AE64 - _0222AE52 - 2 ; case 3
	.short _0222AE64 - _0222AE52 - 2 ; case 4
_0222AE5C:
	bl ov74_02229DF8
	bl sub_0203A914
_0222AE64:
	pop {r3, pc}
	nop
_0222AE68: .word 0x000015D4
	thumb_func_end ov74_0222AE3C


	thumb_func_start ov74_0222AE6C
ov74_0222AE6C: ; 0x0222AE6C
	push {r3, r4, r5, lr}
	add r5, r0, #0
	bl ov74_0222CD88
	mov r0, #3
	mov r1, #0x54
	lsl r2, r0, #0x10
	bl Heap_Create
	ldr r1, _0222AF14 ; =0x000029F8
	add r0, r5, #0
	mov r2, #0x54
	bl OverlayManager_CreateAndGetData
	ldr r2, _0222AF14 ; =0x000029F8
	mov r1, #0
	add r4, r0, #0
	bl memset
	mov r0, #0x54
	bl BgConfig_Alloc
	str r0, [r4]
	bl GfGfx_DisableEngineAPlanes
	bl GfGfx_DisableEngineBPlanes
	bl ov74_0222FCA4
	ldr r0, [r4]
	bl ov74_0222A744
	mov r0, #0
	add r1, r0, #0
	bl sub_0200FBF4
	mov r0, #1
	mov r1, #0
	bl sub_0200FBF4
	ldr r1, _0222AF18 ; =0x0000047E
	mov r0, #0xa
	mov r2, #1
	bl Sound_SetSceneAndPlayBGM
	mov r0, #0x54
	bl ov74_0223512C
	bl ov74_02235230
	cmp r0, #0
	beq _0222AEE8
	ldr r0, _0222AF1C ; =0x000015D4
	mov r1, #2
	str r1, [r4, r0]
	ldr r0, _0222AF20 ; =gGameVersion
	ldrb r0, [r0]
	bl sub_0201A4B0
	mov r0, #1
	bl ov74_02236034
_0222AEE8:
	ldr r0, _0222AF24 ; =0x000005C8
	mov r1, #0x1d
	str r1, [r4, r0]
	add r0, r5, #0
	bl OverlayManager_GetArgs
	ldr r0, [r0, #8]
	str r0, [r4, #4]
	bl Save_PlayerData_GetOptionsAddr
	str r0, [r4, #8]
	mov r0, #0xff
	mov r2, #0x57
	str r0, [r4, #0x68]
	mov r0, #0
	mov r1, #0x59
	lsl r2, r2, #4
	bl Heap_Create
	mov r0, #1
	pop {r3, r4, r5, pc}
	nop
_0222AF14: .word 0x000029F8
_0222AF18: .word 0x0000047E
_0222AF1C: .word 0x000015D4
_0222AF20: .word gGameVersion
_0222AF24: .word 0x000005C8
	thumb_func_end ov74_0222AE6C


	thumb_func_start ov74_0222AF28
ov74_0222AF28: ; 0x0222AF28
	push {r4, r5, lr}
	sub sp, #0x2c
	add r4, r0, #0
	ldr r0, _0222B0C0 ; =0x00004E21
	mov r3, #1
	str r0, [sp]
	str r3, [sp, #4]
	mov r1, #0x54
	str r1, [sp, #8]
	ldr r0, [r4, #0xc]
	add r1, #0xd8
	ldr r0, [r0, r1]
	mov r1, #0x71
	mov r2, #0x2b
	bl AddCharResObjFromNarc
	str r0, [r4, #0x58]
	ldr r0, _0222B0C0 ; =0x00004E21
	mov r1, #0x54
	str r0, [sp]
	mov r0, #1
	str r0, [sp, #4]
	str r0, [sp, #8]
	str r1, [sp, #0xc]
	ldr r0, [r4, #0xc]
	add r1, #0xdc
	ldr r0, [r0, r1]
	mov r1, #0x71
	mov r2, #0x28
	mov r3, #0
	bl AddPlttResObjFromNarc
	str r0, [r4, #0x5c]
	ldr r0, _0222B0C0 ; =0x00004E21
	mov r1, #0x54
	str r0, [sp]
	mov r0, #2
	str r0, [sp, #4]
	str r1, [sp, #8]
	ldr r0, [r4, #0xc]
	add r1, #0xe0
	ldr r0, [r0, r1]
	mov r1, #0x71
	mov r2, #0x2a
	mov r3, #1
	bl AddCellOrAnimResObjFromNarc
	str r0, [r4, #0x60]
	ldr r0, _0222B0C0 ; =0x00004E21
	mov r1, #0x54
	str r0, [sp]
	mov r0, #3
	str r0, [sp, #4]
	str r1, [sp, #8]
	ldr r0, [r4, #0xc]
	add r1, #0xe4
	ldr r0, [r0, r1]
	mov r1, #0x71
	mov r2, #0x29
	mov r3, #1
	bl AddCellOrAnimResObjFromNarc
	str r0, [r4, #0x64]
	ldr r0, _0222B0C4 ; =0x00004E22
	mov r1, #0x54
	str r0, [sp]
	mov r0, #2
	str r0, [sp, #4]
	str r1, [sp, #8]
	ldr r0, [r4, #0xc]
	add r1, #0xd8
	ldr r0, [r0, r1]
	mov r1, #0x71
	mov r2, #0x2b
	mov r3, #1
	bl AddCharResObjFromNarc
	str r0, [r4, #0x70]
	ldr r0, _0222B0C4 ; =0x00004E22
	mov r1, #0x54
	str r0, [sp]
	mov r0, #2
	str r0, [sp, #4]
	mov r0, #1
	str r0, [sp, #8]
	str r1, [sp, #0xc]
	ldr r0, [r4, #0xc]
	add r1, #0xdc
	ldr r0, [r0, r1]
	mov r1, #0x71
	mov r2, #0x28
	mov r3, #0
	bl AddPlttResObjFromNarc
	str r0, [r4, #0x74]
	ldr r0, _0222B0C4 ; =0x00004E22
	mov r1, #0x54
	str r0, [sp]
	mov r0, #2
	str r0, [sp, #4]
	str r1, [sp, #8]
	ldr r0, [r4, #0xc]
	add r1, #0xe0
	ldr r0, [r0, r1]
	mov r1, #0x71
	mov r2, #0x2a
	mov r3, #1
	bl AddCellOrAnimResObjFromNarc
	str r0, [r4, #0x78]
	ldr r0, _0222B0C4 ; =0x00004E22
	mov r1, #0x54
	str r0, [sp]
	mov r0, #3
	str r0, [sp, #4]
	str r1, [sp, #8]
	ldr r0, [r4, #0xc]
	add r1, #0xe4
	ldr r0, [r0, r1]
	mov r1, #0x71
	mov r2, #0x29
	mov r3, #1
	bl AddCellOrAnimResObjFromNarc
	str r0, [r4, #0x7c]
	ldr r0, [r4, #0x58]
	bl SpriteTransfer_CreateCharTransferTask
	ldr r0, [r4, #0x70]
	bl SpriteTransfer_CreateCharTransferTask
	ldr r0, [r4, #0x5c]
	bl SpriteTransfer_CreatePlttTransferTask
	ldr r0, [r4, #0x74]
	bl SpriteTransfer_CreatePlttTransferTask
	mov r0, #0
	mov r5, #0x4b
	ldr r2, [r4, #0xc]
	ldr r1, _0222B0C0 ; =0x00004E21
	mvn r0, r0
	str r1, [sp]
	str r0, [sp, #4]
	str r0, [sp, #8]
	mov r0, #0
	str r0, [sp, #0xc]
	str r0, [sp, #0x10]
	lsl r5, r5, #2
	ldr r3, [r2, r5]
	str r3, [sp, #0x14]
	add r3, r5, #4
	ldr r3, [r2, r3]
	str r3, [sp, #0x18]
	add r3, r5, #0
	add r3, #8
	ldr r3, [r2, r3]
	add r5, #0xc
	str r3, [sp, #0x1c]
	ldr r2, [r2, r5]
	add r3, r1, #0
	str r2, [sp, #0x20]
	str r0, [sp, #0x24]
	str r0, [sp, #0x28]
	add r0, r4, #0
	add r0, #0x10
	add r2, r1, #0
	bl CreateSpriteResourcesHeader
	ldr r2, [r4, #0xc]
	ldr r1, _0222B0C4 ; =0x00004E22
	mov r0, #0
	mov r5, #0x4b
	add r4, #0x10
	str r1, [sp]
	mvn r0, r0
	str r0, [sp, #4]
	str r0, [sp, #8]
	mov r0, #0
	str r0, [sp, #0xc]
	str r0, [sp, #0x10]
	lsl r5, r5, #2
	ldr r3, [r2, r5]
	add r4, #0x24
	str r3, [sp, #0x14]
	add r3, r5, #4
	ldr r3, [r2, r3]
	str r3, [sp, #0x18]
	add r3, r5, #0
	add r3, #8
	ldr r3, [r2, r3]
	add r5, #0xc
	str r3, [sp, #0x1c]
	ldr r2, [r2, r5]
	add r3, r1, #0
	str r2, [sp, #0x20]
	str r0, [sp, #0x24]
	str r0, [sp, #0x28]
	add r0, r4, #0
	add r2, r1, #0
	bl CreateSpriteResourcesHeader
	add sp, #0x2c
	pop {r4, r5, pc}
	.balign 4, 0
_0222B0C0: .word 0x00004E21
_0222B0C4: .word 0x00004E22
	thumb_func_end ov74_0222AF28


	thumb_func_start ov74_0222B0C8
ov74_0222B0C8: ; 0x0222B0C8
	push {r4, lr}
	sub sp, #0x30
	cmp r1, #1
	beq _0222B0D4
	mov r3, #1
	b _0222B0D6
_0222B0D4:
	mov r3, #0
_0222B0D6:
	ldr r2, [r0, #0xc]
	add r0, #0x10
	ldr r2, [r2]
	str r2, [sp]
	mov r2, #0x24
	mul r2, r3
	add r0, r0, r2
	mov r3, #1
	lsl r3, r3, #0xc
	mov r2, #0
	str r0, [sp, #4]
	str r2, [sp, #0x10]
	str r3, [sp, #0x14]
	str r3, [sp, #0x18]
	str r3, [sp, #0x1c]
	add r0, sp, #0
	strh r2, [r0, #0x20]
	lsl r0, r3, #7
	str r0, [sp, #8]
	mov r0, #0xa
	lsl r2, r3, #5
	str r0, [sp, #0x24]
	mov r0, #0x54
	str r2, [sp, #0xc]
	str r1, [sp, #0x28]
	str r0, [sp, #0x2c]
	cmp r1, #2
	bne _0222B114
	lsl r0, r3, #8
	add r0, r2, r0
	str r0, [sp, #0xc]
_0222B114:
	add r0, sp, #0
	bl Sprite_CreateAffine
	add r4, r0, #0
	beq _0222B13C
	mov r1, #1
	bl Sprite_SetAnimActiveFlag
	add r0, r4, #0
	mov r1, #0
	bl Sprite_SetPriority
	add r0, r4, #0
	mov r1, #0
	bl Sprite_SetAnimCtrlSeq
	add r0, r4, #0
	mov r1, #1
	bl Sprite_SetDrawFlag
_0222B13C:
	add r0, r4, #0
	add sp, #0x30
	pop {r4, pc}
	.balign 4, 0
	thumb_func_end ov74_0222B0C8
