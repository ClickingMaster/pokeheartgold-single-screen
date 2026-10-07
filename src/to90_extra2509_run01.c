#pragma force_active on
#include "global.h"
#include "msgdata.h"
#pragma force_active on

extern u8 ov27_0225CFC8[][8];
extern struct {u16 a;u16 b:15,flag:1;} ov27_0225CF94[];
extern int ov27_0225CED8[];
typedef struct{u8 p[16];void*args;u8 q[0x460];struct{void*text;int a;} entries[6];void*last;MsgData*msg;MessageFormat*fmt;String*strings[5],*s1,*s2;u8 r[0x50];struct{u32 first:1,selection:4,rest:27;}flags;}Work;
void ov27_0225BB6C(Work*);
void ov27_0225BB6C(Work*w){int i,j;Work*cursor;BufferPlayersName(w->fmt,0,Save_PlayerData_GetProfile(((void**)w->args)[3]));i=0;cursor=w;for(;i<7;i++,cursor=(Work*)((u8*)cursor+8)){u8 id=ov27_0225CFC8[w->flags.selection][i];if(id!=13)cursor->entries[0].text=ReadMsgData_ExpandPlaceholders(w->fmt,w->msg,ov27_0225CF94[id].b,(enum HeapID)8);}for(j=0;j<5;j++)w->strings[j]=NewString_ReadMsgData(w->msg,ov27_0225CED8[j]);w->s1=ReadMsgData_ExpandPlaceholders(w->fmt,w->msg,12,(enum HeapID)8);w->s2=ReadMsgData_ExpandPlaceholders(w->fmt,w->msg,13,(enum HeapID)8);}
