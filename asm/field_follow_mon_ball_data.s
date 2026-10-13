#include <nitro/fs/overlay.h>
	.include "asm/macros.inc"
	.include "global.inc"

	.public sFollowMonBallRecallScale
	.public sFollowMonBallRecallWhiteScale
	.public sFollowMonBallRecallTemplate
	.public sFollowMonBallReleaseTemplate
	.public sFollowMonBallRecallWhiteTemplate
	.public sFollowMonBallBurstTemplate
	.public sFollowMonWhitePalette
	.public FollowMonBallEffect_Destroy
	.public FollowMonBallEffect_DrawBall
	.public FollowMonBallEffect_DrawBurst
	.public FollowMonBallEffect_DrawRelease
	.public FollowMonBallEffect_InitBurst
	.public FollowMonBallEffect_InitRecall
	.public FollowMonBallEffect_InitRecallWhite
	.public FollowMonBallEffect_UpdateBurst
	.public FollowMonBallEffect_UpdateRecall
	.public FollowMonBallEffect_UpdateRecallWhite
	.public FollowMonBallEffect_UpdateRelease

	.text
	.rodata

sFollowMonBallRecallScale: ; 0x022094B0
	.word 0x1000, 0x1000, 0x1000

sFollowMonBallRecallWhiteScale: ; 0x022094BC
	.word 0x1000, 0x1000, 0x1000

sFollowMonBallRecallTemplate: ; 0x022094C8
	.word 0x00000054
	.word FollowMonBallEffect_InitRecall
	.word FollowMonBallEffect_Destroy
	.word FollowMonBallEffect_UpdateRecall
	.word FollowMonBallEffect_DrawBall

sFollowMonBallReleaseTemplate: ; 0x022094DC
	.word 0x00000054
	.word FollowMonBallEffect_InitBurst
	.word FollowMonBallEffect_Destroy
	.word FollowMonBallEffect_UpdateRelease
	.word FollowMonBallEffect_DrawRelease

sFollowMonBallRecallWhiteTemplate: ; 0x022094F0
	.word 0x00000054
	.word FollowMonBallEffect_InitRecallWhite
	.word FollowMonBallEffect_Destroy
	.word FollowMonBallEffect_UpdateRecallWhite
	.word FollowMonBallEffect_DrawBall

sFollowMonBallBurstTemplate: ; 0x02209504
	.word 0x00000054
	.word FollowMonBallEffect_InitBurst
	.word FollowMonBallEffect_Destroy
	.word FollowMonBallEffect_UpdateBurst
	.word FollowMonBallEffect_DrawBurst

	.data

	.balign 4, 0
sFollowMonWhitePalette: ; 0x02209B18
	.byte 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
	.byte 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
	.byte 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
	.byte 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
	.byte 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
