#ifndef R18_OVERLAY49_PRIVATE_H
#define R18_OVERLAY49_PRIVATE_H
#include "global.h"

typedef struct Config49 {
    u8 pad0[3];
    u8 index3;
    u8 value4;
    u8 value5;
    u8 timer6;
    u8 action7;
} Config49;

void WiFiPlazaPlayerState_SetStep(void *, u8);
BOOL WiFiPlazaRoom_IsMessageFinished(void *);
u32 WiFiPlazaPlayer_GetLanguage(void *);
void *WiFiPlazaRoom_GetMsgString(void *, u32, u32);
void WiFiPlazaRoom_PrintMessage(void *, void *);
void ov49_0225A3AC(void *, u32, void *);
void ov49_0225A3BC(void *, u32, void *);
void ov49_0225A3CC(void *, u32, void *);
void ov49_0225A3DC(void *, u32, void *);
void ov49_0225A3EC(void *, u32, void *);
void ov49_0225A3FC(void *, u32, void *);
void *WiFiPlazaWork_GetMyPlayer(void *);
void *WiFiPlazaWork_GetPlayer(void *, u8);
u32 WiFiPlazaPlayer_IsLanguageValid(void *);
void *ov45_0222AAC8(void *);
void ov49_0225A39C(void *, void *, void *);
void WiFiPlazaRoom_GetLobby(void *);
u32 ov45_0222B094(void);
void ov49_0225A0CC(void *);
void ov49_0225A334(void *, u8, u32);
void *ov49_02264C04(void *, u8, u32);
void ov49_0225A0BC(void *);
extern const u16 ov49_02269E44[][8];

void ov49_02264CFC(Config49 *, u8, u8, void *, u8);
void ov49_02264D14(Config49 *, void *);
void ov49_02264D30(Config49 *, void *, void *);
void ov49_02264D4C(void *, void *, void *, void *, void *, void *);
void ov49_02264D9C(void *, u32, void *, void *);
BOOL ov49_02264E20(Config49 *, void *, void *);
void ov49_02264E90(Config49 *, void *, void *, void *, u32);
void ov49_02264EC8(Config49 *, void *);

#endif
