	.include "asm/macros.inc"
	.include "overlay_03.inc"
	.include "global.inc"

	.text

	thumb_func_start ov03_02254D24
ov03_02254D24: ; 0x02254D24
	push {r4, r5, lr}
	sub sp, #0x54
	ldr r5, _02254D60 ; =ov03_02259370
	add r4, r1, #0
	add r3, sp, #0
	mov r2, #0x29
_02254D30:
	ldrh r1, [r5]
	add r5, r5, #2
	strh r1, [r3]
	add r3, r3, #2
	sub r2, r2, #1
	bne _02254D30
	bl sub_02034818
	add r2, r0, #0
	beq _02254D4C
	ldr r0, [r4, #0x58]
	mov r1, #1
	bl BufferPlayersName
_02254D4C:
	add r4, #0x91
	ldrb r0, [r4]
	lsl r1, r0, #1
	add r0, sp, #0
	ldrh r0, [r0, r1]
	mov r1, #1
	bl ov03_02253E20
	add sp, #0x54
	pop {r4, r5, pc}
	.balign 4, 0
_02254D60: .word ov03_02259370
	thumb_func_end ov03_02254D24
	