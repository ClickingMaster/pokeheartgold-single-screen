.public ov01_021EDAFC
.public ov01_021EDC28
.public ov01_021EDC7C
.public ov01_021EDC84
.public ov01_021EDD68
.public ov01_021EDDD8
.public ov01_021EDE18
.public ov01_021EDE8C
.public ov01_021EDF00
.public ov01_021EDF38
.public ov01_021EDF78
.public MoveTutorMenu_SetListItem
.public ov01_021EDFA4
.public ov01_021EE014
.public ov01_021EE01C
.public ov01_021EE0EC
.public MoveTutorMenu_SetListItem_Internal
.public ov01_021EE2E4
.public ov01_021EE324
.public ov01_021EE434
.public ov01_021EE458
.public ov01_021EE49C
.public ov01_021EE568
.public ov01_021EE5D0
.public ov01_021EE634
.public PrintCurFloorInNewWindow
.public ov01_021EE754
.public ov01_021EE7B8
.public MapNumToFloorNo
.public ov01_021EE934
.public ov01_021EE974
.public ov01_021EEA44
.public FieldSystem_ShowMoneyBox
.public MoneyBoxSys_Delete
.public MoneyBoxSys_Update
.public ov01_021EEC00
.public ov01_021EEC68
.public ov01_021EEC7C
.public ov01_021EED60
.public ov01_021EEE30
.public ov01_021EEE44
.public ov01_021EEF58
.public ov01_021EEF60
.public ov01_021EEF68
.public ov01_021EEF88
.public ov01_021EEF9C
.public ov01_021EF00C
.public ov01_021EF018
.public ov01_021EF034
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
	.public MoveTutorMenu_SetListItem
	.public MoveTutorMenu_SetListItem_Internal
	.public ov01_021EDAFC
	.public ov01_021EDC28
	.public ov01_021EDC7C
	.public ov01_021EDC84
	.public ov01_021EDD68
	.public ov01_021EDDD8
	.public ov01_021EDE18
	.public ov01_021EDF00
	.public ov01_021EDF38
	.public ov01_021EDF78
	.public ov01_021EE014
	.public ov01_021EE01C
	.public ov01_021EE2E4
	.public ov01_021EE324
	.public ov01_021EE434
	.public ov01_021EE458
	.public ov01_021EE568
	.public ov01_021EE5D0
	.public ov01_021EE634
	.public ov01_021EE7B8
	.public ov01_021EE934
	.public ov01_021EEC68
	.public ov01_021EEE30
	.public ov01_021EEF58
	.public ov01_021EEF60
	.public ov01_021EEF68
	.public ov01_021EEF88
	.public ov01_021EEF9C
	.public ov01_021EF00C

	thumb_func_start ov01_021EEC00
ov01_021EEC00: ; 0x021EEC00
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x18
	add r5, r0, #0
	add r7, r1, #0
	str r2, [sp, #0x14]
	add r6, r3, #0
	mov r0, #4
	mov r1, #1
	bl AllocWindows
	add r4, r0, #0
	str r6, [sp]
	mov r0, #0xa
	str r0, [sp, #4]
	mov r0, #2
	str r0, [sp, #8]
	mov r0, #0xd
	str r0, [sp, #0xc]
	mov r0, #0x29
	str r0, [sp, #0x10]
	ldr r0, [r5, #8]
	ldr r3, [sp, #0x14]
	add r1, r4, #0
	mov r2, #3
	bl AddWindowParameterized
	mov r0, #0
	str r0, [sp]
	mov r0, #4
	str r0, [sp, #4]
	ldr r0, [r5, #8]
	ldr r2, _021EEC64 ; =0x000003D9
	mov r1, #3
	mov r3, #0xb
	bl LoadUserFrameGfx1
	ldr r2, _021EEC64 ; =0x000003D9
	add r0, r4, #0
	mov r1, #1
	mov r3, #0xb
	bl DrawFrameAndWindow1
	add r0, r5, #0
	add r1, r4, #0
	add r2, r7, #0
	bl ov01_021EEC7C
	add r0, r4, #0
	add sp, #0x18
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
_021EEC64: .word 0x000003D9
	thumb_func_end ov01_021EEC00
