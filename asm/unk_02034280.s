	.include "asm/macros.inc"
	.include "unk_02033AE0.inc"
	.include "global.inc"
	.text
	thumb_func_start Comm_GetCommandBuffer
Comm_GetCommandBuffer: ; 0x02034280
	push {r3, r4, r5, lr}
	add r3, r0, #0
	cmp r3, #0x16
	bge _02034298
	mov r4, #0xc
	mul r4, r3
	ldr r3, _020342B0 ; =gCommBaseCommands + 8
	add r0, r1, #0
	ldr r3, [r3, r4]
	mov r1, #0
	blx r3
	pop {r3, r4, r5, pc}
_02034298:
	ldr r0, _020342B4 ; =gCommCommandTable
	sub r3, #0x16
	ldr r4, [r0]
	add r0, r1, #0
	ldr r1, [r4, #8]
	ldr r5, [r4]
	mov r4, #0xc
	mul r4, r3
	add r3, r5, r4
	ldr r3, [r3, #8]
	blx r3
	pop {r3, r4, r5, pc}
	.balign 4, 0
_020342B0: .word gCommBaseCommands + 8
_020342B4: .word gCommCommandTable
	thumb_func_end Comm_GetCommandBuffer
