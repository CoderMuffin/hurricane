from pathlib import Path
import threading, os, subprocess
from typing import Sequence
import fnmatch

TOOL_DIR = Path(os.path.realpath(__file__)).parent.resolve()
ROOT = TOOL_DIR.parent.resolve()
BUILD_DIR = TOOL_DIR / ".bbcache"
LOCK = threading.Lock()
VERSION = "0.0.1"

def run(cmd: str | Sequence[str | Path], *, silent=False, **kwargs) -> str:
    cmd_str = cmd if isinstance(cmd, str) else " ".join(map(str, cmd))

    try:
        with subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True, **kwargs) as process:
            output = ""
            for line in process.stdout:
                output += line
                if not silent:
                    print(line, end='')

            process.wait()

            if process.returncode != 0:
                raise RuntimeError(f"util.run: command failed with exit code {process.returncode}\ncommand: \x1b[0m{cmd_str}\n\noutput:\n{output}")

            return output
    except FileNotFoundError:
        raise FileNotFoundError(f"util.run: command not found\ncommand: \x1b[0m{cmd_str}")

def glob(dir: Path, pattern: str, exclude: list[str] = []):
    result = []

    exclude_patterns = [str(Path(dir / p).resolve()) for p in exclude]

    for candidate in [x.resolve() for x in dir.rglob(pattern)]:
        # Check if candidate matches any exclude pattern
        if any(fnmatch.fnmatch(str(candidate), pat) for pat in exclude_patterns):
            continue
        result.append(candidate)

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
            print("\x1b[31;1m" + str(type(e).__name__) + "\x1b[0;31m: " + str(e).rstrip() + " \x1b[36m(run with -v for verbose error output)\x1b[0m")
            
