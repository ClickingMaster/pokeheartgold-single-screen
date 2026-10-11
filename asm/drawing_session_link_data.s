#include "constants/items.h"
#include "constants/moves.h"
#include "constants/pokemon.h"
#include "constants/ribbon.h"
#include "constants/field_move_response.h"
#include "msgdata/msg/msg_0300.h"
	.include "asm/macros.inc"
	.include "unk_0208F814.inc"
	.include "global.inc"

	.public gOverlayTemplate_Battle
	.public gNatureStatMods

	.public DrawingSessionLink_RegisterCommands
	.public DrawingSessionLink_ReceivePictureChunk
	.public sub_0208F8BC
	.public sub_0208F8E4
	.public sub_0208F918
	.public DrawingSessionLink_HandleJoinRequest
	.public sub_0208FA50
	.public sub_0208FA54
	.public sub_0208FA88
	.public sub_0208FA8C
	.public sub_0208FA90
	.public sub_0208FAA8
	.public sub_0208FAEC
	.public DrawingSessionLink_SendPictureChunk
	.public DrawingSessionLink_GetChunkRecvBuffer
	.public sub_0208FB5C
	.public sub_0208FB60
	.public gDrawingSessionLinkCommands

	.rodata

gDrawingSessionLinkCommands:
	.word sub_02091570, sub_020342C4, 0
	.word sub_02091570, sub_020342C4, 0
	.word sub_02091570, sub_020342C4, 0
	.word sub_02091570, sub_020342C4, 0
	.word sub_02091570, sub_020342C4, 0
	.word sub_02091570, sub_020342C4, 0
	.word sub_02091570, sub_020342C4, 0
	.word sub_02091570, sub_020342C4, 0
	.word sub_02091570, sub_020342C4, 0
	.word sub_02091570, sub_020342C4, 0
	.word sub_02091570, sub_020342C4, 0
	.word sub_02091570, sub_020342C4, 0
	.word sub_02091570, sub_020342C4, 0
	.word sub_02091570, sub_020342C4, 0
	.word sub_02091570, sub_020342C4, 0
	.word sub_02091570, sub_020342C4, 0
	.word sub_02091570, sub_020342C4, 0
	.word sub_02091570, sub_020342C4, 0
	.word sub_02091570, sub_020342C4, 0
	.word sub_02091570, sub_020342C4, 0
	.word sub_02091570, sub_020342C4, 0
	.word sub_02091570, sub_020342C4, 0
	.word sub_02091570, sub_020342C4, 0
	.word sub_02091570, sub_020342C4, 0
	.word sub_02091570, sub_020342C4, 0
	.word sub_02091570, sub_020342C4, 0
	.word sub_02091570, sub_020342C4, 0
	.word sub_02091570, sub_020342C4, 0
	.word sub_02091570, sub_020342C4, 0
	.word sub_02091570, sub_020342C4, 0
	.word sub_02091570, sub_020342C4, 0
	.word sub_02091570, sub_020342C4, 0
	.word sub_02091570, sub_020342C4, 0
	.word sub_02091570, sub_020342C4, 0
	.word sub_02091570, sub_020342C4, 0
	.word sub_02091570, sub_020342C4, 0
	.word sub_02091570, sub_020342C4, 0
	.word sub_02091570, sub_020342C4, 0
	.word sub_02091570, sub_020342C4, 0
	.word sub_02091570, sub_020342C4, 0
	.word sub_02091570, sub_020342C4, 0
	.word sub_02091570, sub_020342C4, 0
	.word sub_02091570, sub_020342C4, 0
	.word sub_02091570, sub_020342C4, 0
	.word sub_02091570, sub_020342C4, 0
	.word sub_02091570, sub_020342C4, 0
	.word sub_02091570, sub_020342C4, 0
	.word sub_02091570, sub_020342C4, 0
	.word sub_02091570, sub_020342C4, 0
	.word sub_02091570, sub_020342C4, 0
	.word sub_02091570, sub_020342C4, 0
	.word sub_02091570, sub_020342C4, 0
	.word sub_02091570, sub_020342C4, 0
	.word sub_02091570, sub_020342C4, 0
	.word sub_02091570, sub_020342C4, 0
	.word sub_02091570, sub_020342C4, 0
	.word sub_02091570, sub_020342C4, 0
	.word sub_02091570, sub_020342C4, 0
	.word sub_02091570, sub_020342C4, 0
	.word sub_02091570, sub_020342C4, 0
	.word sub_02091570, sub_020342C4, 0
	.word sub_02091570, sub_020342C4, 0
	.word sub_02091570, sub_020342C4, 0
	.word sub_02091570, sub_020342C4, 0
	.word sub_02091570, sub_020342C4, 0
	.word sub_02091570, sub_020342C4, 0
	.word sub_02091570, sub_020342C4, 0
	.word sub_02091570, sub_020342C4, 0
	.word sub_02091570, sub_020342C4, 0
	.word sub_02091570, sub_020342C4, 0
	.word sub_02091570, sub_020342C4, 0
	.word sub_02091570, sub_020342C4, 0
	.word sub_02091570, sub_020342C4, 0
	.word sub_02091570, sub_020342C4, 0
	.word sub_02091570, sub_020342C4, 0
	.word sub_02091570, sub_020342C4, 0
	.word sub_02091570, sub_020342C4, 0
	.word sub_02091570, sub_020342C4, 0
	.word sub_02091570, sub_020342C4, 0
	.word sub_02091570, sub_020342C4, 0
	.word sub_02091570, sub_020342C4, 0
	.word sub_02091570, sub_020342C4, 0
	.word sub_02091570, sub_020342C4, 0
	.word sub_02091570, sub_020342C4, 0
	.word sub_02091570, sub_020342C4, 0
	.word sub_02091570, sub_020342C4, 0
	.word sub_02091570, sub_020342C4, 0
	.word sub_02091570, sub_020342C4, 0
	.word sub_02091570, sub_020342C4, 0
	.word sub_02091570, sub_020342C4, 0
	.word sub_02091570, sub_0208FB5C, 0
	.word sub_02091570, sub_020342C4, 0
	.word sub_02091570, sub_020342C4, 0
	.word sub_02091570, sub_020342C4, 0
	.word sub_02091570, sub_020342C4, 0
	.word sub_02091570, sub_020342C4, 0
	.word DrawingSessionLink_ReceivePictureChunk, sub_02091598, DrawingSessionLink_GetChunkRecvBuffer
	.word sub_0208F8BC, sub_020915A0, 0
	.word sub_0208F8E4, sub_020915A4, 0
	.word sub_0208FA88, sub_020342C4, 0
	.word sub_0208FA8C, sub_020342C4, 0
	.word sub_0208FA54, sub_020342C4, 0
	.word sub_0208F918, sub_020342C0, 0
	.word sub_0208FA50, sub_020342C0, 0
	.word DrawingSessionLink_HandleJoinRequest, sub_0208FB60, 0
	.word sub_0208FA90, sub_020342C0, 0
	.word sub_0208FAA8, sub_020342C0, 0
	.word sub_0208FAEC, sub_020342C0, 0
	.word sub_02091570, sub_020342C4, 0
	.word sub_02091570, sub_020342C4, 0
