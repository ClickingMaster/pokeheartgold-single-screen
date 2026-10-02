	.include "asm/macros.inc"
	.include "unk_0201010C.inc"
	.include "global.inc"

.text
	thumb_func_start sub_02010C38
sub_02010C38: ; 0x02010C38
	push {r4, r5, r6, lr}
	add r4, r0, #0
	bne _02010C42
	bl GF_AssertFail
_02010C42:
	ldr r0, _02010E54 ; =0x04000006
	ldrh r0, [r0]
	cmp r0, #0xc0
	bge _02010CE8
	add r5, r0, #1
	cmp r5, #0xbf
	ble _02010C52
	sub r5, #0xc0
_02010C52:
	ldr r0, [r4, #4]
	cmp r0, #1
	bne _02010D02
	add r0, r4, #0
	mov r1, #0
	bl sub_02010EE0
	ldr r3, [r4, #8]
	lsl r5, r5, #1
	mov r4, #6
	add r1, r0, r5
	lsl r4, r4, #6
	ldrsh r2, [r1, r4]
	lsl r4, r4, #2
	ldrsh r1, [r0, r5]
	ldr r0, [r0, r4]
	cmp r0, #0
	ldr r0, _02010E58 ; =0x04000004
	bne _02010CBC
	cmp r3, #0
	bne _02010C9C
	ldrh r4, [r0]
	mov r3, #2
	tst r3, r4
	beq _02010CE8
	lsl r3, r1, #8
	mov r1, #0xff
	lsl r1, r1, #8
	and r3, r1
	lsl r1, r2, #0x18
	lsr r1, r1, #0x18
	orr r1, r3
	strh r1, [r0, #0x3c]
	mov r1, #0xc0
	add r0, #0x40
	strh r1, [r0]
	pop {r4, r5, r6, pc}
_02010C9C:
	ldrh r3, [r0]
	mov r0, #2
	tst r0, r3
	beq _02010CE8
	mov r0, #0xff
	lsl r1, r1, #8
	lsl r0, r0, #8
	and r1, r0
	lsl r0, r2, #0x18
	lsr r0, r0, #0x18
	orr r0, r1
	ldr r1, _02010E5C ; =0x04001040
	strh r0, [r1]
	mov r0, #0xc0
	strh r0, [r1, #4]
	pop {r4, r5, r6, pc}
_02010CBC:
	cmp r3, #0
	bne _02010CE0
	ldrh r4, [r0]
	mov r3, #2
	tst r3, r4
	beq _02010CE8
	lsl r3, r1, #8
	mov r1, #0xff
	lsl r1, r1, #8
	and r3, r1
	lsl r1, r2, #0x18
	lsr r1, r1, #0x18
	orr r1, r3
	strh r1, [r0, #0x3e]
	mov r1, #0xc0
	add r0, #0x42
	strh r1, [r0]
	pop {r4, r5, r6, pc}
_02010CE0:
	ldrh r3, [r0]
	mov r0, #2
	tst r0, r3
	bne _02010CEA
_02010CE8:
	b _02010E50
_02010CEA:
	mov r0, #0xff
	lsl r1, r1, #8
	lsl r0, r0, #8
	and r1, r0
	lsl r0, r2, #0x18
	lsr r0, r0, #0x18
	orr r0, r1
	ldr r1, _02010E60 ; =0x04001042
	strh r0, [r1]
	mov r0, #0xc0
	strh r0, [r1, #4]
	pop {r4, r5, r6, pc}
_02010D02:
	add r0, r4, #0
	mov r1, #0
	bl sub_02010EE0
	add r3, r0, #0
	lsl r5, r5, #1
	mov r6, #6
	add r1, r3, r5
	lsl r6, r6, #6
	ldrsh r2, [r1, r6]
	lsl r6, r6, #2
	ldrsh r1, [r3, r5]
	ldr r3, [r3, r6]
	ldr r0, [r4, #8]
	cmp r3, #0
	bne _02010D68
	cmp r0, #0
	ldr r0, _02010E58 ; =0x04000004
	bne _02010D48
	ldrh r6, [r0]
	mov r3, #2
	tst r3, r6
	beq _02010DAC
	lsl r3, r1, #8
	mov r1, #0xff
	lsl r1, r1, #8
	and r3, r1
	lsl r1, r2, #0x18
	lsr r1, r1, #0x18
	orr r1, r3
	strh r1, [r0, #0x3c]
	mov r1, #0xc0
	add r0, #0x40
	strh r1, [r0]
	b _02010DAC
_02010D48:
	ldrh r3, [r0]
	mov r0, #2
	tst r0, r3
	beq _02010DAC
	mov r0, #0xff
	lsl r1, r1, #8
	lsl r0, r0, #8
	and r1, r0
	lsl r0, r2, #0x18
	lsr r0, r0, #0x18
	orr r0, r1
	ldr r1, _02010E5C ; =0x04001040
	strh r0, [r1]
	mov r0, #0xc0
	strh r0, [r1, #4]
	b _02010DAC
_02010D68:
	cmp r0, #0
	ldr r0, _02010E58 ; =0x04000004
	bne _02010D8E
	ldrh r6, [r0]
	mov r3, #2
	tst r3, r6
	beq _02010DAC
	lsl r3, r1, #8
	mov r1, #0xff
	lsl r1, r1, #8
	and r3, r1
	lsl r1, r2, #0x18
	lsr r1, r1, #0x18
	orr r1, r3
	strh r1, [r0, #0x3e]
	mov r1, #0xc0
	add r0, #0x42
	strh r1, [r0]
	b _02010DAC
_02010D8E:
	ldrh r3, [r0]
	mov r0, #2
	tst r0, r3
	beq _02010DAC
	mov r0, #0xff
	lsl r1, r1, #8
	lsl r0, r0, #8
	and r1, r0
	lsl r0, r2, #0x18
	lsr r0, r0, #0x18
	orr r0, r1
	ldr r1, _02010E60 ; =0x04001042
	strh r0, [r1]
	mov r0, #0xc0
	strh r0, [r1, #4]
_02010DAC:
	add r0, r4, #0
	mov r1, #1
	bl sub_02010EE0
	ldr r3, [r4, #8]
	mov r4, #6
	add r1, r0, r5
	lsl r4, r4, #6
	ldrsh r2, [r1, r4]
	lsl r4, r4, #2
	ldrsh r1, [r0, r5]
	ldr r0, [r0, r4]
	cmp r0, #0
	ldr r0, _02010E58 ; =0x04000004
	bne _02010E0E
	cmp r3, #0
	bne _02010DEE
	ldrh r4, [r0]
	mov r3, #2
	tst r3, r4
	beq _02010E50
	lsl r3, r1, #8
	mov r1, #0xff
	lsl r1, r1, #8
	and r3, r1
	lsl r1, r2, #0x18
	lsr r1, r1, #0x18
	orr r1, r3
	strh r1, [r0, #0x3c]
	mov r1, #0xc0
	add r0, #0x40
	strh r1, [r0]
	pop {r4, r5, r6, pc}
_02010DEE:
	ldrh r3, [r0]
	mov r0, #2
	tst r0, r3
	beq _02010E50
	mov r0, #0xff
	lsl r1, r1, #8
	lsl r0, r0, #8
	and r1, r0
	lsl r0, r2, #0x18
	lsr r0, r0, #0x18
	orr r0, r1
	ldr r1, _02010E5C ; =0x04001040
	strh r0, [r1]
	mov r0, #0xc0
	strh r0, [r1, #4]
	pop {r4, r5, r6, pc}
_02010E0E:
	cmp r3, #0
	bne _02010E32
	ldrh r4, [r0]
	mov r3, #2
	tst r3, r4
	beq _02010E50
	lsl r3, r1, #8
	mov r1, #0xff
	lsl r1, r1, #8
	and r3, r1
	lsl r1, r2, #0x18
	lsr r1, r1, #0x18
	orr r1, r3
	strh r1, [r0, #0x3e]
	mov r1, #0xc0
	add r0, #0x42
	strh r1, [r0]
	pop {r4, r5, r6, pc}
_02010E32:
	ldrh r3, [r0]
	mov r0, #2
	tst r0, r3
	beq _02010E50
	mov r0, #0xff
	lsl r1, r1, #8
	lsl r0, r0, #8
	and r1, r0
	lsl r0, r2, #0x18
	lsr r0, r0, #0x18
	orr r0, r1
	ldr r1, _02010E60 ; =0x04001042
	strh r0, [r1]
	mov r0, #0xc0
	strh r0, [r1, #4]
_02010E50:
	pop {r4, r5, r6, pc}
	nop
_02010E54: .word 0x04000006
_02010E58: .word 0x04000004
_02010E5C: .word 0x04001040
_02010E60: .word 0x04001042
	thumb_func_end sub_02010C38
