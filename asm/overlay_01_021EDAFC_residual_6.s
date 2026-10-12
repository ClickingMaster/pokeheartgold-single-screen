.public ScriptMenu_Init
.public ScriptMenu_New
.public ScriptMenu_AddItem
.public ScriptMenu_ShowMultichoice
.public ScriptMenu_AppendItem
.public ScriptMenu_GetItemsWidth
.public ScriptMenu_SetupMultichoiceTemplate
.public ScriptMenu_MultichoiceTask
.public ScriptMenu_Free
.public ScriptMenu_CloseMultichoice
.public ScriptMenu_Create
.public ScriptMenu_AddListItem
.public ScriptMenu_ShowList
.public ScriptMenu_ShowListWithWidth
.public ScriptMenu_OpenListWindow
.public ScriptMenu_ShowListAtPosition
.public ScriptMenu_AppendListItem
.public ScriptMenu_GetListItemsWidth
.public ScriptMenu_SetupListTemplate
.public ScriptMenu_ListItemPrintCallback
.public ScriptMenu_ListMoveCursorCallback
.public ScriptMenu_ListTask
.public ScriptMenu_CloseList
.public ScriptMenu_PrintHelpMessage
.public ScriptMenu_PrintCursorHelpMessage
.public PrintCurFloorInNewWindow
.public ScriptMenu_PrintMessage
.public ScriptMenu_FloorWindowTask
.public MapNumToFloorNo
.public GetFloorNameMessage
.public ScriptMenu_ShowMultichoiceGrid
.public ScriptMenu_SetupGridTemplate
.public FieldSystem_ShowMoneyBox
.public MoneyBoxSys_Delete
.public MoneyBoxSys_Update
.public PointsBox_New
.public PointsBox_Delete
.public PointsBox_Update
.public SavingsBox_New
.public SavingsBox_Delete
.public SavingsBox_Update
.public ScriptMenu_GetListItems
.public ScriptMenu_GetCount
.public ov01_021EEF68
.public ov01_021EEF88
.public ScriptMenu_PrintItemHelpMessage
.public ScriptMenu_IsCancellable
.public ScriptMenu_SetAnchorRight
.public ScriptMenu_SetAnchorBottom
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
	.include "overlay_01_021EDAFC.inc"
	.include "global.inc"

	.text
	.public MoneyBoxSys_Delete
	.public ScriptMenu_AddListItem
	.public ScriptMenu_AppendListItem
	.public ScriptMenu_Init
	.public ScriptMenu_New
	.public ScriptMenu_AddItem
	.public ScriptMenu_ShowMultichoice
	.public ScriptMenu_AppendItem
	.public ScriptMenu_GetItemsWidth
	.public ScriptMenu_SetupMultichoiceTemplate
	.public ScriptMenu_Free
	.public ScriptMenu_CloseMultichoice
	.public ScriptMenu_Create
	.public ScriptMenu_ShowListWithWidth
	.public ScriptMenu_OpenListWindow
	.public ScriptMenu_GetListItemsWidth
	.public ScriptMenu_SetupListTemplate
	.public ScriptMenu_ListItemPrintCallback
	.public ScriptMenu_ListMoveCursorCallback
	.public ScriptMenu_CloseList
	.public ScriptMenu_PrintHelpMessage
	.public ScriptMenu_PrintCursorHelpMessage
	.public ScriptMenu_FloorWindowTask
	.public GetFloorNameMessage
	.public PointsBox_Delete
	.public SavingsBox_Delete
	.public ScriptMenu_GetListItems
	.public ScriptMenu_GetCount
	.public ov01_021EEF68
	.public ov01_021EEF88
	.public ScriptMenu_PrintItemHelpMessage
	.public ScriptMenu_IsCancellable

	thumb_func_start MapNumToFloorNo
MapNumToFloorNo: ; 0x021EE81C
	push {r3, lr}
	ldr r1, _021EE92C ; =MAP_CELADON_DEPARTMENT_STORE_4F
	cmp r0, r1
	bgt _021EE878
	bge _021EE906
	cmp r0, #MAP_OLIVINE_LIGHTHOUSE_LIGHT_ROOM
	bgt _021EE860
	bge _021EE8D2
	cmp r0, #MAP_OLIVINE_LIGHTHOUSE_1F
	bgt _021EE834
	beq _021EE8CE
	b _021EE922
