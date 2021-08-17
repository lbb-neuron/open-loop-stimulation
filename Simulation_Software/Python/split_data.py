import numpy as np
import multiprocessing
import threading

from scipy import signal as sg
from scipy.linalg import solveh_banded
from scipy.signal import find_peaks as fp

import tkinter as tk
import gui
import matplotlib as mpl

import sys

import transform_full
import transform_circle

class SplitData:    
    def __init__(self,n,fs,hs_type,queue_in,queue_plot,queue_out,thr,std_thr):
        self.thd_filt    = multiprocessing.Process(target=self.start_process, args=[n,fs,hs_type,queue_in,queue_plot,queue_out,thr,std_thr])
        self.thd_filt.start()
    
    def start_process(self,n,fs,hs_type,queue_in,queue_plot,queue_out,thr,std_thr):
        self.electrode_encoding = [6,7,9,11,14,15,18,20,22,23,
                                   4,3,5,10,13,16,19,24,26,25,
                                   1,2,0,8,12,17,21,29,27,28,
                                   58,57,59,51,47,42,38,30,32,31,
                                   55,56,54,49,46,43,40,35,33,34,
                                   53,52,50,48,45,44,41,39,37,36]
        
        self.n           = n
        self.fs          = fs
        self.thr         = thr
        self.std_thr     = std_thr
        self.hs_type     = hs_type
                
        self.queue_plot  = queue_plot
        self.queue_out   = queue_out
        self.queue_in    = queue_in
        
        # Start the GUI
        self.thd_gui = multiprocessing.Process(target=gui.MyGUI,args=[self.queue_plot,self.fs,self.thr,self.std_thr,self.electrode_encoding])
        self.thd_gui.start()
        
        self.split_data()
        
    def split_data(self):
        while True:
            element = self.queue_in.get()
            for i in range(self.n):
                while self.queue_out[i].qsize() >= 50:
                    self.queue_out[i].get()
                if self.hs_type[i] == 0:
                    # No PDMS mask is being used
                    new_element = transform_full.transform([element['spikes'][j+i*60] for j in self.electrode_encoding], 
                                                           [element['pattern'][j+i*4] for j in range(4)])
                elif self.hs_type[i] == 1:
                    # Using circular mask types
                    new_element = transform_circle.transform([element['spikes'][j+i*60] for j in self.electrode_encoding], 
                                                             [element['pattern'][j+i*4] for j in range(4)])
                else:
                    assert 1==2 # This throws an error, since these features are not implemented yet.
                self.queue_out[i].put(new_element)
            
            # This part is used for plotting
            if self.queue_plot.qsize() < 50:
                self.queue_plot.put({'spikes': element["spikes"],'filt': element["filt"],'raw': element["raw"]})
