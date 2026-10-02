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
extern void bindMdlPltt_Internal_(Materials *,Binding *,Texture *,void *);
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
int NNS_G3dForceBindMdlPltt(Model *model,Texture *texture,u32 matIndex,u32 texIndex){Materials *mat=GetMaterials(model);Dict *dict=(Dict*)((u8*)mat+mat->paletteOffset);void *data;if(texture && texture->paletteOffset)data=GetData((Dict*)((u8*)texture+texture->paletteOffset),texIndex);else data=0;Binding *bind=GetData(dict,matIndex);if(bind && !(bind->flags&1)){bindMdlPltt_Internal_(mat,bind,texture,data);return 1;}return 0;}
void NNS_G3dReleaseMdlPltt(Model *model){Materials *mat=GetMaterials(model);List *dict=(List*)((u8*)mat+mat->paletteOffset);u32 i=0;while(i<dict->count){Binding *bind=GetData((Dict*)dict,i);if(bind->flags&1)bind->flags&=~1;i++;}}
int NNS_G3dBindMdlSet(ModelSet *set,Texture *texture){u32 i;int success=1;i=0;while(i<set->count){Model *model=GetModel(set,i);success&=NNS_G3dBindMdlTex(model,texture);success&=NNS_G3dBindMdlPltt(model,texture);i++;}return success;}
void NNS_G3dReleaseMdlSet(ModelSet *set){u32 i=0;while(i<set->count){Model *model=GetModel(set,i);NNS_G3dReleaseMdlTex(model);NNS_G3dReleaseMdlPltt(model);i++;}}
