#include "constants/sndseq.h"
#include "constants/moves.h"
#include "constants/std_script.h"
#include "constants/species.h"
#include "constants/sprites.h"
#include "constants/maps.h"
#include "constants/mmodel.h"
#include "constants/items.h"
#include "msgdata/msg/msg_0096_D31R0201.h"
#include "msgdata/msg/msg_0066_D23R0102.h"
	.include "asm/macros.inc"
	.include "field_emote_mark_data.inc"
	.include "global.inc"

	.public EmoteMarkEffect_Init
	.public EmoteMarkEffect_Update
	.public EmoteMarkEffect_Destroy
	.public EmoteMarkEffect_Draw
	.public MapObjectEmoteMarkEffect_Init
	.public MapObjectEmoteMarkEffect_Update
	.public sEmoteMarkTemplate
	.public gMapObjectEmoteMarkTemplate
	.public gEmoteMarkRecordParams

	.rodata

sEmoteMarkTemplate:
	.word 0x00000044
	.word EmoteMarkEffect_Init
	.word EmoteMarkEffect_Destroy
	.word EmoteMarkEffect_Update
	.word EmoteMarkEffect_Draw

gMapObjectEmoteMarkTemplate:
	.word 0x00000044
	.word MapObjectEmoteMarkEffect_Init
	.word EmoteMarkEffect_Destroy
	.word MapObjectEmoteMarkEffect_Update
	.word EmoteMarkEffect_Draw

gEmoteMarkRecordParams:
	.byte 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00
