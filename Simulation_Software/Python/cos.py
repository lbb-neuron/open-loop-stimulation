import tkinter as Tkinter
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
from matplotlib.figure import Figure
from math import cos, sin

class App:
    def __init__(self, master):
        # Create a container
        frame = Tkinter.Frame(master)
        # Create 2 buttons
        self.button_left = Tkinter.Button(frame,text="< Decrease Slope",
                                        command=self.decrease)
        self.button_left.pack(side="left")
        self.button_right = Tkinter.Button(frame,text="Increase Slope >",
                                        command=self.increase)
        self.button_right.pack(side="left")

        fig = Figure()
        ax = fig.add_subplot(111)
        self.line, = ax.plot([x/0.5 for x in range(20)])
        self.canvas = FigureCanvasTkAgg(fig,master=master)
        self.canvas.draw()
        self.canvas.get_tk_widget().pack(side='top', fill='both', expand=1)
        frame.pack()

    def decrease(self):
        x, y = self.line.get_data()
        self.line.set_ydata(y+[cos(xx) for xx in x])
        self.canvas.draw()

    def increase(self):
        x, y = self.line.get_data()
        self.line.set_ydata(y + 0.2 * x)
        self.canvas.draw()

if __name__ == "__main__":
    root = Tkinter.Tk()
    app = App(root)
    root.mainloop()