#pragma force_active on
#include "global.h"

#include "dwc/chat_internal.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on

extern void __msl_assertion_failed(const char *, const char *, const char *, int);
extern const char ov45_02255458[];
extern const char ov45_02255468[];
extern const char ov45_02255478[];
extern const char ov45_02255488[];
extern const char ov45_022554A4[];
extern const char ov45_022554BC[];
extern const char ov45_022554DC[];
extern const char ov45_022553C8[];
extern const char ov45_022553D8[];
extern const char ov45_022553E8[];
extern const char ov45_022553F8[];
extern const char ov45_0225540C[];
extern const char ov45_02255420[];
extern const char ov45_022554EC[];
extern const char ov45_022554F0[];
extern const char ov45_02255500[];
extern const char ov45_0225550C[];
extern const char ov45_02255520[];
extern const char ov45_02255544[];
extern const char ov45_02255558[];
extern const char ov45_02255570[];
extern const char ov45_02255588[];
extern const char ov45_02255590[];
extern const char ov45_022555B0[];
extern const char ov45_022555CC[];
extern const char ov45_022555E8[];
extern const char ov45_02255604[];
extern const char ov45_02255620[];
extern const char ov45_02255644[];

// Assertion messages of the callback checks, with the name of the checking function and the line.
#define ASSERT_CHAT(chat, func, line)                                     \
    if (!(chat)) {                                                        \
        __msl_assertion_failed(ov45_022554F0, ov45_02255468, func, line); \
    }
#define ASSERT_CALLBACK(callback, func, line)                             \
    if (!(callback)) {                                                    \
        __msl_assertion_failed(ov45_02255458, ov45_02255468, func, line); \
    }                                                                     \
    if ((callback)->type < 0) {                                           \
        __msl_assertion_failed(ov45_02255478, ov45_02255468, func, line); \
    }                                                                     \
    if ((callback)->type >= 32) {                                         \
        __msl_assertion_failed(ov45_02255488, ov45_02255468, func, line); \
    }                                                                     \
    if (!(callback)->callback) {                                          \
        __msl_assertion_failed(ov45_022554A4, ov45_02255468, func, line); \
    }                                                                     \
    if (!(callback)->param) {                                             \
        __msl_assertion_failed(ov45_022554BC, ov45_02255468, func, line); \
    }                                                                     \
    if ((callback)->id < 0) {                                             \
        __msl_assertion_failed(ov45_022554DC, ov45_02255468, func, line); \
    }

