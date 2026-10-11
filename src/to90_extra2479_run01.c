#pragma force_active on
#include "global.h"
#include "heap.h"
#pragma force_active on
typedef struct { u32 zero; u32 ids[4]; void *names[4]; u32 heap; u8 flag, multi; u8 unused[6]; } Request;
typedef struct { u8 beforeFlags[44]; u32 flags; u8 beforeProfiles[24]; void *profiles[4]; u8 beforeIDs[16]; u32 ids[4]; u8 beforeRequest[0x14c]; Request *request; u8 beforeState[0x2244]; u32 state; } Work;
void *OverlayManager_GetData(void *), *OverlayManager_GetArgs(void *), *PlayerProfile_GetPlayerName_NewString(void *, int);
void WirelessStatusIcon_Destroy(void), ov05_0221BA00(Request *);
int Comm_GetLocalNetId(void);
int sub_020378AC(int);
int ov12_0223A260(void *);
int ov12_0223A260(void *manager)
{
 Work *work = OverlayManager_GetData(manager);
 u8 player;
 int i, destination;
 OverlayManager_GetArgs(manager);
 if (!(work->flags & 4) || (work->state & 16) || (work->flags & 128)) {
  WirelessStatusIcon_Destroy(); return 0;
 }
 player = Comm_GetLocalNetId();
 work->request = Heap_Alloc(HEAP_ID_BATTLE, sizeof(Request));
 MI_CpuClearFast(work->request, sizeof(Request));
 if (work->flags & 8) {
  Work *cursor;
  for (i = 0, cursor = work; i < 4; cursor = (Work *)((u8 *)cursor + 4), i++) {
   work->request->ids[sub_020378AC(i)] = cursor->ids[0];
   destination = sub_020378AC(i);
   work->request->names[destination] = PlayerProfile_GetPlayerName_NewString(cursor->profiles[0], 5);
  }
  work->request->heap = 5;
  work->request->flag = 1;
  work->request->multi = 1;
 } else {
  int other;
  work->request->ids[sub_020378AC(player)] = work->ids[player];
  other = player ^ 1;
  work->request->ids[sub_020378AC(other)] = work->ids[other];
  destination = sub_020378AC(player);
  work->request->names[destination] = PlayerProfile_GetPlayerName_NewString(work->profiles[player], 5);
  destination = sub_020378AC(other);
  work->request->names[destination] = PlayerProfile_GetPlayerName_NewString(work->profiles[other], 5);
  work->request->heap = 5;
  work->request->flag = 1;
  work->request->multi = 0;
 }
 ov05_0221BA00(work->request);
 return 1;
}
