	.include "asm/macros.inc"
	.include "overlay_18.inc"
	.include "global.inc"
	.public ov18_021F14FC
	.public ov18_021F1160

	.text
	thumb_func_start ov18_021F1598
ov18_021F1598: ; 0x021F1598
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #8
	add r5, r0, #0
	mov r0, #0x67
	lsl r0, r0, #4
	add r6, r2, #0
	add r7, r5, r0
	lsl r0, r6, #2
	str r0, [sp]
	add r4, r1, #0
	ldr r0, [r7, r0]
	mov r1, #0
	bl ManagedSprite_SetDrawFlag
	ldr r0, _021F1618 ; =0x00001030
	lsl r4, r4, #2
	add r0, r5, r0
	ldrh r1, [r0, r4]
	str r0, [sp, #4]
	cmp r1, #0
	beq _021F1614
	ldr r0, [r5]
	mov r2, #0
	ldr r0, [r0]
	bl Pokedex_GetSeenFormByIdx
	add r2, r0, #0
	ldr r0, [sp, #4]
	ldrh r1, [r0, r4]
	cmp r1, #0xac
	bne _021F15E0
	cmp r2, #2
	bne _021F15DE
	mov r2, #1
	b _021F15E0
_021F15DE:
	mov r2, #0
_021F15E0:
	add r0, r5, #0
	add r3, r6, #0
	bl ov18_021F14FC
	ldr r0, [sp]
	mov r1, #1
	ldr r0, [r7, r0]
	bl ManagedSprite_SetDrawFlag
	ldr r0, _021F161C ; =0x00001032
	add r1, r5, r4
	ldrh r0, [r1, r0]
	cmp r0, #1
	bne _021F160A
	add r0, r5, #0
	add r1, r6, #0
	mov r2, #1
	bl ov18_021F1160
	add sp, #8
	pop {r3, r4, r5, r6, r7, pc}
_021F160A:
	add r0, r5, #0
	add r1, r6, #0
	mov r2, #0
	bl ov18_021F1160
_021F1614:
	add sp, #8
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
_021F1618: .word 0x00001030
_021F161C: .word 0x00001032
	thumb_func_end ov18_021F1598