void ov45_02244500(ChatCallback *callback) {
    ASSERT_CALLBACK(callback, ov45_022553F8, 171);
    switch (callback->type) {
    case 0: {
        u32 *p = callback->param;
        ov00_021EC3D8((void *)p[0]);
        break;
    }
    case 1: {
        u32 *p = callback->param;
        ov00_021EC3D8((void *)p[0]);
        break;
    }
    case 2: {
        u32 *p = callback->param;
        ov00_021EC3D8((void *)p[0]);
        ov00_021EC3D8((void *)p[1]);
        break;
    }
    case 3: {
        u32 *p = callback->param;
        ov00_021EC3D8((void *)p[0]);
        ov00_021EC3D8((void *)p[1]);
        break;
    }
    case 4: {
        u32 *p = callback->param;
        ov00_021EC3D8((void *)p[0]);
        ov00_021EC3D8((void *)p[1]);
        ov00_021EC3D8((void *)p[2]);
        break;
    }
    case 5: {
        u32 *p = callback->param;
        ov00_021EC3D8((void *)p[0]);
        ov00_021EC3D8((void *)p[1]);
        ov00_021EC3D8((void *)p[2]);
        break;
    }
    case 6: {
        u32 *p = callback->param;
        ov00_021EC3D8((void *)p[0]);
        ov00_021EC3D8((void *)p[1]);
        break;
    }
    case 7: {
        u32 *p = callback->param;
        ov00_021EC3D8((void *)p[0]);
        ov00_021EC3D8((void *)p[1]);
        ov00_021EC3D8((void *)p[3]);
        ov00_021EC3D8((void *)p[4]);
        break;
    }
    case 8: {
        u32 *p = callback->param;
        ov00_021EC3D8((void *)p[0]);
        ov00_021EC3D8((void *)p[1]);
        ov00_021EC3D8((void *)p[2]);
        break;
    }
    case 9: {
        u32 *p = callback->param;
        ov00_021EC3D8((void *)p[0]);
        ov00_021EC3D8((void *)p[1]);
        break;
    }
    case 10: {
        u32 *p = callback->param;
        ov00_021EC3D8((void *)p[0]);
        ov00_021EC3D8((void *)p[1]);
        break;
    }
    case 11: {
        u32 *p = callback->param;
        ov00_021EC3D8((void *)p[0]);
        ov00_021EC3D8((void *)p[1]);
        break;
    }
    case 12: {
        u32 *p = callback->param;
        ov00_021EC3D8((void *)p[0]);
        break;
    }
    case 13: {
        u32 *p = callback->param;
        ov00_021EC3D8((void *)p[2]);
        ov00_021EC3D8((void *)p[3]);
        break;
    }
    case 14: {
        int i;
        u32 *p = callback->param;
        for (i = 0; i < (int)p[1]; i++) {
            ov00_021EC3D8(((void **)p[2])[i]);
            ov00_021EC3D8(((void **)p[3])[i]);
        }
        ov00_021EC3D8((void *)p[2]);
        ov00_021EC3D8((void *)p[3]);
        ov00_021EC3D8((void *)p[4]);
        break;
    }
    case 15: {
        u32 *p = callback->param;
        ov00_021EC3D8((void *)p[2]);
        break;
    }
    case 16: {
        u32 *p = callback->param;
        ov00_021EC3D8((void *)p[1]);
        ov00_021EC3D8((void *)p[2]);
        break;
    }
    case 17: {
        u32 *p = callback->param;
        ov00_021EC3D8((void *)p[1]);
        ov00_021EC3D8((void *)p[2]);
        break;
    }
    case 18: {
        u32 *p = callback->param;
        ov00_021EC3D8((void *)p[1]);
        ov00_021EC3D8((void *)p[3]);
        break;
    }
    case 19: {
        int i;
        u32 *p = callback->param;
        ov00_021EC3D8((void *)p[1]);
        for (i = 0; i < (int)p[2]; i++) {
            ov00_021EC3D8(((void **)p[3])[i]);
        }
        ov00_021EC3D8((void *)p[3]);
        ov00_021EC3D8((void *)p[4]);
        break;
    }
    case 20: {
        int i;
        u32 *p = callback->param;
        ov00_021EC3D8((void *)p[1]);
        ov00_021EC3D8((void *)p[2]);
        ov00_021EC3D8((void *)p[3]);
        ov00_021EC3D8((void *)p[4]);
        for (i = 0; i < (int)p[5]; i++) {
            ov00_021EC3D8(((void **)p[6])[i]);
        }
        ov00_021EC3D8((void *)p[6]);
        break;
    }
    case 21: {
        u32 *p = callback->param;
        ov00_021EC3D8((void *)p[1]);
        ov00_021EC3D8((void *)p[2]);
        ov00_021EC3D8((void *)p[3]);
        break;
    }
    case 22: {
        u32 *p = callback->param;
        ov00_021EC3D8((void *)p[1]);
        ov00_021EC3D8((void *)p[2]);
        ov00_021EC3D8((void *)p[3]);
        ov00_021EC3D8((void *)p[4]);
        break;
    }
    case 23: {
        u32 *p = callback->param;
        ov00_021EC3D8((void *)p[1]);
        ov00_021EC3D8((void *)p[2]);
        break;
    }
    case 24: {
        int i;
        u32 *p = callback->param;
        ov00_021EC3D8((void *)p[1]);
        for (i = 0; i < (int)p[2]; i++) {
            ov00_021EC3D8(((void **)p[3])[i]);
        }
        ov00_021EC3D8((void *)p[3]);
        break;
    }
    case 25: {
        int i;
        u32 *p = callback->param;
        ov00_021EC3D8((void *)p[1]);
        for (i = 0; i < (int)p[2]; i++) {
            ov00_021EC3D8(((void **)p[3])[i]);
        }
        ov00_021EC3D8((void *)p[3]);
        break;
    }
    case 26: {
        u32 *p = callback->param;
        ov00_021EC3D8((void *)p[1]);
        ov00_021EC3D8((void *)p[2]);
        break;
    }
    case 27: {
        int i;
        u32 *p = callback->param;
        ov00_021EC3D8((void *)p[0]);
        for (i = 0; i < (int)p[1]; i++) {
            ov00_021EC3D8(((void **)p[2])[i]);
        }
        ov00_021EC3D8((void *)p[2]);
        ov00_021EC3D8((void *)p[3]);
        break;
    }
    case 28: {
        u32 *p = callback->param;
        ov00_021EC3D8((void *)p[0]);
        ov00_021EC3D8((void *)p[1]);
        ov00_021EC3D8((void *)p[2]);
        ov00_021EC3D8((void *)p[3]);
        break;
    }
    case 29: {
        int i;
        u32 *p = callback->param;
        ov00_021EC3D8((void *)p[1]);
        for (i = 0; i < (int)p[2]; i++) {
            ov00_021EC3D8(((void **)p[3])[i]);
            if (p[4]) {
                ov00_021EC3D8(((void **)p[4])[i]);
            }
        }
        ov00_021EC3D8((void *)p[3]);
        ov00_021EC3D8((void *)p[4]);
        break;
    }
    case 30: {
        int i;
        u32 *p = callback->param;
        ov00_021EC3D8((void *)p[1]);
        ov00_021EC3D8((void *)p[2]);
        for (i = 0; i < (int)p[3]; i++) {
            ov00_021EC3D8(((void **)p[4])[i]);
            if (p[5]) {
                ov00_021EC3D8(((void **)p[5])[i]);
            }
        }
        ov00_021EC3D8((void *)p[4]);
        ov00_021EC3D8((void *)p[5]);
        break;
    }
    case 31: {
        u32 *p = callback->param;
        ov00_021EC3D8((void *)p[1]);
        break;
    }
    default:
        __msl_assertion_failed(ov45_022554EC, ov45_02255468, ov45_022553F8, 485);
        break;
    }
    ov00_021EC3D8(callback->param);
}

