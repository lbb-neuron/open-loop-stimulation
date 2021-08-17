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

class FilterData:
    def __init__(self,n,thr,queue_patterns,queue_cut,queue_filt,fs,f_high,std_thr,distance,remove):
        self.thd_filt   = multiprocessing.Process(target=self.start_process, args=[n,thr,queue_patterns,queue_cut,queue_filt,fs,f_high,std_thr,distance,remove])
        self.thd_filt.start()
    
    def start_process(self,n,thr,queue_patterns,queue_cut,queue_filt,fs,f_high,std_thr,distance,remove): 
        self.n          = n
        self.thr        = thr
        self.patterns   = queue_patterns
        self.queue_in   = queue_cut
        self.queue_out  = queue_filt
        self.fs         = fs
        self.f_high     = f_high
        self.std_thr    = std_thr
        
        self.distance   = distance
        self.remove     = remove  
        
        self.patterns_l = [] # History patterns
        self.patterns_b = [] # Backup history patterns
        
        self.filter_and_spike_detection()
    
    def butterworth(self, signal, cutoff, sampling_frequency, btype='high'):
        nyquist         = sampling_frequency/2.
        cutoff          = cutoff/nyquist
        v1,v2           = sg.butter(2,cutoff, btype=btype, analog=False)
        return sg.filtfilt(v1,v2,signal)

    def filter_and_spike_detection(self):

        while True:
            while self.patterns.qsize() > 0:
                element = self.patterns.get()
                l_ele   = 33 # 32 + 1 (note, that right now element has 64 elements. Last two are not backuped)
                self.patterns_l.append([element[i] for i in range(0,l_ele)])
                self.patterns_b.append([element[i] for i in range(l_ele,len(element))])
            while len(self.patterns_l) > 50:
                del self.patterns_l[0]
            while len(self.patterns_b) > 50:
                del self.patterns_b[0]
            
            seg_data = self.queue_in.get()
            data     = seg_data[:,:60*self.n]
            
            # Remove the median
            scaling_factor = int(self.fs/50+0.5)
            offset         = int(self.remove/1000.*self.fs)
            data     = data[:((data.shape[0]-offset)//scaling_factor)*scaling_factor+offset,:]
            data     = data - np.expand_dims(np.median(data[(data.shape[0]//2):,:],0),0)
            
            sys.stdout.flush()
            
            filt     = [self.butterworth(data[offset:,i],self.f_high,self.fs) for i in range(60*self.n)]
            
            # Do 50 Hz noise cancellation
            filt     = [np.reshape(np.reshape(filt[i],[-1,scaling_factor]) - 
                                   np.median(np.reshape(filt[i],[-1,scaling_factor]),0,keepdims=True),-1) for i in range(60*self.n)]
            
            spikes   = [fp(np.abs(filt[i][:])/float(self.thr[i]),height=self.std_thr.value,distance=int(self.distance/1000.*self.fs))[0] for i in range(60*self.n)]
            
            # Remove the first spike, if it is at 0. Because this means it is an artifact
            for i in range(60*self.n):
                if len(spikes[i])>0 and spikes[i][0] == 0:
                    del spikes[i][0]
            
            pattern  = seg_data[0,60*self.n] - 1 # (-1) since DSP is super fast and it already has the next pattern when you read out block
            
            for p in self.patterns_l:
                if p[0] == pattern:
                    pattern = np.copy(np.array(p[1:]))
                    break
            try:
                if pattern.shape[0] == 0:
                    raise ValueError('Pattern not in main pattern history list')
                else:
                    self.queue_out.put({'spikes': spikes,'filt': filt,'raw': np.copy(data),'pattern': np.copy(pattern)})
            except:
                # print('Search for pattern in backup')
                for p in self.patterns_b:
                    if p[0] == pattern:
                        pattern = np.copy(np.array(p[1:]))
                        break
                try:
                    if pattern.shape[0] == 0:
                        raise ValueError('Pattern not in backup pattern history list')
                    else:
                        self.queue_out.put({'spikes': spikes,'filt': filt,'raw': np.copy(data),'pattern': np.copy(pattern)})
                except:
                    print('Could not find pattern ' + str(int(pattern+0.5)) + ' in backup! (skip trial)')
                    sys.stdout.flush()