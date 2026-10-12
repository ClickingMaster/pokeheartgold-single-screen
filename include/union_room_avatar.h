#ifndef POKEHEARTGOLD_UNION_ROOM_AVATAR_H
#define POKEHEARTGOLD_UNION_ROOM_AVATAR_H

#include "message_format.h"

void BufferUnionRoomAvatarChoicesNames(u32 trainerId, u32 playerGender, MessageFormat *msgFmt);
int UnionRoomAvatarIdxToSprite(u32 trainerId, u32 playerGender, u32 choice);
int SpriteToUnionRoomAvatarIdx(int playerGender, int trclass);
int GetUnionRoomAvatarAttrBySprite(int playerGender, int trclass, int attr);
u16 *sub_0205B4A4(enum HeapID heapID);

#endif // POKEHEARTGOLD_UNION_ROOM_AVATAR_H
