from font import chars

for char in chars:
    C_char_array = "{}"
    char_array = []
    for line in char:
        value = hex(int(line, 2))
        char_array += [value]
    C_char_array = C_char_array.format("{" + ", ".join(char_array) + "}")
    print(C_char_array + ",")
    C_var = ""
