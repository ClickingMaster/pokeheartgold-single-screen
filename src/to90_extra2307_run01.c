#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct{u8 pad[0x1c];u8 type;} Data;typedef struct{u8 pad[0x30];Data data;} Mid;
typedef struct{u32 a;void*data;} Global;extern Global _021D41CC;
typedef struct{u8 pad[0x18];int selected,unk1c,mode,unk24,unk28,unk2c,next;u8 pad2[0x110-0x34];void*items[1];} Work;
int sub_0205A284(Work*,int,int);
int sub_0205A284(Work*w,int index,int kind){u8*item;Data*data;index--;item=w->items[index];if(item==NULL)return 5;data=&((Mid*)(item+0x50))->data;_021D41CC.data=data;switch(data->type){case 2:if(kind!=3)return 5;w->next=5;w->selected=index;w->mode=1;w->unk24=0;w->unk1c=0;return 1;case 0:if(kind!=1)return 5;w->selected=index;w->mode=1;w->unk24=0;w->unk1c=0;return 1;case 1:if(kind!=2)return 5;w->selected=index;w->mode=2;w->unk24=0;w->unk1c=0;return 1;case 3:case 13:if(kind!=4)return 5;w->next=6;w->selected=index;w->mode=1;w->unk1c=0;return 1;case 4:case 5:case 6:case 7:case 8:case 11:return 5;case 9:case 10:case 12:default:return 5;}}
