import threading, time
from typing import Optional

class ProgressBar:
    def __init__(self, total: int, width: int = 30, text: str = ""):
        self.total = total
        self.width = width
        self.current = 0
        self.start = time.time()
        self.text = text
        self.lock = threading.Lock()
        self.stop_event = threading.Event()

        self.print_queue = []

        self.thread = threading.Thread(target=self._run, daemon=True)
        self.thread.start()

    def _run(self):
        while not self.stop_event.is_set():
            with self.lock:
                self.render()
            self.stop_event.wait(0.1)

    def update(self, n: int = 1, text: Optional[str] = None):
        with self.lock:
            self.current += n
            if text is not None:
                self.text = text

    def render(self):
        frac = self.current / self.total if self.total else 1
        filled = int(self.width * frac)
        bar = "#" * filled + "-" * (self.width - filled)
        elapsed = time.time() - self.start
        msg = f"[{bar}] {self.current}/{self.total} {elapsed:5.1f}s {self.text}"

        while len(self.print_queue) > 0:
            msg = " ".join(self.print_queue.pop(0)) + "\n" + msg

        print("\r" + msg, end="", flush=True)

    def print(self, *args):
        self.print_queue.append(args)

    def done(self):
        self.stop_event.set()
        self.thread.join(timeout=0.2)
        self.render()
        print()
