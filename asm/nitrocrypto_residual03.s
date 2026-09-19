	.include "asm/macros.inc"
	.include "nitrocrypto.inc"
	.include "global.inc"

	.text

	.extern ASN1Skip
	.extern BN_CTX_free
	.extern BN_CTX_init
	.extern BN_CTX_new
	.extern BN_MONT_CTX_free
	.extern BN_MONT_CTX_init
	.extern BN_MONT_CTX_new
	.extern BN_MONT_CTX_set_word
	.extern BN_RECP_CTX_free
	.extern BN_RECP_CTX_init
	.extern BN_RECP_CTX_set
	.extern BN_bin2bn
	.extern BN_bn2bin
	.extern BN_clear
	.extern BN_clear_free
	.extern BN_copy
	.extern BN_div_recp
	.extern BN_free
	.extern BN_gen_exp_bits
	.extern BN_gen_exp_string
	.extern BN_init
	.extern BN_is_bit_set
	.extern BN_mod
	.extern BN_mod_exp
	.extern BN_mod_exp_mont
	.extern BN_mod_exp_recp
	.extern BN_mod_inverse_word
	.extern BN_mod_mul_reciprocal
	.extern BN_new
	.extern BN_num_bits
	.extern BN_num_bits_word
	.extern BN_reciprocal
	.extern BN_set_bit
	.extern BN_set_word
	.extern BN_sqr
	.extern BN_ucmp
	.extern CRYPTO_RC4Encrypt
	.extern CRYPTO_SetAllocator
	.extern CRYPTO_VerifySignature
	.extern CRYPTO_VerifySignatureWithHash
	.extern CRYPTOi_MyAlloc
	.extern CRYPTOi_MyAllocFunc
	.extern CRYPTOi_MyFree
	.extern CRYPTOi_MyFreeFunc
	.extern CRYPTOi_RSA
	.extern ParseSignHash
	.extern SkipPadding
	.extern TEMPNAME__$p16
	.extern TEMPNAME__$p2
	.extern TEMPNAME__$p4
	.extern bits$7808
	.extern bn_div_words
	.extern bn_expand2
	.extern bn_fix_top
	.extern bn_from_montgomery_words
	.extern bn_sqr_normal
	.extern bn_zexpand
	.extern shift_val


	arm_func_start BN_div
