from pathlib import Path
import json, hashlib, threading

from util import BUILD_DIR, run, TOOL_DIR, glob

SCRIPTS = glob(TOOL_DIR, "*.py")
DEPS_FILE = BUILD_DIR / "deps.json"
LOCK = threading.Lock()

def load_state() -> dict:
    if DEPS_FILE.exists():
        return json.loads(DEPS_FILE.read_text())
    return {}

def save_state(state: dict):
    BUILD_DIR.mkdir(exist_ok=True)
    DEPS_FILE.write_text(json.dumps(state, indent=2))

def hash_file(path: Path) -> str:
    h = hashlib.sha1()
    with open(path, "rb") as f:
        for chunk in iter(lambda: f.read(8192), b""):
            h.update(chunk)
    return h.hexdigest()

def scan_deps(compiler: str, src: Path, cflags: list[str]) -> set[str]:
    """Return set of dependency file paths using compiler -MM"""
    out = run([compiler, "-MM", *cflags, str(src)], silent=True)
    # format: file.o: dep1 dep2 dep3
    deps = out.split(":", 1)[1]
    return {d.strip() for d in deps.replace("\\\n", " ").split()}

def save_deps(state: dict, src: Path, deps: set[str]):
    with LOCK:
        state[str(src)] = {
            "hash": hash_file(src),
            "deps": sorted(deps),
        }

def needs_rebuild(src: Path, obj: Path, deps: set[str], state: dict) -> bool:
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
    for d in list(deps) + SCRIPTS:
        if Path(d).exists() and Path(d).stat().st_mtime > obj_time:
            return True

    return False
