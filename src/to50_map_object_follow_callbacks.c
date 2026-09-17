#include "to50_map_object_follow_callbacks_private.h"

extern const FollowCallback _020FE3CC[2];
extern const FollowCallback _020FE3D4[2];
extern const VecFx32 _020FE3DC;
extern const VecFx32 _020FE3E8;
extern const u32 _020FE3F4[4];
extern const FollowCallback _020FE404[4];
extern const FollowCallback _020FE414[4];
extern const FollowCallback _020FE424[4];
extern const u32 _020FE434[4];
extern const u32 _020FE444[4];

void sub_02065900(LocalMapObject *object) {
    FollowState *state = (FollowState *)sub_0205F394(object);
    if (sub_02065CD0(object, state)) {
        MapObject_ClearFlag18(object, FALSE);
        while (_020FE404[state->state](object, state) == TRUE) {
        }
    }
}

void sub_02065938(LocalMapObject *object) {
    FollowState *state = (FollowState *)sub_0205F394(object);
    if (sub_02065CD0(object, state)) {
        while (_020FE414[state->state](object, state) == TRUE) {
        }
    }
}

void sub_02065968(LocalMapObject *object) {
    FollowState *state = (FollowState *)sub_0205F394(object);
    if (sub_02065CD0(object, state)) {
        while (_020FE424[state->state](object, state) == TRUE) {
        }
    }
}

const FollowCallback _020FE3D4[2] = { sub_02066064, sub_020660A0 };
const FollowCallback _020FE3CC[2] = { sub_020663B4, sub_020663E4 };
const VecFx32 _020FE3E8 = { 0, -0x20000, 0 };
const VecFx32 _020FE3DC = { 0, -0x20000, 0 };
const u32 _020FE3F4[4] = { 12, 13, 14, 15 };
const FollowCallback _020FE414[4] = { sub_02065B70, sub_02065C2C, sub_02065C48, sub_02065C90 };
const FollowCallback _020FE424[4] = { sub_02065BE8, sub_02065C2C, sub_02065C48, sub_02065C90 };
const FollowCallback _020FE404[4] = { sub_02065A4C, sub_02065C2C, sub_02065C48, sub_02065C90 };
const u32 _020FE434[4] = { 16, 17, 18, 19 };
const u32 _020FE444[4] = { 20, 21, 22, 23 };
