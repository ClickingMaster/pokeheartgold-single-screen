	.include "asm/macros.inc"
	.include "overlay_13_thumb_2.inc"
	.include "global.inc"

	.extern ov13_0224DEE0

	.text

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
