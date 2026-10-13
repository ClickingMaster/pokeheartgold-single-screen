#include <nitro/fs/overlay.h>
	.include "asm/macros.inc"
	.include "global.inc"

	.public ov01_022067D0
	.public sParticleBaseScale
	.public sParticleGraphicsModes
	.public sParticleCaptureBgTemplate
	.public sParticleGraphicsBanks
	.public sEncounterEffectFuncs
	.public sBlockWipeColumnOrder
	.public gEncounterEffectState
	.public EncounterEffect_Champion
	.public EncounterEffect_EliteFourBruno
	.public EncounterEffect_EliteFourKaren
	.public EncounterEffect_EliteFourKoga
	.public EncounterEffect_EliteFourWill
	.public EncounterEffect_ExecutiveArcher
	.public EncounterEffect_ExecutiveAriana
	.public EncounterEffect_ExecutivePetrel
	.public EncounterEffect_ExecutiveProton
	.public EncounterEffect_KimonoGirl
	.public EncounterEffect_LeaderBlaine
	.public EncounterEffect_LeaderBlue
	.public EncounterEffect_LeaderBrock
	.public EncounterEffect_LeaderBugsy
	.public EncounterEffect_LeaderChuck
	.public EncounterEffect_LeaderClair
	.public EncounterEffect_LeaderErika
	.public EncounterEffect_LeaderFalkner
	.public EncounterEffect_LeaderJanine
	.public EncounterEffect_LeaderJasmine
	.public EncounterEffect_LeaderLtSurge
	.public EncounterEffect_LeaderMisty
	.public EncounterEffect_LeaderMorty
	.public EncounterEffect_LeaderPryce
	.public EncounterEffect_LeaderSabrina
	.public EncounterEffect_LeaderWhitney
	.public EncounterEffect_PkmnTrainerRed
	.public EncounterEffect_Rival
	.public EncounterEffect_RocketBoss
	.public EncounterEffect_TeamRocket
	.public EncounterEffect_TrainerBallDrop
	.public EncounterEffect_TrainerBallFall
	.public EncounterEffect_TrainerFadeSpin
	.public EncounterEffect_TrainerRisingBall
	.public EncounterEffect_TrainerSpinApart
	.public EncounterEffect_TrainerSpinZoom
	.public EncounterEffect_WildDiagonalsBlack
	.public EncounterEffect_WildDiagonalsWhite
	.public EncounterEffect_WildRowsBlack
	.public EncounterEffect_WildRowsWhite
	.public EncounterEffect_WildWaveBlack
	.public EncounterEffect_WildWaveWhite
	.public EncounterEffect_FrontierOrLinkSingle
	.public EncounterEffect_Double
	.public EncounterEffect_Legendary
	.public EncounterEffect_HoOh
	.public EncounterEffect_Lugia

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
	.word EncounterEffect_Legendary
	.word EncounterEffect_HoOh
	.word EncounterEffect_Lugia
	.word EncounterEffect_FrontierOrLinkSingle
	.word EncounterEffect_Double
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
