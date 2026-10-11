#ifndef TO52_R7_W02_PRIVATE_H
#define TO52_R7_W02_PRIVATE_H

#include "global.h"

#define U32_AT(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define PTR_AT(p, o) (*(void **)((u8 *)(p) + (o)))

void WirelessStatusIcon_Destroy(void);
void sub_02039418(void *);
void Sys_SetSleepDisableFlag(int);
void ov70_02244FA4(void *, void *, int, int, int);
void ov70_02238D84(void *, int, int);
void ov70_02245124(void *);
void ov00_021EC3F0(void *, int, int, int);
void ov00_021EC454(int);
void ov00_021EC4A4(void);
void ov00_021EC60C(void);
int ov00_021EC5B4(void);
int ov00_021EC724(void);
int ov00_021EC11C(u32 *, u32 *);
void ov00_021EC210(void);
void ov00_021EC8D8(void);
void ov70_02238F80(void *);
void ov00_021EC0FC(void *);
void ov00_021EC9E0(void *);

int ov70_02244854(void *);
void ov70_02244888(void *);
int ov70_022448C0(void *);

#endif