_021EE834:
	add r1, r0, #0
	sub r1, #MAP_GOLDENROD_RADIO_TOWER_5F
	cmp r1, #0xb
	bhi _021EE922
	add r1, r1, r1
	add r1, pc
	ldrh r1, [r1, #6]
	lsl r1, r1, #0x10
	asr r1, r1, #0x10
	add pc, r1
_021EE848: ; jump table
	.short _021EE8C6 - _021EE848 - 2 ; case MAP_GOLDENROD_RADIO_TOWER_5F
	.short _021EE8CA - _021EE848 - 2 ; case MAP_GOLDENROD_RADIO_TOWER_OBSERVATION_DECK
	.short _021EE8DE - _021EE848 - 2 ; case MAP_GOLDENROD_DEPARTMENT_STORE_1F
	.short _021EE8E2 - _021EE848 - 2 ; case MAP_GOLDENROD_DEPARTMENT_STORE_2F
	.short _021EE8E6 - _021EE848 - 2 ; case MAP_GOLDENROD_DEPARTMENT_STORE_3F
	.short _021EE8EA - _021EE848 - 2 ; case MAP_GOLDENROD_DEPARTMENT_STORE_4F
	.short _021EE8EE - _021EE848 - 2 ; case MAP_GOLDENROD_DEPARTMENT_STORE_5F
	.short _021EE8F2 - _021EE848 - 2 ; case MAP_GOLDENROD_DEPARTMENT_STORE_6F
	.short _021EE922 - _021EE848 - 2 ; case MAP_GOLDENROD_MAGNET_TRAIN_STATION_1F
	.short _021EE922 - _021EE848 - 2 ; case MAP_GOLDENROD_MAGNET_TRAIN_STATION_2F
	.short _021EE922 - _021EE848 - 2 ; case MAP_GOLDENROD_TUNNEL_B1F
	.short _021EE8F6 - _021EE848 - 2 ; case MAP_GOLDENROD_DEPARTMENT_STORE_BASEMENT
_021EE860:
	ldr r1, _021EE930 ; =MAP_CELADON_DEPARTMENT_STORE_2F
	cmp r0, r1
	bgt _021EE870
	bge _021EE8FE
	sub r1, r1, #MAP_CELADON_DEPARTMENT_STORE_2F-MAP_CELADON_DEPARTMENT_STORE_1F
	cmp r0, r1
	beq _021EE8FA
	b _021EE922
_021EE870:
	add r1, r1, #MAP_CELADON_DEPARTMENT_STORE_3F-MAP_CELADON_DEPARTMENT_STORE_2F
	cmp r0, r1
	beq _021EE902
	b _021EE922
_021EE878:
	add r2, r1, #MAP_CELADON_CONDOMINIUMS_2F-MAP_CELADON_DEPARTMENT_STORE_4F
	cmp r0, r2
	bgt _021EE898
	bge _021EE916
	add r2, r1, #MAP_CELADON_DEPARTMENT_STORE_ROOF-MAP_CELADON_DEPARTMENT_STORE_4F
	cmp r0, r2
	bgt _021EE890
	bge _021EE90E
	add r1, r1, #MAP_CELADON_DEPARTMENT_STORE_5F-MAP_CELADON_DEPARTMENT_STORE_4F
	cmp r0, r1
	beq _021EE90A
	b _021EE922
_021EE890:
	add r1, r1, #MAP_CELADON_CONDOMINIUMS_1F-MAP_CELADON_DEPARTMENT_STORE_4F
	cmp r0, r1
	beq _021EE912
	b _021EE922
_021EE898:
	add r2, r1, #0
	add r2, #MAP_SAFFRON_SILPH_CO_HQ-MAP_CELADON_DEPARTMENT_STORE_4F
	cmp r0, r2
	bgt _021EE8BE
	add r2, r1, #0
	add r2, #MAP_SAFFRON_SILPH_CO_HQ-MAP_CELADON_DEPARTMENT_STORE_4F
	cmp r0, r2
	bge _021EE8D6
	add r2, r1, #MAP_CELADON_CONDOMINIUMS_ROOF-MAP_CELADON_DEPARTMENT_STORE_4F
	cmp r0, r2
	bgt _021EE922
	add r2, r1, #MAP_CELADON_CONDOMINIUMS_3F-MAP_CELADON_DEPARTMENT_STORE_4F
	cmp r0, r2
	blt _021EE922
	beq _021EE91A
	add r1, r1, #MAP_CELADON_CONDOMINIUMS_ROOF-MAP_CELADON_DEPARTMENT_STORE_4F
	cmp r0, r1
	beq _021EE91E
	b _021EE922
_021EE8BE:
	add r1, #MAP_SAFFRON_SILPH_CO_ROTOM_ROOM-MAP_CELADON_DEPARTMENT_STORE_4F
	cmp r0, r1
	beq _021EE8DA
	b _021EE922
_021EE8C6:
	mov r0, #0
	pop {r3, pc}
_021EE8CA:
	mov r0, #1
	pop {r3, pc}
_021EE8CE:
	mov r0, #0
	pop {r3, pc}
_021EE8D2:
	mov r0, #1
	pop {r3, pc}
_021EE8D6:
	mov r0, #0
	pop {r3, pc}
_021EE8DA:
	mov r0, #1
	pop {r3, pc}
_021EE8DE:
	mov r0, #1
	pop {r3, pc}
_021EE8E2:
	mov r0, #2
	pop {r3, pc}
_021EE8E6:
	mov r0, #3
	pop {r3, pc}
_021EE8EA:
	mov r0, #4
	pop {r3, pc}
_021EE8EE:
	mov r0, #5
	pop {r3, pc}
_021EE8F2:
	mov r0, #6
	pop {r3, pc}
_021EE8F6:
	mov r0, #0
	pop {r3, pc}
_021EE8FA:
	mov r0, #0
	pop {r3, pc}
_021EE8FE:
	mov r0, #1
	pop {r3, pc}
_021EE902:
	mov r0, #2
	pop {r3, pc}
_021EE906:
	mov r0, #3
	pop {r3, pc}
_021EE90A:
	mov r0, #4
	pop {r3, pc}
_021EE90E:
	mov r0, #5
	pop {r3, pc}
_021EE912:
	mov r0, #0
	pop {r3, pc}
_021EE916:
	mov r0, #1
	pop {r3, pc}
_021EE91A:
	mov r0, #2
	pop {r3, pc}
_021EE91E:
	mov r0, #3
	pop {r3, pc}
_021EE922:
	bl GF_AssertFail
	mov r0, #0
	pop {r3, pc}
	nop
_021EE92C: .word MAP_CELADON_DEPARTMENT_STORE_4F
_021EE930: .word MAP_CELADON_DEPARTMENT_STORE_2F
	thumb_func_end MapNumToFloorNo
