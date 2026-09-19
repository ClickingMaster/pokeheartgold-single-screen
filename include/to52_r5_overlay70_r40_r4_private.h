#ifndef W20_R5_PRIVATE_H
#define W20_R5_PRIVATE_H
#include "global.h"

#include "gf_gfx_planes.h"
#include "palette.h"
#include "sys_flags.h"
#define U16(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define U32(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define PTR(p, o) (*(void **)((u8 *)(p) + (o)))
typedef struct PairW20 {
    s32 a, b;
} PairW20;
extern const s32 ov70_02245E84[];
extern const PairW20 ov70_02245F5C[];
extern const PairW20 ov70_02245F58[];
extern int (*ov70_02246780[])(void *);
int ov70_02243FD4(void *, u32);
void ov70_02244670(void *);
void ov70_022441A4(void *);
void ov70_022442B4(void *);
void ov70_0224458C(void *);
int ov00_021EC5B4(void);
void sub_02039418(void *);
void ov70_02244FA4(void *, void *, int, int, int);
void ov70_02238D84(void *, int, int);
void ov70_02238F64(void *);
void ov70_02245124(void *);
void ov70_02238E44(void *);
void sub_0203A930(void);

struct Ov70W20;
int ov70_02243FE0(void *, int);
int ov70_02244038(void *);
void ov70_02244124(struct Ov70W20 *);
void BeginNormalPaletteFade(u32, u32, u32, u16, u8, u8, u32);
#endif
