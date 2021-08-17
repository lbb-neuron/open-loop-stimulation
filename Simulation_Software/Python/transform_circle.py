import numpy as np

def transform(spikes,patterns):
    # Transform the spikes and patterns from the raw electrode into the circuit data and return it.
    #
    # Input:  Spikes:   List (60 elements) containing a list of spikes for each electrode
    #         patterns: List (4 elements) Each containing the pattern used for 4 circuits. Up to 16 in total. Order is as follows:
    #                   Index 0: [(Circuit_3 << 8*3) + (Circuit_2 << 8*2) + (Circuit_1 << 8*1) + (Circuit_0 << 8*0)]
    #                   Index 1: [(Circuit_7 << 8*3) + (Circuit_6 << 8*2) + (Circuit_5 << 8*1) + (Circuit_4 << 8*0)]
    #                   Index 2: [(Circuit_B << 8*3) + (Circuit_A << 8*2) + (Circuit_9 << 8*1) + (Circuit_8 << 8*0)]
    #                   Index 3: [(Circuit_F << 8*3) + (Circuit_E << 8*2) + (Circuit_D << 8*1) + (Circuit_C << 8*0)]
    #                   If not all circuits are used, the most significant circuits are 0.
    #
    # Output: list      Containing list of length #circuits. Each element is a dictionary due to backwards compatibility reasons.
    #                   Dictionary:    pattern     0-255. Pattern id used to stimulate. If standard values used: 0-127. It is the 
    #                                              responsibility of the higher level programs to know, what the corresponding pattern
    #                                              looks like.
    #                                  spikes      List of len #electrodes_per_circuit. Each list is a list containing the spike of
    #                                              the corresponding electrodes
    
    # Electrode encoding. This should be the only part being changed for different network architectures
    net = [[1,11,10,0],
           [21,31,30,20],
           [41,51,50,40],
           [3,13,12,2],
           [23,33,32,22],
           [43,53,52,42],
           [5,15,14,4],
           [25,35,34,24],
           [45,55,54,44],
           [7,17,16,6],
           [27,37,36,26],
           [47,57,56,46],
           [9,19,18,8],
           [29,39,38,28],
           [49,59,58,48]]
    
    # This part should not be changed
    network = []
    for i in range(len(net)):
        pattern = (patterns[i//4] >> ((i%4)*8)) & 0xff
        
        s       = []
        for j in range(len(net[0])):
            s.append(spikes[net[i][j]])
        network.append({'spikes': s, 'pattern': pattern})
    return network
            
