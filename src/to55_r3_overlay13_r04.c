#include "global.h"

#define U8_AT(p, o)  (*(u8 *)((u8 *)(p) + (o)))
#define U16_AT(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define U32_AT(p, o) (*(u32 *)((u8 *)(p) + (o)))

u32 ov13_02222A84(int value);
int ov13_02222A5C(int value);
void ov13_022214AC(int value);
int ov13_02221C2C(const u8 *a, u8 *b);
int ov13_02221C78(const void *a, void *b);
int ov13_022220A4(int value);
void ov13_02222978(void *dst, int value, int size);
int ov13_02221F94(int a0, void *body, int value, void *buffer, void *extra);
extern u8 ov13_0224CFD8[];
extern u8 ov13_0224D658[];
extern int _0224CF98[];
int ov13_02221AF4(void *packet);
int ov13_0222189C(int state, void *packet, int *counter, void *context);
int ov13_02221978(int state, void *packet, int *counter, void *context);
int ov13_02221A50(int state, void *packet, int *counter, void *context);
int ov13_022217FC(int state, void *packet, int *counter, void *context);

int ov13_022217FC(int state, void *packet, int *counter, void *context) {
    u8 *header = (u8 *)packet + 0xC;
    int type;

    if (ov13_02222A84(U16_AT(header, 0)) < 1) {
        (*counter)++;
        return state;
    }
    if (U8_AT(header, 0xF) != 0x11) {
        (*counter)++;
        return state;
    }
    if (ov13_02221AF4((u8 *)packet + 0xC) > 0) {
        (*counter)++;
        return state;
    }
    type = ov13_02222A84(U16_AT(header, 6));
    switch (type) {
    case 0x1010:
        state = ov13_0222189C(state, packet, counter, context);
        break;
    case 0x2010:
        state = ov13_02221978(state, packet, counter, context);
        break;
    case 0x3010:
        state = ov13_02221A50(state, packet, counter, context);
        break;
    }
    return state;
}

int ov13_0222189C(int state, void *packet, int *counter, void *context) {
    u8 *header;
    u8 *body;
    int value;

    if (state != 0) {
        (*counter)++;
        return state;
    }
    header = (u8 *)packet + 0xC;
    body = (u8 *)packet + 0x24;
    if (ov13_02221C2C(context, header + 0x10) < 0) {
        (*counter)++;
        return state;
    }
    if (ov13_02222A84(U16_AT(body, 2)) == 0) {
        (*counter)++;
        return state;
    }
    if (body[0] == 7) {
        value = ov13_02222A5C(U32_AT(body, 4));
        if (value == -2) {
            ov13_022214AC(0x14);
        } else {
            value = ov13_02222A5C(U32_AT(body, 4));
            if (value == -3) {
                ov13_022214AC(0x15);
            } else {
                ov13_022214AC(0x18);
            }
        }
        return -1;
    }
    if (body[0] != 1) {
        (*counter)++;
        return state;
    }
    value = ov13_02221C78(body + 4, ov13_0224CFD8);
    if (value < 0) {
        if (value == -2) {
            ov13_022214AC(0x16);
            return -1;
        }
        (*counter)++;
        return state;
    }
    _0224CF98[0] = ov13_022220A4(ov13_02222A84(U16_AT(header, 0xC)));
    *counter = 0;
    return 1;
}

int ov13_02221978(int state, void *packet, int *counter, void *context) {
    u8 *header;
    u8 *body;
    int value;

    if (state != 1) {
        (*counter)++;
        return state;
    }
    header = (u8 *)packet + 0xC;
    body = (u8 *)packet + 0x24;
    if (ov13_02221C2C((u8 *)context + 8, header + 0x10) < 0) {
        (*counter)++;
        return state;
    }
    if (ov13_02222A84(U16_AT(body, 2)) == 0) {
        (*counter)++;
        return state;
    }
    if (body[0] == 7) {
        value = ov13_02222A5C(U32_AT(body, 4));
        if (value == -2) {
            ov13_022214AC(0x14);
        } else {
            value = ov13_02222A5C(U32_AT(body, 4));
            if (value == -3) {
                ov13_022214AC(0x15);
            } else {
                ov13_022214AC(0x18);
            }
        }
        return -1;
    }
    ov13_02222978(ov13_0224D658, 0, 0x6A0);
    value = ov13_02222A84(U16_AT(header, 0xA));
    if (ov13_02221F94(0, body, value, ov13_0224D658, ov13_0224CFD8) < 0) {
        (*counter)++;
        return state;
    }
    if ((_0224CF98[0x30 / 4] & _0224CF98[0x2C / 4]) == 0) {
        return state;
    }
    *counter = 0;
    return 2;
}

int ov13_02221A50(int state, void *packet, int *counter, void *context) {
    u8 *body;
    int value;

    if (state != 2) {
        (*counter)++;
        return state;
    }
    body = (u8 *)packet + 0x24;
    context = (u8 *)context + 0x10;
    packet = (u8 *)packet + 0xC;
    if (ov13_02221C2C(context, (u8 *)packet + 0x10) < 0) {
        (*counter)++;
        return state;
    }
    if (body[0] != 7) {
        (*counter)++;
        return state;
    }
    if (ov13_02222A84(U16_AT(body, 2)) == 0) {
        (*counter)++;
        return state;
    }
    value = ov13_02222A5C(U32_AT(body, 4));
    if (value == 0) {
        return 100;
    }
    value = ov13_02222A5C(U32_AT(body, 4));
    if (value == -2) {
        ov13_022214AC(0x14);
        return -1;
    }
    value = ov13_02222A5C(U32_AT(body, 4));
    if (value == -3) {
        ov13_022214AC(0x15);
        return -1;
    }
    ov13_022214AC(0x18);
    return -1;
}
