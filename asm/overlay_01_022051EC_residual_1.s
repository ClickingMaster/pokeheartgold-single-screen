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
	.public ov01_022096CC
	.include "overlay_01_022051EC.inc"
	.include "global.inc"

	.text
	.public ShayminFormChange_New
	.public ShayminFormChange_Free
	.public ShayminFormChange_LoadModel
	.public ShayminFormChange_FreeModel
	.public ShayminFormChange_Spawn
	.public ShayminFormChange_IsDone
	.public ShayminFormChangeEffect_Init
	.public ShayminFormChangeEffect_Update
	.public ShayminFormChangeEffect_Destroy
	.public ShayminFormChangeEffect_Draw
	.public ShayminFormChange_AdvanceAnimations
	.public ShayminFormChange_SetAnimationsFrame
	.rodata

ov01_022096CC: ; 0x022096CC
	.word 0x00000008
	.word ShayminFormChangeEffect_Init
	.word ShayminFormChangeEffect_Destroy
	.word ShayminFormChangeEffect_Update
	.word ShayminFormChangeEffect_Draw
