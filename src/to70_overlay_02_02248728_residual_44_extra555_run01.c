#pragma force_active on
#include "global.h"
#include "constants/sndseq.h"
extern void *memset(void *,int,u32);
extern void HeapExp_FndInitAllocator(void *,int,int);
extern int PlayerAvatar_GetFacingDirection(void *);
extern void ov02_0224D0C8(void *,u8,u8,int,void *);
extern void ov02_0224D1AC(void *);
extern void ov02_0224D0AC(void *,void *);
extern u16 GF_DegreeToSinCosIdxNoWrap(u16);
extern void Field3dObject_SetXRotation(void *,u16,int);
extern void PlaySE(int);
typedef struct {u8 a,b;u8 pad[4];u16 angle[4];} Template;
extern Template ov02_022538EC;
typedef struct {u8 p[0x40];void *avatar;} Field;
typedef struct {u8 p[0xdc];u8 allocator[16];u16 state;} Work;
void ov02_0224D5B4(void *,Field *,Work *);
void ov02_0224D5B4(void *unused,Field *field,Work *w){struct {u8 b,a;u16 angle[4];} t;int dir;t.a=ov02_022538EC.a;t.b=ov02_022538EC.b;t.angle[0]=ov02_022538EC.angle[0];t.angle[1]=ov02_022538EC.angle[1];t.angle[2]=ov02_022538EC.angle[2];t.angle[3]=ov02_022538EC.angle[3];memset(w,0,240);HeapExp_FndInitAllocator(w->allocator,4,32);dir=PlayerAvatar_GetFacingDirection(field->avatar);ov02_0224D0C8(w,t.a,t.b,1,w->allocator);ov02_0224D1AC(w);ov02_0224D0AC(field->avatar,w);Field3dObject_SetXRotation(w,GF_DegreeToSinCosIdxNoWrap(t.angle[dir]),1);PlaySE(SEQ_SE_DP_FW463);w->state=0;}
