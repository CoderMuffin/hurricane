from pathlib import Path
from util import ROOT, glob, bbmain, run
import shutil
from compile import compile_all

fxcgsdk = Path("C:\\Working\\PrizmSDK-win-0.6")
libfxcg = Path("C:\\Working\\libfxcg")
fxcg_cflags = [
    "-I", str(libfxcg/"include"),
    '-flto',
    '-mb',
    '-m4a-nofpu',
    '-mhitachi',
    '-nostdlib',
    '-ffunction-sections',
    '-fdata-sections',
    '-DTARGET_PRIZM=1',
    '-DNDEBUG',
    '-ffast-math',
    '-O3'
]
fxcg_ldflags = [
    "-L" + str(libfxcg/"libc"),
    "-L" + str(libfxcg/"libfxcg"),
    '-lc',
    "-lfxcg",
    '-lgcc',
    '-nostdlib',
    '-flto',
    '-Wl,-static',
    '-Wl,-gc-sections',
    '-T' + str(libfxcg/'toolchain'/'prizm.x')
]

def cg50main():
    executable = ROOT / "build/hc"
    
    compile_all(
        glob(ROOT, "*.c", exclude=[
            "example/src/example.c",
            "monster_maze.c",
            "maze.c",
            "hurricane/src/renderer/SDL.c",
            "hurricane/src/renderer/console.c",
            "hurricane/src/renderer/video.c",
            "hurricane/src/renderer/xlib.c",
            "hurricane/src/util/legacy_vec.c",
        ]),
        executable,
        compiler=str(fxcgsdk/"bin"/"sh3eb-elf-gcc.exe"),
        cflags=fxcg_cflags + [
            "-I", str(ROOT/"hurricane"/"include"),
            "-I", str(ROOT/"taylor_math"),
            
            "-Wall", "-Wpedantic", "-Werror",
            "-fdiagnostics-color=always"
        ],
        ldflags=fxcg_ldflags
    )
    run([fxcgsdk/"bin"/"mkg3a.exe", executable])
    shutil.copy(str(executable) + ".g3a", "E:\\hc.g3a")
    run('powershell $driveEject = New-Object -comObject Shell.Application; $driveEject.Namespace(17).ParseName("""E:""").InvokeVerb("""Eject""")', shell=True)


def main():
    executable = ROOT / "build/hc.exe"
    compile_all(
        glob(ROOT, "*.c", exclude=[
            "example/src/example.c",
            "monster_maze.c",
            "maze.c",
            "hurricane/src/renderer/prizm.c",
            "hurricane/src/renderer/console.c",
            "hurricane/src/renderer/video.c",
            "hurricane/src/renderer/xlib.c",
            "hurricane/src/util/legacy_vec.c",
        ]),
        executable,
        libs=["sdl2"],
        cflags=[
            "-I", str(ROOT / "hurricane/include"), 
            "-O3",
            "-flto",
            "-Wall", "-Wpedantic", "-Werror", "-fdiagnostics-color=always", "-g"
        ],
        ldflags=[
            "-lm",
            "-flto"
        ]
    )
    run([executable])

if __name__ == "__main__":
    bbmain(main)