ChatResult ov45_02244B9C(ChatConnection *chat) {
    chat->callbacks = ov00_021FE5B4(sizeof(ChatCallback), 0x80, ov45_022444C8);
    return chat->callbacks == NULL ? CHAT_FAILURE : CHAT_SUCCESS;
}

void ov45_02244BCC(ChatConnection *chat) {
    ChatCallback *callback;
    int n, i;
    ASSERT_CHAT(chat, ov45_0225540C, 506);
    if (!chat->callbacks) {
        return;
    }
    n = ArrayLength(chat->callbacks);
    for (i = 0; i < n; i++) {
        callback = ov00_021FE6EC(chat->callbacks, i);
        ASSERT_CALLBACK(callback, ov45_0225540C, 525);
        ov45_02244500(callback);
    }
    ov00_021FE678(chat->callbacks);
}

#define ASSERT(test, string, line) \
    if (!(test))                   \
    __msl_assertion_failed(string, ov45_02255468, ov45_022553C8, line)
#define FAIL()                       \
    {                                \
        ov00_021EC3D8(dst);          \
        ov00_021EC3D8(item.channel); \
        return FALSE;                \
    }
// Copies the string argument i into the stored parameters.
#define COPY_STRING(i)                               \
    if (src[i]) {                                    \
        dst[i] = (u32)ov00_021FF3D4((char *)src[i]); \
        if (!dst[i]) {                               \
            FAIL();                                  \
        }                                            \
    }

