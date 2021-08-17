import tkinter as tk
import matplotlib as mpl

from matplotlib.figure import Figure
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
from tkinter import filedialog as fd
from matplotlib import style

import multiprocessing
import threading
import numpy as np

import time
import sys

class MyGUI:
    def __init__(self, queue,fs,thr,std_thr,electrode_encoding):
        style.use("ggplot")
        root                    = tk.Tk()
        master                  = root # tk.Frame(root)
        
        self.master             = master
        self.queue              = queue
        self.fs                 = fs
        self.thr                = thr
        self.std_thr            = std_thr
        self.electrode_encoding = electrode_encoding
        root.title("Online data from MEA2100 mini headstages")
        
        self.ylim               = 10000   # This is the limit of the y-axis when plotting the data
        self.skip               = 8       # plot every skip.th stimulus
        
        button1_offset          = 0
        button2_offset          = 13
        
        # Count how many packages have currently been used
        self.counter            = 0 
        
        # Variables that save with electrodes to plot
        self.select_SCU1        = 0
        self.select_SCU2        = 0
        self.select_row1        = 0
        self.select_row2        = 0
        self.select_col1        = 0
        self.select_col2        = 1
        
        # First figure
        self.figure1            = Figure(figsize=(8,3),dpi=100)
        self.ax1                = self.figure1.add_subplot(111)
        self.canvas1            = FigureCanvasTkAgg(self.figure1, self.master)
        self.canvas1.get_tk_widget().grid(row=0+button1_offset, column=0,columnspan=7,rowspan=10)
        self.canvas1.draw()
        self.ax1.clear()
        
        # Control panel
        self.text_ylim   = tk.StringVar()
        self.text_skip   = tk.StringVar()
        self.b_y_half    = tk.Button(master, text="/2", command=self.half_ylim).grid(row=10+button1_offset,column=0)
        self.l_Y_lim     = tk.Label(master, textvariable=self.text_ylim).grid(row=10+button1_offset,column=1,sticky=tk.W+tk.N+tk.S)
        self.b_y_double  = tk.Button(master, text="x2", command=self.double_ylim).grid(row=10+button1_offset,column=2)
        
        # Slider
        self.max_length         = 1000
        self.lower_t            = 0
        self.upper_t            = self.max_length
        self.slider_low  = tk.Scale(master, from_=0, to=self.max_length, tickinterval=25, 
                                    orient=tk.HORIZONTAL, length = 800, command=self.update_lower_slider)
        self.slider_low.grid(row=11+button1_offset,column=0,columnspan=7)
        self.slider_high = tk.Scale(master, from_=0, to=self.max_length, tickinterval=25, 
                                    orient=tk.HORIZONTAL, length = 800, command=self.update_upper_slider)
        self.slider_high.grid(row=12+button1_offset,column=0,columnspan=7)
        self.slider_low.set(0)
        self.update_lower_slider(0)
        self.slider_high.set(self.max_length)
        self.update_upper_slider(self.max_length)
        
        self.text_sc     = tk.StringVar()
        self.spike_count = tk.Label(master,textvariable=self.text_sc).grid(row=11+button1_offset,
                                                                           column=8,
                                                                           columnspan=6+1+6,
                                                                           sticky=tk.W+tk.N+tk.S)
        self.spacer      = tk.Label(master,text=" ").grid(row=10+button1_offset,column=3,sticky=tk.W+tk.N+tk.S)
        
        self.b_skip_m    = tk.Button(master, text="-", command=self.skip_minus).grid(row=10+button1_offset,column=4)
        self.l_skip      = tk.Label(master, textvariable=self.text_skip).grid(row=10+button1_offset,column=5,sticky=tk.W+tk.N+tk.S)
        self.b_skip_p    = tk.Button(master, text="+", command=self.skip_plus).grid(row=10+button1_offset,column=6)
                
        # Second figure
        self.figure2     = Figure(figsize=(8,3),dpi=100)
        self.ax2         = self.figure2.add_axes([0.1,0.1,0.8,0.8])
        self.canvas2     = FigureCanvasTkAgg(self.figure2, self.master)
        self.canvas2.get_tk_widget().grid(row=button2_offset, column=0,columnspan=7,rowspan=10)
        self.canvas2.draw()
        self.ax1.clear()
        
        # Create the Electrode buttons
        labels = ["K","I","H","G","F","E","D","C","B","A"]
        tk.Label(master, text = "       ").grid(row=0,column=7) # spacer. Can be ignored
        tk.Label(master, text = "       ").grid(row=0,column=14) # spacer. Can be ignored
        self.electrode_buttons = []
        for i in range(4):
            self.electrode_buttons.append([])
            for j in range(10):
                self.electrode_buttons[i].append([])
                for k in range(6):
                    if i == 0:
                        r = button1_offset
                        c = 8
                    elif i == 1:
                        r = button1_offset
                        c = 15
                    elif i == 2:
                        r = button2_offset
                        c = 8
                    else:
                        r = button2_offset
                        c = 15
                    button = tk.Button(master, text=labels[j]+str(k+1),height = 1, width = 2,font='Helvetica 9')
                    button.bind('<Button-1>', lambda _,i=i, j=j, k=k: self.set_source(0,i,j,k))  # left click
                    button.bind('<Button-3>', lambda _,i=i, j=j, k=k: self.set_source(1,i,j,k))  # right click
                    button.grid(row=r+j,column=c+k)
                    self.electrode_buttons[i][j].append(button)
        
        self.set_source(0,self.select_SCU1,self.select_row1,self.select_col1)
        self.set_source(1,self.select_SCU2,self.select_row2,self.select_col2)
                
        self.update_ylim_text()
        self.update_skip_text()
        
        self.plot_data()
        root.mainloop()
        
    def set_source(self,graph,SCU,row,column):
        sys.stdout.flush()
        if graph == 0:
            if self.select_SCU2 == SCU and self.select_row2 == row and self.select_col2 == column:
                return
            self.electrode_buttons[self.select_SCU1][self.select_row1][self.select_col1]["fg"]="black"
            self.electrode_buttons[self.select_SCU1][self.select_row1][self.select_col1].configure(font='Helvetica 9')
            self.select_SCU1 = SCU
            self.select_row1 = row
            self.select_col1 = column
            self.electrode_buttons[self.select_SCU1][self.select_row1][self.select_col1]["fg"]="red"
            self.electrode_buttons[self.select_SCU1][self.select_row1][self.select_col1].configure(font='Helvetica 9 bold underline')
        elif graph == 1:
            if self.select_SCU1 == SCU and self.select_row1 == row and self.select_col1 == column:
                return
            self.electrode_buttons[self.select_SCU2][self.select_row2][self.select_col2]["fg"]="black"
            self.electrode_buttons[self.select_SCU2][self.select_row2][self.select_col2].configure(font='Helvetica 9')
            self.select_SCU2 = SCU
            self.select_row2 = row
            self.select_col2 = column
            self.electrode_buttons[self.select_SCU2][self.select_row2][self.select_col2]["fg"]="blue"
            self.electrode_buttons[self.select_SCU2][self.select_row2][self.select_col2].configure(font='Helvetica 9 bold underline')
        
    def plot_data(self):
        length_element = 250
        if self.queue.qsize() == 0:
            None
        elif self.counter < self.skip-1:
            self.queue.get()
            self.counter += 1
        else:
            self.counter = 0
            element = self.queue.get()

            #self.update_ylim_text()
            #self.update_skip_text()
            
            index_1     = 60*self.select_SCU1+self.electrode_encoding[9-self.select_row1+self.select_col1*10]
            index_2     = 60*self.select_SCU2+self.electrode_encoding[9-self.select_row2+self.select_col2*10]
            
            raw_data_1  = np.array(element["raw"][:,index_1])
            filt_data_1 = np.array(element["filt"][index_1])
            raw_data_2  = np.array(element["raw"][:,index_2])
            filt_data_2 = np.array(element["filt"][index_2])
            thr_1       = self.thr[index_1]*self.std_thr.value
            thr_2       = self.thr[index_2]*self.std_thr.value
            spikes_1    = element["spikes"][index_1]
            spikes_2    = element["spikes"][index_2]
            
            # Update slider lengths for the first pattern received
            if self.slider_low["to"] == 1000:
                self.max_length        = int(filt_data_1.shape[0]*1000/self.fs)
                self.slider_low["to"]  = self.max_length
                self.slider_high["to"] = self.max_length
            
            try:
                self.ax1.clear()
                self.ax1.plot((np.arange(raw_data_1.shape[0])*1000/self.fs),(raw_data_1),'k-')
                self.ax1.plot((np.arange(filt_data_1.shape[0])*1000/self.fs),(filt_data_1),'r-')
                for timepoint in spikes_1:
                    self.ax1.axvline(timepoint*1000/self.fs,c='g',ls='-')
                self.ax1.plot([0,raw_data_1.shape[0]*1000/self.fs],[thr_1,thr_1],'y--')
                self.ax1.plot([0,raw_data_1.shape[0]*1000/self.fs],[-thr_1,-thr_1],'y--')
                self.ax1.grid(True)
                self.ax1.set_ylim([-self.ylim,self.ylim])
                self.ax1.set_xlim([self.lower_t,self.upper_t])
                self.canvas1.draw()
                
                self.ax2.clear()
                self.ax2.plot((np.arange(raw_data_2.shape[0])*1000/self.fs),raw_data_2,'k-')
                self.ax2.plot((np.arange(filt_data_2.shape[0])*1000/self.fs),filt_data_2,'b-')
                for timepoint in spikes_2:
                    self.ax2.axvline(timepoint*1000/self.fs,c='g',ls='-')
                self.ax2.plot([0,raw_data_2.shape[0]*1000/self.fs],[thr_2,thr_2],'y--')
                self.ax2.plot([0,raw_data_2.shape[0]*1000/self.fs],[-thr_2,-thr_2],'y--')
                self.ax2.grid(True)
                self.ax2.set_ylim([-self.ylim,self.ylim])
                self.ax2.set_xlim([self.lower_t,self.upper_t])
                self.canvas2.draw()
                
                count = 0
                for i in range(4):
                    for j in range(10):
                        for k in range(6):
                            index = 60*i+self.electrode_encoding[9-j+k*10]
                            count += len(element["spikes"][index])
                            if len(element["spikes"][index])>=1:
                                self.electrode_buttons[i][j][k].configure(bg="yellow")
                            else:
                                self.electrode_buttons[i][j][k].configure(bg="lightgray")
                                
                self.text_sc.set("Total number of spikes: " + str(int(count)))
            except:
                print("exception thrown")
                sys.stdout.flush()
        self.master.after(1, self.plot_data)
                
    def update_upper_slider(self,value):
        value = int(value)
        if value < 2:
            print('now')
            sys.stdout.flush()
            value == 1
            self.slider_high.set(self.value)
        if self.lower_t + 1 > value:
            self.lower_t = value - 1
            self.slider_low.set(self.lower_t)
        self.upper_t            = value
                
    def update_lower_slider(self,value):
        value = int(value)
        if value > self.max_length-1:
            value == self.max_length-1
            self.slider_low.set(self.value)
        if self.upper_t - 1 < value:
            self.upper_t = value + 1
            self.slider_high.set(self.upper_t)
        self.lower_t            = value
            
    def update_ylim_text(self):
        self.text_ylim.set("Y limit: +- " + str(self.ylim))
        self.ax1.set_ylim([-self.ylim,self.ylim])
        self.ax2.set_ylim([-self.ylim,self.ylim])
        self.canvas1.draw()
        self.canvas2.draw()
        
    def update_skip_text(self):
        if self.skip == 1:
            self.text_skip.set("Plot every response")
        elif self.skip > 3 and self.skip < 20:
            self.text_skip.set("Plot every " + str(self.skip) + ".th response")
        elif self.skip%10 == 1:
            self.text_skip.set("Plot every " + str(self.skip) + ".st response")
        elif self.skip%10 == 2:
            self.text_skip.set("Plot every " + str(self.skip) + ".nd response")
        elif self.skip%10 == 3:
            self.text_skip.set("Plot every " + str(self.skip) + ".rd response")
        else:
            self.text_skip.set("Plot every " + str(self.skip) + ".th response")
        
    def half_ylim(self):
        self.ylim       = max(1,self.ylim//2)
        self.update_ylim_text()
    
    def double_ylim(self):
        self.ylim       = 2 * self.ylim
        self.update_ylim_text()
        
    def skip_minus(self):
        self.skip = max(1,self.skip-1)
        self.update_skip_text()
        
    def skip_plus(self):
        self.skip = self.skip + 1
        self.update_skip_text()