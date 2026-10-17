#include "constants/items.h"
#include "constants/moves.h"
#include "constants/pokemon.h"
#include "constants/ribbon.h"
#include "constants/field_move_response.h"
#include "msgdata/msg/msg_0013.h"
	.include "asm/macros.inc"
	.include "vs_recorder_screen_data.inc"
	.include "global.inc"

	.public gOverlayTemplate_Battle
	.public gNatureStatMods

	.text
	.public VsRecorderApp_GetStartMenu
	.public VsRecorderApp_CountMenuChoices

	// enabled entry: message, animation, action, next substate, selection, unused, submenu
	.macro menu_entry msgId, anim, action, next, selection, unk1C, child
	.word 1, 1, \msgId, \anim, \action, \next, \selection, \unk1C, \child
	.endm

	.macro menu_entry_empty
	.space 0x24
	.endm

	.rodata

sVsRecorderQuitMenu:
	menu_entry msg_0013_00130, 4, 2, 2, 0xFBFBFBFB, 0xFBFBFBFB, 0
	menu_entry msg_0013_00131, 3, 2, 4, 0xFBFBFBFB, 0xFBFBFBFB, 0
	menu_entry_empty
	menu_entry_empty
	menu_entry_empty

sVsRecorderMode3QuitMenu:
	menu_entry msg_0013_00130, 5, 2, 2, 0xFBFBFBFB, 0xFBFBFBFB, 0
	menu_entry msg_0013_00131, 4, 2, 4, 0xFBFBFBFB, 0xFBFBFBFB, 0
	menu_entry_empty
	menu_entry_empty
	menu_entry_empty

sVsRecorderMode2QuitMenu:
	menu_entry msg_0013_00130, 4, 2, 2, 0xFBFBFBFB, 0xFBFBFBFB, 0
	menu_entry msg_0013_00131, 3, 2, 4, 0xFBFBFBFB, 0xFBFBFBFB, 0
	menu_entry_empty
	menu_entry_empty
	menu_entry_empty

sVsRecorderMode4QuitMenu:
	menu_entry msg_0013_00130, 5, 2, 2, 0xFBFBFBFB, 0xFBFBFBFB, 0
	menu_entry msg_0013_00131, 4, 2, 4, 0xFBFBFBFB, 0xFBFBFBFB, 0
	menu_entry_empty
	menu_entry_empty
	menu_entry_empty

sVsRecorderMode2Menu:
	menu_entry msg_0013_00103, 2, 1, 3, 0xFBFBFBFB, 0xFBFBFBFB, sVsRecorderViewVideosMenu
	menu_entry msg_0013_00104, 6, 1, 3, 9999, 0xFBFBFBFB, sVsRecorderSendVideoMenu
	menu_entry msg_0013_00002, 9, 2, 3, 999, 0xFBFBFBFB, sVsRecorderMode2QuitMenu
	menu_entry_empty
	menu_entry_empty

sVsRecorderSendVideoMenu:
	menu_entry msg_0013_00130, 4, 19, 3, 100, 0xFBFBFBFB, 0
	menu_entry msg_0013_00003, 3, 3, 4, 0xFBFBFBFB, 0xFBFBFBFB, 0
	menu_entry_empty
	menu_entry_empty
	menu_entry_empty

sVsRecorderMode3Menu:
	menu_entry msg_0013_00092, 0, 14, 3, 300, 0xFBFBFBFB, 0
	menu_entry msg_0013_00093, 1, 15, 3, 301, 0xFBFBFBFB, 0
	menu_entry msg_0013_00002, 2, 2, 3, 999, 0xFBFBFBFB, sVsRecorderMode3QuitMenu
	menu_entry_empty
	menu_entry_empty

	.public sVsRecorderMode3MenuNoFrontier
sVsRecorderMode3MenuNoFrontier:
	menu_entry msg_0013_00092, 0, 14, 3, 300, 0xFBFBFBFB, 0
	menu_entry msg_0013_00002, 2, 2, 3, 999, 0xFBFBFBFB, sVsRecorderMode3QuitMenu
	menu_entry_empty
	menu_entry_empty
	menu_entry_empty

