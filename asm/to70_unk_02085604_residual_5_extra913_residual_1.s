	.extern sub_02085820
	.extern sub_020858DC
	.extern sub_02085938
	.extern sub_02085974
#include "constants/items.h"
#include "constants/moves.h"
#include "constants/pokemon.h"
#include "constants/ribbon.h"
#include "constants/field_move_response.h"
#include "msgdata/msg/msg_0300.h"
	.include "asm/macros.inc"
	.include "unk_02085604.inc"
	.include "global.inc"

.text
	thumb_func_start sub_020863C0
sub_020863C0: ; 0x020863C0
	push {r3, r4, r5, r6}
	mov r2, #0x2d
	lsl r2, r2, #4
	ldr r6, [r0, r2]
	mov r5, #0
	add r4, r5, #0
	cmp r6, #0
	ble _020863EE
	mov r2, #1
_020863D2:
	ldr r3, [r0, #4]
	cmp r1, r3
	bne _020863DC
	add r5, r2, #0
	b _020863E6
_020863DC:
	cmp r5, #1
	bne _020863E6
	sub r0, r4, #1
	pop {r3, r4, r5, r6}
	bx lr
_020863E6:
	add r4, r4, #1
	add r0, #0x1c
	cmp r4, r6
	blt _020863D2
_020863EE:
	sub r0, r6, #1
	pop {r3, r4, r5, r6}
	bx lr
	thumb_func_end sub_020863C0
