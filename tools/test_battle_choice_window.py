"""Exercise the production widescreen compositor with synthetic VRAM, no ROM."""
from pathlib import Path
import shutil
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[1]


class BattleChoiceWindowTests(unittest.TestCase):
    def test_choice_layer_stays_complete_in_front_of_shifted_health_panels(self):
        source = (ROOT / 'frontend/battlewide.h').read_text()
        # Allow the unfixed interface to run too, so RED demonstrates pixel damage.
        extra = ', LAYER_BG1' if 'uint8_t fixedUiLayers' in source else ''
        with tempfile.TemporaryDirectory() as directory:
            tmp = Path(directory)
            (tmp / 'battlewide.h').write_text(source)
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
            (tmp / 'GPU2D_Soft.h').write_text('#include "NDS.h"\n')
            (tmp / 'test.cpp').write_text(r'''
#include "battlewide.h"
#include <cassert>
#include <cstdio>
static melonDS::NDS nds;
static uint32_t pixels[256*192];
static uint8_t flags[256*192];
static void put16(uint8_t* dest,int at,uint16_t v) { memcpy(dest+at,&v,2); }
int main() {
 auto& gpu=nds.GPU; auto& r=gpu.renderer;
 gpu.GPU2D_A.DispCnt=0x10000|0xE00|0x1000; // text layers 1,2,3 and OBJ
 gpu.GPU2D_A.BGCnt[1]=0x1000; // 256-wide dialog, map 0x8000, char 0
 gpu.GPU2D_A.BGCnt[3]=0x4000|0x1800|3; // 512-wide terrain, map 0xC000
 memset(gpu.bg+32,0x11,32); memset(gpu.bg+64,0x22,32);
 for(int i=0;i<2048;i++) put16(gpu.bg,0xC000+i*2,1);
 // Two separate UI windows on BG1: a choice and the question underneath.
 for(int ty=12;ty<17;ty++) for(int tx=14;tx<28;tx++) put16(gpu.bg,0x8000+(ty*32+tx)*2,2);
 for(int ty=19;ty<23;ty++) for(int tx=2;tx<29;tx++) put16(gpu.bg,0x8000+(ty*32+tx)*2,2);
 put16(r.PalSnap[0],2,0x03e0); put16(r.PalSnap[0],4,0x7fff);
 for(int i=0;i<128;i++) put16(r.OAMSnap[0],i*8,0x200);
 for(int y=0;y<192;y++) for(int x=0;x<256;x++) {
  int i=y*256+x;
  bool ui=(x>=112&&x<224&&y>=96&&y<136)||(x>=16&&x<232&&y>=152&&y<184);
  pixels[i]=ui?0xfbfbfb:0x00fb00; flags[i]=ui?LAYER_BG1:LAYER_BG3;
  // Simulate an HP panel covering part of the choice in the DS output.
  if(x>=160&&y>=104&&y<120) { pixels[i]=0x0000ff; flags[i]=LAYER_OBJ; }
 }
 for(int extend: {24,43,64,0}) {
  BattleWide battle; WideImage out;
  for(int i=0;i<325;i++) assert(battle.Build(nds,{pixels,flags},extend,nullptr,256+2*extend,out,0/*EXTRA*/));
  auto at=[&](int x,int y) { return out.pixels[y*out.w+x+extend]; };
  for(int y=96;y<136;y++) for(int x=112;x<224;x++) {
   if(at(x,y)!=0xfffbfbfb) { fprintf(stderr,"choice damaged at %d,%d: %08x\n",x,y,at(x,y)); return 1; }
  }
  assert(at(20,160)==0xfffbfbfb); // question stays intact too
  assert(at(110,110)!=0xfffbfbfb); // transparent UI tiles do not cover the battle
  assert(at(250+extend,110)==0xff0000ff); // health panel still moves to the edge
 }
 return 0;
}
'''.replace('/*EXTRA*/', extra))
            subprocess.run([shutil.which('clang++') or 'c++', '-std=c++17', '-Wall', '-Wextra',
                            '-Werror', str(tmp / 'test.cpp'), '-o', str(tmp / 'test')], check=True,
                           capture_output=True, text=True)
            result = subprocess.run([str(tmp / 'test')], capture_output=True, text=True)
            self.assertEqual(result.returncode, 0, result.stderr)


if __name__ == '__main__':
    unittest.main()
