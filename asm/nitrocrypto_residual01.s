	.include "asm/macros.inc"
	.include "nitrocrypto.inc"
	.include "global.inc"

	.text

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
	.extern BN_free
	.extern BN_gen_exp_bits
	.extern BN_gen_exp_string
	.extern BN_init
	.extern BN_is_bit_set
	.extern BN_lshift
	.extern BN_mod
	.extern BN_mod_exp
	.extern BN_mod_inverse_word
	.extern BN_mul
	.extern BN_new
	.extern BN_num_bits
	.extern BN_num_bits_word
	.extern BN_rshift
	.extern BN_set_bit
	.extern BN_set_word
	.extern BN_sqr
	.extern BN_sub
	.extern BN_sub_word
	.extern BN_uadd
	.extern BN_ucmp
	.extern BN_usub
	.extern CRYPTO_RC4Encrypt
	.extern CRYPTO_SetAllocator
	.extern CRYPTO_VerifySignature
	.extern CRYPTOi_MyAlloc
	.extern CRYPTOi_MyAllocFunc
	.extern CRYPTOi_MyFree
	.extern CRYPTOi_MyFreeFunc
	.extern TEMPNAME__$p16
	.extern TEMPNAME__$p2
	.extern TEMPNAME__$p4
	.extern bits$7808
	.extern bn_add_words
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


	arm_func_start CRYPTO_VerifySignatureWithHash
