#include <nitro/code16.h>

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

typedef struct Ov14Context {
    u8 pad00[0x30];
    u32 state;
    u8 *work;
} Ov14Context;

typedef int (*Ov14Handler)(Ov14Context *);

typedef struct Ov14HandlerPair {
    Ov14Handler handler;
    Ov14Handler other;
} Ov14HandlerPair;

extern Ov14Handler sPCBoxStateTable[];
extern Ov14HandlerPair sPCBoxYesNoHandlers[];
extern Ov14HandlerPair ov14_021F7D78[];
extern u8 gSystem[];

int System_GetTouchNew(void);
void PlaySE(int seqNo);
int YesNoPrompt_HandleInput(void *prompt);
void YesNoPrompt_Reset(void *prompt);

int PCBox_State_Delay(Ov14Context *ctx);
int PCBox_State_WaitUpdateFunc(Ov14Context *ctx);
int PCBox_State_WaitButton(Ov14Context *ctx);
int PCBox_State_HandleYesNo(Ov14Context *ctx);
