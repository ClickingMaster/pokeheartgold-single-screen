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
	.include "unk_data_020FCBD8.inc"
	.include "global.inc"

	.rodata

sWalkOnSpotFastMovements:
	.word 0x00000024
	.word 0x00000025
	.word 0x00000026
	.word 0x00000027

sJumpNearFastMovements:
	.word 0x00000034
	.word 0x00000035
	.word 0x00000036
	.word 0x00000037

sJumpFartherMovements:
	.word 0x0000005E
	.word 0x0000005F
	.word 0x0000005E
	.word 0x0000005F

sWalkOnSpotFasterMovements:
	.word 0x00000028
	.word 0x00000029
	.word 0x0000002A
	.word 0x0000002B

sJumpNearSlowMovements:
	.word 0x0000005C
	.word 0x0000005D
	.word 0x0000005C
	.word 0x0000005D

sWalkOnSpotNormalMovements:
	.word 0x00000020
	.word 0x00000021
	.word 0x00000022
	.word 0x00000023

sWalkSlightlyFastMovements:
	.word 0x0000004C
	.word 0x0000004D
	.word 0x0000004E
	.word 0x0000004F

sWalkFastestMovements:
	.word 0x00000054
	.word 0x00000055
	.word 0x00000056
	.word 0x00000057

sJumpOnSpotFastMovements:
	.word 0x00000030
	.word 0x00000031
	.word 0x00000032
	.word 0x00000033

sRunMovements:
	.word 0x00000058
	.word 0x00000059
	.word 0x0000005A
	.word 0x0000005B

sWalkOnSpotSlowMovements:
	.word 0x0000001C
	.word 0x0000001D
	.word 0x0000001E
	.word 0x0000001F

sJumpFarMovements:
	.word 0x00000038
	.word 0x00000039
	.word 0x0000003A
	.word 0x0000003B

sWalkOnSpotSlowerMovements:
	.word 0x00000018
	.word 0x00000019
	.word 0x0000001A
	.word 0x0000001B

sWalkFastMovements:
	.word 0x00000010
	.word 0x00000011
	.word 0x00000012
	.word 0x00000013

sWalkFasterMovements:
	.word 0x00000014
	.word 0x00000015
	.word 0x00000016
	.word 0x00000017

sWalkEverSoSlightlyFastMovements:
	.word 0x00000060
	.word 0x00000061
	.word 0x00000062
	.word 0x00000063

sWalkSlightlyFasterMovements:
	.word 0x00000050
	.word 0x00000051
	.word 0x00000052
	.word 0x00000053

sWalkNormalMovements:
	.word 0x0000000C
	.word 0x0000000D
	.word 0x0000000E
	.word 0x0000000F

sFaceMovements:
	.word 0x00000000
	.word 0x00000001
	.word 0x00000002
	.word 0x00000003

sWalkSlowerMovements:
	.word 0x00000004
	.word 0x00000005
	.word 0x00000006
	.word 0x00000007

sWalkSlowMovements:
	.word 0x00000008
	.word 0x00000009
	.word 0x0000000A
	.word 0x0000000B

sJumpOnSpotSlowMovements:
	.word 0x0000002C
	.word 0x0000002D
	.word 0x0000002E
	.word 0x0000002F

sHiddenObjectCover1MovementCallbacks:
	.word 0x00000034
	.word HiddenObjectMovement_InitCover1
	.word HiddenObjectMovement_Update
	.word HiddenObjectMovement_Delete
	.word HiddenObjectMovement_Resume

sPathTable15MovementCallbacks:
	.word 0x00000003
	.word PathMovement_InitTable15
	.word PathMovement_Update
	.word MapObjectMovement_NoOpDelete
	.word MapObjectMovement_NoOpRestore

sHiddenObjectCover3MovementCallbacks:
	.word 0x00000036
	.word HiddenObjectMovement_InitCover3
	.word HiddenObjectMovement_Update
	.word HiddenObjectMovement_Delete
	.word HiddenObjectMovement_Resume

sFollowPlayer55MovementCallbacks:
	.word 0x00000003
	.word FollowPlayerMovement_Init
	.word FollowPlayerMovement_Update
	.word FollowPlayerMovement_Delete
	.word MapObjectMovement_NoOpRestore

