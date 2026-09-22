	.include "asm/macros.inc"
	.include "overlay_13_thumb_2.inc"
	.include "global.inc"

	.text

	.extern ov13_022208E8
	.extern ov13_022208F8
	.extern ov13_02220908
	.extern ov13_022209F8
	.extern ov13_02221410
	.extern ov13_02221428
	.extern ov13_02221454
	.extern ov13_022214AC
	.extern ov13_022214B8
	.extern ov13_022214C4
	.extern ov13_022216BC
	.extern ov13_022216E0
	.extern ov13_0222175C
	.extern ov13_022217A0
	.extern ov13_022217D0
	.extern ov13_022217FC
	.extern ov13_0222189C
	.extern ov13_02221978
	.extern ov13_02221A50
	.extern ov13_02221AF4
	.extern ov13_02221BE8
	.extern ov13_02221C2C
	.extern ov13_02221C78
	.extern ov13_02221D34
	.extern ov13_02221D58
	.extern ov13_02221EAC
	.extern ov13_02221F64
	.extern ov13_02221F94
	.extern ov13_022220A4
	.extern ov13_022220B4
	.extern ov13_02222108
	.extern ov13_0222224C
	.extern ov13_02222328
	.extern ov13_02222394
	.extern ov13_02222420
	.extern ov13_02222474
	.extern ov13_022224CC
	.extern ov13_02222534
	.extern ov13_022225B0
	.extern ov13_02222638
	.extern ov13_0222269C
	.extern ov13_022226C8
	.extern ov13_02222708
	.extern ov13_02222730
	.extern ov13_02222768
	.extern ov13_022227A0
	.extern ov13_0222281C
	.extern ov13_02222850
	.extern ov13_02222874
	.extern ov13_022228A4
	.extern ov13_022228BC
	.extern ov13_022228CC
	.extern ov13_02222924
	.extern ov13_02222948
	.extern ov13_02222968
	.extern ov13_02222978
	.extern ov13_02222984
	.extern ov13_02222998
	.extern ov13_022229EC
	.extern ov13_022229FC
	.extern ov13_02222A00
	.extern ov13_02222A08
	.extern ov13_02222A14
	.extern ov13_02222A1C
	.extern ov13_02222A44
	.extern ov13_02222A5C
	.extern ov13_02222A84
	.extern ov13_02222A9C
	.extern ov13_02222AB0
	.extern ov13_02222B20
	.extern ov13_02222B2C
	.extern ov13_02222B34
	.extern ov13_02222BC0
	.extern ov13_02222BE4
	.extern ov13_02222BF4
	.extern ov13_02222C08
	.extern ov13_022230F8
	.extern ov13_02223100
	.extern ov13_02223114
	.extern ov13_022233CC
	.extern ov13_02223434
	.extern ov13_02223478
	.extern ov13_02223568
	.extern ov13_022235A4
	.extern ov13_022235DC
	.extern ov13_02223634
	.extern ov13_022236B8
	.extern ov13_02223770
	.extern ov13_02223A7C
	.extern ov13_02223AE4
	.extern ov13_02223B28
	.extern ov13_02223C10
	.extern ov13_02223C48
	.extern ov13_02223CA0
	.extern ov13_02223D24
	.extern ov13_02223DE0
	.extern ov13_02223DEC
	.extern ov13_02223E1C
	.extern ov13_02223E44
	.extern ov13_02223E6C
	.extern ov13_02223EA4
	.extern ov13_02223ED4
	.extern ov13_02223EE0
	.extern ov13_02223F84
	.extern ov13_02223FDC
	.extern ov13_02224100
	.extern ov13_02224164
	.extern ov13_02224330
	.extern ov13_022245C4
	.extern ov13_022245DC
	.extern ov13_02224618
	.extern ov13_02224624
	.extern ov13_02224694
	.extern ov13_022246E4
	.extern ov13_02224718
	.extern ov13_022247DC
	.extern ov13_0222483C
	.extern ov13_02224864
	.extern ov13_02224938
	.extern ov13_02224988
	.extern ov13_02224A24
	.extern ov13_02224A30
	.extern ov13_02224B2C
	.extern ov13_02224D00
	.extern ov13_02224DB4
	.extern ov13_02224F3C
	.extern ov13_02225320
	.extern ov13_02225358
	.extern ov13_02225510
	.extern ov13_022256C8
	.extern ov13_02225710
	.extern ov13_022259C8
	.extern ov13_02225B58
	.extern ov13_02225F14
	.extern ov13_022262D0
	.extern ov13_022262F8
	.extern ov13_02226370
	.extern ov13_022263C4
	.extern ov13_02226C0C
	.extern ov13_02226C38
	.extern ov13_02226C64
	.extern ov13_02226C7C
	.extern ov13_02226C94
	.extern ov13_02226CBC
	.extern ov13_02226CD8
	.extern ov13_02226CFC
	.extern ov13_02226D0C
	.extern ov13_02226D40
	.extern ov13_02226D74
	.extern ov13_02226E5C
	.extern ov13_02226EF0
	.extern ov13_02226F28
	.extern ov13_02226F3C
	.extern _0224267C
	.extern ov13_02242680
	.extern ov13_02242688
	.extern ov13_022426B0
	.extern ov13_02242AB0
	.extern ov13_02242EB0
	.extern ov13_022432B0
	.extern ov13_022436B0
	.extern ov13_02243AB0
	.extern ov13_02243EB0
	.extern ov13_022442B0
	.extern ov13_022446B0
	.extern ov13_02244AB0
	.extern _022459B0
	.extern ov13_022459B4
	.extern ov13_022459BC
	.extern ov13_02245A14
	.extern ov13_02245A20
	.extern ov13_02245A28
	.extern ov13_02245A58
	.extern ov13_02245A6C
	.extern ov13_02245AC4
	.extern ov13_02245ACC
	.extern ov13_02245AD8
	.extern ov13_02245B18
	.extern _0224CF98
	.extern ov13_0224CF9C
	.extern ov13_0224CFB8
	.extern ov13_0224CFBC
	.extern ov13_0224CFD8
	.extern ov13_0224D258
	.extern ov13_0224D658
	.extern ov13_0224D660
	.extern ov13_0224D790
	.extern ov13_0224D8C0
	.extern ov13_0224D930
	.extern ov13_0224DCF8
	.extern ov13_0224DD00
	.extern ov13_0224DD02
	.extern ov13_0224DD80
	.extern ov13_0224DD8C
	.extern ov13_0224DD90
	.extern ov13_0224DDA0
	.extern ov13_0224DDC0
	.extern ov13_0224DE80
	.extern ov13_0224DEE0
	.extern ov13_0224DF08
	.extern ov13_0224DF10
	.extern ov13_0224DF30
	.extern ov13_0224DF80
	.extern ov13_0224DFB0
	.extern ov13_0224DFB4
	.extern ov13_0224DFC4
	.extern ov13_0224DFDC
	.extern ov13_0224E09C
	.extern ov13_0224E0B0
	.extern ov13_0224E0C4
	.extern ov13_0224E144
	.extern ov13_0224E184
	.extern ov13_0224E270
	.extern ov13_0224E284
	.extern ov13_0224E2B0
	.extern ov13_0224E2E8
	.extern ov13_0224E380
	.extern ov13_0224E3D8
	.extern ov13_0224E3E0
	.extern ov13_0224E3E8
	.extern ov13_0224E3F0
	.extern ov13_0224E400
	.extern ov13_0224E408
	.extern ov13_0224E420
	.extern ov13_0224E440
	.extern ov13_0224E464
	.extern ov13_0224EC64



	thumb_func_start ov13_02222F28
