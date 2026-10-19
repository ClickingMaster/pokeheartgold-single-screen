	.public sWifiSetupSecurityRecordTypes
	.public sWifiSetupLinkSocConfig
	.public sWifiSetupRateMasks
	.public gWifiSetupReply
	.public gWifiSetupRecvSsid
	.public gWifiSetupRecvSecurity
	.public gWifiSetupRecvWepKeys
	.public gWifiSetupRecvWpaKey
	.public gWifiSetupWcm
	.public gWifiSetupConfig
	.public gWifiSetupOwnMacAddress
	.public gWifiSetupRecordBuffer
	.public gWifiSetupState
	.public gWifiSetupSocket
	.public gWifiSetupRuntime
	.public gWifiSetupFreeHook
	.public sStr_MELCO
	.public gWifiSetupPeerAddress
	.public gWifiSetupWork
	.public gWifiSetupLinkEventCallback
	.include "asm/macros.inc"
	.include "overlay_13_thumb_2.inc"
	.include "global.inc"

	.extern WifiSetup_Alloc
	.extern WifiSetup_Free
	.extern WifiSetup_RunAoss
	.extern WifiSetup_RunAossSession
	.extern WifiSetup_NextHostAddress
	.extern WifiSetup_FreeWorkBuffers
	.extern WifiSetup_InitState
	.extern WifiSetup_SetError
	.extern WifiSetup_GetError
	.extern WifiSetup_StoreAossResult
	.extern WifiSetup_CheckPrintable
	.extern WifiSetup_FindAossAccessPoint
	.extern WifiSetup_InitAossConnectInfo
	.extern WifiSetup_InitPacketAddresses
	.extern WifiSetup_SetStatus
	.extern WifiSetup_HandlePacket
	.extern WifiSetup_HandleJoinReply
	.extern WifiSetup_HandleSettingsReply
	.extern WifiSetup_HandleDoneReply
	.extern WifiSetup_DecryptPacket
	.extern WifiSetup_CheckPeer
	.extern WifiSetup_CheckReplySequence
	.extern WifiSetup_ParseJoinReply
	.extern WifiSetup_ReadLittleEndian
	.extern WifiSetup_ParseWepRecord
	.extern WifiSetup_ParseWpaRecord
	.extern WifiSetup_ParseType70Record
	.extern WifiSetup_ParseSecurityRecords
	.extern WifiSetup_IsEncryptionRequested
	.extern WifiSetup_SendRequest
	.extern WifiSetup_SendJoinRequest
	.extern WifiSetup_SendSettingsRequest
	.extern WifiSetup_SendDoneRequest
	.extern WifiSetup_BuildJoinPayload
	.extern WifiSetup_WriteBody
	.extern WifiSetup_WritePacketHeader
	.extern WifiSetup_SendPacket
	.extern WifiSetup_Encrypt
	.extern WifiSetup_Decrypt
	.extern WifiSetup_CipherInit
	.extern WifiSetup_CipherXor
	.extern WifiSetup_CipherNextByte
	.extern WifiSetup_CalcCrc32
	.extern WifiSetup_UpdateCrc32
	.extern WifiSetup_InitCrc32Table
	.extern WifiSetup_Scramble
	.extern WifiSetup_ScrambleMakeKey
	.extern WifiSetup_ScrambleXorHalf
	.extern WifiSetup_ScrambleSwapHalves
	.extern WifiSetup_SocketAlloc
	.extern WifiSetup_SocketFree
	.extern WifiSetup_StartupSocket
	.extern WifiSetup_CleanupSocket
	.extern WifiSetup_MemCompare
	.extern WifiSetup_MemCopy
	.extern WifiSetup_MemFill
	.extern WifiSetup_RecvFrom
	.extern WifiSetup_Poll
	.extern WifiSetup_SendTo
	.extern WifiSetup_SetSockOpt
	.extern WifiSetup_Socket
	.extern WifiSetup_Bind
	.extern WifiSetup_Close
	.extern WifiSetup_HostToNet32
	.extern WifiSetup_HostToNet16
	.extern WifiSetup_NetToHost32
	.extern WifiSetup_NetToHost16
	.extern WifiSetup_StrLen
	.extern WifiSetup_Random
	.extern WifiSetup_ClearPollSet
	.extern WifiSetup_AddToPollSet
	.extern WifiSetup_ConvertBssDesc
	.extern WifiSetup_CopyConnectSsid
	.extern WifiSetup_StoreConnectResult
	.extern WifiSetup_OnWcmNotify
	.extern WifiSetup_OnTimeoutAlarm
	.extern WifiSetup_Startup
	.extern WifiSetup_Cleanup
	.extern WifiSetup_DisconnectAccessPoint
	.extern WifiSetup_SearchAccessPoints
	.extern WifiSetup_ConnectAccessPoint
	.extern WifiSetup_Sleep
	.extern WifiSetup_NotifyStatus
	.extern WifiSetupWcm_CopyApList
	.extern WifiSetupWcm_StartSearch
	.extern WifiSetupWcm_StopSearch
	.extern WifiSetupWcm_StartDisconnect
	.extern WifiSetupWcm_StartCleanup
	.extern WifiSetupWcm_StartConnect
	.extern WifiSetupWcm_Init
	.extern WifiSetupLink_HandleEvent
	.extern WifiSetupLink_ReturnToIdle
	.extern WifiSetupLink_CopyApList
	.extern WifiSetupLink_StartSearch
	.extern WifiSetupLink_StartDisconnect
	.extern WifiSetupLink_StartCleanup
	.extern WifiSetupLink_StartConnect
	.extern WifiSetupLink_Init
	.extern WifiSetupLink_GetState
	.extern WifiSetupLink_Alloc
	.extern WifiSetupLink_Free
	.extern WifiSetupLink_ClearEvents
	.extern WifiSetupLink_PopEvent
	.extern WifiSetupLink_PushEvent
	.extern WifiSetupLink_PostEvent
	.extern WifiSetupLink_Open
	.extern WifiSetupLink_Close
	.extern WifiSetupLink_ConnectSelectedAp
	.extern WifiSetupLink_DisconnectAp
	.extern WifiSetup_PickSetupAccessPoint
	.extern WifiSetup_FindSetupAccessPoint
	.extern WifiSetup_SendDatagram
	.extern WifiSetup_BroadcastDatagram
	.extern WifiSetup_SendSetupPacket
	.extern WifiSetup_GetPacketPayload
	.extern WifiSetup_NextRecord
	.extern WifiSetup_FirstRecord
	.extern WifiSetup_BuildPacket
	.extern WifiSetup_AppendRecord
	.extern WifiSetup_BuildSingleRecordList
	.extern WifiSetup_CheckVersionReply
	.extern WifiSetup_ReadPacket
	.extern WifiSetup_BuildVersionRequest
	.extern WifiSetup_GetOwnMacAddress
	.extern WifiSetup_BuildMacPair
	.extern WifiSetup_ParseSettingsRecords
	.extern WifiSetup_HexToBytes
	.extern WifiSetup_StoreSimpleStartResult
	.extern WifiSetup_RunSession
	.extern WifiSetup_SimpleStartThread
	.extern WifiSetup_AesKeyWrap
	.extern WifiSetup_AesKeyUnwrap
	.extern WifiSetup_Md5Init
	.extern WifiSetup_Md5Update
	.extern WifiSetup_Md5Final
	.extern WifiSetup_Md5Transform
	.extern WifiSetup_EncodeLittleEndian
	.extern WifiSetup_DecodeLittleEndian
	.extern WifiSetup_CopyBytes
	.extern WifiSetup_FillBytes
	.extern WifiSetup_CalcMd5
	.extern WifiSetup_GetMilliseconds
	.extern WifiSetup_WorkCalloc
	.extern WifiSetup_WorkFree
	.extern WifiSetup_FormatHexByte
	.extern WifiSetup_FormatMacAddress
	.extern WifiSetup_StartSimpleStart
	.extern WifiSetup_StopSimpleStart
	.extern WifiSetup_GetSimpleStartResult
	.extern WifiSetup_ReportSimpleStartStatus
	.global sWifiSetupSecurityRecordTypes
	.global gWifiSetupSocket
	.global sWifiSetupSocConfig
	.global sStr_ESSID_AOSS
	.global sStr_MELCO
	.global sWifiSetupRateMasks
	.global gWifiSetupConfig
	.global sWifiSetupLinkSocConfig
	.global sWifiSetupSetupApSsid
	.global sStr_NINTENDO_DS_2
	.global sWifiSetupMd5Padding
	.global sStr_WARP
	.global gWifiSetupState
	.global ov13_0224CF9C
	.global ov13_0224CFB8
	.global ov13_0224CFBC
	.global gWifiSetupJoinReply
	.global gWifiSetupCrc32Table
	.global gWifiSetupSecurityRecords
	.global gWifiSetupWep64Record
	.global gWifiSetupWep128Record
	.global ov13_0224D8C0
	.global ov13_0224D930
	.global gWifiSetupPeerAddress
	.global gWifiSetupCipherSeed
	.global gWifiSetupCipherKey
	.global gWifiSetupRuntime
	.global gWifiSetupFreeHook
	.global gWifiSetupMessages
	.global gWifiSetupMessageQueue
	.global gWifiSetupConnectApInfo
	.global gWifiSetupConnectSetting
	.global gWifiSetupWcm
	.global gWifiSetupWcmSearchBssid
	.global gWifiSetupWcmSearchSsid
	.global gWifiSetupWork
	.global gWifiSetupExtendedRequest
	.global gWifiSetupLinkEventCallback
	.global gWifiSetupLinkEventQueue
	.global gWifiSetupLinkMutex
	.global gWifiSetupThread
	.global gWifiSetupResult
	.global ov13_0224E0B0
	.global gWifiSetupResultWepKeys
	.global gWifiSetupResultWpaKey
	.global gWifiSetupReply
	.global gWifiSetupClientSettings
	.global gWifiSetupRecvSsid
	.global gWifiSetupRecvSecurity
	.global gWifiSetupRecvWepKeys
	.global gWifiSetupRecvWpaKey
	.global gWifiSetupOwnMacAddress
	.global gWifiSetupApBssid
	.global gWifiSetupLinkSearchBssid
	.global gWifiSetupSessionKey
	.global gWifiSetupSettingsKey
	.global gWifiSetupTimestampDigest
	.global gWifiSetupLinkSearchSsid
	.global gWifiSetupApSsid
	.global gWifiSetupRecordBuffer
	.global gWifiSetupPacketBuffer

	.data

