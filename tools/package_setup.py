#!/usr/bin/env python3
"""Builds the one-file setup program with PyInstaller.

The bundle is `git archive HEAD`: tracked source and extracted game data only, never a ROM,
mwccarm or the Nitro SDK (those are untracked and supplied by the user).
"""
import hashlib
import io
import os
import shutil
import subprocess
import sys
import tarfile

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
STAGE = os.path.join(ROOT, 'build-setup')


def main():
    shutil.rmtree(STAGE, ignore_errors=True)
    proj = os.path.join(STAGE, 'project')
    os.makedirs(proj)
    tar = subprocess.run(['git', 'archive', 'HEAD'], cwd=ROOT, check=True, stdout=subprocess.PIPE).stdout
    tarfile.open(fileobj=io.BytesIO(tar)).extractall(proj)
    ident = subprocess.run(['git', 'rev-parse', 'HEAD'], cwd=ROOT, check=True, stdout=subprocess.PIPE,
                           text=True).stdout.strip()
    open(os.path.join(STAGE, 'bundle.id'), 'w').write(hashlib.sha1(ident.encode()).hexdigest())
    sep = os.pathsep
    cmd = [sys.executable, '-m', 'PyInstaller', '--onefile', '--noconfirm', '--clean',
           '--name', 'pokeheartgold-single-screen-setup',
           '--distpath', os.path.join(ROOT, 'dist'), '--workpath', os.path.join(STAGE, 'work'),
           '--specpath', STAGE,
           '--add-data', os.path.join(proj) + sep + 'project',
           '--add-data', os.path.join(STAGE, 'bundle.id') + sep + '.',
           '--hidden-import', 'tkinter', '--hidden-import', 'zipfile']
    if sys.platform == 'win32':
        cmd.append('--windowed')
    cmd.append(os.path.join(ROOT, 'pokeheartgold_setup.py'))
    subprocess.run(cmd, check=True)


if __name__ == '__main__':
    main()
