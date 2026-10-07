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
	.extern ov74_02235A74
	.extern ov74_0223D454
	.extern ov74_0223D48C
	.extern ov74_0223D654
#include "config.h"
#include "msgdata/msg/msg_0247.h"
#include "msgdata/msg/msg_0442.h"
#include "constants/sndseq.h"
	.include "asm/macros.inc"
	.include "overlay_74_thumb.inc"
	.include "global.inc"

.text
	thumb_func_start ov74_02235690
ov74_02235690: ; 0x02235690
	push {r4, r5, r6, r7, lr}
	sub sp, #0x14
	ldr r6, _022356F8 ; =ov74_0223D454
	bl NNS_G2dInitOamManagerModule
	mov r0, #0
	str r0, [sp]
	mov r1, #0x7e
	str r1, [sp, #4]
	str r0, [sp, #8]
	mov r3, #0x20
	ldr r2, _022356FC ; =ov74_0223D454
	str r3, [sp, #0xc]
	ldr r2, [r2, #8]
	str r2, [sp, #0x10]
	add r2, r0, #0
	bl OamManager_Create
	ldr r2, _022356FC ; =ov74_0223D454
	ldr r1, _02235700 ; =ov74_0223D48C
	ldr r2, [r2, #8]
	mov r0, #0x80
	bl G2dRenderer_Init
	ldr r1, _022356FC ; =ov74_0223D454
	mov r2, #1
	str r0, [r1, #0x34]
	ldr r0, _02235700 ; =ov74_0223D48C
	mov r1, #0
	lsl r2, r2, #0x14
	bl G2dRenderer_SetSubSurfaceCoords
	mov r1, #3
	mov r7, #0x16
	ldr r0, _02235704 ; =ov74_0223D654
	lsl r1, r1, #0x12
	str r1, [r0, #0x10]
	mov r4, #0
	add r5, r6, #0
	lsl r7, r7, #4
_022356E0:
	ldr r2, [r6, #8]
	mov r0, #0x20
	add r1, r4, #0
	bl Create2DGfxResObjMan
	str r0, [r5, r7]
	add r4, r4, #1
	add r5, r5, #4
	cmp r4, #6
	blt _022356E0
	add sp, #0x14
	pop {r4, r5, r6, r7, pc}
	.balign 4, 0
_022356F8: .word ov74_0223D454
_022356FC: .word ov74_0223D454
_02235700: .word ov74_0223D48C
_02235704: .word ov74_0223D654
	thumb_func_end ov74_02235690

	thumb_func_start ov74_02235708
ov74_02235708: ; 0x02235708
	push {r4, lr}
	ldr r0, _02235720 ; =ov74_0223D454
	ldr r4, _02235724 ; =ov74_0223D454
	ldr r0, [r0, #0x34]
	cmp r0, #0
	bne _02235718
	bl GF_AssertFail
_02235718:
	add r4, #0x34
	add r0, r4, #0
	pop {r4, pc}
	nop
_02235720: .word ov74_0223D454
_02235724: .word ov74_0223D454
	thumb_func_end ov74_02235708
