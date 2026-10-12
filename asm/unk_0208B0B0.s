#include "constants/items.h"
#include "constants/moves.h"
#include "constants/pokemon.h"
#include "constants/ribbon.h"
#include "constants/field_move_response.h"
#include "msgdata/msg/msg_0300.h"
	.include "asm/macros.inc"
	.include "unk_02088288.inc"
	.include "global.inc"
	.text
	thumb_func_start PokemonSummary_FlashMonButton
PokemonSummary_FlashMonButton: ; 0x0208B0B0
	push {lr}
	sub sp, #0x14
	cmp r1, #0
	bne _0208B0D6
	mov r1, #6
	str r1, [sp]
	mov r3, #3
	str r3, [sp, #4]
	mov r3, #1
	str r3, [sp, #8]
	mov r3, #0
	str r3, [sp, #0xc]
	str r2, [sp, #0x10]
	mov r2, #0x18
	mov r3, #5
	bl PokemonSummary_StartButtonFlash
	add sp, #0x14
	pop {pc}
_0208B0D6:
	mov r1, #6
	str r1, [sp]
	mov r3, #3
	str r3, [sp, #4]
	mov r3, #1
	str r3, [sp, #8]
	mov r3, #0
	str r3, [sp, #0xc]
	str r2, [sp, #0x10]
	mov r2, #0x18
	mov r3, #0xd
	bl PokemonSummary_StartButtonFlash
	add sp, #0x14
	pop {pc}
	thumb_func_end PokemonSummary_FlashMonButton
