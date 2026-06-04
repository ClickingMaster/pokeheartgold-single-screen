	.include "asm/macros.inc"
	.include "overlay_03.inc"
	.include "global.inc"

	.text

	thumb_func_start ov03_0225558C
ov03_0225558C: ; 0x0225558C
	push {r4, r5, lr}
	sub sp, #0x54
	ldr r3, _022555E8 ; =ov03_022593C2
	add r4, r1, #0
	add r5, r0, #0
	add r2, sp, #0
	mov r1, #0x29
_0225559A:
	ldrh r0, [r3]
	add r3, r3, #2
	strh r0, [r2]
	add r2, r2, #2
	sub r1, r1, #1
	bne _0225559A
	add r0, r4, #0
	bl ov03_02254C9C
	bl sub_020347A0
	cmp r0, #1
	ble _022555CE
	add r4, #0x91
	ldrb r0, [r4]
	lsl r1, r0, #1
	add r0, sp, #0
	ldrh r0, [r0, r1]
	mov r1, #0
	bl ov03_02253E20
	ldr r0, _022555EC ; =ov03_022555F4
	bl ov03_02254B4C
	add sp, #0x54
	pop {r4, r5, pc}
_022555CE:
	bl ov03_02254B44
	add r0, r5, #0
	add r1, r4, #0
	bl ov03_0225574C
	ldr r0, _022555F0 ; =ov03_022598A0
	mov r1, #1
	ldr r0, [r0]
	add r0, #0x88
	str r1, [r0]
	add sp, #0x54
	pop {r4, r5, pc}
	.balign 4, 0
_022555E8: .word ov03_022593C2
_022555EC: .word ov03_022555F4
_022555F0: .word ov03_022598A0
	thumb_func_end ov03_0225558C
	