sPathTable14MovementCallbacks:
	.word 0x00000003
	.word PathMovement_InitTable14
	.word PathMovement_Update
	.word MapObjectMovement_NoOpDelete
	.word MapObjectMovement_NoOpRestore

sHiddenObjectCover2MovementCallbacks:
	.word 0x00000035
	.word HiddenObjectMovement_InitCover2
	.word HiddenObjectMovement_Update
	.word HiddenObjectMovement_Delete
	.word HiddenObjectMovement_Resume

sSpinClockwiseMovementCallbacks:
	.word 0x00000003
	.word SpinMovement_InitClockwise
	.word SpinMovement_Update
	.word MapObjectMovement_NoOpDelete
	.word MapObjectMovement_NoOpRestore

sHiddenObjectCover0MovementCallbacks:
	.word 0x00000033
	.word HiddenObjectMovement_InitCover0
	.word HiddenObjectMovement_Update
	.word HiddenObjectMovement_Delete
	.word HiddenObjectMovement_Resume

sPathTable19MovementCallbacks:
	.word 0x00000003
	.word PathMovement_InitTable19
	.word PathMovement_Update
	.word MapObjectMovement_NoOpDelete
	.word MapObjectMovement_NoOpRestore

sFollowPlayer56MovementCallbacks:
	.word 0x00000003
	.word FollowPlayerMovement_Init
	.word FollowPlayerMovement_UpdateMirror
	.word FollowPlayerMovement_Delete
	.word MapObjectMovement_NoOpRestore

sPathTable20MovementCallbacks:
	.word 0x00000003
	.word PathMovement_InitTable20
	.word PathMovement_Update
	.word MapObjectMovement_NoOpDelete
	.word MapObjectMovement_NoOpRestore

sPathTable21MovementCallbacks:
	.word 0x00000003
	.word PathMovement_InitTable21
	.word PathMovement_Update
	.word MapObjectMovement_NoOpDelete
	.word MapObjectMovement_NoOpRestore

sPathTable22MovementCallbacks:
	.word 0x00000003
	.word PathMovement_InitTable22
	.word PathMovement_Update
	.word MapObjectMovement_NoOpDelete
	.word MapObjectMovement_NoOpRestore

sPathTable23MovementCallbacks:
	.word 0x00000003
	.word PathMovement_InitTable23
	.word PathMovement_Update
	.word MapObjectMovement_NoOpDelete
	.word MapObjectMovement_NoOpRestore

sPathTable25MovementCallbacks:
	.word 0x00000003
	.word PathMovement_InitTable25
	.word PathMovement_Update
	.word MapObjectMovement_NoOpDelete
	.word MapObjectMovement_NoOpRestore

sPathTable26MovementCallbacks:
	.word 0x00000003
	.word PathMovement_InitTable26
	.word PathMovement_Update
	.word MapObjectMovement_NoOpDelete
	.word MapObjectMovement_NoOpRestore

sPathTable27MovementCallbacks:
	.word 0x00000003
	.word PathMovement_InitTable27
	.word PathMovement_Update
	.word MapObjectMovement_NoOpDelete
	.word MapObjectMovement_NoOpRestore

sPathTable28MovementCallbacks:
	.word 0x00000003
	.word PathMovement_InitTable28
	.word PathMovement_Update
	.word MapObjectMovement_NoOpDelete
	.word MapObjectMovement_NoOpRestore

sPathTable29MovementCallbacks:
	.word 0x00000003
	.word PathMovement_InitTable29
	.word PathMovement_Update
	.word MapObjectMovement_NoOpDelete
	.word MapObjectMovement_NoOpRestore

sPathTable30MovementCallbacks:
	.word 0x00000003
	.word PathMovement_InitTable30
	.word PathMovement_Update
	.word MapObjectMovement_NoOpDelete
	.word MapObjectMovement_NoOpRestore

sNoMovementCallbacks:
	.word 0
	.word MapObjectMovement_NoOpInit
	.word MapObjectMovement_NoOpUpdate
	.word MapObjectMovement_NoOpDelete
	.word MapObjectMovement_NoOpRestore

sPlayerMovementCallbacks:
	.word 0x00000001
	.word MapObjectMovement_NoOpInit
	.word MapObjectMovement_NoOpUpdate
	.word MapObjectMovement_NoOpDelete
	.word MapObjectMovement_NoOpRestore

