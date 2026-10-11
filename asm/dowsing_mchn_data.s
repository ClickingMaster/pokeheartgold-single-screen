	.include "asm/macros.inc"
	.include "overlay_28.inc"
	.include "global.inc"

	.public DowsingMchn_Init
	.public DowsingMchn_Exit
	.public DowsingMchn_IsExitDone
	.public DowsingMchn_SetVramBanks
	.public DowsingMchn_InitBgs
	.public DowsingMchn_FreeBgs
	.public DowsingMchn_LoadBgGraphics
	.public DowsingMchn_LoadStrings
	.public DowsingMchn_FreeStrings
	.public DowsingMchn_CreateWindows
	.public DowsingMchn_RemoveWindows
	.public DowsingMchn_CreateSprites
	.public DowsingMchn_DestroySprites
	.public DowsingMchn_CreateResourceManagers
	.public DowsingMchn_DestroyResourceManagers
	.public DowsingMchn_LoadSpriteResources
	.public DowsingMchn_CreateResourcesHeader
	.public DowsingMchn_CreateSprite
	.public DowsingMchn_CreateButtonSprites
	.public DowsingMchn_DestroyButtonSprites
	.public DowsingMchn_CreateIndicatorSprites
	.public DowsingMchn_DestroyIndicatorSprites
	.public DowsingMchn_Task
	.public DowsingMchn_GetHiddenItemLocs
	.public DowsingMchn_FreeHiddenItemLocs
	.public DowsingMchn_FindNearestHiddenItem
	.public DowsingMchn_GetIndicatorPos
	.public DowsingMchn_GetTouchAngle
	.public DowsingMchn_Atan2Idx
	.public DowsingMchn_AngleIdxToDegrees
	.public DowsingMchn_StepAngleLevel0
	.public DowsingMchn_SetTargetAnglesLevel0
	.public DowsingMchn_StepAngleLevel1
	.public DowsingMchn_SetTargetAnglesLevel1
	.public DowsingMchn_StepAngleLevel2
	.public DowsingMchn_SetTargetAnglesLevel2
	.public DowsingMchn_StepAngleLevel3
	.public DowsingMchn_SetTargetAnglesLevel3
	.public DowsingMchn_StepAngleLevel4
	.public DowsingMchn_SetTargetAnglesLevel4
	.public DowsingMchn_UpdateTargetAngles
	.public DowsingMchn_UpdateSignal
	.public DowsingMchn_GetIndicatorAnim
	.public DowsingMchn_UpdateIndicatorAppearance
	.public DowsingMchn_SetSpriteRotation
	.public DowsingMchn_UpdateIndicatorPositions
	.public DowsingMchn_GetGridCellTarget
	.public DowsingMchn_UpdateGrid
	.public DowsingMchn_PlaySignalSE
	.public DowsingMchn_UpdateTouchPos
	.public DowsingMchn_UpdateActionLabel
	.public DowsingMchn_HandleButton
	.public DowsingMchn_HandleActionButton
	.public DowsingMchn_HandleRunningShoesButton
	.public DowsingMchn_HandleExitButton
	.public DowsingMchn_ShouldHideButtons
	.public sDowsingMchnActionButtonHitbox
	.public sDowsingMchnExitButtonHitbox
	.public sDowsingMchnRunningShoesHitbox
	.public sDowsingMchnSearchAreaHitbox
	.public sDowsingMchnLevelSignalCaps
	.public sDowsingMchnLevelGridValues
	.public sDowsingMchnActionSpriteParams
	.public sDowsingMchnRunningShoesSpriteParams
	.public sDowsingMchnRunningShoesSprite2Params
	.public sDowsingMchnExitSpriteParams
	.public sDowsingMchnStepAngleFuncs
	.public sDowsingMchnBgTemplateSub0
	.public sDowsingMchnBgTemplateSub1
	.public sDowsingMchnBgTemplateSub2
	.public sDowsingMchnGridMask
	.public sDowsingMchnGridTiles
	.public sDowsingMchnWindowTemplates
	.public sDowsingMchnActionTypeToWindow
	.public sDowsingMchnLevelSignalGains

	.text


	.public sDowsingMchnActionButtonHitbox
	.public sDowsingMchnExitButtonHitbox
	.public sDowsingMchnRunningShoesHitbox
	.public sDowsingMchnSearchAreaHitbox
	.public sDowsingMchnLevelSignalCaps
	.public sDowsingMchnLevelGridValues
	.public sDowsingMchnActionSpriteParams
	.public sDowsingMchnRunningShoesSpriteParams
	.public sDowsingMchnRunningShoesSprite2Params
	.public sDowsingMchnExitSpriteParams
	.public sDowsingMchnStepAngleFuncs
	.public sDowsingMchnBgTemplateSub0
	.public sDowsingMchnBgTemplateSub1
	.public sDowsingMchnBgTemplateSub2
	.public sDowsingMchnGridMask
	.public sDowsingMchnGridTiles
	.public sDowsingMchnWindowTemplates
	.public sDowsingMchnActionTypeToWindow
	.public sDowsingMchnLevelSignalGains

	.rodata

