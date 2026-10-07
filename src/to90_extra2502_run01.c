#pragma force_active on
#include "global.h"
#include "pokeathlon/pokeathlon.h"
#include "sol_r10_ov96_r54r2_private.h"
#define B(P,O) (*(u8 *)((u8 *)(P)+(O)))
#define H(P,O) (*(u16 *)((u8 *)(P)+(O)))
#define W(P,O) (*(int *)((u8 *)(P)+(O)))
#define P(PP,O) (*(void **)((u8 *)(PP)+(O)))
typedef struct {int v[3];} Vector;
typedef struct {int v[9];} Sync;
typedef struct { u32 touched;u8 x,y,pad06[2],ready,pad09[27]; } Input;



extern int ov96_021F32C4(int),ov96_021F20C4(void *,u8,int,int),ov96_021F2F7C(void *,int,int),ov96_021F342C(void *,void*);

extern void ov96_021E8228(void *,u32,u32,int,int),ov96_021F208C(int,int,const VecFx32 *,fx32,VecFx32 *),GF_AssertFail(void),ov96_021F1CC0(void *),ov96_021F21EC(void *),ov96_021F2834(void *),ov96_021F0F04(void *,void *);
typedef struct { int count;u8 *area;union{Input *ready;u8 *drag;} phase; } Frame;
void ov96_021F1170(void *);
#pragma force_active on
void ov96_021F1170(void *ctx) {
 Frame frame;frame.area=PokeathlonCourse_GetDataCopyArea((PokeathlonCourseData*)ctx);u8 *w=PokeathlonCourse_GetHeapAllocPtr4((PokeathlonCourseData*)ctx);if(ov96_021E5F24((PokeathlonCourseData*)ctx))return;
 void *info=ov96_021E8A20(frame.area+40);
 if(W(w,0x7ec)==1){u8 ready=0;int i;frame.count=PokeathlonCourse_GetParticipantCount((PokeathlonCourseData*)ctx);i=ready;if(frame.count>0){u8*entry=frame.area+80;do{frame.phase.ready=(Input*)ov96_021E8A20(entry);if(frame.phase.ready->ready)ready++;entry+=40;i++;}while(i<frame.count);}if(ready==frame.count){B(w,0x726)=0;B(w,0x74a)=0;W(w,0x7ec)=0;}}
 else if(B(w,0x728)){B(w,0x728)--;if(B(w,0x728)==0)W(w,0x7ec)=1;else if(B(w,0x728)==60){B(w,0x72a)=1;B(w,0x727)++;}}
 else if(B(w,0x74a)){B(w,0x726)=44;if(B(w,0x749)==0){if(B(w,0x74d)==0){if(B(w,0x74f)>=B(w,0x74e)){ov96_021F2A00((Ov96R54Course*)w);B(w,0x728)=90;}else{B(w,0x74b)=B(w+B(w,0x74f),0x750);B(w,0x74c)=ov96_021F32C4(B(w+B(w,0x74f),0x75c));u8 player=B(w,0x74b)/3;int *scores=(int*)(w+0x6e0);scores[player]+=B(w+B(w,0x74f),0x75c);B(w,0x74f)++;B(w,0x74d)=6;ov96_021E8228(ctx,player,(u8)(B(w,0x74b)%3),3,1);}}else B(w,0x74d)--;}else B(w,0x749)--;}
 else {int limit;if(B(w,0x727)<1)limit=8;else if(B(w,0x727)<3)limit=6;else limit=4;B(w,0x725)++;if(B(w,0x725)>=limit){B(w,0x725)=0;B(w,0x726)++;if(B(w,0x726)>=43){B(w,0x72b)=1;B(w,0x748)=1;B(w,0x74a)=1;B(w,0x749)=20;int i;for(i=0;i<4;i++)B(w+i,0x720)=12;for(i=0;i<12;i++){u8 *entry=w+32+(i/3)*432+(i%3)*144;*(Vector*)(entry+0x1c)=*(Vector*)(entry+0x28);W(entry,0x34)=0;W(entry,0x38)=0;H(entry,0x8e)=H(entry,0x8c);B(entry,0x44)=0;B(entry,0x45)=0;B(entry,0x46)=0;B(entry,0x47)=0;}}}}
 if(B(w,0x727)>=6)B(w,0x729)=1;else B(w,0x729)=0;
 Sync *dst=(Sync*)ov96_021E8A20(frame.area+80);Sync *src=(Sync*)ov96_021E8A20(frame.area);*dst=*src;
 if(!B(w,0x74a)){int player=0;u8 *input=frame.area+80,*flags=w,*entry=w+32;for(;player<4;player++,input+=40,flags+=12,entry+=432){Input *data=(Input*)ov96_021E8A20(input);if(data->touched){if(W(flags,0x6f0)&&W(flags,0x6f4))W(flags,0x6f0)=0;else if(!W(flags,0x6f0)&&!W(flags,0x6f4)){W(flags,0x6f0)=1;W(flags,0x6f4)=1;}}else{W(flags,0x6f0)=0;W(flags,0x6f4)=0;}
 if(W(flags,0x6f0)){if(!ov96_021F2A84((Ov96R54Object(*)[3])w,player)){int slot=ov96_021F20C4(w,(u8)player,data->x,data->y);if(slot!=12){u8 *p=entry+(slot%3)*144;B(w+player,0x720)=slot;B(p,0x46)=2;B(p,0x47)=3;}}}
 else if(W(flags,0x6f4)){if(B(w+player,0x720)!=12){u8 slot=(u8)(B(w+player,0x720)%3);frame.phase.drag=entry+slot*144;ov96_021F208C(data->x,data->y,(const VecFx32*)(frame.phase.drag+0x28),0,(VecFx32*)(frame.phase.drag+0x1c));if(ov96_021F2F7C(P(frame.phase.drag,0),W(frame.phase.drag,0x1c),W(frame.phase.drag,0x20)))B(w+B(w+player,0x720),0x7e0)=1;else B(w+B(w+player,0x720),0x7e0)=0;}}
 else if(B(w+player,0x720)!=12)B(w+player,0x720)=12;
 }
 int slot=ov96_021F342C(P(w,0x770),(void*)(u32)(u8)H(w,0x732));if(slot!=12){u8 player=(u8)(slot/3);if(player>=PokeathlonCourse_GetParticipantCount((PokeathlonCourseData*)ctx)){u8 *p=w+32+player*432+(slot%3)*144;B(p,0x46)=2;B(p,0x47)=3;}else GF_AssertFail();}
 }
 ov96_021F1CC0(ctx);ov96_021F21EC(w);ov96_021F2834(w);ov96_021F0F04(w,info);
}
