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
	thumb_func_start ov13_02223EE0
ov13_02223EE0: ; 0x02223EE0
	push {r3, r4, r5, r6, r7, lr}
	add r6, r0, #0
	ldr r0, _02223F74 ; =ov13_0224DF30
	mov r4, #1
	str r6, [r0, #0x70]
	bl ov13_02223E44
	mov r0, #0xd0
	add r7, r6, #0
	mul r7, r0
	ldr r0, _02223F78 ; =0x000024D0
	add r1, r7, r0
	mov r0, #0xc0
	mul r0, r6
	add r0, r1, r0
	ldr r1, _02223F74 ; =ov13_0224DF30
	ldr r1, [r1, #4]
	blx r1
	ldr r6, _02223F74 ; =ov13_0224DF30
	cmp r0, #0
	str r0, [r6, #0x54]
	bne _02223F10
	sub r0, r4, #2
	pop {r3, r4, r5, r6, r7, pc}
_02223F10:
	mov r1, #0x1f
	add r2, r0, r1
	add r0, r1, #0
	sub r0, #0x3f
	add r3, r2, #0
	and r3, r0
	ldr r0, _02223F7C ; =0x00002490
	str r3, [r6, #0x40]
	add r2, r7, r0
	add r0, r3, r2
	add r0, r0, r1
	sub r1, #0x3f
	and r0, r1
	str r0, [r6, #0x5c]
	ldr r0, _02223F80 ; =ov13_02223EA4
	add r1, r3, #0
	bl ov13_02223D24
	cmp r0, #0
	bne _02223F3C
	sub r0, r4, #3
	pop {r3, r4, r5, r6, r7, pc}
_02223F3C:
	mov r6, #0
	add r7, r4, #0
_02223F40:
	mov r0, #0xa
	bl OS_Sleep
	bl ov13_02223E6C
	cmp r0, #0
	beq _02223F6C
_02223F4E:
	cmp r0, #4
	beq _02223F64
	cmp r0, #5
	beq _02223F64
	cmp r0, #6
	bne _02223F60
	add r4, r6, #0
	add r5, r7, #0
	b _02223F64
_02223F60:
	mov r4, #0
	sub r5, r4, #2
_02223F64:
	bl ov13_02223E6C
	cmp r0, #0
	bne _02223F4E
_02223F6C:
	cmp r4, #0
	bne _02223F40
	add r0, r5, #0
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
_02223F74: .word ov13_0224DF30
_02223F78: .word 0x000024D0
_02223F7C: .word 0x00002490
_02223F80: .word ov13_02223EA4
	thumb_func_end ov13_02223EE0


	thumb_func_start ov13_02223F84
ov13_02223F84: ; 0x02223F84
	push {r3, r4, r5, r6, r7, lr}
	mov r5, #1
	bl ov13_02223C48
	cmp r0, #0
	beq _02223FC2
	mov r6, #0
	add r4, r6, #0
	mov r7, #0xa
_02223F96:
	add r0, r7, #0
	bl OS_Sleep
	bl ov13_02223E6C
	cmp r0, #0
	beq _02223FBE
_02223FA4:
	cmp r0, #4
	beq _02223FB6
	cmp r0, #5
	beq _02223FB6
	cmp r0, #0x14
	bne _02223FB4
	add r5, r4, #0
	b _02223FB6
_02223FB4:
	add r5, r6, #0
_02223FB6:
	bl ov13_02223E6C
	cmp r0, #0
	bne _02223FA4
_02223FBE:
	cmp r5, #0
	bne _02223F96
_02223FC2:
	ldr r1, _02223FD8 ; =ov13_0224DF30
	ldr r0, [r1, #0x54]
	cmp r0, #0
	beq _02223FD4
	ldr r1, [r1, #0xc]
	blx r1
	ldr r0, _02223FD8 ; =ov13_0224DF30
	mov r1, #0
	str r1, [r0, #0x54]
_02223FD4:
	mov r0, #1
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
_02223FD8: .word ov13_0224DF30
	thumb_func_end ov13_02223F84


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


	thumb_func_start ov13_02224100
ov13_02224100: ; 0x02224100
	push {r3, r4, r5, r6, r7, lr}
	ldr r0, _02224160 ; =ov13_0224DF30
	mov r5, #1
	ldr r0, [r0, #0x20]
	cmp r0, #0
	beq _0222414C
	bl ov13_02223C10
	cmp r0, #0
	beq _02224146
	mov r6, #0
	add r4, r6, #0
	mov r7, #0xa
_0222411A:
	add r0, r7, #0
	bl OS_Sleep
	bl ov13_02223E6C
	cmp r0, #0
	beq _02224142
_02224128:
	cmp r0, #4
	beq _0222413A
	cmp r0, #5
	beq _0222413A
	cmp r0, #0xe
	bne _02224138
	add r5, r4, #0
	b _0222413A
_02224138:
	add r5, r6, #0
_0222413A:
	bl ov13_02223E6C
	cmp r0, #0
	bne _02224128
_02224142:
	cmp r5, #0
	bne _0222411A
_02224146:
	ldr r0, _02224160 ; =ov13_0224DF30
	mov r1, #0
	str r1, [r0, #0x20]
_0222414C:
	ldr r0, _02224160 ; =ov13_0224DF30
	ldr r1, [r0, #0x24]
	cmp r1, #0
	beq _0222415C
	mov r1, #0
	str r1, [r0, #0x24]
	bl SOC_Cleanup
_0222415C:
	pop {r3, r4, r5, r6, r7, pc}
	nop
_02224160: .word ov13_0224DF30
	thumb_func_end ov13_02224100


	thumb_func_start ov13_02224164
ov13_02224164: ; 0x02224164
	push {r4, r5, r6, r7, lr}
	sub sp, #0x6c
	str r0, [sp]
	mov r0, #0
	str r0, [sp, #0x20]
	str r0, [sp, #0x1c]
	ldr r0, [sp]
	str r1, [sp, #4]
	add r6, r0, #4
	add r0, r1, #0
	add r4, r0, #4
	ldr r0, [sp, #0x20]
	str r2, [sp, #8]
	str r0, [sp, #0xc]
	ldr r0, [sp]
	ldr r0, [r0]
	cmp r0, #0
	bls _02224238
_02224188:
	add r2, sp, #0x48
	add r2, #2
	mov r1, #0x22
	mov r0, #0
_02224190:
	strb r0, [r2]
	add r2, r2, #1
	sub r1, r1, #1
	bne _02224190
	add r0, sp, #0x48
	add r0, #2
	add r1, r6, #4
	mov r2, #0x20
	bl memcpy
	add r0, sp, #0x48
	ldr r1, [r6]
	mov r7, #0
	add r0, #2
	strb r7, [r0, r1]
	ldr r0, [sp, #4]
	ldr r0, [r0]
	str r0, [sp, #0x10]
	cmp r0, #0
	bls _0222421C
	add r0, r6, #0
	str r0, [sp, #0x24]
	add r0, #0x28
	ldr r5, [r6]
	str r0, [sp, #0x24]
_022241C2:
	cmp r5, #0
	beq _0222421C
	cmp r5, #0x20
	bhi _0222421C
	cmp r5, #1
	bne _022241D8
	ldrb r0, [r6, #4]
	cmp r0, #0
	beq _0222421C
	cmp r0, #0x20
	beq _0222421C
_022241D8:
	add r0, sp, #0x48
	add r0, #2
	bl strlen
	add r2, r0, #0
	add r0, sp, #0x48
	add r0, #2
	add r1, r4, #4
	bl memcmp
	cmp r0, #0
	bne _02224212
	add r1, r4, #0
	ldr r0, [sp, #0x24]
	add r1, #0x28
	mov r2, #6
	bl memcmp
	cmp r0, #0
	bne _02224212
	ldrh r1, [r6, #0x2e]
	ldrh r0, [r4, #0x2e]
	cmp r1, r0
	beq _02224212
	cmp r1, #0
	bne _02224212
	mov r0, #1
	str r0, [sp, #0x20]
	b _0222421C
_02224212:
	ldr r0, [sp, #0x10]
	add r7, r7, #1
	add r4, #0x30
	cmp r7, r0
	blo _022241C2
_0222421C:
	ldr r0, [sp, #0x20]
	cmp r0, #0
	bne _02224238
	ldr r0, [sp, #4]
	add r6, #0x30
	add r4, r0, #4
	ldr r0, [sp, #0xc]
	add r0, r0, #1
	str r0, [sp, #0xc]
	ldr r0, [sp]
	ldr r1, [r0]
	ldr r0, [sp, #0xc]
	cmp r0, r1
	blo _02224188
_02224238:
	ldr r0, [sp, #0x20]
	cmp r0, #0
	bne _02224314
	add r2, sp, #0x28
	mov r1, #0x22
	mov r0, #0
_02224244:
	strb r0, [r2]
	add r2, r2, #1
	sub r1, r1, #1
	bne _02224244
	mov r0, #0
	str r0, [sp, #0x18]
	str r0, [sp, #0x14]
	ldr r0, [sp]
	ldr r6, [sp, #0x18]
	add r4, r0, #4
	ldr r0, [sp, #4]
	add r5, r0, #4
	ldr r0, [r0]
	cmp r0, #0
	bls _022242A0
	add r7, sp, #0x28
_02224264:
	add r0, r7, #0
	add r1, r5, #4
	mov r2, #0x20
	bl memcpy
	ldr r1, [r5]
	mov r0, #0
	strb r0, [r7, r1]
	ldr r0, _0222432C ; =ov13_02245AC4
	bl strlen
	add r2, r0, #0
	ldr r1, _0222432C ; =ov13_02245AC4
	add r0, r7, #0
	bl memcmp
	cmp r0, #0
	bne _02224294
	ldrh r0, [r5, #0x2e]
	cmp r0, #0
	bne _02224294
	mov r0, #1
	str r0, [sp, #0x14]
	b _022242A0
_02224294:
	ldr r0, [sp, #4]
	add r6, r6, #1
	ldr r0, [r0]
	add r5, #0x30
	cmp r6, r0
	blo _02224264
_022242A0:
	mov r0, #0
	str r0, [sp, #0xc]
	ldr r0, [sp]
	ldr r0, [r0]
	cmp r0, #0
	bls _02224304
	ldr r7, _0222432C ; =ov13_02245AC4
	add r5, sp, #0x28
_022242B0:
	add r0, r5, #0
	add r1, r4, #4
	mov r2, #0x20
	bl memcpy
	ldr r1, [r4]
	mov r0, #0
	strb r0, [r5, r1]
	add r0, r5, #0
	bl strlen
	add r6, r0, #0
	add r0, r7, #0
	bl strlen
	cmp r6, r0
	bne _022242F2
	add r0, r7, #0
	bl strlen
	add r2, r0, #0
	add r0, r5, #0
	add r1, r7, #0
	bl memcmp
	cmp r0, #0
	bne _022242F2
	ldrh r0, [r4, #0x2e]
	cmp r0, #0
	bne _022242F2
	mov r0, #1
	str r0, [sp, #0x18]
	b _02224304
_022242F2:
	ldr r0, [sp, #0xc]
	add r4, #0x30
	add r0, r0, #1
	str r0, [sp, #0xc]
	ldr r0, [sp]
	ldr r1, [r0]
	ldr r0, [sp, #0xc]
	cmp r0, r1
	blo _022242B0
_02224304:
	ldr r0, [sp, #0x18]
	cmp r0, #0
	beq _02224314
	ldr r0, [sp, #0x14]
	cmp r0, #0
	bne _02224314
	mov r0, #1
	str r0, [sp, #0x20]
_02224314:
	ldr r0, [sp, #0x20]
	cmp r0, #0
	beq _02224324
	ldr r1, [sp, #0xc]
	ldr r0, [sp, #8]
	str r1, [r0]
	mov r0, #1
	str r0, [sp, #0x1c]
_02224324:
	ldr r0, [sp, #0x1c]
	add sp, #0x6c
	pop {r4, r5, r6, r7, pc}
	nop
_0222432C: .word ov13_02245AC4
	thumb_func_end ov13_02224164


	thumb_func_start ov13_02224330
ov13_02224330: ; 0x02224330
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x70
	mov r0, #0
	str r0, [sp, #8]
	str r0, [sp, #0x20]
	ldr r0, _022245A8 ; =ov13_0224DF30
	mov r4, #0
	ldr r1, [r0, #0x70]
	mov r0, #0x30
	mul r0, r1
	str r0, [sp, #0x10]
	add r0, #0x34
	str r0, [sp, #0x10]
	ldr r1, [sp, #0x10]
	mov r0, #1
	mvn r4, r4
	bl ov13_02226CD8
	str r0, [sp, #0xc]
	cmp r0, #0
	beq _02224368
	ldr r1, [sp, #0x10]
	mov r0, #1
	bl ov13_02226CD8
	str r0, [sp, #8]
	cmp r0, #0
	bne _0222436A
_02224368:
	b _0222458E
_0222436A:
	mov r0, #0
	str r0, [sp, #0x1c]
	b _02224556
_02224370:
	bl ov13_02226CBC
	ldr r1, _022245AC ; =ov13_02245A58
	ldr r1, [r1, #0xc]
	cmp r0, r1
	bhs _0222445A
	mov r0, #0
	ldr r3, _022245B0 ; =0x0030BFFE
	add r1, r0, #0
	add r2, r0, #0
	bl ov13_02223B28
	cmp r0, #0
	bne _02224392
	mov r4, #1
	mvn r4, r4
	b _0222458E
_02224392:
	add r0, sp, #0x24
	bl OS_CreateAlarm
	mov r0, #0x13
	str r0, [sp]
	ldr r1, _022245B4 ; =0x000FFB10
	ldr r3, _022245B8 ; =ov13_02223ED4
	add r0, sp, #0x24
	mov r2, #0
	bl OS_SetAlarm
	ldr r7, _022245A8 ; =ov13_0224DF30
	mov r5, #1
	mov r4, #0
	add r6, sp, #0x24
_022243B0:
	mov r0, #0xa
	bl OS_Sleep
	bl ov13_02226CBC
	ldr r1, _022245AC ; =ov13_02245A58
	ldr r1, [r1, #0xc]
	cmp r0, r1
	bhs _02224444
	ldr r0, [r7, #0x10]
	cmp r0, #0
	bne _02224444
	bl ov13_02223E6C
	cmp r0, #0
	beq _02224440
_022243D0:
	cmp r0, #0x13
	bhi _02224436
	add r0, r0, r0
	add r0, pc
	ldrh r0, [r0, #6]
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	add pc, r0
_022243E0: ; jump table
	.short _02224436 - _022243E0 - 2 ; case 0
	.short _02224436 - _022243E0 - 2 ; case 1
	.short _02224436 - _022243E0 - 2 ; case 2
	.short _02224436 - _022243E0 - 2 ; case 3
	.short _02224438 - _022243E0 - 2 ; case 4
	.short _0222440C - _022243E0 - 2 ; case 5
	.short _02224436 - _022243E0 - 2 ; case 6
	.short _02224436 - _022243E0 - 2 ; case 7
	.short _02224438 - _022243E0 - 2 ; case 8
	.short _02224436 - _022243E0 - 2 ; case 9
	.short _02224432 - _022243E0 - 2 ; case 10
	.short _02224436 - _022243E0 - 2 ; case 11
	.short _02224436 - _022243E0 - 2 ; case 12
	.short _02224436 - _022243E0 - 2 ; case 13
	.short _02224436 - _022243E0 - 2 ; case 14
	.short _02224436 - _022243E0 - 2 ; case 15
	.short _02224436 - _022243E0 - 2 ; case 16
	.short _02224436 - _022243E0 - 2 ; case 17
	.short _02224438 - _022243E0 - 2 ; case 18
	.short _02224408 - _022243E0 - 2 ; case 19
_02224408:
	mov r5, #0
	b _02224438
_0222440C:
	ldr r0, [r7, #0x5c]
	ldr r1, [r7, #0x70]
	bl ov13_02223AE4
	cmp r0, r4
	ble _02224438
	add r4, r0, #0
	add r0, r6, #0
	bl OS_CancelAlarm
	mov r0, #0x13
	str r0, [sp]
	ldr r1, _022245B4 ; =0x000FFB10
	ldr r3, _022245B8 ; =ov13_02223ED4
	add r0, r6, #0
	mov r2, #0
	bl OS_SetAlarm
	b _02224438
_02224432:
	mov r5, #0
	b _02224438
_02224436:
	mov r5, #0
_02224438:
	bl ov13_02223E6C
	cmp r0, #0
	bne _022243D0
_02224440:
	cmp r5, #0
	bne _022243B0
_02224444:
	add r0, sp, #0x24
	bl OS_CancelAlarm
_0222444A:
	bl ov13_02223E6C
	cmp r0, #0
	bne _0222444A
	ldr r1, _022245A8 ; =ov13_0224DF30
	ldr r0, [r1, #0x10]
	cmp r0, #0
	beq _0222445C
_0222445A:
	b _02224566
_0222445C:
	mov r0, #0
	str r0, [sp, #0x14]
	ldr r0, [r1, #0x70]
	cmp r4, r0
	blt _0222446C
	ldr r0, [sp, #0x14]
	sub r4, r0, #6
	b _0222458E
_0222446C:
	ldr r0, [sp, #0x14]
	ldr r5, [r1, #0x5c]
	str r0, [sp, #0x18]
	cmp r4, #0
	ble _022244E2
	ble _022244E2
	ldr r0, [sp, #0xc]
	ldr r7, [sp, #0xc]
	str r0, [sp, #4]
	add r0, #8
	add r6, r7, #0
	str r0, [sp, #4]
	add r6, #0x2c
_02224486:
	add r1, r5, #0
	ldr r0, [sp, #4]
	add r1, #0xc
	mov r2, #0x20
	bl memcpy
	ldrh r0, [r5, #0xa]
	str r0, [r7, #4]
	ldrh r0, [r5, #0xa]
	add r1, r7, r0
	mov r0, #0
	strb r0, [r1, #8]
	ldrh r1, [r5, #0x2c]
	mov r0, #0x10
	tst r0, r1
	beq _022244AA
	mov r0, #1
	b _022244AC
_022244AA:
	mov r0, #0
_022244AC:
	strh r0, [r7, #0x32]
	ldrb r0, [r5, #4]
	add r7, #0x30
	strb r0, [r6]
	ldrb r0, [r5, #5]
	strb r0, [r6, #1]
	ldrb r0, [r5, #6]
	strb r0, [r6, #2]
	ldrb r0, [r5, #7]
	strb r0, [r6, #3]
	ldrb r0, [r5, #8]
	strb r0, [r6, #4]
	ldrb r0, [r5, #9]
	add r5, #0xc0
	strb r0, [r6, #5]
	ldr r0, [sp, #4]
	add r6, #0x30
	add r0, #0x30
	str r0, [sp, #4]
	ldr r0, [sp, #0x14]
	add r0, r0, #1
	str r0, [sp, #0x14]
	ldr r0, [sp, #0x18]
	add r0, r0, #1
	str r0, [sp, #0x18]
	cmp r0, r4
	blt _02224486
_022244E2:
	ldr r0, [sp, #0xc]
	str r4, [r0]
	ldr r0, _022245A8 ; =ov13_0224DF30
	ldr r0, [r0, #0x38]
	cmp r0, #1
	beq _0222453C
	ldr r0, [sp, #0xc]
	ldr r1, [sp, #8]
	add r2, sp, #0x20
	bl ov13_02224164
	cmp r0, #0
	beq _0222453C
	ldr r0, [sp, #0xc]
	ldr r2, [sp, #0x20]
	add r1, r0, #4
	mov r0, #0x30
	mul r0, r2
	add r4, r1, r0
	ldr r0, _022245A8 ; =ov13_0224DF30
	add r1, r4, #4
	str r2, [r0, #0x18]
	ldr r0, _022245BC ; =ov13_0224E440
	bl strcpy
	add r0, r4, #0
	add r4, #0x28
	ldrb r2, [r4]
	ldr r1, _022245C0 ; =ov13_0224E3E0
	add r0, #0x28
	strb r2, [r1]
	ldrb r2, [r0, #1]
	strb r2, [r1, #1]
	ldrb r2, [r0, #2]
	strb r2, [r1, #2]
	ldrb r2, [r0, #3]
	strb r2, [r1, #3]
	ldrb r2, [r0, #4]
	strb r2, [r1, #4]
	ldrb r0, [r0, #5]
	strb r0, [r1, #5]
	add r0, sp, #0x50
	bl ov13_02226D40
	b _02224566
_0222453C:
	ldr r0, [sp, #8]
	ldr r1, [sp, #0xc]
	ldr r2, [sp, #0x10]
	bl memcpy
	ldr r0, _022245A8 ; =ov13_0224DF30
	mov r1, #2
	str r1, [r0, #0x38]
	bl ov13_02226F3C
	ldr r0, [sp, #0x1c]
	add r0, r0, #1
	str r0, [sp, #0x1c]
_02224556:
	ldr r0, [sp, #0x1c]
	cmp r0, #0x1e
	bge _02224566
	ldr r0, _022245A8 ; =ov13_0224DF30
	ldr r0, [r0, #0x10]
	cmp r0, #0
	bne _02224566
	b _02224370
_02224566:
	ldr r0, [sp, #0x1c]
	cmp r0, #0x1e
	bge _02224578
	bl ov13_02226CBC
	ldr r1, _022245AC ; =ov13_02245A58
	ldr r1, [r1, #0xc]
	cmp r0, r1
	bls _0222457E
_02224578:
	mov r4, #2
	mvn r4, r4
	b _0222458E
_0222457E:
	ldr r0, _022245A8 ; =ov13_0224DF30
	ldr r0, [r0, #0x10]
	cmp r0, #0
	beq _0222458C
	mov r4, #7
	mvn r4, r4
	b _0222458E
_0222458C:
	mov r4, #1
_0222458E:
	ldr r0, [sp, #0xc]
	cmp r0, #0
	beq _02224598
	bl ov13_02226CFC
_02224598:
	ldr r0, [sp, #8]
	cmp r0, #0
	beq _022245A2
	bl ov13_02226CFC
_022245A2:
	add r0, r4, #0
	add sp, #0x70
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
_022245A8: .word ov13_0224DF30
_022245AC: .word ov13_02245A58
_022245B0: .word 0x0030BFFE
_022245B4: .word 0x000FFB10
_022245B8: .word ov13_02223ED4
_022245BC: .word ov13_0224E440
_022245C0: .word ov13_0224E3E0
	thumb_func_end ov13_02224330
