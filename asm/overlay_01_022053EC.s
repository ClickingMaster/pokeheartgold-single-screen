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
	.public ov01_022054E0
	.public ov01_022057DC
	.public MapObject_SetBillboardSepia
	.public ov01_02205B14
	.public Task_FollowMonRecall
	.public ov01_02205F00
	.public Task_FollowMonFlash
	.public FollowMon_IsNextToPlayer
	.public ov01_022096E0
	.public ov01_022096F0
	.public sFollowMonRecallScale
	.public ov01_02209704
	.public sFollowMonFastMovements
	.public sFollowMonSlowMovements
	.public sFollowMonStepMovements
	.public sFollowMonRunMovements
	.public ov01_02209750
	.include "overlay_01_022053EC.inc"
	.include "global.inc"

	.text
	.public ScrCmd_808
	.public FollowMon_MarkForRecreate
	.public FollowMon_GetFasterMovement
	.public MapObject_IsShinyPokemon
	.public MapObject_HasFollowMonSprite
	.public ov01_02205584
	.public ov01_022055B0
	.public FollowMonObject_IsLarge
	.public MapObject_GetCoordsBehind
	.public MapObject_GetPreviousCoordsBehind
	.public ov01_022056C4
	.public MapObject_SetPositionNextTo
	.public FollowMon_SpawnReleaseBall
	.public FollowMon_MoveToPlayer
	.public FollowMon_CheckVisibleFlag
	.public ov01_022057D0
	.public ov01_02205808
	.public ov01_02205870
	.public ov01_0220596C
	.public FollowMon_QueuePlayerStep
	.public FollowMon_RecreateForMap
	.public FollowMon_SetHeight
	.public Task_WaitFollowMonMovement
	.public ov01_02205AEC
	.public ov01_02205CF0
	.public FollowMon_StartRecall
	.public ov01_02205EE0
	.public MapObject_StepTowards
	.public SpriteId_GetModelNo
	.public FollowMon_SetFacingDirection
	.public FollowMon_StartFlash
	.public FieldSystem_GetLeadMonIndex

	thumb_func_start ov01_022054E0
ov01_022054E0: ; 0x022054E0
	push {r3, r4, r5, lr}
	sub sp, #0xc0
	ldr r5, _02205538 ; =ov01_02209750
	add r4, r0, #0
	add r3, sp, #0
	mov r2, #0x18
_022054EC:
	ldmia r5!, {r0, r1}
	stmia r3!, {r0, r1}
	sub r2, r2, #1
	bne _022054EC
	add r0, r4, #0
	bl ov01_02205584
	cmp r0, #0
	beq _02205504
	add sp, #0xc0
	mov r0, #0
	pop {r3, r4, r5, pc}
_02205504:
	add r0, r4, #0
	bl FollowPlayerMovement_GetHopSpeed
	lsl r0, r0, #0x18
	lsr r5, r0, #0x18
	bne _02205516
	add sp, #0xc0
	mov r0, #0
	pop {r3, r4, r5, pc}
_02205516:
	add r0, r4, #0
	bl FollowPlayerMovement_GetHopFrame
	add r4, r0, #0
	cmp r4, #0x10
	blt _02205526
	bl GF_AssertFail
_02205526:
	sub r0, r5, #1
	lsl r0, r0, #0x18
	lsr r2, r0, #0x12
	add r0, sp, #0
	lsl r1, r4, #2
	add r0, r0, r2
	ldr r0, [r1, r0]
	add sp, #0xc0
	pop {r3, r4, r5, pc}
	.balign 4, 0
_02205538: .word ov01_02209750
	thumb_func_end ov01_022054E0
