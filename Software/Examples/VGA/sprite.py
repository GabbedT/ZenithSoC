import tkinter as tk
from tkinter import colorchooser, simpledialog, messagebox
from tkinter import filedialog
import numpy as np

class PixelArtEditor:
    def __init__(self, master):
        self.master = master
        self.color = '#FFFFFF'
        self.pixel_size = 10
        self.canvas_size = 8
        self.canvas_array = np.zeros((64, 64), dtype=np.uint16)
        self.create_widgets()

    def create_widgets(self):
        self.canvas = tk.Canvas(self.master, width=self.canvas_size * self.pixel_size, height=self.canvas_size * self.pixel_size, borderwidth=0, highlightthickness=0)
        self.canvas.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)
        
        controls = tk.Frame(self.master)
        controls.pack(side=tk.RIGHT, fill=tk.BOTH)
        
        color_button = tk.Button(controls, text='Color', command=self.choose_color)
        color_button.pack()

        size_label = tk.Label(controls, text='Sprite Dimension:')
        size_label.pack()

        self.size_var = tk.StringVar(value='8')
        size_options = ['8', '16', '32', '64']
        size_menu = tk.OptionMenu(controls, self.size_var, *size_options, command=self.change_canvas_size)
        size_menu.pack()

        save_button = tk.Button(controls, text='Save', command=self.save_to_file)
        save_button.pack()
        
        self.pixel_widgets = {}
        self.build_grid()

    def build_grid(self):
        self.canvas.delete('all')
        self.pixel_widgets = {}
        for y in range(self.canvas_size):
            for x in range(self.canvas_size):
                x1, y1 = x * self.pixel_size, y * self.pixel_size
                x2, y2 = x1 + self.pixel_size, y1 + self.pixel_size
                pixel_id = self.canvas.create_rectangle(x1, y1, x2, y2, outline='', fill='white')
                self.pixel_widgets[pixel_id] = (x, y)
                self.canvas.tag_bind(pixel_id, '<Button-1>', self.paint_pixel)

    def paint_pixel(self, event):
        pixel_id = self.canvas.find_closest(event.x, event.y)[0]
        x, y = self.pixel_widgets[pixel_id]
        self.canvas_array[y, x] = self.rgb_gen(self.color)
        self.canvas.itemconfig(pixel_id, fill=self.color)

    def rgb_gen(self, hexcolor):
        color = self.master.winfo_rgb(hexcolor)
        return ((color[0] >> 12) << 8) | ((color[1] >> 12) << 4) | (color[2] >> 12)

    def rgb_to_12bit(self, hexcolor):
        print("Before: " + hex(hexcolor))
        print("After: " + hex(((hexcolor >> 8) << 8) | ((hexcolor >> 8) << 4) | (hexcolor >> 8)))
        return ((hexcolor >> 8) << 8) | ((hexcolor >> 8) << 4) | (hexcolor >> 8)

    def choose_color(self):
        color = colorchooser.askcolor(color=self.color)[1]
        if color:
            self.color = color

    def change_canvas_size(self, size):
        self.canvas_size = int(size)
        self.canvas.config(width=self.canvas_size * self.pixel_size, height=self.canvas_size * self.pixel_size)
        self.build_grid()

    def save_to_file(self):
        # for y in range(self.canvas_size):
        #     for x in range(self.canvas_size):
        #         print(self.canvas_array[y, x])
        file_name = filedialog.asksaveasfilename(defaultextension=".h", filetypes=[("C Header File", "*.h")])

        if file_name:
            with open(file_name, 'w') as file:
                file.write(f'#include <inttypes.h> \n\nuint16_t sprite[{self.canvas_size}][{self.canvas_size}] = {{\n')
                for y in range(self.canvas_size):
                    file.write('{')
                    for x in range(self.canvas_size):
                        file.write(f'0x{self.canvas_array[y, x]:03X}, ')
                    file.write('},\n')
                file.write('};\n')
            messagebox.showinfo('Saved', 'Sprite saved correctly.')

if __name__ == '__main__':
    root = tk.Tk()
    app = PixelArtEditor(root)
    root.mainloop()
