import server
import transform

import time
from calendar import timegm
import datetime

import h5py
import numpy as np

import sys

# Needed for testing
from scipy import signal as sg
from scipy.signal import find_peaks as fp
import multiprocessing
from joblib import Parallel, delayed

# Ordering parameter
# 0: t_port     port number to be used for data transformation. Note: There are more ports than that
# 1: n_port     number of ports
# 2: auth_key   password used to access the queues through the network
# 3: s_port     port number of server
  
# 4: fs         sampling frequency [Hz]
# 5: f_high     high-pass frequency [Hz]
# 6: ipp        inter pattern period (isi in c#) [us]
# 7: isp        inter spike period (isp in c#) [ms]
# 8: num_stim   number of stimuli per pattern
  
# 9: SC1        is not 0 if SCU1 is used
#10: SC2        is not 0 if SCU2 is used
#11: num_elec   for ever headstage (8 in total) gives a value of the type of pattern used
#               Code:   0: No structure
#                       1: 5x3  o circles
#                       2: 10+2 / lines
#                       3: 10+2 \ lines


def main(argv):
    
    print('Reading out data ... ',end = "")
    sys.stdout.flush()
    t_host       = '127.0.0.1'
    t_port       = int(argv[0])
    n_port       = int(argv[1])
    auth_key     =     argv[2]
    s_port       = int(argv[3])
    
    fs           = int(argv[4])
    f_high       = int(argv[5])
    ipp          = int(argv[6])       # inter_pattern_period
    isp          = int(argv[7])//1000 # inter_spike_period
    num_stim     = int(argv[8])
    
    SCU1_used    = int(argv[9])
    SCU2_used    = int(argv[10])
        
    hs_type      = argv[11].split(',')
    hs_type      = [int(i) for i in hs_type]
    
    print('OK')
    sys.stdout.flush()
    
    s            = server.Server(t_host,t_port,n_port)

    print('Start transformer ... ',end = "")
    sys.stdout.flush()
    t            = transform.Transform(s, n_port, s_port, auth_key, fs, f_high, num_stim, hs_type, ipp, isp)
    print('OK')
    sys.stdout.flush()

if __name__ == "__main__":
    print(sys.argv)
    sys.stdout.flush()
    main(sys.argv[1:])