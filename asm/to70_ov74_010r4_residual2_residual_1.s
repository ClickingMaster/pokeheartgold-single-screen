	.extern PmAgbCartridgeGetOffsets
	.extern PmAgbCartridgeUnlinkSpec
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
	.extern ov74_02231790
	.extern ov74_022317D8
	.extern ov74_02231930
	.extern ov74_02231A1C
	.extern ov74_02231BC0
	.extern ov74_02231BF0
	.extern ov74_02231CFC
	.extern ov74_02231FF4
	.extern ov74_02232154
	.extern ov74_022322D8
	.extern ov74_022324A0
	.extern ov74_02232758
	.extern ov74_02232910
	.extern ov74_02232940
	.extern ov74_02232A48
	.extern ov74_02232AC8
	.extern ov74_02232BD4
	.extern ov74_02232DA4
	.extern ov74_02232DC4
	.extern ov74_02232E3C
	.extern ov74_02232F5C
	.extern ov74_02232F9C
	.extern ov74_02233024
	.extern ov74_02233060
	.extern ov74_022330D0
	.extern ov74_02233134
	.extern ov74_0223319C
	.extern ov74_02233CE4
	.extern ov74_02233F4C
	.extern ov74_02236074
	.extern ov74_0223C968
	.extern ov74_0223D338
#include "config.h"
#include "msgdata/msg/msg_0247.h"
#include "msgdata/msg/msg_0442.h"
#include "constants/sndseq.h"
	.include "asm/macros.inc"
	.include "overlay_74_thumb.inc"
	.include "global.inc"

.text
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
