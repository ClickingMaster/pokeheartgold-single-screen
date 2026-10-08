#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#include <string.h>

typedef void *(*LoginAlloc)(const char *name, u32 size);
typedef void (*LoginFree)(const char *name, void *ptr, u32 size);

typedef struct LoginMain {
    u8 stack[0x1000];
    int state;
    int error;
    u8 unk1008[0x100];
    LoginAlloc alloc;
    LoginFree free;
    void *param;
    char *redirect;
    char *token;
} LoginMain;

typedef struct LoginRequest {
    u8 stack[0x1000];
    u8 unk1000[0x20];
    int state;
    u8 unk1024[0x108];
    u32 value;
    u8 unk1130[0x8d8];
    char *response;
    u8 unk1A0C[0x12c];
    OSThread thread;
} LoginRequest;

typedef struct LoginRequestParams {
    char *url;
    int method;
    int bufferSize;
    LoginAlloc alloc;
    LoginFree free;
    int custom;
    int timeout;
} LoginRequestParams;

typedef struct LoginAuthParams {
    u16 name[26];
    char realm[12];
    LoginAlloc alloc;
    LoginFree free;
} LoginAuthParams;

typedef struct LoginWork {
    LoginMain *main;
    int redirected;
    u32 value;
    int mode;
    int unk10;
    LoginRequest *request;
    LoginRequestParams reqParams;
    LoginAuthParams authParams;
} LoginWork;

typedef struct LoginWiFiInfo {
    u64 id;
    u8 unk8[12];
} LoginWiFiInfo;

typedef struct LoginAuthResult {
    int error;
    u8 unk4[0x1c0];
} LoginAuthResult;

extern LoginWork ov00_0221A6B8;
extern LoginRequestParams ov00_0221A6D0; // ov00_0221A6B8.reqParams
extern LoginAuthParams ov00_0221A6EC; // ov00_0221A6B8.authParams
extern char *ov00_022168DC;
extern char *ov00_02216420;
extern char ov00_022169F0[], ov00_022169FC[], ov00_02216A1C[], ov00_02216A24[], ov00_02216A2C[], ov00_02216954[], ov00_02216988[];
extern char ov00_02216A3C[], ov00_02216A44[], ov00_02216938[], ov00_02216A4C[], ov00_02216A58[], ov00_02216A5C[], ov00_02216A64[];
extern char ov00_022169A4[], ov00_022169B0[], ov00_022169C0[], ov00_022169D0[], ov00_02216A6C[], ov00_02216A78[], ov00_02216A84[];
extern int errno;
extern long atol(const char *);

extern int ov00_021E8FA0(LoginRequest *req, LoginRequestParams *params);
extern int ov00_021E90BC(LoginRequest *req);
extern void ov00_021E9154(LoginRequest *req, u32 prio);
extern void ov00_021E9754(LoginRequest *req);
extern int ov00_021E9F2C(LoginRequest *req, int a);
extern char *ov00_021EA1CC(LoginRequest *req, const char *key);
extern int ov00_021EA224(LoginRequest *req, const char *key, char *buf, int size);
extern int ov00_021E8984(u8 *buf);
extern int ov00_021E8C5C(LoginRequest *req, u8 *buf, int a);
extern int ov00_021E99A4(LoginRequest *req, const char *key, const char *value, int len);
extern int ov00_021E9AB0(LoginRequest *req, const char *post);
extern int ov00_021E7B70(LoginAuthParams *params, LoginRequest *req);
extern void ov00_021E7E2C(void);
extern int ov00_021E7E5C(void);
extern void ov00_021E7EA8(LoginAuthResult *result);
extern void ov00_021E7DD4(void);
extern void ov00_021EB56C(int result);
extern void DWCi_BM_GetWiFiInfo(LoginWiFiInfo *info);

void ov00_021EA660(void *arg);

