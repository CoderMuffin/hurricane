import subprocess, os, threading, sys
from concurrent.futures import ThreadPoolExecutor, ProcessPoolExecutor, Executor

class ThreadPoolVisualiserTask:
    def __init__(self, task, label):
        self.task = task
        self.label = label

class ThreadPoolVisualiser:
    def __init__(self, executor: ThreadPoolExecutor, max_workers: int):
        self.workers = max_workers
        self.threads = {}
        self.lock = threading.Lock()
        self.executor = executor
        self.first_update = True

    def update(self):
        if not self.first_update:
            clear_last_n_lines(self.workers, False)
        else:
            self.first_update = False

        for thread in self.threads:
            if self.threads[thread] is None:
                print(f"Thread {thread} | Idle", flush=False)
            else:
                print(f"Thread {thread} | " + self.threads[thread], flush=False)
        
        for _ in range(self.workers - len(self.threads)):
            print()
        
        sys.stdout.flush()


    def run_task(self, task, label: str):
        thread_id = threading.current_thread().ident

        with self.lock:
            self.threads[thread_id] = label
            self.update()

        task()

        with self.lock:
            self.threads[thread_id] = None
            self.update()

    def run(self, tasks: list[ThreadPoolVisualiserTask]):
        for task in tasks:
            self.executor.submit(self.run_task, task.task, task.label)

base = os.path.realpath(os.path.join(__file__, "..")).replace("\\", "/")

def clear_last_n_lines(n, flush: bool):
    # Repeat for each of the previous lines
    for _ in range(n):
        print("\x1b[K\x1b[A", end="", flush=flush)
    
    print("\x1b[K", end="", flush=flush)

def get_input(prompt, validator = lambda _: True):
    x = input(prompt)
    while not validator(x):
        print("invalid selection")
        x = input(prompt)
    return x

def join_path(*items):
    return os.path.join(*items).replace("\\", "/")

def get_files(directory):
    paths = []
    for root, dirs, files in os.walk(directory):
        for file in files:
            if file.endswith(".c") or file.endswith(".h"):
                paths.append(join_path(root, file))

        if "include" in dirs:
            dirs.remove("include")
        if "build" in dirs:
            dirs.remove("build")
        if "X11" in dirs:
            dirs.remove("X11")
        if "X11_so" in dirs:
            dirs.remove("X11_so")
        if "SDL" in dirs:
            dirs.remove("SDL")
    return paths

def compile_file(*args):
    subprocess.run(args)

if get_input("recompile? (y/N) ", lambda x: x.lower() in ["y", "n", ""]).lower() != "y":
    exit()

sdl = get_input("enable sdl? (y/N) ", lambda x: x.lower() in ["y", "n", ""]).lower() == "y"
x11 = get_input("enable x11? (y/N) ", lambda x: x.lower() in ["y", "n", ""]).lower() == "y"
target_os = get_input("target os? (windows/unix)", lambda x: x.lower() in ["windows", "linux", ""]).lower() or "windows"
debug = get_input("include debug information? (Y/n) ", lambda x: x.lower() in ["y", "n", ""]).lower() != "n"
target = join_path(base, "build")

gcc = ["gcc", "-c"]
    
if sdl:
    gcc.extend(["-I" + join_path(base, "renderer/SDL/include"), "-L" + join_path(base, "renderer/SDL/bin"), "-lSDL2", "-L" + join_path(base, "renderer/SDL/lib"), "-lSDL2main", "-DHC_ENABLE_SDL2"])

if x11:
    gcc.extend(["-lX11", "-lxcb" "-lXau", "-lXdmcp", "-lbsd", "-DHC_ENABLE_X11"])

if target_os == "windows":
    gcc.append("-fPIC")

if debug:
    gcc.append("-g")

gcc.append("-Wall")
gcc.append("-Wextra")
gcc.append("-Werror")
gcc.append("-lm")
gcc.append("-pthread")

files = get_files(base)
object_files = list(map(lambda x: join_path(target, "object", x.replace(".c", ".o").replace(base, "").lstrip("/")), files)) #duplicate iterator

print()
print("> " + " ".join(gcc))

compile_thread_count = 4

with ThreadPoolExecutor(max_workers=compile_thread_count) as tpe:
    tpv = ThreadPoolVisualiser(tpe, compile_thread_count)
    tpv.run([ThreadPoolVisualiserTask(lambda: compile_file(*gcc, file, "-o", dest), file) for file, dest in zip(files, object_files)])

if target_os == "windows":
    dll_target = join_path(base, "..", "hurricane.dll")

    gcc = ["gcc", "-shared", "-o", dll_target, *object_files]

    if sdl:
        gcc.extend(["-I" + join_path(base, "renderer/SDL/include"), "-L" + join_path(base, "renderer/SDL/bin"), "-lSDL2", "-L" + join_path(base, "renderer/SDL/lib"), "-lSDL2main", "-DHC_ENABLE_SDL2"])

    if x11:
        gcc.extend(["-lX11", "-lxcb" "-lXau", "-lXdmcp", "-lbsd", "-DHC_ENABLE_X11"])

    print()
    print("> link all -> " + dll_target.replace(base, ""))
    subprocess.run(gcc)
