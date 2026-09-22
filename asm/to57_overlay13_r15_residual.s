	.include "asm/macros.inc"
	.include "overlay_13_thumb_2.inc"
	.include "global.inc"

	.extern ov13_02223114
	.extern ov13_0224DEE0
	.extern ov13_0224DF30
	.extern ov13_0224DFB0

	.text

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