sDowsingMchnActionButtonHitbox:
	.byte 0x90, 0xBC, 0x9D, 0xFF

sDowsingMchnExitButtonHitbox: ; 0x0225EA80
	.byte 0x08, 0x27, 0xC0, 0xFF

sDowsingMchnRunningShoesHitbox: ; 0x0225EA84
	.byte 0x56, 0x86, 0xB2, 0xFC

sDowsingMchnSearchAreaHitbox: ; 0x0225EA88
	.byte 0xFE, 0x54, 0x64, 0x48, 0xFF, 0x00, 0x00, 0x00

sDowsingMchnLevelSignalCaps: ; 0x0225EA90
	.byte 0x31

sDowsingMchnLevelGridValues: ; 0x0225EA91
	.byte 0x1D, 0x4F, 0x27, 0x6D, 0x31, 0x81, 0x31, 0x9F, 0x9F

sDowsingMchnActionSpriteParams: ; 0x0225EA9A
	.byte 0xA8, 0x90, 0x00, 0x05

sDowsingMchnRunningShoesSpriteParams: ; 0x0225EA9E
	.byte 0xB8, 0x56
	.byte 0x01, 0x03

sDowsingMchnRunningShoesSprite2Params: ; 0x0225EAA2
	.byte 0xD2, 0x5E, 0x00, 0x07

sDowsingMchnExitSpriteParams: ; 0x0225EAA6
	.byte 0xC8, 0x08, 0x00, 0x08, 0x00, 0x00

sDowsingMchnStepAngleFuncs: ; 0x0225EAAC
	.word DowsingMchn_StepAngleLevel0
	.word DowsingMchn_StepAngleLevel1
	.word DowsingMchn_StepAngleLevel2
	.word DowsingMchn_StepAngleLevel3
	.word DowsingMchn_StepAngleLevel4

sDowsingMchnBgTemplateSub0: ; 0x0225EAC0
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x01, 0x00, 0x0F, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

sDowsingMchnBgTemplateSub1: ; 0x0225EADC
	.byte 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x0E, 0x00
	.byte 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

sDowsingMchnBgTemplateSub2: ; 0x0225EAF8
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x0D, 0x00, 0x00, 0x02, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00

sDowsingMchnGridMask: ; 0x0225EB14
	.byte 0x1F, 0x7C, 0x07, 0x70, 0x03, 0x60, 0x01, 0x40, 0x01, 0x40, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x40, 0x01, 0x40, 0x03, 0x60, 0x07, 0x70
	.byte 0x1F, 0x7C

sDowsingMchnGridTiles: ; 0x0225EB32
	.byte 0x6D, 0x30, 0x6E, 0x20, 0x6F, 0x20, 0x70, 0x20, 0x71, 0x20, 0x72, 0x20, 0x73, 0x20
	.byte 0x74, 0x20, 0x75, 0x20, 0x76, 0x20, 0x77, 0x20, 0x78, 0x20, 0x79, 0x20, 0x7A, 0x20, 0x7B, 0x20
	.byte 0x7C, 0x20

sDowsingMchnWindowTemplates: ; 0x0225EB52
	.byte 0x04, 0x18, 0x14, 0x08, 0x02, 0x04, 0x11, 0x00, 0x04, 0x18, 0x14, 0x08, 0x02, 0x04
	.byte 0x01, 0x00, 0x04, 0x18, 0x14, 0x08, 0x02, 0x04, 0x21, 0x00, 0x04, 0x18, 0x14, 0x08, 0x02, 0x04
	.byte 0x31, 0x00, 0x04, 0x1A, 0x02, 0x06, 0x02, 0x04, 0x41, 0x00, 0x00, 0x00

sDowsingMchnActionTypeToWindow: ; 0x0225EB7C
	.byte 0x00, 0x01, 0x00, 0x02
	.byte 0x03, 0x00, 0x00, 0x00

sDowsingMchnLevelSignalGains: ; 0x0225EB84
	.byte 0x01, 0x02, 0x02, 0x02, 0x04, 0x00, 0x00, 0x00
	; 0x0225EBA0
