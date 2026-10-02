#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct Input{u8 pad[0x15];u8 x,y;u8 pad17[2];u8 a,b;u8 flag:1;u8 flags:7;u16 value;}Input;typedef struct Output{u8 a,b,x,y;u16 value;u8 flag:1;u8 flags:7;u8 pad;}Output;
typedef struct Blob{u32 words[11];}Blob;typedef struct Args{u32 value;int heap;void*owner;Blob blob;u32 a,b;}Args;
typedef struct W{void*owner;u32 a,b,value;u8 version,mode;u8 pad12[0x132];int heap;u8 pad148[4];int selected;Blob blob;u8 pad17c[16];void*allocation;u8 pad190[0x224];void*buffer;volatile int enabled;u8 pad3bc[8];volatile int active;u8 pad3c8[0x20];int size1,size2;u8 pad3f0[4];void*msg,*format,*text;u8 tail[0x14];}W;
extern void String_Delete(void*),MessageFormat_Delete(void*),DestroyMsgData(void*),Heap_Free(void*),ov39_02227E3C(void),ov39_0222A130(W*);extern int ov39_0222A2C0(int),ov39_0222A110(void*);extern void*Heap_Alloc(int,int),*NewMsgDataFromNarc(int,int,int,int),*MessageFormat_New(int),*String_New(int,int),*ov39_02227DEC(int);void ov39_022271CC(void*,Input*,Output*),ov39_02227208(W*,Args*),ov39_022272EC(W*);
void ov39_022272EC(W*w){ov39_0222A130(w);w->active=0;String_Delete(w->text);MessageFormat_Delete(w->format);DestroyMsgData(w->msg);Heap_Free(w->buffer);ov39_02227E3C();Heap_Free(w->allocation);}
