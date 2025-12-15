from concurrent.futures import ThreadPoolExecutor, as_completed
from pathlib import Path
import os

from progress import ProgressBar
from util import sh, BUILD_DIR, ROOT, VERSION
from incremental import scan_deps, save_deps, needs_rebuild, load_state, save_state

def pkg_config(lib: str):
    cflags = sh(["pkg-config", "--cflags", lib]).split()
    libs = sh(["pkg-config", "--libs", lib]).split()
    return cflags, libs

def compile_one(compiler: str, src: Path, obj: Path, cflags: list[str], state: dict):
    deps = scan_deps(compiler, src, cflags)

    if not needs_rebuild(src, obj, deps, state):
        print(f"[skip] {src}")
        return

    obj.parent.mkdir(parents=True, exist_ok=True)
    cmd = [compiler, "-c", str(src), "-o", str(obj), *cflags]
    print(f"[cc] {src}")

    try:
        sh(cmd)
    except Exception as e:
        return e

    save_deps(state, src, deps)

def compile_all(sources: list[Path], dest: str, compiler="gcc", libs=[], cflags=[], ldflags=[], workers=os.cpu_count()):
    state = load_state()
    objects = []
    errors = []

    print(f"BananaBuild {VERSION}")
    print(f"Using {workers} workers")
    
    with ProgressBar(len(sources)) as progress:
        print("Resolving dependencies...")

        try:
            for lib in libs:
                lib_cflags, lib_ldflags = pkg_config(lib)
                cflags.extend(lib_cflags)
                ldflags.extend(lib_ldflags)
        except Exception as e:
            print("\033[31m" + str(e).strip())
            return 1

        print("Compiling...")

        with ThreadPoolExecutor(max_workers=workers) as pool:
            futures = []
            for src in sources:
                obj = BUILD_DIR / "obj" / (str(src.relative_to(ROOT)).replace("$", "$$").replace(os.path.sep, "$") + ".o")
                objects.append(obj)
                futures.append(
                    pool.submit(
                        compile_one,
                        compiler,
                        src,
                        obj,
                        cflags,
                        state,
                    )
                )

            for f in as_completed(futures):
                error = f.result()
                if error is not None:
                    errors.append(error)

                progress.update(text="(Failed)" if len(errors)>0 else None)

        save_state(state)

        for error in errors:
            print(error)

        if len(errors) > 0:
            print(f"Error in {len(errors)} file(s), exiting")
            return 1

        # link
        cmd = [compiler, *map(str, objects), "-o", ROOT / dest, *ldflags]
        print("[ld] Linking...")

        try:
            sh(cmd)
        except Exception as e:
            print("Link failed:")
            print(e)
            return 1

    print(f"Done. Executable at {ROOT / dest}")
    return 0
