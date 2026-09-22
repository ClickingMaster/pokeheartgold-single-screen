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
	.extern BN_add
	.extern BN_add_word
	.extern BN_clear
	.extern BN_clear_free
	.extern BN_copy
	.extern BN_div
	.extern BN_div_recp
	.extern BN_free
	.extern BN_gen_exp_bits
	.extern BN_gen_exp_string
	.extern BN_init
	.extern BN_is_bit_set
	.extern BN_lshift
	.extern BN_mod
	.extern BN_mod_exp
	.extern BN_mod_exp_mont
	.extern BN_mod_exp_recp
	.extern BN_mod_inverse_word
	.extern BN_mod_mul_reciprocal
	.extern BN_mul
	.extern BN_new
	.extern BN_num_bits
	.extern BN_num_bits_word
	.extern BN_reciprocal
	.extern BN_rshift
	.extern BN_sqr
	.extern BN_sub
	.extern BN_sub_word
	.extern BN_uadd
	.extern BN_usub
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
	.extern bn_add_words
	.extern bn_div_words
	.extern bn_expand2
	.extern bn_fix_top
	.extern bn_from_montgomery_words
	.extern bn_mul_add_words
	.extern bn_mul_normal
	.extern bn_mul_words
	.extern bn_sqr_normal
	.extern bn_sqr_words
	.extern bn_sub_words
	.extern bn_zexpand
	.extern shift_val


	arm_func_start BN_bin2bn
BN_bin2bn: ; 0x0223AC60
	stmdb sp!, {r4, r5, r6, r7, lr}
	sub sp, sp, #4
	movs r5, r2
	mov r7, r0
	mov r6, r1
	bne _0223AC80
	bl BN_new
	mov r5, r0
_0223AC80:
	cmp r5, #0
	addeq sp, sp, #4
	moveq r0, #0
	ldmeqia sp!, {r4, r5, r6, r7, lr}
	bxeq lr
	cmp r6, #0
	mov r4, #0
	addeq sp, sp, #4
	moveq r0, r5
	streq r4, [r5, #4]
	ldmeqia sp!, {r4, r5, r6, r7, lr}
	bxeq lr
	add r0, r6, #2
	mov r2, r0, lsl #3
	add r1, r2, #0x1f
	mov r0, r1, asr #4
	add r0, r1, r0, lsr #27
	ldr r1, [r5, #8]
	mov r0, r0, asr #5
	cmp r0, r1
	movle r0, r5
	ble _0223ACF0
	mov r0, r2, asr #4
	add r0, r2, r0, lsr #27
	mov r1, r0, asr #5
	mov r0, r5
	add r1, r1, #1
	bl bn_expand2
_0223ACF0:
	cmp r0, #0
	addeq sp, sp, #4
	moveq r0, #0
	ldmeqia sp!, {r4, r5, r6, r7, lr}
	bxeq lr
	sub r0, r6, #1
	mov r1, r0, lsr #2
	add ip, r1, #1
	cmp r6, #0
	str ip, [r5, #4]
	and r3, r0, #3
	sub r6, r6, #1
	beq _0223AD5C
	mov r1, #0
	mov r0, #3
_0223AD2C:
	ldrb r2, [r7], #1
	cmp r3, #0
	sub r3, r3, #1
	orr r4, r2, r4, lsl #8
	ldreq r2, [r5]
	subeq ip, ip, #1
	streq r4, [r2, ip, lsl #2]
	moveq r4, r1
	moveq r3, r0
	cmp r6, #0
	sub r6, r6, #1
	bne _0223AD2C
_0223AD5C:
	mov r0, r5
	bl bn_fix_top
	mov r0, r5
	add sp, sp, #4
	ldmia sp!, {r4, r5, r6, r7, lr}
	bx lr
	arm_func_end BN_bin2bn
