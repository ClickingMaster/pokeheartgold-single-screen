#include <nitro/fs/overlay.h>
	.include "asm/macros.inc"
	.include "global.inc"

	.public sPlayerApricornLadderBillboardMotions
	.public gPlayerWalkSlightlyFasterFrameSteps
	.public gPlayerFishingBillboardMotions
	.public gPlayerFishingSequences
	.public gWalkSlightlyFasterFrameSteps
	.public gPlayerLadderSequences
	.public gPlayerApricornShakeSequences
	.public gPlayerCyclingStandSequences
	.public gPlayerWalkSlightlyFastFrameSteps
	.public gWalkSlightlyFastFrameSteps
	.public gWalkEverSoSlightlyFastFrameSteps
	.public gPlayerWalkEverSoSlightlyFastFrameSteps
	.public gNpcBillboardMotions
	.public gPlayerCyclingBillboardMotions
	.public gPcWomanBillboardMotions
	.public gPlayerWalkingBillboardMotions
	.public BillboardMotion_Stand
	.public BillboardMotion_AnimateHalfSpeed
	.public BillboardMotion_AnimateNormalSpeed
	.public BillboardMotion_AnimateDoubleSpeed
	.public BillboardMotion_AnimateQuadSpeed
	.public BillboardMotion_WalkSlightlyFast
	.public BillboardMotion_WalkSlightlyFaster
	.public BillboardMotion_WalkEverSoSlightlyFast
	.public BillboardMotion_PcWomanStand
	.public BillboardMotion_PcWomanBow
	.public PlayerBillboardMotion_Stand
	.public PlayerBillboardMotion_AnimateHalfSpeed
	.public PlayerBillboardMotion_AnimateNormalSpeed
	.public PlayerBillboardMotion_AnimateDoubleSpeed
	.public PlayerBillboardMotion_AnimateQuadSpeed
	.public PlayerBillboardMotion_WalkSlightlyFast
	.public PlayerBillboardMotion_WalkSlightlyFaster
	.public PlayerBillboardMotion_WalkEverSoSlightlyFast
	.public PlayerBillboardMotion_Run
	.public PlayerBillboardMotion_CyclingStand
	.public PlayerBillboardMotion_CyclingAnimateHalfSpeed
	.public PlayerBillboardMotion_CyclingAnimateNormalSpeed
	.public PlayerBillboardMotion_FishingIdle
	.public PlayerBillboardMotion_FishingCast
	.public PlayerBillboardMotion_FishingBite
	.public PlayerBillboardMotion_FishingReel
	.public PlayerBillboardMotion_Ladder0
	.public PlayerBillboardMotion_Ladder1
	.public PlayerBillboardMotion_ApricornShake

	.text
	.rodata

sPlayerApricornLadderBillboardMotions: ; 0x022089CC
	.word PlayerBillboardMotion_ApricornShake
	.word PlayerBillboardMotion_Ladder0
	.word PlayerBillboardMotion_Ladder1

gPlayerWalkSlightlyFasterFrameSteps: ; 0x022089D8
	.byte 0x00, 0x30, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00
	.byte 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

gPlayerFishingBillboardMotions: ; 0x022089E8
	.word PlayerBillboardMotion_FishingIdle
	.word PlayerBillboardMotion_FishingCast
	.word PlayerBillboardMotion_FishingBite
	.word PlayerBillboardMotion_FishingReel

gPlayerFishingSequences: ; 0x022089F8
	.byte 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00
	.byte 0x02, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00

gWalkSlightlyFasterFrameSteps: ; 0x02208A08
	.byte 0x00, 0x30, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00
	.byte 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

gPlayerLadderSequences: ; 0x02208A18
	.byte 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00
	.byte 0x02, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00

gPlayerApricornShakeSequences: ; 0x02208A28
	.byte 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00
	.byte 0x02, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00

gPlayerCyclingStandSequences: ; 0x02208A38
	.byte 0x06, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00
	.byte 0x08, 0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00

gPlayerWalkSlightlyFastFrameSteps: ; 0x02208A48
	.byte 0x00, 0x10, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00
	.byte 0x00, 0x20, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00

gWalkSlightlyFastFrameSteps: ; 0x02208A64
	.byte 0x00, 0x10, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00
	.byte 0x00, 0x10, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

gWalkEverSoSlightlyFastFrameSteps: ; 0x02208A80
	.byte 0x00, 0x20, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00
	.byte 0x00, 0x10, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

gPlayerWalkEverSoSlightlyFastFrameSteps: ; 0x02208AA0
	.byte 0x00, 0x20, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00
	.byte 0x00, 0x10, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

gNpcBillboardMotions: ; 0x02208AC0
	.word BillboardMotion_Stand
	.word BillboardMotion_AnimateHalfSpeed
	.word BillboardMotion_AnimateHalfSpeed
	.word BillboardMotion_AnimateNormalSpeed
	.word BillboardMotion_AnimateDoubleSpeed
	.word BillboardMotion_AnimateQuadSpeed
	.word BillboardMotion_WalkSlightlyFast
	.word BillboardMotion_WalkSlightlyFaster
	.word BillboardMotion_WalkEverSoSlightlyFast

gPlayerCyclingBillboardMotions: ; 0x02208AE4
	.word PlayerBillboardMotion_CyclingStand
	.word PlayerBillboardMotion_AnimateHalfSpeed
	.word PlayerBillboardMotion_AnimateHalfSpeed
	.word PlayerBillboardMotion_AnimateNormalSpeed
	.word PlayerBillboardMotion_AnimateDoubleSpeed
	.word PlayerBillboardMotion_WalkSlightlyFast
	.word PlayerBillboardMotion_WalkSlightlyFast
	.word PlayerBillboardMotion_WalkSlightlyFast
	.word PlayerBillboardMotion_CyclingAnimateHalfSpeed
	.word PlayerBillboardMotion_CyclingAnimateNormalSpeed

gPcWomanBillboardMotions: ; 0x02208B0C
	.word BillboardMotion_PcWomanStand
	.word BillboardMotion_AnimateHalfSpeed
	.word BillboardMotion_AnimateHalfSpeed
	.word BillboardMotion_AnimateNormalSpeed
	.word BillboardMotion_AnimateDoubleSpeed
	.word BillboardMotion_AnimateQuadSpeed
	.word BillboardMotion_WalkSlightlyFast
	.word BillboardMotion_WalkSlightlyFaster
	.word BillboardMotion_WalkEverSoSlightlyFast
	.word BillboardMotion_PcWomanBow

gPlayerWalkingBillboardMotions: ; 0x02208B34
	.word PlayerBillboardMotion_Stand
	.word PlayerBillboardMotion_AnimateHalfSpeed
	.word PlayerBillboardMotion_AnimateHalfSpeed
	.word PlayerBillboardMotion_AnimateNormalSpeed
	.word PlayerBillboardMotion_AnimateDoubleSpeed
	.word PlayerBillboardMotion_AnimateQuadSpeed
	.word PlayerBillboardMotion_WalkSlightlyFast
	.word PlayerBillboardMotion_WalkSlightlyFaster
	.word PlayerBillboardMotion_WalkEverSoSlightlyFast
	.word PlayerBillboardMotion_Run
