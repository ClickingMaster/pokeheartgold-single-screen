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

_020FCBD8:
	.word 0x00000024
	.word 0x00000025
	.word 0x00000026
	.word 0x00000027

_020FCBE8:
	.word 0x00000034
	.word 0x00000035
	.word 0x00000036
	.word 0x00000037

_020FCBF8:
	.word 0x0000005E
	.word 0x0000005F
	.word 0x0000005E
	.word 0x0000005F

_020FCC08:
	.word 0x00000028
	.word 0x00000029
	.word 0x0000002A
	.word 0x0000002B

_020FCC18:
	.word 0x0000005C
	.word 0x0000005D
	.word 0x0000005C
	.word 0x0000005D

_020FCC28:
	.word 0x00000020
	.word 0x00000021
	.word 0x00000022
	.word 0x00000023

_020FCC38:
	.word 0x0000004C
	.word 0x0000004D
	.word 0x0000004E
	.word 0x0000004F

_020FCC48:
	.word 0x00000054
	.word 0x00000055
	.word 0x00000056
	.word 0x00000057

_020FCC58:
	.word 0x00000030
	.word 0x00000031
	.word 0x00000032
	.word 0x00000033

_020FCC68:
	.word 0x00000058
	.word 0x00000059
	.word 0x0000005A
	.word 0x0000005B

_020FCC78:
	.word 0x0000001C
	.word 0x0000001D
	.word 0x0000001E
	.word 0x0000001F

_020FCC88:
	.word 0x00000038
	.word 0x00000039
	.word 0x0000003A
	.word 0x0000003B

_020FCC98:
	.word 0x00000018
	.word 0x00000019
	.word 0x0000001A
	.word 0x0000001B

_020FCCA8:
	.word 0x00000010
	.word 0x00000011
	.word 0x00000012
	.word 0x00000013

_020FCCB8:
	.word 0x00000014
	.word 0x00000015
	.word 0x00000016
	.word 0x00000017

_020FCCC8:
	.word 0x00000060
	.word 0x00000061
	.word 0x00000062
	.word 0x00000063

_020FCCD8:
	.word 0x00000050
	.word 0x00000051
	.word 0x00000052
	.word 0x00000053

_020FCCE8:
	.word 0x0000000C
	.word 0x0000000D
	.word 0x0000000E
	.word 0x0000000F

_020FCCF8:
	.word 0x00000000
	.word 0x00000001
	.word 0x00000002
	.word 0x00000003

_020FCD08:
	.word 0x00000004
	.word 0x00000005
	.word 0x00000006
	.word 0x00000007

_020FCD18:
	.word 0x00000008
	.word 0x00000009
	.word 0x0000000A
	.word 0x0000000B

_020FCD28:
	.word 0x0000002C
	.word 0x0000002D
	.word 0x0000002E
	.word 0x0000002F

_020FCD38:
	.word 0x00000034
	.word sub_02066318
	.word HiddenObjectMovement_Update
	.word HiddenObjectMovement_Delete
	.word HiddenObjectMovement_Resume

_020FCD4C:
	.word 0x00000003
	.word PathMovement_InitTable15
	.word PathMovement_Update
	.word MapObjectMovement_NoOpDelete
	.word MapObjectMovement_NoOpRestore

_020FCD60:
	.word 0x00000036
	.word sub_02066330
	.word HiddenObjectMovement_Update
	.word HiddenObjectMovement_Delete
	.word HiddenObjectMovement_Resume

_020FCD74:
	.word 0x00000003
	.word FollowPlayerMovement_Init
	.word FollowPlayerMovement_Update
	.word FollowPlayerMovement_Delete
	.word MapObjectMovement_NoOpRestore

_020FCD88:
	.word 0x00000003
	.word PathMovement_InitTable14
	.word PathMovement_Update
	.word MapObjectMovement_NoOpDelete
	.word MapObjectMovement_NoOpRestore

_020FCD9C:
	.word 0x00000035
	.word sub_02066324
	.word HiddenObjectMovement_Update
	.word HiddenObjectMovement_Delete
	.word HiddenObjectMovement_Resume

_020FCDB0:
	.word 0x00000003
	.word SpinMovement_InitClockwise
	.word SpinMovement_Update
	.word MapObjectMovement_NoOpDelete
	.word MapObjectMovement_NoOpRestore

