	.extern sub_0208B714
	.extern sub_0208B780
#include "constants/items.h"
#include "constants/moves.h"
#include "constants/pokemon.h"
#include "constants/ribbon.h"
#include "constants/field_move_response.h"
#include "msgdata/msg/msg_0300.h"
	.include "asm/macros.inc"
	.include "unk_0208B1AC.inc"
	.include "global.inc"

.text
	thumb_func_start sub_0208B89C
sub_0208B89C: ; 0x0208B89C
	push {r3, r4, r5, lr}
	sub sp, #0x30
	ldr r1, _0208B9C0 ; =0x000007BC
	add r5, r0, #0
	ldrsb r1, [r5, r1]
	cmp r1, #2
	beq _0208B8AC
	b _0208B9B6
_0208B8AC:
	mov r1, #0x8b
	lsl r1, r1, #2
	ldr r1, [r5, r1]
	ldr r1, [r1, #0x34]
	cmp r1, #0
	beq _0208B9B6
	bl sub_0208A520
	add r4, r0, #0
	add r0, sp, #0x14
	mov r1, #0
	mov r2, #5
	bl MI_CpuFill8
	add r1, sp, #0x18
	add r0, r4, #0
	add r1, #2
	bl CalcBoxMonPokeathlonPerformance
	mov r0, #0x8b
	lsl r0, r0, #2
	ldr r2, [r5, r0]
	ldrb r0, [r2, #0x11]
	cmp r0, #1
	bne _0208B8E8
	ldr r0, [r2]
	ldrb r2, [r2, #0x14]
	add r1, sp, #0x14
	bl Party_GetMonAprijuiceModifiers
_0208B8E8:
	add r0, sp, #0xc
	add r1, r4, #0
	add r2, sp, #0x14
	mov r3, #0x13
	bl CalcBoxmonPokeathlonStars
	add r3, sp, #0xc
	ldrb r0, [r3, #6]
	mov r1, #0x4f
	str r0, [sp]
	mov r0, #0xc
	ldrsb r0, [r3, r0]
	str r0, [sp, #4]
	mov r0, #0x68
	str r0, [sp, #8]
	ldrh r2, [r3, #0x1e]
	ldrh r3, [r3]
	ldr r0, _0208B9C4 ; =0x00000404
	lsl r2, r2, #0x1a
	lsl r3, r3, #0x11
	add r0, r5, r0
	lsr r2, r2, #0x1d
	lsr r3, r3, #0x1d
	bl sub_0208B780
	add r3, sp, #0xc
	ldrb r0, [r3, #2]
	mov r1, #0x54
	str r0, [sp]
	mov r0, #8
	ldrsb r0, [r3, r0]
	str r0, [sp, #4]
	mov r0, #0x69
	str r0, [sp, #8]
	ldrh r2, [r3, #0xe]
	ldrh r3, [r3]
	ldr r0, _0208B9C4 ; =0x00000404
	lsl r2, r2, #0x1a
	lsl r3, r3, #0x1d
	add r0, r5, r0
	lsr r2, r2, #0x1d
	lsr r3, r3, #0x1d
	bl sub_0208B780
	add r3, sp, #0xc
	ldrb r0, [r3, #4]
	mov r1, #0x59
	str r0, [sp]
	mov r0, #0xa
	ldrsb r0, [r3, r0]
	str r0, [sp, #4]
	mov r0, #0x6a
	str r0, [sp, #8]
	ldrh r2, [r3, #0x16]
	ldrh r3, [r3]
	ldr r0, _0208B9C4 ; =0x00000404
	lsl r2, r2, #0x1a
	lsl r3, r3, #0x17
	add r0, r5, r0
	lsr r2, r2, #0x1d
	lsr r3, r3, #0x1d
	bl sub_0208B780
	add r3, sp, #0xc
	ldrb r0, [r3, #3]
	mov r1, #0x5e
	str r0, [sp]
	mov r0, #9
	ldrsb r0, [r3, r0]
	str r0, [sp, #4]
	mov r0, #0x6b
	str r0, [sp, #8]
	ldrh r2, [r3, #0x12]
	ldrh r3, [r3]
	ldr r0, _0208B9C4 ; =0x00000404
	lsl r2, r2, #0x1a
	lsl r3, r3, #0x1a
	add r0, r5, r0
	lsr r2, r2, #0x1d
	lsr r3, r3, #0x1d
	bl sub_0208B780
	add r3, sp, #0xc
	ldrb r0, [r3, #5]
	mov r1, #0x63
	str r0, [sp]
	mov r0, #0xb
	ldrsb r0, [r3, r0]
	str r0, [sp, #4]
	mov r0, #0x6c
	str r0, [sp, #8]
	ldrh r2, [r3, #0x1a]
	ldrh r3, [r3]
	ldr r0, _0208B9C4 ; =0x00000404
	lsl r2, r2, #0x1a
	lsl r3, r3, #0x14
	add r0, r5, r0
	lsr r2, r2, #0x1d
	lsr r3, r3, #0x1d
	bl sub_0208B780
	add sp, #0x30
	pop {r3, r4, r5, pc}
_0208B9B6:
	add r0, r5, #0
	bl sub_0208B714
	add sp, #0x30
	pop {r3, r4, r5, pc}
	.balign 4, 0
_0208B9C0: .word 0x000007BC
_0208B9C4: .word 0x00000404
	thumb_func_end sub_0208B89C
