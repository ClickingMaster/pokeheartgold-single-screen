	.include "asm/macros.inc"
	.include "nitrocrypto.inc"
	.include "global.inc"

	; rc4s-arm4cw.o

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
	.extern BN_bin2bn
	.extern BN_bn2bin
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
	.extern BN_set_bit
	.extern BN_set_word
	.extern BN_sqr
	.extern BN_sub
	.extern BN_sub_word
	.extern BN_uadd
	.extern BN_ucmp
	.extern BN_usub
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

	arm_func_start CRYPTO_RC4Encrypt
CRYPTO_RC4Encrypt: ; 0x0223706C
	stmdb sp!, {r4, r5, r6, r7, r8, sb, lr}
	ldrb r7, [r0]
	ldrb r6, [r0, #1]
	add r0, r0, #4
	mov lr, #0x1000000
	add r7, lr, r7, lsl #24
	mov r6, r6, lsl #0x18
	ldrb sb, [r0, r7, lsr #24]
	subs r2, r2, #1
	bmi _022370CC
_02237094:
	add r6, r6, sb, lsl #24
	subs r2, r2, #1
	ldrb r8, [r0, r6, lsr #24]
	ldrb r5, [r1], #1
	strb r8, [r0, r7, lsr #24]
	strb sb, [r0, r6, lsr #24]
	add r4, sb, r8
	and r4, r4, #0xff
	add r7, r7, lr
	ldrb r4, [r0, r4]
	ldrb sb, [r0, r7, lsr #24]
	eor r5, r5, r4
	strb r5, [r3], #1
	bge _02237094
_022370CC:
	sub r7, r7, lr
	mov r7, r7, lsr #0x18
	mov r6, r6, lsr #0x18
	strb r7, [r0, #-4]
	strb r6, [r0, #-3]
	ldmia sp!, {r4, r5, r6, r7, r8, sb, pc}
	arm_func_end CRYPTO_RC4Encrypt

	; sign.o

	.rodata

	.public TEMPNAME__$p16
TEMPNAME__$p16:
	.byte 0x00, 0x08, 0x01, 0x01
	.byte 0x01, 0x10, 0x01, 0x00
	.byte 0x00, 0x00, 0x00, 0x00

	.public TEMPNAME__$p2
TEMPNAME__$p2:
	.byte 0x00, 0x08, 0x01, 0x01
	.byte 0x01, 0x01, 0x01, 0x00
	.byte 0x00, 0x00, 0x00, 0x00

	.public TEMPNAME__$p4
TEMPNAME__$p4:
	.byte 0x00, 0x08, 0x01, 0x01
	.byte 0x01, 0x04, 0x01, 0x00
	.byte 0x00, 0x00, 0x00, 0x00

	.public bits$7808
bits$7808:
	.byte 0x00, 0x01, 0x02, 0x02, 0x03, 0x03, 0x03, 0x03, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04

shift:
	.byte 0x06, 0x00, 0x01, 0x00, 0x02, 0x00, 0x01, 0x00, 0x03, 0x00, 0x01, 0x00, 0x02, 0x00, 0x01, 0x00
	.byte 0x04, 0x00, 0x01, 0x00, 0x02, 0x00, 0x01, 0x00, 0x03, 0x00, 0x01, 0x00, 0x02, 0x00, 0x01, 0x00
	.byte 0x05, 0x00, 0x01, 0x00, 0x02, 0x00, 0x01, 0x00, 0x03, 0x00, 0x01, 0x00, 0x02, 0x00, 0x01, 0x00
	.byte 0x04, 0x00, 0x01, 0x00, 0x02, 0x00, 0x01, 0x00
	.byte 0x03, 0x00, 0x01, 0x00
	.byte 0x02, 0x00
	.byte 0x01
	.byte 0x00

	.data

	.public shift_val
shift_val:
	.word shift + 0x3F
	.word shift + 0x3E
	.word shift + 0x3C
	.word shift + 0x38
	.word shift + 0x30
	.word shift + 0x20
	.word shift

	.text
