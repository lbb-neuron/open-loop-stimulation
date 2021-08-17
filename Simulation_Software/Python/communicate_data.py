import numpy as np
import multiprocessing
from multiprocessing.managers import BaseManager

import sys

class ComData:
    def __init__(self,n,s_port,auth_key,queue_nets,queue_out,std_thr):
        self.thd_bdl       = multiprocessing.Process(target=self.start_process, args=[n,s_port,auth_key,queue_nets,queue_out,std_thr])
        self.thd_bdl.start()
    
    def start_process(self,n,s_port,auth_key,queue_nets,queue_out,std_thr): 
        self.n              = n                          # Number of headstages
        self.s_port         = s_port
        self.auth_key       = auth_key.encode("utf-8")
        self.queue_nets     = queue_nets
        self.queue_out      = queue_out
        self.std_thr        = std_thr
        
        self.queue_software = multiprocessing.Queue()
        self.thread         = multiprocessing.Process(target=self.update_threshold, args=[self.queue_software,self.std_thr])
        self.thread.start()
        
        self.communicate()
    
    def update_threshold(self,queue_software,std_thr):
        while True:
            value = queue_software.get()
            std_thr.value = value
            print('Updated Threshold: ' + str(value) + ' STDs')
            sys.stdout.flush()
                
    def communicate(self):
#         For whatever reason, this approach makes all queues to be the last one. Hardcoding solves that issue
#         for i in range(self.n):
#             i_temp = int(i*1.0);
#             BaseManager.register('queue_' + str(i_temp), callable=lambda: self.queue_nets[i_temp])
        if self.n == 4:
            BaseManager.register('queue_0', callable=lambda:self.queue_nets[0])
            BaseManager.register('queue_1', callable=lambda:self.queue_nets[1])
            BaseManager.register('queue_2', callable=lambda:self.queue_nets[2])
            BaseManager.register('queue_3', callable=lambda:self.queue_nets[3])
        else:
            assert self.n == 4
        BaseManager.register('command_queue',  callable=lambda:self.queue_out)
        BaseManager.register('queue_software', callable=lambda:self.queue_software)
        m = BaseManager(address=('127.0.0.1', self.s_port), authkey=self.auth_key)
        s = m.get_server()
        
        while True:
            print('Setting up server complete')
            sys.stdout.flush()
            s.serve_forever()