CRYPTO_VerifySignatureWithHash: ; 0x02237128
	stmdb sp!, {r4, lr}
	sub sp, sp, #0x120
	mov r3, #0
	str r3, [sp, #0x10]
	str r3, [sp, #0x18]
	str r2, [sp]
	mov r3, #0x80
	mov r4, r0
	mov r2, r1
	ldr ip, _02237224 ; =0x00010001
	str r3, [sp, #4]
	add r0, sp, #0x1c
	mov r1, #0x100
	str ip, [sp, #8]
	bl CRYPTOi_RSA
	mov r1, r0
	add r0, sp, #0x1c
	add r2, sp, #0xc
	add r3, sp, #0x10
	bl SkipPadding
	cmp r0, #0
	addeq sp, sp, #0x120
	moveq r0, #0
	ldmeqia sp!, {r4, lr}
	bxeq lr
	add r1, sp, #0x14
	add r0, sp, #0x18
	str r1, [sp]
	str r0, [sp, #4]
	mov r2, #0
	ldr r0, [sp, #0xc]
	ldr r1, [sp, #0x10]
	mov r3, r2
	bl ParseSignHash
	cmp r0, #0
	addeq sp, sp, #0x120
	moveq r0, #0
	ldmeqia sp!, {r4, lr}
	bxeq lr
	ldr r0, [sp, #0x18]
	cmp r0, #0x14
	addne sp, sp, #0x120
	movne r0, #0
	ldmneia sp!, {r4, lr}
	bxne lr
	ldr r2, [sp, #0x14]
	mov r3, #0
_022371E4:
	ldrb r1, [r2]
	ldrb r0, [r4]
	cmp r1, r0
	addne sp, sp, #0x120
	movne r0, #0
	ldmneia sp!, {r4, lr}
	bxne lr
	add r3, r3, #1
	cmp r3, #0x14
	add r4, r4, #1
	add r2, r2, #1
	blt _022371E4
	mov r0, #1
	add sp, sp, #0x120
	ldmia sp!, {r4, lr}
	bx lr
	.balign 4, 0
_02237224: .word 0x00010001
	arm_func_end CRYPTO_VerifySignatureWithHash

	arm_func_start ParseSignHash
ParseSignHash: ; 0x02237228
	stmdb sp!, {r0, r1, r2, r3}
	stmdb sp!, {r4, r5, lr}
	sub sp, sp, #4
	mov r5, r2
	mov r4, r3
	add r0, sp, #0x10
	add r1, sp, #0x14
	mov r2, #0x30
	mov r3, #0
	bl ASN1Skip
	cmp r0, #0
	addeq sp, sp, #4
	moveq r0, #0
	ldmeqia sp!, {r4, r5, lr}
	addeq sp, sp, #0x10
	bxeq lr
	add r0, sp, #0x10
	add r1, sp, #0x14
	mov r2, #0x30
	mov r3, #0
	bl ASN1Skip
	cmp r0, #0
	addeq sp, sp, #4
	moveq r0, #0
	ldmeqia sp!, {r4, r5, lr}
	addeq sp, sp, #0x10
	bxeq lr
	add r0, sp, #0x10
	add r1, sp, #0x14
	add r3, sp, #0
	mov r2, #6
	bl ASN1Skip
	cmp r0, #0
	addeq sp, sp, #4
	moveq r0, #0
	ldmeqia sp!, {r4, r5, lr}
	addeq sp, sp, #0x10
	bxeq lr
	cmp r5, #0
	ldrne r0, [sp, #0x10]
	strne r0, [r5]
	cmp r4, #0
	ldrne r0, [sp]
	strne r0, [r4]
	ldr r2, [sp]
	ldr r0, [sp, #0x10]
	ldr r1, [sp, #0x14]
	add r0, r0, r2
	str r0, [sp, #0x10]
	cmp r1, r2
	addlo sp, sp, #4
	movlo r0, #0
	ldmloia sp!, {r4, r5, lr}
	addlo sp, sp, #0x10
	bxlo lr
	sub ip, r1, r2
	add r0, sp, #0x10
	add r1, sp, #0x14
	add r3, sp, #0
	mov r2, #5
	str ip, [sp, #0x14]
	bl ASN1Skip
	cmp r0, #0
	addeq sp, sp, #4
	moveq r0, #0
	ldmeqia sp!, {r4, r5, lr}
	addeq sp, sp, #0x10
	bxeq lr
	ldr r2, [sp]
	ldr r0, [sp, #0x10]
	ldr r1, [sp, #0x14]
	add r0, r0, r2
	str r0, [sp, #0x10]
	cmp r1, r2
	addlo sp, sp, #4
	movlo r0, #0
	ldmloia sp!, {r4, r5, lr}
	addlo sp, sp, #0x10
	bxlo lr
	sub ip, r1, r2
	add r0, sp, #0x10
	add r1, sp, #0x14
	add r3, sp, #0
	mov r2, #4
	str ip, [sp, #0x14]
	bl ASN1Skip
	cmp r0, #0
	addeq sp, sp, #4
	moveq r0, #0
	ldmeqia sp!, {r4, r5, lr}
	addeq sp, sp, #0x10
	bxeq lr
	ldr r1, [sp, #0x20]
	cmp r1, #0
	ldrne r0, [sp, #0x10]
	strne r0, [r1]
	ldr r1, [sp, #0x24]
	cmp r1, #0
	ldrne r0, [sp]
	strne r0, [r1]
	mov r0, #1
	add sp, sp, #4
	ldmia sp!, {r4, r5, lr}
	add sp, sp, #0x10
	bx lr
	arm_func_end ParseSignHash

	arm_func_start SkipPadding
SkipPadding: ; 0x022373CC
	stmdb sp!, {lr}
	sub sp, sp, #4
	cmp r1, #0xa
	addlo sp, sp, #4
	movlo r0, #0
	ldmloia sp!, {lr}
	bxlo lr
	ldrb ip, [r0]
	add lr, r0, r1
	cmp ip, #1
	addne sp, sp, #4
	movne r0, #0
	ldmneia sp!, {lr}
	bxne lr
	add ip, r0, #1
	mov r1, #0
_0223740C:
	ldrb r0, [ip], #1
	cmp r0, #0xff
	addne sp, sp, #4
	movne r0, #0
	ldmneia sp!, {lr}
	bxne lr
	add r1, r1, #1
	cmp r1, #8
	blt _0223740C
	cmp ip, lr
	beq _02237450
_02237438:
	ldrb r0, [ip]
	cmp r0, #0xff
	bne _02237450
	add ip, ip, #1
	cmp ip, lr
	bne _02237438
_02237450:
	cmp ip, lr
	addeq sp, sp, #4
	moveq r0, #0
	ldmeqia sp!, {lr}
	bxeq lr
	ldrb r0, [ip]
	cmp r0, #0
	movne r0, #0
	addeq r0, ip, #1
	subeq r1, lr, r0
	streq r1, [r3]
	streq r0, [r2]
	moveq r0, #1
	add sp, sp, #4
	ldmia sp!, {lr}
	bx lr
	arm_func_end SkipPadding

	arm_func_start ASN1Skip
ASN1Skip: ; 0x02237490
	stmdb sp!, {r4, r5, r6, lr}
	ldr r5, [r0]
	mov r6, #0
	ldrb r4, [r5]
	add r5, r5, #1
	mov ip, r6
	cmp r4, r2
	ldr r2, [r1]
	movne r0, r6
	ldmneia sp!, {r4, r5, r6, lr}
	bxne lr
	cmp r2, #1
	movlo r0, r6
	ldmloia sp!, {r4, r5, r6, lr}
	bxlo lr
	ldrb lr, [r5]
	sub r4, r2, #1
	ands r2, lr, #0x80
	beq _0223752C
	and r2, lr, #0x7f
	and lr, r2, #0xff
	add r2, lr, #1
	cmp r4, r2
	movlo r0, r6
	ldmloia sp!, {r4, r5, r6, lr}
	bxlo lr
	cmp r3, #0
	sub r6, r4, lr
	beq _02237524
	ldrb r2, [r5, #1]
	add r5, r5, #1
	and r4, r2, #0x7f
_02237510:
	sub r2, lr, #1
	add ip, r4, ip, lsl #7
	ands lr, r2, #0xff
	bne _02237510
	b _02237548
_02237524:
	add r5, r5, lr
	b _02237548
_0223752C:
	add r5, r5, #1
	cmp r4, #1
	mov ip, lr
	movlo r0, r6
	ldmloia sp!, {r4, r5, r6, lr}
	bxlo lr
	sub r6, r4, #1
_02237548:
	str r5, [r0]
	str r6, [r1]
	cmp r3, #0
	strne ip, [r3]
	mov r0, #1
	ldmia sp!, {r4, r5, r6, lr}
	bx lr
	arm_func_end ASN1Skip

	arm_func_start CRYPTOi_RSA
CRYPTOi_RSA: ; 0x02237564
	stmdb sp!, {r4, r5, r6, r7, r8, lr}
	sub sp, sp, #0x58
	movs r6, r0
	mov r5, r1
	mov r8, r2
	mov r7, r3
	beq _02237594
	cmp r8, #0
	beq _02237594
	ldr r0, [sp, #0x70]
	cmp r0, #0
	bne _022375A4
_02237594:
	add sp, sp, #0x58
	mvn r0, #2
	ldmia sp!, {r4, r5, r6, r7, r8, lr}
	bx lr
_022375A4:
	bl BN_CTX_new
	mov r4, r0
	add r0, sp, #4
	bl BN_init
	add r0, sp, #0x18
	bl BN_init
	add r0, sp, #0x2c
	bl BN_init
	add r0, sp, #0x40
	bl BN_init
	cmp r4, #0
	mvneq r5, #1
	beq _02237680
	add r2, sp, #4
	mov r0, r8
	mov r1, r7
	bl BN_bin2bn
	cmp r0, #0
	mvneq r5, #1
	beq _02237680
	ldr r1, [sp, #0x78]
	add r0, sp, #0x2c
	bl BN_set_word
	cmp r0, #0
	mvneq r5, #1
	beq _02237680
	ldr r0, [sp, #0x70]
	ldr r1, [sp, #0x74]
	add r2, sp, #0x40
	bl BN_bin2bn
	cmp r0, #0
	mvneq r5, #1
	beq _02237680
	add r0, sp, #0x18
	add r1, sp, #4
	add r2, sp, #0x2c
	add r3, sp, #0x40
	str r4, [sp]
	bl BN_mod_exp
	cmp r0, #0
	mvneq r5, #1
	beq _02237680
	add r0, sp, #0x18
	bl BN_num_bits
	add r1, r0, #7
	mov r0, r1, asr #2
	add r0, r1, r0, lsr #29
	mov r0, r0, asr #3
	cmp r0, r5
	mvngt r5, #0
	bgt _02237680
	add r0, sp, #0x18
	mov r1, r6
	bl BN_bn2bin
	mov r5, r0
_02237680:
	add r0, sp, #4
	bl BN_free
	add r0, sp, #0x18
	bl BN_free
	add r0, sp, #0x2c
	bl BN_free
	add r0, sp, #0x40
	bl BN_free
	cmp r4, #0
	beq _022376B0
	mov r0, r4
	bl BN_CTX_free
_022376B0:
	mov r0, r5
	add sp, sp, #0x58
	ldmia sp!, {r4, r5, r6, r7, r8, lr}
	bx lr
	arm_func_end CRYPTOi_RSA

	arm_func_start BN_mod_exp_mont
BN_mod_exp_mont: ; 0x022376C0
	stmdb sp!, {r4, r5, r6, r7, r8, sb, sl, fp, lr}
	sub sp, sp, #0x7c
	mov r4, #0
	mov r5, r3
	mov r3, r4
	str r3, [sp, #0x34]
	ldr r3, [r5]
	str r0, [sp, #4]
	mov r0, r4
	ldr r3, [r3]
	str r0, [sp, #0x1c]
	str r0, [sp, #0x20]
	ands r0, r3, #1
	ldr r0, [sp, #0xa0]
	str r4, [sp, #8]
	str r0, [sp, #0xa0]
	mov r7, r1
	mov r6, r2
	addeq sp, sp, #0x7c
	moveq r0, r4
	ldmeqia sp!, {r4, r5, r6, r7, r8, sb, sl, fp, lr}
	bxeq lr
	ldr r1, [r7, #4]
	ldr r0, [r0]
	cmp r1, #0
	str r0, [sp, #0xc]
	beq _02237744
	cmp r1, #1
	bne _02237760
	ldr r0, [r7]
	ldr r0, [r0]
	cmp r0, #0
	bne _02237760
_02237744:
	ldr r0, [sp, #4]
	mov r1, #0
	bl BN_set_word
	add sp, sp, #0x7c
	mov r0, #1
	ldmia sp!, {r4, r5, r6, r7, r8, sb, sl, fp, lr}
	bx lr
_02237760:
	ldr r1, [r6, #4]
	cmp r1, #0
	beq _02237784
	cmp r1, #1
	bne _022377A0
	ldr r0, [r6]
	ldr r0, [r0]
	cmp r0, #0
	bne _022377A0
_02237784:
	ldr r0, [sp, #4]
	mov r1, #1
	bl BN_set_word
	add sp, sp, #0x7c
	mov r0, #1
	ldmia sp!, {r4, r5, r6, r7, r8, sb, sl, fp, lr}
	bx lr
_022377A0:
	cmp r1, #1
	bne _022377D4
	ldr r0, [r6]
	ldr r0, [r0]
	cmp r0, #1
	bne _022377D4
	ldr r0, [sp, #4]
	mov r1, r7
	bl BN_copy
	add sp, sp, #0x7c
	mov r0, #1
	ldmia sp!, {r4, r5, r6, r7, r8, sb, sl, fp, lr}
	bx lr
_022377D4:
	ldr r0, [sp, #0xa4]
	str r0, [sp, #0x18]
	cmp r0, #0
	bne _02237808
	bl BN_MONT_CTX_new
	str r0, [sp, #0x18]
	cmp r0, #0
	beq _02237E64
	ldr r2, [sp, #0xa0]
	mov r1, r5
	bl BN_MONT_CTX_set_word
	cmp r0, #0
	beq _02237E64
_02237808:
	ldr r3, [sp, #0xa0]
	add r1, sp, #0x34
	mov r4, r3
	ldr r4, [r4, #0x10c]
	mov r0, r6
	mov r2, #0
	str r4, [sp, #0x1c]
	bl BN_gen_exp_bits
	cmp r0, #0
	beq _02237E64
	ldr r0, [sp, #0xa0]
	ldr r0, [r0]
	add r1, r0, #1
	ldr r0, [sp, #0xa0]
	str r1, [r0]
	ldr r0, [sp, #0x34]
	ldrb r1, [r0, #2]
	ldrb r0, [r0, #3]
	str r0, [sp, #0x10]
	add r0, r1, #0x3f
	bl _s32_div_f
	ldr r1, [sp, #0x34]
	mov r4, r0
	add r0, r1, #4
	str r0, [sp, #0x34]
	ldr r0, [sp, #0x18]
	ldr r3, [r7, #4]
	ldr sl, [r0, #0x24]
	cmp r3, sl
	bne _022378A0
	ldr r1, [r7]
	sub r2, sl, #1
	ldr r0, [r5]
	ldr r1, [r1, r2, lsl #2]
	ldr r0, [r0, r2, lsl #2]
	cmp r1, r0
	strlo r7, [sp, #0x14]
	blo _02237958
_022378A0:
	cmp r3, sl
	bge _02237914
	ldr r0, [sp, #0xa0]
	ldr r3, [r0]
	add r1, r0, #4
	add r2, r3, #1
	str r2, [r0]
	mov r0, #0x14
	mla r0, r3, r0, r1
	ldr r2, [r7, #4]
	mov r1, r0
	str r2, [r1, #4]
	mov r1, sl
	str r0, [sp, #0x14]
	bl bn_zexpand
	ldr r0, [r7, #4]
	mov r2, #0
	cmp r0, #0
	ble _02237958
_022378EC:
	ldr r1, [r7]
	ldr r0, [sp, #0x14]
	ldr r1, [r1, r2, lsl #2]
	ldr r0, [r0]
	str r1, [r0, r2, lsl #2]
	ldr r0, [r7, #4]
	add r2, r2, #1
	cmp r2, r0
	blt _022378EC
	b _02237958
_02237914:
	ldr r0, [sp, #0xa0]
	mov r2, r5
	ldr r3, [r0]
	mov r1, r7
	add r5, r3, #1
	str r5, [r0]
	add r5, r0, #4
	mov r0, #0x14
	mla r0, r3, r0, r5
	ldr r3, [sp, #0xa0]
	str r0, [sp, #0x14]
	bl BN_mod
	cmp r0, #0
	beq _02237E64
	ldr r0, [sp, #0x14]
	mov r1, sl
	bl bn_zexpand
_02237958:
	ldr r0, [sp, #0xa0]
	ldr r3, [r0]
	ldr r0, [r6, #4]
	add r1, r3, #1
	mul r2, r0, r4
	ldr r0, [sp, #0xa0]
	mov r4, r2, lsl #1
	str r1, [r0]
	ldr r1, [r0]
	add r4, r4, #7
	add r2, r1, #1
	str r2, [r0]
	ldr r2, [sp, #0x10]
	ldr r0, [r0]
	mul r5, r2, sl
	mov r2, r4, asr #1
	add r2, r4, r2, lsr #30
	add r5, r5, r2, asr #2
	ldr r2, [sp, #0xa0]
	mov r6, #0x14
	add r2, r2, #4
	mla r8, r1, r6, r2
	mla r4, r3, r6, r2
	ldr r1, [sp, #0xa0]
	add r3, r0, #1
	mla r7, r0, r6, r2
	str r3, [r1]
	mov r0, r1
	ldr r0, [r0]
	mla r6, r0, r6, r2
	add r1, r0, #1
	ldr r0, [sp, #0xa0]
	str r1, [r0]
	ldr r0, [sp, #4]
	ldr r0, [r0, #8]
	cmp sl, r0
	ldrle r0, [sp, #4]
	ble _022379FC
	ldr r0, [sp, #4]
	mov r1, sl
	bl bn_expand2
_022379FC:
	cmp r0, #0
	beq _02237E64
	ldr r0, [r8, #8]
	mov r1, sl, lsl #2
	cmp r1, r0
	movle r0, r8
	ble _02237A20
	mov r0, r8
	bl bn_expand2
_02237A20:
	cmp r0, #0
	beq _02237E64
	ldr r1, [r7, #8]
	mov r0, sl, lsl #1
	str r0, [sp, #0x24]
	cmp r0, r1
	movle r0, r7
	ble _02237A4C
	ldr r1, [sp, #0x24]
	mov r0, r7
	bl bn_expand2
_02237A4C:
	cmp r0, #0
	beq _02237E64
	ldr r0, [r4, #8]
	cmp r5, r0
	movle r0, r4
	ble _02237A70
	mov r1, r5
	mov r0, r4
	bl bn_expand2
_02237A70:
	cmp r0, #0
	beq _02237E64
	ldr r1, [r6, #8]
	ldr r0, [sp, #0x24]
	cmp r0, r1
	movle r0, r6
	ble _02237A98
	ldr r1, [sp, #0x24]
	mov r0, r6
	bl bn_expand2
_02237A98:
	cmp r0, #0
	beq _02237E64
	ldr r0, [sp, #0x18]
	ldr fp, [r8]
	ldr r8, [r7]
	ldr r6, [r6]
	ldr r1, [r4]
	ldr r7, [r0, #0x48]
	ldr r5, [r0, #0x20]
	ldr r3, [sp, #0x18]
	str r1, [sp, #0x38]
	str sl, [sp]
	ldr r1, [sp, #0x14]
	ldr r3, [r3, #0xc]
	ldr r1, [r1]
	mov r0, r6
	mov r2, sl
	bl bn_mul_normal
	str r7, [sp]
	ldr r0, [sp, #0x38]
	mov r1, r6
	mov r2, r5
	mov r3, sl
	bl bn_from_montgomery_words
	ldr r0, [sp, #0x10]
	cmp r0, #1
	ble _02237BA0
	ldr r1, [sp, #0x38]
	mov r0, r6
	mov r2, sl
	mov r3, fp
	bl bn_sqr_normal
	mov r0, fp
	mov r1, r6
	mov r2, r5
	mov r3, sl
	str r7, [sp]
	bl bn_from_montgomery_words
	ldr r0, [sp, #0x10]
	mov r4, #1
	cmp r0, #1
	ble _02237BA0
	mov r0, sl, lsl #2
	str r0, [sp, #0x28]
	add sb, sp, #0x38
_02237B4C:
	sub r3, r4, #1
	ldr r2, [sb, r3, lsl #2]
	ldr r1, [sp, #0x28]
	mov r0, r6
	add r1, r2, r1
	str r1, [sb, r4, lsl #2]
	str sl, [sp]
	ldr r1, [sb, r3, lsl #2]
	mov r2, sl
	mov r3, fp
	bl bn_mul_normal
	str r7, [sp]
	ldr r0, [sb, r4, lsl #2]
	mov r1, r6
	mov r2, r5
	mov r3, sl
	bl bn_from_montgomery_words
	ldr r0, [sp, #0x10]
	add r4, r4, #1
	cmp r4, r0
	blt _02237B4C
_02237BA0:
	ldr r3, [sp, #0x34]
	add r0, r3, #1
	str r0, [sp, #0x34]
	add r2, r0, #1
	ldrb r1, [r3]
	str r2, [sp, #0x34]
	ldrb sb, [r3, #1]
	cmp sb, #0xff
	bne _02237C20
	cmp r1, #0
	bne _02237C20
	add r0, r2, #1
	str r0, [sp, #0x34]
	ldrb r1, [r2]
	b _02237BF0
_02237BDC:
	add sb, sb, #0x100
	ldr r1, [sp, #0x34]
	add r1, r1, #2
	str r1, [sp, #0x34]
	ldrb r1, [r0, #1]
_02237BF0:
	ldr r0, [sp, #0x34]
	ldrb r2, [r0]
	cmp r2, #0xff
	bne _02237C08
	cmp r1, #0
	beq _02237BDC
_02237C08:
	ldr r2, [sp, #0x34]
	add r2, r2, #1
	str r2, [sp, #0x34]
	ldrb r0, [r0]
	add r0, r0, #1
	add sb, sb, r0
_02237C20:
	mov r2, r1, asr #1
	add r0, sp, #0x38
	ldr r0, [r0, r2, lsl #2]
	mov r1, r8
	mov r2, sl, lsl #2
	bl MI_CpuCopy8
	cmp sb, #0
	beq _02237DF8
	mov r0, #0xff
	str r0, [sp, #0x2c]
	mov r0, #0
	str r0, [sp, #0x30]
_02237C50:
	ldr r0, [sp, #0x1c]
	cmp r0, #0
	beq _02237C84
	mov r3, r0
	ldr r1, [sp, #0x2c]
	ldr r2, [sp, #0x20]
	ldr r3, [r3]
	blx r3
	cmp r0, #0
	ldr r0, [sp, #0x20]
	add r0, r0, #1
	str r0, [sp, #0x20]
	bne _02237E64
_02237C84:
	ldr r0, [sp, #0xa0]
	ldr r0, [r0, #0x108]
	ands r0, r0, #0x4000
	bne _02237E64
	cmp sb, #0
	ldr r4, [sp, #0x30]
	ble _02237CD8
_02237CA0:
	mov r0, r6
	mov r1, r8
	mov r2, sl
	mov r3, fp
	bl bn_sqr_normal
	str r7, [sp]
	mov r0, r8
	mov r1, r6
	mov r2, r5
	mov r3, sl
	bl bn_from_montgomery_words
	add r4, r4, #1
	cmp r4, sb
	blt _02237CA0
_02237CD8:
	ldr r2, [sp, #0x34]
	add r0, r2, #1
	str r0, [sp, #0x34]
	add r1, r0, #1
	ldrb r0, [r2]
	str r1, [sp, #0x34]
	ldrb sb, [r2, #1]
	cmp sb, #0xff
	bne _02237D58
	cmp r0, #0
	bne _02237D58
	add r0, r1, #1
	str r0, [sp, #0x34]
	ldrb r0, [r1]
	b _02237D28
_02237D14:
	add sb, sb, #0x100
	ldr r0, [sp, #0x34]
	add r0, r0, #2
	str r0, [sp, #0x34]
	ldrb r0, [r1, #1]
_02237D28:
	ldr r1, [sp, #0x34]
	ldrb r2, [r1]
	cmp r2, #0xff
	bne _02237D40
	cmp r0, #0
	beq _02237D14
_02237D40:
	ldr r2, [sp, #0x34]
	add r2, r2, #1
	str r2, [sp, #0x34]
	ldrb r1, [r1]
	add r1, r1, #1
	add sb, sb, r1
_02237D58:
	cmp r0, #0
	bne _02237D68
	cmp sb, #0
	beq _02237DF8
_02237D68:
	cmp sb, #0
	bne _02237D78
	cmp r0, #1
	beq _02237DB4
_02237D78:
	mov r1, r0, asr #1
	str sl, [sp]
	add r0, sp, #0x38
	ldr r3, [r0, r1, lsl #2]
	mov r0, r6
	mov r1, r8
	mov r2, sl
	bl bn_mul_normal
	mov r0, r8
	mov r1, r6
	mov r2, r5
	mov r3, sl
	str r7, [sp]
	bl bn_from_montgomery_words
	b _02237DF0
_02237DB4:
	ldr r0, [sp, #0x14]
	str sl, [sp]
	ldr r3, [r0]
	mov r0, r6
	mov r1, r8
	mov r2, sl
	bl bn_mul_normal
	ldr r0, [sp, #4]
	str r7, [sp]
	ldr r0, [r0]
	mov r1, r6
	mov r2, r5
	mov r3, sl
	bl bn_from_montgomery_words
	b _02237E3C
_02237DF0:
	cmp sb, #0
	bne _02237C50
_02237DF8:
	ldr r0, [sp, #0x24]
	mov r2, sl
	cmp sl, r0
	bge _02237E20
	mov r1, #0
_02237E0C:
	ldr r0, [sp, #0x24]
	str r1, [r8, r2, lsl #2]
	add r2, r2, #1
	cmp r2, r0
	blt _02237E0C
_02237E20:
	ldr r0, [sp, #4]
	str r7, [sp]
	ldr r0, [r0]
	mov r1, r8
	mov r2, r5
	mov r3, sl
	bl bn_from_montgomery_words
_02237E3C:
	ldr r0, [sp, #0xa0]
	ldr r0, [r0, #0x108]
	ands r0, r0, #0x4000
	bne _02237E64
	ldr r0, [sp, #4]
	mov r1, r0
	str sl, [r1, #4]
	bl bn_fix_top
	mov r0, #1
	str r0, [sp, #8]
_02237E64:
	ldr r0, [sp, #0x1c]
	cmp r0, #0
	beq _02237E8C
	ldr r3, [r0]
	mov r1, #0xff
	mvn r2, #0
	blx r3
	cmp r0, #0
	movne r0, #0
	strne r0, [sp, #8]
_02237E8C:
	ldr r0, [sp, #0xa4]
	cmp r0, #0
	bne _02237EA8
	ldr r0, [sp, #0x18]
	cmp r0, #0
	beq _02237EA8
	bl BN_MONT_CTX_free
_02237EA8:
	ldr r2, [sp, #0xc]
	ldr r1, [sp, #0xa0]
	ldr r0, [sp, #8]
	str r2, [r1]
	add sp, sp, #0x7c
	ldmia sp!, {r4, r5, r6, r7, r8, sb, sl, fp, lr}
	bx lr
	arm_func_end BN_mod_exp_mont

	arm_func_start BN_mod_exp_recp
BN_mod_exp_recp: ; 0x02237EC4
	stmdb sp!, {r4, r5, r6, r7, r8, sb, sl, fp, lr}
	sub sp, sp, #0x18c
	mov sl, r0
	mov r0, #0
	str r0, [sp, #8]
	mov r5, r1
	ldr r1, [sp, #8]
	mov r0, r2
	str r2, [sp, #4]
	mov r4, r3
	str r1, [sp, #0xc]
	ldr sb, [sp, #0x1b0]
	bl BN_num_bits
	ldr r1, [r5, #4]
	mov r8, r0
	cmp r1, #0
	beq _02237F20
	cmp r1, #1
	bne _02237F3C
	ldr r0, [r5]
	ldr r0, [r0]
	cmp r0, #0
	bne _02237F3C
_02237F20:
	mov r0, sl
	mov r1, #0
	bl BN_set_word
	add sp, sp, #0x18c
	mov r0, #1
	ldmia sp!, {r4, r5, r6, r7, r8, sb, sl, fp, lr}
	bx lr
_02237F3C:
	ldr r0, [sp, #4]
	ldr r1, [r0, #4]
	cmp r1, #0
	beq _02237F64
	cmp r1, #1
	bne _02237F80
	ldr r0, [r0]
	ldr r0, [r0]
	cmp r0, #0
	bne _02237F80
_02237F64:
	mov r0, sl
	mov r1, #1
	bl BN_set_word
	add sp, sp, #0x18c
	mov r0, #1
	ldmia sp!, {r4, r5, r6, r7, r8, sb, sl, fp, lr}
	bx lr
_02237F80:
	cmp r1, #1
	bne _02237FB8
	ldr r0, [sp, #4]
	ldr r0, [r0]
	ldr r0, [r0]
	cmp r0, #1
	bne _02237FB8
	mov r0, sl
	mov r1, r5
	bl BN_copy
	add sp, sp, #0x18c
	mov r0, #1
	ldmia sp!, {r4, r5, r6, r7, r8, sb, sl, fp, lr}
	bx lr
_02237FB8:
	add r0, sp, #0x18
	bl BN_RECP_CTX_init
	add r0, sp, #0x18
	mov r1, r4
	mov r2, sb
	bl BN_RECP_CTX_set
	cmp r0, #0
	ble _02238204
	add r0, sp, #0x4c
	bl BN_init
	ldr r2, [sb]
	add r1, sb, #4
	mov r0, #0x14
	mla r6, r2, r0, r1
	add r1, r2, #1
	str r1, [sb]
	mov r2, r4
	mov r4, #1
	add r0, sp, #0x4c
	mov r1, r5
	mov r3, sb
	str r4, [sp, #0xc]
	bl BN_mod
	cmp r0, #0
	beq _02238204
	add r1, sp, #0x4c
	add r3, sp, #0x18
	mov r0, r6
	mov r2, r1
	str sb, [sp]
	bl BN_mod_mul_reciprocal
	cmp r0, #0
	beq _02238204
	cmp r8, #0x11
	movle fp, r4
	ble _02238060
	cmp r8, #0x100
	movge fp, #5
	bge _02238060
	cmp r8, #0x80
	movge fp, #4
	movlt fp, #3
_02238060:
	sub r0, fp, #1
	mov r4, #1
	mov r7, r4, lsl r0
	cmp r7, #1
	ble _022380BC
	add r5, sp, #0x60
_02238078:
	mov r0, r5
	bl BN_init
	sub r3, r4, #1
	mov r2, #0x14
	add r1, sp, #0x4c
	mla r1, r3, r2, r1
	mov r0, r5
	mov r2, r6
	add r3, sp, #0x18
	str sb, [sp]
	bl BN_mod_mul_reciprocal
	cmp r0, #0
	beq _02238204
	add r4, r4, #1
	cmp r4, r7
	add r5, r5, #0x14
	blt _02238078
_022380BC:
	mov r5, #1
	mov r0, sl
	mov r1, r5
	str r4, [sp, #0xc]
	sub r8, r8, #1
	bl BN_set_word
	cmp r0, #0
	beq _02238204
	mov r0, r5
	str r0, [sp, #0x10]
	mov r0, #0
	str r0, [sp, #0x14]
_022380EC:
	ldr r0, [sp, #4]
	mov r1, r8
	bl BN_is_bit_set
	cmp r0, #0
	bne _02238138
	cmp r5, #0
	bne _02238128
	mov r0, sl
	mov r1, sl
	mov r2, sl
	add r3, sp, #0x18
	str sb, [sp]
	bl BN_mod_mul_reciprocal
	cmp r0, #0
	beq _02238204
_02238128:
	cmp r8, #0
	beq _022381FC
	sub r8, r8, #1
	b _022380EC
_02238138:
	ldr r6, [sp, #0x10]
	ldr r7, [sp, #0x14]
	cmp fp, #1
	mov r4, r6
	ble _0223817C
_0223814C:
	subs r1, r8, r4
	bmi _0223817C
	ldr r0, [sp, #4]
	bl BN_is_bit_set
	cmp r0, #0
	subne r0, r4, r7
	movne r0, r6, lsl r0
	movne r7, r4
	add r4, r4, #1
	orrne r6, r0, #1
	cmp r4, fp
	blt _0223814C
_0223817C:
	cmp r5, #0
	add r5, r7, #1
	bne _022381C0
	cmp r5, #0
	ldr r4, [sp, #0x14]
	ble _022381C0
_02238194:
	mov r0, sl
	mov r1, sl
	mov r2, sl
	add r3, sp, #0x18
	str sb, [sp]
	bl BN_mod_mul_reciprocal
	cmp r0, #0
	beq _02238204
	add r4, r4, #1
	cmp r4, r5
	blt _02238194
_022381C0:
	mov r3, r6, asr #1
	mov r1, #0x14
	add r0, sp, #0x4c
	mla r2, r3, r1, r0
	mov r0, sl
	mov r1, sl
	add r3, sp, #0x18
	str sb, [sp]
	bl BN_mod_mul_reciprocal
	cmp r0, #0
	beq _02238204
	add r0, r7, #1
	ldr r5, [sp, #0x14]
	subs r8, r8, r0
	bpl _022380EC
_022381FC:
	mov r0, #1
	str r0, [sp, #8]
_02238204:
	ldr r0, [sp, #0xc]
	ldr r1, [sb]
	cmp r0, #0
	sub r0, r1, #1
	str r0, [sb]
	mov r4, #0
	ble _02238240
	add r5, sp, #0x4c
_02238224:
	mov r0, r5
	bl BN_clear_free
	ldr r0, [sp, #0xc]
	add r4, r4, #1
	cmp r4, r0
	add r5, r5, #0x14
	blt _02238224
_02238240:
	add r0, sp, #0x18
	bl BN_RECP_CTX_free
	ldr r0, [sp, #8]
	add sp, sp, #0x18c
	ldmia sp!, {r4, r5, r6, r7, r8, sb, sl, fp, lr}
	bx lr
	arm_func_end BN_mod_exp_recp

	arm_func_start bn_div_words
bn_div_words: ; 0x02238258
	stmdb sp!, {r4, r5, r6, r7, r8, sb, sl, fp, lr}
	sub sp, sp, #4
	movs r8, r2
	mov sl, r0
	mov sb, r1
	mov fp, #0
	mov r5, #2
	addeq sp, sp, #4
	mvneq r0, #0
	ldmeqia sp!, {r4, r5, r6, r7, r8, sb, sl, fp, lr}
	bxeq lr
	mov r0, r8
	bl BN_num_bits_word
	cmp r0, #0x20
	beq _022382AC
	mov r1, #1
	cmp sl, r1, lsl r0
	addhi sp, sp, #4
	movhi r0, fp
	ldmhiia sp!, {r4, r5, r6, r7, r8, sb, sl, fp, lr}
	bxhi lr
_022382AC:
	cmp sl, r8
	rsb r2, r0, #0x20
	subhs sl, sl, r8
	cmp r2, #0
	movne r1, sl, lsl r2
	rsbne r0, r2, #0x20
	orrne sl, r1, sb, lsr r0
	movne r8, r8, lsl r2
	ldr r4, _02238394 ; =0x0000FFFF
	movne sb, sb, lsl r2
	and r7, r4, r8, lsr #16
	and r6, r8, r4
_022382DC:
	mov r0, sl, lsr #0x10
	cmp r0, r7
	moveq r0, r4
	beq _022382F8
	mov r0, sl
	mov r1, r7
	bl _u32_div_f
_022382F8:
	mul ip, r0, r7
	mul r3, r0, r6
	and r2, r4, sb, lsr #16
_02238304:
	mov lr, #0x10000
	sub r1, sl, ip
	rsb lr, lr, #0
	ands lr, r1, lr
	bne _02238330
	add r1, r2, r1, lsl #16
	cmp r3, r1
	subhi ip, ip, r7
	subhi r3, r3, r6
	subhi r0, r0, #1
	bhi _02238304
_02238330:
	mul r2, r0, r6
	and r1, r2, r4
	mul r3, r0, r7
	mov r1, r1, lsl #0x10
	cmp sb, r1
	add r2, r3, r2, lsr #16
	addlo r2, r2, #1
	cmp sl, r2
	addlo sl, sl, r8
	sub sb, sb, r1
	sublo r0, r0, #1
	sub r1, sl, r2
	subs r5, r5, #1
	beq _02238384
	and r2, r0, r4
	mov r1, r1, lsl #0x10
	and r0, sb, r4
	orr sl, r1, sb, lsr #16
	mov fp, r2, lsl #0x10
	mov sb, r0, lsl #0x10
	b _022382DC
_02238384:
	orr r0, fp, r0
	add sp, sp, #4
	ldmia sp!, {r4, r5, r6, r7, r8, sb, sl, fp, lr}
	bx lr
	.balign 4, 0
_02238394: .word 0x0000FFFF
	arm_func_end bn_div_words

	arm_func_start BN_reciprocal
BN_reciprocal: ; 0x02238398
	stmdb sp!, {r4, r5, r6, r7, r8, lr}
	sub sp, sp, #0x18
	mov r8, r0
	add r0, sp, #4
	mov r7, r1
	mov r6, r2
	mov r5, r3
	mvn r4, #0
	bl BN_init
	add r0, sp, #4
	mov r1, #0
	bl BN_set_word
	add r0, sp, #4
	mov r1, r6
	bl BN_set_bit
	cmp r0, #0
	beq _022383FC
	add r2, sp, #4
	mov r0, r8
	mov r3, r7
	mov r1, #0
	str r5, [sp]
	bl BN_div
	cmp r0, #0
	movne r4, r6
_022383FC:
	add r0, sp, #4
	bl BN_free
	mov r0, r4
	add sp, sp, #0x18
	ldmia sp!, {r4, r5, r6, r7, r8, lr}
	bx lr
	arm_func_end BN_reciprocal

	arm_func_start BN_div_recp
BN_div_recp: ; 0x02238414
	stmdb sp!, {r4, r5, r6, r7, r8, sb, sl, fp, lr}
	sub sp, sp, #0xc
	ldr r6, [sp, #0x30]
	mov sb, r1
	ldr fp, [r6]
	movs sl, r0
	add r0, fp, #1
	str r0, [r6]
	ldr r7, [r6]
	add r4, r6, #4
	mov r0, #0x14
	mla r5, fp, r0, r4
	add ip, r7, #1
	mov r1, #0
	str r5, [sp, #4]
	mla r5, r7, r0, r4
	str ip, [r6]
	str r1, [sp]
	ldreq r1, [r6]
	mov r8, r2
	mlaeq sl, r1, r0, r4
	addeq r0, r1, #1
	streq r0, [r6]
	mov r7, r3
	cmp sb, #0
	bne _02238494
	ldr r2, [r6]
	add r1, r6, #4
	mov r0, #0x14
	mla sb, r2, r0, r1
	add r0, r2, #1
	str r0, [r6]
_02238494:
	mov r0, r8
	mov r1, r7
	bl BN_ucmp
	cmp r0, #0
	bge _022384D4
	mov r0, sl
	mov r1, #0
	bl BN_set_word
	mov r0, sb
	mov r1, r8
	bl BN_copy
	add sp, sp, #0xc
	str fp, [r6]
	mov r0, #1
	ldmia sp!, {r4, r5, r6, r7, r8, sb, sl, fp, lr}
	bx lr
_022384D4:
	mov r0, r8
	bl BN_num_bits
	ldr r1, [r7, #0x28]
	mov r2, r0
	mov r0, r1, lsl #1
	cmp r0, r2
	movgt r2, r0
	suble r0, r2, r0
	movgt r4, #0
	addle r0, r0, r0, lsr #31
	movle r4, r0, asr #1
	ldr r1, [r7, #0x2c]
	add r0, r2, r2, lsr #31
	mov r0, r0, asr #1
	cmp r2, r1
	str r0, [sp, #8]
	beq _0223852C
	mov r1, r7
	mov r3, r6
	add r0, r7, #0x14
	bl BN_reciprocal
	str r0, [r7, #0x2c]
_0223852C:
	ldr r2, [sp, #8]
	ldr r0, [sp, #4]
	mov r1, r8
	sub r2, r2, r4
	bl BN_rshift
	cmp r0, #0
	beq _0223867C
	ldr r1, [sp, #4]
	mov r0, r5
	mov r3, r6
	add r2, r7, #0x14
	bl BN_mul
	cmp r0, #0
	beq _0223867C
	ldr r2, [sp, #8]
	mov r0, sl
	mov r1, r5
	add r2, r2, r4
	bl BN_rshift
	cmp r0, #0
	beq _0223867C
	mov r4, #0
	mov r0, r5
	mov r1, r7
	mov r2, sl
	mov r3, r6
	str r4, [sl, #0xc]
	bl BN_mul
	cmp r0, #0
	beq _0223867C
	mov r0, sb
	mov r1, r8
	mov r2, r5
	bl BN_usub
	cmp r0, #0
	beq _0223867C
	mov r5, r4
	mov r0, sb
	mov r1, r7
	str r5, [sb, #0xc]
	bl BN_ucmp
	cmp r0, #0
	blt _02238628
	mov r4, #1
_022385DC:
	cmp r5, #2
	add r5, r5, #1
	bgt _0223867C
	mov r0, sb
	mov r1, sb
	mov r2, r7
	bl BN_usub
	cmp r0, #0
	beq _0223867C
	mov r0, sl
	mov r1, r4
	bl BN_add_word
	cmp r0, #0
	beq _0223867C
	mov r0, sb
	mov r1, r7
	bl BN_ucmp
	cmp r0, #0
	bge _022385DC
_02238628:
	ldr r0, [sb, #4]
	mov r1, #1
	cmp r0, #0
	beq _02238654
	cmp r0, #1
	bne _02238650
	ldr r0, [sb]
	ldr r0, [r0]
	cmp r0, #0
	beq _02238654
_02238650:
	mov r1, #0
_02238654:
	cmp r1, #0
	movne r0, #0
	ldreq r0, [r8, #0xc]
	str r0, [sb, #0xc]
	mov r0, #1
	ldr r2, [r8, #0xc]
	ldr r1, [r7, #0xc]
	str r0, [sp]
	eor r0, r2, r1
	str r0, [sl, #0xc]
_0223867C:
	ldr r0, [sp]
	str fp, [r6]
	add sp, sp, #0xc
	ldmia sp!, {r4, r5, r6, r7, r8, sb, sl, fp, lr}
	bx lr
	arm_func_end BN_div_recp

	arm_func_start BN_mod_mul_reciprocal
BN_mod_mul_reciprocal: ; 0x02238690
	stmdb sp!, {r4, r5, r6, r7, r8, lr}
	sub sp, sp, #8
	ldr r4, [sp, #0x20]
	mov ip, #0x14
	ldr r5, [r4]
	add lr, r4, #4
	mla r6, r5, ip, lr
	add ip, r5, #1
	mov r8, r0
	mov r7, r3
	str ip, [r4]
	cmp r2, #0
	mov r5, #0
	beq _02238700
	cmp r1, r2
	bne _022386E8
	mov r0, r6
	mov r2, r4
	bl BN_sqr
	cmp r0, #0
	bne _02238704
	b _02238720
_022386E8:
	mov r0, r6
	mov r3, r4
	bl BN_mul
	cmp r0, #0
	bne _02238704
	b _02238720
_02238700:
	mov r6, r1
_02238704:
	mov r1, r8
	mov r2, r6
	mov r3, r7
	mov r0, #0
	str r4, [sp]
	bl BN_div_recp
	mov r5, #1
_02238720:
	ldr r1, [r4]
	mov r0, r5
	sub r1, r1, #1
	str r1, [r4]
	add sp, sp, #8
	ldmia sp!, {r4, r5, r6, r7, r8, lr}
	bx lr
	arm_func_end BN_mod_mul_reciprocal