_020FCDC4:
	.word 0x00000033
	.word sub_0206630C
	.word HiddenObjectMovement_Update
	.word HiddenObjectMovement_Delete
	.word HiddenObjectMovement_Resume

_020FCDD8:
	.word 0x00000003
	.word PathMovement_InitTable19
	.word PathMovement_Update
	.word MapObjectMovement_NoOpDelete
	.word MapObjectMovement_NoOpRestore

_020FCDEC:
	.word 0x00000003
	.word FollowPlayerMovement_Init
	.word FollowPlayerMovement_UpdateMirror
	.word FollowPlayerMovement_Delete
	.word MapObjectMovement_NoOpRestore

_020FCE00:
	.word 0x00000003
	.word PathMovement_InitTable20
	.word PathMovement_Update
	.word MapObjectMovement_NoOpDelete
	.word MapObjectMovement_NoOpRestore

_020FCE14:
	.word 0x00000003
	.word PathMovement_InitTable21
	.word PathMovement_Update
	.word MapObjectMovement_NoOpDelete
	.word MapObjectMovement_NoOpRestore

_020FCE28:
	.word 0x00000003
	.word PathMovement_InitTable22
	.word PathMovement_Update
	.word MapObjectMovement_NoOpDelete
	.word MapObjectMovement_NoOpRestore

_020FCE3C:
	.word 0x00000003
	.word PathMovement_InitTable23
	.word PathMovement_Update
	.word MapObjectMovement_NoOpDelete
	.word MapObjectMovement_NoOpRestore

_020FCE50:
	.word 0x00000003
	.word PathMovement_InitTable25
	.word PathMovement_Update
	.word MapObjectMovement_NoOpDelete
	.word MapObjectMovement_NoOpRestore

_020FCE64:
	.word 0x00000003
	.word PathMovement_InitTable26
	.word PathMovement_Update
	.word MapObjectMovement_NoOpDelete
	.word MapObjectMovement_NoOpRestore

_020FCE78:
	.word 0x00000003
	.word PathMovement_InitTable27
	.word PathMovement_Update
	.word MapObjectMovement_NoOpDelete
	.word MapObjectMovement_NoOpRestore

_020FCE8C:
	.word 0x00000003
	.word PathMovement_InitTable28
	.word PathMovement_Update
	.word MapObjectMovement_NoOpDelete
	.word MapObjectMovement_NoOpRestore

_020FCEA0:
	.word 0x00000003
	.word PathMovement_InitTable29
	.word PathMovement_Update
	.word MapObjectMovement_NoOpDelete
	.word MapObjectMovement_NoOpRestore

_020FCEB4:
	.word 0x00000003
	.word PathMovement_InitTable30
	.word PathMovement_Update
	.word MapObjectMovement_NoOpDelete
	.word MapObjectMovement_NoOpRestore

_020FCEC8:
	.word 0
	.word MapObjectMovement_NoOpInit
	.word MapObjectMovement_NoOpUpdate
	.word MapObjectMovement_NoOpDelete
	.word MapObjectMovement_NoOpRestore

_020FCEDC:
	.word 0x00000001
	.word MapObjectMovement_NoOpInit
	.word MapObjectMovement_NoOpUpdate
	.word MapObjectMovement_NoOpDelete
	.word MapObjectMovement_NoOpRestore

_020FCEF0:
	.word 0x00000003
	.word PathMovement_InitTable31
	.word PathMovement_Update
	.word MapObjectMovement_NoOpDelete
	.word MapObjectMovement_NoOpRestore

_020FCF04:
	.word 0x00000003
	.word PathMovement_InitTable24
	.word PathMovement_Update
	.word MapObjectMovement_NoOpDelete
	.word MapObjectMovement_NoOpRestore

_020FCF18:
	.word 0x00000003
	.word PathMovement_InitTable32
	.word PathMovement_Update
	.word MapObjectMovement_NoOpDelete
	.word MapObjectMovement_NoOpRestore

_020FCF2C:
	.word 0x00000003
	.word RandomWalkMovement_InitLeftRight
	.word RandomWalkMovement_Update
	.word MapObjectMovement_NoOpDelete
	.word MapObjectMovement_NoOpRestore

_020FCF40:
	.word 0x00000003
	.word PathMovement_InitTable33
	.word PathMovement_Update
	.word MapObjectMovement_NoOpDelete
	.word MapObjectMovement_NoOpRestore

