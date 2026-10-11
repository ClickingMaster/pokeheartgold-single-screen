#include "global.h"
#include "to91_bag_choices41560_private.h"
#include "heap.h"
#include "bag_view.h"
#include "bag.h"
#include "item.h"
#include "sound_system.h"
#pragma force_active on
typedef struct{u8 prefix[0x234];BagView*bag;Bag*save;u8 gap23c[0x7f0-0x23c];void*actions[5];u8 remaining[0x94c-0x804];}State;
typedef struct{u8 v[5];}Choices;
extern const Choices sDefaultItemMenuChoices;extern void*sItemActionFuncs[];
extern BOOL BagApp_IsBerryOrMulch(u32,u32);
extern void BagApp_DrawItemMenu(void*,const u8*);
void BagApp_SetupItemMenu(void*);
void BagApp_SetupItemMenu(void*argument){State*w=argument;Choices c=sDefaultItemMenuChoices;ItemData*item=LoadItemDataOrGfx(w->bag->itemId,0,(enum HeapID)6);u32 kind=w->bag->pockets[w->bag->unk64].pocketId;int i;for(i=0;i<5;i++)w->actions[i]=0;if(w->bag->unk65==0){u16 mode=w->bag->unk76_1;if((u16)(mode+65534)<=1){if(w->bag->pockets[w->bag->unk64].pocketId==5)c.v[0]=2;}else if(GetItemAttr_PreloadedItemData(item,6)){if(w->bag->itemId==450 &&w->bag->unk76_0==1)c.v[0]=1;else if(w->bag->pockets[w->bag->unk64].pocketId==5)c.v[0]=2;else if(w->bag->itemId==449)c.v[0]=4;else if(w->bag->pockets[w->bag->unk64].pocketId==4 &&Leftover_CanPlantBerry(w->bag->checkUseData)==1)c.v[0]=3;else if(w->bag->itemId==502 &&SoundSys_GetGBSoundsState()==1)c.v[0]=15;else c.v[0]=0;}if(!GetItemAttr_PreloadedItemData(item,3)){if(ItemIdIsNotJohtoBall(w->bag->itemId)==1)c.v[2]=8;if(kind!=3)c.v[1]=5;}if(GetItemAttr_PreloadedItemData(item,4)){if(w->bag->itemId==Bag_GetRegisteredItem1(w->save)||w->bag->itemId==Bag_GetRegisteredItem2(w->save))c.v[1]=7;else c.v[1]=6;}}else if(w->bag->unk65==6 &&BagApp_IsBerryOrMulch(kind,w->bag->itemId)==1)c.v[0]=14;if(w->bag->unk65!=6 &&kind!=3 &&kind!=4)c.v[3]=12;c.v[4]=11;for(i=0;i<5;i++)if(c.v[i]!=255)w->actions[i]=sItemActionFuncs[c.v[i]];BagApp_PrintItemSelectedMessage(w,c.v,5);BagApp_DrawItemMenu(w,c.v);Heap_Free(item);}
