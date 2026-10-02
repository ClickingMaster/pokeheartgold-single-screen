	.extern PmAgbCartridge_GetVersionInternal
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
	.extern ov74_0223195C
	.extern ov74_02233E8C
	.extern ov74_02233ED4
	.extern ov74_02233F14
	.extern ov74_02233F68
#include "config.h"
#include "msgdata/msg/msg_0247.h"
#include "msgdata/msg/msg_0442.h"
#include "constants/sndseq.h"
	.include "asm/macros.inc"
	.include "overlay_74_thumb.inc"
	.include "global.inc"

.text
	thumb_func_start ov74_022317D8
ov74_022317D8: ; 0x022317D8
	push {r3, r4, r5, lr}
	sub sp, #0x10
	ldr r1, _0223192C ; =0x0000E890
	add r5, r0, #0
	add r4, r5, r1
	ldr r1, [r4]
	cmp r1, #9
	bls _022317EA
	b _02231924
_022317EA:
	add r1, r1, r1
	add r1, pc
	ldrh r1, [r1, #6]
	lsl r1, r1, #0x10
	asr r1, r1, #0x10
	add pc, r1
_022317F6: ; jump table
	.short _0223180A - _022317F6 - 2 ; case 0
	.short _0223182A - _022317F6 - 2 ; case 1
	.short _02231842 - _022317F6 - 2 ; case 2
	.short _02231852 - _022317F6 - 2 ; case 3
	.short _02231862 - _022317F6 - 2 ; case 4
	.short _0223187E - _022317F6 - 2 ; case 5
	.short _02231892 - _022317F6 - 2 ; case 6
	.short _022318AE - _022317F6 - 2 ; case 7
	.short _022318DA - _022317F6 - 2 ; case 8
	.short _02231900 - _022317F6 - 2 ; case 9
_0223180A:
	bl ov74_0223195C
	ldr r0, [r5, #0x10]
	bl Save_MigratedPokemon_Get
	add r5, r0, #0
	bl ov74_02233F68
	add r1, r0, #0
	add r0, r5, #0
	bl MigratedPokemon_RecordMigration
	ldr r0, [r4]
	add r0, r0, #1
	str r0, [r4]
	b _02231924
_0223182A:
	bl OS_GetTick
	mov r2, #0x78
	mov r3, #0
	bl _ull_mod
	add r0, r0, #1
	str r0, [r4, #4]
	ldr r0, [r4]
	add r0, r0, #1
	str r0, [r4]
	b _02231924
_02231842:
	ldr r0, [r4, #4]
	sub r0, r0, #1
	str r0, [r4, #4]
	bne _02231924
	ldr r0, [r4]
	add r0, r0, #1
	str r0, [r4]
	b _02231924
_02231852:
	ldr r0, [r5, #0x10]
	mov r1, #2
	bl Save_PrepareForAsyncWrite
	ldr r0, [r4]
	add r0, r0, #1
	str r0, [r4]
	b _02231924
_02231862:
	ldr r0, [r5, #0x10]
	bl Save_WriteFileAsync
	cmp r0, #3
	bne _02231872
	add sp, #0x10
	mov r0, #0xc
	pop {r3, r4, r5, pc}
_02231872:
	cmp r0, #1
	bne _02231924
	ldr r0, [r4]
	add r0, r0, #1
	str r0, [r4]
	b _02231924
_0223187E:
	mov r0, #0
	add r1, r0, #0
	add r2, sp, #0
	mov r3, #0x10
	bl CTRDG_ReadAgbFlash
	ldr r0, [r4]
	add r0, r0, #1
	str r0, [r4]
	b _02231924
_02231892:
	bl ov74_02233E8C
	cmp r0, #0
	bne _022318A6
	mov r0, #1
	bl Sys_ClearSleepDisableFlag
	add sp, #0x10
	mov r0, #0xc
	pop {r3, r4, r5, pc}
_022318A6:
	ldr r0, [r4]
	add r0, r0, #1
	str r0, [r4]
	b _02231924
_022318AE:
	bl ov74_02233F14
	cmp r0, #9
	beq _02231924
	bl ov74_02233F14
	cmp r0, #0xb
	bne _022318C6
	ldr r0, [r4]
	add r0, r0, #1
	str r0, [r4]
	b _02231924
_022318C6:
	bl ov74_02233ED4
	cmp r0, #8
	bne _02231924
	mov r0, #1
	bl Sys_ClearSleepDisableFlag
	add sp, #0x10
	mov r0, #0xc
	pop {r3, r4, r5, pc}
_022318DA:
	ldr r0, [r5, #0x10]
	bl Save_WriteFileAsync
	cmp r0, #3
	bne _022318EA
	add sp, #0x10
	mov r0, #0xc
	pop {r3, r4, r5, pc}
_022318EA:
	cmp r0, #2
	bne _022318DA
	bl ov74_02233ED4
	mov r0, #1
	bl Sys_SetSleepDisableFlag
	ldr r0, [r4]
	add r0, r0, #1
	str r0, [r4]
	b _02231924
_02231900:
	bl ov74_02233ED4
	cmp r0, #8
	bne _02231914
	mov r0, #1
	bl Sys_ClearSleepDisableFlag
	add sp, #0x10
	mov r0, #0xc
	pop {r3, r4, r5, pc}
_02231914:
	cmp r0, #0
	bne _02231924
	mov r0, #1
	bl Sys_ClearSleepDisableFlag
	add sp, #0x10
	mov r0, #0xb
	pop {r3, r4, r5, pc}
_02231924:
	mov r0, #0xa
	add sp, #0x10
	pop {r3, r4, r5, pc}
	nop
_0223192C: .word 0x0000E890
	thumb_func_end ov74_022317D8
