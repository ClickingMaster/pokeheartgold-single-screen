	.include "asm/macros.inc"
	.include "overlay_03.inc"
	.include "global.inc"

	.text

	thumb_func_start ov03_02255388
ov03_02255388: ; 0x02255388
	push {r3, r4, lr}
	sub sp, #0x54
	ldr r3, _022553C0 ; =ov03_022591D6
	add r4, r1, #0
	add r2, sp, #0
	mov r1, #0x29
_02255394:
	ldrh r0, [r3]
	add r3, r3, #2
	strh r0, [r2]
	add r2, r2, #2
	sub r1, r1, #1
	bne _02255394
	add r0, r4, #0
	bl ov03_02254C9C
	add r4, #0x91
	ldrb r0, [r4]
	lsl r1, r0, #1
	add r0, sp, #0
	ldrh r0, [r0, r1]
	mov r1, #0
	bl ov03_02253E20
	ldr r0, _022553C4 ; =ov03_022553C8
	bl ov03_02254B4C
	add sp, #0x54
	pop {r3, r4, pc}
	.balign 4, 0
_022553C0: .word ov03_022591D6
_022553C4: .word ov03_022553C8
	thumb_func_end ov03_02255388
	