BOOL ov45_02244D30(ChatConnection *chat, int type, void *func, u32 *src, void *user, int id, char *channel, int size) {
    ChatCallback item;
    ASSERT(chat, ov45_022554F0, 0x21f);
    ASSERT(type >= 0, ov45_02255500, 0x221);
    ASSERT(type < 32, ov45_0225550C, 0x222);
    ASSERT(chat->callbacks, ov45_02255520, 0x223);
    ASSERT(func, ov45_02255544, 0x224);
    ASSERT(src, ov45_02255558, 0x225);
    ASSERT(size, ov45_02255570, 0x226);
    ASSERT(id >= 0, ov45_02255588, 0x227);
    {
        u32 *zero = (u32 *)&item;
        zero[0] = 0;
        zero[1] = 0;
        zero[2] = 0;
        zero[3] = 0;
        zero[4] = 0;
        zero[5] = 0;
    }
    item.type = type;
    item.callback = func;
    item.param = ov00_021EC3A8(size);
    if (!item.param) {
        return FALSE;
    }
    memcpy(item.param, src, size);
    item.user = user;
    item.id = id;
    if (!channel) {
        item.channel = NULL;
    } else {
        int length = strlen(channel);
        item.channel = ov00_021EC3A8(length + 1);
        if (!item.channel) {
            ov00_021EC3D8(item.param);
            return FALSE;
        }
        memcpy(item.channel, channel, length + 1);
    }
    switch (item.type) {
    case 0: {
        u32 *dst = item.param;
        COPY_STRING(0);
        break;
    }
    case 1: {
        u32 *dst = item.param;
        COPY_STRING(0);
        break;
    }
    case 2: {
        u32 *dst = item.param;
        COPY_STRING(0);
        COPY_STRING(1);
        break;
    }
    case 3: {
        u32 *dst = item.param;
        COPY_STRING(0);
        COPY_STRING(1);
        break;
    }
    case 4: {
        u32 *dst = item.param;
        COPY_STRING(0);
        COPY_STRING(1);
        COPY_STRING(2);
        break;
    }
    case 5: {
        u32 *dst = item.param;
        COPY_STRING(0);
        COPY_STRING(1);
        COPY_STRING(2);
        break;
    }
    case 6: {
        u32 *dst = item.param;
        COPY_STRING(0);
        COPY_STRING(1);
        break;
    }
    case 7: {
        u32 *dst = item.param;
        COPY_STRING(0);
        COPY_STRING(1);
        COPY_STRING(3);
        COPY_STRING(4);
        break;
    }
    case 8: {
        u32 *dst = item.param;
        COPY_STRING(0);
        COPY_STRING(1);
        COPY_STRING(2);
        break;
    }
    case 9: {
        u32 *dst = item.param;
        COPY_STRING(0);
        COPY_STRING(1);
        break;
    }
    case 10: {
        u32 *dst = item.param;
        COPY_STRING(0);
        if (src[1]) {
            dst[1] = (u32)ov00_021EC3A8(32);
            if (!dst[1]) {
                FAIL();
            }
            memcpy((void *)dst[1], (void *)src[1], 32);
        }
        break;
    }
    case 11: {
        u32 *dst = item.param;
        COPY_STRING(0);
        COPY_STRING(1);
        break;
    }
    case 12: {
        u32 *dst = item.param;
        COPY_STRING(0);
        break;
    }
    case 13: {
        u32 *dst = item.param;
        COPY_STRING(2);
        COPY_STRING(3);
        break;
    }
    case 14: {
        int i;
        u32 *dst = item.param;
        ASSERT((int)src[1] >= 0, ov45_02255590, 0x2d3);
        if (!src[2]) {
            dst[2] = 0;
        } else {
            dst[2] = (u32)ov00_021EC3A8(src[1] * 4);
            if (!dst[2]) {
                FAIL();
            }
            for (i = 0; i < (int)src[1]; i++) {
                if (!((u32 *)src[2])[i]) {
                    ((u32 *)dst[2])[i] = 0;
                } else {
                    ((u32 *)dst[2])[i] = (u32)ov00_021FF3D4((char *)((u32 *)src[2])[i]);
                    if (!((u32 *)dst[2])[i]) {
                        for (i--; i >= 0; i--) {
                            ov00_021EC3D8((void *)((u32 *)dst[2])[i]);
                        }
                        ov00_021EC3D8((void *)dst[2]);
                        FAIL();
                    }
                }
            }
        }
        ASSERT((int)src[1] >= 0, ov45_02255590, 0x2d4);
        if (!src[3]) {
            dst[3] = 0;
        } else {
            dst[3] = (u32)ov00_021EC3A8(src[1] * 4);
            if (!dst[3]) {
                FAIL();
            }
            for (i = 0; i < (int)src[1]; i++) {
                if (!((u32 *)src[3])[i]) {
                    ((u32 *)dst[3])[i] = 0;
                } else {
                    ((u32 *)dst[3])[i] = (u32)ov00_021FF3D4((char *)((u32 *)src[3])[i]);
                    if (!((u32 *)dst[3])[i]) {
                        for (i--; i >= 0; i--) {
                            ov00_021EC3D8((void *)((u32 *)dst[3])[i]);
                        }
                        ov00_021EC3D8((void *)dst[3]);
                        FAIL();
                    }
                }
            }
        }
        ASSERT((int)src[1] >= 0, ov45_02255590, 0x2d5);
        if ((int)src[1] > 0) {
            int bytes;
            ASSERT(src[4], ov45_022555B0, 0x2d5);
            bytes = src[1] * 4;
            dst[4] = (u32)ov00_021EC3A8(bytes);
            if (!dst[4]) {
                ov00_021EC3D8(dst);
                ov00_021EC3D8(item.channel);
                return FALSE;
            }
            memcpy((void *)dst[4], (void *)src[4], bytes);
        }
        break;
    }
    case 15: {
        u32 *dst = item.param;
        COPY_STRING(2);
        break;
    }
    case 16: {
        u32 *dst = item.param;
        COPY_STRING(1);
        COPY_STRING(2);
        break;
    }
    case 17: {
        u32 *dst = item.param;
        COPY_STRING(1);
        if (src[2]) {
            dst[2] = (u32)ov00_021EC3A8(32);
            if (!dst[2]) {
                FAIL();
            }
            memcpy((void *)dst[2], (void *)src[2], 32);
        }
        break;
    }
    case 18: {
        u32 *dst = item.param;
        COPY_STRING(1);
        COPY_STRING(3);
        break;
    }
    case 19: {
        u32 *dst = item.param;
        COPY_STRING(1);
        ASSERT((int)src[2] >= 0, ov45_022555CC, 0x301);
        if (!src[3]) {
            dst[3] = 0;
        } else {
            int i;
            dst[3] = (u32)ov00_021EC3A8(src[2] * 4);
            if (!dst[3]) {
                FAIL();
            }
            for (i = 0; i < (int)src[2]; i++) {
                if (!((u32 *)src[3])[i]) {
                    ((u32 *)dst[3])[i] = 0;
                } else {
                    ((u32 *)dst[3])[i] = (u32)ov00_021FF3D4((char *)((u32 *)src[3])[i]);
                    if (!((u32 *)dst[3])[i]) {
                        for (i--; i >= 0; i--) {
                            ov00_021EC3D8((void *)((u32 *)dst[3])[i]);
                        }
                        ov00_021EC3D8((void *)dst[3]);
                        FAIL();
                    }
                }
            }
        }
        ASSERT((int)src[2] >= 0, ov45_022555CC, 0x302);
        if ((int)src[2] > 0) {
            int bytes;
            ASSERT(src[4], ov45_022555E8, 0x302);
            bytes = src[2] * 4;
            dst[4] = (u32)ov00_021EC3A8(bytes);
            if (!dst[4]) {
                FAIL();
            }
            memcpy((void *)dst[4], (void *)src[4], bytes);
        }
        break;
    }
    case 20: {
        u32 *dst = item.param;
        COPY_STRING(1);
        COPY_STRING(2);
        COPY_STRING(3);
        COPY_STRING(4);
        ASSERT((int)src[5] >= 0, ov45_02255590, 0x30e);
        if (!src[6]) {
            dst[6] = 0;
        } else {
            int i;
            dst[6] = (u32)ov00_021EC3A8(src[5] * 4);
            if (!dst[6]) {
                FAIL();
            }
            for (i = 0; i < (int)src[5]; i++) {
                if (!((u32 *)src[6])[i]) {
                    ((u32 *)dst[6])[i] = 0;
                } else {
                    ((u32 *)dst[6])[i] = (u32)ov00_021FF3D4((char *)((u32 *)src[6])[i]);
                    if (!((u32 *)dst[6])[i]) {
                        for (i--; i >= 0; i--) {
                            ov00_021EC3D8((void *)((u32 *)dst[6])[i]);
                        }
                        ov00_021EC3D8((void *)dst[6]);
                        FAIL();
                    }
                }
            }
        }
        break;
    }
    case 21: {
        u32 *dst = item.param;
        COPY_STRING(1);
        COPY_STRING(2);
        COPY_STRING(3);
        break;
    }
    case 22: {
        u32 *dst = item.param;
        COPY_STRING(1);
        COPY_STRING(2);
        COPY_STRING(3);
        COPY_STRING(4);
        break;
    }
    case 23: {
        u32 *dst = item.param;
        COPY_STRING(1);
        COPY_STRING(2);
        break;
    }
    case 24: {
        u32 *dst = item.param;
        COPY_STRING(1);
        ASSERT((int)src[2] >= 0, ov45_02255604, 0x335);
        if (!src[3]) {
            dst[3] = 0;
        } else {
            int i;
            dst[3] = (u32)ov00_021EC3A8(src[2] * 4);
            if (!dst[3]) {
                FAIL();
            }
            for (i = 0; i < (int)src[2]; i++) {
                if (!((u32 *)src[3])[i]) {
                    ((u32 *)dst[3])[i] = 0;
                } else {
                    ((u32 *)dst[3])[i] = (u32)ov00_021FF3D4((char *)((u32 *)src[3])[i]);
                    if (!((u32 *)dst[3])[i]) {
                        for (i--; i >= 0; i--) {
                            ov00_021EC3D8((void *)((u32 *)dst[3])[i]);
                        }
                        ov00_021EC3D8((void *)dst[3]);
                        FAIL();
                    }
                }
            }
        }
        break;
    }
    case 25: {
        u32 *dst = item.param;
        COPY_STRING(1);
        ASSERT((int)src[2] >= 0, ov45_02255620, 0x33e);
        if (!src[3]) {
            dst[3] = 0;
        } else {
            int i;
            dst[3] = (u32)ov00_021EC3A8(src[2] * 4);
            if (!dst[3]) {
                FAIL();
            }
            for (i = 0; i < (int)src[2]; i++) {
                if (!((u32 *)src[3])[i]) {
                    ((u32 *)dst[3])[i] = 0;
                } else {
                    ((u32 *)dst[3])[i] = (u32)ov00_021FF3D4((char *)((u32 *)src[3])[i]);
                    if (!((u32 *)dst[3])[i]) {
                        for (i--; i >= 0; i--) {
                            ov00_021EC3D8((void *)((u32 *)dst[3])[i]);
                        }
                        ov00_021EC3D8((void *)dst[3]);
                        FAIL();
                    }
                }
            }
        }
        break;
    }
    case 26: {
        u32 *dst = item.param;
        COPY_STRING(1);
        COPY_STRING(2);
        break;
    }
    case 27: {
        u32 *dst = item.param;
        COPY_STRING(0);
        ASSERT((int)src[1] >= 0, ov45_022555CC, 0x350);
        if (!src[2]) {
            dst[2] = 0;
        } else {
            int i;
            dst[2] = (u32)ov00_021EC3A8(src[1] * 4);
            if (!dst[2]) {
                FAIL();
            }
            for (i = 0; i < (int)src[1]; i++) {
                if (!((u32 *)src[2])[i]) {
                    ((u32 *)dst[2])[i] = 0;
                } else {
                    ((u32 *)dst[2])[i] = (u32)ov00_021FF3D4((char *)((u32 *)src[2])[i]);
                    if (!((u32 *)dst[2])[i]) {
                        for (i--; i >= 0; i--) {
                            ov00_021EC3D8((void *)((u32 *)dst[2])[i]);
                        }
                        ov00_021EC3D8((void *)dst[2]);
                        FAIL();
                    }
                }
            }
        }
        ASSERT((int)src[1] >= 0, ov45_022555CC, 0x351);
        if ((int)src[1] > 0) {
            int bytes;
            ASSERT(src[3], ov45_022555E8, 0x351);
            bytes = src[1] * 4;
            dst[3] = (u32)ov00_021EC3A8(bytes);
            if (!dst[3]) {
                FAIL();
            }
            memcpy((void *)dst[3], (void *)src[3], bytes);
        }
        break;
    }
    case 28: {
        u32 *dst = item.param;
        COPY_STRING(0);
        COPY_STRING(1);
        COPY_STRING(2);
        COPY_STRING(3);
        break;
    }
    case 29: {
        u32 *dst = item.param;
        COPY_STRING(1);
        ASSERT((int)src[2] >= 0, ov45_02255644, 0x365);
        if (!src[3]) {
            dst[3] = 0;
        } else {
            int i;
            dst[3] = (u32)ov00_021EC3A8(src[2] * 4);
            if (!dst[3]) {
                FAIL();
            }
            for (i = 0; i < (int)src[2]; i++) {
                if (!((u32 *)src[3])[i]) {
                    ((u32 *)dst[3])[i] = 0;
                } else {
                    ((u32 *)dst[3])[i] = (u32)ov00_021FF3D4((char *)((u32 *)src[3])[i]);
                    if (!((u32 *)dst[3])[i]) {
                        for (i--; i >= 0; i--) {
                            ov00_021EC3D8((void *)((u32 *)dst[3])[i]);
                        }
                        ov00_021EC3D8((void *)dst[3]);
                        FAIL();
                    }
                }
            }
        }
        ASSERT((int)src[2] >= 0, ov45_02255644, 0x366);
        if (!src[4]) {
            dst[4] = 0;
        } else {
            int i;
            dst[4] = (u32)ov00_021EC3A8(src[2] * 4);
            if (!dst[4]) {
                FAIL();
            }
            for (i = 0; i < (int)src[2]; i++) {
                if (!((u32 *)src[4])[i]) {
                    ((u32 *)dst[4])[i] = 0;
                } else {
                    ((u32 *)dst[4])[i] = (u32)ov00_021FF3D4((char *)((u32 *)src[4])[i]);
                    if (!((u32 *)dst[4])[i]) {
                        for (i--; i >= 0; i--) {
                            ov00_021EC3D8((void *)((u32 *)dst[4])[i]);
                        }
                        ov00_021EC3D8((void *)dst[4]);
                        FAIL();
                    }
                }
            }
        }
        break;
    }
    case 30: {
        u32 *dst = item.param;
        COPY_STRING(1);
        COPY_STRING(2);
        ASSERT((int)src[3] >= 0, ov45_02255644, 0x370);
        if (!src[4]) {
            dst[4] = 0;
        } else {
            int i;
            dst[4] = (u32)ov00_021EC3A8(src[3] * 4);
            if (!dst[4]) {
                FAIL();
            }
            for (i = 0; i < (int)src[3]; i++) {
                if (!((u32 *)src[4])[i]) {
                    ((u32 *)dst[4])[i] = 0;
                } else {
                    ((u32 *)dst[4])[i] = (u32)ov00_021FF3D4((char *)((u32 *)src[4])[i]);
                    if (!((u32 *)dst[4])[i]) {
                        for (i--; i >= 0; i--) {
                            ov00_021EC3D8((void *)((u32 *)dst[4])[i]);
                        }
                        ov00_021EC3D8((void *)dst[4]);
                        FAIL();
                    }
                }
            }
        }
        ASSERT((int)src[3] >= 0, ov45_02255644, 0x371);
        if (!src[5]) {
            dst[5] = 0;
        } else {
            int i;
            dst[5] = (u32)ov00_021EC3A8(src[3] * 4);
            if (!dst[5]) {
                FAIL();
            }
            for (i = 0; i < (int)src[3]; i++) {
                if (!((u32 *)src[5])[i]) {
                    ((u32 *)dst[5])[i] = 0;
                } else {
                    ((u32 *)dst[5])[i] = (u32)ov00_021FF3D4((char *)((u32 *)src[5])[i]);
                    if (!((u32 *)dst[5])[i]) {
                        for (i--; i >= 0; i--) {
                            ov00_021EC3D8((void *)((u32 *)dst[5])[i]);
                        }
                        ov00_021EC3D8((void *)dst[5]);
                        FAIL();
                    }
                }
            }
        }
        break;
    }
    case 31: {
        u32 *dst = item.param;
        COPY_STRING(1);
        break;
    }
    default:
        __msl_assertion_failed(ov45_022554EC, ov45_02255468, ov45_022553C8, 0x380);
        break;
    }
    ov00_021FE754(chat->callbacks, &item);
    return TRUE;
}