sWifiSetupSecurityRecordTypes:
	.byte 0x09, 0x08, 0x00, 0x00

gWifiSetupSocket:
	.byte 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF

sWifiSetupSocConfig:
	.byte 0x00, 0x00, 0x00, 0x01
	.word WifiSetup_SocketAlloc
	.word WifiSetup_SocketFree
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x10, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00

	.balign 4, 0
sStr_ESSID_AOSS:
	.asciz "ESSID-AOSS"

	.balign 4, 0
sStr_MELCO:
	.asciz "MELCO"

	.balign 4, 0
sWifiSetupRateMasks:
	.byte 0x01, 0x00, 0x02, 0x00, 0x02, 0x00, 0x04, 0x00
	.byte 0x04, 0x00, 0x0B, 0x00, 0x08, 0x00, 0x0C, 0x00, 0x10, 0x00, 0x12, 0x00, 0x20, 0x00, 0x16, 0x00
	.byte 0x40, 0x00, 0x18, 0x00, 0x80, 0x00, 0x24, 0x00, 0x00, 0x01, 0x30, 0x00, 0x00, 0x02, 0x48, 0x00
	.byte 0x00, 0x04, 0x60, 0x00, 0x00, 0x08, 0x6C, 0x00

gWifiSetupConfig:
	.word gWifiSetupRecordBuffer + 8
	.byte 0x01, 0x00, 0x00, 0x00
	.byte 0x00, 0x08, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x40, 0x00, 0x00, 0x00

