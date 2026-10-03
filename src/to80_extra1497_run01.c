#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct Results {u32 heap;void *bg,*a,*sprites,*manager,*other;u32 pad[10];void *sub40,*sub44,*sub48,*sub4c;} Results;
typedef struct Record {u32 a,b,c,d;} Record;
typedef struct Params {u32 a,b,c,d,e;u32 ids[9];u32 pad[3];} Params;
extern Results *PokeathlonCourse_GetHeapAllocPtr4(void *);
extern u32 PokeathlonCourse_GetField1ED(void *);
extern void PokeathlonCourse_IncrementField1ED(void *),PokeathlonCourse_SetField1F4(void *,int),PokeathlonCourse_SetVBlankIntrCB(void *);
extern Results *PokeathlonCourse_AllocPtr4FromHeap(void *,u32);
extern void Heap_Create(int,int,u32),Main_SetVBlankIntrCB(void *,void *),Main_SetHBlankIntrCB(void *,void *),GfGfx_DisableEngineAPlanes(void),GfGfx_DisableEngineBPlanes(void),GfGfx_SwapDisplay(void),MI_CpuFill8(void *,u8,u32),GF_AssertFail(void);
extern u8 gSystem[];
extern void *BgConfig_Alloc(u32),*SpriteManager_GetSpriteList(void *);
extern void ov96_02209820(void),ov96_021E6670(void *,int),ov96_02209C14(void *,u32),ov96_02209DE4(void *),ov96_02209D14(Results *),ov96_02209840(Results *);
extern void *ov96_021EA854(u32,int,int,void *,void *);
extern int ov96_021E5F24(void *);
extern void ov96_021E6168(void *,int,int,Record *);
extern void *ov96_021E60C0(void *,int,int);
extern u32 ov96_021E6108(void *);
extern void ov96_021EA8A8(void *,int,Record *,Params *,int,int);
extern int ov96_021EAA00(void *);
extern void ov96_02209A14(Results *),ov96_02209B04(void *,void *);
extern void **ov96_021E61D8(void *,int,void *,void *);
extern void Sprite_SetDrawPriority(void *,int),ov96_02209910(Results *);
extern void *ov96_0220A744(u32,void *,void *,void *),*ov96_0220B374(u32,void *,void *,void *),*ov96_0220B7F4(u32,void *,void *,void *,void *),*ov96_0220C93C(void *,u32);
extern void ov96_021EB144(void *,int),ov96_02209F14(Results *),ov96_02209F8C(void *),sub_0203A994(int),GfGfx_EngineATogglePlanes(int,int),GfGfx_EngineBTogglePlanes(int,int),ov96_0220C9CC(void *),BeginNormalPaletteFade(int,int,int,int,int,int,u32),ov96_0220A424(void *),ov96_0220A4DC(void *);
extern int IsPaletteFadeFinished(void);
int ov96_02209448(void *);
int ov96_02209448(void *course){Results *r=PokeathlonCourse_GetHeapAllocPtr4(course);int i,j;
 switch(PokeathlonCourse_GetField1ED(course)){
 case 0:Heap_Create(92,141,0x60000);Main_SetVBlankIntrCB(NULL,NULL);Main_SetHBlankIntrCB(NULL,NULL);GfGfx_DisableEngineAPlanes();GfGfx_DisableEngineBPlanes();*(vu32*)0x4000000 &=~0x1f00;*(vu32*)0x4001000 &=~0x1f00;ov96_02209820();gSystem[0x69]=1;GfGfx_SwapDisplay();r=PokeathlonCourse_AllocPtr4FromHeap(course,0x270);MI_CpuFill8(r,0,0x270);r->heap=141;*(u32*)((u8*)r+0x260)=900;PokeathlonCourse_IncrementField1ED(course);break;
 case 1:r->bg=BgConfig_Alloc(r->heap);ov96_021E6670(course,4);ov96_02209C14(r->bg,r->heap);ov96_02209DE4(course);ov96_02209D14(r);ov96_02209840(r);r->manager=ov96_021EA854(r->heap,9,32,r->other,SpriteManager_GetSpriteList(r->sprites));PokeathlonCourse_IncrementField1ED(course);break;
 case 2:{Record records[9];int n=0;Params args={0};for(i=0;i<4;i++){if(i!=ov96_021E5F24(course)){for(j=0;j<3;j++){if(n>=9)GF_AssertFail();ov96_021E6168(course,i,j,&records[n]);args.ids[n]=ov96_021E6108(ov96_021E60C0(course,i,j));n++;}}}args.b=1;args.d=1;args.e=1;ov96_021EA8A8(r->manager,9,records,&args,0,0);PokeathlonCourse_IncrementField1ED(course);break;
 }case 3:if(ov96_021EAA00(r->manager)){ov96_02209A14(r);ov96_02209B04(r->a,r->sprites);Sprite_SetDrawPriority(*ov96_021E61D8(course,0,r->other,SpriteManager_GetSpriteList(r->sprites)),0);PokeathlonCourse_IncrementField1ED(course);}break;
 case 4:ov96_02209910(r);r->sub40=ov96_0220A744(r->heap,r->a,r->sprites,course);PokeathlonCourse_IncrementField1ED(course);break;
 case 5:r->sub4c=ov96_0220B374(r->heap,r->a,r->sprites,course);r->sub44=ov96_0220B7F4(r->heap,r->a,r->sprites,course,r->manager);ov96_021EB144(r->manager,1);if(ov96_021E5F24(course)==0)r->sub48=ov96_0220C93C(course,r->heap);ov96_02209F14(r);ov96_02209F8C(course);PokeathlonCourse_SetVBlankIntrCB(r->bg);sub_0203A994(1);PokeathlonCourse_SetField1F4(course,1);GfGfx_EngineATogglePlanes(16,1);GfGfx_EngineBTogglePlanes(16,1);if(ov96_021E5F24(course)==0)ov96_0220C9CC(r->sub48);BeginNormalPaletteFade(0,1,1,0x7fff,6,1,r->heap);PokeathlonCourse_IncrementField1ED(course);break;
 case 6:if(ov96_021E5F24(course)==0)ov96_0220A424(course);ov96_0220A4DC(course);PokeathlonCourse_IncrementField1ED(course);break;
 case 7:if(IsPaletteFadeFinished())return 1;break;
 default:GF_AssertFail();break;
 }return 0;
}
