import numpy as np
import multiprocessing

import time
from calendar import timegm
from datetime import datetime

import sys

class SegmentData:
    def __init__(self,n,thr,ipp,isp,fs,num_stim,queue_in,queue_pattern,queue_out):
        self.thd_cut       = multiprocessing.Process(target=self.start_process, args=[n,thr,ipp,isp,fs,num_stim,queue_in,queue_pattern,queue_out])
        self.thd_cut.start()
    
    def start_process(self,n,thr,ipp,isp,fs,num_stim,queue_in,queue_pattern,queue_out): 
        self.n             = n                          # Number of headstages (hardcoded)
        
        self.thr           = thr
        self.ipp           = ipp
        self.isp           = isp
        self.fs            = fs
        self.num_stim      = num_stim
        self.queue_in      = queue_in
        self.patterns      = queue_pattern
        self.queue_out     = queue_out
        
        self.id_count      = 0
        self.stim_count    = 0
        self.buffer        = np.zeros((0,60*self.n+2))  # Last 2 dimension (n*60.th) are package id and digital inputs
        self.started_stim  = False
        self.std_delay     = 503.                       # ToDo: Do not hardcode this
        self.error_del     = 10
        self.flag_thr      = True
        
        self.old_stim_id   = 1
        self.id_error      = 10
        
        self.cut_into_events()
        
    def cut_into_events(self):
        sig_length         = int(self.ipp*self.fs/1000.)
        isp_length         = int(self.isp*self.fs/1000.)
        spike_length       = int(0.001*self.fs)
        exp_length         = sig_length - self.num_stim*isp_length
        while True:
            while self.queue_in.qsize() == 0:
                None
            element        = self.queue_in.get()
            
            queue_element  = element
            data           = np.reshape(element['data'],[-1,64*self.n+1]) # +1 for the DigIn
            digIn          = data[:,64*self.n]
            for i in range(1,self.n):
                data[:,60*i:60*(i+1)] = data[:,64*i:64*i+60]
            data           = data[:,:60*self.n+2]
            data[:,60*self.n]   = element['pattern'][0]
            data[:,60*self.n+1]   = digIn
            
            # Check if age of packages checks out
            t_diff = time.time() - timegm(time.strptime(element['date'], '%Y-%m-%d %H:%M:%S')) - time.timezone
            if t_diff > 10:
                print('Package is more than 10 sec old. Age: -' + 
                      str(np.round(t_diff,2)) +
                      ' sec')
                sys.stdout.flush()
            
            # Check if package id adds up
            if self.id_count + 1 != int(element['id']):
                print('Some packages got lost (ID missmatch). Is: ' +
                      str(int(element['id'])) + ' Ought: ' +
                      str(int(self.id_count + 1)))
                sys.stdout.flush()
            self.id_count    = int(element['id'])

            # Add new pattern to list
            if self.stim_count != data[0,60*self.n]:
                self.patterns.put(element['pattern'])
            
            # Check if stimulation pattern is getting lost
            if self.stim_count - data[0,60*self.n] > 1:
                print('Some stimulation patterns got lost. Loss count: ' +
                      str(int(self.stim_count - data[0,60*self.n]-1)))
                sys.stdout.flush()
            self.stim_count  = data[0,60*self.n]
            
            self.buffer      = np.concatenate([self.buffer, data],0)

            if self.buffer.shape[0] > sig_length * 2:
                #This means at least one pattern should be in here
                mask         = self.buffer[:,-1]
                element      = np.argmax(mask)
                if not mask[element]:
                    if not self.started_stim:
                        if self.flag_thr:
                            self.thr[:]   = (np.median(np.abs(self.buffer[1:,:60*self.n]-self.buffer[:-1,:60*self.n]),0)/(0.6745*np.sqrt(2)))
                            self.flag_thr = False
                        else:
                            self.thr[:]   = (np.median(np.abs(self.buffer[1:,:60*self.n]-self.buffer[:-1,:60*self.n]),0)/(0.6745*np.sqrt(2)))*0.01 + 0.99 * np.array(self.thr[:])
                    self.buffer = np.zeros((0,60*self.n+2))
                    continue
                    
                flag  = False
                mask  = mask[element:]
                stims = np.array(np.argwhere(mask))[:,0]
                for i in range(stims.shape[0]-1):
                    # This finds the first stim event, where there are at least exp_length datapoints until the next event.
                    if stims[i+1] - stims[i] > exp_length:
                        end_stim = stims[i] + 1 + element
                        break
                    if i == stims.shape[0]-2:
                        print('No stimulation in buffer.')
                        sys.stdout.flush()
                        self.buffer = self.buffer[max(0,element-1):,:]
                        flag = True
                if flag:
                    continue
                        
                if end_stim + exp_length > self.buffer.shape[0]:
                    print('Not long enough. This should never happen.')
                    sys.stdout.flush()
                    self.buffer = self.buffer[max(0,element-1):,:]
                    continue
                
                if abs(self.std_delay - end_stim) > 2:
                    self.error_del += 1
                    if self.error_del < 3:
                        print('Shift in prediction. Ignore')
                        sys.stdout.flush()
                        end_stim = int(self.std_delay + 0.5)
                    else:
                        print('Shift in prediction. Accept and adapt shift!!!')
                        print('Shift: ' + str(end_stim))
                        sys.stdout.flush()
                        self.std_delay = end_stim
                else:
                    self.error_del = 0
                    # This allows to have also cases with shifts between two integers. Unclear, if this can happen.
                    self.std_delay = end_stim*0.1 + self.std_delay*0.9 
                    
                # Check, if ids check out.
                current_stim_id    = int(self.buffer[end_stim,60*self.n]+0.5)
                if current_stim_id != 1 + self.old_stim_id:
                    self.id_error += 1
                    if self.id_error > 6:
                        print(str(int(self.id_error-1)) + ' package ID errors in a row. (Adapt)')
                        print("Is: " + str(current_stim_id) + " | Ought: "+ str(self.old_stim_id+1))
                        sys.stdout.flush()
                        self.old_stim_id = current_stim_id - 1
                    elif self.id_error > 3:
                        print(str(int(self.id_error-1)) + ' package ID errors in a row. (Ignore)')
                        sys.stdout.flush()
                    current_stim_id = 1 + self.old_stim_id
                    self.buffer[end_stim:(end_stim+exp_length),60*self.n] = current_stim_id
                else:
                    self.id_error   = 0
                self.old_stim_id   += 1
                
                # Now, everything is fine and element can be added to next queue
                self.queue_out.put(self.buffer[end_stim:(end_stim+exp_length),:])
                self.buffer = self.buffer[(end_stim+exp_length):,:]
                                
                self.started_stim = True