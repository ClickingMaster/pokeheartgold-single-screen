	.extern ov85_021E8530
	.include "asm/macros.inc"
	.include "overlay_85.inc"
	.include "global.inc"

.text
	thumb_func_start ov85_021E7E88
ov85_021E7E88: ; 0x021E7E88
	push {r4, lr}
	ldr r2, _021E7ECC ; =0x00000B54
	mov r3, #0
	add r0, r0, r2
	add r4, r0, #0
_021E7E92:
	ldr r2, [r4]
	cmp r2, #1
	bne _021E7EA4
	ldr r2, [r4, #0x10]
	cmp r2, r1
	bne _021E7EA4
	mov r0, #0
	str r0, [r4, #4]
	pop {r4, pc}
_021E7EA4:
	add r3, r3, #1
	add r4, #0x14
	cmp r3, #5
	blt _021E7E92
	mov r3, #0
_021E7EAE:
	ldr r2, [r0]
	cmp r2, #0
	bne _021E7EBC
	mov r2, #1
	str r2, [r0]
	str r1, [r0, #0x10]
	pop {r4, pc}
_021E7EBC:
	add r3, r3, #1
	add r0, #0x14
	cmp r3, #5
	blt _021E7EAE
	bl GF_AssertFail
	pop {r4, pc}
	nop
_021E7ECC: .word 0x00000B54
	thumb_func_end ov85_021E7E88
