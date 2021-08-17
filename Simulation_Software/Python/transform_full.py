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
    net = [[0,1,2,3,4,5,6,7,8,9,
            11,11,12,13,14,15,16,17,18,19,
            21,21,22,23,24,25,26,27,28,29,
            31,31,32,33,34,35,36,37,38,39,
            41,41,42,43,44,45,46,47,48,49,
            51,51,52,53,54,55,56,57,58,59]]
    
    # This part should not be changed
    network = []
    for i in range(len(net)):
        pattern = (patterns[i//4] >> ((i%4)*8)) & 0xff
        
        s       = []
        for j in range(len(net[0])):
            s.append(spikes[net[i][j]])
        network.append({'spikes': s, 'pattern': pattern})
    return network
            
