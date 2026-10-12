	.public ov01_02208B64
	.public ov01_02208B70
	.public ov01_02208B80
	.public ov01_02208B90
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
	.public MapObjectRender_GetBillboardResourceSlot
	.public MapObjectRender_GetBillboardResource
	.public MapObjectRender_ReleaseBillboardResource
	.public MapObjectRender_ReleaseAllBillboardResources
	.public MapObjectManager_IsSpriteUsedByOtherActiveObject
	.public MapObjectRender_CreateResourceBanks
	.public MapObjectRender_LoadIntoBank
	.public MapObjectRender_LoadSpriteTexture
	.public MapObjectRender_LoadResidentSpriteTextures
	.public MapObjectRender_AddDynamicModelId
	.public MapObjectRender_IsDynamicModel
	.public MapObjectRender_RemoveDynamicModelId
	.public MapObjectRender_FreeUnusedModels
	.public MapObjectRender_AddDynamicSequenceId
	.public MapObjectRender_IsDynamicSequence
	.public MapObjectRender_RemoveDynamicSequenceId
	.public MapObjectRender_FreeUnusedSequences
	.public MapObjectRender_AddDynamicTextureId
	.public MapObjectRender_RemoveDynamicTextureId
	.public MapObjectRender_FreeUnusedTextures
	.public MapObjectRender_IsDynamicTexture
	.public MapObjectManager_IsSpriteInUse
	.public MapObjectManager_IsModelInUse
	.public MapObjectManager_IsSequenceInUse
	.public MapObjectRender_ReleaseSpriteResources
	.public MapObjectBillboards_CreateBillboard
	.public ov01_021FA370
	.public ov01_021FA470
	.public ov01_021FA564
	.public ov01_021FA668
	.public MapObjectRender_CancelTextureTransfer
	.public ov01_021FA6E0
	.public ov01_021FA71C
	.public MapObjectRender_QueueBillboard
	.public ov01_021FA854
	.public ov01_021FA97C
	.public ov01_021FAB9C
	.public SpriteId_GetBillboardInfo
	.include "overlay_01_021F944C.inc"
	.include "global.inc"

.text
	.rodata

ov01_02208B64: ; 0x02208B64
	.byte 0x00, 0x10, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00

ov01_02208B70: ; 0x02208B70
	.byte 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00

ov01_02208B80: ; 0x02208B80
	.byte 0x04, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00

ov01_02208B90: ; 0x02208B90
	.byte 0x04, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00
