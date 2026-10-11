#pragma force_active on
#include "global.h"
#pragma force_active on
extern int PaletteData_GetSelectedBuffersBitmask(void*);
extern void ov08_02223F34(void*),ov08_02223464(void*),ov08_02223228(void*),ov08_022230CC(void*),BattleDpadCursor_Free(void*),FontID_Release(int),DestroySysTaskAndEnvironment(void*);
extern int BattleDpadCursor_IsActive(void*);
extern void *BattleSystem_GetBagCursor(void*);
extern void BagCursor_Battle_PocketSetPosition(void*,int,int,int),BagCursor_Battle_SetPocket(void*,int);
typedef struct {void*state,*p4,*palette;u8 pad[0x28];void*aux;} Work;
int ov08_02222E2C(void*task,Work*w);
int ov08_02222E2C(void*task,Work*w){void*cursor;u8 i;if(PaletteData_GetSelectedBuffersBitmask(w->palette))return 0;
 ov08_02223F34(w);ov08_02223464(w);ov08_02223228(w);ov08_022230CC(w->p4);
 *((u8*)w->state+0x25)=BattleDpadCursor_IsActive(w->aux);BattleDpadCursor_Free(w->aux);FontID_Release(4);
 if(*(u16*)((u8*)w->state+0x1c)){cursor=BattleSystem_GetBagCursor(*(void**)w->state);for(i=0;i<5;i++)BagCursor_Battle_PocketSetPosition(cursor,i,*((u8*)w->state+i+0x27),*((u8*)w->state+i+0x2c));BagCursor_Battle_SetPocket(cursor,*((u8*)w+0x114d));}
 *((u8*)w->state+0x26)=1;DestroySysTaskAndEnvironment(task);return 1;}
