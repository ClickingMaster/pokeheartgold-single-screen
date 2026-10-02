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
	.extern ov74_02236168
	.extern ov74_022366E8
	.extern ov74_0223E2FC
#include "config.h"
#include "msgdata/msg/msg_0247.h"
#include "msgdata/msg/msg_0442.h"
#include "constants/sndseq.h"
	.include "asm/macros.inc"
	.include "overlay_74_thumb.inc"
	.include "global.inc"

.text
	thumb_func_start ov74_02236768
ov74_02236768: ; 0x02236768
	push {r3, r4, r5, lr}
	add r4, r0, #0
	ldr r0, _02236884 ; =ov74_0223E2FC
	ldrh r1, [r4]
	ldr r3, [r0]
	ldr r0, _02236888 ; =0x0000115A
	ldrh r2, [r3, r0]
	cmp r2, r1
	bne _02236784
	add r0, r0, #6
	ldr r1, [r3, r0]
	ldr r0, [r4, #4]
	cmp r1, r0
	beq _022367B0
_02236784:
	ldr r0, _02236884 ; =ov74_0223E2FC
	ldr r3, [r0]
	ldr r0, _0223688C ; =0x00001164
	ldr r2, [r3, r0]
	add r1, r2, #1
	str r1, [r3, r0]
	cmp r2, #0x10
	blo _022367AC
	add r0, r4, #0
	bl ov74_022366E8
	ldr r0, _02236884 ; =ov74_0223E2FC
	ldr r1, [r0]
	ldr r0, _02236890 ; =0x0000117C
	ldr r1, [r1, r0]
	cmp r1, #0
	beq _022367B0
	mov r0, #5
	blx r1
	b _022367B0
_022367AC:
	mov r0, #0
	pop {r3, r4, r5, pc}
_022367B0:
	ldrh r3, [r4, #2]
	ldr r0, _02236894 ; =0x0000FFFF
	cmp r3, r0
	bne _022367D8
	ldr r1, _02236884 ; =ov74_0223E2FC
	add r4, #8
	ldr r2, [r1]
	mov r1, #0xfe
	lsl r1, r1, #4
	add r1, r2, r1
	add r0, r4, #0
	mov r2, #0x68
	bl MIi_CpuCopy32
	ldr r0, _02236884 ; =ov74_0223E2FC
	mov r2, #1
	ldr r1, [r0]
	ldr r0, _02236898 ; =0x00001048
	str r2, [r1, r0]
	b _02236880
_022367D8:
	ldr r1, _02236884 ; =ov74_0223E2FC
	ldr r0, _0223689C ; =0x00001168
	ldr r2, [r1]
	str r3, [r2, r0]
	ldr r3, [r1]
	add r1, r0, #0
	sub r1, #0xc
	ldrh r2, [r4, #2]
	ldrh r1, [r3, r1]
	cmp r2, r1
	blo _02236800
	add r0, #0x14
	ldr r1, [r3, r0]
	cmp r1, #0
	beq _022367FA
	mov r0, #4
	blx r1
_022367FA:
	mov r0, #9
	bl ov74_02236168
_02236800:
	ldr r0, _02236884 ; =ov74_0223E2FC
	mov r2, #0x46
	ldr r1, [r0]
	lsl r2, r2, #6
	ldrh r3, [r4, #2]
	add r5, r1, r2
	ldrb r1, [r5, r3]
	cmp r1, #0
	bne _02236880
	mov r1, #1
	strb r1, [r5, r3]
	ldr r5, [r0]
	add r0, r2, #0
	sub r0, #0x24
	ldrh r0, [r5, r0]
	ldrh r1, [r4, #2]
	sub r0, r0, #1
	cmp r1, r0
	bne _02236844
	add r3, r1, #0
	add r1, r2, #0
	sub r1, #8
	sub r2, #0x20
	mov r0, #0x68
	ldr r1, [r5, r1]
	mul r3, r0
	ldr r2, [r5, r2]
	add r4, #8
	add r0, r4, #0
	add r1, r1, r3
	sub r2, r2, r3
	bl MIi_CpuCopy32
	b _02236858
_02236844:
	add r4, #8
	sub r2, #8
	add r0, r4, #0
	ldr r4, [r5, r2]
	mov r2, #0x68
	add r3, r1, #0
	mul r3, r2
	add r1, r4, r3
	bl MIi_CpuCopy32
_02236858:
	ldr r1, _02236884 ; =ov74_0223E2FC
	ldr r0, _022368A0 ; =0x0000115E
	ldr r3, [r1]
	ldrh r2, [r3, r0]
	add r2, r2, #1
	strh r2, [r3, r0]
	ldr r3, [r1]
	sub r1, r0, #2
	ldrh r2, [r3, r0]
	ldrh r1, [r3, r1]
	cmp r2, r1
	bne _02236880
	add r0, #0x1e
	ldr r1, [r3, r0]
	cmp r1, #0
	beq _0223687C
	mov r0, #2
	blx r1
_0223687C:
	mov r0, #1
	pop {r3, r4, r5, pc}
_02236880:
	mov r0, #0
	pop {r3, r4, r5, pc}
	.balign 4, 0
_02236884: .word ov74_0223E2FC
_02236888: .word 0x0000115A
_0223688C: .word 0x00001164
_02236890: .word 0x0000117C
_02236894: .word 0x0000FFFF
_02236898: .word 0x00001048
_0223689C: .word 0x00001168
_022368A0: .word 0x0000115E
	thumb_func_end ov74_02236768
