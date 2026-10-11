#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
struct String;
extern "C" void ov45_02237A24(String *dst, const String *src);
extern "C" void ov45_02237B58(String *dst, const char *src);
extern "C" void ov45_02237D48(String *dst, const String *src);
extern "C" void ov45_02237BC8(String *s);

struct String {
    u32 w[3];

    String() {
        for (u32 i = 0; i < 3; i++) {
            w[i] = 0;
        }
    }

    String(const char *s) {
        ov45_02237B58(this, s);
    }

    String(const String &s) {
        ov45_02237A24(this, &s);
    }

    void destroy() {
        ov45_02237BC8(this);
    }

    String &operator=(const String &s) {
        ov45_02237D48(this, &s);
        return *this;
    }
};

extern "C" String ov45_02233ABC(const String &a, const char *b);
extern "C" String ov45_022339F4(const String &a, const String &b);

inline String operator+(const String &a, const char *b) {
    return ov45_02233ABC(a, b);
}

inline String operator+(const String &a, const String &b) {
    return ov45_022339F4(a, b);
}

struct Triple {
    u32 v[3];
};

struct NameTable {
    const char *names[12];
};

struct Obj {
    u32 unk0;
    u32 unk4;
    u32 unk8;
    String strC;
    String str18;
    String str24;
    String str30;
    String str3C;
    String str48;
    u32 unk54, unk58, unk5C, unk60;
    Triple unk64;
    u32 unk70, unk74, unk78;
    void *unk7C;
    u32 unk80, unk84, unk88;
    void *unk8C;
    u32 unk90, unk94;
    void *unk98;
    u32 unk9C, unkA0, unkA4;
    void *unkA8;
};

extern "C" NameTable ov45_02254DC4;
extern "C" char ov45_02254DBC[];
extern "C" char ov45_02255398[];
extern "C" Obj *ov45_0224268C(Obj *obj, const String &a1, const String &a2, int idx, u32 v8, const String &aC, const String &a18, const String &a24, const Triple *t);
#pragma opt_propagation off

extern "C" Obj *ov45_0224268C(Obj *obj, const String &a1, const String &a2, int idx, u32 v8, const String &aC, const String &a18, const String &a24, const Triple *t) {
    obj->unk0 = 0;
    obj->unk4 = 0;
    obj->unk8 = v8;
    ov45_02237A24(&obj->strC, &aC);
    ov45_02237A24(&obj->str18, &a18);
    ov45_02237A24(&obj->str24, &a24);
    ov45_02237A24(&obj->str30, &a1);
    ov45_02237A24(&obj->str3C, &a2);
    u32 *w = obj->str48.w;
    u32 i = 0;
    do {
        w[i] = 0;
        i++;
    } while (i < 3);
    obj->unk54 = 0;
    obj->unk58 = 0;
    obj->unk5C = 0;
    obj->unk60 = 0;
    obj->unk64 = *t;
    obj->unk70 = 0;
    obj->unk74 = 0;
    obj->unk7C = &obj->unk74;
    obj->unk80 = 0;
    obj->unk84 = 0;
    obj->unk8C = &obj->unk84;
    obj->unk90 = 0;
    obj->unk94 = 0;
    obj->unk98 = &obj->unk94;
    obj->unk9C = 0;
    obj->unkA0 = 0;
    obj->unkA8 = &obj->unkA0;
    NameTable names = ov45_02254DC4;
    String s48(ov45_02254DBC);
    String s3C = ov45_02233ABC(s48, ov45_02255398);
    String s30 = ov45_022339F4(s3C, obj->str30);
    String s24 = ov45_02233ABC(s30, ov45_02255398);
    String s18(names.names[idx]);
    String sC = ov45_022339F4(s24, s18);
    String r = ov45_02233ABC(sC, ov45_02255398);
    const String *pr = &r;
    ov45_02237D48(&obj->str48, pr);
    r.destroy();
    sC.destroy();
    s18.destroy();
    s24.destroy();
    s30.destroy();
    s3C.destroy();
    s48.destroy();
    return obj;
}

#pragma opt_propagation reset
