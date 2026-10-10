#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on
#include <string.h>
#define W(p, o) (*(int *)((char *)(p) + (o)))
#define U(p, o) (*(u32 *)((char *)(p) + (o)))
#define P(p, o) (*(void **)((char *)(p) + (o)))
#define B(p, o) (*(char *)((char *)(p) + (o)))
#define A(p, o) ((void *)((char *)(p) + (o)))
extern void __msl_assertion_failed(char *, char *, char *, int);
extern void *ov00_021EC3BC(void *, int);
extern void *ov00_021EC3A8(int);
extern void ov00_021EC3D8(void *);
extern void ov00_021EC2EC(int, void *, int);
extern void ov00_021FA0D8(void);
extern void ov00_021EC24C(int, int);
extern int ov00_021EC230(void);
extern int ov45_0225478C(int, int, int, int, int, int);
extern int ov45_02253528(void *, char *);
extern void ov45_02251DB4(void *, int);
extern char ov45_02257148[];
extern char ov45_02257154[];
extern char ov45_02257164[];
extern char ov45_02257184[];
extern char ov45_022571A4[];
extern char ov45_022571B4[];
extern char ov45_022571C4[];
extern char ov45_02257220[];
extern char ov45_02257248[];
extern char ov45_02257258[];
extern char ov45_02257334[];
extern char ov45_02257344[];
extern char ov45_022573C4[];
extern char ov45_022573E4[];
extern char ov45_02257408[];
extern char ov45_02257438[];
extern char ov45_0225745C[];
extern char ov45_0225749C[];
extern char ov45_022574BC[];
extern char ov45_022574E0[];
extern char ov45_02257514[];
extern char ov45_02257538[];
extern char ov45_022575D8[];
extern char ov45_022575FC[];
extern char ov45_02257600[];
extern char ov45_02257610[];
extern char ov45_02257620[];
extern char ov45_02257624[];
extern char ov45_02257628[];
extern char ov45_0225762C[];
extern char ov45_0225763C[];
extern char ov45_0225765C[];
extern char ov45_022576A0[];
extern char ov45_022576CC[];
extern char ov45_022576D4[];
extern char ov45_022576DC[];
extern char ov45_02257704[];
extern char ov45_02257728[];
extern char ov45_02257750[];
extern char ov45_02257758[];
extern char ov45_0225775C[];
extern char ov45_02257764[];
extern char ov45_02257780[];
extern char ov45_022577AC[];
extern char ov45_022577F8[];
extern char ov45_022587F8[];
extern int ov45_02253968(void *);
extern int ov45_02253374(char *s, void *p);
extern int ov45_022533E4(char *s, void *p);
extern u32 ov45_02253D50(u32 n);
extern void ov45_02253D88(u32 n);
extern int ov45_02253D9C(void);
extern int ov45_02253DDC(char *a, char *b, u32 c, u32 d, u32 e, u32 f);
extern void ov45_022545BC(void);
extern void ov45_022546F8(void);
extern int ov45_02254774(int *p);
extern int ov45_022549E0(int a, int b, int c, int d, int e);
extern int ov45_02254A04(int a, int b, int c, int d, int e);
extern void *ov45_02253B5C(void *p);
extern void ov45_02254088(void);
extern void *ov00_021EC2A8(int, int);
extern void ov00_021FFC18(void *, void *, int, int);
extern void ov00_021FA0B4(int);
extern unsigned long strtoul(const char *, char **, int);
extern int ov45_022542F4(u8 *out, u8 *a, int na, u8 *b, int nb);
extern void ov45_0225443C(int mode, char *s);
extern int ov45_02253968(void *p);
extern void ov45_0225408C(void);
extern int ov00_021FA488(void *, int, int, void (*)(void), void (*)(void), int), ov00_021FA118(void);
extern int ov45_022545F0(void);

int ov45_022549E0(int a, int b, int c, int d, int e) {
    return ov45_0225478C(a, b, c, d, e, 0);
}

int ov45_02254A04(int a, int b, int c, int d, int e) {
    return ov45_0225478C(a, b, c, d, e, 1);
}