sPathTable31MovementCallbacks:
	.word 0x00000003
	.word PathMovement_InitTable31
	.word PathMovement_Update
	.word MapObjectMovement_NoOpDelete
	.word MapObjectMovement_NoOpRestore

sPathTable24MovementCallbacks:
	.word 0x00000003
	.word PathMovement_InitTable24
	.word PathMovement_Update
	.word MapObjectMovement_NoOpDelete
	.word MapObjectMovement_NoOpRestore

sPathTable32MovementCallbacks:
	.word 0x00000003
	.word PathMovement_InitTable32
	.word PathMovement_Update
	.word MapObjectMovement_NoOpDelete
	.word MapObjectMovement_NoOpRestore

sRandomWalkLeftRightMovementCallbacks:
	.word 0x00000003
	.word RandomWalkMovement_InitLeftRight
	.word RandomWalkMovement_Update
	.word MapObjectMovement_NoOpDelete
	.word MapObjectMovement_NoOpRestore

sPathTable33MovementCallbacks:
	.word 0x00000003
	.word PathMovement_InitTable33
	.word PathMovement_Update
	.word MapObjectMovement_NoOpDelete
	.word MapObjectMovement_NoOpRestore

sPathTable34MovementCallbacks:
	.word 0x00000003
	.word PathMovement_InitTable34
	.word PathMovement_Update
	.word MapObjectMovement_NoOpDelete
	.word MapObjectMovement_NoOpRestore

sRandomLookDownLeftMovementCallbacks:
	.word 0x00000003
	.word RandomLookMovement_InitDownLeft
	.word RandomLookMovement_Update
	.word RandomLookMovement_Delete
	.word MapObjectMovement_NoOpRestore

sPathTable18MovementCallbacks:
	.word 0x00000003
	.word PathMovement_InitTable18
	.word PathMovement_Update
	.word MapObjectMovement_NoOpDelete
	.word MapObjectMovement_NoOpRestore

sPathTable17MovementCallbacks:
	.word 0x00000003
	.word PathMovement_InitTable17
	.word PathMovement_Update
	.word MapObjectMovement_NoOpDelete
	.word MapObjectMovement_NoOpRestore

sPathTable16MovementCallbacks:
	.word 0x00000003
	.word PathMovement_InitTable16
	.word PathMovement_Update
	.word MapObjectMovement_NoOpDelete
	.word MapObjectMovement_NoOpRestore

sRandomLookDownRightMovementCallbacks:
	.word 0x00000003
	.word RandomLookMovement_InitDownRight
	.word RandomLookMovement_Update
	.word RandomLookMovement_Delete
	.word MapObjectMovement_NoOpRestore

sPathTable35MovementCallbacks:
	.word 0x00000003
	.word PathMovement_InitTable35
	.word PathMovement_Update
	.word MapObjectMovement_NoOpDelete
	.word MapObjectMovement_NoOpRestore

sBackAndForthMovementCallbacks:
	.word 0x00000003
	.word BackAndForthMovement_Init
	.word BackAndForthMovement_Update
	.word MapObjectMovement_NoOpDelete
	.word MapObjectMovement_NoOpRestore

sAlternatingSpinMovementCallbacks:
	.word 0x00000003
	.word AlternatingSpinMovement_Init
	.word AlternatingSpinMovement_Update
	.word MapObjectMovement_NoOpDelete
	.word MapObjectMovement_NoOpRestore

sPathTable36MovementCallbacks:
	.word 0x00000003
	.word PathMovement_InitTable36
	.word PathMovement_Update
	.word MapObjectMovement_NoOpDelete
	.word MapObjectMovement_NoOpRestore

sSpinCounterclockwiseMovementCallbacks:
	.word 0x00000003
	.word SpinMovement_InitCounterclockwise
	.word SpinMovement_Update
	.word MapObjectMovement_NoOpDelete
	.word MapObjectMovement_NoOpRestore

sFaceDirectionRightMovementCallbacks:
	.word 0x00000003
	.word FaceDirectionMovement_InitRight
	.word FaceDirectionMovement_Update
	.word MapObjectMovement_NoOpDelete
	.word MapObjectMovement_NoOpRestore

