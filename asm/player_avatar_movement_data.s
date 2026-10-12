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
	.include "unk_0205CB48.inc"
	.include "global.inc"

	.text
	.public PlayerAvatar_MoveControl
	.public sub_0205CBE4
	.public PlayerAvatar_IsReadyToMove
	.rodata

	.public gPlayerForcedMovementFuncs
gPlayerForcedMovementFuncs:
	.word ForcedMovement_None
	.word ForcedMovement_Ice
	.word ForcedMovement_CyclingRoadSlope
	.public gPlayerForcedMovementBehaviors
gPlayerForcedMovementBehaviors:
	.word MetatileBehavior_IsIce, 1
	.word 0, 3
	.public gPlayerFootstepSounds
gPlayerFootstepSounds:
	.short SEQ_SE_GS_ASHIOTO_A_WALK, 0
	.short SEQ_SE_GS_ASHIOTO_A, 0
	.short SEQ_SE_GS_OCHIBA, 0
	.short SEQ_SE_GS_EDAPAKI, 0
	.short SEQ_SE_GS_KUSA2, 0
	.short SEQ_SE_GS_ASHIOTO_B, 1
	.short SEQ_SE_GS_ASHIOTO_D, 1
	.short SEQ_SE_GS_ASHIOTO_D_2, 0
	.short SEQ_SE_GS_ASHIOTO_E, 0
	.short SEQ_SE_GS_ASHIOTO_B, 1
	.short SEQ_SE_GS_ASHIOTO_F, 1
	.short SEQ_SE_GS_ASHIOTO_D, 0
	.short SEQ_SE_GS_ASHIOTO_H, 0
	.short SEQ_SE_GS_ASHIOTO_G, 0
	.short SEQ_SE_DP_BOX02, 0
	.short SEQ_SE_DP_BOX01, 0
