#include "constants/items.h"
#include "constants/moves.h"
#include "constants/pokemon.h"
#include "constants/ribbon.h"
#include "constants/field_move_response.h"
#include "msgdata/msg/msg_0300.h"
	.include "asm/macros.inc"
	.include "unk_020863F4.inc"
	.include "global.inc"
	.public sub_02086B6C

.text
	thumb_func_start sub_02086B6C
sub_02086B6C: ; 0x02086B6C
	push {r4, lr}
	add r3, r0, #0
	mov r0, #0x1c
	mul r0, r2
	add r1, #0x10
	mov r2, #0x22
	lsl r1, r1, #2
	add r0, r3, r0
	lsl r2, r2, #4
	add r3, r3, r1
	add r1, r2, #0
	add r1, #0xe6
	ldrb r4, [r3, r1]
	add r1, r2, #0
	add r1, #0xe7
	ldrb r1, [r3, r1]
	ldr r0, [r0, r2]
	add r4, r4, r1
	lsr r1, r4, #0x1f
	add r1, r4, r1
	add r4, r2, #0
	add r4, #0xe4
	add r2, #0xe5
	lsl r1, r1, #0xf
	ldrb r4, [r3, r4]
	ldrb r2, [r3, r2]
	asr r1, r1, #0x10
	add r3, r4, r2
	lsr r2, r3, #0x1f
	add r2, r3, r2
	lsl r2, r2, #0xf
	asr r2, r2, #0x10
	bl ManagedSprite_SetPositionXY
	pop {r4, pc}
	.balign 4, 0
	thumb_func_end sub_02086B6C
