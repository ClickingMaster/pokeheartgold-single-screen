"""Render real CPU/SDL compositor output; no ROM or display server required."""
from pathlib import Path
import shlex
import shutil
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[1]


class WideEdgeSpriteTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        if not shutil.which('pkg-config'):
            raise unittest.SkipTest('SDL2 development files and pkg-config required')
        flags = subprocess.run(['pkg-config', '--cflags', '--libs', 'sdl2'], capture_output=True, text=True)
        if flags.returncode:
            raise unittest.SkipTest('SDL2 development files required')
        cls.directory = tempfile.TemporaryDirectory()
        cls.addClassCleanup(cls.directory.cleanup)
        tmp = Path(cls.directory.name)
        source = tmp / 'edge.cpp'
        source.write_text(r'''
#include "gpucompose.h"
#include <cassert>
#include <cstdio>
#include <string>
static constexpr int W=640,H=384;
static constexpr uint32_t terrain=0xff224466, rain=0xffd0e8fc, box=0xffb8b8b8;
int main(int argc,char** argv) {
 bool gpu=argc>1 && std::string(argv[1])=="gpu";
 std::vector<uint32_t> top(256*192,terrain),worldPixels(W*H,terrain),out;
 std::vector<uint8_t> flags(256*192,0x5f);
 ScreenImage screens[2]={{top.data(),flags.data()},{top.data(),flags.data()}};
 WideImage world;world.pixels=worldPixels.data();world.w=W;world.h=H;
 Scene scene;scene.name="field";scene.wide=true;scene.items={FullScreen(0)};
 // Rain crossing either DS edge and a particle still within the DS screen.
 top[50*256]=rain;flags[50*256]=0x80;
 top[70*256+255]=rain;flags[70*256+255]=0x90;
 top[60*256+128]=rain;flags[60*256+128]=0x10;
 // A real background cover must still extend to the view's edges.
 for(int x=0;x<256;x++) { top[150*256+x]=box;flags[150*256+x]=LAYER_BG3; }
 SDL_Surface* surface=nullptr;SDL_Renderer* renderer=nullptr;
 if(gpu) {
  assert(SDL_Init(0)==0);
  surface=SDL_CreateRGBSurfaceWithFormat(0,W,H,32,SDL_PIXELFORMAT_ARGB8888);assert(surface);
  renderer=SDL_CreateSoftwareRenderer(surface);assert(renderer);
 }
 auto render=[&]() {
  if(gpu) {
   GpuCompositor compositor(renderer);compositor.Draw(scene,screens,W,H,false,&world);
   out.resize(W*H);assert(SDL_RenderReadPixels(renderer,nullptr,SDL_PIXELFORMAT_ARGB8888,out.data(),W*4)==0);
  } else MergedCompose(scene,screens,W,H,out,false,&world);
 };
 render();
 auto expect=[&](int x,int y,uint32_t color,const char* reason) {
  if(out[y*W+x]!=color) { fprintf(stderr,"%s (%s) at %d,%d: %08x != %08x\n",reason,gpu?"gpu":"cpu",x,y,out[y*W+x],color);return false; }return true;
 };
 bool good=true;
 good &= expect(0,101,terrain,"rain stretched into left margin");
 good &= expect(639,141,terrain,"rain stretched into right margin");
 good &= expect(64,101,rain,"source left raindrop lost");
 good &= expect(575,141,rain,"source right raindrop lost");
 good &= expect(320,121,rain,"interior raindrop lost");
 good &= expect(0,301,box,"background cover lost at left");
 good &= expect(639,301,box,"background cover lost at right");
 // A deliberately hidden UI layer must not reappear in the added margins.
 scene.items[0].drop=LAYER_BG3;render();
 good &= expect(0,301,terrain,"dropped layer reappeared at left");
 good &= expect(639,301,terrain,"dropped layer reappeared at right");
 if(gpu) { SDL_DestroyRenderer(renderer);SDL_FreeSurface(surface);SDL_Quit(); }
 return good?0:1;
}
''')
        cls.executable = tmp / 'edge'
        command = [shutil.which('clang++') or 'c++', '-std=c++17', '-Wall', '-Wextra', '-Werror',
                   '-I', str(ROOT / 'frontend'), str(source), '-o', str(cls.executable), *shlex.split(flags.stdout)]
        subprocess.run(command, check=True, capture_output=True, text=True)

    def run_renderer(self, mode):
        result = subprocess.run([str(self.executable), mode], capture_output=True, text=True)
        self.assertEqual(result.returncode, 0, result.stderr)

    def test_cpu_keeps_edge_particles_local(self):
        self.run_renderer('cpu')

    def test_sdl_keeps_edge_particles_local(self):
        self.run_renderer('gpu')


if __name__ == '__main__':
    unittest.main()
