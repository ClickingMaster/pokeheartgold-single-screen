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
	.include "frontier_field_support_data.inc"
	.include "global.inc"

	.text


	.rodata

	.public gFrontierSummaryPages
	.public gFrontierSummaryPages
gFrontierSummaryPages:
	.byte 0x00, 0x01, 0x02, 0x03, 0x04
	.balign 4, 0
