#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#include <string.h>
#define W(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define H(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define B(p, o) (*(u8 *)((u8 *)(p) + (o)))

extern struct {
    u32 pad[2];
    u8 *state;
} ov00_0221A918;

extern struct {
    u32 pad[7];
    u8 a, b;
    u16 c;
} ov00_0221A928;

#define R ((u8 *)&ov00_0221A928)
#define F ov00_0221A918.state
extern int ov00_021F03BC(int);
extern int DWC_IsBuddyFriendData(void *);
extern int DWC_GetFriendDataType(void *);
extern void DWC_SetGsProfileId(void *, int);
extern void DWCi_SetBuddyFriendData(void *);
extern void ov00_021F094C(void *, int, int);
extern int ov00_0220014C(void *, int, char *);
extern void ov00_021F0238(int, int);
extern int ov00_021EFC6C(void);
extern void *ov00_021EF3A4(void);
extern int DWC_GetGsProfileId(void *, void *);
extern int ov00_02200540(void *, int, int *);
extern int ov00_02200400(void *, int, void *);
extern char ov00_02217A48[];
extern void ov00_0220FB40(void);
extern void ov00_021F6BA8(void);
extern int ov00_021EC230(void);
extern u8 *ov00_021F955C(void);
extern int ov00_021EE568(void);
extern void ov00_021F36BC(void);
extern void ov00_021F954C(void *);
extern void ov00_021F956C(int);
extern void ov00_021F7AD8(void);
extern void ov00_021F381C(void);
extern void ov00_021F3BB4(int, int, int, int);
extern int ov00_021F8278(void);
extern int ov00_021F179C(int);
extern void ov00_02210AC8(void *);
extern void ov00_02210CBC(void *);
#pragma warn_uninitializedvar off
int ov00_021F09A4(u8 *p, int n, int id);
int ov00_021F0A50(u8 *p, int n, int id);
int ov00_021F0B58(int id);
int ov00_021F0B8C(void *p, void *out);
int ov00_021F0C48(u32 a);
u32 ov00_021F1248(void);
void ov00_021F125C(void);
int ov00_021F1284(void);
int ov00_021F12A4(void);
int ov00_021F12F0(void (*cb)(void *), void *arg);
void ov00_021F1620(void *p, u32 a, u32 b, u32 c, u32 v0, u32 v1, u32 v2, u32 v3);
void ov00_021F1B24(int a, int b, int c, u32 cb, u32 arg);
void ov00_021F1D34(void *p);

extern void *ov00_021EC2A8(int, int);
extern void ov00_021EC2EC(int, void *, int);
extern void ov00_021F2EE4(int, int);
extern int ov00_021F7A98(void);
extern void ov00_02212008(int, char *);
extern char ov00_02219D28[];
extern int ov00_02211A88(void *, void *, int, void *);
extern void ov00_0221075C(void *, int, void *);
extern void ov00_021F8380(int);
extern int ov00_0220D294(void *);
extern int ov00_0220D27C(void *);
extern int ov00_022108BC(void *, int, int, u32, u32, int, int, void *, void *, void *, void *, void *, void *, int);
extern void ov00_02210D18(void *);
extern void ov00_02210AB0(void *, void *);
extern void ov00_02210A80(void *, void *);
extern void ov00_02210A98(void *, void *);
extern void ov00_021F8D18(void);
extern void ov00_021F8E60(void);
extern void ov00_021F8E64(void);
extern void ov00_021F8E68(void);
extern void ov00_021F8F0C(void);
extern void ov00_021F8F14(void);
extern void ov00_021F8F20(void);
extern void ov00_021F8F40(void);
extern void ov00_021F8FF4(void);
extern u8 ov00_0221A948[];
extern u8 ov00_0221A949[];
extern u16 ov00_0221A94A[];
extern char *ov00_0221A94C[];
extern u32 ov00_0221A950[];
u32 ov00_021F13A4(u32 id, const char *s, u32 cb);
int ov00_021F14D4(int mode, u8 *p);
int ov00_021F179C(int arg);
int ov00_021F2880(int unused, u32 ip, int port, u8 *p, int len);

void ov00_021F1B24(int a, int b, int c, u32 cb, u32 arg) {
    ov00_021F3BB4(2, a, b, c);
    W(ov00_021F955C(), 0x46c) = cb;
    W(ov00_021F955C(), 0x470) = arg;
    W(ov00_021F955C(), 0xf4) = W(ov00_021F955C(), 0x200);
    W(ov00_021F955C(), 0x2f0) = 1;
    B(ov00_021F955C(), 0xe) = 0;
    B(R, 0x1d) = 0;
    ov00_021F956C(10);
    ov00_021F36BC();
    if (ov00_021F8278()) {
        return;
    }
    if (W(ov00_021F955C(), 0x10)) {
        return;
    }
    ov00_021F179C(W(ov00_021F955C(), 0x200));
}
