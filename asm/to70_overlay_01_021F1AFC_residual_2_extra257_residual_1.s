	.extern ov01_021F2004
	.extern ov01_021F30F4
	.extern ov01_02206A14
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
	.include "overlay_01_021F1AFC.inc"
	.include "global.inc"

.text
	thumb_func_start ov01_021F1ECC
ov01_021F1ECC: ; 0x021F1ECC
	push {r3, r4, r5, r6, r7, lr}
	add r7, r0, #0
	bl TaskManager_GetEnvironment
	add r4, r0, #0
	ldr r0, [r4, #0xc]
	bl PlayerAvatar_GetMapObject
	add r6, r0, #0
	ldr r0, [r4]
	ldr r5, [r4, #0x10]
	cmp r0, #0
	beq _021F1EF0
	cmp r0, #1
	beq _021F1F3C
	cmp r0, #2
	beq _021F1F62
	b _021F1F82
_021F1EF0:
	add r0, r5, #0
	bl MapObject_AreBitsSetForMovementScriptInit
	cmp r0, #0
	beq _021F1F82
	add r0, r6, #0
	bl MapObject_AreBitsSetForMovementScriptInit
	cmp r0, #0
	bne _021F1F0E
	ldr r0, [r4, #0xc]
	bl sub_0205DE38
	cmp r0, #0
	beq _021F1F82
_021F1F0E:
	ldr r0, [r4, #4]
	mov r1, #8
	bl sub_0206234C
	add r1, r0, #0
	add r0, r5, #0
	bl MapObject_SetHeldMovement
	ldr r0, [r4, #4]
	mov r1, #0x20
	bl sub_0206234C
	add r1, r0, #0
	add r0, r6, #0
	bl MapObject_SetHeldMovement
	ldr r0, _021F1F88 ; =SEQ_SE_DP_UG_023
	bl PlaySE
	ldr r0, [r4]
	add r0, r0, #1
	str r0, [r4]
	b _021F1F82
_021F1F3C:
	add r0, r5, #0
	bl MapObject_IsMovementPaused
	cmp r0, #0
	beq _021F1F82
	add r0, r6, #0
	bl MapObject_IsMovementPaused
	cmp r0, #0
	beq _021F1F82
	add r0, r5, #0
	bl MapObject_ClearHeldMovementIfActive
	add r0, r6, #0
	bl MapObject_ClearHeldMovementIfActive
	ldr r0, [r4]
	add r0, r0, #1
	str r0, [r4]
_021F1F62:
	ldr r6, [r4, #8]
	add r0, r4, #0
	bl ov01_021F30F4
	ldr r0, [r6, #0x20]
	ldr r0, [r0]
	cmp r0, #0xed
	bne _021F1F7E
	add r0, r7, #0
	add r1, r6, #0
	add r2, r5, #0
	bl ov01_021F2004
	pop {r3, r4, r5, r6, r7, pc}
_021F1F7E:
	mov r0, #1
	pop {r3, r4, r5, r6, r7, pc}
_021F1F82:
	mov r0, #0
	pop {r3, r4, r5, r6, r7, pc}
	nop
_021F1F88: .word SEQ_SE_DP_UG_023
	thumb_func_end ov01_021F1ECC
