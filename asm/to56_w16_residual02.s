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



	.extern ov13_02223ED4
	thumb_func_start ov13_02223FDC
ov13_02223FDC: ; 0x02223FDC
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x30
	ldr r0, _022240EC ; =ov13_0224DF30
	mov r4, #1
	ldr r3, [r0, #0x5c]
	ldr r0, [r0, #0x18]
	mov r2, #0xc0
	add r1, r0, #0
	mul r1, r2
	sub r5, r4, #3
	add r6, r3, r1
	bne _02223FFA
	add sp, #0x30
	mov r0, #0
	pop {r3, r4, r5, r6, r7, pc}
_02223FFA:
	add r0, r6, #0
	mov r1, #0
	lsl r2, r2, #0xa
	bl ov13_02223CA0
	cmp r0, #0
	bne _0222400E
	add sp, #0x30
	sub r0, r4, #3
	pop {r3, r4, r5, r6, r7, pc}
_0222400E:
	add r0, sp, #4
	bl OS_CreateAlarm
	mov r0, #0x12
	str r0, [sp]
	ldr r1, _022240F0 ; =0x003FEC42
	ldr r3, _022240F4 ; =ov13_02223ED4
	add r0, sp, #4
	mov r2, #0
	bl OS_SetAlarm
	mov r7, #0
_02224026:
	bl ov13_02226CBC
	ldr r1, _022240F8 ; =ov13_02245A58
	ldr r1, [r1, #0xc]
	cmp r0, r1
	blo _02224038
	mov r5, #2
	mvn r5, r5
	b _022240B8
_02224038:
	ldr r0, _022240EC ; =ov13_0224DF30
	ldr r0, [r0, #0x10]
	cmp r0, #0
	beq _02224046
	mov r5, #7
	mvn r5, r5
	b _022240B8
_02224046:
	mov r0, #0xa
	bl OS_Sleep
	bl ov13_02223E6C
	cmp r0, #0
	beq _022240B4
_02224054:
	cmp r0, #0xc
	bgt _0222406A
	bge _0222407E
	cmp r0, #5
	bgt _022240AA
	cmp r0, #4
	blt _022240AA
	beq _022240AC
	cmp r0, #5
	beq _022240AC
	b _022240AA
_0222406A:
	cmp r0, #0x13
	bgt _022240AA
	cmp r0, #0xd
	blt _022240AA
	beq _02224084
	cmp r0, #0x12
	beq _022240AC
	cmp r0, #0x13
	beq _022240AC
	b _022240AA
_0222407E:
	add r4, r7, #0
	mov r5, #1
	b _022240AC
_02224084:
	ldr r0, _022240EC ; =ov13_0224DF30
	ldr r0, [r0, #0x10]
	cmp r0, #0
	beq _02224094
	mov r4, #0
	add r5, r4, #0
	sub r5, #8
	b _022240AC
_02224094:
	mov r2, #3
	add r0, r6, #0
	mov r1, #0
	lsl r2, r2, #0x10
	bl ov13_02223CA0
	cmp r0, #0
	bne _022240AC
	add sp, #0x30
	add r0, r5, #0
	pop {r3, r4, r5, r6, r7, pc}
_022240AA:
	mov r4, #0
_022240AC:
	bl ov13_02223E6C
	cmp r0, #0
	bne _02224054
_022240B4:
	cmp r4, #0
	bne _02224026
_022240B8:
	add r0, sp, #4
	bl OS_CancelAlarm
_022240BE:
	bl ov13_02223E6C
	cmp r0, #0
	bne _022240BE
	cmp r5, #0
	ble _022240E6
	ldr r0, _022240EC ; =ov13_0224DF30
	mov r1, #1
	str r1, [r0, #0x20]
	ldr r0, _022240FC ; =ov13_02245A6C
	bl SOC_Startup
	cmp r0, #0
	bge _022240E0
	mov r5, #1
	mvn r5, r5
	b _022240E6
_022240E0:
	ldr r0, _022240EC ; =ov13_0224DF30
	mov r1, #1
	str r1, [r0, #0x24]
_022240E6:
	add r0, r5, #0
	add sp, #0x30
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
_022240EC: .word ov13_0224DF30
_022240F0: .word 0x003FEC42
_022240F4: .word ov13_02223ED4
_022240F8: .word ov13_02245A58
_022240FC: .word ov13_02245A6C
	thumb_func_end ov13_02223FDC
