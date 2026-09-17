#ifndef TO50_MAP_OBJECT_FOLLOW_CALLBACKS_PRIVATE_H
#define TO50_MAP_OBJECT_FOLLOW_CALLBACKS_PRIVATE_H
#include "map_object_follow_internal.h"
typedef BOOL (*FollowCallback)(LocalMapObject *, FollowState *);
BOOL sub_02065A4C(LocalMapObject *, FollowState *);
BOOL sub_02065B70(LocalMapObject *, FollowState *);
BOOL sub_02065BE8(LocalMapObject *, FollowState *);
void sub_02065900(LocalMapObject *);
void sub_02065938(LocalMapObject *);
void sub_02065968(LocalMapObject *);
#endif
