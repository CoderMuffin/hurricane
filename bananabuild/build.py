from util import ROOT, glob
from compile import compile_all

def main():
    compile_all(
        glob(ROOT, "*.c", exclude=["monster_maze.c", "hurricane/src/renderer/console.c", "hurricane/src/renderer/video.c", "hurricane/src/renderer/xlib.c", "hurricane/src/util/legacy_vec.c"]),
        "hurricane",
        libs=["sdl2"],
        cflags=["-I", str(ROOT / "hurricane/include"), "-Wall", "-Wpedantic", "-Werror", "-fdiagnostics-color=always", "-g"]
    )

if __name__ == "__main__":
    main()
