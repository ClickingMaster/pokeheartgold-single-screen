	.public gWifiClubGameMaxPlayers
	.public gOverlayTemplate_SwalotPlop
	.public gOverlayTemplate_MimeJrTop
	.public gOverlayTemplate_WifiClubGameEntry
	.public gOverlayTemplate_WifiClub
	.public gOverlayTemplate_WobbuffetPop
#include "constants/items.h"
#include "constants/moves.h"
#include "constants/pokemon.h"
#include "constants/ribbon.h"
#include "constants/field_move_response.h"
#include "msgdata/msg/msg_0300.h"
	.include "asm/macros.inc"
	.include "wifi_club_data.inc"
	.include "global.inc"

	.public gOverlayTemplate_Battle
	.public gNatureStatMods

	.text
	.public Task_WifiClub
	.public WifiClubTask_New
	.public CallTask_WifiClub
	.public CallTask_WifiClubWithResult
	.public WifiClub_LaunchGameEntry
	.public WifiClub_FinishGameEntry
	.public WifiClub_LaunchSwalotPlop
	.public WifiClub_FinishSwalotPlop
	.public WifiClub_LaunchMimeJrTop
	.public WifiClub_FinishMimeJrTop
	.public WifiClub_LaunchWobbuffetPop
	.public WifiClub_FinishWobbuffetPop
	.public WifiClub_IsMatchingActivity
	.rodata

gWifiClubGameMaxPlayers:
	.byte 0x03, 0x04, 0x04, 0x04
gOverlayTemplate_SwalotPlop:
	.word SwalotPlop_Init, SwalotPlop_Main, SwalotPlop_Exit, FS_OVERLAY_ID(OVY_91)
gOverlayTemplate_MimeJrTop:
	.word MimeJrTop_Init, MimeJrTop_Main, MimeJrTop_Exit, FS_OVERLAY_ID(OVY_92)
gOverlayTemplate_WifiClubGameEntry:
	.word WifiClubGameEntry_Init, WifiClubGameEntry_Main, WifiClubGameEntry_Exit, FS_OVERLAY_ID(OVY_44)
gOverlayTemplate_WifiClub:
	.word WifiClub_Init, WifiClub_Main, WifiClub_Exit, FS_OVERLAY_ID(OVY_44)
gOverlayTemplate_WobbuffetPop:
	.word WobbuffetPopApp_Init, WobbuffetPopApp_Main, WobbuffetPopApp_Exit, FS_OVERLAY_ID(OVY_93)
