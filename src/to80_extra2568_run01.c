#pragma force_active on
#include "global.h"
#include "overlay_42.h"
typedef struct Ov49R13Interpolation {
    u32 raw[4];
} Ov49R13Interpolation;

typedef struct Ov49R13Position {
    u32 x;
    u32 z;
    u32 y;
} Ov49R13Position;

typedef struct Ov49R13Work {
    void *field;
    void *object;
    u16 state;
    u8 finished;
    u8 padding_0B;
    union {
        struct {
            s16 timer;
            s16 selected;
        };
        Ov49R13Interpolation interpolation;
    };
    u32 counter;
} Ov49R13Work;

typedef struct Ov49R14Anim {
    struct UnkStruct_ov42_02228110 *unk00;
    void *unk04;
    u16 unk08;
    u8 unk0A;
    u8 unk0B;
    u32 unk0C[4];
    s32 unk1C;
    s32 unk20;
} Ov49R14Anim;

typedef struct Ov49R14Spin {
    void *unk00;
    void *unk04;
    u16 unk08;
    u8 unk0A;
    u8 unk0B;
    s32 unk0C;
    VecFx32 unk10;
} Ov49R14Spin;

typedef struct { void *field; } Ov49InputWork;
typedef struct { void *unk0; void *obj; u16 state; u8 padA[2]; } Ov49R14Work;

typedef struct Ov49EntryR4 Ov49EntryR4;
typedef void (*Ov49EntryCallback)(void*,void*);
struct Ov49EntryR4 {
 union {u8 raw[36];struct {void *first,*second;u16 counter;u8 mode,state;u8 payload[24];} fields;Ov49R13Work transition;Ov49R14Anim animation;Ov49R14Spin spin;Ov49InputWork input;Ov49R14Work cleanup;} storage;
 union {Ov49EntryCallback callback;u32 bits;} tail;
};

typedef struct Ov49ManagerR4 {
    void *object0;
    void *object4;
    Ov49EntryR4 *entries;
    u16 count;
    u16 value;
    void *iterator;
    void *arg;
    union {u8 bytes[0x10];struct {u32 reserved[3];Ov49EntryR4 *special;} fields;} sub18;
} Ov49ManagerR4;
void ov49_022591D8(void*);
extern void ov49_02258EEC(void*,void*,int);
#include "to47_overlay49_grid_search_r3_private.h"
#include <string.h>
typedef struct Obj{u8 state:4;u8 type:2;u8 top:2;u8 unused[3];void *ctx,*handle;u8 model[0x80];u32 lightFlags;} Obj;
typedef struct Manager{void *resource2d,*animation2d,*resource3d,*billboard;u8 unused[8];u8 modelResource[0x10];u8 unused2[4];u32 lightFlags;} Manager;
typedef struct Ov49R10Config{u8 id,unk1;u16 unk2;u8 unk4,pad5[3];} Entry;
typedef struct{u16 y,x;}Location;
typedef struct{Location location;UnkStruct_ov42_02122667 create;}Frame;
extern const Entry ov49_02269678[4];
extern void *ov49_022593C0(void*);
extern Obj *ov45_0223070C(Manager*,void*);
#pragma force_active on
void ov49_022591D8(void*opaque){Ov49ManagerR4*w=opaque;const Entry*e=ov49_02269678;u32 i;u16 n;Frame f;Ov49EntryR4*o;UnkStruct_ov42_02228110 *context;Obj *graphics;
for(i=0;i<4;i++,e++){n=0;while(ov49_022589D8(w->arg,e->id,&f.location.x,&f.location.y,n)==1){o=ov49_022593C0(w);f.create.unk0=f.location.x<<4;f.create.unk2=f.location.y<<4;f.create.unk4=e->unk4;f.create.unk6=0;f.create.unk8=e->unk1;f.create.unkA=e->unk2;
context=ov42_022280B8(w->object0,&f.create);o->storage.fields.first=context;graphics=ov45_0223070C(w->object4,context);o->storage.fields.second=graphics;
ov49_02258EEC(w,o,0);if(e->id==97)w->sub18.fields.special=o;n++;}}}