_020FCF54:
	.word 0x00000003
	.word PathMovement_InitTable34
	.word PathMovement_Update
	.word MapObjectMovement_NoOpDelete
	.word MapObjectMovement_NoOpRestore

_020FCF68:
	.word 0x00000003
	.word RandomLookMovement_InitDownLeft
	.word RandomLookMovement_Update
	.word RandomLookMovement_Delete
	.word MapObjectMovement_NoOpRestore

_020FCF7C:
	.word 0x00000003
	.word PathMovement_InitTable18
	.word PathMovement_Update
	.word MapObjectMovement_NoOpDelete
	.word MapObjectMovement_NoOpRestore

_020FCF90:
	.word 0x00000003
	.word PathMovement_InitTable17
	.word PathMovement_Update
	.word MapObjectMovement_NoOpDelete
	.word MapObjectMovement_NoOpRestore

_020FCFA4:
	.word 0x00000003
	.word PathMovement_InitTable16
	.word PathMovement_Update
	.word MapObjectMovement_NoOpDelete
	.word MapObjectMovement_NoOpRestore

_020FCFB8:
	.word 0x00000003
	.word RandomLookMovement_InitDownRight
	.word RandomLookMovement_Update
	.word RandomLookMovement_Delete
	.word MapObjectMovement_NoOpRestore

_020FCFCC:
	.word 0x00000003
	.word PathMovement_InitTable35
	.word PathMovement_Update
	.word MapObjectMovement_NoOpDelete
	.word MapObjectMovement_NoOpRestore

_020FCFE0:
	.word 0x00000003
	.word BackAndForthMovement_Init
	.word BackAndForthMovement_Update
	.word MapObjectMovement_NoOpDelete
	.word MapObjectMovement_NoOpRestore

_020FCFF4:
	.word 0x00000003
	.word AlternatingSpinMovement_Init
	.word AlternatingSpinMovement_Update
	.word MapObjectMovement_NoOpDelete
	.word MapObjectMovement_NoOpRestore

_020FD008:
	.word 0x00000003
	.word PathMovement_InitTable36
	.word PathMovement_Update
	.word MapObjectMovement_NoOpDelete
	.word MapObjectMovement_NoOpRestore

_020FD01C:
	.word 0x00000003
	.word SpinMovement_InitCounterclockwise
	.word SpinMovement_Update
	.word MapObjectMovement_NoOpDelete
	.word MapObjectMovement_NoOpRestore

_020FD030:
	.word 0x00000003
	.word FaceDirectionMovement_InitRight
	.word FaceDirectionMovement_Update
	.word MapObjectMovement_NoOpDelete
	.word MapObjectMovement_NoOpRestore

_020FD044:
	.word 0x00000003
	.word FaceDirectionMovement_InitLeft
	.word FaceDirectionMovement_Update
	.word MapObjectMovement_NoOpDelete
	.word MapObjectMovement_NoOpRestore

_020FD058:
	.word 0x00000003
	.word RandomLookMovement_InitDownLeftRight
	.word RandomLookMovement_Update
	.word RandomLookMovement_Delete
	.word MapObjectMovement_NoOpRestore

_020FD06C:
	.word 0x00000003
	.word PathMovement_InitTable37
	.word PathMovement_Update
	.word MapObjectMovement_NoOpDelete
	.word MapObjectMovement_NoOpRestore

_020FD080:
	.word 0x00000003
	.word RandomLookMovement_InitUpDown
	.word RandomLookMovement_Update
	.word RandomLookMovement_Delete
	.word MapObjectMovement_NoOpRestore

_020FD094:
	.word 0x00000003
	.word RandomLookMovement_InitLeftRight
	.word RandomLookMovement_Update
	.word RandomLookMovement_Delete
	.word MapObjectMovement_NoOpRestore

_020FD0A8:
	.word 0x00000003
	.word FollowPlayerMovement_Init
	.word FollowPlayerMovement_UpdateFollowMon
	.word FollowPlayerMovement_Delete
	.word MapObjectMovement_NoOpRestore

_020FD0BC:
	.word 0x00000003
	.word RandomLookMovement_InitUpLeftRight
	.word RandomLookMovement_Update
	.word RandomLookMovement_Delete
	.word MapObjectMovement_NoOpRestore