#undef COPY_STRING
#undef FAIL
#undef ASSERT

void ov45_02246BA4(ChatConnection *chat, ChatCallback *callback) {
    void *user;
    ASSERT_CHAT(chat, ov45_022553D8, 909);
    ASSERT_CALLBACK(callback, ov45_022553D8, 911);
    user = callback->user;
    switch (callback->type) {
    case 0: {
        u32 *args = callback->param;
        ChatCallbackFunc1 func = (ChatCallbackFunc1)callback->callback;
        func(chat, args[0], user);
        break;
    }
    case 1: {
        u32 *args = callback->param;
        ChatCallbackFunc1 func = (ChatCallbackFunc1)callback->callback;
        func(chat, args[0], user);
        break;
    }
    case 2: {
        u32 *args = callback->param;
        ChatCallbackFunc3 func = (ChatCallbackFunc3)callback->callback;
        func(chat, args[0], args[1], args[2], user);
        break;
    }
    case 3: {
        u32 *args = callback->param;
        ChatCallbackFunc2 func = (ChatCallbackFunc2)callback->callback;
        func(chat, args[0], args[1], user);
        break;
    }
    case 4: {
        u32 *args = callback->param;
        ChatCallbackFunc4 func = (ChatCallbackFunc4)callback->callback;
        func(chat, args[0], args[1], args[2], args[3], user);
        break;
    }
    case 5: {
        u32 *args = callback->param;
        ChatCallbackFunc3 func = (ChatCallbackFunc3)callback->callback;
        func(chat, args[0], args[1], args[2], user);
        break;
    }
    case 6: {
        u32 *args = callback->param;
        ChatCallbackFunc3 func = (ChatCallbackFunc3)callback->callback;
        func(chat, args[0], args[1], args[2], user);
        break;
    }
    case 7: {
        u32 *args = callback->param;
        ChatCallbackFunc5 func = (ChatCallbackFunc5)callback->callback;
        func(chat, args[0], args[1], args[2], args[3], args[4], user);
        break;
    }
    case 8: {
        u32 *args = callback->param;
        ChatCallbackFunc3 func = (ChatCallbackFunc3)callback->callback;
        func(chat, args[0], args[1], args[2], user);
        break;
    }
    case 9: {
        u32 *args = callback->param;
        ChatCallbackFunc2 func = (ChatCallbackFunc2)callback->callback;
        func(chat, args[0], args[1], user);
        break;
    }
    case 10: {
        u32 *args = callback->param;
        ChatCallbackFunc2 func = (ChatCallbackFunc2)callback->callback;
        func(chat, args[0], args[1], user);
        break;
    }
    case 11: {
        u32 *args = callback->param;
        ChatCallbackFunc3 func = (ChatCallbackFunc3)callback->callback;
        func(chat, args[0], args[1], args[2], user);
        break;
    }
    case 12: {
        u32 *args = callback->param;
        ChatCallbackFunc1 func = (ChatCallbackFunc1)callback->callback;
        func(chat, args[0], user);
        break;
    }
    case 13: {
        u32 *args = callback->param;
        ChatCallbackFunc5 func = (ChatCallbackFunc5)callback->callback;
        func(chat, args[0], args[1], args[2], args[3], args[4], user);
        break;
    }
    case 14: {
        u32 *args = callback->param;
        ChatCallbackFunc5 func = (ChatCallbackFunc5)callback->callback;
        func(chat, args[0], args[1], args[2], args[3], args[4], user);
        break;
    }
    case 15: {
        u32 *args = callback->param;
        ChatCallbackFunc3 func = (ChatCallbackFunc3)callback->callback;
        ov45_02248558(chat, (const char *)args[2]);
        func(chat, args[0], args[1], args[2], user);
        break;
    }
    case 16: {
        u32 *args = callback->param;
        ChatCallbackFunc3 func = (ChatCallbackFunc3)callback->callback;
        func(chat, args[0], args[1], args[2], user);
        break;
    }
    case 17: {
        u32 *args = callback->param;
        ChatCallbackFunc3 func = (ChatCallbackFunc3)callback->callback;
        func(chat, args[0], args[1], args[2], user);
        break;
    }
    case 18: {
        u32 *args = callback->param;
        ChatCallbackFunc4 func = (ChatCallbackFunc4)callback->callback;
        func(chat, args[0], args[1], args[2], args[3], user);
        break;
    }
    case 19: {
        u32 *args = callback->param;
        ChatCallbackFunc5 func = (ChatCallbackFunc5)callback->callback;
        func(chat, args[0], args[1], args[2], args[3], args[4], user);
        break;
    }
    case 20: {
        u32 *args = callback->param;
        ChatCallbackFunc7 func = (ChatCallbackFunc7)callback->callback;
        func(chat, args[0], args[1], args[2], args[3], args[4], args[5], args[6], user);
        break;
    }
    case 21: {
        u32 *args = callback->param;
        ChatCallbackFunc4 func = (ChatCallbackFunc4)callback->callback;
        func(chat, args[0], args[1], args[2], args[3], user);
        break;
    }
    case 22: {
        u32 *args = callback->param;
        ChatCallbackFunc5 func = (ChatCallbackFunc5)callback->callback;
        func(chat, args[0], args[1], args[2], args[3], args[4], user);
        break;
    }
    case 23: {
        u32 *args = callback->param;
        ChatCallbackFunc4 func = (ChatCallbackFunc4)callback->callback;
        func(chat, args[0], args[1], args[2], args[3], user);
        break;
    }
    case 24: {
        u32 *args = callback->param;
        ChatCallbackFunc4 func = (ChatCallbackFunc4)callback->callback;
        func(chat, args[0], args[1], args[2], args[3], user);
        break;
    }
    case 25: {
        u32 *args = callback->param;
        ChatCallbackFunc4 func = (ChatCallbackFunc4)callback->callback;
        func(chat, args[0], args[1], args[2], args[3], user);
        break;
    }
    case 26: {
        u32 *args = callback->param;
        ChatCallbackFunc3 func = (ChatCallbackFunc3)callback->callback;
        func(chat, args[0], args[1], args[2], user);
        break;
    }
    case 27: {
        u32 *args = callback->param;
        ChatCallbackFunc4 func = (ChatCallbackFunc4)callback->callback;
        func(chat, args[0], args[1], args[2], args[3], user);
        break;
    }
    case 28: {
        u32 *args = callback->param;
        ChatCallbackFunc4 func = (ChatCallbackFunc4)callback->callback;
        func(chat, args[0], args[1], args[2], args[3], user);
        break;
    }
    case 29: {
        u32 *args = callback->param;
        ChatCallbackFunc5 func = (ChatCallbackFunc5)callback->callback;
        func(chat, args[0], args[1], args[2], args[3], args[4], user);
        break;
    }
    case 30: {
        u32 *args = callback->param;
        ChatCallbackFunc6 func = (ChatCallbackFunc6)callback->callback;
        func(chat, args[0], args[1], args[2], args[3], args[4], args[5], user);
        break;
    }
    case 31: {
        u32 *args = callback->param;
        ChatCallbackFunc2 func = (ChatCallbackFunc2)callback->callback;
        func(chat, args[0], args[1], user);
        break;
    }
    default:
        __msl_assertion_failed(ov45_022554EC, ov45_02255468, ov45_022553D8, 0x5ef);
        break;
    }
    ov45_02244500(callback);
}

