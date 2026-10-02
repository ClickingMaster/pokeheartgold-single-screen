	.extern AGB_GetBoxMonData
	.extern ConvertRSStringToDPStringInternational
	.extern PmAgbCartridge_GetLanguage
	.extern TranslateAgbSpecies
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
	.extern ov74_02231A1C
	.extern ov74_02234A0C
#include "config.h"
#include "msgdata/msg/msg_0247.h"
#include "msgdata/msg/msg_0442.h"
#include "constants/sndseq.h"
	.include "asm/macros.inc"
	.include "overlay_74_thumb.inc"
	.include "global.inc"

.text
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
