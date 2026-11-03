"""Exercise the production doubles compositor with synthetic pixels; no ROM."""
from pathlib import Path
import shutil
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[1]

class DoublesCompositorTests(unittest.TestCase):
    def test_four_bodies_and_joined_hp_rows_keep_their_game_coordinates(self):
        with tempfile.TemporaryDirectory() as directory:
            tmp = Path(directory)
            (tmp / 'battlewide.h').write_text((ROOT / 'frontend/battlewide.h').read_text())
            (tmp / 'merged.h').write_text(r'''
#pragma once
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdlib>
#include <cstdint>
#include <vector>
enum : uint8_t { LAYER_BG0=1,LAYER_BG1=2,LAYER_BG2=4,LAYER_BG3=8,LAYER_OBJ=16,LAYER_BACKDROP=32,LAYER_3D=64 };
inline uint8_t LayerClass(uint8_t f) { return f; }
struct ScreenImage { const uint32_t* pixels; const uint8_t* flags; };
struct WideImage { const uint32_t* pixels=nullptr; int w=0,h=0; bool placed=false; float baseX=0; };
''')
            (tmp / 'NDS.h').write_text(r'''
#pragma once
#include <cstdint>
#include <cstring>
namespace melonDS {
using u8=uint8_t; using u16=uint16_t; using u32=uint32_t;
namespace GPU2D {
struct Unit { u32 DispCnt=0; u16 BGCnt[4]={}; u8 WinCnt[3]={}; };
struct SoftRenderer {
 u8 PalSnap[2][0x400]={}, OAMSnap[2][1024]={}, ObjPrio[2][256*192]={};
 u16 LineBlend[2][192][3]={}, LineScroll[2][192][4][2]={};
};
}
struct GPUStub {
 GPU2D::Unit GPU2D_A,GPU2D_B; GPU2D::SoftRenderer renderer;
 u8 bg[0x80000]={},obj[0x40000]={};
 GPU2D::SoftRenderer& GetRenderer2D() { return renderer; }
 template<class T> T read(u8* p,u32 a,u32 mask) { T v; memcpy(&v,p+(a&mask),sizeof(v)); return v; }
 template<class T> T ReadVRAM_ABG(u32 a) { return read<T>(bg,a,0x7ffff); }
 template<class T> T ReadVRAM_BBG(u32 a) { return read<T>(bg,a,0x1ffff); }
 template<class T> T ReadVRAM_OBJ(u32 a) { return read<T>(obj,a,0x3ffff); }
};
struct NDS { GPUStub GPU; };
}
''')
            (tmp / 'GPU2D_Soft.h').write_text(r'''#include "NDS.h"
''')
            (tmp / "test.cpp").write_text(r'''
#include "battlewide.h"
#include <cassert>
#include <cstdio>
static melonDS::NDS nds;
static uint32_t pixels[256*192];
static uint8_t flags[256*192];
static void put16(uint8_t* dest,int at,uint16_t v) { memcpy(dest+at,&v,2); }
int main() {
 auto& gpu=nds.GPU;auto& r=gpu.renderer;
 gpu.GPU2D_A.DispCnt=0x10000|0x800|0x1000;
 gpu.GPU2D_A.BGCnt[3]=0x4000|0x1800|3;
 memset(gpu.bg+32,0x11,32);
 for(int i=0;i<2048;i++)put16(gpu.bg,0xC000+i*2,1);
 put16(r.PalSnap[0],2,0x03e0);
 for(int i=0;i<128;i++)put16(r.OAMSnap[0],i*8,0x200);

 for(int y=0;y<192;y++) for(int x=0;x<256;x++) {
  int i=y*256+x;
  bool mon=y>=32&&y<96&&((x>=20&&x<55)||(x>=65&&x<100)||(x>=155&&x<190)||(x>=200&&x<235));
  mon |= x>=108&&x<112&&y>=25&&y<29;
  bool hp=y<24||(y>=104&&y<128);
  pixels[i]=hp?0x0000fb:mon?0xfb0000:0x00fb00;
  flags[i]=hp?LAYER_OBJ:mon?LAYER_3D:LAYER_BG3;
  r.ObjPrio[0][i]=hp?0:4;
 }
 for(int extend: {24,43,64}) {
  BattleWide battle;WideImage out;
  for(int frame=0;frame<325;frame++) {
   assert(battle.Build(nds,{pixels,flags},extend,nullptr,256+2*extend,out,0, true));
   for(int y=0;y<192;y++) for(int x=0;x<256;x++)
    assert(out.pixels[y*out.w+x+extend]==(0xff000000|pixels[y*256+x]));
  }
 }
 return 0;
}
''')
            subprocess.run([shutil.which('clang++') or 'c++', '-std=c++17', '-Wall', '-Wextra',
                            '-Werror', str(tmp / 'test.cpp'), '-o', str(tmp / 'test')],
                           check=True, capture_output=True, text=True)
            result = subprocess.run([str(tmp / 'test')], capture_output=True, text=True)
            self.assertEqual(result.returncode, 0, result.stderr)

if __name__ == '__main__':
    unittest.main()
