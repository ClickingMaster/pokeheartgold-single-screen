#!/usr/bin/env python3
"""Setup program for the single-screen Pokemon SoulSilver decompilation.

It carries only the decomp's tracked source (no ROM, no compiler, no SDK, and not the full files/ and lib/ trees). You supply the
Metrowerks mwccarm compiler and the Nitro SDK tools yourself (see INSTALL.md); this
program places them, checks the build tools, builds the single-screen ROM and copies it
to a folder of your choice.

    python3 pokeheartgold_setup.py                 # window
    python3 pokeheartgold_setup.py --mwcc DIR_OR_ZIP --sdk DIR_OR_ZIP [--save FILE.sav] [--out DIR] [--jobs N]
"""
import argparse
import hashlib
import os
import queue
import shutil
import subprocess
import sys
import tempfile
import threading
import zipfile

NAME = 'pokeheartgold-single-screen'
ROM_REL = os.path.join('build', 'soulsilver.us', 'pokesoulsilver.us.nds')
OUT_NAME = 'SoulSilver-single-screen.nds'
LIBS = ['NitroSDK', 'NitroSystem', 'NitroDWC', 'NitroWiFi', 'MSL_C', 'libCPS', 'libVCT', 'asm', 'src', 'include']
SAVE_SIZE = 512 * 1024
DSV_FOOTER = 122
TEMPLATES = {'ARM9-TS.lcf.template': '.', 'mwldarm.response.template': '.', 'ARM7-TS.lcf.template': 'sub'}
HERE = os.path.dirname(os.path.abspath(sys.executable if getattr(sys, 'frozen', False) else __file__))


def bundle_dir():
    base = getattr(sys, '_MEIPASS', None)
    return os.path.join(base, 'project') if base else HERE


def work_dir():
    if os.path.exists(os.path.join(HERE, 'portable.txt')):
        return os.path.join(HERE, NAME)
    if sys.platform == 'win32':
        root = os.environ.get('LOCALAPPDATA') or os.path.expanduser('~')
    else:
        root = os.environ.get('XDG_DATA_HOME') or os.path.expanduser('~/.local/share')
    return os.path.join(root, NAME)


def bundle_id():
    base = getattr(sys, '_MEIPASS', None)
    try:
        return open(os.path.join(base, 'bundle.id')).read().strip() if base else 'source'
    except OSError:
        return 'source'


def unpack(src, log):
    """Copies the bundled source into the work folder, keeping build output and your tools."""
    marker = os.path.join(src, '.bundle-id')
    if getattr(sys, '_MEIPASS', None) is None:
        return os.path.dirname(os.path.abspath(__file__))
    if os.path.exists(marker) and open(marker).read() == bundle_id():
        return src
    log('Unpacking the source to %s\n' % src)
    for base, dirs, files in os.walk(bundle_dir()):
        rel = os.path.relpath(base, bundle_dir())
        os.makedirs(os.path.join(src, rel), exist_ok=True)
        for f in files:
            shutil.copy2(os.path.join(base, f), os.path.join(src, rel, f))
    open(marker, 'w').write(bundle_id())
    return src


def materialise(path, scratch):
    """A folder is used as is; a zip is extracted into scratch."""
    if os.path.isdir(path):
        return path
    if zipfile.is_zipfile(path):
        zipfile.ZipFile(path).extractall(scratch)
        return scratch
    raise SystemExit('Not a folder or zip: %s' % path)


def find_dir(root, wanted):
    for base, dirs, _ in os.walk(root):
        if os.path.basename(base) == wanted:
            return base
    return None


def find_file(root, name):
    for base, _, files in os.walk(root):
        if name in files:
            return os.path.join(base, name)
    return None


def place_toolchain(src, mwcc, sdk, log):
    with tempfile.TemporaryDirectory(prefix='phg-tools-') as tmp:
        if mwcc:
            root = materialise(mwcc, os.path.join(tmp, 'mwcc'))
            d = find_dir(root, 'mwccarm')
            if not d or not find_file(d, 'mwccarm.exe'):
                raise SystemExit('No mwccarm folder (with 2.0/sp2p2/mwccarm.exe) in %s' % mwcc)
            dest = os.path.join(src, 'tools', 'mwccarm')
            if os.path.realpath(d) == os.path.realpath(dest):
                raise SystemExit('That is already the installed mwccarm folder; choose your original copy.')
            shutil.rmtree(dest, ignore_errors=True)
            shutil.copytree(d, dest)
            lic = find_file(root, 'license.dat')
            if lic and not os.path.exists(os.path.join(dest, 'license.dat')):
                shutil.copy2(lic, dest)
            log('Placed mwccarm.\n')
        if sdk:
            root = materialise(sdk, os.path.join(tmp, 'sdk'))
            makelcf = find_file(root, 'makelcf.exe')
            if not makelcf:
                raise SystemExit('No tools/bin (with makelcf.exe) in %s' % sdk)
            dest = os.path.join(src, 'tools', 'bin')
            os.makedirs(dest, exist_ok=True)
            shutil.copytree(os.path.dirname(makelcf), dest, dirs_exist_ok=True)
            for name, where in TEMPLATES.items():
                f = find_file(root, name)
                if f:
                    shutil.copy2(f, os.path.join(src, where, name))
            log('Placed the Nitro SDK tools.\n')
            nsdk = find_dir(root, 'NitroSDK')
            if nsdk:
                libroot = os.path.dirname(nsdk)
                for name in LIBS:
                    if os.path.isdir(os.path.join(libroot, name)):
                        shutil.copytree(os.path.join(libroot, name), os.path.join(src, 'lib', name), dirs_exist_ok=True)
                log('Placed the Nitro library sources.\n')


