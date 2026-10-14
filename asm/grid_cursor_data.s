#include "constants/easy_chat.h"
#include "constants/items.h"
#include "constants/maps.h"
#include "constants/moves.h"
#include "constants/species.h"
	.include "asm/macros.inc"
	.include "grid_cursor_data.inc"
	.include "global.inc"

	.text
	.public GridCursor_StepPosition
	.rodata

	; File boundary
gGridPanel6Dimensions:
	.byte 0x06, 0x01, 0x06, 0x06
gGridPanel4Dimensions:
	.byte 0x01, 0x03, 0x03, 0x04
gGridPanel5Dimensions:
	.byte 0x01, 0x01, 0x01, 0x05
gGridPanel3Dimensions:
	.byte 0x01, 0x03, 0x03, 0x03
gGridPanel1Dimensions:
	.byte 0x01, 0x01, 0x01, 0x01
gGridPanel2Dimensions:
	.byte 0x06, 0x05, 0x1E, 0x02
	.public gGridCursorPanels
gGridCursorPanels:
	.word gGridPanel2Dimensions, GridPanel2_Move, GridPanel2_Enter
	.word gGridPanel1Dimensions, GridPanel1_Move, GridPanel1_Enter
	.word gGridPanel3Dimensions, GridPanel3_Move, GridPanel3_Enter
	.word gGridPanel4Dimensions, GridPanel4_Move, GridPanel4_Enter
	.word gGridPanel5Dimensions, GridPanel5_Move, GridPanel5_Enter
	.word gGridPanel6Dimensions, GridPanel6_Move, GridPanel6_Enter
