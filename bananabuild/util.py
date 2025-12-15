from pathlib import Path
import threading, os, subprocess

TOOL_DIR = Path(os.path.realpath(__file__)).parent.resolve()
ROOT = TOOL_DIR.parent.resolve()
BUILD_DIR = TOOL_DIR / ".bbcache"
LOCK = threading.Lock()
VERSION = "0.0.1"

def sh(cmd: list[str]) -> str:
    p = subprocess.run(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
    if p.returncode != 0:
        raise RuntimeError(p.stderr)
    return p.stdout

def glob(dir: Path, pattern: str, exclude: list[str] = []):
    exclude_paths = [Path(dir / p).resolve() for p in exclude]
    return [x.resolve() for x in dir.rglob(pattern) if x.resolve() not in exclude_paths]