sVsRecorderMode4Menu:
	menu_entry msg_0013_00078, 0, 12, 3, 0, 0xFBFBFBFB, 0
	menu_entry msg_0013_00079, 1, 13, 3, 1, 0xFBFBFBFB, 0
	menu_entry msg_0013_00002, 2, 2, 3, 999, 0xFBFBFBFB, sVsRecorderMode4QuitMenu
	menu_entry_empty
	menu_entry_empty

sVsRecorderViewVideosMenu:
	menu_entry msg_0013_00105, 7, 16, 3, 200, 0xFBFBFBFB, 0
	menu_entry msg_0013_00106, 0, 17, 3, 210, 0xFBFBFBFB, 0
	menu_entry msg_0013_00107, 8, 18, 3, 220, 0xFBFBFBFB, 0
	menu_entry msg_0013_00003, 3, 3, 4, 0xFBFBFBFB, 0xFBFBFBFB, 0
	menu_entry_empty

sVsRecorderMode5Menu:
	menu_entry msg_0013_00052, 0, 9, 3, 0xFBFBFBFB, 0xFBFBFBFB, 0
	menu_entry msg_0013_00051, 0, 8, 3, 0, 0xFBFBFBFB, 0
	menu_entry msg_0013_00002, 2, 2, 3, 999, 0xFBFBFBFB, sVsRecorderQuitMenu
	menu_entry_empty
	menu_entry_empty

sVsRecorderSearchMenu:
	menu_entry msg_0013_00048, 1, 20, 6, 0xFBFBFBFB, 0xFBFBFBFB, 0
	menu_entry msg_0013_00049, 0, 20, 6, 0xFBFBFBFB, 0xFBFBFBFB, 0
	menu_entry msg_0013_00050, 0, 20, 6, 0xFBFBFBFB, 0xFBFBFBFB, 0
	menu_entry msg_0013_00003, 5, 3, 4, 0xFBFBFBFB, 0xFBFBFBFB, 0
	menu_entry_empty

sVsRecorderMode1SubMenu:
	menu_entry msg_0013_00045, 10, 20, 6, 0xFBFBFBFB, 0xFBFBFBFB, 0
	menu_entry msg_0013_00046, 0, 1, 3, 0xFBFBFBFB, 0xFBFBFBFB, sVsRecorderSearchMenu
	menu_entry msg_0013_00047, 11, 20, 6, 0xFBFBFBFB, 0xFBFBFBFB, 0
	menu_entry msg_0013_00003, 5, 3, 4, 0xFBFBFBFB, 0xFBFBFBFB, 0
	menu_entry_empty

sVsRecorderMode1Menu:
	menu_entry msg_0013_00043, 2, 1, 3, 0xFBFBFBFB, 0xFBFBFBFB, sVsRecorderMode1SubMenu
	menu_entry msg_0013_00044, 9, 20, 6, 0xFBFBFBFB, 0xFBFBFBFB, 0
	menu_entry msg_0013_00002, 14, 2, 2, 0xFBFBFBFB, 0xFBFBFBFB, 0
	menu_entry_empty
	menu_entry_empty

sVsRecorderDeleteOtherVideoMenu:
	menu_entry msg_0013_00041, 7, 7, 3, 1, 0xFBFBFBFB, 0
	menu_entry msg_0013_00041, 7, 7, 3, 2, 0xFBFBFBFB, 0
	menu_entry msg_0013_00041, 7, 7, 3, 3, 0xFBFBFBFB, 0
	menu_entry msg_0013_00003, 5, 3, 4, 0xFBFBFBFB, 0xFBFBFBFB, 0
	menu_entry_empty

