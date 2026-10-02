#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
typedef struct {u8 revision,count;u16 size;u16 pad,offset;} Dict;
typedef struct {u16 flags,pad;s32 translation[3];} Node;
typedef struct {u32 flags;u8 pad[72];s32 translation[3];} Result;
typedef struct {u8 *command;u8 pad[208];Dict *model;u8 pad2[16];void (*scale)(Result *,void *);} State;
extern State *_021DA700;
void getMdlTrans_(Result *);void getMdlScale_(Result *);
static inline void *GetData(Dict *dict,u32 index){if(dict && index<dict->count){u16 *data=(u16*)((u8*)dict+dict->offset);return (u8*)(data+2)+*data*index;}return 0;}
static inline Node *GetNode(Dict *dict,u8 index){if(dict){u32 *entry=GetData(dict,index);if(entry)return (Node*)((u8*)dict+*entry);}return 0;}
void getMdlTrans_(Result *out){State *state=_021DA700;Node *node=GetNode(state->model,state->command[1]);if(node->flags&1){out->flags|=4;return;}out->translation[0]=node->translation[0];out->translation[1]=node->translation[1];out->translation[2]=node->translation[2];}
