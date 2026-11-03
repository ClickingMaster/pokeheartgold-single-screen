"""Host-execute doubles positioning and target cards without the SDK or a ROM."""
import re
import struct
import unittest
from pathlib import Path
import subprocess
import tempfile

ROOT = Path(__file__).resolve().parents[1]
INPUT = 'src/battle/battle_input.c'
class game:
    @staticmethod
    def function_source(path, name):
        text = (ROOT / path).read_text()
        match = re.search(r'^[\w *]+\b' + re.escape(name) + r'\([^;{}]*\)\s*\{', text, re.M)
        assert match, name
        pos, depth = match.end(), 1
        while depth:
            depth += (text[pos] == '{') - (text[pos] == '}')
            pos += 1
        return text[match.start():pos]
    class GameLogicTests:
        def compile_run(self, source, *flags):
            with tempfile.TemporaryDirectory() as directory:
                folder = Path(directory)
                (folder / 'test.c').write_text(source)
                compiled = subprocess.run(['cc', '-std=c99', '-Wall', '-Wextra', '-Werror',
                                '-Wno-unused-parameter', '-iquote', str(ROOT / 'include'),
                                *flags, str(folder / 'test.c'), '-o', str(folder / 'test')],
                               capture_output=True, text=True)
                assert compiled.returncode == 0, compiled.stderr
                result = subprocess.run([str(folder / 'test')], capture_output=True, text=True)
                assert result.returncode == 0, result.stderr


def target_source():
    text = (ROOT / INPUT).read_text()
    item = re.search(r'typedef struct SingleScreenMenuItem \{.*?\} SingleScreenMenuItem;', text, re.S)[0]
    builder = game.function_source(INPUT, 'BattleInput_SingleScreen_BuildItems')
    target = builder[builder.index('    case SS_MENU_TARGET: {'):builder.index('    case SS_MENU_TWO_OPTIONS: {')]
    return r'''
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "constants/battle_menu.h"
typedef unsigned char u8;typedef unsigned short u16;typedef int BOOL;
#define TRUE 1
#define FALSE 0
#define SS_SELECTION_ALL 16
#define SS_MENU_TARGET 5
#define HEAP_ID_BATTLE 0
#define msg_0197_00964 964
typedef struct {int value;} String;
typedef int Pokemon;typedef int MsgData;typedef int MessageFormat;
typedef struct {int exists,selectedMon;} TargetMon;
typedef struct {TargetMon targetMons[4];} BattleInputTargetMenu;
typedef struct {int move;} BattleContext;
typedef struct {BattleContext ctx;} BattleSystem;
typedef struct {BattleSystem *battleSystem;int battlerType;struct {BattleInputTargetMenu target;} menu;} BattleInput;
static int allocations,group;
static String *String_New(int n,int h){allocations++;return calloc(1,sizeof(String));}
static void String_Delete(String *s){allocations--;free(s);}
static String *NewString_ReadMsgData(MsgData *m,int id){return String_New(24,0);}
static void BufferBoxMonNickname(MessageFormat *f,int n,Pokemon *p){}
static void StringExpandPlaceholders(MessageFormat *f,String *d,String *s){}
static Pokemon *Mon_GetBoxMon(Pokemon *p){return p;}
static Pokemon *BattleSystem_GetPartyMon(BattleSystem *s,int b,int n){static Pokemon p;return &p;}
static void BattleSystem_GetBattlerIdsByType(BattleSystem *s,u8 *ids){for(int i=0;i<4;i++)ids[2+i]=i;}
static void BattleInput_GetMonsHitArray(BattleInput *b,u8 *hits,int selected){
    for(int i=0;i<4;i++)hits[i]=selected?!!(group&(1<<i)):(i!=0);
}
static BattleContext *BattleSystem_GetBattleContext(BattleSystem *s){return &s->ctx;}
static int BattleSystem_GetBattlerFromBattlerType(BattleSystem *s,int t){return t;}
static int GetBattlerSelectedMove(BattleContext *c,int b){return c->move;}
static String *BattleInput_SingleScreen_Effectiveness(BattleInput *b,int m,int t){String *s=String_New(24,0);s->value=t;return s;}
''' + item + r'''
static int targetItems(BattleInput *battleInput,int selection,SingleScreenMenuItem *items,u8 *rect){
    MsgData *msgLoader=NULL;MessageFormat *format=NULL;int count=0,i;
    switch(SS_MENU_TARGET){
''' + target + r'''
    }return count;
}
'''


