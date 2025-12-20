from pathlib import Path
from bananabuild.util import ROOT, glob, bbmain, run
import shutil
from bananabuild.compile import compile_all

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

import win32file
import win32con

def eject_drive(drive_letter):
    if not drive_letter.endswith(":"):
        drive_letter += ":"
    
    handle = win32file.CreateFile(
        f"\\\\.\\{drive_letter}",
        win32con.GENERIC_READ,
        win32con.FILE_SHARE_READ | win32con.FILE_SHARE_WRITE,
        None,
        win32con.OPEN_EXISTING,
        0,
        None
    )
    
    try:
        win32file.DeviceIoControl(handle, 0x2D4808, None, None)
        print(f"{drive_letter} ejected successfully.")
    except Exception as e:
        print(f"Failed to eject {drive_letter}: {e}")
    finally:
        handle.close()

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
    eject_drive("E")


def main():
    executable = ROOT / "build/hc.exe"
    compile_all(
        glob(ROOT, "*.cpp", exclude=[
            "example/src/example.*",
            "monster_maze.*",
            "maze.*",
            "hurricane/src/renderer/prizm.*",
            "hurricane/src/renderer/console.*",
            "hurricane/src/renderer/video.*",
            "hurricane/src/renderer/xlib.*",
            "hurricane/src/util/legacy_vec.*",
        ]),
        executable,
        libs=["sdl2"],
        cflags=[
            "-I", str(ROOT / "hurricane/include"), 
            "-I", str(ROOT / "taylor_math"), 
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
