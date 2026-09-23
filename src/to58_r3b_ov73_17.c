#include "global.h"

typedef struct {
    u32 words[4];
} Ov73CharTransferTemplate;

extern Ov73CharTransferTemplate ov73_021EA69C;
extern void ObjCharTransfer_Init(void *);
extern void ObjPlttTransfer_Init(int, int);
extern void ObjCharTransfer_ClearBuffers(void);
extern void ObjPlttTransfer_Reset(void);
void ov73_021E8168(void);

void ov73_021E8168(void) {
    Ov73CharTransferTemplate template = ov73_021EA69C;
    ObjCharTransfer_Init(&template);
    ObjPlttTransfer_Init(20, 150);
    ObjCharTransfer_ClearBuffers();
    ObjPlttTransfer_Reset();
}
