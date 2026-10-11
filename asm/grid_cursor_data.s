#include "constants/easy_chat.h"
#include "constants/items.h"
#include "constants/maps.h"
#include "constants/moves.h"
#include "constants/species.h"
	.include "asm/macros.inc"
	.include "unk_020957B0.inc"
	.include "global.inc"

	.text
	.public GridCursor_StepPosition
	.rodata

	; File boundary
_0210841C:
	.byte 0x06, 0x01, 0x06, 0x06
_02108420:
	.byte 0x01, 0x03, 0x03, 0x04
_02108424:
	.byte 0x01, 0x01, 0x01, 0x05
_02108428:
	.byte 0x01, 0x03, 0x03, 0x03
_0210842C:
	.byte 0x01, 0x01, 0x01, 0x01
_02108430:
	.byte 0x06, 0x05, 0x1E, 0x02
	.public gGridCursorPanels
gGridCursorPanels:
	.word _02108430, GridPanel2_Move, GridPanel2_Enter
	.word _0210842C, GridPanel1_Move, GridPanel1_Enter
	.word _02108428, GridPanel3_Move, GridPanel3_Enter
	.word _02108420, GridPanel4_Move, GridPanel4_Enter
	.word _02108424, GridPanel5_Move, GridPanel5_Enter
	.word _0210841C, GridPanel6_Move, GridPanel6_Enter
