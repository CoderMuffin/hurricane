import os
import re
from pycparser import c_parser, c_ast

class TypedefVisitor(c_ast.NodeVisitor):
    def __init__(self):
        self.typedefs = []
        self.code_strings = []

    def visit_Typedef(self, node):
        self.typedefs.append(node)
        self.code_strings.append(node.coord.file + ':' + str(node.coord.line) + ': ' + node.to_string())

def extract_typedefs_with_code(code):
    parser = c_parser.CParser()
    ast = parser.parse(code)
    typedef_visitor = TypedefVisitor()
    typedef_visitor.visit(ast)
    return typedef_visitor.typedefs, typedef_visitor.code_strings

def process_header(file):
    re_include = re.compile("^(#include .+)$", re.MULTILINE)
    re_func = re.compile("^(\w+ \*?\w+\(.*\)) {", re.MULTILINE)
    re_typedef = re.compile("^(typedef (?:.|\n)*?;(?![^{]*}))", re.MULTILINE)
    with open(file.replace("hurricane\\", "hurricane\\include\\").replace(".c", ".h"), "w") as out:
        with open(file, "r") as f:
            content = f.read()
            guard = re.findall("#ifndef (\w+)", content)[0] + "_H"
            out.write("#ifndef " + guard + "\n")
            out.write("#define " + guard + "\n\n")

            for item in re.findall(re_include, content):
                out.write(item.replace(".c", ".h") + "\n")
            
            out.write("\n")

            for item in re.findall(re_typedef, content):
                out.write(item + "\n")

            out.write("\n")

            for item in re.findall(re_func, content):
                out.write(item + ";\n")

            out.write("\n#endif\n")

for root, dirs, files in os.walk("hurricane"):
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
    