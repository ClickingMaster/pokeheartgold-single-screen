#pragma force_active on
#include "global.h"
extern void ov85_021E8454(void *);
extern u32 LCRandom(void);
extern void ov85_021E7274(void *,int,u32);
extern void *Save_Bag_Get(void *);
extern int Bag_AddItem(void *,u16,int,int);
extern void PlayFanfare(int);
extern u32 ov85_021EA728[][2],ov85_021EA72C[][2];
typedef struct {u8 p[12];int item;} Selection;
typedef struct {u8 p[28];void *save;} Args;
typedef struct {int state;u8 p[28];u32 flags;u32 unk;Selection *selection;u32 unk2;int level;u8 p2[0x98];Args *args;} Work;
int ov85_021E6498(Work *);
int ov85_021E6498(Work *w){u32 item,high;int level;int result;ov85_021E8454(w);if(!(w->flags&(1<<w->selection->item))){w->state=43;return 0;}level=w->level;item=ov85_021EA728[level][0];high=ov85_021EA72C[level][0];item+=LCRandom()%(high-item+1);if(item<ov85_021EA728[level][0])item=ov85_021EA728[level][0];else if(item>high)item=high;ov85_021E7274(w,3,item);result=Bag_AddItem(Save_Bag_Get(w->args->save),(u16)item,1,102);PlayFanfare(1185);if(result==1)w->state=40;else w->state=41;return 0;}
