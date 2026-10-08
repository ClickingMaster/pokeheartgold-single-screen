#pragma force_active on
#include "global.h"
typedef struct { u8 pad[0x30]; fx32 x; fx32 z; u8 pad38[0x7C - 0x38]; } Ent96;
typedef struct { Ent96 ents[3]; u8 pad[0x174 - 3 * 0x7C]; } Row96;
typedef struct { u8 pad[0x5C]; Row96 rows[4]; } Course96;
extern int ov96_021E6104(void);
extern BOOL ov96_021E872C(int x, int z, int a, int b, int limit, int *out);
int ov96_02213364(Course96 *c, u8 row, u8 a, u8 b);
// @fn ov96_02213364
int ov96_02213364(Course96 *c, u8 row, u8 a, u8 b){int vals[3]; u8 flags[3]; int out; int limit; int min; u8 i; u8 count; u8 best; limit = ov96_021E6104(); count = 0; min = limit << 12; for (i = 0; i < 3; i++) { Ent96 *e = &c->rows[row].ents[i]; if (ov96_021E872C(e->x / FX32_ONE, e->z / FX32_ONE, a, b, limit, &out)) { vals[i] = out; flags[i] = TRUE; count++; } else { vals[i] = 0; flags[i] = FALSE; } } if (count == 0) { return 12; } best = 3; for (i = 0; i < 3; i++) { if (flags[i] && vals[i] < min) { best = i; min = vals[i]; } } if (best == 3) { GF_AssertFail(); best = 12; } return best;}
