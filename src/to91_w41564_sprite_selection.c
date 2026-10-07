#include "global.h"
#include "unk020932e0_opus_partial_internal.h"
int sub_02094794(UnkStruct_020932E0 *, int);
int sub_02094860(UnkStruct_020932E0 *, int);
void sub_020948C4(UnkStruct_020932E0 *, int, int);
void sub_0209569C(UnkStruct_020932E0 *);
void sub_02094A70(UnkStruct_020932E0 *);
#pragma force_active on
int sub_02095354(UnkStruct_020932E0 *);
typedef char TrueOwnerExtent[sizeof(UnkStruct_020932E0)==0x46e8?1:-1];
typedef char TrueRowsOffset[offsetof(UnkStruct_020932E0,rowsPtr)==0x46a4?1:-1];
typedef char TrueEntriesOffset[offsetof(UnkStruct_020932E0,entriesPtr)==0x46a0?1:-1];
// @fn sub_02095354
int sub_02095354(UnkStruct_020932E0 *w){int result=0;u32 mode=sub_02095DD8(w->cursor),selected=sub_02095DE8(w->cursor);
switch(mode){case 2:case 6:
if(sub_02094794(w,selected)==1){int i;sub_020948C4(w,1,selected);for(i=0;i<w->count;i++){VecFx32 point={0,0,0},temp;if(!w->rowsPtr[i].unk_04){w->posX=212;w->posY=40*((u8)i+1);point.x=(s32)((u32)w->posX*4096);point.y=(s32)((u32)w->posY*4096);temp=point;temp.y=(s32)((u32)temp.y-0x4000);Sprite_SetMatrix(w->entriesPtr[w->unk_4684].sprite,&temp);Sprite_SetMatrix(w->spriteB,&point);sub_02095D40(w->cursor,4,(u8)i);break;}}w->unk_46BC=1;result=1;}break;
case 3:
if(sub_02094860(w,selected)==1){VecFx32 old;sub_020948C4(w,2,selected);old=*Sprite_GetMatrixPtr(w->entriesPtr[w->unk_4684].sprite);old.y=(s32)((u32)old.y-0x4000);Sprite_SetMatrix(w->entriesPtr[w->unk_4684].sprite,&old);w->unk_46BC=1;result=1;w->state=4;}break;
case 1:sub_0209569C(w);result=3;break;
case 4:sub_02094A70(w);break;
case 0:case 5:break;
}return result;}
