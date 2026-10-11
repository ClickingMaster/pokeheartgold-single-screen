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
	.include "overlay_01_02203E40.inc"
	.include "global.inc"

	.public FlashDarkness_New
	.public FlashDarkness_Free
	.public FlashDarkness_LoadModel
	.public FlashDarkness_FreeModel
	.public FlashDarkness_Show
	.public FlashDarkness_SetScale
	.public FlashDarknessEffect_Init
	.public FlashDarknessEffect_Destroy
	.public FlashDarknessEffect_Update
	.public FlashDarknessEffect_Draw
	.public ov01_022095B4
	.public ov01_022095C8

	.rodata

ov01_022095B4: ; 0x022095B4
	.word 0x00000020
	.word FlashDarknessEffect_Init
	.word FlashDarknessEffect_Destroy
	.word FlashDarknessEffect_Update
	.word FlashDarknessEffect_Draw
