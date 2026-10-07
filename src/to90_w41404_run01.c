#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u8 prefix[0x18];u32 *pc;u8 gap[0x78];u32 args[10];} Work;
typedef void (*Callback)(void*);
extern const void *ov07_02223038(u32);
void ov07_0221D0E8(Work*);
void ov07_0221D0E8(Work *w){
    Callback callback;
    int i;
    u32 count;
    u32 opcode;
    w->pc++;
    opcode=*w->pc;
    w->pc++;
    callback=(Callback)ov07_02223038(opcode);
    count=*w->pc;
    w->pc++;
    for(i=0;(u32)i<count;i++){
        w->args[i]=*w->pc;
        w->pc++;
    }
    for(;i<10;i++)w->args[i]=0;
    callback(w);
}
