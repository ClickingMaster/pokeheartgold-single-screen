#pragma force_active on
#include "global.h"
#include "player_data.h"
void PlaySE(u16 id);
typedef struct Sprite Sprite;
typedef struct ProfileSlot {
    PlayerProfile *cur;
    PlayerProfile *prev;
} ProfileSlot;
typedef struct Ov73Work {
    u8 unk000[0x22C];
    Sprite *sprites[5];
    u8 unk240[0xB4];
    u8 unk2F4[0x10];
    u8 unk304[0x34];
    ProfileSlot profiles[5];
    int states[5];
} Ov73Work;
int Comm_GetLocalNetId(void);
void ov73_021E77A4(void *a0, void *a1, int index, int avatar, int gender);
void Sprite_SetAnimCtrlSeq(Sprite *sprite, int seq);
void Sprite_SetDrawFlag(Sprite *sprite, int flag);
void ov73_021E762C(Ov73Work *work);

// @fn ov73_021E762C
void ov73_021E762C(Ov73Work *work) {
    int i;
    BOOL appeared = FALSE;
    int gender;
    int avatar;
    for (i = 0; i < 5; i++) {
        switch (work->states[i]) {
        case 0:
            break;
        case 1:
            GF_ASSERT(work->profiles[i].cur != NULL);
            gender = PlayerProfile_GetTrainerGender(work->profiles[i].cur);
            avatar = PlayerProfile_GetAvatar(work->profiles[i].cur);
            if (i == Comm_GetLocalNetId()) {
                Sprite_SetAnimCtrlSeq(work->sprites[i], gender * 2 + 38);
            } else {
                ov73_021E77A4(work->unk2F4, work->unk304, i, avatar, gender);
                Sprite_SetAnimCtrlSeq(work->sprites[i], i * 2 + 27);
            }
            Sprite_SetDrawFlag(work->sprites[i], TRUE);
            work->states[i] = 2;
            appeared = TRUE;
            break;
        case 2:
            break;
        case 3:
            if (i == Comm_GetLocalNetId()) {
                Sprite_SetAnimCtrlSeq(work->sprites[i], PlayerProfile_GetTrainerGender(work->profiles[i].cur) * 2 + 39);
            } else {
                Sprite_SetAnimCtrlSeq(work->sprites[i], i * 2 + 28);
            }
            work->states[i] = 0;
            break;
        }
    }
    if (appeared) {
        PlaySE(1614);
    }
}