sVsRecorderDeleteVideosMenu:
	menu_entry msg_0013_00039, 6, 7, 3, 0, 0xFBFBFBFB, 0
	menu_entry msg_0013_00040, 7, 0, 3, 0xFBFBFBFB, 0xFBFBFBFB, sVsRecorderDeleteOtherVideoMenu
	menu_entry msg_0013_00003, 5, 3, 4, 0xFBFBFBFB, 0xFBFBFBFB, 0
	menu_entry_empty
	menu_entry_empty

sVsRecorderOtherVideosMenu:
	menu_entry msg_0013_00008, 7, 4, 3, 1, 0xFBFBFBFB, 0
	menu_entry msg_0013_00008, 7, 4, 3, 2, 0xFBFBFBFB, 0
	menu_entry msg_0013_00008, 7, 4, 3, 3, 0xFBFBFBFB, 0
	menu_entry msg_0013_00003, 5, 3, 4, 0xFBFBFBFB, 0xFBFBFBFB, 0
	menu_entry_empty

sVsRecorderBattleVideosMenu:
	menu_entry msg_0013_00004, 6, 4, 3, 0, 0xFBFBFBFB, 0
	menu_entry msg_0013_00005, 7, 1, 3, 0xFBFBFBFB, 0xFBFBFBFB, sVsRecorderOtherVideosMenu
	menu_entry msg_0013_00006, 1, 1, 3, 0xFBFBFBFB, 0xFBFBFBFB, sVsRecorderDeleteVideosMenu
	menu_entry msg_0013_00003, 5, 3, 4, 0xFBFBFBFB, 0xFBFBFBFB, 0
	menu_entry_empty

	.public sVsRecorderMode0MenuNoFrontier
sVsRecorderMode0MenuNoFrontier:
	menu_entry msg_0013_00000, 0, 1, 3, 0xFBFBFBFB, 0xFBFBFBFB, sVsRecorderBattleVideosMenu
	menu_entry msg_0013_00002, 4, 2, 2, 0xFBFBFBFB, 0xFBFBFBFB, 0
	menu_entry_empty
	menu_entry_empty
	menu_entry_empty

sVsRecorderMode0Menu:
	menu_entry msg_0013_00000, 0, 1, 3, 0xFBFBFBFB, 0xFBFBFBFB, sVsRecorderBattleVideosMenu
	menu_entry msg_0013_00001, 3, 5, 3, 0xFBFBFBFB, 0xFBFBFBFB, 0
	menu_entry msg_0013_00002, 4, 2, 2, 0xFBFBFBFB, 0xFBFBFBFB, 0
	menu_entry_empty
	menu_entry_empty

sVsRecorderMode6Menu:
	menu_entry msg_0013_00059, 1, 11, 3, 0xFBFBFBFB, 0xFBFBFBFB, 0
	menu_entry msg_0013_00058, 1, 10, 3, 100, 0xFBFBFBFB, 0
	menu_entry msg_0013_00002, 2, 2, 3, 999, 0xFBFBFBFB, sVsRecorderQuitMenu
	.space 4
	.public _02103754
_02103754:
	.space 0x20
	menu_entry_empty

	.public sVsRecorderRootMenu
sVsRecorderRootMenu:
	menu_entry 255, 65535, 0, 9, 0xFBFBFBFB, 0xFBFBFBFB, sVsRecorderMode0Menu
	menu_entry 255, 65535, 0, 9, 0xFBFBFBFB, 0xFBFBFBFB, sVsRecorderMode1Menu
	menu_entry 255, 65535, 0, 9, 0xFBFBFBFB, 0xFBFBFBFB, sVsRecorderMode2Menu
	menu_entry 255, 65535, 0, 9, 0xFBFBFBFB, 0xFBFBFBFB, sVsRecorderMode3Menu
	menu_entry 255, 65535, 0, 9, 0xFBFBFBFB, 0xFBFBFBFB, sVsRecorderMode4Menu
	menu_entry 255, 65535, 0, 9, 0xFBFBFBFB, 0xFBFBFBFB, sVsRecorderMode5Menu
	menu_entry 255, 65535, 0, 9, 0xFBFBFBFB, 0xFBFBFBFB, sVsRecorderMode6Menu
