#!/usr/bin/env python3
"""
Minimal C build tool with:
- Incremental compilation (timestamp + header dependency tracking)
- Threaded compilation

Requirements:
- Python 3.9+
- gcc or clang in PATH

Example:
  python cbuild.py build src/*.c -o app
  python cbuild.py clean

Design goals:
- Simple, readable, hackable
- No external Python dependencies
"""

import argparse
import hashlib
import json
import os
import subprocess
import threading
from concurrent.futures import ThreadPoolExecutor, as_completed
from pathlib import Path
import pathlib
from typing import Dict, List, Set
from progress import ProgressBar

SCRIPT = Path(os.path.realpath(__file__))
ROOT = SCRIPT.parent.parent.resolve()
BUILD_DIR = Path(".cbuild")
DEPS_FILE = BUILD_DIR / "deps.json"
LOCK = threading.Lock()

# ----------------------------- utilities -----------------------------

def sh(cmd: List[str]) -> str:
    p = subprocess.run(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
    if p.returncode != 0:
        raise RuntimeError(p.stderr)
    return p.stdout


def hash_file(path: Path) -> str:
    h = hashlib.sha1()
    with open(path, "rb") as f:
        for chunk in iter(lambda: f.read(8192), b""):
            h.update(chunk)
    return h.hexdigest()

def glob(dir: Path, pattern: str, exclude: str=[]):
    exclude_paths = [Path(dir / p).resolve() for p in exclude]
    return [x.resolve() for x in dir.rglob(pattern) if x.resolve() not in exclude_paths]

# ----------------------------- build state -----------------------------

def load_state() -> Dict:
    if DEPS_FILE.exists():
        return json.loads(DEPS_FILE.read_text())
    return {}


def save_state(state: Dict):
    BUILD_DIR.mkdir(exist_ok=True)
    DEPS_FILE.write_text(json.dumps(state, indent=2))

# ----------------------------- compilation -----------------------------

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


def scan_deps(compiler: str, src: Path, cflags: List[str]) -> Set[str]:
    """Return set of dependency file paths using compiler -MM"""
    out = sh([compiler, "-MM", *cflags, str(src)])
    # format: file.o: dep1 dep2 dep3
    deps = out.split(":", 1)[1]
    return {d.strip() for d in deps.replace("\\\n", " ").split()}

def needs_rebuild(src: Path, obj: Path, deps: Set[str], state: Dict) -> bool:
    if not obj.exists():
        return True

    prev = state.get(str(src))
    if not prev:
        return True

    # source changed
    if hash_file(src) != prev.get("hash"):
        return True

    # any dependency newer than object
    obj_time = obj.stat().st_mtime
    for d in list(deps) + [SCRIPT]:
        if Path(d).exists() and Path(d).stat().st_mtime > obj_time:
            return True

    return False


def compile_one(compiler: str, src: Path, obj: Path, cflags: List[str], state: Dict, progress: ProgressBar):
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

    with LOCK:
        state[str(src)] = {
            "hash": hash_file(src),
            "deps": sorted(deps),
        }

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


def main():
    exit(compile_all(
        glob(ROOT, "*.c", exclude=["monster_maze.c", "hurricane/src/renderer/console.c", "hurricane/src/renderer/video.c", "hurricane/src/renderer/xlib.c", "hurricane/src/util/legacy_vec.c"]),
        "hurricane",
        libs=["sdl2"],
        cflags=["-I", str(ROOT / "hurricane/include"), "-Wall", "-Wpedantic", "-Werror", "-fdiagnostics-color=always", "-g"]
    ))


if __name__ == "__main__":
    main()
