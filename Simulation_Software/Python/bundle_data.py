import numpy as np
import multiprocessing

import time
from calendar import timegm
from datetime import datetime

import sys

class BundleData:
    def __init__(self,n,queue_in,queue_out):
        self.thd_bdl       = multiprocessing.Process(target=self.start_process, args=[n,queue_in,queue_out])
        self.thd_bdl.start()
    
    def start_process(self,n,queue_in,queue_out): 
        self.n             = n                          # Number of packages to bundle
        
        self.queue_in      = queue_in
        self.queue_out     = queue_out
        
        self.id_count_in   = n
        self.id_count_out  = 1
        
        self.bundle()
                
    def bundle(self):
        while True:
            temp_count = 0
            packages = []
            
            for i in range(self.n):
                package = self.queue_in.get()
                
                # Get data and save information from 1st package
                packages.append(package['data'])
                if i == 0:
                    temp_date = package['date']
                    temp_pattern = package['pattern']
                
                # Check id of packages
                if int(package['id']) - 1 != self.id_count_in:
                    print('Some packages got lost (ID missmatch during bundeling). Is: ' +
                          str(int(package['id'])) + ' Ought: ' +
                          str(int(self.id_count_in + 1)))
                    sys.stdout.flush()
                self.id_count_in = int(package['id'])
                
            element = {'data': np.concatenate(packages),
                       'id': self.id_count_out,
                       'date': temp_date,
                       'pattern': temp_pattern}
            
            self.queue_out.put(element)
            
            self.id_count_out += 1