sWifiSetupLinkSocConfig:
	.byte 0x00, 0x00, 0x00, 0x01
	.word WifiSetupLink_Alloc
	.word WifiSetupLink_Free
	.byte 0x00, 0x00, 0x00, 0x00, 0xC0, 0xA8, 0x00, 0xB0
	.byte 0xFF, 0xFF, 0xFF, 0x00, 0xC0, 0xA8, 0x00, 0xC8, 0xC0, 0xA8, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x10, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00
	.word sStr_NINTENDO_DS_2
	.byte 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00

sWifiSetupSetupApSsid:
	.byte 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x00, 0x00

	.balign 4, 0
sStr_NINTENDO_DS_2:
	.asciz "NINTENDO-DS"

	.balign 4, 0
sWifiSetupMd5Padding:
	.byte 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

	.balign 4, 0
sStr_WARP:
	.asciz "WARP"
	.balign 4, 0

	.bss

gWifiSetupState:
	.space 0x4

ov13_0224CF9C:
	.space 0x1C

ov13_0224CFB8:
	.space 0x4

ov13_0224CFBC:
	.space 0x1C

gWifiSetupJoinReply:
	.space 0x280

gWifiSetupCrc32Table:
	.space 0x400

gWifiSetupSecurityRecords:
	.space 0x8