sFaceDirectionLeftMovementCallbacks:
	.word 0x00000003
	.word FaceDirectionMovement_InitLeft
	.word FaceDirectionMovement_Update
	.word MapObjectMovement_NoOpDelete
	.word MapObjectMovement_NoOpRestore

sRandomLookDownLeftRightMovementCallbacks:
	.word 0x00000003
	.word RandomLookMovement_InitDownLeftRight
	.word RandomLookMovement_Update
	.word RandomLookMovement_Delete
	.word MapObjectMovement_NoOpRestore

sPathTable37MovementCallbacks:
	.word 0x00000003
	.word PathMovement_InitTable37
	.word PathMovement_Update
	.word MapObjectMovement_NoOpDelete
	.word MapObjectMovement_NoOpRestore

sRandomLookUpDownMovementCallbacks:
	.word 0x00000003
	.word RandomLookMovement_InitUpDown
	.word RandomLookMovement_Update
	.word RandomLookMovement_Delete
	.word MapObjectMovement_NoOpRestore

sRandomLookLeftRightMovementCallbacks:
	.word 0x00000003
	.word RandomLookMovement_InitLeftRight
	.word RandomLookMovement_Update
	.word RandomLookMovement_Delete
	.word MapObjectMovement_NoOpRestore

sFollowPlayer48MovementCallbacks:
	.word 0x00000003
	.word FollowPlayerMovement_Init
	.word FollowPlayerMovement_UpdateFollowMon
	.word FollowPlayerMovement_Delete
	.word MapObjectMovement_NoOpRestore

sRandomLookUpLeftRightMovementCallbacks:
	.word 0x00000003
	.word RandomLookMovement_InitUpLeftRight
	.word RandomLookMovement_Update
	.word RandomLookMovement_Delete
	.word MapObjectMovement_NoOpRestore

sRandomLookUpDownRightMovementCallbacks:
	.word 0x00000003
	.word RandomLookMovement_InitUpDownRight
	.word RandomLookMovement_Update
	.word RandomLookMovement_Delete
	.word MapObjectMovement_NoOpRestore

sRandomLookUpDownLeftMovementCallbacks:
	.word 0x00000003
	.word RandomLookMovement_InitUpDownLeft
	.word RandomLookMovement_Update
	.word RandomLookMovement_Delete
	.word MapObjectMovement_NoOpRestore

sFaceDirectionUpMovementCallbacks:
	.word 0x00000003
	.word FaceDirectionMovement_InitUp
	.word FaceDirectionMovement_Update
	.word MapObjectMovement_NoOpDelete
	.word MapObjectMovement_NoOpRestore

sFaceDirectionDownMovementCallbacks:
	.word 0x00000003
	.word FaceDirectionMovement_InitDown
	.word FaceDirectionMovement_Update
	.word MapObjectMovement_NoOpDelete
	.word MapObjectMovement_NoOpRestore

sRandomLookUpRightMovementCallbacks:
	.word 0x00000003
	.word RandomLookMovement_InitUpRight
	.word RandomLookMovement_Update
	.word RandomLookMovement_Delete
	.word MapObjectMovement_NoOpRestore

sRandomLookUpLeftMovementCallbacks:
	.word 0x00000003
	.word RandomLookMovement_InitUpLeft
	.word RandomLookMovement_Update
	.word RandomLookMovement_Delete
	.word MapObjectMovement_NoOpRestore

sFollowPartnerMovementCallbacks:
	.word 0x00000003
	.word FollowPartnerMovement_Init
	.word FollowPartnerMovement_Update
	.word FollowPartnerMovement_Delete
	.word FollowPartnerMovement_Resume

sRandomWalkUpDownMovementCallbacks:
	.word 0x00000003
	.word RandomWalkMovement_InitUpDown
	.word RandomWalkMovement_Update
	.word MapObjectMovement_NoOpDelete
	.word MapObjectMovement_NoOpRestore

sRandomWalkAnyDirectionMovementCallbacks:
	.word 0x00000003
	.word RandomWalkMovement_InitAnyDirection
	.word RandomWalkMovement_Update
	.word MapObjectMovement_NoOpDelete
	.word MapObjectMovement_NoOpRestore

sRandomLookAnyDirectionMovementCallbacks:
	.word 0x00000002
	.word RandomLookMovement_InitAnyDirection
	.word RandomLookMovement_Update
	.word RandomLookMovement_Delete
	.word MapObjectMovement_NoOpRestore
