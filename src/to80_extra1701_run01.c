#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct W {u8 p0[4];u8 type, a,b,p7;u16 stat;u8 pa[14];u16 values[20];u8 p40[0x220];u8 indices[2];u8 p262[6];u16 values2[20];u8 p290[0x468];void *aux, *save;u8 p700[4];u8 matrix[1];} W;
extern void *sub_02030C5C(void *);
extern void *Save_Frontier_GetStatic(void *);
extern void *SaveArray_Party_Get(void *);
extern void *Party_GetMonByIndex(void *,int);
extern u32 GetMonData(void *,int,void *);
extern void sub_02030B30(void *,int,u8,int,void *);
extern void sub_02030B1C(void *,int);
extern int sub_0205C11C(u8);
extern int sub_0205C268(int);
extern int sub_0205C0CC(u8);
extern int sub_0205C0F4(u8);
extern u16 FrontierSave_GetStat(void *,int,int);
extern void sub_02031108(void *,int,int,u16);
extern void sub_0203126C(void *,int,int,u16);
extern void sub_02030C6C(void *,int,u8,int,void *);
extern u8 sub_02030BD0(u8,void *);
extern void ov80_02231930(void *,u8,u8,u8);
void ov80_0223157C(W *,int);
void ov80_0223157C(W *w,int mode) {
 struct {u16 value;u8 pad[6];u8 byte;u8 tail[3];} temp;
 void *frontier;
 void *mon;
 void *ancillary;
 u16 species,saved,i;
 u8 type;
 int index;
 ancillary=sub_02030C5C(w->save);frontier=Save_Frontier_GetStatic(w->save);
 temp.byte=w->type;sub_02030B30(w->aux,0,0,0,&temp.byte);sub_02030B1C(w->aux,1);
 mon=Party_GetMonByIndex(SaveArray_Party_Get(w->save),w->indices[0]);
 species=(u16)GetMonData(mon,5,0);
 index=sub_0205C11C(w->type);saved=FrontierSave_GetStat(frontier,index,sub_0205C268(sub_0205C11C(w->type)));
 temp.byte=w->a;sub_02030B30(w->aux,1,0,0,&temp.byte);
 index=sub_0205C0CC(w->type);sub_02031108(frontier,index,sub_0205C268(sub_0205C0CC(w->type)),w->stat);
 if(mode!=2){
 type=w->type;
 if(type!=3){index=sub_0205C0F4(type);sub_02031108(frontier,index,sub_0205C268(sub_0205C0F4(type)),w->stat);}
 else if(saved!=species){index=sub_0205C0F4(type);sub_02031108(frontier,index,sub_0205C268(sub_0205C0F4(type)),w->stat);}
 else {index=sub_0205C0F4(type);sub_0203126C(frontier,index,sub_0205C268(sub_0205C0F4(type)),w->stat);}
 temp.byte=w->b;sub_02030C6C(ancillary,5,w->type,0,&temp.byte);
 if(w->type==3)sub_02031108(frontier,106,sub_0205C268(106),w->b);
 }
 for(i=0;i<20;i++){temp.value=w->values[i];sub_02030B30(w->aux,2,(u8)i,0,&temp.value);}
 for(i=0;i<2;i++){temp.byte=w->indices[i];sub_02030B30(w->aux,3,(u8)i,0,&temp.byte);}
 for(i=0;i<18;i++){temp.byte=sub_02030BD0((u8)i,w->matrix+w->type*9);ov80_02231930(w->save,w->type,(u8)i,temp.byte);}
 for(i=0;i<20;i++){temp.value=w->values2[i];sub_02030B30(w->aux,4,(u8)i,0,&temp.value);}
 index=sub_0205C11C(w->type);{int group=sub_0205C268(sub_0205C11C(w->type));sub_02031108(frontier,index,group,(u16)GetMonData(mon,5,0));}
}
