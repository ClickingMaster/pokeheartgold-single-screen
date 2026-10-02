	.extern ov75_02246960
	.extern ov75_022469D8
	.extern ov75_02246B00
	.extern ov75_02246D00
	.extern ov75_02246D04
	.extern ov75_02246D08
	.extern ov75_02246D40
	.extern ov75_02246DB4
	.extern ov75_02246DFC
	.extern ov75_02246E3C
	.extern ov75_02246E78
	.extern ov75_02246EAC
	.extern ov75_02246EDC
	.extern ov75_02246F0C
	.extern ov75_02247118
	.extern ov75_02247180
	.extern ov75_022478E0
	.extern ov75_02247A78
	.extern ov75_02247A98
	.extern ov75_02247B98
	.extern ov75_022483EC
	.extern ov75_0224852C
	.extern ov75_0224854C
	.extern ov75_02248584
	.extern ov75_02248684
	.extern ov75_022486EC
	.extern ov75_02248714
	.extern ov75_0224874C
	.extern ov75_02248774
	.extern ov75_02248800
	.extern ov75_022488AC
	.extern ov75_022488BC
	.extern ov75_02248994
	.extern ov75_022489F8
	.extern ov75_02248A20
	.extern ov75_02248B8C
	.extern ov75_02248C64
	.extern ov75_02248C84
	.extern ov75_02248D2C
	.extern ov75_02248F18
	.extern ov75_02248F7C
	.extern ov75_02248FE8
	.extern ov75_022490D8
	.extern ov75_022491CC
	.extern ov75_022491F0
	.extern ov75_0224921C
	.extern ov75_02249258
	.extern ov75_02249278
	.extern ov75_0224937C
	.extern ov75_02249460
	.extern ov75_02249478
	.extern ov75_022494A4
	.extern ov75_02249550
	.extern ov75_022495B0
	.extern ov75_02249758
	.extern ov75_0224976C
	.include "asm/macros.inc"
	.include "overlay_75.inc"
	.include "global.inc"

.text
	thumb_func_start ov75_02249884
ov75_02249884: ; 0x02249884
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #8
	mov r4, #0
	add r5, r0, #0
	add r7, r1, #0
	str r4, [sp, #4]
_02249890:
	mov r0, #0
	str r0, [r7]
	str r0, [sp]
	b _022498CC
_02249898:
	cmp r0, #0
	bne _022498A4
	bne _022498B2
	bl GF_AssertFail
	b _022498B2
_022498A4:
	add r4, r4, #1
_022498A6:
	ldrsb r0, [r5, r4]
	add r6, r5, r4
	cmp r0, #0x30
	blt _02249898
	cmp r0, #0x39
	bgt _02249898
_022498B2:
	ldr r1, [r7]
	mov r0, #0xa
	mul r0, r1
	str r0, [r7]
	mov r1, #0
	ldrsb r1, [r6, r1]
	add r4, r4, #1
	sub r1, #0x30
	add r0, r0, r1
	str r0, [r7]
	ldr r0, [sp]
	add r0, r0, #1
	str r0, [sp]
_022498CC:
	ldr r0, [sp]
	cmp r0, #4
	blt _022498A6
	ldr r0, [sp, #4]
	add r7, r7, #4
	add r0, r0, #1
	str r0, [sp, #4]
	cmp r0, #4
	blt _02249890
	add sp, #8
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
	thumb_func_end ov75_02249884
