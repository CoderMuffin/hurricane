from pathlib import Path
import threading, os, subprocess
import fnmatch

TOOL_DIR = Path(os.path.realpath(__file__)).parent.resolve()
ROOT = TOOL_DIR.parent.resolve()
BUILD_DIR = TOOL_DIR / ".bbcache"
LOCK = threading.Lock()
VERSION = "0.0.1"

def sh(cmd: str | Path | list[str | Path], *args: list[str | Path]) -> str:
    if not isinstance(cmd, list):
        cmd = [cmd, *args]
    elif len(args) > 0:
        raise Exception("Cannot specify args when using cmd as a list. Use the first argument list for all arguments.")

    cmd = list(map(str, cmd))
    
    try:
        p = subprocess.run(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
        if p.returncode != 0:
            raise RuntimeError(p.stderr)
        return p.stdout
    except FileNotFoundError:
        raise FileNotFoundError(" ".join(cmd))

def glob(dir: Path, pattern: str, exclude: list[str] = []):
    """
    Recursively find files matching 'pattern' in 'dir', excluding any that match
    glob patterns in 'exclude'.
    """
    result = []

    # Resolve exclude patterns relative to dir
    exclude_patterns = [str(Path(dir / p).resolve()) for p in exclude]

    for candidate in [x.resolve() for x in dir.rglob(pattern)]:
        # Check if candidate matches any exclude pattern
        if any(fnmatch.fnmatch(str(candidate), pat) for pat in exclude_patterns):
            continue
        result.append(candidate)

    # Optional: warn about unused exclude patterns (harder with globs)
    # We'll skip this because it's ambiguous with globs

    return result

def bbmain(fn):
    print(f"BananaBuild {VERSION}")
    try:
        fn()
    except Exception as e:
        import sys
        if "-v" in sys.argv or "--verbose" in sys.argv:
            raise
        else:
            print("\x1b[31m" + str(e).rstrip() + " \x1b[36m(run with -v for verbose error output)\x1b[0m")
