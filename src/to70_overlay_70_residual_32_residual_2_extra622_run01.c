#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u8 p[0x356];u8 kind;u8 p2[0x29];u8 flag;u8 p3[0xb67];void*sprites[7];u8 p4[0x2e0];void*first;u32 pad;void*second;} Work;
extern void PlaySE(int);
extern void ov70_022412C8(void*,void*,int,int,int);
extern void Sprite_SetAnimCtrlSeq(void*,int);
extern void Sprite_SetDrawFlag(void*,int);
void ov70_02241184(Work*,int,int);
void ov70_02241184(Work*w,int count,int flag){int i,a,b;Work*record;Work*cursor;if(count && flag==1)PlaySE(0x64e);record=w;i=0;a=14;cursor=w;b=17;for(;i<7;i++){if(i<count){ov70_022412C8(w->first,w->second,i,record->flag,record->kind);if(flag)Sprite_SetAnimCtrlSeq(cursor->sprites[0],a);else Sprite_SetAnimCtrlSeq(cursor->sprites[0],b);Sprite_SetDrawFlag(cursor->sprites[0],1);}else Sprite_SetDrawFlag(cursor->sprites[0],0);record=(Work*)((u8*)record+0x124);a+=4;cursor=(Work*)((u8*)cursor+4);b+=4;}}
