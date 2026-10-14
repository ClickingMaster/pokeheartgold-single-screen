#include "constants/map_sections.h"
	.include "asm/macros.inc"
	.include "map_section_data.inc"
	.include "global.inc"

	.public gMapsecTypeStarts

	.rodata

gMapsecTypeStarts:
	.short MAPSEC_MYSTERY_ZONE
	.short METLOC_DAY_CARE_COUPLE
	.short METLOC_LOVELY_PLACE
