#ifndef TO90_LOGIN_CALLBACK_PRIVATE_H
#define TO90_LOGIN_CALLBACK_PRIVATE_H
#include "global.h"
typedef struct GPIConnection GPIConnection;
typedef GPIConnection *GPConnection;
typedef void (*GPCallback)(GPConnection *, void *, void *);
typedef void (*LoginCallback)(int, int, void *);
typedef union W41287Account {
    u8 bytes[64];
    struct { u8 prefix[0x1c]; u32 profile; u8 tail[0x20]; } fields;
} W41287Account;
#endif
