	.extern ov90_0225886C
	.include "asm/macros.inc"
	.include "overlay_90.inc"
	.include "global.inc"

.text
	thumb_func_start ov90_022588CC
ov90_022588CC: ; 0x022588CC
	push {r3, r4, r5, lr}
	add r4, r1, #0
	add r5, r0, #0
	cmp r4, #4
	blo _022588E0
	blo _022588DC
	bl GF_AssertFail
_022588DC:
	mov r0, #0
	pop {r3, r4, r5, pc}
_022588E0:
	ldrb r2, [r5, #0xb]
	cmp r2, #1
	bne _02258900
	bl ov90_0225886C
	ldrb r1, [r5, #9]
	cmp r0, r1
	bne _022588F8
	ldr r0, [r5]
	bl Save_PlayerData_GetProfile
	pop {r3, r4, r5, pc}
_022588F8:
	add r0, r4, #0
	bl sub_02034818
	pop {r3, r4, r5, pc}
_02258900:
	ldr r0, [r5, #0xc]
	cmp r0, #0
	bne _0225890A
	bl GF_AssertFail
_0225890A:
	ldr r1, [r5, #0xc]
	lsl r0, r4, #2
	add r0, r1, r0
	ldr r0, [r0, #0x20]
	pop {r3, r4, r5, pc}
	thumb_func_end ov90_022588CC
