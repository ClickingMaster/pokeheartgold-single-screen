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
	.extern ov74_0223145C
#include "config.h"
#include "msgdata/msg/msg_0247.h"
#include "msgdata/msg/msg_0442.h"
#include "constants/sndseq.h"
	.include "asm/macros.inc"
	.include "overlay_74_thumb.inc"
	.include "global.inc"
	.public ov74_02231264
	.public ov74_0223127C

.text
	thumb_func_start ov74_02231264
ov74_02231264: ; 0x02231264
	push {r4, lr}
	ldr r0, _02231278 ; =0x04000006
	ldrh r4, [r0]
	bl OS_GetTick
	add r1, r0, r4
	mov r0, #1
	bic r1, r0
	add r0, r1, #1
	pop {r4, pc}
	.balign 4, 0
_02231278: .word 0x04000006
	thumb_func_end ov74_02231264

	thumb_func_start ov74_0223127C
ov74_0223127C: ; 0x0223127C
	push {r4, r5, r6, r7}
	add r3, r0, #0
	mov r0, #0
	lsr r1, r1, #2
	beq _022312B2
	ldr r4, _022312B8 ; =0x00269EC3
_02231288:
	ldr r5, _022312BC ; =0x5D588B65
	ldr r6, _022312BC ; =0x5D588B65
	mul r5, r3
	ldr r3, _022312B8 ; =0x00269EC3
	add r0, r0, #1
	add r3, r5, r3
	lsr r5, r3, #0x10
	mul r6, r3
	add r3, r6, r4
	lsl r5, r5, #0x10
	lsr r7, r3, #0x10
	lsr r5, r5, #0x10
	lsl r7, r7, #0x10
	ldr r6, [r2]
	lsl r5, r5, #0x10
	lsr r7, r7, #0x10
	orr r5, r7
	eor r5, r6
	stmia r2!, {r5}
	cmp r0, r1
	blo _02231288
_022312B2:
	add r0, r3, #0
	pop {r4, r5, r6, r7}
	bx lr
	.balign 4, 0
_022312B8: .word 0x00269EC3
_022312BC: .word 0x5D588B65
	thumb_func_end ov74_0223127C
