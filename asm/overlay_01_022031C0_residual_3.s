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
	.public FollowMonBallEffect_UpdateRecallWhite
	.public FollowMonBallEffect_UpdateRecall
	.public ov01_022094B0
	.public ov01_022094BC
	.public ov01_022094C8
	.public ov01_022094DC
	.public ov01_022094F0
	.public ov01_02209504
	.public ov01_02209B18
	.include "overlay_01_022031C0.inc"
	.include "global.inc"

	.text
	.public FollowMonBall_New
	.public FollowMonBall_Free
	.public FollowMonBall_LoadModels
	.public FollowMonBall_FreeModels
	.public FollowMonBall_Spawn
	.public FollowMonBallEffect_InitBurst
	.public FollowMonBallEffect_Destroy
	.public FollowMonBallEffect_UpdateRelease
	.public FollowMonBallEffect_DrawRelease
	.public FollowMonBallEffect_UpdateBurst
	.public FollowMonBallEffect_DrawBurst
	.public FollowMonBallEffect_InitRecallWhite
	.public FollowMonBallEffect_DrawBall
	.public FollowMonBallEffect_InitRecall
	.public FollowMonBall_GetPullCoordinate
	.public FollowMonBall_GetPullStep
	.rodata

ov01_022094B0: ; 0x022094B0
	.word 0x1000, 0x1000, 0x1000

ov01_022094BC: ; 0x022094BC
	.word 0x1000, 0x1000, 0x1000

ov01_022094C8: ; 0x022094C8
	.word 0x00000054
	.word FollowMonBallEffect_InitRecall
	.word FollowMonBallEffect_Destroy
	.word FollowMonBallEffect_UpdateRecall
	.word FollowMonBallEffect_DrawBall

ov01_022094DC: ; 0x022094DC
	.word 0x00000054
	.word FollowMonBallEffect_InitBurst
	.word FollowMonBallEffect_Destroy
	.word FollowMonBallEffect_UpdateRelease
	.word FollowMonBallEffect_DrawRelease

ov01_022094F0: ; 0x022094F0
	.word 0x00000054
	.word FollowMonBallEffect_InitRecallWhite
	.word FollowMonBallEffect_Destroy
	.word FollowMonBallEffect_UpdateRecallWhite
	.word FollowMonBallEffect_DrawBall

ov01_02209504: ; 0x02209504
	.word 0x00000054
	.word FollowMonBallEffect_InitBurst
	.word FollowMonBallEffect_Destroy
	.word FollowMonBallEffect_UpdateBurst
	.word FollowMonBallEffect_DrawBurst

	.data

	.balign 4, 0
ov01_02209B18: ; 0x02209B18
	.byte 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
	.byte 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
	.byte 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
	.byte 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
	.byte 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
