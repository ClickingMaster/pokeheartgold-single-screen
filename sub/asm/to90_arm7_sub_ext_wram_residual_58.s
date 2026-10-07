	.include "asm/macros.inc"
	.include "global.inc"

	.extern AllocateHeapBuf
	.extern InitManHeader
	.extern IsEnableManagement
	.extern SetFatalErr
	.extern SetSSIDElement
	.extern SetSupRateSet
	.extern WL_ReadByte
	.extern WL_WriteByte

.text

	arm_func_start MakeProbeResFrame
MakeProbeResFrame: ; 0x06014970
	stmdb sp!, {r4, r5, r6, r7, r8, sb, sl, lr}
	ldr r1, _06014B84 ; =0x0380FFF4
	mov r5, r0
	ldr r0, [r1]
	add r4, r0, #0x344
	bl IsEnableManagement
	cmp r0, #0
	moveq r0, #0
	beq _06014B7C
	ldr r1, _06014B84 ; =0x0380FFF4
	ldrh r0, [r4, #0xa0]
	ldr r2, [r1]
	add r1, r0, #0x78
	add r0, r2, #0x188
	bl AllocateHeapBuf
	movs r6, r0
	bne _060149C4
	mov r0, #2
	bl SetFatalErr
	mov r0, r6
	b _06014B7C
_060149C4:
	ldr r0, _06014B88 ; =0x0000FFFF
	mov r1, r5
	strh r0, [r6, #0xc]
	add r0, r6, #0x10
	bl InitManHeader
	ldrh r1, [r4, #0x6e]
	add r0, r6, #0x48
	strh r1, [r6, #0x44]
	ldrh r1, [r4, #0x7c]
	strh r1, [r6, #0x46]
	bl SetSSIDElement
	mov r4, r0
	add r0, r6, #0x48
	add r0, r0, r4
	bl SetSupRateSet
	add r7, r4, r0
	add r0, r6, #0x48
	add r4, r0, r7
	mov r0, r4
	mov r1, #3
	bl WL_WriteByte
	add r0, r4, #1
	mov r1, #1
	bl WL_WriteByte
	ldr r1, _06014B84 ; =0x0380FFF4
	add r0, r4, #2
	ldr r1, [r1]
	add r1, r1, #0x300
	ldrh r1, [r1, #0xbe]
	and r1, r1, #0xff
	bl WL_WriteByte
	ldr r0, _06014B84 ; =0x0380FFF4
	add r7, r7, #3
	ldr r1, [r0]
	add r0, r6, #0x48
	add r5, r0, r7
	add r4, r1, #0x344
	add r8, r1, #0x31c
	mov r0, r5
	mov r1, #0xdd
	mov sl, #0
	bl WL_WriteByte
	ldr r1, _06014B84 ; =0x0380FFF4
	add r0, r5, #1
	ldr r1, [r1]
	add r1, r1, #0x300
	ldrh r1, [r1, #0xe4]
	add r1, r1, #8
	and r1, r1, #0xff
	bl WL_WriteByte
	add r0, r5, #2
	mov r1, sl
	bl WL_WriteByte
	add r0, r5, #3
	mov r1, #9
	bl WL_WriteByte
	add r0, r5, #4
	mov r1, #0xbf
	bl WL_WriteByte
	add r0, r5, #5
	mov r1, sl
	bl WL_WriteByte
	ldrh r1, [r8, #0x20]
	add r0, r5, #6
	and r1, r1, #0xff
	bl WL_WriteByte
	ldrh r1, [r8, #0x20]
	add r0, r5, #7
	mov r1, r1, asr #8
	and r1, r1, #0xff
	bl WL_WriteByte
	ldr r1, _06014B8C ; =0x0380FFF0
	add r0, r5, #8
	ldrh r8, [r1]
	and r1, r8, #0xff
	bl WL_WriteByte
	mov r1, r8, lsr #8
	add r0, r5, #9
	and r1, r1, #0xff
	bl WL_WriteByte
	ldrh r0, [r4, #0xa0]
	add sl, sl, #0xa
	cmp r0, #0
	beq _06014B58
	ldrh r0, [r4, #0xa2]
	ldr r8, [r4, #0x9c]
	tst r0, #1
	addne r8, r8, #1
	mov sb, #0
	b _06014B4C
_06014B2C:
	mov r0, r8
	bl WL_ReadByte
	mov r1, r0
	add r0, r5, sl
	bl WL_WriteByte
	add sl, sl, #1
	add r8, r8, #1
	add sb, sb, #1
_06014B4C:
	ldrh r0, [r4, #0xa0]
	cmp sb, r0
	blo _06014B2C
_06014B58:
	add r0, r7, sl
	add r0, r0, #0xc
	strh r0, [r6, #0x16]
	ldrh r1, [r6, #0x16]
	mov r0, #0x50
	add r1, r1, #0x1c
	strh r1, [r6, #0x22]
	strh r0, [r6, #0x24]
	add r0, r6, #0x10
_06014B7C:
	ldmia sp!, {r4, r5, r6, r7, r8, sb, sl, lr}
	bx lr
	.align 2, 0
_06014B84: .word 0x0380FFF4
_06014B88: .word 0x0000FFFF
_06014B8C: .word 0x0380FFF0
	arm_func_end MakeProbeResFrame
