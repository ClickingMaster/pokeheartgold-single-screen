#include "constants/species.h"
#include "constants/sndseq.h"
#include "msgdata/msg/msg_0666.h"
	.include "asm/macros.inc"
	.include "field_misc_util_data.inc"
	.include "global.inc"

	.rodata
	.public sRegiSpecies

sRegiSpecies:
	.short SPECIES_REGIROCK
	.short SPECIES_REGICE
	.short SPECIES_REGISTEEL
