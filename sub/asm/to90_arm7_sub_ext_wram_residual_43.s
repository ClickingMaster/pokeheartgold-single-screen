	.include "asm/macros.inc"
	.include "global.inc"

	.extern SetFatalErr

.text

	arm_func_start WlIntrStartTx
WlIntrStartTx: ; 0x0600D444
	stmdb sp!, {r3, lr}
	ldr r3, _0600D540 ; =0x0380FFF4
	ldr r0, _0600D544 ; =0x04808010
	ldr r2, [r3]
	mov r1, #0x80
	strh r1, [r0]
	ldr r1, [r3]
	add r2, r2, #0x2c
	add r1, r1, #0x600
	ldrh r1, [r1, #0x90]
	tst r1, #0x20
	beq _0600D4D8
	add r1, r0, #0x204
	ldrh r3, [r1]
	add r1, r0, #0x258
	and r3, r3, #0xff
	ldrh ip, [r1]
	cmp r3, #3
	blo _0600D4D8
	cmp r3, #5
	bhi _0600D4D8
	ldr r3, [r2, #0x458]
	ldr r1, _0600D548 ; =0x00000FFF
	and r3, r1, r3, lsr #1
	cmp ip, r3
	blo _0600D4D8
	ldr r2, [r2, #0x430]
	and r1, r1, r2, lsr #1
	cmp ip, r1
	bhi _0600D4D8
	add r1, r0, #0x234
	ldrh r0, [r1]
	orr r0, r0, #0x80
	strh r0, [r1]
	ldrh r0, [r1]
	bic r0, r0, #0x80
	strh r0, [r1]
_0600D4D8:
	ldr r1, _0600D54C ; =0x04808000
	ldrh r0, [r1]
	cmp r0, #0x1440
	beq _0600D538
	add r0, r1, #0x19c
	ldrh r0, [r0]
	and r0, r0, #0x42
	cmp r0, #0x42
	bne _0600D538
	add r1, r1, #0x2b8
	ldrh r2, [r1]
	cmp r2, #0
	beq _0600D538
	mov r3, #0
	b _0600D52C
_0600D514:
	cmp r3, #0x3e8
	add r3, r3, #1
	bls _0600D52C
	mov r0, #0x40
	bl SetFatalErr
	b _0600D538
_0600D52C:
	ldrh r0, [r1]
	cmp r2, r0
	beq _0600D514
_0600D538:
	ldmia sp!, {r3, lr}
	bx lr
	.align 2, 0
_0600D540: .word 0x0380FFF4
_0600D544: .word 0x04808010
_0600D548: .word 0x00000FFF
_0600D54C: .word 0x04808000
	arm_func_end WlIntrStartTx
