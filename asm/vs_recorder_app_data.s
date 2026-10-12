#include "constants/items.h"
#include "constants/moves.h"
#include "constants/pokemon.h"
#include "constants/ribbon.h"
#include "constants/field_move_response.h"
#include "msgdata/msg/msg_0300.h"
	.include "asm/macros.inc"
	.include "unk_02087A78.inc"
	.include "global.inc"
	.include "field_transition_finish.inc"

	.public gOverlayTemplate_Battle
	.public gOverlayTemplate_VsRecorderMenu
	.public gOverlayTemplate_VsRecorderMenuFromTerminal
	.public gNatureStatMods

	.text
	.public VsRecorderApp_RunPlayback
	.rodata

	.public gOverlayTemplate_VsRecorderApp
	.public gOverlayTemplate_VsRecorderApp
gOverlayTemplate_VsRecorderApp:
	.word VsRecorderApp_InitMode0, VsRecorderApp_Main, VsRecorderApp_Exit, 0xFFFFFFFF
	.public gOverlayTemplate_VsRecorderAppMode6
gOverlayTemplate_VsRecorderAppMode6:
	.word VsRecorderApp_InitMode6, VsRecorderApp_Main, VsRecorderApp_Exit, 0xFFFFFFFF
	.public gOverlayTemplate_VsRecorderAppMode5
gOverlayTemplate_VsRecorderAppMode5:
	.word VsRecorderApp_InitMode5, VsRecorderApp_Main, VsRecorderApp_Exit, 0xFFFFFFFF
	.public gOverlayTemplate_VsRecorderAppMode4
gOverlayTemplate_VsRecorderAppMode4:
	.word VsRecorderApp_InitMode4, VsRecorderApp_Main, VsRecorderApp_Exit, 0xFFFFFFFF
	.public gOverlayTemplate_VsRecorderAppMode3
gOverlayTemplate_VsRecorderAppMode3:
	.word VsRecorderApp_InitMode3, VsRecorderApp_Main, VsRecorderApp_Exit, 0xFFFFFFFF
	.public gOverlayTemplate_VsRecorderAppMode2
gOverlayTemplate_VsRecorderAppMode2:
	.word VsRecorderApp_InitMode2, VsRecorderApp_Main, VsRecorderApp_Exit, 0xFFFFFFFF
	.public gOverlayTemplate_VsRecorderAppMode1
gOverlayTemplate_VsRecorderAppMode1:
	.word VsRecorderApp_InitMode1, VsRecorderApp_Main, VsRecorderApp_Exit, 0xFFFFFFFF
	.public _021028A0
_021028A0:
	.word 0x61
	.word 0x63
	.word 0x64
	.word 0x65
	.word 0x66

	.data

	.public gVsRecorderAppTemplates
gVsRecorderAppTemplates:
	.word gOverlayTemplate_VsRecorderApp
	.word gOverlayTemplate_VsRecorderAppMode1
	.word gOverlayTemplate_VsRecorderAppMode2
	.word gOverlayTemplate_VsRecorderAppMode3
	.word gOverlayTemplate_VsRecorderAppMode4
	.word gOverlayTemplate_VsRecorderAppMode5
	.word gOverlayTemplate_VsRecorderAppMode6
