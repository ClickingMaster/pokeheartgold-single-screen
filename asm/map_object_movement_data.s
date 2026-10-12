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
	.include "unk_02061284.inc"
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
	.public _020FD4F4
_020FD4F4:
	.word 0, 2, -1
	.public _020FD500
_020FD500:
	.word 1, 2, -1
	.public _020FD50C
_020FD50C:
	.word 2, 3, -1
	.public _020FD518
_020FD518:
	.word 0, 3, -1
	.public _020FD524
_020FD524:
	.word 1, 3, -1
	.public _020FD530
_020FD530:
	.word 0, 1, -1
	.public _020FD53C
_020FD53C:
	.word 2, 3, -1
	.public gBackAndForthMovementStateFuncs
	.public gBackAndForthMovementStateFuncs
gBackAndForthMovementStateFuncs:
	.word BackAndForthMovement_FaceStart
	.word BackAndForthMovement_Step
	.word BackAndForthMovement_WaitStep
	.public _020FD554
_020FD554:
	.word 0, 1, -1
	.public _020FD560
_020FD560:
	.word 1, 0, 2, 3
	.public _020FD570
_020FD570:
	.word 2, 3, 0, 1
	.public _020FD580
_020FD580:
	.word 0, 1, 2, 3
	.public _020FD590
_020FD590:
	.word 0, 3, 1, 2
	.public gAlternatingSpinMovementStateFuncs
	.public gAlternatingSpinMovementStateFuncs
gAlternatingSpinMovementStateFuncs:
	.word AlternatingSpinMovement_Face
	.word AlternatingSpinMovement_WaitFace
	.word AlternatingSpinMovement_Wait
	.word AlternatingSpinMovement_Turn
	.public _020FD5B0
_020FD5B0:
	.word 1, 2, 3, 0
	.public _020FD5C0
_020FD5C0:
	.word 1, 3, 0, 2
	.public gSpinMovementStateFuncs
gSpinMovementStateFuncs:
	.word SpinMovement_FaceNearbyPlayer
	.word SpinMovement_WaitFace
	.word SpinMovement_Wait
	.word SpinMovement_Turn
	.public _020FD5E0
_020FD5E0:
	.word 0x00000001, 0x00000002, 0x00000000, 0x00000003
	.public _020FD5F0
_020FD5F0:
	.word 0x00000002, 0x00000000, 0x00000003, 0x00000001
	.public _020FD600
_020FD600:
	.word 0x00000002, 0x00000003, 0x00000001, 0x00000000
	.public _020FD610
_020FD610:
	.word 0x00000000, 0x00000002, 0x00000001, 0x00000003
	.public _020FD620
_020FD620:
	.word 0x00000001, 0x00000000, 0x00000003, 0x00000002
	.public _020FD630
_020FD630:
	.word 0x00000002, 0x00000001, 0x00000000, 0x00000003
	.public _020FD640
_020FD640:
	.word 0x00000003, 0x00000002, 0x00000001, 0x00000000
	.public _020FD650
_020FD650:
	.word 0x00000000, 0x00000003, 0x00000002, 0x00000001
	.public _020FD660
_020FD660:
	.word 0x00000002, 0x00000000, 0x00000001, 0x00000003
	.public _020FD670
_020FD670:
	.word 0x00000000, 0x00000001, 0x00000003, 0x00000002
	.public _020FD680
_020FD680:
	.word 0x00000003, 0x00000000, 0x00000001, 0x00000002
	.public _020FD690
_020FD690:
	.word 0x00000002, 0x00000001, 0x00000003, 0x00000000
	.public _020FD6A0
_020FD6A0:
	.word 0x00000000, 0x00000001, 0x00000003, 0xFFFFFFFF
	.public _020FD6B0
_020FD6B0:
	.word 0x00000000, 0x00000002, 0x00000003, 0xFFFFFFFF
	.public _020FD6C0
_020FD6C0:
	.word 0x00000003, 0x00000002, 0x00000000, 0x00000001
	.public _020FD6D0
_020FD6D0:
	.word 0x00000001, 0x00000002, 0x00000003, 0xFFFFFFFF
	.public _020FD6E0
_020FD6E0:
	.word 0x00000000, 0x00000001, 0x00000002, 0xFFFFFFFF
	.public _020FD6F0
_020FD6F0:
	.word 0x00000001, 0x00000003, 0x00000002, 0x00000000
	.public _020FD700
_020FD700:
	.word 0x00000003, 0x00000000, 0x00000002, 0x00000001
	.public _020FD710
_020FD710:
	.word 0x00000003, 0x00000001, 0x00000002, 0x00000000
	.public _020FD720
_020FD720:
	.word 0x00000002, 0x00000003, 0x00000001, 0x00000000
	.public _020FD730
_020FD730:
	.word 0x00000003, 0x00000001, 0x00000000, 0x00000002
	.public _020FD740
_020FD740:
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
	.public _020FD790
_020FD790:
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
	.public _020FD7CC
_020FD7CC:
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
	.word 0, _020FD7CC
	.word 0x00000001, _020FD4F4
	.word 0x00000002, _020FD518
	.word 0x00000003, _020FD500
	.word 0x00000004, _020FD524
	.word 0x00000005, _020FD6E0
	.word 0x00000006, _020FD6A0
	.word 0x00000007, _020FD6B0
	.word 0x00000008, _020FD6D0
	.word 0x00000009, _020FD554
	.word 0x0000000A, _020FD53C
	.word 0x0000000B, _020FD740
	.word 0x0000000C, _020FD530
	.word 0x0000000D, _020FD50C
	.word 0x0000000E, _020FD650
	.word 0x0000000F, _020FD640
	.word 0x00000010, _020FD620
	.word 0x00000011, _020FD630
	.word 0x00000012, _020FD720
	.word 0x00000013, _020FD600
	.word 0x00000014, _020FD560
	.word 0x00000015, _020FD730
	.word 0x00000016, _020FD660
	.word 0x00000017, _020FD670
	.word 0x00000018, _020FD6C0
	.word 0x00000019, _020FD6F0
	.word 0x0000001A, _020FD680
	.word 0x0000001B, _020FD580
	.word 0x0000001C, _020FD570
	.word 0x0000001D, _020FD5B0
	.word 0x0000001E, _020FD610
	.word 0x0000001F, _020FD5C0
	.word 0x00000020, _020FD690
	.word 0x00000021, _020FD700
	.word 0x00000022, _020FD590
	.word 0x00000023, _020FD5E0
	.word 0x00000024, _020FD5F0
	.word 0x00000025, _020FD710
	.word 0x00000026, _020FD790
	.word 0x00000027, 0