sMovementsByDirection:
	.word sFaceMovements
	.word sWalkSlowerMovements
	.word sWalkSlowMovements
	.word sWalkNormalMovements
	.word sWalkFastMovements
	.word sWalkFasterMovements
	.word sWalkOnSpotSlowerMovements
	.word sWalkOnSpotSlowMovements
	.word sWalkOnSpotNormalMovements
	.word sWalkOnSpotFastMovements
	.word sWalkOnSpotFasterMovements
	.word sJumpOnSpotSlowMovements
	.word sJumpOnSpotFastMovements
	.word sJumpNearFastMovements
	.word sJumpFarMovements
	.word sWalkSlightlyFastMovements
	.word sWalkSlightlyFasterMovements
	.word sWalkFastestMovements
	.word sRunMovements
	.word sJumpNearSlowMovements
	.word sJumpFartherMovements
	.word sWalkEverSoSlightlyFastMovements
	.word 0
	.public gMapObjectMovementCallbacks
gMapObjectMovementCallbacks:
	.word sNoMovementCallbacks
	.word sPlayerMovementCallbacks
	.word sRandomLookAnyDirectionMovementCallbacks
	.word sRandomWalkAnyDirectionMovementCallbacks
	.word sRandomWalkUpDownMovementCallbacks
	.word sRandomWalkLeftRightMovementCallbacks
	.word sRandomLookUpLeftMovementCallbacks
	.word sRandomLookUpRightMovementCallbacks
	.word sRandomLookDownLeftMovementCallbacks
	.word sRandomLookDownRightMovementCallbacks
	.word sRandomLookUpDownLeftMovementCallbacks
	.word sRandomLookUpDownRightMovementCallbacks
	.word sRandomLookUpLeftRightMovementCallbacks
	.word sRandomLookDownLeftRightMovementCallbacks
	.word sFaceDirectionUpMovementCallbacks
	.word sFaceDirectionDownMovementCallbacks
	.word sFaceDirectionLeftMovementCallbacks
	.word sFaceDirectionRightMovementCallbacks
	.word sSpinCounterclockwiseMovementCallbacks
	.word sSpinClockwiseMovementCallbacks
	.word sBackAndForthMovementCallbacks
	.word sPathTable14MovementCallbacks
	.word sPathTable15MovementCallbacks
	.word sPathTable16MovementCallbacks
	.word sPathTable17MovementCallbacks
	.word sPathTable18MovementCallbacks
	.word sPathTable19MovementCallbacks
	.word sPathTable20MovementCallbacks
	.word sPathTable21MovementCallbacks
	.word sPathTable22MovementCallbacks
	.word sPathTable23MovementCallbacks
	.word sPathTable24MovementCallbacks
	.word sPathTable25MovementCallbacks
	.word sPathTable26MovementCallbacks
	.word sPathTable27MovementCallbacks
	.word sPathTable28MovementCallbacks
	.word sPathTable29MovementCallbacks
	.word sPathTable30MovementCallbacks
	.word sPathTable31MovementCallbacks
	.word sPathTable32MovementCallbacks
	.word sPathTable33MovementCallbacks
	.word sPathTable34MovementCallbacks
	.word sPathTable35MovementCallbacks
	.word sPathTable36MovementCallbacks
	.word sPathTable37MovementCallbacks
	.word sRandomLookUpDownMovementCallbacks
	.word sRandomLookLeftRightMovementCallbacks
	.word 0
	.word sFollowPlayer48MovementCallbacks
	.word sAlternatingSpinMovementCallbacks
	.word sFollowPartnerMovementCallbacks
	.word sHiddenObjectCover0MovementCallbacks
	.word sHiddenObjectCover1MovementCallbacks
	.word sHiddenObjectCover2MovementCallbacks
	.word sHiddenObjectCover3MovementCallbacks
	.word sFollowPlayer55MovementCallbacks
	.word sFollowPlayer56MovementCallbacks
