from concurrent.futures import ThreadPoolExecutor, as_completed
from pathlib import Path
import os

from progress import ProgressBar
from util import run, BUILD_DIR, ROOT
from incremental import scan_deps, save_deps, needs_rebuild, load_state, save_state

def pkg_config(lib: str):
    cflags = run(["pkg-config", "--cflags", lib], silent=True).split()
    libs = run(["pkg-config", "--libs", lib], silent=True).split()
    return cflags, libs

def compile_one(compiler: str, src: Path, obj: Path, cflags: list[str], state: dict):
    deps = scan_deps(compiler, src, cflags)

    if not needs_rebuild(src, obj, deps, state):
        print(f"[skip] {src}")
        return

    obj.parent.mkdir(parents=True, exist_ok=True)
    cmd = [compiler, "-c", str(src), "-o", str(obj), *cflags]
    print(f"[cc] {' '.join(cmd)}")

    try:
        run(cmd)
    except Exception as e:
        return e

    save_deps(state, src, deps)

def compile_all(sources: list[Path], dest: Path, *, compiler="gcc", libs=[], cflags=[], ldflags=[], workers=os.cpu_count()):
    state = load_state()
    objects = []
    errors = []

    os.makedirs(dest.parent, exist_ok=True)

    print(f"Using {workers} workers")
    
    with ProgressBar(len(sources)) as progress:
        print("Resolving dependencies...")

        for lib in libs:
            lib_cflags, lib_ldflags = pkg_config(lib)
            cflags.extend(lib_cflags)
            ldflags.extend(lib_ldflags)

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
            raise Exception(f"Error in {len(errors)} file(s)")

        # link
        cmd = [compiler, *map(str, objects), "-o", str(dest), *ldflags]
        print(f"[ld] {' '.join(cmd)}")

        run(cmd)

    print(f"Executable at {dest}")
    return 0
