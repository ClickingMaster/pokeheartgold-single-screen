#ifndef W16_OV81_H
#define W16_OV81_H
#include "global.h"

#include "unk_02019BA4.h"
void ov81_02243220(void *);
int ov81_02242710(void *);
void ov81_022432DC(void *, int);
void ov81_022432AC(void *, int);
int ov81_02242704(void *);
extern const TouchscreenHitbox ov81_022436CC[];
extern const TouchscreenHitbox ov81_022436A8[];
extern const DpadMenuBox ov81_02243788[];
extern const DpadMenuBox ov81_022437C8[];
extern const GridCallbacks ov81_02243638;
void ov81_02243370(void *, void *);
GridInputHandler *ov81_02243398(void *, void *);
void ov81_022433FC(GridInputHandler *, int, void *);
#endif
