	.include "asm/macros.inc"
	.include "overlay_41.inc"
	.include "global.inc"

.text
	thumb_func_start ov41_02249978
ov41_02249978: ; 0x02249978
	push {r3, r4}
	ldr r4, [sp, #8]
	sub r4, r2, r4
	bmi _02249982
	b _02249984
_02249982:
	mov r4, #0
_02249984:
	strb r4, [r0]
	ldr r4, [sp, #8]
	add r2, r2, r4
	cmp r2, #0xbf
	bgt _02249990
	b _02249992
_02249990:
	mov r2, #0xbf
_02249992:
	strb r2, [r0, #1]
	sub r2, r1, r3
	bmi _0224999A
	b _0224999C
_0224999A:
	mov r2, #0
_0224999C:
	add r1, r1, r3
	strb r2, [r0, #2]
	cmp r1, #0xff
	bgt _022499AA
	strb r1, [r0, #3]
	pop {r3, r4}
	bx lr
_022499AA:
	mov r1, #0xff
	strb r1, [r0, #3]
	pop {r3, r4}
	bx lr
	.balign 4, 0
	thumb_func_end ov41_02249978
