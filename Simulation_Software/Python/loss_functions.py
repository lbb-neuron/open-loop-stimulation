# This is a collection of loss functions. They all have the same layout:
# input:  list containing n lists, where n is the number of electrodes. Inside, you have the spike times of each electrode
# output: a single value (setR)

import numpy as np

def maximal_activity(spikes):
    counts = [len(spikes[i]) for i in range(len(spikes))]
    return np.mean(np.array(counts))

def minimal_activity(spikes):
    counts = [len(spikes[i]) for i in range(len(spikes))]
    return -np.mean(np.array(counts))

def circular_activity(spikes):
    loc    = []
    for i in range(4):
        for j in spikes[i]:
            loc.append(np.array([i,j]))
    loc    = np.array(loc)
    try:
        loc    = loc[np.argsort(loc[:,1]),:][:,0]
        reward = 0
        for i in range(1,loc.shape[0]):
            diff = (loc[i]-loc[i-1]+4) % 4
            if diff == 1:
                reward += 1
            elif diff == 3:
                reward -= 1
        return reward
    except:
        return 0
    
def counter_circular_activity(spikes):
    return -circular_activity(spikes)
    
def random_loss(spikes):
    return np.random.randn(1)