	.public gEvolutionSummaryPages
#include "constants/items.h"
#include "constants/moves.h"
#include "constants/pokemon.h"
	.include "asm/macros.inc"
	.include "evolution_scene_data.inc"
	.include "global.inc"

	.rodata

gEvolutionSummaryPages:
	.byte 0x01, 0x04, 0x00, 0x00
.text
