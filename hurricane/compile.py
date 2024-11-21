import subprocess

def get_input(prompt, validator = lambda _: True):
    x = input(prompt)
    while not validator(x):
        print("invalid selection")
        x = input(prompt)
    return x

sdl = get_input("enable sdl? (y/N) ", lambda x: x.lower() in ["y", "n", ""]).lower() == "y"
debug = get_input("include debug information? (Y/n) ", lambda x: x.lower() in ["y", "n", ""]).lower() != "n"
target = get_input("target? (build/hurricane.dll) ") or "build/hurricane.dll"

gcc = ["gcc"]

if debug:
    gcc.append("-g")

if sdl:
    gcc.extend(["-Ihurricane/renderer/SDL/include", "-Lhurricane/renderer/SDL/bin/", "-lSDL2", "-Lhurricane/renderer/SDL/lib/", "-lSDL2main", "-D"])

gcc.append("*.c")
gcc.append("-o")
gcc.append(target)

subprocess.run(gcc)
