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
	.extern BN_init
	.extern BN_is_bit_set
	.extern BN_lshift
	.extern BN_mod
	.extern BN_mod_exp
	.extern BN_mod_exp_mont
	.extern BN_mod_exp_recp
	.extern BN_mod_mul_reciprocal
	.extern BN_mul
	.extern BN_new
	.extern BN_num_bits
	.extern BN_num_bits_word
	.extern BN_reciprocal
	.extern BN_rshift
	.extern BN_set_bit
	.extern BN_set_word
	.extern BN_sub
	.extern BN_sub_word
	.extern BN_uadd
	.extern BN_ucmp
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
	.extern bn_mul_add_words
	.extern bn_mul_normal
	.extern bn_mul_words
	.extern bn_sqr_words
	.extern bn_sub_words
	.extern bn_zexpand
	.extern shift_val


	arm_func_start BN_MONT_CTX_set_word
BN_MONT_CTX_set_word: ; 0x0223888C
	stmdb sp!, {r4, r5, r6, r7, lr}
	sub sp, sp, #0x14
	mov r6, r1
	ldr r3, [r6, #4]
	mov r7, r0
	cmp r3, #0
	mov r5, r2
	addeq sp, sp, #0x14
	moveq r0, #0
	ldmeqia sp!, {r4, r5, r6, r7, lr}
	bxeq lr
	add r0, r7, #0x20
	add r4, r7, #0xc
	bl BN_copy
	cmp r0, #0
	addeq sp, sp, #0x14
	moveq r0, #0
	ldmeqia sp!, {r4, r5, r6, r7, lr}
	bxeq lr
	add r0, sp, #0
	bl BN_init
	mov r1, #1
	mov r0, r6
	str r1, [r7]
	bl BN_num_bits
	add r1, r0, #0x1f
	mov r0, r1, asr #4
	add r0, r1, r0, lsr #27
	mov r2, r0, asr #5
	mov r0, r4
	mov r1, #0
	str r2, [r7, #8]
	bl BN_set_word
	cmp r0, #0
	addeq sp, sp, #0x14
	moveq r0, #0
	ldmeqia sp!, {r4, r5, r6, r7, lr}
	bxeq lr
	mov r0, r4
	mov r1, #0x20
	bl BN_set_bit
	cmp r0, #0
	beq _02238A30
	ldr r0, [r6]
	ldr r4, [r0]
	mov r0, r4
	bl BN_mod_inverse_word
	mov r1, r0
	add r0, sp, #0
	bl BN_set_word
	cmp r0, #0
	beq _02238A30
	add r0, sp, #0
	mov r2, #0x20
	mov r1, r0
	bl BN_lshift
	cmp r0, #0
	beq _02238A30
	ldr r0, [sp, #4]
	cmp r0, #0
	beq _022389A8
	cmp r0, #1
	bne _02238998
	ldr r0, [sp]
	ldr r0, [r0]
	cmp r0, #0
	beq _022389A8
_02238998:
	add r0, sp, #0
	mov r1, #1
	bl BN_sub_word
	b _022389BC
_022389A8:
	add r0, sp, #0
	mvn r1, #0
	bl BN_set_word
	cmp r0, #0
	beq _02238A30
_022389BC:
	ldr r2, [sp, #4]
	cmp r2, #1
	ldrge r0, [sp]
	ldrge r1, [r0]
	movlt r1, #0
	cmp r2, #2
	ldrge r0, [sp]
	mov r2, r4
	ldrge r0, [r0, #4]
	movlt r0, #0
	bl bn_div_words
	str r0, [r7, #0x48]
	add r0, r7, #0xc
	mov r1, #0
	bl BN_set_word
	ldr r1, [r7, #8]
	add r0, r7, #0xc
	mov r1, r1, lsl #6
	bl BN_set_bit
	cmp r0, #0
	beq _02238A30
	add r0, r7, #0xc
	mov r1, r0
	mov r3, r5
	add r2, r7, #0x20
	bl BN_mod
	ldr r1, [r7, #8]
	add r0, r7, #0xc
	bl bn_zexpand
_02238A30:
	add r0, sp, #0
	bl BN_free
	mov r0, #1
	add sp, sp, #0x14
	ldmia sp!, {r4, r5, r6, r7, lr}
	bx lr
	arm_func_end BN_MONT_CTX_set_word

	arm_func_start BN_gen_exp_string
BN_gen_exp_string: ; 0x02238A48
	stmdb sp!, {r4, r5, r6, r7, r8, sb, sl, fp, lr}
	sub sp, sp, #0xc
	mov r4, r2
	mov r7, r1
	cmp r4, #6
	mov r5, r0
	movgt r4, #6
	mov r1, #1
	mov r2, r1, lsl r4
	ldr r0, [r7, #4]
	sub r2, r2, #1
	add r0, r4, r0, lsl #5
	ldr r3, _02238C1C ; =shift_val
	str r2, [sp]
	ldr r2, [r3, r4, lsl #2]
	mov r1, r4
	sub r0, r0, #1
	str r2, [sp, #4]
	mov r6, #0
	bl _s32_div_f
	mov ip, #0
	mov r0, r0, lsl #1
	add r0, r0, #2
	mov r8, r6
	add r1, r5, r0
	strb r8, [r5, r0]
	strb r8, [r1, #-1]
	ldr r0, [r7]
	ldr r2, [r7, #4]
	ldr r7, [r0]
	add lr, r0, #4
	cmp r2, #1
	sub sb, r1, #2
	ldrgt r8, [lr], #4
	mov sl, r7
	mov r0, ip
	str ip, [sp, #8]
	mov r1, #0xff
	mov fp, ip
_02238AE4:
	ldr r3, [sp]
	and r3, sl, r3
	ldr sl, [sp, #4]
	ldrb sl, [sl, r3]
	cmp sl, #0
	beq _02238B44
	add ip, ip, sl
	add r6, r6, sl
	cmp r6, #0x20
	blo _02238B2C
	cmp r2, #1
	ble _02238B44
	sub r2, r2, #1
	mov r7, r8
	cmp r2, #1
	movle r8, fp
	ldrgt r8, [lr], #4
	sub r6, r6, #0x20
_02238B2C:
	cmp r6, #0
	moveq sl, r7
	movne sl, r7, lsr r6
	rsbne r3, r6, #0x20
	orrne sl, sl, r8, lsl r3
	b _02238AE4
_02238B44:
	cmp r3, #0
	beq _02238BC8
	strb ip, [sb]
	strb r3, [sb, #-1]
	cmp ip, #0x100
	sub sb, sb, #2
	blo _02238B80
	cmp ip, #0x100
	blo _02238B80
_02238B68:
	strb r1, [sb]
	sub ip, ip, #0x100
	strb r0, [sb, #-1]
	sub sb, sb, #2
	cmp ip, #0x100
	bhs _02238B68
_02238B80:
	mov ip, r4
	add r6, r6, r4
	cmp r6, #0x20
	blo _02238BB0
	cmp r2, #1
	ble _02238BC8
	sub r2, r2, #1
	mov r7, r8
	cmp r2, #1
	ldrle r8, [sp, #8]
	sub r6, r6, #0x20
	ldrgt r8, [lr], #4
_02238BB0:
	cmp r6, #0
	moveq sl, r7
	movne sl, r7, lsr r6
	rsbne r3, r6, #0x20
	orrne sl, sl, r8, lsl r3
	b _02238AE4
_02238BC8:
	add sb, sb, #1
	mov r0, #2
	b _02238BEC
_02238BD4:
	strb r2, [r5]
	ldrb r1, [sb, #1]
	add sb, sb, #2
	add r0, r0, #2
	strb r1, [r5, #1]
	add r5, r5, #2
_02238BEC:
	ldrb r2, [sb]
	cmp r2, #0
	bne _02238BD4
	ldrb r1, [sb, #1]
	cmp r1, #0
	bne _02238BD4
	mov r1, #0
	strb r1, [r5]
	strb r1, [r5, #1]
	add sp, sp, #0xc
	ldmia sp!, {r4, r5, r6, r7, r8, sb, sl, fp, lr}
	bx lr
	.balign 4, 0
_02238C1C: .word shift_val
	arm_func_end BN_gen_exp_string

	arm_func_start BN_gen_exp_bits
BN_gen_exp_bits: ; 0x02238C20
	stmdb sp!, {r4, r5, r6, r7, r8, sb, lr}
	sub sp, sp, #4
	ldr r5, [r3], #4
	mov r2, #0x14
	mla r4, r5, r2, r3
	mov sb, r0
	ldr r0, [sb, #4]
	mov r8, r1
	cmp r0, #0
	mov r5, #0
	mov r1, r0, lsl #5
	addeq sp, sp, #4
	moveq r0, r5
	ldmeqia sp!, {r4, r5, r6, r7, r8, sb, lr}
	bxeq lr
	cmp r0, #1
	bne _02238CC0
	cmp r0, #1
	bne _02238C84
	ldr r2, [sb]
	ldr r1, _02238D84 ; =0x00010001
	ldr r2, [r2]
	cmp r2, r1
	ldreq r5, _02238D88 ; =TEMPNAME__$p16
	beq _02238CB0
_02238C84:
	ldr r1, [sb]
	ldr r1, [r1]
	cmp r1, #0x11
	bne _02238CA0
	cmp r0, #1
	ldreq r5, _02238D8C ; =TEMPNAME__$p4
	beq _02238CB0
_02238CA0:
	cmp r1, #3
	bne _02238CB0
	cmp r0, #1
	ldreq r5, _02238D90 ; =TEMPNAME__$p2
_02238CB0:
	mov r7, #1
	mov r6, r7
	mov r1, #0x20
	b _02238CF0
_02238CC0:
	cmp r1, #0x100
	movge r7, #5
	movge r6, #0x10
	movge r1, #7
	bge _02238CF0
	cmp r1, #0x80
	movge r6, #8
	movge r1, r6
	movge r7, #4
	movlt r7, #3
	movlt r6, #4
	movlt r1, #0xb
_02238CF0:
	mul r1, r0, r1
	mov r0, r1, lsl #1
	add r1, r0, #7
	mov r0, r1, asr #1
	add r0, r1, r0, lsr #30
	cmp r5, #0
	mov r1, r0, asr #2
	bne _02238D6C
	ldr r0, [r4, #8]
	cmp r1, r0
	movle r0, r4
	ble _02238D28
	mov r0, r4
	bl bn_expand2
_02238D28:
	cmp r0, #0
	addeq sp, sp, #4
	moveq r0, #0
	ldmeqia sp!, {r4, r5, r6, r7, r8, sb, lr}
	bxeq lr
	ldr r5, [r4]
	mov r1, sb
	mov r2, r7
	add r0, r5, #4
	bl BN_gen_exp_string
	add r1, r0, #2
	mov r0, r1, asr #8
	strb r0, [r5]
	strb r1, [r5, #1]
	strb r7, [r5, #2]
	strb r6, [r5, #3]
	b _02238D70
_02238D6C:
	mov r1, #8
_02238D70:
	str r5, [r8]
	add r0, r1, #2
	add sp, sp, #4
	ldmia sp!, {r4, r5, r6, r7, r8, sb, lr}
	bx lr
	.balign 4, 0
_02238D84: .word 0x00010001
_02238D88: .word TEMPNAME__$p16
_02238D8C: .word TEMPNAME__$p4
_02238D90: .word TEMPNAME__$p2
	arm_func_end BN_gen_exp_bits

	arm_func_start BN_mod_inverse_word
BN_mod_inverse_word: ; 0x02238D94
	stmdb sp!, {r4, r5, r6, r7, r8, sb, sl, lr}
	mov sl, r0
	mov r1, sl
	rsb r0, sl, #0
	bl _u32_div_f
	movs r8, r1
	mov r7, sl
	mov r5, #0
	mov r6, #1
	mvn r4, #0
	beq _02238DFC
_02238DC0:
	mov r0, r7
	mov r1, r8
	bl _u32_div_f
	mov sb, r1
	mov r0, r7
	mov r1, r8
	bl _u32_div_f
	mla r1, r0, r6, r5
	mov r5, r6
	mov r7, r8
	mov r6, r1
	mov r8, sb
	cmp sb, #0
	rsb r4, r4, #0
	bne _02238DC0
_02238DFC:
	cmp r4, #0
	sublt r5, sl, r5
	cmp r7, #1
	movne r1, #0
	bne _02238E1C
	mov r0, r5
	mov r1, sl
	bl _u32_div_f
_02238E1C:
	mov r0, r1
	ldmia sp!, {r4, r5, r6, r7, r8, sb, sl, lr}
	bx lr
	arm_func_end BN_mod_inverse_word

	arm_func_start bn_from_montgomery_words
bn_from_montgomery_words: ; 0x02238E28
	stmdb sp!, {r4, r5, r6, r7, r8, sb, sl, fp, lr}
	sub sp, sp, #0xc
	mov r7, r3
	mov sb, r1
	mov r6, #0
	mov sl, r0
	mov r8, r2
	mov r4, r6
	cmp r7, #0
	add r5, sb, r7, lsl #2
	ldr fp, [sp, #0x30]
	ble _02238EB8
	mov r0, #1
	str r6, [sp, #4]
	str r0, [sp]
_02238E64:
	ldr r1, [sb]
	mov r0, sb
	mul r3, r1, fp
	mov r1, r8
	mov r2, r7
	bl bn_mul_add_words
	add r1, r0, r6
	ldr r0, [r5]
	cmp r1, r6
	ldrlo r6, [sp]
	add r0, r0, r1
	str r0, [r5]
	ldr r0, [r5]
	ldrhs r6, [sp, #4]
	cmp r0, r1
	add r4, r4, #1
	addlo r6, r6, #1
	cmp r4, r7
	add sb, sb, #4
	add r5, r5, #4
	blt _02238E64
_02238EB8:
	cmp r6, #0
	sub r2, r7, #1
	bne _02238F0C
	ldr r1, [sb, r2, lsl #2]
	ldr r0, [r8, r2, lsl #2]
	cmp r1, r0
	bne _02238EF8
	cmp r2, #0
	ble _02238EF8
_02238EDC:
	ldr r1, [sb, r2, lsl #2]
	ldr r0, [r8, r2, lsl #2]
	cmp r1, r0
	bne _02238EF8
	sub r2, r2, #1
	cmp r2, #0
	bgt _02238EDC
_02238EF8:
	ldr r1, [sb, r2, lsl #2]
	ldr r0, [r8, r2, lsl #2]
	cmp r1, r0
	movhs r6, #1
	movlo r6, #0
_02238F0C:
	cmp r6, #0
	beq _02238F34
	mov r0, sl
	mov r1, sb
	mov r2, r8
	mov r3, r7
	bl bn_sub_words
	add sp, sp, #0xc
	ldmia sp!, {r4, r5, r6, r7, r8, sb, sl, fp, lr}
	bx lr
_02238F34:
	cmp r7, #0
	addle sp, sp, #0xc
	mov r1, #0
	ldmleia sp!, {r4, r5, r6, r7, r8, sb, sl, fp, lr}
	bxle lr
_02238F48:
	ldr r0, [sb, r1, lsl #2]
	str r0, [sl, r1, lsl #2]
	add r1, r1, #1
	cmp r1, r7
	blt _02238F48
	add sp, sp, #0xc
	ldmia sp!, {r4, r5, r6, r7, r8, sb, sl, fp, lr}
	bx lr
	arm_func_end bn_from_montgomery_words

	arm_func_start bn_sqr_normal
bn_sqr_normal: ; 0x02238F68
	stmdb sp!, {r4, r5, r6, r7, r8, sb, sl, fp, lr}
	sub sp, sp, #4
	mov fp, r2
	mov r6, fp, lsl #1
	mov sl, r0
	mov r7, fp
	mov sb, r1
	sub r7, r7, #1
	sub r2, r6, #1
	mov r0, #0
	str r0, [sl, r2, lsl #2]
	ldr r0, [sl, r2, lsl #2]
	str r3, [sp]
	mov r5, sb
	str r0, [sl]
	cmp r7, #0
	add r4, sl, #4
	ble _02238FD0
	add r5, r5, #4
	ldr r3, [sb]
	mov r0, r4
	mov r1, r5
	mov r2, r7
	bl bn_mul_words
	str r0, [r4, r7, lsl #2]
	add r4, r4, #8
_02238FD0:
	sub r8, fp, #2
	cmp r8, #0
	ble _02239010
_02238FDC:
	mov r0, r5
	sub r7, r7, #1
	add r5, r5, #4
	ldr r3, [r0]
	mov r0, r4
	mov r1, r5
	mov r2, r7
	bl bn_mul_add_words
	sub r8, r8, #1
	str r0, [r4, r7, lsl #2]
	cmp r8, #0
	add r4, r4, #8
	bgt _02238FDC
_02239010:
	mov r0, sl
	mov r1, sl
	mov r2, sl
	mov r3, r6
	bl bn_add_words
	ldr r0, [sp]
	mov r1, sb
	mov r2, fp
	bl bn_sqr_words
	ldr r2, [sp]
	mov r0, sl
	mov r1, sl
	mov r3, r6
	bl bn_add_words
	add sp, sp, #4
	ldmia sp!, {r4, r5, r6, r7, r8, sb, sl, fp, lr}
	bx lr
	arm_func_end bn_sqr_normal

	arm_func_start BN_sqr
BN_sqr: ; 0x02239054
	stmdb sp!, {r4, r5, r6, r7, r8, sb, lr}
	sub sp, sp, #0x64
	ldr r5, [r2]
	mov sb, r0
	mov r8, r1
	cmp r8, sb
	add r3, r2, #4
	mov r2, #0x14
	movne r7, sb
	addeq r0, r5, #1
	mla r4, r5, r2, r3
	mlaeq r7, r0, r2, r3
	ldr r5, [r8, #4]
	cmp r5, #0
	movle r0, #0
	strle r0, [sb, #4]
	addle sp, sp, #0x64
	movle r0, #1
	ldmleia sp!, {r4, r5, r6, r7, r8, sb, lr}
	bxle lr
	ldr r0, [r7, #8]
	mov r6, r5, lsl #1
	cmp r6, r0
	movle r0, r7
	ble _022390C4
	mov r0, r7
	mov r1, r6
	bl bn_expand2
_022390C4:
	cmp r0, #0
	addeq sp, sp, #0x64
	moveq r0, #0
	ldmeqia sp!, {r4, r5, r6, r7, r8, sb, lr}
	bxeq lr
	str r6, [r7, #4]
	mov r0, #0
	str r0, [r7, #0xc]
	cmp r5, #4
	bne _02239104
	ldr r0, [r7]
	ldr r1, [r8]
	add r3, sp, #0
	mov r2, #4
	bl bn_sqr_normal
	b _02239168
_02239104:
	cmp r5, #8
	bne _02239124
	ldr r0, [r7]
	ldr r1, [r8]
	add r3, sp, #0x20
	mov r2, #8
	bl bn_sqr_normal
	b _02239168
_02239124:
	ldr r0, [r4, #8]
	cmp r6, r0
	movle r0, r4
	ble _02239140
	mov r0, r4
	mov r1, r6
	bl bn_expand2
_02239140:
	cmp r0, #0
	addeq sp, sp, #0x64
	moveq r0, #0
	ldmeqia sp!, {r4, r5, r6, r7, r8, sb, lr}
	bxeq lr
	ldr r0, [r7]
	ldr r1, [r8]
	ldr r3, [r4]
	mov r2, r5
	bl bn_sqr_normal
_02239168:
	cmp r6, #0
	ble _0223918C
	ldr r1, [r7]
	sub r0, r6, #1
	ldr r0, [r1, r0, lsl #2]
	cmp r0, #0
	ldreq r0, [r7, #4]
	subeq r0, r0, #1
	streq r0, [r7, #4]
_0223918C:
	cmp r7, sb
	beq _022391A0
	mov r0, sb
	mov r1, r7
	bl BN_copy
_022391A0:
	mov r0, #1
	add sp, sp, #0x64
	ldmia sp!, {r4, r5, r6, r7, r8, sb, lr}
	bx lr
	arm_func_end BN_sqr
