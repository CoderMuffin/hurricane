#not intended to be used routinely, created for a one off operation
print("overwriting action halted")
exit()

import os
import re

def process_header(file):
    re_include = re.compile("^(#(?:include|define) .+)$", re.MULTILINE)
    re_func = re.compile("^(\w+ \*?\w+\(.*\)) {", re.MULTILINE)
    re_typedef = re.compile("^((?:typedef|enum) (?:.|\n)*?;(?![^{]*}))", re.MULTILINE)
    re_decl = re.compile("^((?:const )?\w+ *?\w+) ? =(?![^{]*})", re.MULTILINE)
    with open(file.replace("hurricane\\", "hurricane\\include\\").replace(".c", ".h"), "w") as out:
        with open(file, "r") as f:
            content = f.read()
            guard = re.findall("#ifndef (\w+)", content)[0]
            out.write("#ifndef " + guard + "_H\n")
            out.write("#define " + guard + "_H\n\n//macros\n")

            for item in re.findall(re_include, content):
                if item == "#define " + guard:
                    continue
                out.write(item.replace(".c", ".h").replace("include/", "") + "\n")
            
            out.write("\n//types\n")

            for item in re.findall(re_typedef, content):
                out.write(item + "\n")

            out.write("\n//decls\n")

            for item in re.findall(re_decl, content):
                out.write(item + ";\n")

            out.write("\n//functions\n")

            for item in re.findall(re_func, content):
                out.write(item + ";\n")

            out.write("\n#endif\n")

for root, dirs, files in os.walk("hurricane_GUARD_NO_OVERWRITE"):
    for file in files:
        if file.endswith(".c"):
            header = os.path.join(root, file)
            print(header)
            process_header(header)

    if "include" in dirs:
        dirs.remove("include")
    if "X11" in dirs:
        dirs.remove("X11")
    if "X11_so" in dirs:
        dirs.remove("X11_so")
    if "SDL" in dirs:
        dirs.remove("SDL")
    