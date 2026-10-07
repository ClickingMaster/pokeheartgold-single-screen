#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
typedef struct {u8 revision,count;u16 size;u16 pad,offset;} Dict;
typedef struct {u8 revision,count;u16 size;u16 pad,offset;} List;
typedef struct {u8 pad[8];u32 offset;} Model;
typedef struct {u16 texOffset,paletteOffset;} Materials;
typedef struct {u16 offset;u8 count,flags;} Binding;
typedef struct {u8 pad[8];u8 revision,count;u8 rest[6];} ModelSet;
typedef struct {u8 pad[52];u16 paletteOffset;u8 rest[6];Dict texDict;} Texture;
extern void bindMdlTex_Internal_(Materials *,Binding *,Texture *,void *);
extern void releaseMdlTex_Internal_(Materials *,Binding *);
extern void bindMdlPltt_Internal_(void *,void *,void *,void *);
int NNS_G3dForceBindMdlTex(Model *,Texture *,u32,u32);
int NNS_G3dForceBindMdlPltt(Model *,Texture *,u32,u32);
void NNS_G3dReleaseMdlTex(Model *);void NNS_G3dReleaseMdlPltt(Model *);
void NNS_G3dReleaseMdlSet(ModelSet *);
int NNS_G3dBindMdlSet(ModelSet *,Texture *);
extern int NNS_G3dBindMdlTex(Model *,Texture *);
extern int NNS_G3dBindMdlPltt(Model *,Texture *);
static inline void *GetData(Dict *dict,u32 index){if(dict && index<dict->count){u16 *data=(u16*)((u8*)dict+dict->offset);return (u8*)(data+2)+*data*index;}return 0;}
static inline Materials *GetMaterials(Model *model){if(model && model->offset)return (Materials*)((u8*)model+model->offset);return 0;}
static inline Model *GetModel(ModelSet *set,u32 index){if(set){u32 *data=GetData((Dict*)((u8*)set+8),index);if(data)return (Model*)((u8*)set+*data);}return 0;}
typedef struct {u8 pad[20];u32 texture;u8 pad2[12];s32 scaleX,scaleY;} Material;
static inline Material *GetMaterial(Materials *mat,u32 index){if(mat){u32 *data=GetData((Dict*)((u8*)mat+4),index);if(data)return (Material*)((u8*)mat+*data);}return 0;}
void releaseMdlTex_Internal_(Materials *mat,Binding *binding){u8 *indices=(u8*)mat+binding->offset;u32 i=0;while(i<binding->count){Material *material=GetMaterial(mat,indices[i]);material->texture&=0xc00f0000;material->scaleX=4096;material->scaleY=4096;i++;}binding->flags&=~1;}
