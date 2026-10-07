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
extern void *NNS_G3dGetResDataByName(void *,const void *);
static inline void *GetName(Dict *dict,u32 index){if(dict && index<dict->count){u16 *data=(u16*)((u8*)dict+dict->offset);return (u8*)data+data[1]+index*16;}return 0;}
int NNS_G3dBindMdlPltt(Model *model,Texture *texture){List *dict;u32 i;int success=1;Materials *mat=GetMaterials(model);dict=(List*)((u8*)mat+mat->paletteOffset);i=0;while(i<dict->count){void *name=GetName((Dict*)dict,i);void *data;if(texture && texture->paletteOffset)data=NNS_G3dGetResDataByName((u8*)texture+texture->paletteOffset,name);else data=0;if(data){Binding *bind=GetData((Dict*)dict,i);if(!(bind->flags&1))bindMdlPltt_Internal_(mat,bind,texture,data);}else success=0;i++;}return success;}
