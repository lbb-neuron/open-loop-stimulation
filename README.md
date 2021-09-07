## Investigating stimulation dependent activity of biological neuronal networks

The here presented code can be used to stimulate and record the activity of biological neuronal networks. The code has been written for the MEA2100 IFB (version 3) using 60 electrode mini headstages (MEA2100-Mini-60) from Multi Channel Systems.

This code is an adaptation of the example code generously provided by Multi Channel Systems (Multi Channel Systems MCS GmbH, Germany). The original example code can be found [here](https://github.com/multichannelsystems/McsDspRealtimeFeedback).

### Simulation_Software

Contains the online part of the software package. The code in **CSharp** needs to be run in order to start the software. This software will start a python server which does the online data analysis (code in **Python**) and uploads the firmware to the interface board (DSP). The code for the interface board can be found in **DSP_C**. If you would like to compile it from source, some libraries provided by Multi Channel Systems may be required. A binary of the compiled code is provided.

### Post_Processing

Contains the offline part of the software package. First, the data needs to be transformad into a denser format. For this, we provide an example script in **Transformation** that achieves this. Afterwards, the data is processed in 6 steps (**Processing**).
