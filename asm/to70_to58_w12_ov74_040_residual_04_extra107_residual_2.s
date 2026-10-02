	.extern ov74_02229200
	.extern ov74_02229294
	.extern ov74_02229450
	.extern ov74_02229DA0
	.extern ov74_02229DCC
	.extern ov74_02229DD4
	.extern ov74_02229DE0
	.extern ov74_02229E60
	.extern ov74_02229F90
	.extern ov74_02229FF4
	.extern ov74_0222A2A4
	.extern ov74_0222A2E0
	.extern ov74_0222A35C
	.extern ov74_0222A3CC
	.extern ov74_0222A494
	.extern ov74_0222A4CC
	.extern ov74_0222A504
	.extern ov74_0222A538
	.extern ov74_0222A560
	.extern ov74_0222A5AC
	.extern ov74_0222A5CC
	.extern ov74_0222A650
	.extern ov74_0222A688
	.extern ov74_0222A6C0
	.extern ov74_0222ACD8
	.extern ov74_0222AE6C
	.extern ov74_0222C2EC
	.extern ov74_0222CD94
	.extern ov74_0222D2D4
	.extern ov74_0222D308
	.extern ov74_0222D358
	.extern ov74_0222D414
	.extern ov74_0222D490
	.extern ov74_0222D494
	.extern ov74_0222D4E4
	.extern ov74_0222D55C
	.extern ov74_0222D5C4
	.extern ov74_0222D614
	.extern ov74_0222D690
	.extern ov74_0222D774
	.extern ov74_0222D78C
	.extern ov74_0222D7D0
	.extern ov74_0222DDFC
	.extern ov74_0222E1F4
	.extern ov74_0222EB44
	.extern ov74_0222EE08
	.extern ov74_0222EE0C
	.extern ov74_0222EE58
	.extern ov74_0222EEB0
	.extern ov74_0222EF18
	.extern ov74_0222EF64
	.extern ov74_0222EF68
	.extern ov74_0222F598
	.extern ov74_0222F7D4
	.extern ov74_0222FB8C
	.extern ov74_02230018
	.extern ov74_02230030
	.extern ov74_02230070
	.extern ov74_022300A8
	.extern ov74_02230110
	.extern ov74_02230138
	.extern ov74_02230404
	.extern ov74_02230520
	.extern ov74_02230590
	.extern ov74_022305E4
	.extern ov74_022306C8
	.extern ov74_022308E0
	.extern ov74_02230964
	.extern ov74_02230988
	.extern ov74_022309AC
	.extern ov74_022309F0
	.extern ov74_02231054
	.extern ov74_022310AC
	.extern ov74_022310C4
	.extern ov74_022310D0
	.extern ov74_022310DC
	.extern ov74_022310E8
	.extern ov74_022310F4
	.extern ov74_02231100
	.extern ov74_02231154
	.extern ov74_0223115C
	.extern ov74_022311D8
	.extern ov74_022311DC
	.extern ov74_022311E8
	.extern ov74_02231260
	.extern ov74_02235AC4
	.extern ov74_02235BD0
	.extern ov74_02235C10
	.extern ov74_02235CE4
	.extern ov74_02235DA4
	.extern ov74_0223D454
	.extern ov74_0223E2D4
#include "config.h"
#include "msgdata/msg/msg_0247.h"
#include "msgdata/msg/msg_0442.h"
#include "constants/sndseq.h"
	.include "asm/macros.inc"
	.include "overlay_74_thumb.inc"
	.include "global.inc"

.text
	thumb_func_start ov74_02235F58
ov74_02235F58: ; 0x02235F58
	push {r4, r5, r6, r7, lr}
	sub sp, #0xc
	add r7, r1, #0
	mov r1, #2
	add r5, r0, #0
	add r0, r2, #0
	lsl r1, r1, #8
	str r2, [sp]
	bl Heap_Alloc
	ldr r1, _02235FF0 ; =0x0000A001
	add r4, r0, #0
	bl MATHi_CRC16InitTableRev
	add r0, r4, #0
	add r1, r5, #0
	mov r2, #0x50
	bl MATH_CalcCRC16
	add r6, r0, #0
	add r0, r4, #0
	bl Heap_Free
	mov r0, #0
	bl sub_02035754
	ldrb r2, [r0, #4]
	add r1, sp, #4
	mov r3, #0
	strb r2, [r1]
	ldrb r2, [r0, #5]
	strb r2, [r1, #1]
	ldrb r2, [r0, #6]
	strb r2, [r1, #2]
	ldrb r2, [r0, #7]
	strb r2, [r1, #3]
	ldrb r2, [r0, #8]
	strb r2, [r1, #4]
	ldrb r0, [r0, #9]
	strb r0, [r1, #5]
	add r0, sp, #4
	ldrh r2, [r0, #2]
	strh r2, [r0, #6]
	ldr r2, _02235FF4 ; =0x0000D679
	strh r6, [r0, #2]
_02235FB2:
	ldrh r0, [r1]
	add r3, r3, #1
	eor r0, r2
	strh r0, [r1]
	ldrh r2, [r1]
	add r1, r1, #2
	cmp r3, #4
	blt _02235FB2
	mov r1, #0x41
	ldr r0, [sp]
	lsl r1, r1, #2
	bl Heap_Alloc
	add r1, sp, #4
	mov r2, #8
	add r4, r0, #0
	bl CRYPTO_RC4Init
	add r5, #0x50
	mov r2, #0xd6
	add r0, r4, #0
	add r1, r5, #0
	lsl r2, r2, #2
	add r3, r7, #0
	bl CRYPTO_RC4Encrypt
	add r0, r4, #0
	bl Heap_Free
	add sp, #0xc
	pop {r4, r5, r6, r7, pc}
	.balign 4, 0
_02235FF0: .word 0x0000A001
_02235FF4: .word 0x0000D679
	thumb_func_end ov74_02235F58
