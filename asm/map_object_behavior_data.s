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
	.include "map_object_behavior_data.inc"
	.include "global.inc"

	.public gLookSidewaysAxisByFacing
	.public gLookSidewaysDirections
	.public gTypeBehaviorSpinOrders
	.public gMapObjectTypeBehaviorInits
	.public gMapObjectTypeBehaviorChecks
	.public gMapObjectTypeBehaviorUpdates

	.text
	.rodata

	.public gLookSidewaysAxisByFacing
gLookSidewaysAxisByFacing:
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00
	.byte 0x01, 0x00, 0x00, 0x00
	.public gLookSidewaysDirections
gLookSidewaysDirections:
	.byte 0x02, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x01, 0x00, 0x00, 0x00
	.public gTypeBehaviorSpinOrders
gTypeBehaviorSpinOrders:
	.byte 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00
	.byte 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00
	.byte 0x02, 0x00, 0x00, 0x00
	.public gMapObjectTypeBehaviorInits
gMapObjectTypeBehaviorInits:
	.word TypeBehavior_NoOpInit
	.word TypeBehavior_NoOpInit
	.word TypeBehavior_NoOpInit
	.word TypeBehavior_NoOpInit
	.word TypeBehavior_LookSidewaysInit
	.word TypeBehavior_SpinInit
	.word TypeBehavior_SpinInit
	.word TypeBehavior_NoOpInit
	.word TypeBehavior_NoOpInit
	.word TypeBehavior_NoOpInit
	.word TypeBehavior_NoOpInit
	.word TypeBehavior_NoOpInit
	.public gMapObjectTypeBehaviorChecks
gMapObjectTypeBehaviorChecks:
	.word TypeBehavior_NoOpCheck
	.word TypeBehavior_NoOpCheck
	.word TypeBehavior_NoOpCheck
	.word TypeBehavior_NoOpCheck
	.word TypeBehavior_LookSidewaysCheck
	.word TypeBehavior_SpinCheck
	.word TypeBehavior_SpinCheck
	.word TypeBehavior_NoOpCheck
	.word TypeBehavior_NoOpCheck
	.word TypeBehavior_NoOpCheck
	.word TypeBehavior_NoOpCheck
	.word TypeBehavior_NoOpCheck
	.public gMapObjectTypeBehaviorUpdates
gMapObjectTypeBehaviorUpdates:
	.word TypeBehavior_NoOpUpdate
	.word TypeBehavior_NoOpUpdate
	.word TypeBehavior_NoOpUpdate
	.word TypeBehavior_NoOpUpdate
	.word TypeBehavior_LookSidewaysUpdate
	.word TypeBehavior_SpinUpdate
	.word TypeBehavior_SpinUpdate
	.word TypeBehavior_NoOpUpdate
	.word TypeBehavior_NoOpUpdate
	.word TypeBehavior_NoOpUpdate
	.word TypeBehavior_NoOpUpdate
	.word TypeBehavior_NoOpUpdate
	; File boundary?