void ov45_022471A0(ChatConnection *chat, int id) {
    ChatCallback *callback;
    int i;
    ChatCallback copy;
    ASSERT_CHAT(chat, ov45_022553E8, 0x5fc);
    i = 0;
    while (i < ArrayLength(chat->callbacks)) {
        callback = ov00_021FE6EC(chat->callbacks, i);
        ASSERT_CALLBACK(callback, ov45_022553E8, 0x605);
        if (callback->channel && !ov45_02248238(chat, callback->channel)) {
            ov45_02244500(callback);
            ov00_021FE968(chat->callbacks, i);
        } else if ((!callback->channel || ov45_0224861C(chat, callback->channel)) && (!id || callback->id == id)) {
            copy = *callback;
            ov00_021FE968(chat->callbacks, i);
            ov45_02246BA4(chat, &copy);
            if (id) {
                return;
            }
        } else {
            ++i;
        }
    }
}

int ov45_0224739C(ChatConnection *chat, int id) {
    ChatCallback *callback;
    int i, count;
    ASSERT_CHAT(chat, ov45_02255420, 0x63b);
    count = ArrayLength(chat->callbacks);
    for (i = 0; i < count; i++) {
        callback = ov00_021FE6EC(chat->callbacks, i);
        ASSERT_CALLBACK(callback, ov45_02255420, 0x648);
        if (callback->id == id) {
            return i;
        }
    }
    return -1;
}

ChatResult ov45_022474FC(ChatConnection *chat, int id) {
    return ov45_0224739C(chat, id) == -1 ? CHAT_FAILURE : CHAT_SUCCESS;
}
