	.extern GetVariablePtr
	.include "asm/macros.inc"
	.include "global.inc"

	.extern ChannelCallback
	.extern ClosePlayerTrack
	.extern FreeTrackChannelAll
	.extern GetPlayerTrack
	.extern InitCache
	.extern Read24
	.extern ReadArg
	.extern ReadByte
	.extern ReleaseTrackChannelAll
	.extern SND_AllocExChannel
	.extern SND_CalcRandom
	.extern SND_FreeExChannel
	.extern SND_NoteOn
	.extern SND_ReadInstData
	.extern SND_ReleaseExChannel
	.extern SND_SetExChannelAttack
	.extern SND_SetExChannelDecay
	.extern SND_SetExChannelRelease
	.extern SND_SetExChannelSustain
	.extern SNDi_DecibelTable
	.extern SNDi_SharedWork
	.extern SetTrackMute
	.extern StartTrack
	.extern _s32_div_f
	.extern sMmlPrintEnable

.text

	arm_func_start UpdateTrackChannel
UpdateTrackChannel: ; 0x037FDB30
	stmdb sp!, {r3, r4, r5, r6, r7, r8, sb, sl, fp, lr}
	mov sl, r0
	ldrb r0, [sl, #7]
	ldrb r6, [sl, #4]
	ldrb r3, [sl, #5]
	ldrb fp, [r1, #5]
	mov r5, r0, lsl #6
	mov r8, r3, lsl #1
	ldrb r4, [sl, #1]
	ldrsb r7, [sl, #6]
	mov sb, r6, lsl #1
	ldr r0, _037FDC84 ; =SNDi_DecibelTable
	mul r6, r7, r5
	mov r3, fp, lsl #1
	ldrsh sb, [r0, sb]
	ldrsh r7, [r0, r8]
	ldrsh r0, [r0, r3]
	add r8, sb, r7
	ldrsb r3, [sl, #8]
	cmp r4, #0x7f
	mulne r4, r3, r4
	addne r3, r4, #0x40
	mov r4, #0x8000
	ldrsh r5, [sl, #0xc]
	mov sb, r2
	add r2, r5, r6, asr #7
	ldrsh r7, [sl, #0xa]
	ldrsh r1, [r1, #6]
	ldrsb r5, [sl, #9]
	movne r3, r3, asr #7
	add r0, r0, r8
	rsb r4, r4, #0
	cmp r0, r4
	movlt r0, r4
	mov r4, #0x8000
	add r1, r7, r1
	rsb r4, r4, #0
	cmp r1, r4
	movlt r1, r4
	add r3, r3, r5
	mvn r4, #0x7f
	cmp r3, r4
	movlt r3, r4
	blt _037FDBE8
	cmp r3, #0x7f
	movgt r3, #0x7f
_037FDBE8:
	mov r0, r0, lsl #0x10
	mov r2, r2, lsl #0x10
	mov r3, r3, lsl #0x18
	mov r1, r1, lsl #0x10
	ldr r8, [sl, #0x3c]
	mov r6, r0, asr #0x10
	mov r5, r2, asr #0x10
	mov r4, r3, asr #0x18
	mov r7, r1, asr #0x10
	mov fp, #1
	b _037FDC74
_037FDC14:
	strh r7, [r8, #6]
	ldrb r0, [r8, #2]
	cmp r0, #3
	beq _037FDC70
	strh r6, [r8, #0xc]
	strh r5, [r8, #0xe]
	strb r4, [r8, #0xb]
	ldrb r0, [sl, #1]
	strb r0, [r8, #4]
	ldrh r0, [sl, #0x18]
	strh r0, [r8, #0x28]
	ldrh r0, [sl, #0x1a]
	strh r0, [r8, #0x2a]
	ldrh r0, [sl, #0x1c]
	strh r0, [r8, #0x2c]
	ldr r0, [r8, #0x34]
	cmp r0, #0
	bne _037FDC70
	cmp sb, #0
	beq _037FDC70
	mov r0, r8
	strb fp, [r8, #0x22]
	bl SND_ReleaseExChannel
_037FDC70:
	ldr r8, [r8, #0x50]
_037FDC74:
	cmp r8, #0
	bne _037FDC14
	ldmia sp!, {r3, r4, r5, r6, r7, r8, sb, sl, fp, lr}
	bx lr
	.align 2, 0
_037FDC84: .word SNDi_DecibelTable
	arm_func_end UpdateTrackChannel

	arm_func_start PlayerSeqMain
PlayerSeqMain: ; 0x037FDC88
	stmdb sp!, {r4, r5, r6, r7, r8, sb, sl, fp, lr}
	sub sp, sp, #0x24
	mov r2, #0
	str r2, [sp, #0x10]
	str r1, [sp, #8]
	mov r7, r0
	mov sl, r2
_037FDCA4:
	mov r0, r7
	mov r1, sl
	bl GetPlayerTrack
	movs r8, r0
	ldrne r0, [r8, #0x28]
	cmpne r0, #0
	beq _037FE7A4
	ldr r0, [r8, #0x3c]
	b _037FDD00
_037FDCC8:
	ldr r1, [r0, #0x34]
	cmp r1, #0
	subgt r1, r1, #1
	strgt r1, [r0, #0x34]
	ldrb r1, [r0, #3]
	mov r1, r1, lsl #0x1d
	movs r1, r1, lsr #0x1f
	bne _037FDCFC
	ldr r2, [r0, #0x14]
	ldr r1, [r0, #0x18]
	cmp r2, r1
	addlt r1, r2, #1
	strlt r1, [r0, #0x14]
_037FDCFC:
	ldr r0, [r0, #0x50]
_037FDD00:
	cmp r0, #0
	bne _037FDCC8
	ldrb r1, [r8]
	mov r0, r1, lsl #0x1b
	movs r0, r0, lsr #0x1f
	beq _037FDD30
	ldr r0, [r8, #0x3c]
	cmp r0, #0
	movne r0, #0
	bne _037FE788
	bic r0, r1, #0x10
	strb r0, [r8]
_037FDD30:
	ldr r0, [r8, #0x20]
	cmp r0, #0
	subgt r0, r0, #1
	strgt r0, [r8, #0x20]
	cmpgt r0, #0
	movgt r0, #0
	bgt _037FE788
	ldr r0, [r8, #0x28]
	bl InitCache
	ldr r0, _037FE7CC ; =0x0000FFFF
	sub fp, r0, #0x10000
	b _037FE768
_037FDD60:
	mov r0, r8
	mov sb, #0
	mov r4, #1
	bl ReadByte
	mov r6, r0
	cmp r6, #0xa2
	bne _037FDD94
	mov r0, r8
	bl ReadByte
	mov r6, r0
	ldrb r0, [r8]
	mov r0, r0, lsl #0x19
	mov r4, r0, lsr #0x1f
_037FDD94:
	cmp r6, #0xa0
	bne _037FDDB0
	mov r0, r8
	bl ReadByte
	mov r6, r0
	mov r5, #3
	mov sb, #1
_037FDDB0:
	cmp r6, #0xa1
	bne _037FDDCC
	mov r0, r8
	bl ReadByte
	mov r6, r0
	mov r5, #4
	mov sb, #1
_037FDDCC:
	tst r6, #0x80
	bne _037FE050
	mov r0, r8
	bl ReadByte
	cmp sb, #0
	str r0, [sp, #0xc]
	movne r2, r5
	moveq r2, #2
	mov r0, r8
	mov r1, r7
	bl ReadArg
	cmp r4, #0
	mov r4, r0
	ldrsb r0, [r8, #0x13]
	add sb, r6, r0
	beq _037FE768
	cmp sb, #0
	movlt sb, #0
	blt _037FDE20
	cmp sb, #0x7f
	movgt sb, #0x7f
_037FDE20:
	ldrb r1, [r8]
	mov r0, r1, lsl #0x1d
	movs r0, r0, lsr #0x1f
	bne _037FE024
	ldr r0, [sp, #8]
	cmp r0, #0
	beq _037FE024
	mov r0, r1, lsl #0x1c
	mov r6, #0
	movs r0, r0, lsr #0x1f
	ldrne r6, [r8, #0x3c]
	cmpne r6, #0
	strneb sb, [r6, #8]
	ldrne r0, [sp, #0xc]
	strneb r0, [r6, #9]
	cmp r6, #0
	bne _037FDF54
	ldr r0, [r7, #0x20]
	ldrh r1, [r8, #2]
	mov r2, sb
	add r3, sp, #0x16
	bl SND_ReadInstData
	cmp r0, #0
	beq _037FE024
	ldrb r0, [sp, #0x16]
	cmp r0, #4
	addls pc, pc, r0, lsl #2
	b _037FE024
_037FDE90: ; jump table
	b _037FE024 ; case 0
	b _037FDEA4 ; case 1
	b _037FDEAC ; case 2
	b _037FDEB4 ; case 3
	b _037FDEA4 ; case 4
_037FDEA4:
	ldr r1, _037FE7CC ; =0x0000FFFF
	b _037FDEB8
_037FDEAC:
	mov r1, #0x3f00
	b _037FDEB8
_037FDEB4:
	mov r1, #0xc000
_037FDEB8:
	ldrh r0, [r8, #0x1e]
	ldr r3, _037FE7D0 ; =ChannelCallback
	str r8, [sp]
	ldrb r2, [r8]
	and r0, r1, r0
	mov r2, r2, lsl #0x18
	ldrb r6, [r7, #4]
	ldrb r1, [r8, #0x12]
	mov r2, r2, lsr #0x1f
	add r1, r6, r1
	bl SND_AllocExChannel
	movs r6, r0
	beq _037FE024
	ldrb r0, [r8]
	mov r0, r0, lsl #0x1c
	movs r0, r0, lsr #0x1f
	movne r3, fp
	bne _037FDF0C
	cmp r4, #0
	movgt r3, r4
	movle r3, fp
_037FDF0C:
	ldr r2, [sp, #0xc]
	ldr r1, [r7, #0x20]
	mov r0, r6
	str r1, [sp]
	add r1, sp, #0x16
	str r1, [sp, #4]
	mov r1, sb
	bl SND_NoteOn
	cmp r0, #0
	bne _037FDF48
	mov r0, #0
	strb r0, [r6, #0x22]
	mov r0, r6
	bl SND_FreeExChannel
	b _037FE024
_037FDF48:
	ldr r0, [r8, #0x3c]
	str r0, [r6, #0x50]
	str r6, [r8, #0x3c]
_037FDF54:
	ldrb r1, [r8, #0xe]
	cmp r1, #0xff
	beq _037FDF68
	mov r0, r6
	bl SND_SetExChannelAttack
_037FDF68:
	ldrb r1, [r8, #0xf]
	cmp r1, #0xff
	beq _037FDF7C
	mov r0, r6
	bl SND_SetExChannelDecay
_037FDF7C:
	ldrb r1, [r8, #0x10]
	cmp r1, #0xff
	beq _037FDF90
	mov r0, r6
	bl SND_SetExChannelSustain
_037FDF90:
	ldrb r1, [r8, #0x11]
	cmp r1, #0xff
	beq _037FDFA4
	mov r0, r6
	bl SND_SetExChannelRelease
_037FDFA4:
	ldrsh r0, [r8, #0x16]
	strh r0, [r6, #0x32]
	ldrb r0, [r8]
	mov r0, r0, lsl #0x1a
	movs r0, r0, lsr #0x1f
	beq _037FDFD4
	ldrsh r1, [r6, #0x32]
	ldrb r0, [r8, #0x14]
	sub r0, r0, sb
	mov r0, r0, lsl #0x16
	add r0, r1, r0, asr #16
	strh r0, [r6, #0x32]
_037FDFD4:
	ldrb r0, [r8, #0x15]
	cmp r0, #0
	bne _037FE000
	cmp r4, #0
	movgt r0, r4
	movle r0, fp
	str r0, [r6, #0x18]
	ldrb r0, [r6, #3]
	bic r0, r0, #4
	strb r0, [r6, #3]
	b _037FE01C
_037FE000:
	mul r1, r0, r0
	ldrsh r0, [r6, #0x32]
	cmp r0, #0
	rsblt r0, r0, #0
	mul r0, r1, r0
	mov r0, r0, asr #0xb
	str r0, [r6, #0x18]
_037FE01C:
	mov r0, #0
	str r0, [r6, #0x14]
_037FE024:
	strb sb, [r8, #0x14]
	ldrb r0, [r8]
	mov r0, r0, lsl #0x1e
	movs r0, r0, lsr #0x1f
	beq _037FE768
	str r4, [r8, #0x20]
	cmp r4, #0
	ldreqb r0, [r8]
	orreq r0, r0, #0x10
	streqb r0, [r8]
	b _037FE768
_037FE050:
	and r0, r6, #0xf0
	cmp r0, #0xc0
	bgt _037FE084
	bge _037FE1CC
	cmp r0, #0x90
	bgt _037FE078
	bge _037FE0F0
	cmp r0, #0x80
	beq _037FE0A8
	b _037FE768
_037FE078:
	cmp r0, #0xb0
	beq _037FE47C
	b _037FE768
_037FE084:
	cmp r0, #0xe0
	bgt _037FE09C
	bge _037FE428
	cmp r0, #0xd0
	beq _037FE1CC
	b _037FE768
_037FE09C:
	cmp r0, #0xf0
	beq _037FE6C4
	b _037FE768
_037FE0A8:
	cmp sb, #0
	movne r2, r5
	moveq r2, #2
	mov r0, r8
	mov r1, r7
	bl ReadArg
	cmp r4, #0
	beq _037FE768
	cmp r6, #0x80
	beq _037FE0DC
	cmp r6, #0x81
	beq _037FE0E4
	b _037FE768
_037FE0DC:
	str r0, [r8, #0x20]
	b _037FE768
_037FE0E4:
	cmp r0, #0x10000
	strlth r0, [r8, #2]
	b _037FE768
_037FE0F0:
	cmp r6, #0x93
	beq _037FE10C
	cmp r6, #0x94
	beq _037FE16C
	cmp r6, #0x95
	beq _037FE188
	b _037FE768
_037FE10C:
	mov r0, r8
	bl ReadByte
	mov r6, r0
	mov r0, r8
	bl Read24
	cmp r4, #0
	mov sb, r0
	beq _037FE768
	mov r1, r6
	mov r0, r7
	bl GetPlayerTrack
	movs r4, r0
	cmpne r4, r8
	beq _037FE768
	mov r1, r7
	mov r2, fp
	bl ReleaseTrackChannelAll
	mov r0, r4
	bl FreeTrackChannelAll
	mov r0, r4
	mov r2, sb
	ldr r1, [r8, #0x24]
	bl StartTrack
	b _037FE768
_037FE16C:
	mov r0, r8
	bl Read24
	cmp r4, #0
	ldrne r1, [r8, #0x24]
	addne r0, r1, r0
	strne r0, [r8, #0x28]
	b _037FE768
_037FE188:
	mov r0, r8
	bl Read24
	cmp r4, #0
	beq _037FE768
	ldrb r1, [r8, #0x3b]
	cmp r1, #3
	bhs _037FE768
	add r1, r8, r1, lsl #2
	ldr r2, [r8, #0x28]
	str r2, [r1, #0x2c]
	ldrb r1, [r8, #0x3b]
	add r1, r1, #1
	strb r1, [r8, #0x3b]
	ldr r1, [r8, #0x24]
	add r0, r1, r0
	str r0, [r8, #0x28]
	b _037FE768
_037FE1CC:
	cmp sb, #0
	movne r2, r5
	moveq r2, #0
	mov r0, r8
	mov r1, r7
	bl ReadArg
	cmp r4, #0
	strb r0, [sp, #0x14]
	beq _037FE768
	sub r1, r6, #0xc0
	cmp r1, #0x17
	addls pc, pc, r1, lsl #2
	b _037FE768
_037FE200: ; jump table
	b _037FE3F8 ; case 0
	b _037FE260 ; case 1
	b _037FE278 ; case 2
	b _037FE3E0 ; case 3
	b _037FE3EC ; case 4
	b _037FE284 ; case 5
	b _037FE290 ; case 6
	b _037FE29C ; case 7
	b _037FE35C ; case 8
	b _037FE3A4 ; case 9
	b _037FE2C4 ; case 10
	b _037FE2D0 ; case 11
	b _037FE2DC ; case 12
	b _037FE2E8 ; case 13
	b _037FE3C4 ; case 14
	b _037FE2B8 ; case 15
	b _037FE2F4 ; case 16
	b _037FE300 ; case 17
	b _037FE30C ; case 18
	b _037FE318 ; case 19
	b _037FE324 ; case 20
	b _037FE26C ; case 21
	b _037FE408 ; case 22
	b _037FE390 ; case 23
_037FE260:
	and r0, r0, #0xff
	strb r0, [r8, #4]
	b _037FE768
_037FE26C:
	and r0, r0, #0xff
	strb r0, [r8, #5]
	b _037FE768
_037FE278:
	and r0, r0, #0xff
	strb r0, [r7, #5]
	b _037FE768
_037FE284:
	and r0, r0, #0xff
	strb r0, [r8, #7]
	b _037FE768
_037FE290:
	and r0, r0, #0xff
	strb r0, [r8, #0x12]
	b _037FE768
_037FE29C:
	and r0, r0, #0xff
	mov r0, r0, lsl #0x1f
	ldrb r1, [r8]
	bic r1, r1, #2
	orr r0, r1, r0, lsr #30
	strb r0, [r8]
	b _037FE768
_037FE2B8:
	and r0, r0, #0xff
	strb r0, [r8, #0x15]
	b _037FE768
_037FE2C4:
	and r0, r0, #0xff
	strb r0, [r8, #0x1a]
	b _037FE768
_037FE2D0:
	and r0, r0, #0xff
	strb r0, [r8, #0x19]
	b _037FE768
_037FE2DC:
	and r0, r0, #0xff
	strb r0, [r8, #0x18]
	b _037FE768
_037FE2E8:
	and r0, r0, #0xff
	strb r0, [r8, #0x1b]
	b _037FE768
_037FE2F4:
	and r0, r0, #0xff
	strb r0, [r8, #0xe]
	b _037FE768
_037FE300:
	and r0, r0, #0xff
	strb r0, [r8, #0xf]
	b _037FE768
_037FE30C:
	and r0, r0, #0xff
	strb r0, [r8, #0x10]
	b _037FE768
_037FE318:
	and r0, r0, #0xff
	strb r0, [r8, #0x11]
	b _037FE768
_037FE324:
	ldrb r1, [r8, #0x3b]
	cmp r1, #3
	bhs _037FE768
	add r2, r8, r1, lsl #2
	and r1, r0, #0xff
	ldr r0, [r8, #0x28]
	str r0, [r2, #0x2c]
	ldrb r0, [r8, #0x3b]
	add r0, r8, r0
	strb r1, [r0, #0x38]
	ldrb r0, [r8, #0x3b]
	add r0, r0, #1
	strb r0, [r8, #0x3b]
	b _037FE768
_037FE35C:
	and r0, r0, #0xff
	mov r1, r0, lsl #0x1f
	ldrb r2, [r8]
	mov r0, r8
	bic r2, r2, #8
	orr r1, r2, r1, lsr #28
	strb r1, [r8]
	mov r1, r7
	mov r2, fp
	bl ReleaseTrackChannelAll
	mov r0, r8
	bl FreeTrackChannelAll
	b _037FE768
_037FE390:
	and r2, r0, #0xff
	mov r0, r8
	mov r1, r7
	bl SetTrackMute
	b _037FE768
_037FE3A4:
	and r1, r0, #0xff
	ldrsb r0, [r8, #0x13]
	add r0, r1, r0
	strb r0, [r8, #0x14]
	ldrb r0, [r8]
	orr r0, r0, #0x20
	strb r0, [r8]
	b _037FE768
_037FE3C4:
	and r0, r0, #0xff
	mov r0, r0, lsl #0x1f
	ldrb r1, [r8]
	bic r1, r1, #0x20
	orr r0, r1, r0, lsr #26
	strb r0, [r8]
	b _037FE768
_037FE3E0:
	ldrsb r0, [sp, #0x14]
	strb r0, [r8, #0x13]
	b _037FE768
_037FE3EC:
	ldrsb r0, [sp, #0x14]
	strb r0, [r8, #6]
	b _037FE768
_037FE3F8:
	and r0, r0, #0xff
	sub r0, r0, #0x40
	strb r0, [r8, #8]
	b _037FE768
_037FE408:
	ldr r1, _037FE7D4 ; =sMmlPrintEnable
	ldr r1, [r1]
	cmp r1, #0
	beq _037FE768
	and r1, r0, #0xff
	mov r0, r7
	bl GetVariablePtr
	b _037FE768
_037FE428:
	cmp sb, #0
	movne r2, r5
	moveq r2, #1
	mov r0, r8
	mov r1, r7
	bl ReadArg
	mov r0, r0, lsl #0x10
	cmp r4, #0
	mov r0, r0, asr #0x10
	beq _037FE768
	cmp r6, #0xe0
	beq _037FE474
	cmp r6, #0xe1
	beq _037FE46C
	cmp r6, #0xe3
	streqh r0, [r8, #0x16]
	b _037FE768
_037FE46C:
	strh r0, [r7, #0x18]
	b _037FE768
_037FE474:
	strh r0, [r8, #0x1c]
	b _037FE768
_037FE47C:
	mov r0, r8
	bl ReadByte
	cmp sb, #0
	movne r2, r5
	mov sb, r0
	moveq r2, #1
	mov r0, r8
	mov r1, r7
	bl ReadArg
	mov r0, r0, lsl #0x10
	mov r1, sb
	mov sb, r0, asr #0x10
	mov r0, r7
	bl GetVariablePtr
	cmp r4, #0
	mov r4, r0
	cmpne r4, #0
	beq _037FE768
	sub r0, r6, #0xb0
	cmp r0, #0xd
	addls pc, pc, r0, lsl #2
	b _037FE768
_037FE4D4: ; jump table
	b _037FE50C ; case 0
	b _037FE514 ; case 1
	b _037FE524 ; case 2
	b _037FE534 ; case 3
	b _037FE544 ; case 4
	b _037FE560 ; case 5
	b _037FE584 ; case 6
	b _037FE768 ; case 7
	b _037FE5BC ; case 8
	b _037FE5E8 ; case 9
	b _037FE614 ; case 10
	b _037FE640 ; case 11
	b _037FE66C ; case 12
	b _037FE698 ; case 13
_037FE50C:
	strh sb, [r4]
	b _037FE768
_037FE514:
	ldrsh r0, [r4]
	add r0, r0, sb
	strh r0, [r4]
	b _037FE768
_037FE524:
	ldrsh r0, [r4]
	sub r0, r0, sb
	strh r0, [r4]
	b _037FE768
_037FE534:
	ldrsh r0, [r4]
	mul r1, r0, sb
	strh r1, [r4]
	b _037FE768
_037FE544:
	cmp sb, #0
	beq _037FE768
	mov r1, sb
	ldrsh r0, [r4]
	bl _s32_div_f
	strh r0, [r4]
	b _037FE768
_037FE560:
	cmp sb, #0
	ldrgesh r0, [r4]
	movge r0, r0, lsl sb
	strgeh r0, [r4]
	ldrltsh r1, [r4]
	rsblt r0, sb, #0
	movlt r0, r1, asr r0
	strlth r0, [r4]
	b _037FE768
_037FE584:
	cmp sb, #0
	rsblt r0, sb, #0
	movlt r0, r0, lsl #0x10
	mov r6, #0
	movlt sb, r0, asr #0x10
	movlt r6, #1
	bl SND_CalcRandom
	add r1, sb, #1
	mul r1, r0, r1
	mov r0, r1, asr #0x10
	cmp r6, #0
	rsbne r0, r0, #0
	strh r0, [r4]
	b _037FE768
_037FE5BC:
	ldrsh r0, [r4]
	ldrb r1, [r8]
	cmp r0, sb
	moveq r0, #1
	movne r0, #0
	and r0, r0, #0xff
	mov r0, r0, lsl #0x1f
	bic r1, r1, #0x40
	orr r0, r1, r0, lsr #25
	strb r0, [r8]
	b _037FE768
_037FE5E8:
	ldrsh r0, [r4]
	ldrb r1, [r8]
	cmp r0, sb
	movge r0, #1
	movlt r0, #0
	and r0, r0, #0xff
	mov r0, r0, lsl #0x1f
	bic r1, r1, #0x40
	orr r0, r1, r0, lsr #25
	strb r0, [r8]
	b _037FE768
_037FE614:
	ldrsh r0, [r4]
	ldrb r1, [r8]
	cmp r0, sb
	movgt r0, #1
	movle r0, #0
	and r0, r0, #0xff
	mov r0, r0, lsl #0x1f
	bic r1, r1, #0x40
	orr r0, r1, r0, lsr #25
	strb r0, [r8]
	b _037FE768
_037FE640:
	ldrsh r0, [r4]
	ldrb r1, [r8]
	cmp r0, sb
	movle r0, #1
	movgt r0, #0
	and r0, r0, #0xff
	mov r0, r0, lsl #0x1f
	bic r1, r1, #0x40
	orr r0, r1, r0, lsr #25
	strb r0, [r8]
	b _037FE768
_037FE66C:
	ldrsh r0, [r4]
	ldrb r1, [r8]
	cmp r0, sb
	movlt r0, #1
	movge r0, #0
	and r0, r0, #0xff
	mov r0, r0, lsl #0x1f
	bic r1, r1, #0x40
	orr r0, r1, r0, lsr #25
	strb r0, [r8]
	b _037FE768
_037FE698:
	ldrsh r0, [r4]
	ldrb r1, [r8]
	cmp r0, sb
	movne r0, #1
	moveq r0, #0
	and r0, r0, #0xff
	mov r0, r0, lsl #0x1f
	bic r1, r1, #0x40
	orr r0, r1, r0, lsr #25
	strb r0, [r8]
	b _037FE768
_037FE6C4:
	cmp r4, #0
	beq _037FE768
	sub r0, r6, #0xfc
	cmp r0, #3
	addls pc, pc, r0, lsl #2
	b _037FE768
_037FE6DC: ; jump table
	b _037FE714 ; case 0
	b _037FE6EC ; case 1
	b _037FE768 ; case 2
	b _037FE760 ; case 3
_037FE6EC:
	ldrb r0, [r8, #0x3b]
	cmp r0, #0
	beq _037FE768
	sub r0, r0, #1
	strb r0, [r8, #0x3b]
	and r0, r0, #0xff
	add r0, r8, r0, lsl #2
	ldr r0, [r0, #0x2c]
	str r0, [r8, #0x28]
	b _037FE768
_037FE714:
	ldrb r0, [r8, #0x3b]
	cmp r0, #0
	beq _037FE768
	add r1, r8, r0
	ldrb r0, [r1, #0x37]
	cmp r0, #0
	beq _037FE748
	sub r0, r0, #1
	ands r0, r0, #0xff
	ldreqb r0, [r8, #0x3b]
	subeq r0, r0, #1
	streqb r0, [r8, #0x3b]
	beq _037FE768
_037FE748:
	strb r0, [r1, #0x37]
	ldrb r0, [r8, #0x3b]
	add r0, r8, r0, lsl #2
	ldr r0, [r0, #0x28]
	str r0, [r8, #0x28]
	b _037FE768
_037FE760:
	mvn r0, #0
	b _037FE788
_037FE768:
	ldr r0, [r8, #0x20]
	cmp r0, #0
	bne _037FE784
	ldrb r0, [r8]
	mov r0, r0, lsl #0x1b
	movs r0, r0, lsr #0x1f
	beq _037FDD60
_037FE784:
	mov r0, #0
_037FE788:
	cmp r0, #0
	moveq r0, #1
	streq r0, [sp, #0x10]
	beq _037FE7A4
	mov r0, r7
	mov r1, sl
	bl ClosePlayerTrack
_037FE7A4:
	add sl, sl, #1
	cmp sl, #0x10
	blt _037FDCA4
	ldr r0, [sp, #0x10]
	cmp r0, #0
	moveq r0, #1
	movne r0, #0
	add sp, sp, #0x24
	ldmia sp!, {r4, r5, r6, r7, r8, sb, sl, fp, lr}
	bx lr
	.align 2, 0
_037FE7CC: .word 0x0000FFFF
_037FE7D0: .word ChannelCallback
_037FE7D4: .word sMmlPrintEnable
	arm_func_end PlayerSeqMain
