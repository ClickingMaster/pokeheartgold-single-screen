#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on
#include <string.h>
#define W(p, o) (*(int *)((char *)(p) + (o)))
#define P(p, o) (*(void **)((char *)(p) + (o)))
#define B(p, o) (*(char *)((char *)(p) + (o)))
#define A(p, o) ((void *)((char *)(p) + (o)))
extern void __msl_assertion_failed(char *, char *, char *, int);
extern char *strchr(const char *, int);
extern int sprintf(char *, const char *, ...);
extern void *ov00_021EC3A8(int);
extern void ov00_021EC3D8(void *);
extern void ov00_021FF3D0(void);
extern void ov00_021FF3C0(int);
extern int ov45_0224F770(void *, int);
extern int ov45_0224F660(void *, int);
extern int ov45_0224A7F0(void *, void *, int, int);
extern int ov45_02248238(void *, char *);
extern int ov45_022487BC(void *, char *);
extern void ov45_0224A6B4(void *);
extern void ov45_022479F0(void *);
extern void ov45_02244BCC(void *);
extern void ov45_02252CB4(void *, char *);
extern void ov45_02252AA0(void *);
extern void ov45_022522B4(void *);
extern void ov45_02247D84(void *, char *);
extern int ov45_02252F74(void *, char *, ...);
extern void ov00_021FF59C(void *, int *, int *, int *);
extern char ov45_022569C0[];
extern char ov45_022569D8[];
extern char ov45_022569E8[];
extern char ov45_022569F8[];
extern char ov45_02256A18[];
extern char ov45_02256A2C[];
extern char ov45_02256A40[];
extern char ov45_02256A68[];
extern char ov45_02256A7C[];
extern char ov45_02256A90[];
extern char ov45_02256AA4[];
extern char ov45_02256AB8[];
extern char ov45_02256ACC[];
extern char ov45_02256AE0[];
extern char ov45_02256AF4[];
extern char ov45_02256B0C[];
extern char ov45_02256B3C[];
extern char ov45_02256B54[];
extern char ov45_02256B6C[];
extern char ov45_02256B94[];
extern char ov45_02256BA0[];
extern char ov45_02256BC0[];
extern char ov45_02256C2C[];
extern char ov45_02256C48[];
extern char ov45_02256DD0[];
extern char ov45_02256DDC[];
extern char ov45_02256DF4[];
extern char ov45_02256E04[];
extern char ov45_02256E64[];
extern char ov45_02256E94[];
extern char ov45_02256E9C[];
extern char ov45_02256EAC[];
extern char ov45_02256EC0[];
extern char ov45_02256ECC[];
extern char ov45_02256ED8[];
extern char ov45_02256F50[];
extern char ov45_02256F60[];
extern char ov45_02256F78[];
extern char ov45_02256F88[];
extern char ov45_02256F94[];
extern char ov45_02256FA0[];
extern char ov45_02256FB4[];
extern char ov45_02256FC4[];
extern char ov45_02256FD8[];
extern char ov45_02256FDC[];
extern char ov45_02256FE8[];
extern char ov45_02256FF0[];
extern char ov45_02256FFC[];
extern char ov45_0225700C[];
extern char ov45_02257018[];
extern char ov45_02257028[];
extern char ov45_02257038[];
extern char ov45_02257050[];
extern char ov45_0225705C[];
extern char ov45_0225706C[];
extern char ov45_0225707C[];
extern char ov45_02257094[];
extern char ov45_0225709C[];
extern char ov45_022570A4[];
extern char ov45_022570B8[];
extern char ov45_022570C4[];
extern char ov45_022570D4[];
extern char ov45_022570E4[];
extern char ov45_022570FC[];
extern char ov45_0225710C[];
extern char ov45_02257124[];
extern char ov45_0225713C[];
extern char ov45_02257140[];
extern char ov45_02257174[];
extern char ov45_02257194[];
extern char ov45_022571D4[];
extern char ov45_022571E4[];
extern char ov45_022571F8[];
extern char ov45_0225720C[];
extern char ov45_02257234[];
extern char ov45_02257248[];
extern char ov45_02257258[];
extern char ov45_02257268[];
extern char ov45_0225727C[];
extern char ov45_02257290[];
extern char ov45_022572B4[];
extern char ov45_022572CC[];
extern char ov45_022572F0[];
extern char ov45_022572FC[];
extern char ov45_0225730C[];
extern char ov45_02257324[];
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
extern char ov45_02257578[];
extern char ov45_02257590[];
extern char ov45_0225759C[];
extern char ov45_022575B0[];
extern char ov45_022575D8[];
extern char ov45_022577F0[];
extern char ov45_022577F4[];
extern void ov45_02251B0C(void *, int, char *, int, int);
extern void ov45_0225035C(void *p, char *name);
extern void ov45_02250424(void *p, int x, char *name, char *other);
extern void ov45_022504E0(void *p);
extern int ov45_02250590(void *p);
extern void ov45_022505A0(void *p, char *name, void *x, int y, int z, int wait);
extern void ov45_022507D8(void *p, char *name, char *other);
extern void ov45_02250E6C(void *p, char *name, int n);
extern int ov45_02250F5C(void *p, char *name);
extern int ov45_02251024(void *p, char *name);
extern char *ov45_022514F4(void);
extern int ov45_02251A8C(char *name);
extern int ov45_02251BE0(void *p);
extern void ov45_02251C40(void *p);
extern int ov45_02251EF0(void *p);
extern void ov45_02252530(void *p, int *a, int *b, int *c);
extern int ov45_0224FABC(int, void *, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int);
extern void ov45_02244D30(void *, int, void *, void *, void *, int, int, int);
#include "to90_ov45_input_private.h"
extern void ov45_02247A44(void *, char *);
extern int ov45_022502D0(void *p, int a, int b, int c, int d, int e, int f, int g, int h, int i, int j, int k);

extern void ov45_02251554(void *p, char *a, char *b, int n, char **keys, char **values);
extern void *ov00_021EC3BC(void *, int);
extern int ov45_02251C50(void *p, int n);
extern void ov45_02251DB4(void *p, int n);
extern void ov45_02251350(void *p, char *a, char *b, u32 mode);

typedef struct {
    void *fn;
    void *arg;
} Callback;

extern int ov45_022510D4(void *, char *, int, int, int, Callback *);

void ov45_02252530(void *p, int *a, int *b, int *c) {
    int x = 0, y = 0, z = 0;
    ov00_021FF59C(p, &x, &y, &z);
    if (a) {
        *a = x;
    }
    if (b) {
        *b = y;
    }
    if (c) {
        *c = z;
    }
}
