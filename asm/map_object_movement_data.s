#include "constants/sndseq.h"
#include "constants/species.h"
#include "constants/maps.h"
#include "constants/pokemon.h"
#include "constants/flags.h"
#include "constants/vars.h"
#include "constants/items.h"
#include "constants/std_script.h"
#include "fielddata/script/scr_seq/event_D24R0204.h"
#include "constants/field_move_response.h"
	.include "asm/macros.inc"
	.public _02061416
	.public _02061420
	.public _0206144A
	.public _02061468
	.public _0206147E
	.public _020614AA
	.public _020614BC
	.public _020614DA
	.public _020614EE
	.public _020614F0
	.public _02061530
	.public _0206154C
	.public _0206155E
	.public _02061570
	.public _02061582
	.public _02061594
	.public _020615AA
	.public _020615C0
	.public _020615D4
	.public _020615E8
	.public _0206162A
	.public _02061630
	.public _0206163C
	.public _02061642
	.include "map_object_movement_data.inc"
	.include "global.inc"

	.text
	.public RandomWalkMovement_Update
	.public MapObject_GetMovementBounds
	.public MapObject_IsWithinMovementBounds
	.rodata

	.public gPathMovementStateFuncs
	.public gPathMovementStateFuncs
gPathMovementStateFuncs:
	.word PathMovement_Step
	.word PathMovement_WaitStep
	.public gDirectionTable01
gDirectionTable01:
	.word 0, 2, -1
	.public gDirectionTable03
gDirectionTable03:
	.word 1, 2, -1
	.public gDirectionTable13
gDirectionTable13:
	.word 2, 3, -1
	.public gDirectionTable02
gDirectionTable02:
	.word 0, 3, -1
	.public gDirectionTable04
gDirectionTable04:
	.word 1, 3, -1
	.public gDirectionTable12
gDirectionTable12:
	.word 0, 1, -1
	.public gDirectionTable10
gDirectionTable10:
	.word 2, 3, -1
	.public gBackAndForthMovementStateFuncs
	.public gBackAndForthMovementStateFuncs
gBackAndForthMovementStateFuncs:
	.word BackAndForthMovement_FaceStart
	.word BackAndForthMovement_Step
	.word BackAndForthMovement_WaitStep
	.public gDirectionTable09
gDirectionTable09:
	.word 0, 1, -1
	.public gDirectionTable20
gDirectionTable20:
	.word 1, 0, 2, 3
	.public gDirectionTable28
gDirectionTable28:
	.word 2, 3, 0, 1
	.public gDirectionTable27
gDirectionTable27:
	.word 0, 1, 2, 3
	.public gDirectionTable34
gDirectionTable34:
	.word 0, 3, 1, 2
	.public gAlternatingSpinMovementStateFuncs
	.public gAlternatingSpinMovementStateFuncs
gAlternatingSpinMovementStateFuncs:
	.word AlternatingSpinMovement_Face
	.word AlternatingSpinMovement_WaitFace
	.word AlternatingSpinMovement_Wait
	.word AlternatingSpinMovement_Turn
	.public gDirectionTable29
gDirectionTable29:
	.word 1, 2, 3, 0
	.public gDirectionTable31
gDirectionTable31:
	.word 1, 3, 0, 2
	.public gSpinMovementStateFuncs
gSpinMovementStateFuncs:
	.word SpinMovement_FaceNearbyPlayer
	.word SpinMovement_WaitFace
	.word SpinMovement_Wait
	.word SpinMovement_Turn
	.public gDirectionTable35
gDirectionTable35:
	.word 0x00000001, 0x00000002, 0x00000000, 0x00000003
	.public gDirectionTable36
gDirectionTable36:
	.word 0x00000002, 0x00000000, 0x00000003, 0x00000001
	.public gDirectionTable19
gDirectionTable19:
	.word 0x00000002, 0x00000003, 0x00000001, 0x00000000
	.public gDirectionTable30
gDirectionTable30:
	.word 0x00000000, 0x00000002, 0x00000001, 0x00000003
	.public gDirectionTable16
gDirectionTable16:
	.word 0x00000001, 0x00000000, 0x00000003, 0x00000002
	.public gDirectionTable17
gDirectionTable17:
	.word 0x00000002, 0x00000001, 0x00000000, 0x00000003
	.public gDirectionTable15
gDirectionTable15:
	.word 0x00000003, 0x00000002, 0x00000001, 0x00000000
	.public gDirectionTable14
gDirectionTable14:
	.word 0x00000000, 0x00000003, 0x00000002, 0x00000001
	.public gDirectionTable22
gDirectionTable22:
	.word 0x00000002, 0x00000000, 0x00000001, 0x00000003
	.public gDirectionTable23
gDirectionTable23:
	.word 0x00000000, 0x00000001, 0x00000003, 0x00000002
	.public gDirectionTable26
gDirectionTable26:
	.word 0x00000003, 0x00000000, 0x00000001, 0x00000002
	.public gDirectionTable32
gDirectionTable32:
	.word 0x00000002, 0x00000001, 0x00000003, 0x00000000
	.public gDirectionTable06
gDirectionTable06:
	.word 0x00000000, 0x00000001, 0x00000003, 0xFFFFFFFF
	.public gDirectionTable07
