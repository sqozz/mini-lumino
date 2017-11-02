icons = {

"play" : [
    ["001000"],
    ["001100"],
    ["001110"],
    ["001100"],
    ["001000"],
    ["000000"]],

"pause" : [
    ["011011"],
    ["011011"],
    ["011011"],
    ["011011"],
    ["011011"],
    ["000000"]],

"stop" : [
    ["011111"],
    ["011111"],
    ["011111"],
    ["011111"],
    ["011111"],
    ["000000"]]
}

for icon in icons:
    C_var = "char {}[] = {};".format(icon, "{}")
    icon = icons[icon]
    print(icon)
    icon_array = []
    for line in icon:
        print(hex(int(line[0], 2)))
        icon_array += [hex(int(line[0], 2))]
    icon_array = (", ".join(icon_array))
    print(C_var.format("{ " + icon_array + " }"))
