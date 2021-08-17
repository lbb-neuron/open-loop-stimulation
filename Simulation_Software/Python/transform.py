# This class transforms the raw data into 

import server
import os
import numpy as np
import multiprocessing

import     segment_data
import      filter_data
import      bundle_data
import       split_data
import communicate_data

import time
import threading
import sys

class Transform:
    def __init__(self, s, num_ports, s_port, auth_key, fs, f_high, num_stim, hs_type, inter_pattern_period, inter_stim_period):
        self.n          = 4                    # Number of headstages
        #self.std_thr    = 18                   # Number of STDs needed to detect spike
        self.std_thr    = multiprocessing.Value('d', 18)
        
        self.distance   = 3                    # [ms] Minimal distance between two spikes
        self.remove     = 1                    # [ms] Amount of ms deleted after stimulus
        
        self.num_ports  = num_ports            # Number of parallel port connections from C# code
        self.s_port     = s_port               # Port on which the queues can be accessed
        self.auth_key   = auth_key             # Authorization key used to access the queues
        
        self.s          = s                    # server that records the raw data
        self.fs         = fs                   # sampling frequency in Hz
        self.f_high     = f_high               # highpass filter cutoff in Hz
        
        self.num_stim   = num_stim             # Number of stimuli per pattern
        self.hs_type    = hs_type              # Vector of network types on each hs
        
        self.ipp        = inter_pattern_period # time between two consecutive stimulus pattern in ms
        self.isp        = inter_stim_period    # time between two consecutive stimuli in a pattern in ms
        
        # Queue for data
        self.queue_bdl  = multiprocessing.Queue()
        self.queue_cut  = multiprocessing.Queue()
        self.queue_filt = multiprocessing.Queue()
        self.queue_plot = multiprocessing.Queue()
        self.queue_nets = []
        for i in range(self.n):
            self.queue_nets.append(multiprocessing.Queue())
            
        # Create process that is used to commicate with the server and the DSP
        self.thd_com    = communicate_data.ComData(self.n,self.s_port,self.auth_key,self.queue_nets,self.s.queue_out,self.std_thr)
        
        self.patterns   = multiprocessing.Queue()
        
        # Threshold. This parameter is created by the data_Segmentation process and needed by the spike_detection process
        self.thr        = multiprocessing.Array('d', range(self.n*60))
        
        # Create process that does the data bundleing
        self.thd_bdl    = bundle_data.BundleData(self.num_ports,self.s.queue_in,self.queue_bdl)        
        
        # Create process that does the data segmentation
        self.thd_cut    = segment_data.SegmentData(self.n,self.thr,self.ipp,self.isp,self.fs,self.num_stim,self.queue_bdl,self.patterns,self.queue_cut)
        
        # Create process that filters segmented data and does spike detection
        self.thd_filt   = filter_data.FilterData(self.n,self.thr,self.patterns,self.queue_cut,self.queue_filt,self.fs,self.f_high,self.std_thr,self.distance,self.remove)
        
        # Split the data into the right format
        self.thd_split  = split_data.SplitData(self.n,self.fs,self.hs_type,self.queue_filt,self.queue_plot,self.queue_nets,self.thr,self.std_thr)
        
        # Create thread that periodically checks size of all of the queues
        self.thd_print  = threading.Thread(target=self.print_queue_sizes)
        self.thd_print.start()
        
    def print_queue_sizes(self):
        while True:
            time.sleep(1)
            print(self.s.queue_out.qsize(),
                  self.s.queue_in.qsize(),
                  self.queue_bdl.qsize(),
                  self.queue_cut.qsize(), 
                  self.queue_filt.qsize(),
                  self.queue_plot.qsize(),
                  [self.queue_nets[i].qsize() for i in range(self.n)])
            sys.stdout.flush()
