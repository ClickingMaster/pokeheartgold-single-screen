#include "constants/items.h"
#include "constants/moves.h"
#include "constants/pokemon.h"
#include "constants/ribbon.h"
#include "constants/field_move_response.h"
#include "msgdata/msg/msg_0300.h"
	.include "asm/macros.inc"
	.include "unk_02087E70.inc"
	.include "global.inc"

	.public gOverlayTemplate_Battle
	.public gNatureStatMods

	.text

	thumb_func_start VsRecorderMenu_InitCommon
VsRecorderMenu_InitCommon: ; 0x02087E70
	push {r3, r4, r5, lr}
	add r4, r0, #0
	mov r0, #0
	add r5, r2, #0
	bl StopAllSE
	mov r2, #0x55
	mov r0, #3
	mov r1, #0x6d
	lsl r2, r2, #0xc
	bl Heap_Create
	add r0, r4, #0
	bl VsRecorderApp_GetRecorderWork
	add r4, r0, #0
	bl VsRecorder_MenuPop
	mov r0, #1
	mov r1, #0x2a
	bl GF_SndHandleSetPlayerVolume
	mov r0, #7
	mov r1, #0x2a
	bl GF_SndHandleSetPlayerVolume
	cmp r5, #0
	beq _02087EB0
	ldr r1, _02087EF0 ; =0x00000482
	mov r0, #0
	bl sub_02055198
_02087EB0:
	cmp r5, #0
	bne _02087EE2
	mov r0, #0x83
	lsl r0, r0, #4
	ldr r0, [r4, r0]
	bl Save_Misc_Get
	add r1, r4, #0
	add r1, #0x5c
	bl sub_0202AC0C
	add r0, r4, #0
	add r0, #0x5c
	ldrb r0, [r0]
	cmp r0, #7
	blo _02087ED8
	add r0, r4, #0
	mov r1, #0
	add r0, #0x5c
	strb r1, [r0]
_02087ED8:
	add r0, r4, #0
	bl VsRecorder_GetBlendColor
	str r0, [r4, #0x58]
	b _02087EE6
_02087EE2:
	ldr r0, _02087EF4 ; =0x00007FDD
	str r0, [r4, #0x58]
_02087EE6:
	add r0, r4, #0
	bl VsRecorder_InitResources
	mov r0, #1
	pop {r3, r4, r5, pc}
	.balign 4, 0
_02087EF0: .word 0x00000482
_02087EF4: .word 0x00007FDD
	thumb_func_end VsRecorderMenu_InitCommon

	thumb_func_start VsRecorderMenu_Init
VsRecorderMenu_Init: ; 0x02087EF8
	ldr r3, _02087F00 ; =VsRecorderMenu_InitCommon
	mov r2, #0
	bx r3
	nop
_02087F00: .word VsRecorderMenu_InitCommon
	thumb_func_end VsRecorderMenu_Init

	thumb_func_start VsRecorderMenu_InitFromTerminal
VsRecorderMenu_InitFromTerminal: ; 0x02087F04
	ldr r3, _02087F0C ; =VsRecorderMenu_InitCommon
	mov r2, #1
	bx r3
	nop
_02087F0C: .word VsRecorderMenu_InitCommon
	thumb_func_end VsRecorderMenu_InitFromTerminal

	thumb_func_start VsRecorderMenu_Main
VsRecorderMenu_Main: ; 0x02087F10
	push {r4, lr}
	add r4, r1, #0
	bl VsRecorderApp_GetRecorderWork
	add r1, r4, #0
	str r4, [r0, #0x10]
	bl VsRecorder_Main
	cmp r0, #0
	beq _02087F28
	mov r0, #1
	pop {r4, pc}
_02087F28:
	mov r0, #0
	pop {r4, pc}
	thumb_func_end VsRecorderMenu_Main

	thumb_func_start VsRecorderMenu_Exit
VsRecorderMenu_Exit: ; 0x02087F2C
	push {r3, r4, r5, lr}
	add r4, r1, #0
	bl VsRecorderApp_GetRecorderWork
	ldr r1, [r4]
	add r5, r0, #0
	cmp r1, #3
	bhi _02087F9E
	add r1, r1, r1
	add r1, pc
	ldrh r1, [r1, #6]
	lsl r1, r1, #0x10
	asr r1, r1, #0x10
	add pc, r1
_02087F48: ; jump table
	.short _02087F50 - _02087F48 - 2 ; case 0
	.short _02087F5C - _02087F48 - 2 ; case 1
	.short _02087F74 - _02087F48 - 2 ; case 2
	.short _02087F8A - _02087F48 - 2 ; case 3
_02087F50:
	bl VsRecorder_DestroyDecorTasks
	ldr r0, [r4]
	add r0, r0, #1
	str r0, [r4]
	b _02087FC2
_02087F5C:
	bl VsRecorder_DeleteDecorSprites
	add r0, r5, #0
	bl VsRecorder_DeleteMenuRowSprites
	add r0, r5, #0
	bl VsRecorder_FreeMenuRowText
	ldr r0, [r4]
	add r0, r0, #1
	str r0, [r4]
	b _02087FC2
_02087F74:
	mov r0, #0x6f
	lsl r0, r0, #4
	ldr r0, [r5, r0]
	bl OrbitCursor_Delete
	cmp r0, #0
	bne _02087FC2
	ldr r0, [r4]
	add r0, r0, #1
	str r0, [r4]
	b _02087FC2
_02087F8A:
	ldr r0, _02087FC8 ; =0x000006F4
	ldr r0, [r5, r0]
	bl OrbitCursor_Delete
	cmp r0, #0
	bne _02087FC2
	ldr r0, [r4]
	add r0, r0, #1
	str r0, [r4]
	b _02087FC2
_02087F9E:
	add r0, r5, #0
	bl VsRecorder_FreeResources
	mov r0, #0x6d
	bl Heap_Destroy
	ldr r0, _02087FCC ; =FS_OVERLAY_ID(OVY_40)
	bl UnloadOverlayByID
	ldr r0, _02087FD0 ; =gSystem + 0x60
	mov r1, #0
	strb r1, [r0, #9]
	bl GfGfx_SwapDisplay
	bl Field_SetEnvironmentSoundState_None_Unk2
	mov r0, #1
	pop {r3, r4, r5, pc}
_02087FC2:
	mov r0, #0
	pop {r3, r4, r5, pc}
	nop
_02087FC8: .word 0x000006F4
_02087FCC: .word FS_OVERLAY_ID(OVY_40)
_02087FD0: .word gSystem + 0x60
	thumb_func_end VsRecorderMenu_Exit

	.rodata

	.public gOverlayTemplate_VsRecorderMenu
gOverlayTemplate_VsRecorderMenu:
	.word VsRecorderMenu_Init, VsRecorderMenu_Main, VsRecorderMenu_Exit, FS_OVERLAY_ID(OVY_40)
	.public gOverlayTemplate_VsRecorderMenuFromTerminal
gOverlayTemplate_VsRecorderMenuFromTerminal:
	.word VsRecorderMenu_InitFromTerminal, VsRecorderMenu_Main, VsRecorderMenu_Exit, FS_OVERLAY_ID(OVY_40)
