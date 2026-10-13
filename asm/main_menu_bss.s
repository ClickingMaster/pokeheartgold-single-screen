#include "config.h"
#include "msgdata/msg/msg_0247.h"
#include "msgdata/msg/msg_0442.h"
#include "constants/sndseq.h"
	.include "asm/macros.inc"
	.include "overlay_74_thumb.inc"
	.include "global.inc"
	.extern ConnectToRanger_Init
	.extern ConnectToRanger_Main
	.extern ConnectToRanger_Exit
	.extern MysteryGiftComm_HandleReceived
	.extern MysteryGiftComm_GetRecvBufferSize
	.extern MysteryGiftComm_GetRecvBuffer
	.extern ov74_02229DE0
	.extern ov74_02229E60
	.extern MysteryGiftApp_StartSearch
	.extern MysteryGiftApp_AcceptGift
	.extern MysteryGiftApp_DeclineSearch
	.extern MysteryGiftApp_SelectGetFromFriend
	.extern MysteryGiftApp_SelectGetViaWfc
	.extern MysteryGiftApp_SelectGetViaWireless
	.extern MysteryGiftApp_DeclineAcceptGift
	.extern MysteryGiftApp_ConfirmWirelessSearch
	.extern MysteryGiftApp_ConfirmReceiveGift
	.extern MysteryGiftApp_DeclineReceiveGift
	.extern MysteryGiftApp_ConfirmWfcSearch
	.extern MysteryGiftApp_CancelReceiveMenu
	.extern MysteryGiftApp_SelectReceiveGift
	.extern MysteryGiftApp_SelectCheckCard
	.extern MysteryGiftApp_SelectViewLockCapsuleCard
	.extern MysteryGiftApp_SelectExit
	.extern MysteryGiftApp_ShowWfcError
	.extern MysteryGiftApp_Init
	.extern MysteryGiftApp_Main
	.extern MysteryGiftApp_Exit
	.extern WonderCardAlbum_SelectShare
	.extern WonderCardAlbum_SelectTrash
	.extern WonderCardAlbum_ConfirmTrash
	.extern WonderCardAlbum_ReturnToCardMenu
	.extern WonderCardAlbum_PrintMessageOnly
	.extern WonderCardAlbum_PrintCardTitle
	.extern WonderCardAlbum_PrintGiftStatus
	.extern WonderCardAlbum_BufferReceiveDate
	.extern WonderCardAlbum_PrintCardText
	.extern WonderCardAlbum_PrintSharesLeft
	.extern WonderCardAlbum_ShowCardMenu
	.extern WonderCardAlbum_ShowTrashYesNo
	.extern WonderCardAlbum_ShowShareYesNo
	.extern ov74_0222D7D0
	.extern WonderCardAlbum_Init
	.extern WonderCardAlbum_Main
	.extern WonderCardAlbum_Exit
	.extern LockCapsuleCard_PrintMessageOnly
	.extern LockCapsuleCard_PrintCardTitle
	.extern LockCapsuleCard_PrintStatus
	.extern LockCapsuleCard_BufferReceiveDate
	.extern LockCapsuleCard_PrintCardText
	.extern LockCapsuleCard_PrintNothing
	.extern LockCapsuleCard_ShowCardMenu
	.extern LockCapsuleCard_Init
	.extern LockCapsuleCard_Main
	.extern LockCapsuleCard_Exit
	.extern RangerLink_SetMPDataCallback
	.extern RangerLink_EnableCallback
	.extern RangerLink_DisableCallback
	.extern RangerLink_MeasureChannelCallback
	.extern RangerLink_SetParentParameterCallback
	.extern RangerLink_StartParentCallback
	.extern RangerLink_ScanCallback
	.extern RangerLink_ScanSelectedCallback
	.extern RangerLink_EndScanCallback
	.extern RangerLink_StartConnectCallback
	.extern RangerLink_StartMPCallback
	.extern RangerLink_PortCallback
	.extern RangerLink_PowerOnCallback
	.extern RangerLink_PowerOffCallback
	.extern RangerLink_ResetCallback
	.extern RangerLink_EndCallback
	.extern RangerLink_GetParentParam
	.extern RangerLink_GetPort
	.extern RangerLink_GetScanParam
	.extern RangerLink_GetSendSize
	.extern RangerLink_GetRecvSize
	.extern RangerLink_GetSendBuffer
	.extern RangerLink_GetRecvBuffer
	.extern RangerLink_GetBssDesc
	.extern RangerLink_GetFoundParents
	.extern RangerLink_GetChildren
	.extern RangerLink_GetGameInfoLength
	.extern RangerLink_GetGameInfo
	.extern RangerLink_GetSsid
	.extern RangerLink_GetPacketHeaderSize
	.public sMysteryGiftCommWork
	.public ov74_0223D0A8
	.public ov74_0223D0BC
	.public ov74_0223D0C0
	.public gRangerLinkWork
	.public sMigrateFromAgbIcons
	.public sPmAgbSave
	.public sPmAgbSaveCounter
	.public sPmAgbRomHeader
	.public sMainMenuUtilCryptoHeapId
	.bss
sMysteryGiftCommWork: ; 0x0223D0A4
	.space 0x4

ov74_0223D0A8: ; 0x0223D0A8
	.space 0x14

ov74_0223D0BC: ; 0x0223D0BC
	.space 0x4

ov74_0223D0C0: ; 0x0223D0C0
	.space 0x4

	.public gRangerLinkWork
gRangerLinkWork: ; 0x0223D0C4
	.space 0x274

sMigrateFromAgbIcons: ; 0x0223D338
	.space 0x4

sPmAgbSave: ; 0x0223D33C
	.space 0x10

sPmAgbSaveCounter: ; 0x0223D34C
	.space 0x8

sPmAgbRomHeader: ; 0x0223D354
	.space 0xFC

sMainMenuUtilCryptoHeapId: ; 0x0223D450
	.space 0x4