ov13_02222F28: ; 0x02222F28
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x40
	mov r4, #1
	add r7, r0, #0
	sub r0, r4, #2
	str r0, [sp, #0xc]
	ldr r0, [r7, #0x24]
	mov r6, #0
	str r1, [sp, #4]
	cmp r0, #0
	bne _02222F42
	lsl r5, r4, #0x13
	b _02222F4A
_02222F42:
	cmp r0, #1
	bne _02222F4A
	mov r5, #3
	lsl r5, r5, #0x12
_02222F4A:
	mov r0, #3
	lsl r0, r0, #0x10
	orr r5, r0
	ldr r0, _022230DC ; =ov13_0224DE80
	mov r1, #0
	mov r2, #0x60
	bl MI_CpuFill8
	ldr r0, [r7, #0x28]
	cmp r0, #5
	bne _02222F68
	ldr r0, _022230DC ; =ov13_0224DE80
	mov r1, #1
	strb r1, [r0]
	b _02222F88
_02222F68:
	cmp r0, #0xd
	bne _02222F74
	ldr r0, _022230DC ; =ov13_0224DE80
	mov r1, #2
	strb r1, [r0]
	b _02222F88
_02222F74:
	cmp r0, #0x10
	bne _02222F80
	ldr r0, _022230DC ; =ov13_0224DE80
	mov r1, #3
	strb r1, [r0]
	b _02222F88
_02222F80:
	mov r0, #0
	add sp, #0x40
	mvn r0, r0
	pop {r3, r4, r5, r6, r7, pc}
_02222F88:
	mov r1, #0x14
	ldr r2, _022230DC ; =ov13_0224DE80
	mul r1, r6
	add r1, r1, #2
	add r0, r7, #0
	strb r6, [r2, #1]
	add r1, r2, r1
	ldr r2, [r7, #0x28]
	add r0, #0x2c
	bl MI_CpuCopy8
	bl WCM_ClearApList
	ldr r2, [r7]
	ldr r3, _022230E0 ; =0x0030BFFE
	mov r0, #0
	add r1, r7, #4
	bl ov13_02223478
	cmp r0, #0
	bne _02222FB4
	b _022230C2
_02222FB4:
	mov r0, #0
	str r0, [sp, #8]
	add r0, sp, #0x14
	bl OS_CreateAlarm
	mov r0, #0x12
	str r0, [sp]
	ldr r1, _022230E4 ; =0x003FEC42
	ldr r3, _022230E8 ; =ov13_02222C08
	add r0, sp, #0x14
	mov r2, #0
	bl OS_SetAlarm
	b _022230A4
_02222FD0:
	ldr r0, _022230EC ; =ov13_0224DDA0
	add r1, sp, #0x10
	mov r2, #1
	bl OS_ReceiveMessage
	ldr r0, [sp, #0x10]
	cmp r0, #0x13
	bhi _022230A2
	add r0, r0, r0
	add r0, pc
	ldrh r0, [r0, #6]
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	add pc, r0
_02222FEC: ; jump table
	.short _022230A2 - _02222FEC - 2 ; case 0
	.short _022230A2 - _02222FEC - 2 ; case 1
	.short _022230A2 - _02222FEC - 2 ; case 2
	.short _022230A2 - _02222FEC - 2 ; case 3
	.short _022230A4 - _02222FEC - 2 ; case 4
	.short _0222301C - _02222FEC - 2 ; case 5
	.short _022230A2 - _02222FEC - 2 ; case 6
	.short _022230A2 - _02222FEC - 2 ; case 7
	.short _022230A4 - _02222FEC - 2 ; case 8
	.short _022230A2 - _02222FEC - 2 ; case 9
	.short _02223062 - _02222FEC - 2 ; case 10
	.short _022230A2 - _02222FEC - 2 ; case 11
	.short _0222307C - _02222FEC - 2 ; case 12
	.short _02223082 - _02222FEC - 2 ; case 13
	.short _022230A2 - _02222FEC - 2 ; case 14
	.short _022230A2 - _02222FEC - 2 ; case 15
	.short _022230A2 - _02222FEC - 2 ; case 16
	.short _022230A2 - _02222FEC - 2 ; case 17
	.short _02223014 - _02222FEC - 2 ; case 18
	.short _0222301A - _02222FEC - 2 ; case 19
_02223014:
	cmp r6, #0
	bne _022230A4
	mov r4, #0
_0222301A:
	b _022230A4
_0222301C:
	cmp r6, #0
	bne _022230A4
	add r0, sp, #0x14
	bl OS_CancelAlarm
	ldr r0, _022230F0 ; =ov13_0224DDC0
	mov r1, #1
	bl ov13_02223434
	cmp r0, #1
	beq _02223036
	mov r4, #0
	b _022230A4
_02223036:
	ldr r1, _022230F0 ; =ov13_0224DDC0
	add r0, r7, #0
	bl ov13_02222BC0
	ldr r1, _022230F4 ; =ov13_0224DDC0
	mov r0, #0
	ldrh r1, [r1, #0xa]
	b _02223048
_02223046:
	add r0, r0, #1
_02223048:
	cmp r0, r1
	blt _02223046
	ldr r0, _022230F0 ; =ov13_0224DDC0
	ldr r1, _022230DC ; =ov13_0224DE80
	add r2, r5, #0
	bl ov13_02223634
	cmp r0, #0
	bne _0222305E
	mov r4, #0
	b _022230A4
_0222305E:
	mov r6, #1
	b _022230A4
_02223062:
	ldr r1, _022230F0 ; =ov13_0224DDC0
	add r0, r7, #0
	bl ov13_02222BC0
	ldr r0, _022230F0 ; =ov13_0224DDC0
	ldr r1, _022230DC ; =ov13_0224DE80
	add r2, r5, #0
	bl ov13_02223634
	cmp r0, #0
	bne _022230A4
	mov r4, #0
	b _022230A4
_0222307C:
	mov r4, #0
	str r4, [sp, #0xc]
	b _022230A4
_02223082:
	ldr r0, [sp, #8]
	add r0, r0, #1
	str r0, [sp, #8]
	cmp r0, #3
	bge _0222309E
	ldr r0, _022230F0 ; =ov13_0224DDC0
	ldr r1, _022230DC ; =ov13_0224DE80
	add r2, r5, #0
	bl ov13_02223634
	cmp r0, #0
	bne _022230A4
	mov r4, #0
	b _022230A4
_0222309E:
	mov r4, #0
	b _022230A4
_022230A2:
	mov r4, #0
_022230A4:
	cmp r4, #0
	bne _02222FD0
	add r0, sp, #0x14
	bl OS_CancelAlarm
	ldr r5, _022230EC ; =ov13_0224DDA0
	add r4, sp, #0x10
	mov r6, #0
_022230B4:
	add r0, r5, #0
	add r1, r4, #0
	add r2, r6, #0
	bl OS_ReceiveMessage
	cmp r0, #1
	beq _022230B4
_022230C2:
	ldr r0, [sp, #0xc]
	cmp r0, #0
	bne _022230CC
	mov r2, #1
	b _022230CE
_022230CC:
	mov r2, #0
_022230CE:
	ldr r0, [sp, #4]
	ldr r1, _022230F0 ; =ov13_0224DDC0
	bl ov13_02222BE4
	ldr r0, [sp, #0xc]
	add sp, #0x40
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
_022230DC: .word ov13_0224DE80
_022230E0: .word 0x0030BFFE
_022230E4: .word 0x003FEC42
_022230E8: .word ov13_02222C08
_022230EC: .word ov13_0224DDA0
_022230F0: .word ov13_0224DDC0
_022230F4: .word ov13_0224DDC0
	thumb_func_end ov13_02222F28
