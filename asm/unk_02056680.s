	.include "asm/macros.inc"
	.include "unk_02056680.inc"
	.include "global.inc"

	.text
	.public FieldTransition_EnterThroughSideDoor
	.public FieldTransition_EnterFromWarpStairs
	.public FieldTransition_EnterWithDoorExit
	.public FieldTransition_EnterRising
	.public FieldTransition_EnterSinking
	.public FieldTransition_PlaceInsideSideDoor
	.public FieldTransition_PlaceOffWarpStairs
	.public FieldTransition_PlaceBelowFloor
	.public FieldTransition_PlaceAboveFloor
	.public CallTask_WiFiPlaza
	.public Task_WiFiPlaza
	.public FieldTransition_EnterFromWarpStairs
	.public FieldTransition_EnterWithDoorExit
	.public FieldTransition_EnterRising
	.public FieldTransition_EnterSinking
	.public FieldTransition_PlaceBelowFloor
	.public FieldTransition_PlaceAboveFloor
	.public CallTask_WiFiPlaza
	.public Task_WiFiPlaza

	thumb_func_start FieldTransition_EnterThroughSideDoor
FieldTransition_EnterThroughSideDoor: ; 0x02056680
	push {r4, r5, r6, lr}
	add r4, r0, #0
	bl TaskManager_GetFieldSystem
	add r5, r0, #0
	add r0, r4, #0
	bl TaskManager_GetEnvironment
	add r4, r0, #0
	ldrh r0, [r4, #2]
	cmp r0, #0
	beq _020566A2
	cmp r0, #1
	beq _020566C8
	cmp r0, #2
	beq _020566F0
	b _020566F4
_020566A2:
	bl DoorTransition_New
	str r0, [r4, #0x18]
	ldr r0, [r5, #0x40]
	bl PlayerAvatar_GetXCoord
	add r6, r0, #0
	ldr r0, [r5, #0x40]
	bl PlayerAvatar_GetZCoord
	add r1, r0, #0
	ldr r2, [r4, #0x18]
	add r0, r6, #0
	bl DoorTransition_SetPosition
	ldrh r0, [r4, #2]
	add r0, r0, #1
	strh r0, [r4, #2]
	b _020566F4
_020566C8:
	ldr r0, [r5, #0x40]
	ldr r6, [r4, #0x18]
	bl PlayerAvatar_GetFacingDirection
	add r2, r0, #0
	lsl r2, r2, #0x18
	add r0, r5, #0
	add r1, r6, #0
	lsr r2, r2, #0x18
	bl DoorTransition_WalkOutOfSideDoor
	cmp r0, #0
	beq _020566F4
	ldr r0, [r4, #0x18]
	bl DoorTransition_Free
	ldrh r0, [r4, #2]
	add r0, r0, #1
	strh r0, [r4, #2]
	b _020566F4
_020566F0:
	mov r0, #1
	pop {r4, r5, r6, pc}
_020566F4:
	mov r0, #0
	pop {r4, r5, r6, pc}
	thumb_func_end FieldTransition_EnterThroughSideDoor
