typedef struct {
    int count;
    u8 unk04[8];
    void *pic;
    u8 unk10[4];
    int index;
} PicList;
typedef struct {
    s16 x, y;
    u8 raw[0x2c];
} Motion30;
typedef struct {
    void *owner, *unk04;
    Motion30 motion;
} WorkF4;
typedef struct {
    int count, timer, limit;
    u8 cfg[8];
    void *pic;
    u8 pad18[8];
    u8 motion[0x1c];
    void *owner;
} WorkBlink;
typedef struct {
    s16 x, y;
    u8 raw[0x1c];
} Motion20;
typedef struct {
    int unk00;
    u8 pad04[4];
    int param;
    s16 delta;
    u8 pad0e[6];
    s16 x, y;
    void *pic;
    u8 pad1c[8];
    union {
        u8 cfg[0x1c];
        struct {
            u8 state;
        };
    };
    Motion20 motion;
    u8 pad60[4];
    void *owner;
} WorkSlideA;
typedef struct {
    int unk00;
    u8 pad04[4];
    int param;
    u8 pad0c[4];
    s16 x, y;
    void *pic;
    u8 pad18[8];
    union {
        u8 cfg[0x1c];
        struct {
            u8 state;
        };
    };
    Motion20 motion;
    u8 pad5c[4];
    void *owner;
    s16 delta;
    u8 pad66[2];
} WorkSlideB;
typedef struct {
    s16 x, y;
    u8 raw04[0x10];
    void *unk14;
    u8 raw18[0xc];
} Move24;
typedef struct {
    s16 x, y;
    u8 raw[8];
} Bounce0C;
typedef struct {
    u8 state, pad1;
    s16 baseY;
    int unk04;
    u8 pad8, timer, delay, padB;
    void *owner;
    void *pic;
    Move24 move;
    u8 pad38[0];
    Bounce0C bounce;
    u8 pad44[0x1c];
} WorkMoveA;
typedef struct {
    u8 state, pad1;
    s16 baseY;
    int unk04;
    u8 pad8, timer, delay, padB;
    int p0, p1, p2;
    void *p2x;
    void *owner;
    void *pic;
    Move24 move;
    Bounce0C bounce;
} WorkMoveB;
extern void *BattleAnimUtil_AllocWork(void *, int);
extern void *ov07_0221C514(void *);

extern u32 ov07_0221C4A8(void *p, int i);
extern void *ov07_0221C4E8(void *, int);
extern void ManagedSprite_SetOamMode(void *, int);
extern void BattleAnimUtil_SetBlendWithBattleBg(void *, int, int);
extern void ov07_02222AC4(void *, s16, s16, s16, s16, s16);
extern void *ov07_0221C410(void *, void (*)(void *, void *), void *);
extern void ov07_022247C8(void *, void *);
extern void BattleAnimUtil_CollectPicsByFlags(void *, int, void *, PicList *);
extern int ov07_0221FAC8(void *, int);
extern void Pokepic_SetAttr(void *, int, int);
extern int Pokepic_GetAttr(void *, int);
extern void Heap_Free(void *);
extern void ov07_0221C448(void *, void *);
extern void BattleAnimUtil_InitCommonWork(void *, void *);
extern int ov07_0221C468(void *);
extern int ov07_0221C470(void *);
extern void *ov07_0221FA48(void *, int);
extern int ov07_0221FAB0(void *);
extern int BattleAnimUtil_GetPartnerBattler(void *, int);
extern void GF_AssertFail(void);
extern void BattleAnimUtil_GetPicPosition(void *, void *);
extern int ov07_02222004(void *, int);
extern void ov07_02222268(void *, s16, s16, s16, s16, u16);
extern int ov07_022222B4(void *);
extern int ov07_02222558(void *);
extern void ov07_022227D8(void *);
extern void Pokepic_AddAttr(void *, int, int);
extern int ov07_02222674(int, int, void *);
extern void ov07_02222508(void *, s16, s16, s16, int);
extern int ov07_0221BFD0(void *);
extern void *Heap_Alloc(int, int);
extern int ov07_0221FAA0(void *, int);
extern void ov07_022227A8(void *, int, int, int, int);
extern int ov07_0222260C(void *);
extern void ov07_02222590(void *, s16, s16, s16, s16, int, int);
extern void ov07_02221F38(void *, int, int, s16 *);
extern void ov07_0221BFC0(void *);
void ov07_022247F4(void *);
void ov07_022248A0(void *);
void ov07_02224948(void *);
void ov07_02224A20(void *);
void ov07_02224BAC(void *);
void ov07_02224E58(void *);
void ov07_02225098(void *);
static void UpdateB(WorkMoveB *w) {
    Pokepic_SetAttr(w->pic, 12, w->move.x);
    Pokepic_SetAttr(w->pic, 13, w->move.y);
    Pokepic_SetAttr(w->pic, 1, w->baseY + ov07_02222674(w->baseY, w->unk04, w->p2x));
}
void ov07_022248F4(void *, WorkBlink *);
void ov07_02224998(void *, WorkSlideA *);
void ov07_02224B14(void *, WorkSlideB *);
void ov07_02224CB8(void *, WorkMoveA *);
void ov07_02224EF4(void *, WorkMoveB *);
