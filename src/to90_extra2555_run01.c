#pragma force_active on
#include "global.h"
#include "sys_task.h"
#include "overlay_28_helpers_internal.h"
#include "to45_overlay28_gap07_private.h"
#pragma force_active on
typedef struct {u8 pad[0x18];FieldSystem *field;u8 pad2[8];SpriteList *sprites;u8 pad3[0x24c-0x28];u16 low:15;u16 high:1;u8 pad4[0x338-0x24e];struct {u16 x,y;s32 touch;} input;} State;
extern const TouchscreenHitbox ov28_0225EA88[];
extern BOOL FieldSystem_TaskIsRunning(FieldSystem *);
extern void ov28_0225E578(State *,int),ov28_0225E1A4(State *),ov28_0225E730(State *),StopSE(u16,int);
extern BOOL ov28_0225E7D4(DowsingMchn *);
extern void ov28_0225E900(DowsingMchn *),ov28_0225E938(DowsingMchn *),ov28_0225E9E0(DowsingMchn *);
typedef struct Work {u8 pad0[0x210];struct {int index;u8 pad214[0x3a];s16 meter;} data;} Work31;
typedef struct {int x,y,z;} Vec43;
typedef struct {u8 p[0x190];void *sprites[4];u8 q[0x210-0x1a0];int index;u8 r[8];Vec43 a[4];u8 s[0x24c-0x24c];u16 count:15,flag:1;} Work43;
extern void ov28_0225E31C(Work31 *,int),ov28_0225E43C(Work43 *);
void ov28_0225DC2C(SysTask *,void *);
void ov28_0225DC2C(SysTask *task,void *data){State *s=data;if(s->field->unkD2_7&&FieldSystem_IsPlayerMovementAllowed(s->field))s->field->unkD2_7=0;if(s->input.touch==0&&TouchscreenHitbox_FindHitboxAtTouchNew(ov28_0225EA88)!=-1)s->input.touch=1;if(FieldSystem_TaskIsRunning(s->field)==1||TouchscreenHitbox_FindHitboxAtTouchHeld(ov28_0225EA88)==-1||s->input.touch==0){ov28_0225E31C((Work31 *)s,-1);ov28_0225E578(s,0);s->high=0;s->input.x=0;s->input.y=0;s->input.touch=0;StopSE(0x93d,0);}else if(s->input.touch==1){ov28_0225E7D4((DowsingMchn *)s);ov28_0225E1A4(s);ov28_0225E31C((Work31 *)s,1);ov28_0225E578(s,1);ov28_0225E730(s);}ov28_0225E3BC((To45Ov28Work *)s);ov28_0225E43C((Work43 *)s);ov28_0225E900((DowsingMchn *)s);ov28_0225E938((DowsingMchn *)s);ov28_0225E9E0((DowsingMchn *)s);SpriteList_RenderAndAnimateSprites(s->sprites);}
