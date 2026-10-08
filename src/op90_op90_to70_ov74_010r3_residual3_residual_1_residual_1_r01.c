#include "global.h"
#include "bg_window.h"
#pragma force_active on
extern u32 AGB_GetBoxMonData(void *,int,void *);
void ov74_02232700(u8 *,void *);
extern void ov74_02231A1C(void *,void *,int);
extern void ConvertRSStringToDPStringInternational(u8 *,u16 *,int,u32);
extern void *String_New(int,int),*NewMsgDataFromNarc(int,int,int,int);
extern u32 TranslateAgbSpecies(u32),UpConvertItemId_Gen3to4(u16);
extern void ReadMsgDataIntoString(void *,int,void *),DestroyMsgData(void *),String_Delete(void *);
extern void GetItemNameIntoString(void *,u16,int),String16_FormatInteger(void *,int,int,int,int),PlayCry(u16,int);
extern int ov74_02234A0C(void *);
typedef struct { void *window; u32 unk4; u32 unk8; u32 unkC; u32 width; u32 fontId; u32 x; u32 y; u32 color; u32 unk24; u32 unk28; u32 attr; u32 unk30; int msgId; const u16 *raw; void *string; u32 unk40; u32 unk44; u32 unk48; } TextParams;
void ov74_02232758(u8 *,void *);


// @fn ov74_02232758
void ov74_02232758(u8 *p, void *mon) {
    TextParams params = {0};
    struct {
        u16 name[11];
        u8 agbname[11];
    } text;
    int species;
    void *str, *msg;

    params.window = p + 0x488;
    params.unk8 = 0;
    params.unkC = 0;
    params.fontId = 4;
    params.width = 32;
    params.unk24 = 1;
    params.unk28 = 1;
    params.color = 188;
    params.attr = 0xf0200;
    params.msgId = 44;
    params.x = 144;
    params.raw = NULL;
    params.y = 0;
    ov74_02231A1C(p, &params, 4);
    params.msgId = 43;
    params.x = 80;
    params.y = 8;
    ov74_02231A1C(p, &params, 6);
    ov74_02232700(p, mon);
    if (!mon) {
        CopyWindowToVram(params.window);
        return;
    }
    AGB_GetBoxMonData(mon, 2, text.agbname);
    ConvertRSStringToDPStringInternational(text.agbname, text.name, 11, AGB_GetBoxMonData(mon, 3, NULL));
    params.raw = text.name;
    params.msgId = -1;
    params.x = 8;
    params.y = 0;
    ov74_02231A1C(p, &params, 6);
    str = String_New(64, 0x4c);
    msg = NewMsgDataFromNarc(1, 27, 0xed, 0x4c);
    species = TranslateAgbSpecies(AGB_GetBoxMonData(mon, 11, NULL));
    ReadMsgDataIntoString(msg, species, str);
    params.msgId = -1;
    params.x = 16;
    params.y = 16;
    params.string = str;
    ov74_02231A1C(p, &params, 6);
    DestroyMsgData(msg);
    String_Delete(str);
    {
        int item = AGB_GetBoxMonData(mon, 12, NULL);
        if (item) {
            int converted = UpConvertItemId_Gen3to4((u16)item);
            str = String_New(64, 0x4c);
            GetItemNameIntoString(str, (u16)converted, 0x4c);
            params.msgId = -1;
            params.x = 152;
            params.y = 16;
            params.string = str;
            ov74_02231A1C(p, &params, 6);
            String_Delete(str);
        }
    }
    {
        int level = ov74_02234A0C(mon);
        str = String_New(10, 0x4c);
        String16_FormatInteger(str, level, 3, 1, 1);
        params.msgId = -1;
        params.x = 100;
        params.y = 8;
        params.string = str;
        ov74_02231A1C(p, &params, 2);
        String_Delete(str);
    }
    PlayCry((u16)species, 0);
}
