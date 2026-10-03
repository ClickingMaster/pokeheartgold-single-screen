#pragma force_active on
#include "global.h"
typedef struct{u32 words[4];}Data;typedef struct{int state,value,pad;Data data;void*field,*avatar,*map;}Job;typedef struct{u8 pad[0x10];void*task;u8 gap[0x40-0x14];void*avatar;}Field;extern void*ov01_021F30D0(int);extern void*PlayerAvatar_GetMapObject(void*);extern void ov01_021F2118(void);extern void TaskManager_Call(void*,void(*)(void),void*);

void ov01_021F202C(Field*f,int value,Data*data);

void ov01_021F202C(Field*f,int value,Data*data){Job*j=ov01_021F30D0(44);j->value=value;j->field=f;j->avatar=f->avatar;j->map=PlayerAvatar_GetMapObject(j->avatar);j->data=*data;TaskManager_Call(f->task,ov01_021F2118,j);}