def fetch_assets(src, rom, log):
    sys.path.insert(0, os.path.join(src, 'tools'))
    import fetch_assets as fa
    import contextlib
    class W:
        def write(self, t): log(t)
        def flush(self): pass
    with contextlib.redirect_stdout(W()):
        fa.fetch_pret(None, False)
        if rom:
            fa.fetch_rom(rom, False)


def check_ready(src):
    problems = []
    if not os.path.isdir(os.path.join(src, 'files', 'data')):
        problems.append('The files/ asset tree is missing or incomplete. Only part of it is in this repo; '
                        'see INSTALL.md.')
    if not os.path.isdir(os.path.join(src, 'lib', 'NitroSDK')):
        problems.append('The Nitro library sources (lib/NitroSDK, NitroSystem, ... see INSTALL.md) are missing.')
    if not os.path.exists(os.path.join(src, 'tools', 'mwccarm', '2.0', 'sp2p2', 'mwccarm.exe')):
        problems.append('mwccarm is missing (choose it in step 1).')
    if not os.path.exists(os.path.join(src, 'tools', 'bin', 'makelcf.exe')):
        problems.append('The Nitro SDK tools are missing (choose them in step 2).')
    for name, where in TEMPLATES.items():
        if not os.path.exists(os.path.join(src, where, name)):
            problems.append('%s is missing; it comes with the Nitro SDK.' % name)
    need = ['make', 'python3' if sys.platform != 'win32' else 'python', 'pkg-config', 'arm-none-eabi-gcc']
    if sys.platform != 'win32':
        need.append('wine')
    for tool in need:
        if not shutil.which(tool, path=build_env()['PATH']):
            problems.append('Tool not found: %s. %s' % (tool, install_hint()))
    return problems


def msys_bin():
    for root in (r'C:\msys64', r'C:\msys32', os.path.expanduser(r'~\msys64')):
        if os.path.exists(os.path.join(root, 'usr', 'bin', 'bash.exe')):
            return root
    return None


def build_env():
    env = dict(os.environ)
    if sys.platform == 'win32':
        root = msys_bin()
        extra = [os.path.join(root, 'usr', 'bin'), os.path.join(root, 'mingw64', 'bin')] if root else []
        dk = env.get('DEVKITARM') or r'C:\devkitPro\devkitARM'
        extra.append(os.path.join(dk, 'bin'))
        env['PATH'] = os.pathsep.join(extra + [env.get('PATH', '')])
    return env


def install_hint():
    if sys.platform == 'win32':
        return 'Install MSYS2 (make, git, python, pkg-config, libpng, pugixml) and devkitARM; see INSTALL.md.'
    if sys.platform == 'darwin':
        return 'See INSTALL.md (Homebrew: coreutils make gnu-sed arm-gcc-bin libpng pkg-config pugixml wine-crossover).'
    return ('Linux: install make, g++, binutils-arm-none-eabi (arm-none-eabi-gcc), wine, python3, '
            'pkg-config, libpng and pugixml development packages; see INSTALL.md.')


def run_build(src, jobs, out, log):
    cmd = ['make', '-j%d' % jobs, 'soulsilver', 'COMPARE=0']
    env = build_env()
    log('Building (this takes a while): %s\n' % ' '.join(cmd))
    p = subprocess.Popen(cmd, cwd=src, env=env, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True,
                         errors='replace')
    for line in p.stdout:
        log(line)
    if p.wait() != 0:
        raise SystemExit('The build failed; the log above has the first error.')
    rom = os.path.join(src, ROM_REL)
    os.makedirs(out, exist_ok=True)
    dest = os.path.join(out, OUT_NAME)
    shutil.copy2(rom, dest)
    sha = hashlib.sha1(open(dest, 'rb').read()).hexdigest()
    log('Done: %s\nSHA-1 %s\nPlay it with the ssrecomp frontend (single-screen builds are tagged '
        'SSRECOMP-SINGLE-SCREEN-1).\n' % (dest, sha))
    return dest


def import_save(sav, rom, log):
    """Copies an existing save next to the built ROM, where the frontend looks for it (<rom>.sav)."""
    data = open(sav, 'rb').read()
    if len(data) == SAVE_SIZE + DSV_FOOTER:
        data = data[:SAVE_SIZE]  # DeSmuME .dsv: raw save plus a footer
    if len(data) != SAVE_SIZE:
        raise SystemExit('Not a SoulSilver save (%d bytes, expected %d): %s' % (len(data), SAVE_SIZE, sav))
    dest = rom + '.sav'
    if os.path.exists(dest):
        shutil.copy2(dest, dest + '.bak')
        log('Existing save kept as %s.bak\n' % dest)
    with open(dest, 'wb') as f:
        f.write(data)
    log('Imported save: %s\n' % dest)


