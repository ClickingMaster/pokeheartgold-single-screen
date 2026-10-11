#ifndef TO57_NEW49_OVERLAY08_BANK00_INPUT_PRIVATE_H
#define TO57_NEW49_OVERLAY08_BANK00_INPUT_PRIVATE_H
#include "global.h"

#include "system.h"

typedef struct Ov08N49Root {
    void *unk00;
    u8 pad04[4];
    void *unk08;
    u8 pad0C[5];
    u8 battler;
    u8 pad12[0x10];
    u16 item;
    u8 pad24[4];
    void *unk28;
    u8 data2C[4];
    u8 pad30[4];
    u8 selected;
} Ov08N49Root;

typedef struct Ov08N49Work {
    Ov08N49Root *root;
    u8 pad0004[0x1FA8 - 4];
    void *msgData;
    u8 pad1FAC[4];
    void *string;
    u8 pad1FB4[0x2077 - 0x1FB4];
    u8 modeLow : 4;
    u8 modeHigh : 4;
    u8 pad2078;
    u8 state;
    u8 pad207A[2];
    u8 field207C;
    u8 pad207D[0x2088 - 0x207D];
    void *smallState;
    u8 field208C;
    u8 result;
    u8 field208E;
} Ov08N49Work;

extern const u8 ov08_02224F3C[];
extern const u8 ov08_02224E44[];
extern const u8 ov08_02224E94[];
int ov08_0221D5D0(void *, const void *);
int BattleDpadCursor_HandleInput(void *);
void ov08_022217C8(void *);
void ov08_022220AC(void *, int);
void PlaySE(int);
int ov08_0221DB54(void *);
void ov08_0221F220(void *);
void ov08_0221DB7C(void *);
void ov08_0221DBB4(void *);
int BattleSystem_RecoverStatus(void *, int, int, int, int);
void ReadMsgDataIntoString(void *, int, void *);
void ov08_022201C0(void *);

int ov08_0221C604(Ov08N49Work *);
int ov08_0221C6F8(Ov08N49Work *);
int ov08_0221C814(Ov08N49Work *);
#endif
