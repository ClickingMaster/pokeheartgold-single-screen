#ifndef OV70_WAVE42_PROMPT_PRIVATE_H
#define OV70_WAVE42_PROMPT_PRIVATE_H
#include "global.h"
#define PTR(p, o) (*(void **)((u8 *)(p) + (o)))
#define U16(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define U32(p, o) (*(u32 *)((u8 *)(p) + (o)))
void ov70_02244FA4(void *, void *, int, int, int);
void Gts_SetState(void *, int, int);
int Gts_HandleYesNoPrompt(void *);
void YesNoPrompt_Destroy(void *);
void Gts_CreateWaitingIcon(void *);
void sub_0203946C(void);
void Gts_SetNextScreen(void *, int, int);
int Handle2dMenuInput_DeleteOnFinish(void *, int);
int ov00_021EC5B4(void);
void ov00_021EC8D8(void);
int ov70_022446D0(void *);
int ov70_02244708(void *);
int ov70_02244774(void *);
int ov70_022447A0(void *);
int ov70_02244804(void *);
#endif
