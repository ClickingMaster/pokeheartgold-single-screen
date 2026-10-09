#pragma force_active on
#include "global.h"
typedef struct Ov45Work Ov45Work;
typedef struct {u8 state,substate,pending,sel;u16 flag,init;u8 buf[0x10c];u8 handler[0x38];void *extra;u16 response;u8 byte,active;} Work;
typedef struct {u8 pad0[0x34];Ov45Work *chat;u32 p38;struct {u8 graphics[0x148];Work state;u32 reserved;}ui;u8 config[0x3c],pending[0xc4];void *feature;}Ctx;
typedef struct {u32 b,a;}Pair;
typedef char work_size[(sizeof(Work)==0x154)?1:-1];
typedef char ui_size[(sizeof(((Ctx*)0)->ui)==0x2a0)?1:-1];
extern void BeginNormalPaletteFade(int,int,int,int,int,int,int);extern int IsPaletteFadeFinished(void);
extern void GfGfx_EngineBTogglePlanes(int,int);
extern void *ov45_0222A5C0(Ov45Work *),*ov45_0222A578(Ov45Work *,u32);
extern int ov45_0222A53C(Ov45Work*),ov45_0222ADA8(void *,int),ov45_0222AB28(void *,int),ov45_0222AB48(void *,int),ov45_0222AB58(void *,int);
extern void ov45_0222ADD8(void *,int),ov45_0222AE54(Ov45Work*),ov45_0222AE08(u32,u32 *,u32 *);
extern void ov49_0225B9AC(Work *,void *,void *,int,void *),ov49_0225B444(Ctx *),ov49_0225CB68(void *),ov49_0225BBCC(void *,void *),ov49_0225BA20(Work *,void *),ov49_0225B99C(Work *,void *,void *),ov49_0225B9F0(Work *,void *,int,int);
extern int ov49_0225C8D4(void *,void *,void *,int),ov49_02268968(void *,int,int);
extern void ov49_0225B944(Work *,void *,void *,void *,int,int,void *,void *,int,int,int,int);
void ov49_0225B518(Work *,Ctx *,int,int);
#pragma opt_propagation off
// @fn ov49_0225B518
void ov49_0225B518(Work *w, Ctx *c, int input, int heap) {
    Pair a, b;
    Ov45Work *chat;
    int offset;
    void *ui;
    int value, other, local, last, token;
    void *first;
    int sel;
    void *source;

    ui = &c->ui;
    chat = c->chat;
    offset = 0x2dc;
    switch (w->state) {
    case 0:
        if (w->init == 1) {
            w->state++;
        }
        break;
    case 1:
        BeginNormalPaletteFade(4, 0, 0, 0x7fff, 4, 1, heap);
        w->state++;
        break;
    case 2:
        if (IsPaletteFadeFinished() == 1) {
            ov49_0225B9AC(w, ui, c->config, heap, ov45_0222A5C0(chat));
            w->substate = 4;
            GfGfx_EngineBTogglePlanes(1, 1);
            GfGfx_EngineBTogglePlanes(2, 1);
            GfGfx_EngineBTogglePlanes(4, 1);
            GfGfx_EngineBTogglePlanes(16, 1);
            w->state++;
        }
        break;
    case 3:
        BeginNormalPaletteFade(4, 1, 1, 0x7fff, 6, 1, heap);
        w->state++;
        break;
    case 4:
        if (IsPaletteFadeFinished() == 1) {
            w->state++;
        }
        break;
    case 5:
        if (input != 1) {
            switch (w->substate) {
            case 0:
            case 4:
                break;
            case 1:
                if (ov49_0225C8D4(w->handler, ui, w->extra, heap) == 1) {
                    ov49_0225B444(c);
                }
                break;
            case 3:
                if (w->active) {
                    if (ov49_02268968(c->feature, w->response, w->byte) == 0) {
                        w->active = 0;
                        ov49_0225CB68(w->handler);
                    }
                }
                if (ov49_0225C8D4(w->handler, ui, w->extra, heap) == 1) {
                    ov45_0222ADD8(chat, ov45_0222A53C(chat));
                    ov45_0222AE54(chat);
                    ov45_0222AE08(ov45_0222ADA8(chat, ov45_0222A53C(chat)), &a.a, &a.b);
                    w->active = 1;
                    w->response = a.a;
                    w->byte = a.b;
                }
                break;
            case 2:
                ov49_0225BBCC(w->buf, (u8 *)c + (offset + 0x3c));
                offset += 0x3c;
                break;
            }
            if (w->pending) {
                w->state++;
            }
        }
        break;
    case 6:
        BeginNormalPaletteFade(4, 1, 0, 0, 3, 1, heap);
        w->state++;
        break;
    case 7:
        if (IsPaletteFadeFinished() != 1) {
            break;
        }
        switch (w->substate) {
        case 0:
            break;
        case 1:
        case 3:
        case 4:
            ov49_0225BA20(w, ui);
            break;
        case 2:
            ov49_0225B99C(w, c->pending, ui);
            break;
        }
        w->state++;
        break;
    case 8:
        switch (w->pending) {
        case 0:
            break;
        case 1:
        case 4:
            ov49_0225B9AC(w, ui, c->config, heap, ov45_0222A5C0(chat));
            break;
        case 2:
            first = ov45_0222A5C0(chat);
            sel = w->sel;
            if (sel == ov45_0222A53C(chat)) {
                source = ov45_0222A5C0(chat);
                local = 1;
            } else {
                source = ov45_0222A578(chat, sel);
                local = 0;
            }
            value = ov45_0222AB28(chat, w->sel);
            if (w->flag == 1) {
                other = ov45_0222AB48(chat, w->sel);
                last = ov45_0222AB58(chat, w->sel);
                if (last == 0) {
                    other = 0;
                }
            } else {
                other = 0;
                last = 0;
            }
            ov49_0225B944(w, c->pending, ui, (u8 *)c + offset, local, heap, source, first, value, other, last, 1);
            break;
        case 3:
            token = ov45_0222ADA8(chat, ov45_0222A53C(chat));
            if (token == -1) {
                GF_ASSERT(0);
                token = 0;
            }
            ov45_0222AE08(token, &b.a, &b.b);
            ov49_0225B9F0(w, ui, heap, b.b);
            break;
        }
        w->pending = 0;
        w->state++;
        break;
    case 9:
        BeginNormalPaletteFade(4, 1, 1, 0, 3, 1, heap);
        if (w->substate == 2) {
            ov49_0225BBCC(w->buf, c->pending);
        }
        w->state++;
        break;
    case 10:
        if (IsPaletteFadeFinished() == 1) {
            w->state = 5;
        }
        break;
    }
}
#pragma opt_propagation reset
