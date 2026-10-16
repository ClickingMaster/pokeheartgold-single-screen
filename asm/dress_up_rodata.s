	.include "asm/macros.inc"
	.public AccessoryPortrait_Exit
	.public AccessoryPortrait_Init
	.public AccessoryPortrait_Main
	.public sDressUpGraphicsModes
	.public DressUpGfx_CreateSlotList
	.public DressUpGfx_FreeSlotList
	.public DressUpGfx_InitDisplay
	.public DressUpGfx_ResetDisplay
	.public DressUpGfx_UploadRawResTables
	.public DressUpGfx_FreeAllTexAndPltt
	.public DressUpGfx_CreateSprite
	.public DressUpGfx_LoadCharRes
	.public DressUpGfx_LoadPlttRes
	.public DressUpGfx_LoadCellRes
	.public DressUpGfx_LoadAnimRes
	.public DressUpGfx_FreeCharRes
	.public DressUpGfx_FreePlttRes
	.public DressUpGfx_FreeCellRes
	.public DressUpGfx_FreeAnimRes
	.public DressUpGfx_DestroyRawResTables
	.public AccessoryPortrait_AllocRawResTables
	.public AccessoryPortrait_InitGfxCore
	.public AccessoryPortrait_InitBg
	.public AccessoryPortrait_FreeGfxCore
	.public AccessoryPortrait_FreeBg
	.public DressUpGfx_HandlePokepicLoad
	.public AccessoryPortrait_DrawTransformed
	.public AccessoryPortrait_InitAppGfx
	.public AccessoryPortrait_FreeAppGfx
	.public AccessoryPortrait_AppVBlank
	.public AccessoryPortrait_RenderSprites
	.public DressUp_Init
	.public DressUp_Main
	.public DressUp_Exit
	.public DressUp_InitHud
	.public DressUpCanvas_Init
	.public DressUpCanvas_Destroy
	.public AccessoryPortrait_CreateCanvasMonPic
	.public AccessoryPortrait_PlaceCanvasAccessory
	.public DressUpCanvas_ClearObjects
	.public DressUpCanvas_SetOrigin
	.public DressUpCanvas_SetBackdrop
	.public DressUpCanvas_FreeBackdrop
	.public DressUpMon_GetPriority
	.public DressUpMon_GetCenterPos
	.public DressUpNodePool_Init
	.public DressUpNodePool_Free
	.public DressUpHud_Init
	.public DressUp_StartBackdropAnim
	.public DressUp_UpdateBackdropAnim
	.public AccessoryPortrait_Create
	.public AccessoryPortrait_Draw
	.public AccessoryPortrait_Destroy
	.public AccessoryPortrait_VBlank
	.public AccessoryPortrait_SetMonVisible
	.public sDressUpObjCharTransferTemplate
	.public sDressUpGraphicsManagerTemplate
	.public sDressUpBgTemplate_Sub0
	.public sDressUpBgTemplate_Main1
	.public sDressUpBgTemplate_Main2
	.public sAccessoryPortraitBgTemplate_Main2
	.public sDressUpBgTemplate_Main3
	.public sDressUpBgTemplate_Sub1
	.public sDressUpGraphicsBanks
	.public sDressUpKeyPressTouchStates
	.include "overlay_41.inc"
	.include "global.inc"

	.text
	.public DressUpGfx_CreateSlotList
	.public DressUpGfx_FreeSlotList
	.public DressUpGfx_InitDisplay
	.public DressUpGfx_ResetDisplay
	.public DressUpGfx_UploadRawResTables
	.public DressUpGfx_FreeAllTexAndPltt
	.public DressUpGfx_LoadCharRes
	.public DressUpGfx_LoadCellRes
	.public DressUpGfx_LoadAnimRes
	.public DressUpGfx_FreeCharRes
	.public DressUpGfx_FreePlttRes
	.public DressUpGfx_FreeCellRes
	.public DressUpGfx_FreeAnimRes
	.public DressUpGfx_DestroyRawResTables
	.public AccessoryPortrait_AllocRawResTables
	.public AccessoryPortrait_FreeBg
	.public DressUpGfx_HandlePokepicLoad
	.public AccessoryPortrait_FreeAppGfx
	.public AccessoryPortrait_AppVBlank
	.public AccessoryPortrait_RenderSprites
	.public DressUpCanvas_Destroy
	.public DressUpCanvas_ClearObjects
	.public DressUpCanvas_FreeBackdrop
	.public DressUpMon_GetPriority
	.public DressUpMon_GetCenterPos
	.public DressUpNodePool_Init
	.public DressUpNodePool_Free
	.public DressUp_UpdateBackdropAnim
	.public AccessoryPortrait_VBlank
	.public AccessoryPortrait_SetMonVisible


	.rodata

sDressUpGraphicsModes:
	.byte 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x01, 0x00, 0x00, 0x00

sDressUpObjCharTransferTemplate: ; 0x0224BFA4
	.byte 0x08, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00
	.byte 0x0E, 0x00, 0x00, 0x00

sDressUpGraphicsManagerTemplate: ; 0x0224BFB4
	.byte 0xCE, 0x02, 0x00, 0x00, 0x76, 0x00, 0x00, 0x00, 0x13, 0x00, 0x00, 0x00
	.byte 0x0E, 0x00, 0x00, 0x00

sDressUpBgTemplate_Sub0: ; 0x0224BFC4
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x0F, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

sDressUpBgTemplate_Main1: ; 0x0224BFE0
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x01, 0x00, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

sDressUpBgTemplate_Main2: ; 0x0224BFFC
	.byte 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x1E, 0x01
	.byte 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

sAccessoryPortraitBgTemplate_Main2: ; 0x0224C018
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x1E, 0x01, 0x00, 0x02, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00

sDressUpBgTemplate_Main3: ; 0x0224C034
	.byte 0x00, 0x00, 0x00, 0x00, 0x6F, 0xFF, 0xFF, 0xFF, 0x00, 0x08, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x1D, 0x02, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

sDressUpBgTemplate_Sub1: ; 0x0224C050
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x01, 0x00, 0x0E, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

sDressUpGraphicsBanks: ; 0x0224C06C
	.byte 0x04, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00
	.byte 0x10, 0x00, 0x00, 0x00

sDressUpKeyPressTouchStates: ; 0x0224C094
	.byte 0x02, 0x01, 0x00, 0x00
