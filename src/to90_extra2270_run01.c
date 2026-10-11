#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {int x,y;} Position;
extern const Position sBattlePartyListIconPos[],sBattlePartyListStatusIconPos[];
extern void BattlePartyMenu_ShowSpriteAt(void *,int,int),BattlePartyMenu_ShowStatusIcon(int,void *,int,int),BattlePartyMenu_ShowHeldItemIcon(int,void *,int,int),BattlePartyMenu_ShowCapsuleIcon(int,void *,int,int);
typedef struct {u8 data[0x2040];} State;
void BattlePartyMenu_ShowPartyListSprites(State *);
void BattlePartyMenu_ShowPartyListSprites(State *s){
 int i;u8 *a,*bits;const Position *first=sBattlePartyListIconPos,*second=sBattlePartyListStatusIconPos;u8 *views;
 for(i=0,a=s->data,views=s->data,bits=s->data+27;i<6;bits+=80,a+=80,first++,views+=4,second++,i++){
  if(*(u16 *)(a+8)!=0){
   BattlePartyMenu_ShowSpriteAt(*(void **)(views+0x1fd4),first->x,first->y);
   BattlePartyMenu_ShowStatusIcon((((u32)*bits<<25)>>28),*(void **)(views+0x1fec),second->x,second->y);
   BattlePartyMenu_ShowHeldItemIcon(*(u16 *)(a+30),*(void **)(views+0x1fb8),first->x+8,first->y+8);
   BattlePartyMenu_ShowCapsuleIcon(a[49],*(void **)(views+0x2038),first->x+16,first->y+8);
  }
 }
}
