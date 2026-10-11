#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u32 a[6];} Setup;
extern const Setup ov99_021EA3AC;
extern void *PokeathlonUtil_CreateSpriteContext(void *,Setup *,int);
extern void ov99_021E8C88(void *),ov99_021E8D5C(void *),ov99_021E8D88(void *),ov99_021E8EBC(void *),ov99_021E8C50(void *);
void ov99_021E8C04(void *);
void ov99_021E8C04(void *state){Setup t=ov99_021EA3AC;*(void **)((u8 *)state+20)=PokeathlonUtil_CreateSpriteContext(*(void **)((u8 *)state+12),&t,37);ov99_021E8C88(state);ov99_021E8D5C(state);ov99_021E8D88(state);ov99_021E8EBC(state);ov99_021E8C50(state);}
