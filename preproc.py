IN_FILE = "./6x8.txt"
OUT_FILE = "./src/6x8.h"

with open(IN_FILE) as f:
    r, c = tuple(map(int, f.readline().split()))
    n = r * c
    f.readline()
    students = [f.readline().strip() for _ in range(n)]
    s2i = {s: i for i, s in enumerate(students)}

    f.readline()
    friends = {}
    for _ in range(n):
        s, fs = tuple(map(lambda s: s.strip(), f.readline().split(": ")))
        for fr in fs.split(","):
            if fr != "":
                friends.setdefault(s2i[s], set()).add(s2i[fr.strip()])

    f.readline()
    enemies = {}
    for _ in range(n):
        s, es = tuple(map(lambda s: s.strip(), f.readline().split(": ")))
        for e in es.split(","):
            if e != "":
                enemies.setdefault(s2i[s], set()).add(s2i[e.strip()])


with open(OUT_FILE, "w") as f:
    f.write("#ifndef SIX_BY_EIGHT_H\n")
    f.write("#define SIX_BY_EIGHT_H\n\n")
    f.write("#include <array>\n")
    f.write("#include <unordered_set>\n\n")
    f.write("constexpr int row_num = {};\n".format(r))
    f.write("constexpr int col_num = {};\n\n".format(c))
    f.write("constexpr int s_num = {};\n".format(n))
    f.write("\n")
    f.write("constexpr std::array<const char*, {}> student_names = {{\n".format(r * c))
    f.write(",\n".join('    "{}"'.format(s) for s in students))
    f.write("\n};\n")
    f.write("\n")

    f.write(
        "const std::array<std::unordered_set<short>, {}> friends = {{{{\n".format(r * c)
    )
    for i in range(n):
        if len(friends.get(i, set())) == 0:
            f.write("    {},")
            continue
        f.write("    {{")
        f.write(", ".join(str(fr) for fr in sorted(friends.get(i, []))))
        f.write("}}")
        if i != n - 1:
            f.write(",")
        f.write("\n")
    f.write("}};\n")
    f.write("\n")

    f.write(
        "const std::array<std::unordered_set<short>, {}> enemies = {{{{\n".format(r * c)
    )
    for i in range(n):
        if len(enemies.get(i, set())) == 0:
            f.write("    {},")
            continue
        f.write("    {{")
        f.write(", ".join(str(e) for e in sorted(enemies.get(i, []))))
        f.write("}}")
        if i != n - 1:
            f.write(",")
        f.write("\n")
    f.write("}};\n")
    f.write("\n")
    f.write("#endif")
