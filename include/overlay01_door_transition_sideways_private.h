#ifndef POKEHEARTGOLD_OVERLAY01_DOOR_TRANSITION_SIDEWAYS_PRIVATE_H
#define POKEHEARTGOLD_OVERLAY01_DOOR_TRANSITION_SIDEWAYS_PRIVATE_H

#include "global.h"

#include "constants/sndseq.h"

#include "field/fieldmap.h"
#include "field/map_prop.h"
#include "field/map_prop_animation.h"

#include "field_metatile_query_internal.h"
#include "field_system.h"
#include "follow_mon.h"
#include "map_object.h"
#include "player_avatar.h"
#include "screen_fade.h"
#include "unk_02062108.h"

typedef struct DoorTransitionWork2 {
    int state;
    int x;
    int z;
    u8 active;
    u8 pad0D[3];
} DoorTransitionWork2;

typedef struct DoorModelList {
    int ids[4];
} DoorModelList;

extern const DoorModelList ov01_02206408;
extern const DoorModelList ov01_02206418;
extern NNSG3dResTex *ov01_021FB9E0(AreaDataManager *);
extern void FollowMon_MoveToPlayer(FieldSystem *, u8);
extern void FollowMon_QueuePlayerStep(int, int, int, void *);
extern void StopSE(u16, int);
extern BOOL ov01_021E971C(FieldSystem *, DoorTransitionWork2 *, u8);
extern BOOL ov01_021E98F0(FieldSystem *, DoorTransitionWork2 *, u8);

#endif