def default_jobs():
    return max(1, min(8, (os.cpu_count() or 2)))


def cli(args):
    src = unpack(work_dir(), sys.stdout.write)
    place_toolchain(src, args.mwcc, args.sdk, sys.stdout.write)
    fetch_assets(src, args.rom, sys.stdout.write)
    problems = check_ready(src)
    if problems:
        raise SystemExit('Not ready:\n  ' + '\n  '.join(problems))
    rom = run_build(src, args.jobs, args.out, sys.stdout.write)
    if args.save:
        import_save(args.save, rom, sys.stdout.write)


def gui(args):
    import tkinter as tk
    from tkinter import filedialog, scrolledtext, ttk
    root = tk.Tk()
    root.title('Pokemon SoulSilver single-screen setup')
    root.geometry('720x520')
    q = queue.Queue()
    v = {k: tk.StringVar(value=getattr(args, k) or '') for k in ('mwcc', 'sdk', 'rom', 'save', 'out')}
    if not v['out'].get():
        v['out'].set(os.path.expanduser('~'))
    f = ttk.Frame(root, padding=12)
    f.pack(fill='both', expand=True)
    rows = [('1. mwccarm (folder or zip)', 'mwcc', True), ('2. Nitro SDK tools (folder or zip)', 'sdk', True),
            ('3. Your SoulSilver (USA) .nds', 'rom', True),
            ('4. Existing save to import (optional .sav)', 'save', True),
            ('5. Save the ROM in', 'out', False)]
    for i, (label, key, pick_file) in enumerate(rows):
        ttk.Label(f, text=label).grid(row=i, column=0, sticky='w', pady=3)
        ttk.Entry(f, textvariable=v[key], width=48).grid(row=i, column=1, padx=6)
        def browse(key=key, pick_file=pick_file):
            p = (filedialog.askopenfilename() or filedialog.askdirectory()) if pick_file else filedialog.askdirectory()
            if p:
                v[key].set(p)
        ttk.Button(f, text='Browse', command=browse).grid(row=i, column=2)
    ttk.Label(f, text='You supply the compiler and SDK; no ROM, compiler or Nintendo SDK is bundled, game files are extracted from your own ROM, and the Nitro library sources in lib/ still come from your own copy (see INSTALL.md).',
              wraplength=680).grid(row=5, column=0, columnspan=3, sticky='w', pady=6)
    logbox = scrolledtext.ScrolledText(f, height=18, state='disabled')
    logbox.grid(row=7, column=0, columnspan=3, sticky='nsew')
    f.rowconfigure(7, weight=1)
    f.columnconfigure(1, weight=1)

    def log(text):
        q.put(text)

    def pump():
        try:
            while True:
                logbox.configure(state='normal')
                logbox.insert('end', q.get_nowait())
                logbox.see('end')
                logbox.configure(state='disabled')
        except queue.Empty:
            pass
        root.after(100, pump)

    def work(mwcc, sdk, out, rom, sav):
        try:
            src = unpack(work_dir(), log)
            place_toolchain(src, mwcc or None, sdk or None, log)
            fetch_assets(src, rom or None, log)
            problems = check_ready(src)
            if problems:
                raise SystemExit('Not ready:\n  ' + '\n  '.join(problems))
            dest = run_build(src, args.jobs, out, log)
            if sav:
                import_save(sav, dest, log)
        except SystemExit as e:
            log('\n%s\n' % e)
        except Exception as e:
            log('\nError: %s\n' % e)
        finally:
            root.after(0, lambda: button.configure(state='normal'))

    def start():
        button.configure(state='disabled')
        threading.Thread(target=work, args=(v['mwcc'].get(), v['sdk'].get(), v['out'].get(), v['rom'].get(), v['save'].get()), daemon=True).start()

    button = ttk.Button(f, text='Build', command=start)
    button.grid(row=6, column=0, columnspan=3, pady=6)
    pump()
    root.mainloop()


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--mwcc', help='folder or zip holding mwccarm/ (with 2.0/sp2p2/mwccarm.exe)')
    ap.add_argument('--sdk', help='folder or zip holding the Nitro SDK tools/bin, the .template files and (next to each other) the library sources NitroSDK, NitroSystem, ...')
    ap.add_argument('--rom', help='your own SoulSilver (USA) .nds, used to extract the raw game files')
    ap.add_argument('--save', help='an existing SoulSilver .sav (or DeSmuME .dsv) to copy next to the built ROM')
    ap.add_argument('--out', default=os.getcwd(), help='folder for %s' % OUT_NAME)
    ap.add_argument('--jobs', type=int, default=default_jobs())
    args = ap.parse_args()
    if len(sys.argv) > 1:
        cli(args)
    else:
        gui(args)


if __name__ == '__main__':
    main()