BN_div: ; 0x02239240
	stmdb sp!, {r4, r5, r6, r7, r8, sb, sl, fp, lr}
	sub sp, sp, #0x4c
	mov r6, r3
	ldr r3, [r6, #4]
	mov r8, r0
	str r1, [sp]
	str r2, [sp, #4]
	cmp r3, #0
	ldr r4, [sp, #0x70]
	beq _02239280
	cmp r3, #1
	bne _02239290
	ldr r0, [r6]
	ldr r0, [r0]
	cmp r0, #0
	bne _02239290
_02239280:
	add sp, sp, #0x4c
	mov r0, #0
	ldmia sp!, {r4, r5, r6, r7, r8, sb, sl, fp, lr}
	bx lr
_02239290:
	ldr r0, [sp, #4]
	mov r1, r6
	bl BN_ucmp
	cmp r0, #0
	bge _022392F0
	ldr r0, [sp]
	cmp r0, #0
	beq _022392CC
	ldr r1, [sp, #4]
	bl BN_copy
	cmp r0, #0
	addeq sp, sp, #0x4c
	moveq r0, #0
	ldmeqia sp!, {r4, r5, r6, r7, r8, sb, sl, fp, lr}
	bxeq lr
_022392CC:
	cmp r8, #0
	beq _022392E0
	mov r0, r8
	mov r1, #0
	bl BN_set_word
_022392E0:
	add sp, sp, #0x4c
	mov r0, #1
	ldmia sp!, {r4, r5, r6, r7, r8, sb, sl, fp, lr}
	bx lr
_022392F0:
	ldr r3, [r4]
	add r0, r4, #4
	mov r2, #0x14
	mla r1, r3, r2, r0
	mov r3, #0
	str r3, [r1, #0xc]
	ldr r5, [r4]
	str r1, [sp, #0x14]
	add r4, r5, #1
	mla r1, r4, r2, r0
	add r3, r5, #2
	str r1, [sp, #0x18]
	mla r1, r3, r2, r0
	cmp r8, #0
	str r1, [sp, #0x1c]
	addeq r1, r5, #3
	mlaeq r8, r1, r2, r0
	mov r0, r6
	bl BN_num_bits
	mov r1, r0, lsr #0x1f
	rsb r0, r1, r0, lsl #27
	add r0, r1, r0, ror #27
	rsb r0, r0, #0x20
	str r0, [sp, #8]
	ldr r0, [sp, #0x1c]
	ldr r2, [sp, #8]
	mov r1, r6
	bl BN_lshift
	cmp r0, #0
	addeq sp, sp, #0x4c
	moveq r0, #0
	ldmeqia sp!, {r4, r5, r6, r7, r8, sb, sl, fp, lr}
	bxeq lr
	ldr r2, [sp, #8]
	ldr r0, [sp, #0x18]
	ldr r1, [sp, #4]
	ldr r3, [sp, #0x1c]
	mov r4, #0
	add r2, r2, #0x20
	str r4, [r3, #0xc]
	bl BN_lshift
	cmp r0, #0
	addeq sp, sp, #0x4c
	moveq r0, r4
	ldmeqia sp!, {r4, r5, r6, r7, r8, sb, sl, fp, lr}
	bxeq lr
	ldr r0, [sp, #0x18]
	mov r1, r4
	str r1, [r0, #0xc]
	ldr r0, [sp, #0x1c]
	ldr r0, [r0, #4]
	str r0, [sp, #0x28]
	ldr r0, [sp, #0x18]
	ldr r1, [sp, #0x28]
	ldr r4, [r0, #4]
	add r0, sp, #0x38
	sub r1, r4, r1
	str r1, [sp, #0x10]
	bl BN_init
	ldr r0, [sp, #0x18]
	ldr r2, [r0]
	ldr r0, [sp, #0x28]
	sub r1, r0, #1
	ldr r0, [sp, #0x10]
	add r0, r2, r0, lsl #2
	str r0, [sp, #0x38]
	ldr r0, [sp, #0x28]
	str r0, [sp, #0x3c]
	ldr r0, [sp, #0x18]
	ldr r2, [r0, #8]
	ldr r0, [sp, #0x28]
	cmp r0, #1
	add r0, r2, #1
	str r0, [sp, #0x40]
	ldr r0, [sp, #0x1c]
	moveq r5, #0
	ldr r2, [r0]
	ldr r0, [r2, r1, lsl #2]
	str r0, [sp, #0x24]
	ldrne r0, [sp, #0x28]
	subne r0, r0, #2
	ldrne r5, [r2, r0, lsl #2]
	ldr r0, [sp, #0x18]
	ldr r2, [r8, #8]
	ldr r1, [r0]
	sub r0, r4, #1
	add r7, r1, r0, lsl #2
	ldr r0, [sp, #0x10]
	add r1, r0, #1
	cmp r1, r2
	movle r0, r8
	ble _02239468
	mov r0, r8
	bl bn_expand2
_02239468:
	cmp r0, #0
	beq _022397CC
	ldr r0, [sp, #4]
	ldr r1, [r6, #0xc]
	ldr r3, [r0, #0xc]
	ldr r0, [sp, #0x10]
	sub r2, r0, #1
	eor r0, r3, r1
	str r0, [r8, #0xc]
	ldr r0, [sp, #0x10]
	str r0, [r8, #4]
	ldr r0, [sp, #0x28]
	ldr r3, [r8]
	add r1, r0, #1
	add r0, r3, r2, lsl #2
	str r0, [sp, #0x20]
	ldr r0, [sp, #0x14]
	ldr r0, [r0, #8]
	cmp r1, r0
	ldrle r0, [sp, #0x14]
	ble _022394C4
	ldr r0, [sp, #0x14]
	bl bn_expand2
_022394C4:
	cmp r0, #0
	beq _022397CC
	ldr r1, [sp, #0x1c]
	add r0, sp, #0x38
	bl BN_ucmp
	cmp r0, #0
	blt _02239518
	add r0, sp, #0x38
	ldr r2, [sp, #0x1c]
	mov r1, r0
	bl BN_usub
	cmp r0, #0
	beq _022397CC
	ldr r0, [sp, #0x20]
	mov r2, #1
	str r2, [r0]
	ldr r0, [r8, #4]
	ldr r1, [r8]
	sub r0, r0, #1
	str r2, [r1, r0, lsl #2]
	b _02239524
_02239518:
	ldr r0, [r8, #4]
	sub r0, r0, #1
	str r0, [r8, #4]
_02239524:
	ldr r0, [sp, #0x10]
	sub r1, r0, #1
	ldr r0, [sp, #0x20]
	cmp r1, #0
	sub r0, r0, #4
	str r0, [sp, #0x20]
	mov r0, #0
	str r0, [sp, #0xc]
	ble _02239774
	ldr r8, _022397DC ; =0x0000FFFF
	cmp r1, #0
	and r0, r5, r8
	str r0, [sp, #0x2c]
	and r0, r8, r5, lsr #16
	str r0, [sp, #0x30]
	ldr r0, [sp, #0x24]
	and r4, r0, r8
	ble _02239774
	and sb, r8, r0, lsr #16
	mvn r0, #0
	str r0, [sp, #0x34]
_02239578:
	ldr r1, [sp, #0x38]
	ldr r0, [sp, #0x3c]
	sub r1, r1, #4
	add r0, r0, #1
	str r0, [sp, #0x3c]
	str r1, [sp, #0x38]
	ldr fp, [r7]
	ldr r0, [sp, #0x24]
	ldr r5, [r7, #-4]
	cmp fp, r0
	ldreq r6, [sp, #0x34]
	beq _022395BC
	ldr r2, [sp, #0x24]
	mov r0, fp
	mov r1, r5
	bl bn_div_words
	mov r6, r0
_022395BC:
	ldr r0, [sp, #0x2c]
	and r2, r6, r8
	mul ip, r0, r2
	ldr r0, [sp, #0x30]
	and lr, r8, r6, lsr #16
	ldr r1, [sp, #0x30]
	mul r0, r2, r0
	mul r3, r1, lr
	ldr r1, [sp, #0x2c]
	mul sl, sb, lr
	mla r1, lr, r1, r0
	cmp r1, r0
	addlo r3, r3, #0x10000
	and r0, r8, r1, lsr #16
	add r3, r3, r0
	and r0, r1, r8
	add ip, ip, r0, lsl #16
	cmp ip, r0, lsl #16
	mul r0, r2, sb
	mul r1, r4, r2
	mla r2, lr, r4, r0
	addlo r3, r3, #1
	cmp r2, r0
	addlo sl, sl, #0x10000
	and r0, r8, r2, lsr #16
	add sl, sl, r0
	and r0, r2, r8
	add r1, r1, r0, lsl #16
	cmp r1, r0, lsl #16
	addlo sl, sl, #1
	sub r1, r5, r1
	cmp r1, r5
	addhi sl, sl, #1
	subs r0, fp, sl
	bne _0223966C
	cmp r3, r1
	blo _0223966C
	cmp r3, r1
	bne _02239664
	ldr r0, [r7, #-8]
	cmp ip, r0
	bls _0223966C
_02239664:
	sub r6, r6, #1
	b _022395BC
_0223966C:
	ldr r0, [sp, #0x14]
	ldr r1, [sp, #0x1c]
	ldr r0, [r0]
	ldr r1, [r1]
	ldr r2, [sp, #0x28]
	mov r3, r6
	bl bn_mul_words
	ldr r1, [sp, #0x14]
	ldr r2, [sp, #0x28]
	ldr r3, [r1]
	ldr r1, [sp, #0x28]
	str r0, [r3, r2, lsl #2]
	add r1, r1, #1
	cmp r1, #0
	ble _022396CC
	ldr r0, [sp, #0x14]
	ldr r2, [r0]
_022396B0:
	sub r0, r1, #1
	ldr r0, [r2, r0, lsl #2]
	cmp r0, #0
	bne _022396CC
	sub r1, r1, #1
	cmp r1, #0
	bgt _022396B0
_022396CC:
	ldr r0, [sp, #0x14]
	ldr r2, [sp, #0x14]
	str r1, [r0, #4]
	add r0, sp, #0x38
	mov r1, r0
	ldr r5, [sp, #0x3c]
	bl BN_sub
	ldr r0, [sp, #0x18]
	ldr r1, [r0, #4]
	ldr r0, [sp, #0x3c]
	add r0, r1, r0
	sub r1, r0, r5
	ldr r0, [sp, #0x18]
	str r1, [r0, #4]
	ldr r0, [sp, #0x44]
	cmp r0, #0
	beq _02239744
	add r0, sp, #0x38
	ldr r2, [sp, #0x1c]
	mov r1, r0
	sub r6, r6, #1
	ldr r5, [sp, #0x3c]
	bl BN_add
	ldr r0, [sp, #0x18]
	ldr r1, [r0, #4]
	ldr r0, [sp, #0x3c]
	sub r0, r0, r5
	add r1, r1, r0
	ldr r0, [sp, #0x18]
	str r1, [r0, #4]
_02239744:
	ldr r0, [sp, #0x20]
	sub r7, r7, #4
	str r6, [r0], #-4
	str r0, [sp, #0x20]
	ldr r0, [sp, #0xc]
	add r0, r0, #1
	str r0, [sp, #0xc]
	ldr r0, [sp, #0x10]
	sub r1, r0, #1
	ldr r0, [sp, #0xc]
	cmp r0, r1
	blt _02239578
_02239774:
	ldr r0, [sp, #0x18]
	bl bn_fix_top
	ldr r0, [sp]
	cmp r0, #0
	beq _022397BC
	ldr r2, [sp, #8]
	ldr r3, [sp, #4]
	ldr r1, [sp, #0x18]
	add r2, r2, #0x20
	ldr r4, [r3, #0xc]
	bl BN_rshift
	cmp r0, #0
	addeq sp, sp, #0x4c
	moveq r0, #0
	ldmeqia sp!, {r4, r5, r6, r7, r8, sb, sl, fp, lr}
	bxeq lr
	ldr r0, [sp]
	str r4, [r0, #0xc]
_022397BC:
	add sp, sp, #0x4c
	mov r0, #1
	ldmia sp!, {r4, r5, r6, r7, r8, sb, sl, fp, lr}
	bx lr
_022397CC:
	mov r0, #0
	add sp, sp, #0x4c
	ldmia sp!, {r4, r5, r6, r7, r8, sb, sl, fp, lr}
	bx lr
	.balign 4, 0
_022397DC: .word 0x0000FFFF
	arm_func_end BN_div

	arm_func_start bn_mul_normal
bn_mul_normal: ; 0x022397E0
	stmdb sp!, {r4, r5, r6, r7, r8, sb, lr}
	sub sp, sp, #4
	ldr r5, [sp, #0x20]
	mov r7, r2
	mov sb, r0
	mov r8, r1
	mov r6, r3
	cmp r7, r5
	bge _0223981C
	mov r1, r7
	mov r0, r8
	mov r7, r5
	mov r8, r6
	mov r5, r1
	mov r6, r0
_0223981C:
	ldr r3, [r6]
	mov r0, sb
	mov r1, r8
	mov r2, r7
	add r4, sb, r7, lsl #2
	bl bn_mul_words
	str r0, [sb, r7, lsl #2]
_02239838:
	sub r0, r5, #1
	cmp r0, #0
	addle sp, sp, #4
	ldmleia sp!, {r4, r5, r6, r7, r8, sb, lr}
	bxle lr
	ldr r3, [r6, #4]
	mov r1, r8
	mov r2, r7
	add r0, sb, #4
	bl bn_mul_add_words
	sub r1, r5, #2
	cmp r1, #0
	addle sp, sp, #4
	str r0, [r4, #4]
	ldmleia sp!, {r4, r5, r6, r7, r8, sb, lr}
	bxle lr
	ldr r3, [r6, #8]
	mov r1, r8
	mov r2, r7
	add r0, sb, #8
	bl bn_mul_add_words
	sub r1, r5, #3
	cmp r1, #0
	addle sp, sp, #4
	str r0, [r4, #8]
	ldmleia sp!, {r4, r5, r6, r7, r8, sb, lr}
	bxle lr
	ldr r3, [r6, #0xc]
	mov r1, r8
	mov r2, r7
	add r0, sb, #0xc
	bl bn_mul_add_words
	sub r5, r5, #4
	cmp r5, #0
	addle sp, sp, #4
	str r0, [r4, #0xc]
	ldmleia sp!, {r4, r5, r6, r7, r8, sb, lr}
	bxle lr
	ldr r3, [r6, #0x10]
	mov r1, r8
	mov r2, r7
	add r0, sb, #0x10
	bl bn_mul_add_words
	str r0, [r4, #0x10]
	add r4, r4, #0x10
	add sb, sb, #0x10
	add r6, r6, #0x10
	b _02239838
_022398F8:
	add sp, sp, #4
	ldmia sp!, {r4, r5, r6, r7, r8, sb, lr}
	bx lr
	arm_func_end bn_mul_normal

	arm_func_start BN_mul
BN_mul: ; 0x02239904
	stmdb sp!, {r4, r5, r6, r7, r8, sb, sl, fp, lr}
	sub sp, sp, #4
	mov sb, r1
	ldr r6, [sb, #4]
	mov r8, r2
	mov sl, r0
	cmp r6, #0
	ldr r5, [r8, #4]
	beq _02239930
	cmp r5, #0
	bne _0223994C
_02239930:
	mov r0, sl
	mov r1, #0
	bl BN_set_word
	add sp, sp, #4
	mov r0, #1
	ldmia sp!, {r4, r5, r6, r7, r8, sb, sl, fp, lr}
	bx lr
_0223994C:
	ldr r1, [sb, #0xc]
	ldr r0, [r8, #0xc]
	cmp sl, sb
	add r7, r6, r5
	eor fp, r1, r0
	beq _0223996C
	cmp sl, r8
	bne _02239984
_0223996C:
	ldr r0, [r3]
	add r2, r3, #4
	add r1, r0, #1
	mov r0, #0x14
	mla r4, r1, r0, r2
	b _02239988
_02239984:
	mov r4, sl
_02239988:
	ldr r0, [r4, #8]
	cmp r7, r0
	movle r0, r4
	ble _022399A4
	mov r0, r4
	mov r1, r7
	bl bn_expand2
_022399A4:
	cmp r0, #0
	addeq sp, sp, #4
	moveq r0, #0
	ldmeqia sp!, {r4, r5, r6, r7, r8, sb, sl, fp, lr}
	bxeq lr
	str r7, [r4, #4]
	str r5, [sp]
	ldr r0, [r4]
	ldr r1, [sb]
	ldr r3, [r8]
	mov r2, r6
	bl bn_mul_normal
	mov r0, r4
	str fp, [sl, #0xc]
	bl bn_fix_top
	cmp sl, r4
	beq _022399F4
	mov r0, sl
	mov r1, r4
	bl BN_copy
_022399F4:
	mov r0, #1
	add sp, sp, #4
	ldmia sp!, {r4, r5, r6, r7, r8, sb, sl, fp, lr}
	bx lr
	arm_func_end BN_mul

	arm_func_start BN_sub
BN_sub: ; 0x02239A04
	stmdb sp!, {r4, r5, r6, r7, lr}
	sub sp, sp, #4
	mov r6, r1
	ldr r1, [r6, #0xc]
	mov r3, #0
	mov r7, r0
	mov r5, r2
	mov r4, r3
	cmp r1, #0
	beq _02239A4C
	ldr r0, [r5, #0xc]
	cmp r0, #0
	movne r0, r6
	moveq r3, #1
	movne r6, r5
	movne r5, r0
	moveq r4, r3
	b _02239A58
_02239A4C:
	ldr r0, [r5, #0xc]
	cmp r0, #0
	movne r3, #1
_02239A58:
	cmp r3, #0
	beq _02239A8C
	mov r0, r7
	mov r1, r6
	mov r2, r5
	bl BN_uadd
	cmp r0, #0
	moveq r0, #0
	add sp, sp, #4
	strne r4, [r7, #0xc]
	movne r0, #1
	ldmia sp!, {r4, r5, r6, r7, lr}
	bx lr
_02239A8C:
	ldr r0, [r5, #4]
	ldr r1, [r6, #4]
	cmp r1, r0
	movle r1, r0
	ldr r0, [r7, #8]
	cmp r1, r0
	movle r0, r7
	ble _02239AB4
	mov r0, r7
	bl bn_expand2
_02239AB4:
	cmp r0, #0
	addeq sp, sp, #4
	moveq r0, #0
	ldmeqia sp!, {r4, r5, r6, r7, lr}
	bxeq lr
	mov r0, r6
	mov r1, r5
	bl BN_ucmp
	cmp r0, #0
	bge _02239B0C
	mov r0, r7
	mov r1, r5
	mov r2, r6
	bl BN_usub
	cmp r0, #0
	addeq sp, sp, #4
	moveq r0, #0
	ldmeqia sp!, {r4, r5, r6, r7, lr}
	bxeq lr
	mov r0, #1
	str r0, [r7, #0xc]
	b _02239B38
_02239B0C:
	mov r0, r7
	mov r1, r6
	mov r2, r5
	bl BN_usub
	cmp r0, #0
	addeq sp, sp, #4
	moveq r0, #0
	ldmeqia sp!, {r4, r5, r6, r7, lr}
	bxeq lr
	mov r0, #0
	str r0, [r7, #0xc]
_02239B38:
	mov r0, #1
	add sp, sp, #4
	ldmia sp!, {r4, r5, r6, r7, lr}
	bx lr
	arm_func_end BN_sub

	arm_func_start BN_usub
BN_usub: ; 0x02239B48
	stmdb sp!, {r4, r5, r6, r7, r8, sb, sl, lr}
	mov r8, r1
	mov r7, r2
	ldr r5, [r7, #4]
	ldr r6, [r8, #4]
	mov r4, r0
	cmp r6, r5
	movlt r0, #0
	ldmltia sp!, {r4, r5, r6, r7, r8, sb, sl, lr}
	bxlt lr
	ldr r1, [r4, #8]
	cmp r6, r1
	ble _02239B84
	mov r1, r6
	bl bn_expand2
_02239B84:
	cmp r0, #0
	moveq r0, #0
	ldmeqia sp!, {r4, r5, r6, r7, r8, sb, sl, lr}
	bxeq lr
	mov r3, #0
	mov r0, r3
	cmp r5, #0
	ldr r2, [r8]
	ldr sb, [r7]
	ldr r1, [r4]
	ble _02239C04
	mov ip, r3
	mov r7, r3
	mov lr, #1
_02239BBC:
	cmp r3, #0
	ldr sl, [r2], #4
	ldr r8, [sb], #4
	beq _02239BE4
	cmp sl, r8
	movls r3, lr
	sub r8, sl, r8
	movhi r3, ip
	sub sl, r8, #1
	b _02239BF4
_02239BE4:
	cmp sl, r8
	movlo r3, lr
	movhs r3, r7
	sub sl, sl, r8
_02239BF4:
	add r0, r0, #1
	cmp r0, r5
	str sl, [r1], #4
	blt _02239BBC
_02239C04:
	cmp r3, #0
	beq _02239C34
	cmp r0, r6
	bge _02239C34
_02239C14:
	ldr r5, [r2], #4
	add r0, r0, #1
	sub r3, r5, #1
	cmp r5, r3
	str r3, [r1], #4
	bhi _02239C34
	cmp r0, r6
	blt _02239C14
_02239C34:
	cmp r1, r2
	beq _02239C94
_02239C3C:
	cmp r0, r6
	bge _02239C94
	ldr r5, [r2]
	add r3, r0, #1
	str r5, [r1]
	cmp r3, r6
	bge _02239C94
	ldr r5, [r2, #4]
	add r3, r0, #2
	str r5, [r1, #4]
	cmp r3, r6
	bge _02239C94
	ldr r5, [r2, #8]
	add r3, r0, #3
	str r5, [r1, #8]
	cmp r3, r6
	ldrlt r3, [r2, #0xc]
	add r0, r0, #4
	strlt r3, [r1, #0xc]
	addlt r2, r2, #0x10
	addlt r1, r1, #0x10
	blt _02239C3C
_02239C94:
	mov r0, r4
	str r6, [r4, #4]
	bl bn_fix_top
	mov r0, #1
	ldmia sp!, {r4, r5, r6, r7, r8, sb, sl, lr}
	bx lr
	arm_func_end BN_usub

	arm_func_start BN_uadd
BN_uadd: ; 0x02239CAC
	stmdb sp!, {r4, r5, r6, r7, r8, sb, lr}
	sub sp, sp, #4
	mov r4, r1
	mov r8, r2
	ldr r2, [r4, #4]
	ldr r1, [r8, #4]
	mov sb, r0
	cmp r2, r1
	movlt r0, r4
	movlt r4, r8
	movlt r8, r0
	ldr r6, [r4, #4]
	ldr r0, [sb, #8]
	add r1, r6, #1
	cmp r1, r0
	ldr r7, [r8, #4]
	movle r0, sb
	ble _02239CFC
	mov r0, sb
	bl bn_expand2
_02239CFC:
	cmp r0, #0
	addeq sp, sp, #4
	moveq r0, #0
	ldmeqia sp!, {r4, r5, r6, r7, r8, sb, lr}
	bxeq lr
	str r6, [sb, #4]
	ldr r5, [r4]
	ldr r4, [sb]
	ldr r2, [r8]
	mov r0, r4
	mov r1, r5
	mov r3, r7
	bl bn_add_words
	cmp r0, #0
	add r4, r4, r7, lsl #2
	add r5, r5, r7, lsl #2
	beq _02239D94
	cmp r7, r6
	bge _02239D74
_02239D48:
	ldr r2, [r5], #4
	mov r3, r4
	add r1, r2, #1
	str r1, [r4], #4
	ldr r1, [r3]
	add r7, r7, #1
	cmp r1, r2
	movhs r0, #0
	bhs _02239D74
	cmp r7, r6
	blt _02239D48
_02239D74:
	cmp r7, r6
	blt _02239D94
	cmp r0, #0
	movne r0, #1
	strne r0, [r4], #4
	ldrne r0, [sb, #4]
	addne r0, r0, #1
	strne r0, [sb, #4]
_02239D94:
	cmp r4, r5
	beq _02239DB8
	cmp r7, r6
	bge _02239DB8
_02239DA4:
	ldr r0, [r5], #4
	add r7, r7, #1
	cmp r7, r6
	str r0, [r4], #4
	blt _02239DA4
_02239DB8:
	mov r0, #1
	add sp, sp, #4
	ldmia sp!, {r4, r5, r6, r7, r8, sb, lr}
	bx lr
	arm_func_end BN_uadd

	arm_func_start BN_add
BN_add: ; 0x02239DC8
	stmdb sp!, {r4, r5, r6, lr}
	mov r5, r1
	mov r4, r2
	ldr r2, [r5, #0xc]
	ldr r1, [r4, #0xc]
	mov r6, r0
	eors r0, r2, r1
	beq _02239E6C
	cmp r2, #0
	movne r0, r5
	movne r5, r4
	movne r4, r0
	mov r0, r5
	mov r1, r4
	bl BN_ucmp
	cmp r0, #0
	bge _02239E38
	mov r0, r6
	mov r1, r4
	mov r2, r5
	bl BN_usub
	cmp r0, #0
	moveq r0, #0
	ldmeqia sp!, {r4, r5, r6, lr}
	bxeq lr
	mov r0, #1
	str r0, [r6, #0xc]
	b _02239E60
_02239E38:
	mov r0, r6
	mov r1, r5
	mov r2, r4
	bl BN_usub
	cmp r0, #0
	moveq r0, #0
	ldmeqia sp!, {r4, r5, r6, lr}
	bxeq lr
	mov r0, #0
	str r0, [r6, #0xc]
_02239E60:
	mov r0, #1
	ldmia sp!, {r4, r5, r6, lr}
	bx lr
_02239E6C:
	cmp r2, #0
	movne r0, #1
	strne r0, [r6, #0xc]
	moveq r0, #0
	streq r0, [r6, #0xc]
	mov r0, r6
	mov r1, r5
	mov r2, r4
	bl BN_uadd
	cmp r0, #0
	moveq r0, #0
	movne r0, #1
	ldmia sp!, {r4, r5, r6, lr}
	bx lr
	arm_func_end BN_add

	arm_func_start BN_sub_word
BN_sub_word: ; 0x02239EA4
	stmdb sp!, {r4, r5, lr}
	sub sp, sp, #4
	movs r4, r1
	mov r5, r0
	addeq sp, sp, #4
	moveq r0, #1
	ldmeqia sp!, {r4, r5, lr}
	bxeq lr
	ldr r2, [r5, #0xc]
	cmp r2, #0
	beq _02239EF0
	mov r2, #0
	str r2, [r5, #0xc]
	bl BN_add_word
	mov r1, #1
	add sp, sp, #4
	str r1, [r5, #0xc]
	ldmia sp!, {r4, r5, lr}
	bx lr
_02239EF0:
	ldr r1, [r5, #4]
	cmp r1, #1
	bgt _02239F94
	cmp r1, #0
	bne _02239F4C
	ldr r1, [r5, #8]
	cmp r1, #1
	bge _02239F18
	mov r1, #1
	bl bn_expand2
_02239F18:
	cmp r0, #0
	addeq sp, sp, #4
	moveq r0, #0
	ldmeqia sp!, {r4, r5, lr}
	bxeq lr
	ldr r1, [r5]
	mov r0, #1
	str r4, [r1]
	str r0, [r5, #0xc]
	add sp, sp, #4
	str r0, [r5, #4]
	ldmia sp!, {r4, r5, lr}
	bx lr
_02239F4C:
	ldr r2, [r5]
	ldr r1, [r2]
	cmp r1, r4
	moveq r0, #0
	streq r0, [r5, #4]
	beq _02239F84
	cmp r1, r4
	subhi r0, r1, r4
	strhi r0, [r2]
	movls r0, #1
	strls r0, [r5, #0xc]
	ldrls r0, [r5]
	subls r1, r4, r1
	strls r1, [r0]
_02239F84:
	add sp, sp, #4
	mov r0, #1
	ldmia sp!, {r4, r5, lr}
	bx lr
_02239F94:
	mov r0, #0
	mov r1, #1
_02239F9C:
	ldr ip, [r5]
	mov r3, r0, lsl #2
	ldr r2, [ip, r0, lsl #2]
	cmp r2, r4
	ldrhs r1, [ip, r3]
	subhs r1, r1, r4
	strhs r1, [ip, r3]
	bhs _02239FD4
	ldr r2, [ip, r3]
	add r0, r0, #1
	sub r2, r2, r4
	mov r4, r1
	str r2, [ip, r3]
	b _02239F9C
_02239FD4:
	ldr r1, [r5]
	ldr r1, [r1, r3]
	cmp r1, #0
	bne _02239FF4
	ldr r1, [r5, #4]
	sub r1, r1, #1
	cmp r0, r1
	streq r1, [r5, #4]
_02239FF4:
	mov r0, #1
	add sp, sp, #4
	ldmia sp!, {r4, r5, lr}
	bx lr
	arm_func_end BN_sub_word

	arm_func_start BN_add_word
BN_add_word: ; 0x0223A004
	stmdb sp!, {r4, r5, lr}
	sub sp, sp, #4
	movs r4, r1
	mov r5, r0
	addeq sp, sp, #4
	moveq r0, #1
	ldmeqia sp!, {r4, r5, lr}
	bxeq lr
	ldr r2, [r5, #0xc]
	cmp r2, #0
	beq _0223A0AC
	ldr r2, [r5, #4]
	cmp r2, #1
	ble _0223A05C
	mov r2, #0
	str r2, [r5, #0xc]
	bl BN_sub_word
	mov r1, #1
	add sp, sp, #4
	str r1, [r5, #0xc]
	ldmia sp!, {r4, r5, lr}
	bx lr
_0223A05C:
	ldr r2, [r5]
	ldr r1, [r2]
	cmp r1, r4
	subhi r0, r1, r4
	strhi r0, [r2]
	bhi _0223A09C
	cmp r1, r4
	movhs r0, #0
	strhs r0, [r5, #0xc]
	strhs r0, [r5, #4]
	bhs _0223A09C
	mov r0, #0
	str r0, [r5, #0xc]
	ldr r0, [r5]
	sub r1, r4, r1
	str r1, [r0]
_0223A09C:
	add sp, sp, #4
	mov r0, #1
	ldmia sp!, {r4, r5, lr}
	bx lr
_0223A0AC:
	ldr r1, [r5, #4]
	ldr r2, [r5, #8]
	add r1, r1, #1
	cmp r1, r2
	ble _0223A0C4
	bl bn_expand2
_0223A0C4:
	cmp r0, #0
	addeq sp, sp, #4
	moveq r0, #0
	ldmeqia sp!, {r4, r5, lr}
	bxeq lr
	ldr r1, [r5]
	ldr r0, [r5, #4]
	mov r3, #0
	str r3, [r1, r0, lsl #2]
	mov r0, #1
_0223A0EC:
	ldr r2, [r5]
	ldr r1, [r2, r3, lsl #2]
	add r1, r4, r1
	cmp r4, r1
	str r1, [r2, r3, lsl #2]
	movhi r4, r0
	addhi r3, r3, #1
	bhi _0223A0EC
	ldr r0, [r5, #4]
	cmp r3, r0
	addge r0, r0, #1
	strge r0, [r5, #4]
	mov r0, #1
	add sp, sp, #4
	ldmia sp!, {r4, r5, lr}
	bx lr
	arm_func_end BN_add_word

	arm_func_start BN_rshift
BN_rshift: ; 0x0223A12C
	stmdb sp!, {r4, r5, r6, r7, r8, lr}
	mov r7, r1
	mov r1, r2, asr #4
	mov r3, r2, lsr #0x1f
	add r1, r2, r1, lsr #27
	rsb r2, r3, r2, lsl #27
	add r4, r3, r2, ror #27
	ldr r2, [r7, #4]
	mov r6, r1, asr #5
	mov r8, r0
	cmp r6, r2
	rsb r5, r4, #0x20
	ble _0223A174
	mov r1, #0
	bl BN_set_word
	mov r0, #1
	ldmia sp!, {r4, r5, r6, r7, r8, lr}
	bx lr
_0223A174:
	cmp r8, r7
	beq _0223A1AC
	sub r1, r2, r6
	ldr r2, [r8, #8]
	add r1, r1, #2
	cmp r1, r2
	ble _0223A194
	bl bn_expand2
_0223A194:
	cmp r0, #0
	moveq r0, #0
	ldmeqia sp!, {r4, r5, r6, r7, r8, lr}
	bxeq lr
	ldr r0, [r7, #0xc]
	str r0, [r8, #0xc]
_0223A1AC:
	ldr r0, [r7]
	ldr r1, [r7, #4]
	add r2, r0, r6, lsl #2
	ldr r0, [r8]
	sub r7, r1, r6
	mov r6, r2
	str r7, [r8, #4]
	cmp r4, #0
	bne _0223A1F4
	add r2, r7, #1
	cmp r2, #0
	ble _0223A234
_0223A1DC:
	ldr r1, [r6], #4
	sub r2, r2, #1
	cmp r2, #0
	str r1, [r0], #4
	bgt _0223A1DC
	b _0223A234
_0223A1F4:
	cmp r7, #1
	add r6, r2, #4
	ldr r3, [r2]
	mov r2, #1
	ble _0223A224
_0223A208:
	mov r1, r3, lsr r4
	ldr r3, [r6], #4
	add r2, r2, #1
	orr r1, r1, r3, lsl r5
	cmp r2, r7
	str r1, [r0], #4
	blt _0223A208
_0223A224:
	mov r1, r3, lsr r4
	str r1, [r0]
	mov r1, #0
	str r1, [r0, #4]
_0223A234:
	mov r0, r8
	bl bn_fix_top
	mov r0, #1
	ldmia sp!, {r4, r5, r6, r7, r8, lr}
	bx lr
	arm_func_end BN_rshift

	arm_func_start BN_lshift
BN_lshift: ; 0x0223A248
	stmdb sp!, {r4, r5, r6, r7, r8, sb, sl, lr}
	mov r4, r1
	mov r7, r2
	mov r1, r7, asr #4
	mov r5, r0
	add r2, r7, r1, lsr #27
	ldr r1, [r4, #4]
	ldr r3, [r5, #8]
	add r1, r1, r2, asr #5
	add r1, r1, #1
	cmp r1, r3
	mov r6, r2, asr #5
	ble _0223A280
	bl bn_expand2
_0223A280:
	cmp r0, #0
	moveq r0, #0
	ldmeqia sp!, {r4, r5, r6, r7, r8, sb, sl, lr}
	bxeq lr
	ldr r0, [r4, #0xc]
	mov r1, r7, lsr #0x1f
	str r0, [r5, #0xc]
	ldr r3, [r4, #4]
	rsb r0, r1, r7, lsl #27
	adds ip, r1, r0, ror #27
	ldr r2, [r4]
	ldr r0, [r5]
	add r1, r3, r6
	mov r3, #0
	str r3, [r0, r1, lsl #2]
	rsb r3, ip, #0x20
	bne _0223A2E8
	ldr r1, [r4, #4]
	subs r7, r1, #1
	bmi _0223A31C
_0223A2D0:
	ldr r3, [r2, r7, lsl #2]
	add r1, r6, r7
	str r3, [r0, r1, lsl #2]
	subs r7, r7, #1
	bpl _0223A2D0
	b _0223A31C
_0223A2E8:
	ldr r1, [r4, #4]
	subs r1, r1, #1
	bmi _0223A31C
_0223A2F4:
	add sb, r6, r1
	add r8, sb, #1
	ldr sl, [r2, r1, lsl #2]
	ldr lr, [r0, r8, lsl #2]
	mov r7, sl, lsl ip
	orr lr, lr, sl, lsr r3
	str lr, [r0, r8, lsl #2]
	str r7, [r0, sb, lsl #2]
	subs r1, r1, #1
	bpl _0223A2F4
_0223A31C:
	mov r2, r6, lsl #2
	mov r1, #0
	bl MI_CpuFill8
	ldr r1, [r4, #4]
	mov r0, r5
	add r1, r1, r6
	add r1, r1, #1
	str r1, [r5, #4]
	bl bn_fix_top
	mov r0, #1
	ldmia sp!, {r4, r5, r6, r7, r8, sb, sl, lr}
	bx lr
	arm_func_end BN_lshift

	arm_func_start bn_sub_words
bn_sub_words: ; 0x0223A34C
	stmdb sp!, {r4, r5, r6, r7, r8, sb, sl, lr}
	cmp r3, #0
	movle r0, #0
	ldmleia sp!, {r4, r5, r6, r7, r8, sb, sl, lr}
	bxle lr
	mov r6, #0
	mov r4, r6
	mov lr, r6
	mov ip, r6
	mov r7, r6
	mov r5, #1
_0223A378:
	ldr sl, [r1]
	ldr sb, [r2]
	sub r8, sl, sb
	sub r8, r8, r6
	str r8, [r0]
	cmp sl, sb
	beq _0223A3A0
	cmp sl, sb
	movlo r6, r5
	movhs r6, r4
_0223A3A0:
	sub r8, r3, #1
	cmp r8, #0
	ble _0223A454
	ldr sl, [r1, #4]
	ldr sb, [r2, #4]
	sub r8, sl, sb
	sub r8, r8, r6
	str r8, [r0, #4]
	cmp sl, sb
	beq _0223A3D4
	cmp sl, sb
	movlo r6, r5
	movhs r6, lr
_0223A3D4:
	sub r8, r3, #2
	cmp r8, #0
	ble _0223A454
	ldr sl, [r1, #8]
	ldr sb, [r2, #8]
	sub r8, sl, sb
	sub r8, r8, r6
	str r8, [r0, #8]
	cmp sl, sb
	beq _0223A408
	cmp sl, sb
	movlo r6, r5
	movhs r6, ip
_0223A408:
	sub r8, r3, #3
	cmp r8, #0
	ble _0223A454
	ldr sl, [r1, #0xc]
	ldr sb, [r2, #0xc]
	sub r8, sl, sb
	sub r8, r8, r6
	str r8, [r0, #0xc]
	cmp sl, sb
	beq _0223A43C
	cmp sl, sb
	movlo r6, r5
	movhs r6, r7
_0223A43C:
	sub r3, r3, #4
	cmp r3, #0
	addgt r1, r1, #0x10
	addgt r2, r2, #0x10
	addgt r0, r0, #0x10
	bgt _0223A378
_0223A454:
	mov r0, r6
	ldmia sp!, {r4, r5, r6, r7, r8, sb, sl, lr}
	bx lr
	arm_func_end bn_sub_words

	arm_func_start bn_add_words
bn_add_words: ; 0x0223A460
	stmdb sp!, {r4, r5, r6, r7, r8, sb, sl, fp, lr}
	sub sp, sp, #0xc
	cmp r3, #0
	addle sp, sp, #0xc
	movle r0, #0
	ldmleia sp!, {r4, r5, r6, r7, r8, sb, sl, fp, lr}
	bxle lr
	mov ip, #0
	mov r7, ip
	mov r6, ip
	mov r5, ip
	mov r4, ip
	mov lr, ip
	mov fp, ip
	str ip, [sp]
	str ip, [sp, #4]
	mov r8, #1
_0223A4A4:
	ldr sb, [r1]
	add sl, sb, ip
	ldr sb, [r2]
	cmp sl, ip
	movlo ip, r8
	add sb, sl, sb
	movhs ip, r7
	cmp sb, sl
	movlo sl, r8
	movhs sl, r6
	str sb, [r0]
	sub sb, r3, #1
	add ip, ip, sl
	cmp sb, #0
	ble _0223A5A0
	ldr sb, [r1, #4]
	ldr sl, [r2, #4]
	add sb, sb, ip
	cmp sb, ip
	movlo ip, r8
	add sl, sb, sl
	movhs ip, r5
	cmp sl, sb
	movlo sb, r8
	movhs sb, r4
	add ip, ip, sb
	sub sb, r3, #2
	str sl, [r0, #4]
	cmp sb, #0
	ble _0223A5A0
	ldr sb, [r1, #8]
	ldr sl, [r2, #8]
	add sb, sb, ip
	cmp sb, ip
	movlo ip, r8
	add sl, sb, sl
	movhs ip, lr
	cmp sl, sb
	movlo sb, r8
	movhs sb, fp
	add ip, ip, sb
	sub sb, r3, #3
	str sl, [r0, #8]
	cmp sb, #0
	ble _0223A5A0
	ldr sb, [r1, #0xc]
	ldr sl, [r2, #0xc]
	add sb, sb, ip
	cmp sb, ip
	movlo ip, r8
	add sl, sb, sl
	ldrhs ip, [sp]
	cmp sl, sb
	movlo sb, r8
	ldrhs sb, [sp, #4]
	sub r3, r3, #4
	cmp r3, #0
	str sl, [r0, #0xc]
	add ip, ip, sb
	addgt r1, r1, #0x10
	addgt r2, r2, #0x10
	addgt r0, r0, #0x10
	bgt _0223A4A4
_0223A5A0:
	mov r0, ip
	add sp, sp, #0xc
	ldmia sp!, {r4, r5, r6, r7, r8, sb, sl, fp, lr}
	bx lr
	arm_func_end bn_add_words

	arm_func_start bn_sqr_words
bn_sqr_words: ; 0x0223A5B0
	stmdb sp!, {r4, r5, r6, r7, r8, lr}
	cmp r2, #0
	ldmleia sp!, {r4, r5, r6, r7, r8, lr}
	bxle lr
	mov r3, #0x8000
	ldr lr, _0223A700 ; =0x0000FFFF
	rsb ip, r3, #0
_0223A5CC:
	ldr r5, [r1]
	mov r3, #0x8000
	and r4, r5, lr
	and r7, lr, r5, lsr #16
	mul r6, r4, r7
	mul r5, r4, r4
	and r4, r6, lr
	add r8, r5, r4, lsl #17
	rsb r3, r3, #0
	mul r5, r7, r7
	and r3, r6, r3
	add r3, r5, r3, lsr #15
	cmp r8, r4, lsl #17
	addlo r3, r3, #1
	str r8, [r0]
	str r3, [r0, #4]
	subs r3, r2, #1
	ldmeqia sp!, {r4, r5, r6, r7, r8, lr}
	bxeq lr
	ldr r4, [r1, #4]
	and r7, lr, r4, lsr #16
	and r3, r4, lr
	mul r6, r3, r7
	mul r4, r3, r3
	and r3, r6, lr
	add r8, r4, r3, lsl #17
	mul r5, r7, r7
	and r4, r6, ip
	cmp r8, r3, lsl #17
	add r3, r5, r4, lsr #15
	addlo r3, r3, #1
	str r8, [r0, #8]
	str r3, [r0, #0xc]
	subs r3, r2, #2
	ldmeqia sp!, {r4, r5, r6, r7, r8, lr}
	bxeq lr
	ldr r5, [r1, #8]
	mov r3, #0x8000
	and r4, r5, lr
	and r7, lr, r5, lsr #16
	mul r6, r4, r7
	mul r5, r4, r4
	and r4, r6, lr
	add r8, r5, r4, lsl #17
	rsb r3, r3, #0
	mul r5, r7, r7
	and r3, r6, r3
	add r3, r5, r3, lsr #15
	cmp r8, r4, lsl #17
	addlo r3, r3, #1
	str r8, [r0, #0x10]
	str r3, [r0, #0x14]
	subs r3, r2, #3
	ldmeqia sp!, {r4, r5, r6, r7, r8, lr}
	bxeq lr
	ldr r5, [r1, #0xc]
	mov r3, #0x8000
	and r4, r5, lr
	and r7, lr, r5, lsr #16
	mul r6, r4, r7
	mul r5, r4, r4
	and r4, r6, lr
	add r8, r5, r4, lsl #17
	rsb r3, r3, #0
	mul r5, r7, r7
	and r3, r6, r3
	add r3, r5, r3, lsr #15
	cmp r8, r4, lsl #17
	addlo r3, r3, #1
	str r8, [r0, #0x18]
	subs r2, r2, #4
	str r3, [r0, #0x1c]
	addne r1, r1, #0x10
	addne r0, r0, #0x20
	bne _0223A5CC
	ldmia sp!, {r4, r5, r6, r7, r8, lr}
	bx lr
	.balign 4, 0
_0223A700: .word 0x0000FFFF
	arm_func_end bn_sqr_words

	arm_func_start bn_mul_words
bn_mul_words: ; 0x0223A704
	stmdb sp!, {r4, r5, r6, r7, r8, sb, lr}
	sub sp, sp, #4
	cmp r2, #0
	mov r4, #0
	addle sp, sp, #4
	movle r0, r4
	ldmleia sp!, {r4, r5, r6, r7, r8, sb, lr}
	bxle lr
	ldr r5, _0223A8A0 ; =0x0000FFFF
	and lr, r3, r5
	and ip, r5, r3, lsr #16
_0223A730:
	ldr r3, [r1]
	and r7, r5, r3, lsr #16
	mul r6, lr, r7
	and r3, r3, r5
	mla sb, ip, r3, r6
	mul r8, r3, lr
	mul r3, r7, ip
	cmp sb, r6
	and r6, sb, r5
	addlo r3, r3, #0x10000
	and r7, r5, sb, lsr #16
	add r8, r8, r6, lsl #16
	cmp r8, r6, lsl #16
	add r3, r3, r7
	add r6, r8, r4
	addlo r3, r3, #1
	cmp r6, r4
	addlo r3, r3, #1
	str r6, [r0]
	mov r4, r3
	subs r6, r2, #1
	beq _0223A890
	ldr r4, [r1, #4]
	and r7, r5, r4, lsr #16
	mul r6, lr, r7
	and r4, r4, r5
	mla sb, ip, r4, r6
	mul r8, r4, lr
	mul r4, r7, ip
	cmp sb, r6
	and r6, sb, r5
	addlo r4, r4, #0x10000
	and r7, r5, sb, lsr #16
	add r8, r8, r6, lsl #16
	cmp r8, r6, lsl #16
	add r4, r4, r7
	add r6, r8, r3
	addlo r4, r4, #1
	cmp r6, r3
	addlo r4, r4, #1
	str r6, [r0, #4]
	subs r3, r2, #2
	beq _0223A890
	ldr r3, [r1, #8]
	and r7, r5, r3, lsr #16
	mul r6, lr, r7
	and r3, r3, r5
	mla sb, ip, r3, r6
	mul r8, r3, lr
	mul r3, r7, ip
	cmp sb, r6
	and r6, sb, r5
	addlo r3, r3, #0x10000
	and r7, r5, sb, lsr #16
	add r8, r8, r6, lsl #16
	cmp r8, r6, lsl #16
	add r3, r3, r7
	add r6, r8, r4
	addlo r3, r3, #1
	cmp r6, r4
	addlo r3, r3, #1
	str r6, [r0, #8]
	mov r4, r3
	subs r6, r2, #3
	beq _0223A890
	ldr r4, [r1, #0xc]
	and r7, r5, r4, lsr #16
	mul r6, lr, r7
	and r4, r4, r5
	mla sb, ip, r4, r6
	mul r8, r4, lr
	mul r4, r7, ip
	cmp sb, r6
	and r6, sb, r5
	addlo r4, r4, #0x10000
	and r7, r5, sb, lsr #16
	add r8, r8, r6, lsl #16
	cmp r8, r6, lsl #16
	add r4, r4, r7
	add r6, r8, r3
	addlo r4, r4, #1
	cmp r6, r3
	addlo r4, r4, #1
	subs r2, r2, #4
	str r6, [r0, #0xc]
	addne r1, r1, #0x10
	addne r0, r0, #0x10
	bne _0223A730
_0223A890:
	mov r0, r4
	add sp, sp, #4
	ldmia sp!, {r4, r5, r6, r7, r8, sb, lr}
	bx lr
	.balign 4, 0
_0223A8A0: .word 0x0000FFFF
	arm_func_end bn_mul_words

	arm_func_start bn_mul_add_words
bn_mul_add_words: ; 0x0223A8A4
	stmdb sp!, {r4, r5, r6, r7, r8, sb, lr}
	sub sp, sp, #4
	cmp r2, #0
	mov ip, #0
	addle sp, sp, #4
	movle r0, ip
	ldmleia sp!, {r4, r5, r6, r7, r8, sb, lr}
	bxle lr
	ldr r5, _0223AA80 ; =0x0000FFFF
	and r4, r3, r5
	and lr, r5, r3, lsr #16
_0223A8D0:
	ldr r3, [r1]
	and r7, r5, r3, lsr #16
	mul r6, r4, r7
	and r3, r3, r5
	mla sb, lr, r3, r6
	mul r8, r3, r4
	mul r3, r7, lr
	cmp sb, r6
	and r6, sb, r5
	addlo r3, r3, #0x10000
	and r7, r5, sb, lsr #16
	add r8, r8, r6, lsl #16
	cmp r8, r6, lsl #16
	add r3, r3, r7
	ldr r6, [r0]
	addlo r3, r3, #1
	add r7, r8, ip
	cmp r7, ip
	add r7, r7, r6
	addlo r3, r3, #1
	cmp r7, r6
	addlo r3, r3, #1
	mov ip, r3
	str r7, [r0]
	subs r6, r2, #1
	beq _0223AA70
	ldr ip, [r1, #4]
	and r7, r5, ip, lsr #16
	mul r6, r4, r7
	and ip, ip, r5
	mla sb, lr, ip, r6
	mul r8, ip, r4
	mul ip, r7, lr
	cmp sb, r6
	and r6, sb, r5
	addlo ip, ip, #0x10000
	and r7, r5, sb, lsr #16
	add r8, r8, r6, lsl #16
	cmp r8, r6, lsl #16
	add ip, ip, r7
	add r6, r8, r3
	addlo ip, ip, #1
	cmp r6, r3
	ldr r3, [r0, #4]
	addlo ip, ip, #1
	add r6, r6, r3
	cmp r6, r3
	addlo ip, ip, #1
	str r6, [r0, #4]
	subs r3, r2, #2
	beq _0223AA70
	ldr r3, [r1, #8]
	and r7, r5, r3, lsr #16
	mul r6, r4, r7
	and r3, r3, r5
	mla sb, lr, r3, r6
	mul r8, r3, r4
	mul r3, r7, lr
	cmp sb, r6
	and r6, sb, r5
	addlo r3, r3, #0x10000
	and r7, r5, sb, lsr #16
	add r8, r8, r6, lsl #16
	cmp r8, r6, lsl #16
	add r3, r3, r7
	ldr r6, [r0, #8]
	addlo r3, r3, #1
	add r7, r8, ip
	cmp r7, ip
	add r7, r7, r6
	addlo r3, r3, #1
	cmp r7, r6
	addlo r3, r3, #1
	mov ip, r3
	str r7, [r0, #8]
	subs r6, r2, #3
	beq _0223AA70
	ldr ip, [r1, #0xc]
	and r7, r5, ip, lsr #16
	mul r6, r4, r7
	and ip, ip, r5
	mla sb, lr, ip, r6
	mul r8, ip, r4
	mul ip, r7, lr
	cmp sb, r6
	and r6, sb, r5
	addlo ip, ip, #0x10000
	and r7, r5, sb, lsr #16
	add r8, r8, r6, lsl #16
	cmp r8, r6, lsl #16
	add ip, ip, r7
	add r6, r8, r3
	addlo ip, ip, #1
	cmp r6, r3
	ldr r3, [r0, #0xc]
	addlo ip, ip, #1
	add r6, r6, r3
	cmp r6, r3
	addlo ip, ip, #1
	subs r2, r2, #4
	str r6, [r0, #0xc]
	addne r1, r1, #0x10
	addne r0, r0, #0x10
	bne _0223A8D0
_0223AA70:
	mov r0, ip
	add sp, sp, #4
	ldmia sp!, {r4, r5, r6, r7, r8, sb, lr}
	bx lr
	.balign 4, 0
_0223AA80: .word 0x0000FFFF
	arm_func_end bn_mul_add_words
