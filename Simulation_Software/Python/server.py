# Code adapted from Julian Hengsteler

import sys
import selectors
import socket
import json
import libserver
import numpy as np
import threading
import queue
import time
import multiprocessing

class Server:
    def __init__(self, host, port, num_ports):     
                
        self.queue_in  = multiprocessing.Queue()
        self.queue_out = multiprocessing.Queue() 
        self.num_ports = num_ports
        # This might be changed to multiprocessing.Process(...)
        
        # Open server for each port
        self.thread          = []
        self.queue_n_in      = []
        for i in range(self.num_ports):
            
            print('Start server at port ' + str(port+i) + ' ... ',end = "")
            sys.stdout.flush()
            self.queue_n_in.append(multiprocessing.Queue())
            thread_i = multiprocessing.Process(target=self.communication, args=[host, port, self.queue_n_in, self.queue_out,i])
            self.thread.append(thread_i)
            print('OK')
            sys.stdout.flush()
        
        # Add merger server
        thread_n = multiprocessing.Process(target=self.bundle_queues, args=[self.queue_in, self.queue_n_in, self.num_ports])
        self.thread.append(thread_n)
        
        # Start all threads
        for i in range(len(self.thread)):
            self.thread[i].start()
    
    @staticmethod
    def bundle_queues(queue_in, queue_n_in, num_ports):
        while True:
            for i in range(num_ports):
                queue_in.put(queue_n_in[i].get())
        
    @staticmethod    
    def communication(host, port,queue_in,queue_out,index):        
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        
        sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        sock.bind((host, port+index))
        sock.listen()
        sock.setblocking(False)

        sel = selectors.DefaultSelector()
        sel.register(sock, selectors.EVENT_READ, data=None)
        
        data      = []
        timestamp = []
        
        t0        = time.time()
        
        try:
            while True:
                events = sel.select(timeout=None)
                for key, mask in events:
                    if key.data is None:
                        conn, addr = sock.accept()
                        conn.setblocking(False)
                        message = libserver.Message(sel, conn, addr)
                        sel.register(conn, selectors.EVENT_READ, data=message)
                        print("connecting to ", conn, addr)
                        sys.stdout.flush()
                    else:
                        try:
                            message.process_events(mask)
                            message = key.data
                            if message.request:
                                if message.request["Data"] is not None:
                                    sys.stdout.flush()
                                    element = {'data': np.array(message.request["Data"]),
                                              'id': message.request["timestamp"],
                                              'date': message.request["TimeDate"],
                                              'pattern': np.array(message.request["stimpattern"])}
                                    
                                    queue_in[index].put(element)
                                    if index==0 and queue_out.qsize() > 0 and time.time() - t0 > (1/2.):
                                        # Send a message to DSP
                                        pattern = []
                                        while queue_out.qsize() > 0: # Send the whole queue at once
                                            pattern.append(np.array(queue_out.get()))
                                        if len(pattern) == 0:
                                            print("No patterns send. This should not happen.")
                                            sys.stdout.flush()
                                            # Do not send a message to DSP
                                            message.pattern         = np.zeros(0)
                                            message._jsonheader_len = None
                                            message.request         = None
                                        else:
                                            pattern = np.concatenate(pattern).tolist()
                                            message.pattern         = pattern
                                            message._set_selector_events_mask("w")
                                        t0                      = time.time()
                                    else:
                                        # Do not send a message to DSP
                                        message.pattern         = np.zeros(0)
                                        message._jsonheader_len = None
                                        message.request         = None
                        except (RuntimeError,ConnectionResetError):
                            message.close()
                            conn.close()
                            print('Close connection and intitiate new connection ...')
                            sys.stdout.flush()
        except KeyboardInterrupt:
            print("caught keyboard interrupt, exiting")
            sys.stdout.flush()
            try:
                message.close()
            except:
                pass
        finally:
            try:
                sel.close()
            except:
                pass
    
    def length(self):
        return self.queue_in.qsize()
