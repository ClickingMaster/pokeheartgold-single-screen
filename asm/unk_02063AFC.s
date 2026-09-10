#include "constants/sndseq.h"
#include "constants/species.h"
#include "constants/maps.h"
#include "constants/pokemon.h"
#include "constants/flags.h"
#include "constants/vars.h"
#include "constants/items.h"
#include "constants/std_script.h"
#include "fielddata/script/scr_seq/event_D24R0204.h"
#include "constants/field_move_response.h"
	.include "asm/macros.inc"
	.include "unk_020632B0.inc"
	.include "global.inc"
	.text
	thumb_func_start sub_02063B9C
sub_02063B9C: ; 0x02063B9C
	push {r4, r5, r6, lr}
	sub sp, #0x20
	add r5, r0, #0
	bl sub_0205F3BC
	add r4, r0, #0
	ldrb r0, [r4, #1]
	cmp r0, #3
	bhi _02063C7A
	add r0, r0, r0
	add r0, pc
	ldrh r0, [r0, #6]
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	add pc, r0
_02063BBA: ; jump table
	.short _02063BC2 - _02063BBA - 2 ; case 0
	.short _02063BE4 - _02063BBA - 2 ; case 1
	.short _02063C16 - _02063BBA - 2 ; case 2
	.short _02063C2C - _02063BBA - 2 ; case 3
_02063BC2:
	ldr r3, _02063C80 ; =_020FE0C4
	add r2, sp, #0x10
	ldmia r3!, {r0, r1}
	stmia r2!, {r0, r1}
	ldmia r3!, {r0, r1}
	stmia r2!, {r0, r1}
	add r0, r5, #0
	bl MapObject_GetFacingDirection
	strb r0, [r4, #4]
	lsl r1, r0, #2
	add r0, sp, #0x10
	ldr r0, [r0, r1]
	strb r0, [r4, #5]
	ldrb r0, [r4, #1]
	add r0, r0, #1
	strb r0, [r4, #1]
_02063BE4:
	ldr r6, _02063C84 ; =_020FE0D4
	add r3, sp, #0
	add r2, r3, #0
	ldmia r6!, {r0, r1}
	stmia r3!, {r0, r1}
	ldmia r6!, {r0, r1}
	stmia r3!, {r0, r1}
	mov r0, #5
	ldrsb r0, [r4, r0]
	lsl r3, r0, #3
	mov r0, #6
	ldrsb r0, [r4, r0]
	lsl r1, r0, #2
	add r0, r2, r3
	ldr r0, [r1, r0]
	mov r1, #0
	bl sub_0206234C
	add r1, r0, #0
	add r0, r5, #0
	bl MapObject_ForceSetHeldMovement
	ldrb r0, [r4, #1]
	add r0, r0, #1
	strb r0, [r4, #1]
_02063C16:
	add r0, r5, #0
	bl sub_02062428
	cmp r0, #0
	bne _02063C26
	add sp, #0x20
	mov r0, #1
	pop {r4, r5, r6, pc}
_02063C26:
	ldrb r0, [r4, #1]
	add r0, r0, #1
	strb r0, [r4, #1]
_02063C2C:
	mov r0, #8
	ldrsb r1, [r4, r0]
	add r1, r1, #1
	strb r1, [r4, #8]
	ldrsb r0, [r4, r0]
	cmp r0, #8
	bge _02063C40
	add sp, #0x20
	mov r0, #1
	pop {r4, r5, r6, pc}
_02063C40:
	mov r0, #0
	strb r0, [r4, #8]
	mov r0, #7
	ldrsb r1, [r4, r0]
	add r1, r1, #1
	strb r1, [r4, #7]
	ldrsb r0, [r4, r0]
	cmp r0, #4
	bge _02063C64
	mov r0, #6
	ldrsb r0, [r4, r0]
	add sp, #0x20
	add r1, r0, #1
	mov r0, #1
	and r1, r0
	strb r1, [r4, #6]
	strb r0, [r4, #1]
	pop {r4, r5, r6, pc}
_02063C64:
	mov r1, #4
	ldrsb r1, [r4, r1]
	add r0, r5, #0
	bl MapObject_SetFacingDirection
	ldrb r0, [r4, #1]
	add r0, r0, #1
	strb r0, [r4, #1]
	mov r0, #0
	strb r0, [r4, #7]
	strb r0, [r4]
_02063C7A:
	mov r0, #0
	add sp, #0x20
	pop {r4, r5, r6, pc}
	.balign 4, 0
_02063C80: .word _020FE0C4
_02063C84: .word _020FE0D4
	thumb_func_end sub_02063B9C