_020FD0D0:
	.word 0x00000003
	.word RandomLookMovement_InitUpDownRight
	.word RandomLookMovement_Update
	.word RandomLookMovement_Delete
	.word MapObjectMovement_NoOpRestore

_020FD0E4:
	.word 0x00000003
	.word RandomLookMovement_InitUpDownLeft
	.word RandomLookMovement_Update
	.word RandomLookMovement_Delete
	.word MapObjectMovement_NoOpRestore

_020FD0F8:
	.word 0x00000003
	.word FaceDirectionMovement_InitUp
	.word FaceDirectionMovement_Update
	.word MapObjectMovement_NoOpDelete
	.word MapObjectMovement_NoOpRestore

_020FD10C:
	.word 0x00000003
	.word FaceDirectionMovement_InitDown
	.word FaceDirectionMovement_Update
	.word MapObjectMovement_NoOpDelete
	.word MapObjectMovement_NoOpRestore

_020FD120:
	.word 0x00000003
	.word RandomLookMovement_InitUpRight
	.word RandomLookMovement_Update
	.word RandomLookMovement_Delete
	.word MapObjectMovement_NoOpRestore

_020FD134:
	.word 0x00000003
	.word RandomLookMovement_InitUpLeft
	.word RandomLookMovement_Update
	.word RandomLookMovement_Delete
	.word MapObjectMovement_NoOpRestore

_020FD148:
	.word 0x00000003
	.word FollowPartnerMovement_Init
	.word FollowPartnerMovement_Update
	.word FollowPartnerMovement_Delete
	.word FollowPartnerMovement_Resume

_020FD15C:
	.word 0x00000003
	.word RandomWalkMovement_InitUpDown
	.word RandomWalkMovement_Update
	.word MapObjectMovement_NoOpDelete
	.word MapObjectMovement_NoOpRestore

_020FD170:
	.word 0x00000003
	.word RandomWalkMovement_InitAnyDirection
	.word RandomWalkMovement_Update
	.word MapObjectMovement_NoOpDelete
	.word MapObjectMovement_NoOpRestore

_020FD184:
	.word 0x00000002
	.word RandomLookMovement_InitAnyDirection
	.word RandomLookMovement_Update
	.word RandomLookMovement_Delete
	.word MapObjectMovement_NoOpRestore
_020FD198:
	.word _020FCCF8
	.word _020FCD08
	.word _020FCD18
	.word _020FCCE8
	.word _020FCCA8
	.word _020FCCB8
	.word _020FCC98
	.word _020FCC78
	.word _020FCC28
	.word _020FCBD8
	.word _020FCC08
	.word _020FCD28
	.word _020FCC58
	.word _020FCBE8
	.word _020FCC88
	.word _020FCC38
	.word _020FCCD8
	.word _020FCC48
	.word _020FCC68
	.word _020FCC18
	.word _020FCBF8
	.word _020FCCC8
	.word 0
	.public gMapObjectMovementCallbacks
gMapObjectMovementCallbacks:
	.word _020FCEC8
	.word _020FCEDC
	.word _020FD184
	.word _020FD170
	.word _020FD15C
	.word _020FCF2C
	.word _020FD134
	.word _020FD120
	.word _020FCF68
	.word _020FCFB8
	.word _020FD0E4
	.word _020FD0D0
	.word _020FD0BC
	.word _020FD058
	.word _020FD0F8
	.word _020FD10C
	.word _020FD044
	.word _020FD030
	.word _020FD01C
	.word _020FCDB0
	.word _020FCFE0
	.word _020FCD88
	.word _020FCD4C
	.word _020FCFA4
	.word _020FCF90
	.word _020FCF7C
	.word _020FCDD8
	.word _020FCE00
	.word _020FCE14
	.word _020FCE28
	.word _020FCE3C
	.word _020FCF04
	.word _020FCE50
	.word _020FCE64
	.word _020FCE78
	.word _020FCE8C
	.word _020FCEA0
	.word _020FCEB4
	.word _020FCEF0
	.word _020FCF18
	.word _020FCF40
	.word _020FCF54
	.word _020FCFCC
	.word _020FD008
	.word _020FD06C
	.word _020FD080
	.word _020FD094
	.word 0
	.word _020FD0A8
	.word _020FCFF4
	.word _020FD148
	.word _020FCDC4
	.word _020FCD38
	.word _020FCD9C
	.word _020FCD60
	.word _020FCD74
	.word _020FCDEC
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
