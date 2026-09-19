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
	.extern ov13_02222C1C
	.extern ov13_02222CE0
	.extern ov13_02222D78
	.extern ov13_02222DB0
	.extern ov13_02222F28
	.extern ov13_022230F8
	.extern ov13_02223100
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


	thumb_func_start ov13_02223114
ov13_02223114: ; 0x02223114
	push {r3, lr}
	cmp r0, #0
	bne _0222311C
	b _022233C4
_0222311C:
	mov r1, #0
	ldrsh r2, [r0, r1]
	cmp r2, #7
	bls _02223126
	b _022233B6
_02223126:
	add r2, r2, r2
	add r2, pc
	ldrh r2, [r2, #6]
	lsl r2, r2, #0x10
	asr r2, r2, #0x10
	add pc, r2
_02223132: ; jump table
	.short _022233B6 - _02223132 - 2 ; case 0
	.short _02223142 - _02223132 - 2 ; case 1
	.short _0222336A - _02223132 - 2 ; case 2
	.short _022231C2 - _02223132 - 2 ; case 3
	.short _02223226 - _02223132 - 2 ; case 4
	.short _022231F4 - _02223132 - 2 ; case 5
	.short _022232C6 - _02223132 - 2 ; case 6
	.short _022233A2 - _02223132 - 2 ; case 7
_02223142:
	mov r2, #2
	ldrsh r0, [r0, r2]
	cmp r0, #0
	bne _022231AE
	ldr r2, _022233C8 ; =ov13_0224DEE0
	ldr r0, [r2, #0x18]
	cmp r0, #4
	bne _02223162
	mov r0, #3
	str r0, [r2, #0x18]
	ldr r2, [r2, #0x1c]
	cmp r2, #0
	beq _022231BA
	mov r0, #6
	blx r2
	pop {r3, pc}
_02223162:
	cmp r0, #6
	bne _02223188
	ldr r0, [r2, #0x10]
	ldr r1, [r2, #0x14]
	ldr r2, [r2, #4]
	bl WCM_SearchAsync
	cmp r0, #3
	beq _022231BA
	ldr r0, _022233C8 ; =ov13_0224DEE0
	mov r1, #3
	str r1, [r0, #0x18]
	ldr r2, [r0, #0x1c]
	cmp r2, #0
	beq _022231BA
	mov r0, #2
	mov r1, #0
	blx r2
	pop {r3, pc}
_02223188:
	cmp r0, #8
	bne _022231BA
	ldr r0, [r2, #0x20]
	ldr r1, [r2, #0x24]
	ldr r2, [r2, #0xc]
	bl WCM_ConnectAsync
	cmp r0, #3
	beq _022231BA
	ldr r0, _022233C8 ; =ov13_0224DEE0
	mov r1, #3
	str r1, [r0, #0x18]
	ldr r2, [r0, #0x1c]
	cmp r2, #0
	beq _022231BA
	mov r0, #2
	mov r1, #0
	blx r2
	pop {r3, pc}
_022231AE:
	ldr r0, _022233C8 ; =ov13_0224DEE0
	mov r3, #1
	str r3, [r0, #0x18]
	ldr r3, [r0, #0x1c]
	cmp r3, #0
	bne _022231BC
_022231BA:
	b _022233C4
_022231BC:
	add r0, r2, #0
	blx r3
	pop {r3, pc}
_022231C2:
	mov r2, #2
	ldrsh r0, [r0, r2]
	cmp r0, #0
	bne _022231E2
	ldr r0, _022233C8 ; =ov13_0224DEE0
	ldr r2, [r0, #0x18]
	cmp r2, #6
	bne _022232BE
	mov r2, #5
	str r2, [r0, #0x18]
	ldr r2, [r0, #0x1c]
	cmp r2, #0
	beq _022232BE
	mov r0, #8
	blx r2
	pop {r3, pc}
_022231E2:
	ldr r0, _022233C8 ; =ov13_0224DEE0
	mov r2, #3
	str r2, [r0, #0x18]
	ldr r2, [r0, #0x1c]
	cmp r2, #0
	beq _022232BE
	mov r0, #9
	blx r2
	pop {r3, pc}
_022231F4:
	mov r2, #2
	ldrsh r0, [r0, r2]
	cmp r0, #0
	bne _02223214
	ldr r0, _022233C8 ; =ov13_0224DEE0
	ldr r2, [r0, #0x18]
	cmp r2, #8
	bne _022232BE
	mov r2, #7
	str r2, [r0, #0x18]
	ldr r2, [r0, #0x1c]
	cmp r2, #0
	beq _022232BE
	mov r0, #0xc
	blx r2
	pop {r3, pc}
_02223214:
	ldr r0, _022233C8 ; =ov13_0224DEE0
	mov r2, #3
	str r2, [r0, #0x18]
	ldr r2, [r0, #0x1c]
	cmp r2, #0
	beq _022232BE
	mov r0, #0xd
	blx r2
	pop {r3, pc}
_02223226:
	mov r2, #2
	ldrsh r0, [r0, r2]
	cmp r0, #0
	bne _022232B2
	ldr r2, _022233C8 ; =ov13_0224DEE0
	ldr r0, [r2, #0x18]
	cmp r0, #4
	bne _02223246
	mov r0, #3
	str r0, [r2, #0x18]
	ldr r2, [r2, #0x1c]
	cmp r2, #0
	beq _022232BE
	mov r0, #0xa
	blx r2
	pop {r3, pc}
_02223246:
	cmp r0, #6
	bne _0222326C
	ldr r0, [r2, #0x10]
	ldr r1, [r2, #0x14]
	ldr r2, [r2, #4]
	bl WCM_SearchAsync
	cmp r0, #3
	beq _022232BE
	ldr r0, _022233C8 ; =ov13_0224DEE0
	mov r1, #3
	str r1, [r0, #0x18]
	ldr r2, [r0, #0x1c]
	cmp r2, #0
	beq _022232BE
	mov r0, #2
	mov r1, #0
	blx r2
	pop {r3, pc}
_0222326C:
	cmp r0, #2
	bne _0222328C
	bl WCM_CleanupAsync
	cmp r0, #3
	beq _022232BE
	ldr r0, _022233C8 ; =ov13_0224DEE0
	mov r1, #3
	str r1, [r0, #0x18]
	ldr r2, [r0, #0x1c]
	cmp r2, #0
	beq _022232BE
	mov r0, #2
	mov r1, #0
	blx r2
	pop {r3, pc}
_0222328C:
	cmp r0, #8
	bne _022232BE
	ldr r0, [r2, #0x20]
	ldr r1, [r2, #0x24]
	ldr r2, [r2, #0xc]
	bl WCM_ConnectAsync
	cmp r0, #3
	beq _022232BE
	ldr r0, _022233C8 ; =ov13_0224DEE0
	mov r1, #3
	str r1, [r0, #0x18]
	ldr r2, [r0, #0x1c]
	cmp r2, #0
	beq _022232BE
	mov r0, #2
	mov r1, #0
	blx r2
	pop {r3, pc}
_022232B2:
	ldr r0, _022233C8 ; =ov13_0224DEE0
	mov r2, #3
	str r2, [r0, #0x18]
	ldr r2, [r0, #0x1c]
	cmp r2, #0
	bne _022232C0
_022232BE:
	b _022233C4
_022232C0:
	mov r0, #0xb
	blx r2
	pop {r3, pc}
_022232C6:
	mov r2, #2
	ldrsh r0, [r0, r2]
	cmp r0, #0
	bne _02223358
	ldr r2, _022233C8 ; =ov13_0224DEE0
	ldr r0, [r2, #0x18]
	cmp r0, #4
	bne _022232E6
	mov r0, #3
	str r0, [r2, #0x18]
	ldr r2, [r2, #0x1c]
	cmp r2, #0
	beq _022233C4
	mov r0, #0xe
	blx r2
	pop {r3, pc}
_022232E6:
	cmp r0, #6
	bne _0222330C
	ldr r0, [r2, #0x10]
	ldr r1, [r2, #0x14]
	ldr r2, [r2, #4]
	bl WCM_SearchAsync
	cmp r0, #3
	beq _022233C4
	ldr r0, _022233C8 ; =ov13_0224DEE0
	mov r1, #3
	str r1, [r0, #0x18]
	ldr r2, [r0, #0x1c]
	cmp r2, #0
	beq _022233C4
	mov r0, #2
	mov r1, #0
	blx r2
	pop {r3, pc}
_0222330C:
	cmp r0, #2
	bne _0222332C
	bl WCM_CleanupAsync
	cmp r0, #3
	beq _022233C4
	ldr r0, _022233C8 ; =ov13_0224DEE0
	mov r1, #3
	str r1, [r0, #0x18]
	ldr r2, [r0, #0x1c]
	cmp r2, #0
	beq _022233C4
	mov r0, #2
	mov r1, #0
	blx r2
	pop {r3, pc}
_0222332C:
	cmp r0, #8
	bne _02223352
	ldr r0, [r2, #0x20]
	ldr r1, [r2, #0x24]
	ldr r2, [r2, #0xc]
	bl WCM_ConnectAsync
	cmp r0, #3
	beq _022233C4
	ldr r0, _022233C8 ; =ov13_0224DEE0
	mov r1, #3
	str r1, [r0, #0x18]
	ldr r2, [r0, #0x1c]
	cmp r2, #0
	beq _022233C4
	mov r0, #2
	mov r1, #0
	blx r2
	pop {r3, pc}
_02223352:
	mov r0, #3
	str r0, [r2, #0x18]
	pop {r3, pc}
_02223358:
	ldr r0, _022233C8 ; =ov13_0224DEE0
	mov r2, #3
	str r2, [r0, #0x18]
	ldr r2, [r0, #0x1c]
	cmp r2, #0
	beq _022233C4
	mov r0, #0xf
	blx r2
	pop {r3, pc}
_0222336A:
	mov r2, #2
	ldrsh r0, [r0, r2]
	cmp r0, #0
	bne _02223390
	ldr r0, _022233C8 ; =ov13_0224DEE0
	ldr r0, [r0, #0x18]
	cmp r0, #2
	bne _022233C4
	bl WCM_Finish
	ldr r0, _022233C8 ; =ov13_0224DEE0
	mov r1, #0
	str r1, [r0, #0x18]
	ldr r2, [r0, #0x1c]
	cmp r2, #0
	beq _022233C4
	mov r0, #0x14
	blx r2
	pop {r3, pc}
_02223390:
	ldr r0, _022233C8 ; =ov13_0224DEE0
	mov r3, #3
	str r3, [r0, #0x18]
	ldr r3, [r0, #0x1c]
	cmp r3, #0
	beq _022233C4
	add r0, r2, #0
	blx r3
	pop {r3, pc}
_022233A2:
	ldr r0, _022233C8 ; =ov13_0224DEE0
	ldr r2, [r0, #0x18]
	cmp r2, #5
	bne _022233C4
	ldr r2, [r0, #0x1c]
	cmp r2, #0
	beq _022233C4
	mov r0, #5
	blx r2
	pop {r3, pc}
_022233B6:
	ldr r0, _022233C8 ; =ov13_0224DEE0
	ldr r2, [r0, #0x1c]
	cmp r2, #0
	beq _022233C4
	mov r0, #1
	mov r1, #0
	blx r2
_022233C4:
	pop {r3, pc}
	nop
_022233C8: .word ov13_0224DEE0
	thumb_func_end ov13_02223114


	thumb_func_start ov13_022233CC
ov13_022233CC: ; 0x022233CC
	push {r3, lr}
	ldr r1, _0222342C ; =ov13_0224DEE0
	ldr r0, [r1, #0x18]
	cmp r0, #8
	bhi _02223422
	add r0, r0, r0
	add r0, pc
	ldrh r0, [r0, #6]
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	add pc, r0
_022233E2: ; jump table
	.short _02223422 - _022233E2 - 2 ; case 0
	.short _02223412 - _022233E2 - 2 ; case 1
	.short _02223422 - _022233E2 - 2 ; case 2
	.short _02223422 - _022233E2 - 2 ; case 3
	.short _02223422 - _022233E2 - 2 ; case 4
	.short _022233F4 - _022233E2 - 2 ; case 5
	.short _02223422 - _022233E2 - 2 ; case 6
	.short _02223406 - _022233E2 - 2 ; case 7
	.short _02223422 - _022233E2 - 2 ; case 8
_022233F4:
	mov r0, #0
	add r1, r0, #0
	add r2, r0, #0
	bl WCM_SearchAsync
	cmp r0, #3
	beq _02223426
	mov r0, #0
	pop {r3, pc}
_02223406:
	bl WCM_DisconnectAsync
	cmp r0, #3
	beq _02223426
	mov r0, #0
	pop {r3, pc}
_02223412:
	ldr r0, [r1, #8]
	ldr r1, _02223430 ; =ov13_02223114
	bl WCM_StartupAsync
	cmp r0, #3
	beq _02223426
	mov r0, #0
	pop {r3, pc}
_02223422:
	mov r0, #0
	pop {r3, pc}
_02223426:
	mov r0, #1
	pop {r3, pc}
	nop
_0222342C: .word ov13_0224DEE0
_02223430: .word ov13_02223114
	thumb_func_end ov13_022233CC


	thumb_func_start ov13_02223434
ov13_02223434: ; 0x02223434
	push {r3, r4, r5, r6, r7, lr}
	add r5, r0, #0
	mov r0, #1
	add r7, r1, #0
	bl WCM_LockApList
	bl WCM_CountApList
	add r6, r0, #0
	cmp r6, #0
	ble _0222346C
	mov r4, #0
	cmp r6, #0
	ble _0222346C
_02223450:
	cmp r4, r7
	bge _0222346C
	lsl r0, r4, #0x10
	lsr r0, r0, #0x10
	bl WCM_PointApList
	add r1, r5, #0
	mov r2, #0xc0
	bl MIi_CpuCopy32
	add r4, r4, #1
	add r5, #0xc0
	cmp r4, r6
	blt _02223450
_0222346C:
	mov r0, #0
	bl WCM_LockApList
	add r0, r6, #0
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
	thumb_func_end ov13_02223434


	thumb_func_start ov13_02223478
ov13_02223478: ; 0x02223478
	push {r3, r4, r5, r6, r7, lr}
	str r3, [sp]
	add r5, r0, #0
	add r4, r1, #0
	add r6, r2, #0
	bl OS_DisableInterrupts
	add r7, r0, #0
	ldr r1, _02223554 ; =ov13_0224DEE0
	ldr r0, [sp]
	cmp r5, #0
	str r0, [r1, #4]
	beq _022234AC
	ldr r2, _02223558 ; =ov13_0224DF08
	mov r1, #0
_02223496:
	ldrb r0, [r5]
	add r1, r1, #1
	add r5, r5, #1
	strb r0, [r2]
	add r2, r2, #1
	cmp r1, #6
	blt _02223496
	ldr r1, _02223558 ; =ov13_0224DF08
	ldr r0, _02223554 ; =ov13_0224DEE0
	str r1, [r0, #0x10]
	b _022234BC
_022234AC:
	ldr r0, _02223558 ; =ov13_0224DF08
	mov r1, #0xff
	mov r2, #6
	bl MI_CpuFill8
	ldr r1, _0222355C ; =_02108FC0
	ldr r0, _02223554 ; =ov13_0224DEE0
	str r1, [r0, #0x10]
_022234BC:
	cmp r4, #0
	beq _022234FA
	cmp r6, #0
	ble _022234FA
	cmp r6, #0x20
	bgt _022234FA
	mov r0, #0
	cmp r6, #0
	ble _022234DE
	ldr r2, _02223560 ; =ov13_0224DF10
_022234D0:
	ldrb r1, [r4]
	add r0, r0, #1
	add r4, r4, #1
	strb r1, [r2]
	add r2, r2, #1
	cmp r0, r6
	blt _022234D0
_022234DE:
	cmp r0, #0x20
	bge _022234F2
	ldr r1, _02223560 ; =ov13_0224DF10
	add r2, r1, r0
	mov r1, #0
_022234E8:
	add r0, r0, #1
	strb r1, [r2]
	add r2, r2, #1
	cmp r0, #0x20
	blt _022234E8
_022234F2:
	ldr r1, _02223560 ; =ov13_0224DF10
	ldr r0, _02223554 ; =ov13_0224DEE0
	str r1, [r0, #0x14]
	b _0222350A
_022234FA:
	ldr r0, _02223560 ; =ov13_0224DF10
	mov r1, #0xff
	mov r2, #0x20
	bl MI_CpuFill8
	ldr r1, _02223564 ; =_02108FC8
	ldr r0, _02223554 ; =ov13_0224DEE0
	str r1, [r0, #0x14]
_0222350A:
	ldr r2, _02223554 ; =ov13_0224DEE0
	ldr r0, [r2, #0x18]
	cmp r0, #3
	bne _02223530
	ldr r0, [r2, #0x10]
	ldr r1, [r2, #0x14]
	ldr r2, [r2, #4]
	bl WCM_SearchAsync
	cmp r0, #3
	bne _02223548
	ldr r0, _02223554 ; =ov13_0224DEE0
	mov r1, #6
	str r1, [r0, #0x18]
	add r0, r7, #0
	bl OS_RestoreInterrupts
	mov r0, #1
	pop {r3, r4, r5, r6, r7, pc}
_02223530:
	bl ov13_022233CC
	cmp r0, #1
	bne _02223548
	ldr r0, _02223554 ; =ov13_0224DEE0
	mov r1, #6
	str r1, [r0, #0x18]
	add r0, r7, #0
	bl OS_RestoreInterrupts
	mov r0, #1
	pop {r3, r4, r5, r6, r7, pc}
_02223548:
	add r0, r7, #0
	bl OS_RestoreInterrupts
	mov r0, #0
	pop {r3, r4, r5, r6, r7, pc}
	nop
_02223554: .word ov13_0224DEE0
_02223558: .word ov13_0224DF08
_0222355C: .word _02108FC0
_02223560: .word ov13_0224DF10
_02223564: .word _02108FC8
	thumb_func_end ov13_02223478


	thumb_func_start ov13_02223568
ov13_02223568: ; 0x02223568
	push {r4, lr}
	bl OS_DisableInterrupts
	add r4, r0, #0
	ldr r0, _022235A0 ; =ov13_0224DEE0
	ldr r0, [r0, #0x18]
	cmp r0, #5
	bne _02223596
	mov r0, #0
	add r1, r0, #0
	add r2, r0, #0
	bl WCM_SearchAsync
	cmp r0, #3
	bne _02223596
	ldr r0, _022235A0 ; =ov13_0224DEE0
	mov r1, #4
	str r1, [r0, #0x18]
	add r0, r4, #0
	bl OS_RestoreInterrupts
	mov r0, #1
	pop {r4, pc}
_02223596:
	add r0, r4, #0
	bl OS_RestoreInterrupts
	mov r0, #0
	pop {r4, pc}
	.balign 4, 0
_022235A0: .word ov13_0224DEE0
	thumb_func_end ov13_02223568


	thumb_func_start ov13_022235A4
ov13_022235A4: ; 0x022235A4
	push {r4, lr}
	bl OS_DisableInterrupts
	add r4, r0, #0
	ldr r0, _022235D8 ; =ov13_0224DEE0
	ldr r0, [r0, #0x18]
	cmp r0, #7
	bne _022235CC
	bl WCM_DisconnectAsync
	cmp r0, #3
	bne _022235CC
	ldr r0, _022235D8 ; =ov13_0224DEE0
	mov r1, #4
	str r1, [r0, #0x18]
	add r0, r4, #0
	bl OS_RestoreInterrupts
	mov r0, #1
	pop {r4, pc}
_022235CC:
	add r0, r4, #0
	bl OS_RestoreInterrupts
	mov r0, #0
	pop {r4, pc}
	nop
_022235D8: .word ov13_0224DEE0
	thumb_func_end ov13_022235A4


	thumb_func_start ov13_022235DC
ov13_022235DC: ; 0x022235DC
	push {r4, lr}
	bl OS_DisableInterrupts
	add r4, r0, #0
	ldr r0, _02223630 ; =ov13_0224DEE0
	ldr r0, [r0, #0x18]
	cmp r0, #3
	bne _0222360E
	bl WCM_CleanupAsync
	cmp r0, #3
	beq _022235FE
	add r0, r4, #0
	bl OS_RestoreInterrupts
	mov r0, #0
	pop {r4, pc}
_022235FE:
	ldr r0, _02223630 ; =ov13_0224DEE0
	mov r1, #2
	str r1, [r0, #0x18]
	add r0, r4, #0
	bl OS_RestoreInterrupts
	mov r0, #1
	pop {r4, pc}
_0222360E:
	bl ov13_022233CC
	cmp r0, #1
	bne _02223626
	ldr r0, _02223630 ; =ov13_0224DEE0
	mov r1, #2
	str r1, [r0, #0x18]
	add r0, r4, #0
	bl OS_RestoreInterrupts
	mov r0, #1
	pop {r4, pc}
_02223626:
	add r0, r4, #0
	bl OS_RestoreInterrupts
	mov r0, #0
	pop {r4, pc}
	.balign 4, 0
_02223630: .word ov13_0224DEE0
	thumb_func_end ov13_022235DC


	thumb_func_start ov13_02223634
ov13_02223634: ; 0x02223634
	push {r3, r4, r5, r6, r7, lr}
	add r7, r0, #0
	add r5, r1, #0
	add r6, r2, #0
	bl OS_DisableInterrupts
	ldr r1, _022236B4 ; =ov13_0224DEE0
	add r4, r0, #0
	str r6, [r1, #0xc]
	cmp r5, #0
	beq _02223656
	ldr r1, [r1, #0x24]
	add r0, r5, #0
	mov r2, #0x50
	bl MI_CpuCopy8
	b _02223660
_02223656:
	ldr r0, [r1, #0x24]
	mov r1, #0
	mov r2, #0x50
	bl MI_CpuFill8
_02223660:
	ldr r1, _022236B4 ; =ov13_0224DEE0
	add r0, r7, #0
	ldr r1, [r1, #0x20]
	mov r2, #0xc0
	bl MIi_CpuCopy32
	bl ov13_022233CC
	cmp r0, #1
	bne _02223684
	ldr r0, _022236B4 ; =ov13_0224DEE0
	mov r1, #8
	str r1, [r0, #0x18]
	add r0, r4, #0
	bl OS_RestoreInterrupts
	mov r0, #1
	pop {r3, r4, r5, r6, r7, pc}
_02223684:
	ldr r2, _022236B4 ; =ov13_0224DEE0
	ldr r0, [r2, #0x18]
	cmp r0, #3
	bne _022236AA
	ldr r0, [r2, #0x20]
	ldr r1, [r2, #0x24]
	ldr r2, [r2, #0xc]
	bl WCM_ConnectAsync
	cmp r0, #3
	bne _022236AA
	ldr r0, _022236B4 ; =ov13_0224DEE0
	mov r1, #8
	str r1, [r0, #0x18]
	add r0, r4, #0
	bl OS_RestoreInterrupts
	mov r0, #1
	pop {r3, r4, r5, r6, r7, pc}
_022236AA:
	add r0, r4, #0
	bl OS_RestoreInterrupts
	mov r0, #0
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
_022236B4: .word ov13_0224DEE0
	thumb_func_end ov13_02223634


	thumb_func_start ov13_022236B8
ov13_022236B8: ; 0x022236B8
	push {r3, r4, r5, r6, r7, lr}
	add r5, r1, #0
	add r7, r0, #0
	str r2, [sp]
	bl OS_DisableInterrupts
	add r4, r0, #0
	add r0, r5, #0
	ldr r2, _02223764 ; =ov13_0224DEE0
	add r0, #0x53
	mov r1, #3
	bic r0, r1
	add r6, r0, #0
	str r5, [r2, #0x24]
	add r6, #0x2f
	mov r1, #0x1f
	ldr r3, _02223768 ; =0x0000231F
	str r0, [r2, #8]
	bic r6, r1
	str r6, [r2]
	add r6, r6, r3
	bic r6, r1
	str r6, [r2, #0x20]
	add r6, #0xdf
	bic r6, r1
	str r6, [r0, #4]
	ldr r1, [sp]
	ldr r0, [r2, #8]
	add r5, r5, r1
	ldr r1, [r0, #4]
	sub r1, r5, r1
	str r1, [r0, #8]
	ldr r0, [r2, #8]
	mov r1, #0
	str r1, [r0, #0xc]
	ldr r0, [r2, #8]
	mov r1, #3
	str r1, [r0]
	str r7, [r2, #0x1c]
	ldr r0, [r2, #0x18]
	cmp r0, #0
	bne _0222372A
	sub r3, #0x1f
	ldr r0, [r2]
	add r1, r3, #0
	bl WCM_Init
	cmp r0, #0
	beq _02223724
	add r0, r4, #0
	bl OS_RestoreInterrupts
	mov r0, #0
	pop {r3, r4, r5, r6, r7, pc}
_02223724:
	ldr r0, _02223764 ; =ov13_0224DEE0
	mov r1, #1
	str r1, [r0, #0x18]
_0222372A:
	ldr r0, _02223764 ; =ov13_0224DEE0
	ldr r1, [r0, #0x18]
	cmp r1, #1
	bne _02223758
	ldr r0, [r0, #8]
	ldr r1, _0222376C ; =ov13_02223114
	bl WCM_StartupAsync
	cmp r0, #3
	beq _02223748
	add r0, r4, #0
	bl OS_RestoreInterrupts
	mov r0, #0
	pop {r3, r4, r5, r6, r7, pc}
_02223748:
	ldr r0, _02223764 ; =ov13_0224DEE0
	mov r1, #4
	str r1, [r0, #0x18]
	add r0, r4, #0
	bl OS_RestoreInterrupts
	mov r0, #1
	pop {r3, r4, r5, r6, r7, pc}
_02223758:
	add r0, r4, #0
	bl OS_RestoreInterrupts
	mov r0, #0
	pop {r3, r4, r5, r6, r7, pc}
	nop
_02223764: .word ov13_0224DEE0
_02223768: .word 0x0000231F
_0222376C: .word ov13_02223114
	thumb_func_end ov13_022236B8


	thumb_func_start ov13_02223770
ov13_02223770: ; 0x02223770
	push {r3, lr}
	cmp r0, #0
	bne _02223778
	b _02223A72
_02223778:
	mov r1, #0
	ldrsh r2, [r0, r1]
	cmp r2, #9
	bls _02223782
	b _02223A64
_02223782:
	add r2, r2, r2
	add r2, pc
	ldrh r2, [r2, #6]
	lsl r2, r2, #0x10
	asr r2, r2, #0x10
	add pc, r2
_0222378E: ; jump table
	.short _02223A64 - _0222378E - 2 ; case 0
	.short _022237A2 - _0222378E - 2 ; case 1
	.short _022239F2 - _0222378E - 2 ; case 2
	.short _02223828 - _0222378E - 2 ; case 3
	.short _02223896 - _0222378E - 2 ; case 4
	.short _02223860 - _0222378E - 2 ; case 5
	.short _0222393E - _0222378E - 2 ; case 6
	.short _02223A2E - _0222378E - 2 ; case 7
	.short _02223A44 - _0222378E - 2 ; case 8
	.short _02223A52 - _0222378E - 2 ; case 9
_022237A2:
	mov r2, #2
	ldrsh r0, [r0, r2]
	cmp r0, #0
	bne _02223814
	ldr r2, _02223A74 ; =ov13_0224DF30
	ldr r0, [r2, #0x2c]
	cmp r0, #4
	bne _022237C4
	mov r0, #3
	str r0, [r2, #0x2c]
	ldr r0, _02223A78 ; =ov13_0224DFB0
	ldr r2, [r0]
	cmp r2, #0
	beq _02223858
	mov r0, #6
	blx r2
	pop {r3, pc}
_022237C4:
	cmp r0, #6
	bne _022237EC
	ldr r0, [r2, #0x44]
	ldr r1, [r2, #0x48]
	ldr r2, [r2, #0x60]
	bl WCM_SearchAsync
	cmp r0, #3
	beq _02223858
	ldr r0, _02223A74 ; =ov13_0224DF30
	mov r1, #3
	str r1, [r0, #0x2c]
	ldr r0, _02223A78 ; =ov13_0224DFB0
	ldr r2, [r0]
	cmp r2, #0
	beq _02223858
	mov r0, #2
	mov r1, #0
	blx r2
	pop {r3, pc}
_022237EC:
	cmp r0, #8
	bne _02223858
	ldr r0, [r2, #0x4c]
	ldr r1, [r2, #0x28]
	ldr r2, [r2, #0x58]
	bl WCM_ConnectAsync
	cmp r0, #3
	beq _02223858
	ldr r0, _02223A74 ; =ov13_0224DF30
	mov r1, #3
	str r1, [r0, #0x2c]
	ldr r0, _02223A78 ; =ov13_0224DFB0
	ldr r2, [r0]
	cmp r2, #0
	beq _02223858
	mov r0, #2
	mov r1, #0
	blx r2
	pop {r3, pc}
_02223814:
	ldr r0, _02223A74 ; =ov13_0224DF30
	mov r3, #1
	str r3, [r0, #0x2c]
	ldr r0, _02223A78 ; =ov13_0224DFB0
	ldr r3, [r0]
	cmp r3, #0
	beq _02223858
	add r0, r2, #0
	blx r3
	pop {r3, pc}
_02223828:
	mov r2, #2
	ldrsh r0, [r0, r2]
	cmp r0, #0
	bne _0222384A
	ldr r0, _02223A74 ; =ov13_0224DF30
	ldr r2, [r0, #0x2c]
	cmp r2, #6
	bne _02223858
	mov r2, #5
	str r2, [r0, #0x2c]
	ldr r0, _02223A78 ; =ov13_0224DFB0
	ldr r2, [r0]
	cmp r2, #0
	beq _02223858
	mov r0, #8
	blx r2
	pop {r3, pc}
_0222384A:
	ldr r0, _02223A74 ; =ov13_0224DF30
	mov r2, #3
	str r2, [r0, #0x2c]
	ldr r0, _02223A78 ; =ov13_0224DFB0
	ldr r2, [r0]
	cmp r2, #0
	bne _0222385A
_02223858:
	b _02223A72
_0222385A:
	mov r0, #9
	blx r2
	pop {r3, pc}
_02223860:
	mov r2, #2
	ldrsh r0, [r0, r2]
	cmp r0, #0
	bne _02223882
	ldr r0, _02223A74 ; =ov13_0224DF30
	ldr r2, [r0, #0x2c]
	cmp r2, #8
	bne _0222395A
	mov r2, #7
	str r2, [r0, #0x2c]
	ldr r0, _02223A78 ; =ov13_0224DFB0
	ldr r2, [r0]
	cmp r2, #0
	beq _0222395A
	mov r0, #0xc
	blx r2
	pop {r3, pc}
_02223882:
	ldr r0, _02223A74 ; =ov13_0224DF30
	mov r2, #3
	str r2, [r0, #0x2c]
	ldr r0, _02223A78 ; =ov13_0224DFB0
	ldr r2, [r0]
	cmp r2, #0
	beq _0222395A
	mov r0, #0xd
	blx r2
	pop {r3, pc}
_02223896:
	mov r2, #2
	ldrsh r0, [r0, r2]
	cmp r0, #0
	bne _0222392A
	ldr r2, _02223A74 ; =ov13_0224DF30
	ldr r0, [r2, #0x2c]
	cmp r0, #4
	bne _022238B8
	mov r0, #3
	str r0, [r2, #0x2c]
	ldr r0, _02223A78 ; =ov13_0224DFB0
	ldr r2, [r0]
	cmp r2, #0
	beq _0222395A
	mov r0, #0xa
	blx r2
	pop {r3, pc}
_022238B8:
	cmp r0, #6
	bne _022238E0
	ldr r0, [r2, #0x44]
	ldr r1, [r2, #0x48]
	ldr r2, [r2, #0x60]
	bl WCM_SearchAsync
	cmp r0, #3
	beq _0222395A
	ldr r0, _02223A74 ; =ov13_0224DF30
	mov r1, #3
	str r1, [r0, #0x2c]
	ldr r0, _02223A78 ; =ov13_0224DFB0
	ldr r2, [r0]
	cmp r2, #0
	beq _0222395A
	mov r0, #2
	mov r1, #0
	blx r2
	pop {r3, pc}
_022238E0:
	cmp r0, #2
	bne _02223902
	bl WCM_CleanupAsync
	cmp r0, #3
	beq _0222395A
	ldr r0, _02223A74 ; =ov13_0224DF30
	mov r1, #3
	str r1, [r0, #0x2c]
	ldr r0, _02223A78 ; =ov13_0224DFB0
	ldr r2, [r0]
	cmp r2, #0
	beq _0222395A
	mov r0, #2
	mov r1, #0
	blx r2
	pop {r3, pc}
_02223902:
	cmp r0, #8
	bne _0222395A
	ldr r0, [r2, #0x4c]
	ldr r1, [r2, #0x28]
	ldr r2, [r2, #0x58]
	bl WCM_ConnectAsync
	cmp r0, #3
	beq _0222395A
	ldr r0, _02223A74 ; =ov13_0224DF30
	mov r1, #3
	str r1, [r0, #0x2c]
	ldr r0, _02223A78 ; =ov13_0224DFB0
	ldr r2, [r0]
	cmp r2, #0
	beq _0222395A
	mov r0, #2
	mov r1, #0
	blx r2
	pop {r3, pc}
_0222392A:
	ldr r0, _02223A74 ; =ov13_0224DF30
	mov r2, #3
	str r2, [r0, #0x2c]
	ldr r0, _02223A78 ; =ov13_0224DFB0
	ldr r2, [r0]
	cmp r2, #0
	beq _0222395A
	mov r0, #0xb
	blx r2
	pop {r3, pc}
_0222393E:
	mov r2, #2
	ldrsh r0, [r0, r2]
	cmp r0, #0
	bne _022239DE
	ldr r2, _02223A74 ; =ov13_0224DF30
	ldr r0, [r2, #0x2c]
	cmp r0, #4
	bne _02223962
	mov r0, #3
	str r0, [r2, #0x2c]
	ldr r0, _02223A78 ; =ov13_0224DFB0
	ldr r2, [r0]
	cmp r2, #0
	bne _0222395C
_0222395A:
	b _02223A72
_0222395C:
	mov r0, #0xe
	blx r2
	pop {r3, pc}
_02223962:
	cmp r0, #6
	bne _0222398A
	ldr r0, [r2, #0x44]
	ldr r1, [r2, #0x48]
	ldr r2, [r2, #0x60]
	bl WCM_SearchAsync
	cmp r0, #3
	beq _02223A72
	ldr r0, _02223A74 ; =ov13_0224DF30
	mov r1, #3
	str r1, [r0, #0x2c]
	ldr r0, _02223A78 ; =ov13_0224DFB0
	ldr r2, [r0]
	cmp r2, #0
	beq _02223A72
	mov r0, #2
	mov r1, #0
	blx r2
	pop {r3, pc}
_0222398A:
	cmp r0, #2
	bne _022239AC
	bl WCM_CleanupAsync
	cmp r0, #3
	beq _02223A72
	ldr r0, _02223A74 ; =ov13_0224DF30
	mov r1, #3
	str r1, [r0, #0x2c]
	ldr r0, _02223A78 ; =ov13_0224DFB0
	ldr r2, [r0]
	cmp r2, #0
	beq _02223A72
	mov r0, #2
	mov r1, #0
	blx r2
	pop {r3, pc}
_022239AC:
	cmp r0, #8
	bne _022239D4
	ldr r0, [r2, #0x4c]
	ldr r1, [r2, #0x28]
	ldr r2, [r2, #0x58]
	bl WCM_ConnectAsync
	cmp r0, #3
	beq _02223A72
	ldr r0, _02223A74 ; =ov13_0224DF30
	mov r1, #3
	str r1, [r0, #0x2c]
	ldr r0, _02223A78 ; =ov13_0224DFB0
	ldr r2, [r0]
	cmp r2, #0
	beq _02223A72
	mov r0, #2
	mov r1, #0
	blx r2
	pop {r3, pc}
_022239D4:
	cmp r0, #7
	bne _02223A72
	mov r0, #3
	str r0, [r2, #0x2c]
	pop {r3, pc}
_022239DE:
	ldr r0, _02223A74 ; =ov13_0224DF30
	mov r2, #3
	str r2, [r0, #0x2c]
	ldr r0, _02223A78 ; =ov13_0224DFB0
	ldr r2, [r0]
	cmp r2, #0
	beq _02223A72
	mov r0, #0xf
	blx r2
	pop {r3, pc}
_022239F2:
	mov r2, #2
	ldrsh r0, [r0, r2]
	cmp r0, #0
	bne _02223A1A
	ldr r0, _02223A74 ; =ov13_0224DF30
	ldr r0, [r0, #0x2c]
	cmp r0, #2
	bne _02223A72
	bl WCM_Finish
	ldr r0, _02223A74 ; =ov13_0224DF30
	mov r1, #0
	str r1, [r0, #0x2c]
	ldr r0, _02223A78 ; =ov13_0224DFB0
	ldr r2, [r0]
	cmp r2, #0
	beq _02223A72
	mov r0, #0x14
	blx r2
	pop {r3, pc}
_02223A1A:
	ldr r0, _02223A74 ; =ov13_0224DF30
	mov r3, #3
	str r3, [r0, #0x2c]
	ldr r0, _02223A78 ; =ov13_0224DFB0
	ldr r3, [r0]
	cmp r3, #0
	beq _02223A72
	add r0, r2, #0
	blx r3
	pop {r3, pc}
_02223A2E:
	ldr r0, _02223A74 ; =ov13_0224DF30
	ldr r0, [r0, #0x2c]
	cmp r0, #5
	bne _02223A72
	ldr r0, _02223A78 ; =ov13_0224DFB0
	ldr r2, [r0]
	cmp r2, #0
	beq _02223A72
	mov r0, #5
	blx r2
	pop {r3, pc}
_02223A44:
	ldr r0, _02223A78 ; =ov13_0224DFB0
	ldr r2, [r0]
	cmp r2, #0
	beq _02223A72
	mov r0, #4
	blx r2
	pop {r3, pc}
_02223A52:
	ldr r0, _02223A74 ; =ov13_0224DF30
	str r1, [r0, #0x2c]
	ldr r0, _02223A78 ; =ov13_0224DFB0
	ldr r2, [r0]
	cmp r2, #0
	beq _02223A72
	mov r0, #3
	blx r2
	pop {r3, pc}
_02223A64:
	ldr r0, _02223A78 ; =ov13_0224DFB0
	ldr r2, [r0]
	cmp r2, #0
	beq _02223A72
	mov r0, #1
	mov r1, #0
	blx r2
_02223A72:
	pop {r3, pc}
	.balign 4, 0
_02223A74: .word ov13_0224DF30
_02223A78: .word ov13_0224DFB0
	thumb_func_end ov13_02223770


	thumb_func_start ov13_02223A7C
ov13_02223A7C: ; 0x02223A7C
	push {r3, lr}
	ldr r1, _02223ADC ; =ov13_0224DF30
	ldr r0, [r1, #0x2c]
	cmp r0, #8
	bhi _02223AD2
	add r0, r0, r0
	add r0, pc
	ldrh r0, [r0, #6]
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	add pc, r0
_02223A92: ; jump table
	.short _02223AD2 - _02223A92 - 2 ; case 0
	.short _02223AC2 - _02223A92 - 2 ; case 1
	.short _02223AD2 - _02223A92 - 2 ; case 2
	.short _02223AD2 - _02223A92 - 2 ; case 3
	.short _02223AD2 - _02223A92 - 2 ; case 4
	.short _02223AA4 - _02223A92 - 2 ; case 5
	.short _02223AD2 - _02223A92 - 2 ; case 6
	.short _02223AB6 - _02223A92 - 2 ; case 7
	.short _02223AD2 - _02223A92 - 2 ; case 8
_02223AA4:
	mov r0, #0
	add r1, r0, #0
	add r2, r0, #0
	bl WCM_SearchAsync
	cmp r0, #3
	beq _02223AD6
	mov r0, #0
	pop {r3, pc}
_02223AB6:
	bl WCM_DisconnectAsync
	cmp r0, #3
	beq _02223AD6
	mov r0, #0
	pop {r3, pc}
_02223AC2:
	ldr r0, [r1, #8]
	ldr r1, _02223AE0 ; =ov13_02223770
	bl WCM_StartupAsync
	cmp r0, #3
	beq _02223AD6
	mov r0, #0
	pop {r3, pc}
_02223AD2:
	mov r0, #0
	pop {r3, pc}
_02223AD6:
	mov r0, #1
	pop {r3, pc}
	nop
_02223ADC: .word ov13_0224DF30
_02223AE0: .word ov13_02223770
	thumb_func_end ov13_02223A7C


	thumb_func_start ov13_02223AE4
ov13_02223AE4: ; 0x02223AE4
	push {r3, r4, r5, r6, r7, lr}
	add r5, r0, #0
	mov r0, #1
	add r7, r1, #0
	bl WCM_LockApList
	bl WCM_CountApList
	add r6, r0, #0
	cmp r6, #0
	ble _02223B1C
	mov r4, #0
	cmp r6, #0
	ble _02223B1C
_02223B00:
	cmp r4, r7
	bge _02223B1C
	lsl r0, r4, #0x10
	lsr r0, r0, #0x10
	bl WCM_PointApList
	add r1, r5, #0
	mov r2, #0xc0
	bl MIi_CpuCopy32
	add r4, r4, #1
	add r5, #0xc0
	cmp r4, r6
	blt _02223B00
_02223B1C:
	mov r0, #0
	bl WCM_LockApList
	add r0, r6, #0
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
	thumb_func_end ov13_02223AE4


	thumb_func_start ov13_02223B28
ov13_02223B28: ; 0x02223B28
	push {r3, r4, r5, r6, r7, lr}
	str r3, [sp]
	add r5, r0, #0
	add r4, r1, #0
	add r6, r2, #0
	bl OS_DisableInterrupts
	add r7, r0, #0
	ldr r1, _02223BFC ; =ov13_0224DF30
	ldr r0, [sp]
	cmp r5, #0
	str r0, [r1, #0x60]
	ldr r0, _02223C00 ; =ov13_0224E3E8
	str r0, [r1, #0x44]
	beq _02223B58
	mov r2, #0
_02223B48:
	ldrb r1, [r5]
	add r2, r2, #1
	add r5, r5, #1
	strb r1, [r0]
	add r0, r0, #1
	cmp r2, #6
	blt _02223B48
	b _02223B66
_02223B58:
	mov r1, #0xff
	mov r2, #6
	bl MI_CpuFill8
	ldr r1, _02223C04 ; =_02108FC0
	ldr r0, _02223BFC ; =ov13_0224DF30
	str r1, [r0, #0x44]
_02223B66:
	ldr r2, _02223C08 ; =ov13_0224E420
	ldr r0, _02223BFC ; =ov13_0224DF30
	cmp r4, #0
	str r2, [r0, #0x48]
	beq _02223BA2
	cmp r6, #0
	ble _02223BA2
	cmp r6, #0x20
	bge _02223BA2
	mov r0, #0
	cmp r6, #0
	ble _02223B8C
_02223B7E:
	ldrb r1, [r4]
	add r0, r0, #1
	add r4, r4, #1
	strb r1, [r2]
	add r2, r2, #1
	cmp r0, r6
	blt _02223B7E
_02223B8C:
	cmp r0, #0x20
	bge _02223BB2
	ldr r1, _02223C08 ; =ov13_0224E420
	add r2, r1, r0
	mov r1, #0
_02223B96:
	add r0, r0, #1
	strb r1, [r2]
	add r2, r2, #1
	cmp r0, #0x20
	blt _02223B96
	b _02223BB2
_02223BA2:
	ldr r0, _02223C08 ; =ov13_0224E420
	mov r1, #0xff
	mov r2, #0x20
	bl MI_CpuFill8
	ldr r1, _02223C0C ; =_02108FC8
	ldr r0, _02223BFC ; =ov13_0224DF30
	str r1, [r0, #0x48]
_02223BB2:
	ldr r2, _02223BFC ; =ov13_0224DF30
	ldr r0, [r2, #0x2c]
	cmp r0, #3
	bne _02223BD8
	ldr r1, [r2, #0x48]
	ldr r0, _02223C00 ; =ov13_0224E3E8
	ldr r2, [r2, #0x60]
	bl WCM_SearchAsync
	cmp r0, #3
	bne _02223BF0
	ldr r0, _02223BFC ; =ov13_0224DF30
	mov r1, #6
	str r1, [r0, #0x2c]
	add r0, r7, #0
	bl OS_RestoreInterrupts
	mov r0, #1
	pop {r3, r4, r5, r6, r7, pc}
_02223BD8:
	bl ov13_02223A7C
	cmp r0, #1
	bne _02223BF0
	ldr r0, _02223BFC ; =ov13_0224DF30
	mov r1, #6
	str r1, [r0, #0x2c]
	add r0, r7, #0
	bl OS_RestoreInterrupts
	mov r0, #1
	pop {r3, r4, r5, r6, r7, pc}
_02223BF0:
	add r0, r7, #0
	bl OS_RestoreInterrupts
	mov r0, #0
	pop {r3, r4, r5, r6, r7, pc}
	nop
_02223BFC: .word ov13_0224DF30
_02223C00: .word ov13_0224E3E8
_02223C04: .word _02108FC0
_02223C08: .word ov13_0224E420
_02223C0C: .word _02108FC8
	thumb_func_end ov13_02223B28
