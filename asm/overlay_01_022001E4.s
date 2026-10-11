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
	.include "overlay_01_022001E4.inc"
	.include "global.inc"

	.public EmoteMark_New
	.public EmoteMark_Free
	.public EmoteMark_RetainExclamation
	.public EmoteMark_ReleaseExclamation
	.public EmoteMark_RetainQuestion
	.public EmoteMark_ReleaseQuestion
	.public EmoteMark_Retain
	.public EmoteMark_Release
	.public EmoteMark_LoadExclamationGfx
	.public EmoteMark_LoadQuestionGfx
	.public EmoteMark_FreeExclamationGfx
	.public EmoteMark_FreeQuestionGfx
	.public EmoteMark_LoadExclamationGfxIfUnused
	.public EmoteMark_LoadQuestionGfxIfUnused
	.public EmoteMark_FreeExclamationGfxIfUnused
	.public EmoteMark_FreeQuestionGfxIfUnused
	.public EmoteMark_LoadGfxIfUnused
	.public EmoteMark_FreeGfxIfUnused
	.public EmoteMark_CreateBillboard
	.public EmoteMark_IsDone
	.public EmoteMark_Hide
	.public EmoteMarkEffect_Init
	.public EmoteMarkEffect_Update
	.public EmoteMarkEffect_Destroy
	.public EmoteMarkEffect_Draw
	.public EmoteMark_Spawn
	.public MapObjectEmoteMarkEffect_Init
	.public MapObjectEmoteMarkEffect_Update
	.public ov01_022092C8
	.public ov01_022092DC
	.public ov01_022092F0

	.rodata

ov01_022092C8:
	.word 0x00000044
	.word EmoteMarkEffect_Init
	.word EmoteMarkEffect_Destroy
	.word EmoteMarkEffect_Update
	.word EmoteMarkEffect_Draw

ov01_022092DC: ; 0x022092DC
	.word 0x00000044
	.word MapObjectEmoteMarkEffect_Init
	.word EmoteMarkEffect_Destroy
	.word MapObjectEmoteMarkEffect_Update
	.word EmoteMarkEffect_Draw

ov01_022092F0: ; 0x022092F0
	.byte 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00
