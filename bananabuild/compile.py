from concurrent.futures import ThreadPoolExecutor, as_completed
from pathlib import Path
import os

from progress import ProgressBar
from util import sh, BUILD_DIR, ROOT
from incremental import scan_deps, save_deps, needs_rebuild, load_state, save_state

def pkg_config(lib: str):
    try:
        cflags = sh(["pkg-config", "--cflags", lib]).split()
        libs = sh(["pkg-config", "--libs", lib]).split()
        return cflags, libs
    except RuntimeError:
        raise RuntimeError(
            f"Missing dependency: {lib}\n"
            f"Install it using your system package manager."
        )

def compile_one(compiler: str, src: Path, obj: Path, cflags: list[str], state: dict, progress: ProgressBar):
    deps = scan_deps(compiler, src, cflags)

    if not needs_rebuild(src, obj, deps, state):
        progress.print(f"[skip] {src}")
        return

    obj.parent.mkdir(parents=True, exist_ok=True)
    cmd = [compiler, "-c", str(src), "-o", str(obj), *cflags]
    progress.print("[cc]", " ".join(cmd))

    try:
        sh(cmd)
    except Exception as e:
        return e

    save_deps(state, src, deps)

def compile_all(sources: list[Path], dest: str, compiler="gcc", libs=[], cflags=[], ldflags=[], workers=4):
    state = load_state()
    objects = []

    progress = ProgressBar(len(sources))
    errors = []

    for lib in libs:
        lib_cflags, lib_ldflags = pkg_config(lib)
        cflags.extend(lib_cflags)
        ldflags.extend(lib_ldflags)

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
                    progress
                )
            )

        for f in as_completed(futures):
            error = f.result()
            if error is not None:
                errors.append(error)

            progress.update(text="(Failed)" if len(errors)>0 else None)

        progress.done()

    save_state(state)

    for error in errors:
        print(error)

    if len(errors) > 0:
        print(f"error in {len(errors)} file(s), exiting")
        return 1

    # link
    cmd = [compiler, *map(str, objects), "-o", dest, *ldflags]
    progress.print("[ld]", " ".join(cmd))

    try:
        sh(cmd)
        return 0
    except Exception as e:
        print("link failed:")
        print(e)
        return 1