gWifiSetupWep64Record:
	.space 0x130

gWifiSetupWep128Record:
	.space 0x130

ov13_0224D8C0:
	.space 0x70

ov13_0224D930:
	.space 0x3C8

gWifiSetupPeerAddress:
	.space 0x8

gWifiSetupCipherSeed:
	.space 0x2

gWifiSetupCipherKey:
	.space 0x7E

gWifiSetupRuntime:
	.space 0xC

gWifiSetupFreeHook:
	.space 0x4

gWifiSetupMessages:
	.space 0x10

gWifiSetupMessageQueue:
	.space 0x20

gWifiSetupConnectApInfo:
	.space 0xC0

gWifiSetupConnectSetting:
	.space 0x60

gWifiSetupWcm:
	.space 0x28

gWifiSetupWcmSearchBssid:
	.space 0x8

gWifiSetupWcmSearchSsid:
	.space 0x20

gWifiSetupWork:
	.space 0x50

gWifiSetupExtendedRequest:
	.space 0x30

gWifiSetupLinkEventCallback:
	.space 0x4

gWifiSetupLinkEventQueue:
	.space 0x10

gWifiSetupLinkMutex:
	.space 0x18

gWifiSetupThread:
	.space 0xC0

gWifiSetupResult:
	.space 0x14

ov13_0224E0B0:
	.space 0x14

gWifiSetupResultWepKeys:
	.space 0x80

gWifiSetupResultWpaKey:
	.space 0x40

gWifiSetupReply:
	.space 0xEC

gWifiSetupClientSettings:
	.space 0x14

gWifiSetupRecvSsid:
	.space 0x2C

gWifiSetupRecvSecurity:
	.space 0x38

gWifiSetupRecvWepKeys:
	.space 0x98

gWifiSetupRecvWpaKey:
	.space 0x58

gWifiSetupOwnMacAddress:
	.space 0x8

gWifiSetupApBssid:
	.space 0x8

gWifiSetupLinkSearchBssid:
	.space 0x8

gWifiSetupSessionKey:
	.space 0x10

gWifiSetupSettingsKey:
	.space 0x8

gWifiSetupTimestampDigest:
	.space 0x18

gWifiSetupLinkSearchSsid:
	.space 0x20

gWifiSetupApSsid:
	.space 0x24

gWifiSetupRecordBuffer:
	.space 0x800

gWifiSetupPacketBuffer:
	.space 0x800
