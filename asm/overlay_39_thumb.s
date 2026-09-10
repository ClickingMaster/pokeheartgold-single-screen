#include "config.h"
	.include "asm/macros.inc"
	.include "overlay_39_thumb.inc"
	.include "global.inc"

	.text
	.public _0222AB80
	.public ov39_02227088
	.public ov39_022271CC
	.public ov39_02227208
	.public ov39_022272EC
	.public ov39_022273B0
	.public ov39_02227420
	.public ov39_022274D4
	.public ov39_02227534
	.public ov39_02227590
	.public ov39_022275E8
	.public ov39_02227648
	.public ov39_02227778
	.public ov39_022278D4
	.public ov39_02227A5C
	.public ov39_02227B24
	.public ov39_02227B5C
	.public ov39_02227D5C
	.public ov39_02227DE4
	.public ov39_02227DEC
	.public ov39_02227E8C
	.public ov39_02227F14
	.public ov39_02227FEC
	.public ov39_0222801C
	.public ov39_022280D4
	.public ov39_02228140
	.public ov39_02228308
	.public ov39_02228370
	.public ov39_02228418
	.public ov39_02228440
	.public ov39_022285CC
	.public ov39_022287F8
	.public ov39_022288A0
	.public ov39_02228964
	.public ov39_0222899C
	.public ov39_022289D0
	.public ov39_02228A34
	.public ov39_02228AC0
	.public ov39_02228B04
	.public ov39_02228BAC
	.public ov39_02228C78
	.public ov39_02228D0C
	.public ov39_02228DA0
	.public ov39_02228E54
	.public ov39_02228EC8
	.public ov39_02228F18
	.public ov39_02228F54
	.public ov39_02228FC8
	.public ov39_022290A4
	.public ov39_0222915C
	.public ov39_02229224
	.public ov39_0222A8B0
	.public ov39_0222A8B4
	.public ov39_0222A8C0
	.public ov39_0222A8D4
	.public ov39_0222A8E4
	.public ov39_0222A90C
	.public ov39_0222A944
	.public ov39_0222AA20
	.public ov39_02227060
	.public ov39_02227080
	.public ov39_02227188
	.public ov39_022271A4
	.public ov39_022271C0
	.public ov39_02227334
	.public ov39_02227348
	.public ov39_02227364
	.public ov39_0222738C
	.public ov39_022273F8
	.public ov39_0222748C
	.public ov39_022274B4
	.public ov39_022276A4
	.public ov39_022276E0
	.public ov39_02227720
	.public ov39_0222774C
	.public ov39_02227B1C
	.public ov39_02227B20
	.public ov39_02227B50
	.public ov39_02227B54
	.public ov39_02227B58
	.public ov39_02227D44
	.public ov39_02227D50
	.public ov39_02227DB8
	.public ov39_02227E3C
	.public ov39_02227E48
	.public ov39_02227E6C
	.public ov39_02227ECC
	.public ov39_02227EF4
	.public ov39_02227F60
	.public ov39_02227F74
	.public ov39_02227F84
	.public ov39_02227FA8
	.public ov39_02227FC4
	.public ov39_02227FFC
	.public ov39_022280B4
	.public ov39_02228120
	.public ov39_022285A8
	.public ov39_02228948
	.public ov39_02228A70
	.public ov39_02228A8C
	.public ov39_02228AA8
	.public ov39_02228B6C
	.public ov39_02228B74
	.public ov39_02228D90
	.public ov39_02228EA4
	.public ov39_0222908C

	thumb_func_start ov39_02227088
ov39_02227088: ; 0x02227088
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x10
	add r6, r2, #0
	mov r2, #0x67
	add r5, r1, #0
	add r7, r3, #0
	str r0, [sp, #4]
	add r0, r7, #0
	mov r1, #0
	lsl r2, r2, #2
	bl MI_CpuFill8
	ldr r1, [sp, #0x28]
	mov r0, #0x3c
	bl String_New
	add r4, r0, #0
	add r0, r5, #0
	add r1, r6, #0
	add r2, r4, #0
	bl PCStorage_GetBoxName
	add r0, r4, #0
	add r1, r7, #0
	mov r2, #0x14
	bl CopyStringToU16Array
	add r0, r4, #0
	bl String_Delete
	mov r4, #0
	str r7, [sp, #0xc]
	str r7, [sp, #8]
_022270CA:
	mov r0, #0
	str r0, [sp]
	add r0, r5, #0
	add r1, r6, #0
	add r2, r4, #0
	mov r3, #5
	bl PCStorage_GetMonDataByIndexPair
	ldr r1, [sp, #0xc]
	add r2, r4, #0
	strh r0, [r1, #0x28]
	mov r0, #0
	str r0, [sp]
	add r0, r5, #0
	add r1, r6, #0
	mov r3, #0
	bl PCStorage_GetMonDataByIndexPair
	ldr r1, [sp, #8]
	add r2, r4, #0
	str r0, [r1, #0x64]
	mov r0, #0
	str r0, [sp]
	add r0, r5, #0
	add r1, r6, #0
	mov r3, #7
	bl PCStorage_GetMonDataByIndexPair
	ldr r1, [sp, #8]
	add r2, r4, #0
	add r1, #0xdc
	str r0, [r1]
	mov r0, #0
	str r0, [sp]
	add r0, r5, #0
	add r1, r6, #0
	mov r3, #0xae
	bl PCStorage_GetMonDataByIndexPair
	ldr r1, _02227180 ; =0x000001EE
	cmp r0, r1
	bne _02227130
	add r0, r1, #0
	sub r0, #0x9a
	ldr r1, [r7, r0]
	mov r0, #1
	lsl r0, r4
	orr r1, r0
	mov r0, #0x55
	lsl r0, r0, #2
	str r1, [r7, r0]
_02227130:
	mov r0, #0
	str r0, [sp]
	add r0, r5, #0
	add r1, r6, #0
	add r2, r4, #0
	mov r3, #0x70
	bl PCStorage_GetMonDataByIndexPair
	mov r1, #0x56
	add r2, r7, r4
	lsl r1, r1, #2
	strb r0, [r2, r1]
	ldr r0, [sp, #0xc]
	add r4, r4, #1
	add r0, r0, #2
	str r0, [sp, #0xc]
	ldr r0, [sp, #8]
	add r0, r0, #4
	str r0, [sp, #8]
	cmp r4, #0x1e
	blt _022270CA
	add r0, r5, #0
	add r1, r6, #0
	bl PCStorage_GetBoxWallpaper
	ldr r2, _02227184 ; =0x00000176
	mov r1, #0
	strb r0, [r7, r2]
	add r0, r2, #1
	strb r1, [r7, r0]
	ldr r0, [sp, #4]
	add r1, r7, #0
	add r2, #0x22
	bl SaveArray_CalcCRC16
	mov r1, #0x66
	lsl r1, r1, #2
	strh r0, [r7, r1]
	add sp, #0x10
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
_02227180: .word 0x000001EE
_02227184: .word 0x00000176
	thumb_func_end ov39_02227088