// @fn ov00_021EA660
void ov00_021EA660(void *arg) {
    char *response;
    int status;
    char number[4];
    LoginWiFiInfo wifi;
    LoginAuthResult result;
    u8 auth[0x94];
    LoginAlloc allocFunc = ov00_0221A6B8.main->alloc;
    LoginFree freeFunc = ov00_0221A6B8.main->free;
    char *url = NULL;
    char *post = NULL;
    char *wait = NULL;
    int urlLen, postLen, waitLen;
    int n;
    int waitMs;
    char *copy;
    char *str;

    while (TRUE) {
        ov00_0221A6B8.reqParams.url = ov00_022168DC;
        ov00_0221A6B8.reqParams.method = 1;
        ov00_0221A6B8.reqParams.bufferSize = 0x1000;
        ov00_0221A6B8.reqParams.alloc = allocFunc;
        ov00_0221A6B8.reqParams.free = freeFunc;
        ov00_0221A6B8.reqParams.timeout = 20000;
        ov00_0221A6B8.main->error = -2;
        if (ov00_021E8FA0(ov00_0221A6B8.request, &ov00_0221A6D0)) {
            ov00_021E9754(ov00_0221A6B8.request);
            ov00_021EB56C(1);
            goto done;
        }
        if (ov00_021E90BC(ov00_0221A6B8.request)) {
            ov00_021E9754(ov00_0221A6B8.request);
            ov00_021EB56C(1);
            goto done;
        }
        ov00_021E9154(ov00_0221A6B8.request, OS_GetThreadPriority(OSi_ThreadInfo.current) - 1);
        if (ov00_0221A6B8.request->thread.id != 0) {
            OS_JoinThread(&ov00_0221A6B8.request->thread);
        }
        switch (ov00_0221A6B8.request->state) {
        case 2:
            ov00_0221A6B8.main->error = -1;
        default:
            ov00_021E9754(ov00_0221A6B8.request);
            ov00_021EB56C(3);
            goto done;
        case 8:
            break;
        }
        if (ov00_021E9F2C(ov00_0221A6B8.request, 0) != 1) {
            ov00_021E9754(ov00_0221A6B8.request);
            ov00_021EB56C(2);
            goto done;
        }
        status = atol(ov00_021EA1CC(ov00_0221A6B8.request, ov00_022169F0));
        if (errno == 34) {
            ov00_021EB56C(2);
            goto done;
        }
        switch (status) {
        case 200:
            ov00_0221A6B8.value = ov00_0221A6B8.request->value;
            break;
        case 302:
            ov00_0221A6B8.redirected = 1;
            if (ov00_0221A6B8.main->token != NULL) {
                ov00_0221A6B8.main->error = -6;
                ov00_021E9754(ov00_0221A6B8.request);
                ov00_0221A6B8.reqParams.url = ov00_02216420;
                ov00_0221A6B8.reqParams.method = 0;
                ov00_0221A6B8.reqParams.bufferSize = 0x200;
                ov00_0221A6B8.reqParams.alloc = allocFunc;
                ov00_0221A6B8.reqParams.free = freeFunc;
                ov00_0221A6B8.reqParams.timeout = 20000;
                if (strcmp(ov00_0221A6B8.reqParams.url, ov00_022169FC) != 0) {
                    ov00_0221A6B8.reqParams.custom = 1;
                }
                if (ov00_021E8FA0(ov00_0221A6B8.request, &ov00_0221A6D0)) {
                    ov00_021E9754(ov00_0221A6B8.request);
                    ov00_021EB56C(1);
                    goto done;
                }
                if (!ov00_021E8984(auth) || !ov00_021E8C5C(ov00_0221A6B8.request, auth, 1)) {
                    ov00_021E9754(ov00_0221A6B8.request);
                    ov00_021EB56C(8);
                    goto done;
                }
                if (ov00_021E99A4(ov00_0221A6B8.request, ov00_02216A1C, ov00_02216A24, 7)
                    || (str = ov00_0221A6B8.main->token, ov00_021E99A4(ov00_0221A6B8.request, ov00_02216A2C, str, strlen(str)))) {
                    ov00_021E9754(ov00_0221A6B8.request);
                    ov00_021EB56C(8);
                    goto done;
                }
                freeFunc(ov00_02216954, ov00_0221A6B8.main->token, 0);
                ov00_0221A6B8.main->token = NULL;
                if (ov00_021E90BC(ov00_0221A6B8.request)) {
                    ov00_021E9754(ov00_0221A6B8.request);
                    ov00_021EB56C(1);
                    goto done;
                }
                ov00_021E9154(ov00_0221A6B8.request, OS_GetThreadPriority(OSi_ThreadInfo.current) - 1);
                if (ov00_0221A6B8.request->thread.id != 0) {
                    OS_JoinThread(&ov00_0221A6B8.request->thread);
                }
                switch (ov00_0221A6B8.request->state) {
                case 2:
                    ov00_0221A6B8.main->error = -1;
                default:
                    ov00_021E9754(ov00_0221A6B8.request);
                    ov00_021EB56C(3);
                    goto done;
                case 8:
                    ov00_021E9754(ov00_0221A6B8.request);
                    ov00_021EB56C(7);
                    goto done;
                }
            } else {
                response = ov00_0221A6B8.request->response;
                if (response == NULL) {
                    ov00_021E9754(ov00_0221A6B8.request);
                    ov00_021EB56C(2);
                    goto done;
                }
                ov00_0221A6B8.main->redirect = allocFunc(ov00_02216988, strlen(response) + 1);
                copy = ov00_0221A6B8.main->redirect;
                if (copy == NULL) {
                    ov00_021E9754(ov00_0221A6B8.request);
                    ov00_021EB56C(4);
                    goto done;
                }
                strncpy(copy, response, strlen(response) + 1);
            }
            break;
        default:
            ov00_021E9754(ov00_0221A6B8.request);
            ov00_021EB56C(10);
            goto done;
        }
        ov00_021E9754(ov00_0221A6B8.request);
        DWCi_BM_GetWiFiInfo(&wifi);
        if (wifi.id == 0) {
            ov00_0221A6B8.main->error = -3;
            ov00_0221A6B8.authParams.name[0] = 0;
            ov00_0221A6B8.authParams.realm[0] = 0;
            ov00_0221A6B8.authParams.alloc = ov00_0221A6B8.main->alloc;
            ov00_0221A6B8.authParams.free = ov00_0221A6B8.main->free;
            if (ov00_021E7B70(&ov00_0221A6EC, ov00_0221A6B8.request)) {
                ov00_021EB56C(5);
                goto done;
            }
            ov00_021E7E2C();
            if (ov00_021E7E5C() != 21) {
                if (ov00_021E7E5C() == 9) {
                    ov00_0221A6B8.main->error = -1;
                } else {
                    ov00_021E7EA8(&result);
                    if (ov00_0221A6B8.mode == 1 && (result.error == -23302 || ov00_021E7E5C() == 11)) {
                        ov00_0221A6B8.main->error = 0;
                        ov00_021E7DD4();
                        ov00_021EB56C(11);
                        goto done;
                    }
                    ov00_0221A6B8.main->error = result.error;
                }
                ov00_021E7DD4();
                ov00_021EB56C(6);
                goto done;
            }
            ov00_021E7DD4();
        }
        if (status == 200) {
            ov00_0221A6B8.main->error = 0;
            ov00_021EB56C(11);
            goto done;
        }
        ov00_0221A6B8.main->error = -4;
        ov00_0221A6B8.reqParams.url = ov00_02216420;
        ov00_0221A6B8.reqParams.method = 0;
        ov00_0221A6B8.reqParams.bufferSize = 0x1000;
        ov00_0221A6B8.reqParams.alloc = allocFunc;
        ov00_0221A6B8.reqParams.free = freeFunc;
        ov00_0221A6B8.reqParams.timeout = 40000;
        if (strcmp(ov00_0221A6B8.reqParams.url, ov00_022169FC) != 0) {
            ov00_0221A6B8.reqParams.custom = 1;
        }
        if (ov00_021E8FA0(ov00_0221A6B8.request, &ov00_0221A6D0)) {
            ov00_021E9754(ov00_0221A6B8.request);
            ov00_021EB56C(1);
            goto done;
        }
        if (!ov00_021E8984(auth) || !ov00_021E8C5C(ov00_0221A6B8.request, auth, 1)) {
            ov00_021E9754(ov00_0221A6B8.request);
            ov00_021EB56C(8);
            goto done;
        }
        if (ov00_021E99A4(ov00_0221A6B8.request, ov00_02216A1C, ov00_02216A3C, 5)
            || (str = ov00_0221A6B8.main->redirect, ov00_021E99A4(ov00_0221A6B8.request, ov00_02216A44, str, strlen(str)))) {
            ov00_021E9754(ov00_0221A6B8.request);
            ov00_021EB56C(8);
            goto done;
        }
        freeFunc(ov00_02216938, ov00_0221A6B8.main->redirect, 0);
        ov00_0221A6B8.main->redirect = NULL;
        if (ov00_021E90BC(ov00_0221A6B8.request)) {
            ov00_021E9754(ov00_0221A6B8.request);
            ov00_021EB56C(1);
            goto done;
        }
        ov00_021E9154(ov00_0221A6B8.request, OS_GetThreadPriority(OSi_ThreadInfo.current) - 1);
        if (ov00_0221A6B8.request->thread.id != 0) {
            OS_JoinThread(&ov00_0221A6B8.request->thread);
        }
        switch (ov00_0221A6B8.request->state) {
        case 3:
            ov00_021E9754(ov00_0221A6B8.request);
            if (ov00_0221A6B8.mode == 1) {
                ov00_0221A6B8.main->error = 0;
                ov00_021EB56C(11);
            } else {
                ov00_021EB56C(3);
            }
            goto done;
        case 2:
            ov00_0221A6B8.main->error = -1;
        default:
            ov00_021E9754(ov00_0221A6B8.request);
            ov00_021EB56C(3);
            goto done;
        case 8:
            break;
        }
        if (ov00_021E9F2C(ov00_0221A6B8.request, 0) != 1) {
            ov00_021E9754(ov00_0221A6B8.request);
            ov00_021EB56C(2);
            goto done;
        }
        status = atol(ov00_021EA1CC(ov00_0221A6B8.request, ov00_022169F0));
        if (errno == 34) {
            ov00_021E9754(ov00_0221A6B8.request);
            ov00_021EB56C(2);
            goto done;
        }
        if (status != 200) {
            ov00_021E9754(ov00_0221A6B8.request);
            if (ov00_0221A6B8.mode == 1 && status == 302) {
                ov00_0221A6B8.main->error = 0;
                ov00_021EB56C(11);
            } else {
                ov00_021EB56C(2);
            }
            goto done;
        }
        if (ov00_021EA224(ov00_0221A6B8.request, ov00_02216A4C, number, 4) <= 0) {
            ov00_021E9754(ov00_0221A6B8.request);
            ov00_021EB56C(9);
            goto done;
        }
        n = atol(number);
        if (errno == 34) {
            ov00_021E9754(ov00_0221A6B8.request);
            ov00_021EB56C(9);
            goto done;
        }
        if (ov00_0221A6B8.mode == 1 && n == 114) {
            ov00_0221A6B8.main->error = 0;
            ov00_021EB56C(11);
            goto done;
        }
        if (n >= 100) {
            ov00_021E9754(ov00_0221A6B8.request);
            ov00_021EB56C(6);
            goto done;
        }
        urlLen = ov00_021EA224(ov00_0221A6B8.request, ov00_02216A58, NULL, 0);
        if (urlLen <= 0) {
            ov00_021E9754(ov00_0221A6B8.request);
            ov00_021EB56C(9);
            goto done;
        }
        postLen = ov00_021EA224(ov00_0221A6B8.request, ov00_02216A5C, NULL, 0);
        if (postLen <= 0) {
            ov00_021E9754(ov00_0221A6B8.request);
            ov00_021EB56C(9);
            goto done;
        }
        waitLen = ov00_021EA224(ov00_0221A6B8.request, ov00_02216A64, NULL, 0);
        url = allocFunc(ov00_022169A4, urlLen + 1);
        if (url == NULL) {
            ov00_021E9754(ov00_0221A6B8.request);
            ov00_021EB56C(4);
            goto done;
        }
        post = allocFunc(ov00_022169B0, postLen + 1);
        if (post == NULL) {
            ov00_021E9754(ov00_0221A6B8.request);
            ov00_021EB56C(4);
            goto done;
        }
        if (waitLen > 0) {
            wait = allocFunc(ov00_022169C0, waitLen + 1);
            if (wait == NULL) {
                ov00_021E9754(ov00_0221A6B8.request);
                ov00_021EB56C(4);
                goto done;
            }
        }
        n = ov00_021EA224(ov00_0221A6B8.request, ov00_02216A58, url, urlLen + 1);
        if (n < 0) {
            ov00_021E9754(ov00_0221A6B8.request);
            ov00_021EB56C(9);
            goto done;
        }
        url[n] = 0;
        n = ov00_021EA224(ov00_0221A6B8.request, ov00_02216A5C, post, postLen + 1);
        if (n < 0) {
            ov00_021E9754(ov00_0221A6B8.request);
            ov00_021EB56C(9);
            goto done;
        }
        post[n] = 0;
        waitMs = 0;
        if (waitLen > 0) {
            n = ov00_021EA224(ov00_0221A6B8.request, ov00_02216A64, wait, waitLen + 1);
            if (n < 0) {
                ov00_021E9754(ov00_0221A6B8.request);
                ov00_021EB56C(9);
                goto done;
            }
            wait[n] = 0;
            n = atol(wait);
            if (errno == 34) {
                ov00_021E9754(ov00_0221A6B8.request);
                ov00_021EB56C(9);
                goto done;
            }
            waitMs = n * 1000;
            if (waitMs > 180000) {
                waitMs = 180000;
            }
        }
        ov00_021E9754(ov00_0221A6B8.request);
        ov00_0221A6B8.main->error = -5;
        ov00_0221A6B8.reqParams.url = url;
        ov00_0221A6B8.reqParams.method = 0;
        ov00_0221A6B8.reqParams.bufferSize = 0x1000;
        ov00_0221A6B8.reqParams.alloc = allocFunc;
        ov00_0221A6B8.reqParams.free = freeFunc;
        ov00_0221A6B8.reqParams.timeout = 120000;
        if (ov00_021E8FA0(ov00_0221A6B8.request, &ov00_0221A6D0)) {
            ov00_021E9754(ov00_0221A6B8.request);
            ov00_021EB56C(1);
            goto done;
        }
        if (ov00_021E9AB0(ov00_0221A6B8.request, post)) {
            ov00_021E9754(ov00_0221A6B8.request);
            ov00_021EB56C(8);
            goto done;
        }
        if (ov00_021E90BC(ov00_0221A6B8.request)) {
            ov00_021E9754(ov00_0221A6B8.request);
            ov00_021EB56C(1);
            goto done;
        }
        ov00_021E9154(ov00_0221A6B8.request, OS_GetThreadPriority(OSi_ThreadInfo.current) - 1);
        if (ov00_0221A6B8.request->thread.id != 0) {
            OS_JoinThread(&ov00_0221A6B8.request->thread);
        }
        switch (ov00_0221A6B8.request->state) {
        case 2:
            ov00_0221A6B8.main->error = -1;
        default:
            ov00_021E9754(ov00_0221A6B8.request);
            ov00_021EB56C(3);
            goto done;
        case 8:
            break;
        }
        if (ov00_021E9F2C(ov00_0221A6B8.request, 1) != 1) {
            ov00_021E9754(ov00_0221A6B8.request);
            ov00_021EB56C(2);
            goto done;
        }
        response = ov00_0221A6B8.request->response;
        if (response == NULL) {
            ov00_021E9754(ov00_0221A6B8.request);
            ov00_021EB56C(2);
            goto done;
        }
        ov00_0221A6B8.main->token = allocFunc(ov00_022169D0, strlen(response) + 1);
        copy = ov00_0221A6B8.main->token;
        if (copy == NULL) {
            ov00_021E9754(ov00_0221A6B8.request);
            ov00_021EB56C(4);
            goto done;
        }
        strncpy(copy, response, strlen(response) + 1);
        ov00_021E9754(ov00_0221A6B8.request);
        OS_Sleep(waitMs);
    }
done:
    if (url != NULL) {
        freeFunc(ov00_02216A6C, url, 0);
    }
    if (post != NULL) {
        freeFunc(ov00_02216A78, post, 0);
    }
    if (wait != NULL) {
        freeFunc(ov00_02216A84, wait, 0);
    }
}
