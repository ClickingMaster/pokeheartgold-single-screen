#pragma force_active on
#include "global.h"
#include <string.h>
#define P(d,n) ((char*)(d)+(n))
#define W(d,n) (*(void**)P(d,n))
#define B(d,n) (*(u8*)P(d,n))
typedef struct Pair{s16 x,y;} Pair;
typedef struct Vector{int x,y,z;} Vector;
typedef struct Record{char bytes[12];} Record;
typedef struct Entry{u16 id,type;} Entry;
extern Entry ov49_02269AAC[],ov49_02269A88[];
void*Heap_Alloc(int,int);void*ov49_0225CC4C(int,int,int,u32);void ov49_0225CDEC(void*,u32,u32,u32,u32);
int WiFiPlazaMap_FindTileTag(void*,int,u16*,u16*,int);void*ov49_0225D098(void*,int,u16,u16);
void ov49_0225D214(void*,void*,int,int);void ov49_0225EB00(Record*,void*);void ov49_0225D494(void*,int);void ov49_0225EE4C(void*,int);
u32 ov49_0225D1EC(void*);void ov49_0225D1C4(void*,Pair);void*ov49_0225CF28(void*,s32,s32,const void*);void ov49_0225D040(void*,int);
void*ov49_0225DF18(int,int,void*,int,u32);

typedef struct { u8 pad[0x208]; void *specials[9]; } DObj;
static inline void CreateSpecials(void *d,Vector *vector){int j;
 vector->x=0;vector->z=0x28000;vector->y=0;
 for(j=0;j<9;j++){
 ((DObj*)d)->specials[j]=ov49_0225CF28(W(d,4),ov49_02269A88[j].id,ov49_02269A88[j].type,vector);ov49_0225D040(((DObj*)d)->specials[j],0);
 }
}
// @fn ov49_0225DF18
void*ov49_0225DF18(int a,int b,void*input,int heap,u32 extra){void*d=Heap_Alloc(heap,0x614);Entry*entry;struct {struct {u16 y,x;struct {u16 x,y;} original;Pair moved;} coords;Vector vector;} scratch;Record *groupD,*groupB,*groupA;u32 i;int n;
 memset(d,0,0x614);W(d,0)=input;B(d,0x610)=a;B(d,0x611)=b;W(d,4)=ov49_0225CC4C(9,128,heap,extra);ov49_0225CDEC(W(d,4),b,a,heap,extra);
 for(i=0,entry=ov49_02269AAC;i<35;i++,entry++){
 n=0;
 if(WiFiPlazaMap_FindTileTag(W(d,0),entry->id,&scratch.coords.x,&scratch.coords.y,0)==1){groupA=(Record*)P(d,0x248);groupB=(Record*)P(d,0x368);groupD=(Record*)P(d,0x4e8);
 do{
 W(P(d,B(d,0x612)*4),8)=ov49_0225D098(W(d,4),entry->type,scratch.coords.x,scratch.coords.y);
 switch(entry->type){
 case 6:case 7:case 8:ov49_0225D214(W(d,4),W(P(d,B(d,0x612)*4),8),0,0);W(P(d,entry->type*4),0x214)=W(P(d,B(d,0x612)*4),8);break;
 case 0:ov49_0225D214(W(d,4),W(P(d,B(d,0x612)*4),8),0,0);ov49_0225D214(W(d,4),W(P(d,B(d,0x612)*4),8),1,0);ov49_0225D214(W(d,4),W(P(d,B(d,0x612)*4),8),2,0);break;
 case 10:ov49_0225D214(W(d,4),W(P(d,B(d,0x612)*4),8),0,2);W(P(d,entry->id*4),0x114)=W(P(d,B(d,0x612)*4),8);break;
 case 11:case 12:ov49_0225D214(W(d,4),W(P(d,B(d,0x612)*4),8),0,2);break;
 case 13:ov49_0225EB00(&groupA[B(d,0x608)],W(P(d,B(d,0x612)*4),8));B(d,0x608)++;GF_ASSERT(B(d,0x608)<=24);ov49_0225D494(W(P(d,B(d,0x612)*4),8),0);if((u16)(entry->id+0xffa4)<=1)ov49_0225EE4C(W(P(d,B(d,0x612)*4),8),entry->id);break;
 case 14:ov49_0225EB00(&groupB[B(d,0x609)],W(P(d,B(d,0x612)*4),8));B(d,0x609)++;GF_ASSERT(B(d,0x608)<=24);ov49_0225D494(W(P(d,B(d,0x612)*4),8),0);if((u16)(entry->id+0xffa4)<=1)ov49_0225EE4C(W(P(d,B(d,0x612)*4),8),entry->id);break;
 case 15:W(P(d,B(d,0x60a)*4),0x488)=W(P(d,B(d,0x612)*4),8);B(d,0x60a)++;GF_ASSERT(B(d,0x60a)<=24);ov49_0225D494(W(P(d,B(d,0x612)*4),8),0);break;
 case 16:ov49_0225EB00(&groupD[B(d,0x60b)],W(P(d,B(d,0x612)*4),8));B(d,0x60b)++;GF_ASSERT(B(d,0x60b)<=24);ov49_0225D494(W(P(d,B(d,0x612)*4),8),0);
 {u32 pair=ov49_0225D1EC(W(P(d,B(d,0x612)*4),8));scratch.coords.original.x=(s16)pair;scratch.coords.original.y=(s16)(pair>>16);}scratch.coords.moved.x=scratch.coords.original.x;scratch.coords.moved.y=scratch.coords.original.y;scratch.coords.moved.x+=8;scratch.coords.moved.y+=20;ov49_0225D1C4(W(P(d,B(d,0x612)*4),8),scratch.coords.moved);break;
 default:break;
 }
 B(d,0x612)++;n++;
 }while(WiFiPlazaMap_FindTileTag(W(d,0),entry->id,&scratch.coords.x,&scratch.coords.y,n)==1);
 }
 }
 CreateSpecials(d,&scratch.vector);
 B(d,0x613)=9;return d;
}
