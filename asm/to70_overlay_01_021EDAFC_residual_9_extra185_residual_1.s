	.extern ov01_021EEE44
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
	thumb_func_start ov01_021EEC7C
ov01_021EEC7C: ; 0x021EEC7C
	push {r4, r5, r6, r7, lr}
	sub sp, #0x14
	add r5, r0, #0
	str r1, [sp, #0xc]
	add r0, r1, #0
	mov r1, #0xf
	add r4, r2, #0
	bl FillWindowPixelBuffer
	mov r0, #0
	mov r1, #0x1b
	mov r2, #0xbf
	mov r3, #4
	bl NewMsgDataFromNarc
	add r7, r0, #0
	mov r0, #4
	bl MessageFormat_New
	str r0, [sp, #0x10]
	mov r0, #0x10
	mov r1, #4
	bl String_New
	add r6, r0, #0
	cmp r4, #0
	beq _021EECBA
	cmp r4, #1
	beq _021EECD2
	cmp r4, #2
	b _021EECEE
_021EECBA:
	add r0, r7, #0
	mov r1, #0xc1
	bl NewString_ReadMsgData
	add r4, r0, #0
	ldr r0, [r5, #0xc]
	bl Save_PlayerData_GetCoinsAddr
	bl Coins_GetValue
	add r2, r0, #0
	b _021EED04
_021EECD2:
	add r0, r7, #0
	mov r1, #0xdc
	bl NewString_ReadMsgData
	add r4, r0, #0
	ldr r0, [r5, #0xc]
	bl Save_FrontierData_Get
	mov r1, #0
	add r2, r1, #0
	bl FrontierData_BattlePointAction
	add r2, r0, #0
	b _021EED04
_021EECEE:
	add r0, r7, #0
	mov r1, #0xdf
	bl NewString_ReadMsgData
	add r4, r0, #0
	ldr r0, [r5, #0xc]
	bl Save_Pokeathlon_Get
	bl PokeathlonSave_GetAthletePoints
	add r2, r0, #0
_021EED04:
	mov r0, #1
	str r0, [sp]
	str r0, [sp, #4]
	ldr r0, [sp, #0x10]
	mov r1, #0
	mov r3, #5
	bl BufferIntegerAsString
	ldr r0, [sp, #0x10]
	add r1, r6, #0
	add r2, r4, #0
	bl StringExpandPlaceholders
	mov r0, #0
	add r1, r6, #0
	add r2, r0, #0
	bl FontID_String_GetWidth
	mov r1, #0x50
	sub r3, r1, r0
	mov r1, #0
	str r1, [sp]
	mov r0, #0xff
	str r0, [sp, #4]
	ldr r0, [sp, #0xc]
	add r2, r6, #0
	str r1, [sp, #8]
	bl AddTextPrinterParameterized
	add r0, r4, #0
	bl String_Delete
	add r0, r6, #0
	bl String_Delete
	ldr r0, [sp, #0x10]
	bl MessageFormat_Delete
	add r0, r7, #0
	bl DestroyMsgData
	ldr r0, [sp, #0xc]
	bl ScheduleWindowCopyToVram
	add sp, #0x14
	pop {r4, r5, r6, r7, pc}
	thumb_func_end ov01_021EEC7C