gMovementCmdTable:
	.word gMovementCmdSteps_000
	.word gMovementCmdSteps_001
	.word gMovementCmdSteps_002
	.word gMovementCmdSteps_003
	.word gMovementCmdSteps_004
	.word gMovementCmdSteps_005
	.word gMovementCmdSteps_006
	.word gMovementCmdSteps_007
	.word gMovementCmdSteps_008
	.word gMovementCmdSteps_009
	.word gMovementCmdSteps_010
	.word gMovementCmdSteps_011
	.word gMovementCmdSteps_012
	.word gMovementCmdSteps_013
	.word gMovementCmdSteps_014
	.word gMovementCmdSteps_015
	.word gMovementCmdSteps_016
	.word gMovementCmdSteps_017
	.word gMovementCmdSteps_018
	.word gMovementCmdSteps_019
	.word gMovementCmdSteps_020
	.word gMovementCmdSteps_021
	.word gMovementCmdSteps_022
	.word gMovementCmdSteps_023
	.word gMovementCmdSteps_024
	.word gMovementCmdSteps_025
	.word gMovementCmdSteps_026
	.word gMovementCmdSteps_027
	.word gMovementCmdSteps_028
	.word gMovementCmdSteps_029
	.word gMovementCmdSteps_030
	.word gMovementCmdSteps_031
	.word gMovementCmdSteps_032
	.word gMovementCmdSteps_033
	.word gMovementCmdSteps_034
	.word gMovementCmdSteps_035
	.word gMovementCmdSteps_036
	.word gMovementCmdSteps_037
	.word gMovementCmdSteps_038
	.word gMovementCmdSteps_039
	.word gMovementCmdSteps_040
	.word gMovementCmdSteps_041
	.word gMovementCmdSteps_042
	.word gMovementCmdSteps_043
	.word gMovementCmdSteps_044
	.word gMovementCmdSteps_045
	.word gMovementCmdSteps_046
	.word gMovementCmdSteps_047
	.word gMovementCmdSteps_048
	.word gMovementCmdSteps_049
	.word gMovementCmdSteps_050
	.word gMovementCmdSteps_051
	.word gMovementCmdSteps_052
	.word gMovementCmdSteps_053
	.word gMovementCmdSteps_054
	.word gMovementCmdSteps_055
	.word gMovementCmdSteps_056
	.word gMovementCmdSteps_057
	.word gMovementCmdSteps_058
	.word gMovementCmdSteps_059
	.word gMovementCmdSteps_060
	.word gMovementCmdSteps_061
	.word gMovementCmdSteps_062
	.word gMovementCmdSteps_063
	.word gMovementCmdSteps_064
	.word gMovementCmdSteps_065
	.word gMovementCmdSteps_066
	.word gMovementCmdSteps_067
	.word gMovementCmdSteps_068
	.word gMovementCmdSteps_069
	.word gMovementCmdSteps_070
	.word gMovementCmdSteps_071
	.word gMovementCmdSteps_072
	.word gMovementCmdSteps_073
	.word gMovementCmdSteps_074
	.word gMovementCmdSteps_075
	.word gMovementCmdSteps_076
	.word gMovementCmdSteps_077
	.word gMovementCmdSteps_078
	.word gMovementCmdSteps_079
	.word gMovementCmdSteps_080
	.word gMovementCmdSteps_081
	.word gMovementCmdSteps_082
	.word gMovementCmdSteps_083
	.word gMovementCmdSteps_084
	.word gMovementCmdSteps_085
	.word gMovementCmdSteps_086
	.word gMovementCmdSteps_087
	.word gMovementCmdSteps_088
	.word gMovementCmdSteps_089
	.word gMovementCmdSteps_090
	.word gMovementCmdSteps_091
	.word gMovementCmdSteps_092
	.word gMovementCmdSteps_093
	.word gMovementCmdSteps_094
	.word gMovementCmdSteps_095
	.word gMovementCmdSteps_096
	.word gMovementCmdSteps_097
	.word gMovementCmdSteps_098
	.word gMovementCmdSteps_099
	.word gMovementCmdSteps_100
	.word gMovementCmdSteps_101
	.word gMovementCmdSteps_102
	.word gMovementCmdSteps_103
	.word gMovementCmdSteps_104
	.word gMovementCmdSteps_105
	.word gMovementCmdSteps_106
	.word gMovementCmdSteps_107
	.word gMovementCmdSteps_108
	.word gMovementCmdSteps_109
	.word gMovementCmdSteps_110
	.word gMovementCmdSteps_111
	.word gMovementCmdSteps_112