gDirectionTable07:
	.word 0x00000000, 0x00000002, 0x00000003, 0xFFFFFFFF
	.public gDirectionTable24
gDirectionTable24:
	.word 0x00000003, 0x00000002, 0x00000000, 0x00000001
	.public gDirectionTable08
gDirectionTable08:
	.word 0x00000001, 0x00000002, 0x00000003, 0xFFFFFFFF
	.public gDirectionTable05
gDirectionTable05:
	.word 0x00000000, 0x00000001, 0x00000002, 0xFFFFFFFF
	.public gDirectionTable25
gDirectionTable25:
	.word 0x00000001, 0x00000003, 0x00000002, 0x00000000
	.public gDirectionTable33
gDirectionTable33:
	.word 0x00000003, 0x00000000, 0x00000002, 0x00000001
	.public gDirectionTable37
gDirectionTable37:
	.word 0x00000003, 0x00000001, 0x00000002, 0x00000000
	.public gDirectionTable18
gDirectionTable18:
	.word 0x00000002, 0x00000003, 0x00000001, 0x00000000
	.public gDirectionTable21
gDirectionTable21:
	.word 0x00000003, 0x00000001, 0x00000000, 0x00000002
	.public gDirectionTable11
gDirectionTable11:
	.word 0x00000000, 0x00000001, 0x00000002, 0x00000003
	.word 0xFFFFFFFF
	.public gSpinCounterclockwiseOrder
gSpinCounterclockwiseOrder:
	.word 0x00000000, 0x00000002, 0x00000001
	.word 0x00000003, 0xFFFFFFFF
	.public gSpinClockwiseOrder
gSpinClockwiseOrder:
	.word 0x00000000, 0x00000003
	.word 0x00000001, 0x00000002, 0xFFFFFFFF
	.public gAlternatingSpinCounterclockwiseOrder
gAlternatingSpinCounterclockwiseOrder:
	.word 0x00000000
	.word 0x00000002, 0x00000001, 0x00000003, 0xFFFFFFFF
	.public gDirectionTable38
gDirectionTable38:
	.word 0x00000000, 0x00000001, 0x00000002, 0x00000003
	.word 0xFFFFFFFF
	.public gAlternatingSpinClockwiseOrder
gAlternatingSpinClockwiseOrder:
	.word 0x00000000, 0x00000003, 0x00000001
	.word 0x00000002, 0xFFFFFFFF
	.public gRandomLookDelays
	.public gRandomLookDelays
gRandomLookDelays:
	.word 0x00000010, 0x00000020
	.word 0x00000030, 0x00000040, 0xFFFFFFFF
	.public gDirectionTable00
gDirectionTable00:
	.word 0x00000000
	.word 0x00000001, 0x00000002, 0x00000003, 0xFFFFFFFF
	.public gWalkingSpinOrders
gWalkingSpinOrders:
	.byte 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00
	.public gPlayerAwareMovementTypes
gPlayerAwareMovementTypes:
	.byte 0x02, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00
	.byte 0x09, 0x00, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x00, 0x0B, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00
	.byte 0x0D, 0x00, 0x00, 0x00, 0x2D, 0x00, 0x00, 0x00, 0x2E, 0x00, 0x00, 0x00, 0x12, 0x00, 0x00, 0x00
	.byte 0x13, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00
	.public gMovementDirectionTables
gMovementDirectionTables:
	.word 0, gDirectionTable00
	.word 0x00000001, gDirectionTable01
	.word 0x00000002, gDirectionTable02
	.word 0x00000003, gDirectionTable03
	.word 0x00000004, gDirectionTable04
	.word 0x00000005, gDirectionTable05
	.word 0x00000006, gDirectionTable06
	.word 0x00000007, gDirectionTable07
	.word 0x00000008, gDirectionTable08
	.word 0x00000009, gDirectionTable09
	.word 0x0000000A, gDirectionTable10
	.word 0x0000000B, gDirectionTable11
	.word 0x0000000C, gDirectionTable12
	.word 0x0000000D, gDirectionTable13
	.word 0x0000000E, gDirectionTable14
	.word 0x0000000F, gDirectionTable15
	.word 0x00000010, gDirectionTable16
	.word 0x00000011, gDirectionTable17
	.word 0x00000012, gDirectionTable18
	.word 0x00000013, gDirectionTable19
	.word 0x00000014, gDirectionTable20
	.word 0x00000015, gDirectionTable21
	.word 0x00000016, gDirectionTable22
	.word 0x00000017, gDirectionTable23
	.word 0x00000018, gDirectionTable24
	.word 0x00000019, gDirectionTable25
	.word 0x0000001A, gDirectionTable26
	.word 0x0000001B, gDirectionTable27
	.word 0x0000001C, gDirectionTable28
	.word 0x0000001D, gDirectionTable29
	.word 0x0000001E, gDirectionTable30
	.word 0x0000001F, gDirectionTable31
	.word 0x00000020, gDirectionTable32
	.word 0x00000021, gDirectionTable33
	.word 0x00000022, gDirectionTable34
	.word 0x00000023, gDirectionTable35
	.word 0x00000024, gDirectionTable36
	.word 0x00000025, gDirectionTable37
	.word 0x00000026, gDirectionTable38
	.word 0x00000027, 0
