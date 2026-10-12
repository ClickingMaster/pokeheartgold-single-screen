#include <nitro/fs/overlay.h>
	.include "asm/macros.inc"
	.include "global.inc"

	.public gAreaEdgeColorTableLightKind0
	.public gAreaEdgeColorTableLightKindN
	.public gBuildModelShapesFieldPath
	.public gBuildModelShapesRoomPath

	.text
	.rodata

gAreaEdgeColorTableLightKind0: ; 0x02208BA0
	.byte 0x00, 0x00, 0x84, 0x10, 0x84, 0x10, 0x84, 0x10, 0x84, 0x10, 0x84, 0x10, 0x84, 0x10, 0x84, 0x10

gAreaEdgeColorTableLightKindN: ; 0x02208BB0
	.byte 0x84, 0x10, 0x84, 0x10, 0x84, 0x10, 0x84, 0x10, 0x84, 0x10, 0x84, 0x10, 0x84, 0x10, 0x84, 0x10

	.data

	.balign 4, 0
gBuildModelShapesFieldPath: ; 0x02209A88
	.asciz "fielddata/build_model/bm_field_matshp.dat"

	.balign 4, 0
gBuildModelShapesRoomPath: ; 0x02209AB4
	.asciz "fielddata/build_model/bm_room_matshp.dat"
