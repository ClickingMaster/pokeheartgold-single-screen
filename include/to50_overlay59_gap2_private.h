#ifndef TO50_OV59_GAP2_PRIVATE_H
#define TO50_OV59_GAP2_PRIVATE_H
#include "global.h"
typedef struct Ov59R8Work {
    u8 raw[0x254];
    void *sprites[15];
} Ov59R8Work;
void ov59_02239D08(Ov59R8Work *, int);
int ApricornBox_CountQueuedApricorns(void *);
int sub_02032004(void *);
void BufferIntegerAsString(void *, int, int, int, int, int);
void BufferItemNamePlural(void *, int, int);
void BufferItemName(void *, int, int);
void ReadMsgDataIntoString(void *, int, void *);
void BufferString(void *, int, void *, int, int, int);
void StringExpandPlaceholders(void *, void *, void *);
#endif
