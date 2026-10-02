#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
typedef struct AnimHeader {u8 type,pad;u16 kind;} AnimHeader;
typedef struct Anim {u32 frame,scale;AnimHeader *resource;void *callback,*next,*texture;u8 priority,count;} Anim;
typedef struct Entry {u8 type,pad;u16 kind;void (*init)(Anim*,AnimHeader*,void*);} Entry;
extern u32 _02110A00;extern Entry _02110A24[];extern void (*_02110A28[])(Anim*,AnimHeader*,void*);
void NNS_G3dAnmObjInit(Anim*,AnimHeader*,void*,void*);
void NNS_G3dAnmObjInit(Anim *obj,AnimHeader *res,void *model,void *tex){u32 i=0;obj->frame=0;obj->resource=res;obj->next=0;obj->priority=127;obj->scale=0x1000;obj->texture=tex;obj->count=0;obj->callback=0;u32 count=_02110A00;if(i<count){u8 type=res->type;do{if(type==_02110A24[i].type && res->kind==_02110A24[i].kind){void(*init)(Anim*,AnimHeader*,void*)=*(void(**)(Anim*,AnimHeader*,void*))((u8*)_02110A28+i*8);if(!init)return;init(obj,res,model);return;}}while(++i<count);}}
