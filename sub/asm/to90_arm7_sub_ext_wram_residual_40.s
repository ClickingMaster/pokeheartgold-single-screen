	.include "asm/macros.inc"
	.include "global.inc"

	.extern test_pattern
	.extern test_reg

.text

	arm_func_start DiagMacRegister
DiagMacRegister: ; 0x0600BEE0
	stmdb sp!, {r4, r5, r6, lr}
	mov ip, #0
	ldr r0, _0600C074 ; =test_reg
	ldr r6, _0600C078 ; =test_pattern
	mov r3, ip
	mov r1, ip
	b _0600BF54
_0600BEFC:
	mov r4, r3, lsl #1
	ldrh r5, [r6, r4]
	mov r2, r1
	b _0600BF48
_0600BF0C:
	mov lr, r2, lsl #2
	add r4, r0, r2, lsl #2
	ldrh lr, [r0, lr]
	ldrh r4, [r4, #2]
	add lr, lr, #0x4800000
	and r4, r5, r4
	add lr, lr, #0x8000
	strh r4, [lr]
	ldrh lr, [lr]
	cmp lr, r4
	beq _0600BF44
	cmp ip, #0x20
	add ip, ip, #1
	bhi _0600C04C
_0600BF44:
	add r2, r2, #1
_0600BF48:
	cmp r2, #0x1b
	blo _0600BF0C
	add r3, r3, #1
_0600BF54:
	cmp r3, #3
	blo _0600BEFC
	ldr r4, _0600C07C ; =0x00001234
	ldr r3, _0600C074 ; =test_reg
	mov r5, #0
_0600BF68:
	mov r0, r5, lsl #2
	add r1, r3, r5, lsl #2
	ldrh r0, [r3, r0]
	ldrh r2, [r1, #2]
	add r1, r4, #0x234
	add r0, r0, #0x4800000
	add r1, r1, #0x1000
	add r5, r5, #1
	and r2, r4, r2
	add r0, r0, #0x8000
	mov r1, r1, lsl #0x10
	strh r2, [r0]
	cmp r5, #0x1b
	mov r4, r1, lsr #0x10
	blo _0600BF68
	ldr r5, _0600C07C ; =0x00001234
	ldr r3, _0600C074 ; =test_reg
	mov r4, #0
	b _0600BFFC
_0600BFB4:
	mov r0, r4, lsl #2
	ldrh r0, [r3, r0]
	add r1, r3, r4, lsl #2
	add r0, r0, #0x4800000
	add r0, r0, #0x8000
	ldrh r1, [r1, #2]
	ldrh r2, [r0]
	and r0, r5, r1
	cmp r2, r0
	beq _0600BFE8
	cmp ip, #0x20
	add ip, ip, #1
	bhi _0600C04C
_0600BFE8:
	add r0, r5, #0x234
	add r0, r0, #0x1000
	mov r0, r0, lsl #0x10
	mov r5, r0, lsr #0x10
	add r4, r4, #1
_0600BFFC:
	cmp r4, #0x1b
	blo _0600BFB4
	mov r3, #0
	ldr r2, _0600C074 ; =test_reg
	mov r1, r3
	b _0600C044
_0600C014:
	mov r0, r3, lsl #2
	ldrh r0, [r2, r0]
	add r0, r0, #0x4800000
	add r0, r0, #0x8000
	strh r1, [r0]
	ldrh r0, [r0]
	cmp r0, #0
	beq _0600C040
	cmp ip, #0x20
	add ip, ip, #1
	bhi _0600C04C
_0600C040:
	add r3, r3, #1
_0600C044:
	cmp r3, #0x1b
	blo _0600C014
_0600C04C:
	cmp ip, #0
	beq _0600C06C
	ldr r0, _0600C080 ; =0x0380FFF4
	ldr r0, [r0]
	add r0, r0, #0x300
	ldrh r1, [r0, #0x3e]
	orr r1, r1, #1
	strh r1, [r0, #0x3e]
_0600C06C:
	ldmia sp!, {r4, r5, r6, lr}
	bx lr
	.align 2, 0
_0600C074: .word test_reg
_0600C078: .word test_pattern
_0600C07C: .word 0x00001234
_0600C080: .word 0x0380FFF4
	arm_func_end DiagMacRegister
