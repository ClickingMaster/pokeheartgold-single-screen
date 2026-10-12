	.public ov01_022067D0
	.public sParticleBaseScale
	.public sParticleGraphicsModes
	.public sParticleCaptureBgTemplate
	.public sParticleGraphicsBanks
	.public sEncounterEffectFuncs
	.public sBlockWipeColumnOrder
	.public gEncounterEffectState
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
	.public Task_RunEncounterEffect
	.public EncounterEffect_LoadOverlay
	.public EncounterEffect_UnloadOverlay
	.public Task_EncounterEffectFlash
	.public EncounterEffectQuadInterp_Init
	.public EncounterEffectQuadInterp_Update
	.public StripeWipe_New
	.public StripeWipe_Start
	.public Task_StripeWipe_Update
	.public StripeWipe_End
	.public HBlank_StripeWipe
	.public SplitWipe_New
	.public SplitWipe_Start
	.public Task_SplitWipe_Update
	.public HBlank_SplitWipe
	.public SplitWipe_End
	.public EncounterEffect_LoadBgGraphics
	.public EncounterEffect_LoadBgGraphicsMirrored
	.public EncounterEffect_LoadSpriteResources
	.public RectTrail_Update
	.public MovingRect_Update
	.public BlockWipe_Update
	.public ZigzagWipe_Start
	.public ZigzagWipe_End
	.public EncounterEffect_GetDefaultEffect
	.public EncounterEffect_StartParticleMode
	.public EncounterEffect_LoadParticles
	.public Task_ParticleSetup_Main
	.public EncounterEffect_SetupParticleGraphics
	.public EncounterEffectParticles_AllocTexVram
	.public EncounterEffectParticles_AllocPlttVram
	.public EncounterEffectHBlankWave_AlternateDirection
	.include "overlay_01_021EFB38.inc"
	.include "global.inc"

.text
	.rodata

ov01_022067D0:
	.byte 0xFF, 0xFB, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00

sParticleBaseScale: ; 0x022067E0
	.byte 0x00, 0x10, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00
	.byte 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00

sParticleGraphicsModes: ; 0x022067FC
	.byte 0x01, 0x00, 0x00, 0x00
	.byte 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x1D, 0x01
	.byte 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x1E, 0x02, 0x01, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00

sParticleCaptureBgTemplate: ; 0x02206844
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x1D, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x01, 0x01, 0x1F, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x17, 0x00
	.byte 0x2D, 0x00, 0x16, 0x00, 0x2D, 0x00, 0x44, 0x00, 0x5A, 0x00, 0x43, 0x00, 0x5B, 0x00, 0x71, 0x00
	.byte 0x87, 0x00, 0x70, 0x00, 0x87, 0x00, 0x9E, 0x00, 0xB4, 0x00, 0x9D, 0x00

sParticleGraphicsBanks: ; 0x0220689C
	.byte 0x06, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00
	.byte 0x10, 0x00, 0x00, 0x00

sEncounterEffectFuncs: ; 0x022068C4
	.word EncounterEffect_WildRowsWhite
	.word EncounterEffect_WildRowsBlack
	.word EncounterEffect_WildWaveWhite
	.word EncounterEffect_WildWaveBlack
	.word EncounterEffect_WildDiagonalsWhite
	.word EncounterEffect_WildDiagonalsBlack
	.word EncounterEffect_TrainerSpinApart
	.word EncounterEffect_TrainerSpinZoom
	.word EncounterEffect_TrainerBallDrop
	.word EncounterEffect_TrainerFadeSpin
	.word EncounterEffect_TrainerBallFall
	.word EncounterEffect_TrainerRisingBall
	.word EncounterEffect_LeaderFalkner
	.word EncounterEffect_LeaderBugsy
	.word EncounterEffect_LeaderWhitney
	.word EncounterEffect_LeaderMorty
	.word EncounterEffect_LeaderJasmine
	.word EncounterEffect_LeaderChuck
	.word EncounterEffect_LeaderPryce
	.word EncounterEffect_LeaderClair
	.word EncounterEffect_LeaderBrock
	.word EncounterEffect_LeaderMisty
	.word EncounterEffect_LeaderLtSurge
	.word EncounterEffect_LeaderErika
	.word EncounterEffect_LeaderJanine
	.word EncounterEffect_LeaderSabrina
	.word EncounterEffect_LeaderBlaine
	.word EncounterEffect_LeaderBlue
	.word EncounterEffect_Rival
	.word EncounterEffect_EliteFourWill
	.word EncounterEffect_EliteFourKoga
	.word EncounterEffect_EliteFourBruno
	.word EncounterEffect_EliteFourKaren
	.word EncounterEffect_Champion
	.word ov116_0225F054
	.word ov116_0225F364
	.word ov116_0225F374
	.word ov114_0225F020
	.word ov114_0225F280
	.word EncounterEffect_TeamRocket
	.word EncounterEffect_ExecutivePetrel
	.word EncounterEffect_ExecutiveProton
	.word EncounterEffect_ExecutiveArcher
	.word EncounterEffect_ExecutiveAriana
	.word EncounterEffect_RocketBoss
	.word EncounterEffect_KimonoGirl
	.word EncounterEffect_PkmnTrainerRed

sBlockWipeColumnOrder: ; 0x02206980
	.byte 0x00, 0x02, 0x05, 0x07, 0x01, 0x06, 0x03, 0x04

	.bss

gEncounterEffectState: ; 0x02209B64
	.space 0xC