class DoublesLayoutTests(unittest.TestCase):
    compile_run = game.GameLogicTests.compile_run

    def test_target_cards_fit_below_hp_and_follow_battler_mapping(self):
        checks = r'''
int main(void){
    BattleSystem system={0};BattleInput b={.battleSystem=&system};
    for(int i=0;i<4;i++)b.menu.target.targetMons[i].exists=1;
    const int pos[][2]={{14,35},{134,3},{134,35},{14,3}};
    for(int selected=0;selected<=16;selected++){
        SingleScreenMenuItem items[20]={0};u8 rect[4]={0};group=10;
        int count=targetItems(&b,selected,items,rect),cards=0;
        assert(rect[0]==0&&rect[1]==16&&rect[2]==32&&rect[3]==8);
        for(int j=0;j<count;j++){
            if(items[j].button){
                int i=cards++;assert(items[j].x==pos[i][0]&&items[j].y==pos[i][1]);
                assert(items[j].width==108&&items[j].height==29);
                assert(items[j].x+items[j].width<=256&&items[j].y+items[j].height<=64);
                assert(items[j].grey==(i==0));
                assert(items[j].selected==(selected==16?!!(group&(1<<i)):selected==i));
            }else{
                assert(items[j].y>=17&&items[j].y+14<=64);
            }
            String_Delete(items[j].str);
        }assert(cards==4&&allocations==0);
    }return 0;
}
'''
        self.compile_run(target_source()+checks, '-Wno-unused-function')

    def test_move_home_and_particle_anchors_follow_the_same_vertical_shift(self):
        asm = (ROOT / 'asm/battle_anim_data.s').read_text()
        block = asm.split('gBattleAnimEmitterPositions:\n', 1)[1].split('\n\ng', 1)[0]
        data = bytes(int(value, 16) for line in block.splitlines() if '.byte' in line
                     for value in re.findall(r'0x([0-9A-Fa-f]{2})', line))
        numbers = struct.unpack('<576i', data)
        vectors = ['{' + ','.join(map(str, numbers[i:i+3])) + '}' for i in range(0, 576, 3)]
        values = ','.join('{' + ','.join(vectors[i:i+24]) + '}' for i in range(0, 192, 24))
        source = r'''
#include <assert.h>
#include <stddef.h>
typedef unsigned int u32;typedef short s16;typedef int fx32;
#define FX32_ONE 4096
typedef struct {s16 x,y;} BattleAnimPosition;
typedef struct {BattleAnimPosition positions[6];} BattleAnimBattlerTypePositions;
typedef struct {fx32 x,y,z;} VecFx32;
typedef struct {VecFx32 positions[8][24];} BattleAnimEmitterPositions;
static const s16 gBattlerSlideInTargetX[][2]={{64,32},{192,224},{40,32},{216,224},{80,32},{176,224}};
static const s16 gBattlerPokepicPositions[][3]={{336,112,-512},{-80,50,-640},{316,112,-512},{-60,50,-100},{356,120,0},{-100,42,-640}};
static const BattleAnimBattlerTypePositions gBattleAnimBattlerTypePositions={{{64,112},{192,48},{40,112},{216,50},{80,120},{176,42}}};
static const BattleAnimEmitterPositions gBattleAnimEmitterPositions={{''' + values + r'''}};
''' + game.function_source('src/battle/battle_anim_ball.c', 'BattleAnimUtil_GetBattlerTypeCoord') + '\n' + game.function_source('src/battle/battle_anim_battler_util.c', 'BattleAnimUtil_GetBattlerTypePosition') + '\n' + game.function_source('src/battle/battle_anim_battler_util.c', 'BattleAnimUtil_GetEmitterPosition') + r'''
int main(void){
    for(int t=0;t<6;t++){
        BattleAnimPosition p;BattleAnimUtil_GetBattlerTypePosition(t,0,&p);
        int delta=0;
#ifdef SINGLE_SCREEN
        if(t>=2){assert(p.y==64);delta=gBattleAnimBattlerTypePositions.positions[t].y-64;}
#else
        assert(p.y==gBattleAnimBattlerTypePositions.positions[t].y);
#endif
        assert(p.x==gBattleAnimBattlerTypePositions.positions[t].x);
        for(int perspective=0;perspective<2;perspective++)for(int kind=0;kind<12;kind++){
            VecFx32 out,old=gBattleAnimEmitterPositions.positions[t][perspective+kind*2];
            BattleAnimUtil_GetEmitterPosition(t,&out,0,perspective,kind);
            assert(out.x==old.x&&out.z==old.z);
            int depth=4*FX32_ONE-(perspective==0?old.z:0);
            assert(out.y==old.y+delta*depth/96);
            /* Project with the particle camera: the anchor moves by the same
             * number of pixels as its battler, preserving each kind's offset. */
            if(depth>0)assert(abs((out.y-old.y)*96-delta*depth)<=96);
        }
    }return 0;
}
'''
        source = source.replace('#include <stddef.h>', '#include <stddef.h>\n#include <stdlib.h>')
        for flags in ((), ('-DSINGLE_SCREEN',)):
            self.compile_run(source, *flags, '-Wno-missing-braces')

    def test_visible_target_touch_rectangles_keep_original_callback_order(self):
        text = (ROOT / INPUT).read_text()
        arrays = '\n'.join(re.search(r'static const [^;=]+\b' + name + r'\[\] = \{.*?\n\};', text, re.S)[0]
                           for name in ('sSingleScreenTargetTouchRects', 'sTouchArrayTargetMenu'))
        source = r'''
#include <assert.h>
#include "constants/battle_menu.h"
typedef unsigned char u8;typedef unsigned int u32;
#define TOUCHSCREEN_RECTLIST_END 255
#define TOUCH_MENU_NO_INPUT -1
typedef union {struct {u8 top,bottom,left,right;} rect;} TouchscreenHitbox;
''' + arrays + '\n' + game.function_source('src/touchscreen.c', 'TouchscreenHitbox_FindRectAtPoint') + r'''
int main(void){
    const int points[][3]={{68,145,4},{188,145,2},{68,177,1},{188,177,3}};
    for(int i=0;i<4;i++){
        int index=TouchscreenHitbox_FindRectAtPoint(sSingleScreenTargetTouchRects,points[i][0],points[i][1]);
        assert(index>=0&&sTouchArrayTargetMenu[index]==points[i][2]);
    }
    assert(TouchscreenHitbox_FindRectAtPoint(sSingleScreenTargetTouchRects,68,100)==-1);
    assert(TouchscreenHitbox_FindRectAtPoint(sSingleScreenTargetTouchRects,128,145)==-1);
    assert(sTouchArrayTargetMenu[4]==BATTLE_INPUT_CANCEL);
    return 0;
}
'''
        self.compile_run(source)

    def test_compact_cards_keep_enemy_numbers_private_and_stay_in_bounds(self):
        text = (ROOT / 'src/battle/battle_hp_bar.c').read_text()
        enum = re.search(r'typedef enum HPBarType \{.*?\} HPBarType;', text, re.S)[0]
        source = r'''
#include <assert.h>
#include <stdint.h>
#include <string.h>
typedef unsigned char u8;typedef unsigned short u16;typedef unsigned int u32;typedef int BOOL;
#define TRUE 1
#define FALSE 0
#define GF_ASSERT assert
#define NNS_G2D_VRAM_TYPE_2DMAIN 0
#define BATTLE_TYPE_TRAINER 1
typedef struct {struct {uintptr_t baseAddrOfVram[1];} vramLocation;} NNSG2dImageProxy;
typedef struct {int value;} Sprite;
typedef struct {Sprite *sprite;} ManagedSprite;
typedef struct {int type,showHpNumbers,status;void *battleSystem;ManagedSprite *boxObj;} BattleHpBar;
static struct {u16 before[16],pixels[0x800],after[16];} memory;
static int numbers,gauges;
static NNSG2dImageProxy proxy;
static void *G2_GetOBJCharPtr(void){return memory.pixels;}
static NNSG2dImageProxy *Sprite_GetImageProxy(Sprite *s){return &proxy;}
static void MI_CpuFill16(void *p,int color,int n){for(int i=0;i<n/2;i++)((u16*)p)[i]=color;}
static int BattleSystem_GetBattleType(void *s){return 0;}
static void BattleHpBar_InitHpChange(BattleHpBar *h,int n){}
static void BattleHpBar_InitExpChange(BattleHpBar *h,int n){}
static void BattleHpBar_StepGauge(BattleHpBar *h,int exp){assert(!exp);gauges++;}
static void BattleHpBar_DrawCurrentHp(BattleHpBar *h,int n){numbers++;}
static void BattleHpBar_DrawMaxHp(BattleHpBar *h){numbers++;}
static void BattleHpBar_DrawGenderIcon(BattleHpBar *h){}
static void BattleHpBar_DrawLevel(BattleHpBar *h){}
static void BattleHpBar_DrawNickname(BattleHpBar *h){}
static void BattleHpBar_DrawCaughtIcon(BattleHpBar *h){}
static void BattleHpBar_DrawStatusIcon(BattleHpBar *h,int n){}
static void BattleHpBar_PrintSafariOrParkBallsString(BattleHpBar *h,int n){}
static void BattleHpBar_PrintNumRemainingSafariOrParkBalls(BattleHpBar *h,int n){}
''' + enum + '\n' + '\n'.join(game.function_source('src/battle/battle_hp_bar.c', name) for name in (
            'BattleHpBar_IsCompact', 'BattleHpBar_CompactVram', 'BattleHpBar_CompactPutPixel',
            'BattleHpBar_CompactInitialize', 'BattleHpBar_Draw')) + r'''
int main(void){
    ManagedSprite sprite={0};
    for(int type=0;type<8;type++)assert(BattleHpBar_IsCompact(type)==(type>=2&&type<=5));
    for(int type=2;type<=5;type++){
        memset(&memory,0xa5,sizeof(memory));BattleHpBar_CompactInitialize(&sprite,type);
        for(int i=0;i<16;i++)assert(memory.before[i]==0xa5a5&&memory.after[i]==0xa5a5);
        int opaque=0;u8 *data=(u8*)memory.pixels;
        for(int x=0;x<128;x++)for(int y=0;y<64;y++){
            int address=x/64*0x800+((y/8)*8+x%64/8)*32+y%8*4+x%8/2;
            int pixel=(data[address]>>((x&1)*4))&15;
            assert((pixel!=0)==(y>=16&&y<40));opaque+=pixel!=0;
            if(type&1 && y>=32&&y<39&&x>=72)assert(pixel==15);
        }assert(opaque==128*24);
        BattleHpBar hp={.type=type,.boxObj=&sprite};numbers=gauges=0;
        BattleHpBar_Draw(&hp,100,7);assert(gauges==1&&numbers==((type&1)?0:2));
    }return 0;
}
'''
        self.compile_run(source, '-DSINGLE_SCREEN', '-Wno-unused-function', '-Wno-unused-variable',
                         '-fsanitize=address,undefined', '-fno-omit-frame-pointer')

    def test_doubles_bodies_and_ball_anchors_share_clear_arena(self):
        source = r'''
#include <assert.h>
#include <stddef.h>
typedef unsigned int u32;typedef short s16;
static const s16 gBattlerSlideInTargetX[][2]={{64,32},{192,224},{40,32},{216,224},{80,32},{176,224}};
static const s16 gBattlerPokepicPositions[][3]={{336,112,-512},{-80,50,-640},{316,112,-512},{-60,50,-100},{356,120,0},{-100,42,-640}};
static const s16 gBattleAnimBattlerScreenPositions[2][12]={{64,112,192,48,216,112,80,42},{64,112,192,48,40,112,216,50,80,120,176,42}};
static const s16 gBattleAnimBattlerScreenPositionsY[2][12]={{112,192,48,216,112,80,42},{112,192,48,40,112,216,50,80,120,176,42}};
''' + game.function_source('src/battle/battle_anim_ball.c', 'BattleAnimUtil_GetBattlerTypeCoord') + '\n' + game.function_source('src/battle/battle_anim_emitter.c', 'BattleAnimUtil_LookupScreenPos') + r'''
int main(void){
    for(int i=0;i<6;i++){
        int expected=i>=2?64:gBattlerPokepicPositions[i][1];
#ifndef SINGLE_SCREEN
        expected=gBattlerPokepicPositions[i][1];
#endif
        assert(BattleAnimUtil_GetBattlerTypeCoord(i,1)==expected);
        assert(BattleAnimUtil_GetBattlerTypeCoord(i,0)==gBattlerSlideInTargetX[i][0]);
        assert(BattleAnimUtil_GetBattlerTypeCoord(i,2)==gBattlerPokepicPositions[i][2]);
        if(i>=2){s16 x,y;BattleAnimUtil_LookupScreenPos(1,i,&x,&y);
            assert(x==gBattleAnimBattlerScreenPositions[1][i*2]);assert(y==expected);
        }
    }return 0;
}
'''
        for flags in ((), ('-DSINGLE_SCREEN',)):
            self.compile_run(source, *flags)


if __name__ == '__main__':
    unittest.main()
