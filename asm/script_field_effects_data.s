#include "constants/abilities.h"
#include "constants/species.h"
#include "constants/sndseq.h"
#include "constants/items.h"
#include "constants/pokemon.h"
#include "constants/std_script.h"
	.include "asm/macros.inc"
	.public ShowLighthouseLightUp
	.public LighthouseLightUp_CreateTaskEnv
	.public Task_ShowLighthouseLightUp
	.public LighthouseLightUp_Load
	.public LighthouseLightUp_Unload
	.public LighthouseLightUp_Animate
	.public FieldAnimModel_New
	.public FieldAnimModel_Delete
	.public FieldAnimModel_SetAnimations
	.public FieldAnimModel_Animate
	.public FieldAnimModel_AnimateLoop
	.public ShowTrapEffect
	.public TrapEffect_CreateTaskEnv
	.public Task_ShowTrapEffect
	.public ShowLegendaryWing
	.public LegendaryWing_CreateTaskEnv
	.public Task_ShowLegendaryWing
	.public LegendaryWing_AnimateLoop
	.public ShowHideoutAlarm
	.public HideoutAlarm_CreateTaskEnv
	.public Task_ShowHideoutAlarm
	.public HideoutAlarm_Load
	.public HideoutAlarm_Unload
	.public HideoutAlarm_Flash
	.public CameraShake_New
	.public CameraShake_Delete
	.public CameraShake_Start
	.public CameraShake_RestoreCamera
	.public CameraShake_Update
	.public ShakeScreen
	.public ScreenShake_CreateTaskEnv
	.public Task_ShakeScreen
	.public OpenAlphHiddenRoom
	.public AlphHiddenRoom_CreateTaskEnv
	.public Task_OpenAlphHiddenRoom
	.public AlphHiddenRoom_ShowMessage
	.public AlphHiddenRoom_HideMessage
	.public AlphHiddenRoom_IsAButtonPressed
	.public LanceFliesAway
	.public LanceFliesAway_CreateTaskEnv
	.public Task_LanceFliesAway
	.public sElectricTrapAnims
	.public sPoisonTrapAnims
	.public sLegendaryWingIntroAnims1
	.public sLegendaryWingLoopAnims1
	.public sLegendaryWingLoopAnims0
	.public sLegendaryWingEndAnims0
	.public sLegendaryWingEndAnims1
	.public sLegendaryWingIntroAnims0
	.public sRockTrapAnims
	.public sAlphHiddenRoomWindowTemplate
	.public sLegendaryWingModels
	.public sTrapEffects
	.public sLegendaryWingLoopAnimLists
	.public sLegendaryWingEndAnimLists
	.include "overlay_02.inc"
	.include "global.inc"

	.text
	.rodata

sElectricTrapAnims:
	.byte 0x0B, 0x00

sPoisonTrapAnims:
	.byte 0x04, 0x00
	.byte 0x05, 0x00

sLegendaryWingIntroAnims1:
	.byte 0x43, 0x00, 0x42, 0x00, 0x44, 0x00

sLegendaryWingLoopAnims1:
	.byte 0x45, 0x00, 0x46, 0x00, 0x47, 0x00

sLegendaryWingLoopAnims0:
	.byte 0x3B, 0x00, 0x3C, 0x00, 0x3D, 0x00

sLegendaryWingEndAnims0:
	.byte 0x3E, 0x00, 0x3F, 0x00, 0x40, 0x00

sLegendaryWingEndAnims1:
	.byte 0x48, 0x00, 0x49, 0x00, 0x4A, 0x00

sLegendaryWingIntroAnims0:
	.byte 0x39, 0x00, 0x38, 0x00, 0x3A, 0x00

sRockTrapAnims:
	.byte 0x07, 0x00, 0x08, 0x00, 0x09, 0x00

sAlphHiddenRoomWindowTemplate: ; 0x0225324C
	.byte 0x01, 0x09, 0x0A, 0x0E
	.byte 0x02, 0x00, 0x80, 0x00

sLegendaryWingModels: ; 0x02253254
	.byte 0xAE, 0x00, 0x37, 0x03
	.word sLegendaryWingIntroAnims0
	.byte 0xAE, 0x00, 0x41, 0x03
	.word sLegendaryWingIntroAnims1

sTrapEffects: ; 0x02253264
	.byte 0xAE, 0x00, 0x0A, 0x01
	.word sElectricTrapAnims
	.short SEQ_SE_GS_DENKI_TRAP
	.balign 4
	.byte 0xAE, 0x00, 0x03, 0x02
	.word sPoisonTrapAnims
	.short SEQ_SE_GS_DOKU_TRAP
	.balign 4
	.byte 0xAE, 0x00, 0x06, 0x03
	.word sRockTrapAnims
	.short SEQ_SE_GS_IWA_TRAP
	.balign 4

	.data

sLegendaryWingLoopAnimLists: ; 0x02253D80
	.word sLegendaryWingLoopAnims0

sLegendaryWingEndAnimLists: ; 0x02253D84
	.word sLegendaryWingEndAnims0
	.word sLegendaryWingLoopAnims1
	.word sLegendaryWingEndAnims1
