import re,os,pyperclip

file = os.path.join(os.path.dirname(__file__), "auton.atn")
contents = ""
with open(file, "r") as m:
    contents = m.read()


final_file = os.path.join(os.path.dirname(__file__), "src", "auton.cpp")
c_side = ['#include "init.hpp"', '', 'void autonomousTick() {']
comment = ""
drive_name = "UNKNOWN_DRIVETRAIN"
current_angle=0
for line in contents.splitlines():
    if line.startswith("//"):
        comment = line[2::]
        c_side.append("\t//"+comment)
    elif line.startswith("assign"):
        drive_name = line[6::].strip()
    elif line.startswith("rotate"):
        args=line.split(" ")
        if args[1].lower() == "to":
            current_angle = (float(args[2])) % 360
        else:
            if args[1].lower() == "left":
                current_angle = (current_angle - float(args[2])) % 360
            elif args[1].lower() == "right":
                current_angle = (current_angle + float(args[2])) % 360
        c_side.append(f"\t{drive_name}.rotateTo({current_angle if current_angle!=0 else str(current_angle)+".0"}f);")
    elif line.startswith("forward"):
        args=line.split(" ")
        c_side.append(f"\t{drive_name}.moveDistance({args[1]}, {args[2].upper()});")
    elif line.startswith("reverse") or line.startswith("backwards"):
        args=line.split(" ")
        c_side.append(f"\t{drive_name}.moveDistance(-{args[1]}, {args[2].upper()});")
    elif line.startswith("wait"):
        args=line.split(" ")
        time = int(args[1])
        if args[2].lower() == "seconds" or args[2].lower() == "s":
            time *= 1000
        c_side.append(f"\tpros::delay({time});")
    elif line.strip() == "":
        continue
    else:
        c_side.append(f"\t//{line}")
c_side.append("}")
pyperclip.copy("\n".join(c_side))
with open(final_file, "w") as f:
    f.write("\n".join(c_side))