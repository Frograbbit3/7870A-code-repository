import os
from math import floor
import keyboard

LINES=["","", ""]
LIMIT = 19
CURRENT_MENU = None

def _print(line, txt):
    global LINES
    if line<0 or line>2:return
    LINES[line]=txt[:LIMIT]
    _render()

def scroll_text(txt, offset):
    l=len(txt)
    o=offset%l
    return txt[o:]+txt[:o]

def set_text(line, txt):
    global LINES
    if line<0 or line>2:return
    LINES[line]=txt[:LIMIT]
    _render()

def clear_line(line):
    global LINES
    if line<0 or line>2:return
    LINES[line]=""
    _render()

def _render():
    os.system("cls" if os.name=="nt" else "clear")
    print("[THIS IS A DEMO]")
    for l in LINES:
        print(l)


def generate_box(input_txt):
    cropped_input = input_txt[:LIMIT-2]
    space_count =  floor((LIMIT-2)-len(cropped_input))//2
    _print(0, "-"*LIMIT)
    _print(1,"|"+(" " * space_count)+cropped_input+(" " * space_count)+"|")
    _print(2, "-"*LIMIT)


class Button:
    def __init__(self, text: str, on_select=None):
        self.text = "[" + text + "]"
        self.c = 0
        self.on_select = on_select
        pass
    def render(self):
        if len(self.text) > LIMIT:
            generate_box(scroll_text(self.text, self.c))
        else:
            generate_box(self.text)
        self.c+=1
    def select(self):
        if self.on_select:
            self.on_select()
class Slider:
    def __init__(self,minv,maxv,on_sel,editable=True,value=None):
        self.editable = editable
        self.min = minv
        self.max = maxv

        self.size = 15 - len(str(maxv)) - 1
        self.step = (maxv - minv) / self.size

        self.on_sel = on_sel

        if value is not None:
            self.pos = round((value - self.min) / self.step)
        else:
            self.pos = 0

    @property
    def value(self):
        return int(self.min + self.pos * self.step)

    def render(self):
        filled = self.pos
        bar = "#" * filled + "-" * (self.size - filled)
        generate_box(f"[{bar}] {self.value}")

    def next_page(self):
        if self.editable and self.pos < self.size:
            self.pos += 1

    def previous_page(self):
        if self.editable and self.pos > 0:
            self.pos -= 1

    def select(self):
        self.on_sel()




class Page:
    def __init__(self, pages):
        self.pages = pages
        self.pg = 0
        self.count = len(pages)
        for i, page in enumerate(self.pages):
            if i == 0:
                page.text = f"{page.text}>>"
            elif i == self.count-1:
                page.text = f"<<{page.text}"
            else:
                page.text = f"<<{page.text}>>"
    def next_page(self):
        self.pg+=1
        if self.pg > self.count-1:
            self.pg = self.count -1
    def previous_page(self):
        self.pg-=1
        if self.pg < 0:
            self.pg = 0
    def render(self):
        self.pages[self.pg].render()
    def select(self):
        self.pages[self.pg].select()

class Label:
    def __init__(self, text: str):
        self.text = text
        self.c = 0
        pass
    def render(self):
        if len(self.text) > LIMIT:
            generate_box(scroll_text(self.text, self.c))
        else:
            generate_box(self.text)
        self.c+=1
    def select(self):
        return

s=None
def on_select():
    global CURRENT_MENU
    CURRENT_MENU = s
main = Page([
    Button("Settings", on_select),
    Label("Example")
])

def mn():
    global CURRENT_MENU
    CURRENT_MENU = main

s = Slider(0,100,mn, value=25,editable=False)





CURRENT_MENU=main
while True:
    if CURRENT_MENU:
        CURRENT_MENU.render()
        if keyboard.is_pressed("space"):
            CURRENT_MENU.select()
        if keyboard.is_pressed("right"):
            if hasattr(CURRENT_MENU, "next_page"):
                CURRENT_MENU.next_page()
        if keyboard.is_pressed("left"):
            if hasattr(CURRENT_MENU, "previous_page"):
                CURRENT_MENU.previous_page()
    _render()
    __import__("time").sleep(0.12)