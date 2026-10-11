#include "field_link_avatar_alloc_internal.h"

void *sub_0205ABD8(struct UnkStruct_02059E1C *p) {
    int gender;
    int attr;
    p->unk184 = TrainerCardData_New(0);
    p->unk188[0] = TrainerCardData_New(0);
    p->unk188[1] = TrainerCardData_New(0);
    gender = PlayerProfile_GetTrainerGender(p->unk08);
    attr = GetUnionRoomAvatarAttrBySprite(gender, PlayerProfile_GetAvatar(p->unk08), 0);
    TrainerCardData_Build(0, 0, 0, attr, p->unk00, p->unk184);
    return p->unk188[Comm_GetLocalNetId() ^ 1];
}
