#pragma force_active on
// clang-format off
#include <nitro/types.h>
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef struct Node {struct Node *next,*prev;} Node;
typedef struct List {Node *head;u32 count;Node *tail;} List;
typedef struct Bits {u32 b0:1,b1:1,b2:1,b3:1,b4:1,b5:1,b6:1,b7:1,b8:1,b9:1,b10:1,b11:1,b12:1,b13:1,b14:1,b15:1,b16:1,b17:1,b18:1,b19:1,b20:1,b21:1,b22:1,b23:1,b24:1,b25:1,b26:1,b27:1,b28:1,b29:1,b30:1,b31:1;} Bits;
typedef struct Res {u8 *data;u8 *scl,*clr,*alp,*tex,*child,*force;u16 forcecount,pad;} Res;
typedef struct TexData {u32 a; struct {u32 format:4,width:4,height:4,repeat:2,flip:2,zero:1,shared:1,sharedidx:8,pad:6;} flags;u32 size,plttOff,plttSize;u32 pad[3];} TexData;
typedef struct Tex {TexData *data;u32 addr,pltt;u32 flags;u16 width,height;} Tex;
typedef struct Emitter {Node node;List main,child;Res *res;Bits flags;u8 pad[0x9c-0x28];} Emitter;
typedef struct Manager {void *(*alloc)(u32);List active,free,particles;Res *res;Tex *tex;u16 count,texcount;u16 max,maxp;u32 flags;void *pad;Emitter*current;void *matrix;u16 frame,framepad;} Manager;
extern void MI_CpuFill8(void*,int,u32);extern Node *spl_pop_front(List*),*spl_del(List*,Node*);extern void spl_push_front(List*,Node*);extern void spl_init(Emitter*,Res*,void*);extern void spl_draw(Manager*),sDrawChild(Manager*),sDrawParent(Manager*);
extern void GX_BeginLoadTexPltt(void),GX_EndLoadTexPltt(void),GX_BeginLoadTex(void),GX_EndLoadTex(void);extern void GX_LoadTexPltt(void*,u32,u32),GX_LoadTex(void*,u32,u32);
extern u32 (*NNS_GfdDefaultFuncAllocPlttVram)(u32,int,u32),(*NNS_GfdDefaultFuncAllocTexVram)(u32,int,u32);
void SPL_Delete(Manager*,Node*);void SPL_DeleteAll(Manager*);u32 sAllocTexPalette(u32,int),sAllocTex(u32,int);int SPL_LoadTexPlttByCallbackFunction(Manager*,u32(*)(u32,int)),SPL_LoadTexByCallbackFunction(Manager*,u32(*)(u32,int));

Emitter *SPL_CreateWithInitialize(Manager*m,u32 id,void(*cb)(Emitter*));
Emitter *SPL_Create(Manager*m,u32 id,void*vec);
void SPL_Draw(Manager*m,void*matrix);
int SPL_LoadTexPlttByVRAMManager(Manager*m);
int SPL_LoadTexByVRAMManager(Manager*m);
void spl_set_tex_dummy(void*t);

typedef void (*DrawFn)(Manager*,Node*);
extern void spl_draw_chld_bb(Manager*,Node*),spl_draw_chld_dbb(Manager*,Node*),spl_draw_chld_pol(Manager*,Node*),spl_draw_chld_dpl(Manager*,Node*),spl_draw_bb(Manager*,Node*),spl_draw_dbb(Manager*,Node*),spl_draw_pol(Manager*,Node*),spl_draw_dpl(Manager*,Node*);
extern void spl_set_tex(Tex*),spl_set_tex_dummy(void*);
typedef struct ChildFlags {u16 low:7,type:2,rest:7;} ChildFlags;
typedef struct DrawFlags {u32 low:4,type:2,rest:26;} DrawFlags;
int SPL_LoadTexPlttByCallbackFunction(Manager*m,u32(*alloc)(u32,int)){GX_BeginLoadTexPltt();for(int i=0;i<m->texcount;i++){Tex*t=&m->tex[i];TexData*d=t->data;u32 addr=0;if(d->plttSize){addr=alloc(d->plttSize,d->flags.format==2);GX_LoadTexPltt((u8*)t->data+d->plttOff,addr,d->plttSize);}t->pltt=addr;}GX_EndLoadTexPltt();return 1;}
