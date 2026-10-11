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
	.rodata

	.public TEMPNAME__$p16
TEMPNAME__$p16:
	.public gNitroCryptoExpP16
gNitroCryptoExpP16:
	.byte 0x00, 0x08, 0x01, 0x01
	.byte 0x01, 0x10, 0x01, 0x00
	.byte 0x00, 0x00, 0x00, 0x00

	.public TEMPNAME__$p2
TEMPNAME__$p2:
	.public gNitroCryptoExpP2
gNitroCryptoExpP2:
	.byte 0x00, 0x08, 0x01, 0x01
	.byte 0x01, 0x01, 0x01, 0x00
	.byte 0x00, 0x00, 0x00, 0x00

	.public TEMPNAME__$p4
TEMPNAME__$p4:
	.public gNitroCryptoExpP4
gNitroCryptoExpP4:
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
	.public gNitroCryptoShiftVal
gNitroCryptoShiftVal:
	.word shift + 0x3F
	.word shift + 0x3E
	.word shift + 0x3C
	.word shift + 0x38
	.word shift + 0x30
	.word shift + 0x20
	.word shift


