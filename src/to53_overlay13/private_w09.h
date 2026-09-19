#ifndef TO53_W09_PRIVATE_H
#define TO53_W09_PRIVATE_H

#include "global.h"

typedef int (*Ov13AllocFn)(int);
typedef int (*Ov13FreeFn)(void *);

typedef struct Ov13State {
    int unk00;
    void *unk04;
    int unk08;
    void *unk0C;
    int unk10;
    void *unk14;
    u8 pad18[0x0C];
    void *unk24;
    int unk28;
    int unk2C;
    int unk30;
    int unk34;
    int unk38;
} Ov13State;

extern Ov13AllocFn ov13_0224DD80;
extern Ov13FreeFn ov13_0224DD8C;
extern Ov13State _0224CF98;
extern int ov13_022459B4[2];
extern u8 ov13_02245A14[];
extern u8 ov13_02245A20[];
extern u8 ov13_0224DCF8[];
extern u8 ov13_0224CFBC[];
extern u8 ov13_0224CFB8[];

void ov13_02222968(void *dst, const void *src, int size);
void ov13_02222978(void *dst, int value, int size);
int ov13_02222AB0(void);
int ov13_02222A44(int value);
int ov13_02223100(int value);
int ov13_02222108(void *a, void *b, void *c);
int ov13_0222224C(void *a, void *b, void *c);
int ov13_02222328(void *a, void *b, void *c);
int ov13_02222A9C(const void *data);
int ov13_02222948(const void *a, const void *b, int size);
int ov13_022227A0(void *a, int b, const void *c, int d);
int ov13_02222A84(int value);
int ov13_022208E8(int size);
int ov13_022208F8(void *ptr);
u32 ov13_02221410(u32 value, u32 mask);
void ov13_02221428(void);
void ov13_022214AC(int value);
int ov13_022214B8(void);
int ov13_022216BC(const u8 *data, int length);
void ov13_022217A0(int count, u8 *dst, const void *src);
int ov13_022217D0(int value);
int ov13_022220A4(int value);
int ov13_022220B4(int which, void *a, void *b, void *c);
void ov13_02221454(u8 *src);
int ov13_02221BE8(int value, const void *data);
int ov13_02221C2C(const u8 *a, u8 *b);
int ov13_02221F64(const u8 *a, void *dst);

#endif
