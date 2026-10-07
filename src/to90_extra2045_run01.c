#pragma force_active on
#include "global.h"
extern void *sub_02030FA0(void *),*Save_Frontier_GetStatic(void *),*Party_GetMonByIndex(void *,int);
extern int BattleArcade_GetOpponentMonCount(u8,int),sub_0205C2C0(u8),sub_0205C2E8(u8),sub_0205C268(int);
extern void sub_02030EB4(void *,int,u8,int,void *),sub_02030EA0(void *,int),sub_02031108(void *,int,int,u32),sub_0203126C(void *,int,int,u32),sub_02030FB0(void *,int,u8,int,void *);
extern u32 Party_GetCount(void *);
extern u32 GetMonData(void *,int,void *);
void ov80_02234588(void *,int);
#define BYTE(o) (*((u8 *)work+(o)))
#define HALF(o) (*(u16 *)((u8 *)work+(o)))
#define PTR(o) (*(void **)((u8 *)work+(o)))
#pragma force_active on
void ov80_02234588(void *work,int mode){
 struct{union{u16 word;u32 words[2];}buffer;u8 byte;}f;
 void *record,*frontier;int index,key;u32 count;u32 i;
 record=sub_02030FA0(PTR(4));BattleArcade_GetOpponentMonCount(BYTE(0x10),1);
 f.byte=BYTE(0x10);sub_02030EB4(PTR(8),0,0,0,&f.byte);sub_02030EA0(PTR(8),1);
 f.byte=BYTE(0x11);sub_02030EB4(PTR(8),2,0,0,&f.byte);
 f.byte=BYTE(0x1c);sub_02030EB4(PTR(8),3,0,0,&f.byte);
 f.byte=BYTE(0x12);sub_02030EB4(PTR(8),1,0,0,&f.byte);
 frontier=Save_Frontier_GetStatic(PTR(4));index=sub_0205C2C0(BYTE(0x10));key=sub_0205C268(sub_0205C2C0(BYTE(0x10)));sub_02031108(frontier,index,key,HALF(0x18));
 if(mode!=2){
  frontier=Save_Frontier_GetStatic(PTR(4));index=sub_0205C2E8(BYTE(0x10));key=sub_0205C268(sub_0205C2E8(BYTE(0x10)));sub_0203126C(frontier,index,key,HALF(0x18));
  f.byte=BYTE(0x2f);sub_02030FB0(record,8,BYTE(0x10),0,&f.byte);
  if(BYTE(0x10)==3){frontier=Save_Frontier_GetStatic(PTR(4));key=sub_0205C268(110);sub_02031108(frontier,110,key,BYTE(0x2f));}
 }
 for(i=0;i<14;i=(u16)(i+1)){f.buffer.word=HALF(0x78+i*2);sub_02030EB4(PTR(8),5,(u8)i,0,&f.buffer.word);}
 for(i=0;i<3;i=(u16)(i+1)){f.byte=BYTE(0x2c+i);sub_02030EB4(PTR(8),6,(u8)i,0,&f.byte);}
 count=Party_GetCount(PTR(0x70));for(i=0;i<count;i=(u16)(i+1)){f.buffer.word=(u16)GetMonData(Party_GetMonByIndex(PTR(0x70),i),6,NULL);sub_02030EB4(PTR(8),4,(u8)i,0,&f.buffer.word);}
 count=Party_GetCount(PTR(0x74));for(i=0;i<count;i=(u16)(i+1)){Party_GetMonByIndex(PTR(0x74),i);f.buffer.word=HALF(0x314+i*2);sub_02030EB4(PTR(8),7,(u8)i,0,&f.buffer.word);}
}
