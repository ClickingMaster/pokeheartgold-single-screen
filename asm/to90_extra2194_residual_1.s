	.extern ov18_021E5900
	.extern ov18_021E5904
	.extern ov18_021E5908
	.extern ov18_021E590C
	.extern ov18_021E595C
	.extern ov18_021E59A8
	.extern ov18_021E613C
	.extern ov18_021E6D10
	.extern ov18_021E7698
	.extern ov18_021E8AB0
	.extern ov18_021E8ACC
	.extern ov18_021E8AE0
	.extern ov18_021E8B0C
	.extern ov18_021E8B18
	.extern ov18_021E8B24
	.extern ov18_021E8B5C
	.extern ov18_021F1294
	.extern ov18_021F12C8
	.extern ov18_021FA5CC
	.include "asm/macros.inc"
	.include "overlay_18.inc"
	.include "global.inc"

.text
	thumb_func_start ov18_021F504C
ov18_021F504C: ; 0x021F504C
	push {r3, r4, r5, lr}
	add r5, r0, #0
	add r4, r1, #0
	str r5, [r4]
	mov r0, #0
	str r0, [r4, #4]
	strb r0, [r4, #0xb]
	ldr r0, [r5]
	ldr r0, [r0, #4]
	bl PlayerProfile_GetTrainerGender
	cmp r0, #0
	bne _021F506C
	mov r0, #0x19
	lsl r0, r0, #4
	b _021F506E
_021F506C:
	ldr r0, _021F50B4 ; =0x0000019A
_021F506E:
	ldr r1, _021F50B8 ; =0x0000184C
	ldr r2, [r5, r1]
	add r1, #0x56
	ldrh r1, [r5, r1]
	lsl r1, r1, #2
	ldr r1, [r2, r1]
	cmp r1, r0
	blo _021F5086
	add r2, r0, #0
	add r0, r1, #0
	mov r1, #0
	b _021F508A
_021F5086:
	add r2, r1, #0
	mov r1, #1
_021F508A:
	strb r1, [r4, #0xa]
	sub r0, r0, r2
	mov r1, #0xa
	bl _u32_div_f
	ldr r3, _021F50BC ; =ov18_021FA5CC
	mov r2, #0
_021F5098:
	ldrh r1, [r3]
	cmp r0, r1
	blo _021F50A8
	ldrh r1, [r3, #2]
	cmp r0, r1
	bhi _021F50A8
	strh r2, [r4, #8]
	pop {r3, r4, r5, pc}
_021F50A8:
	add r2, r2, #1
	add r3, r3, #4
	cmp r2, #0x11
	blo _021F5098
	pop {r3, r4, r5, pc}
	nop
_021F50B4: .word 0x0000019A
_021F50B8: .word 0x0000184C
_021F50BC: .word ov18_021FA5CC
	thumb_func_end ov18_021F504C
