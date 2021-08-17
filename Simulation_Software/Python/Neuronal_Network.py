from os import environ as cuda_environment
import setproctitle
import tensorflow as tf
import multiprocessing
import numpy as np
import os

class ANN:
    def __init__(self,queue_data,name,gpu_id,network_index,num_stimulus,num_electrodes,patterns):
        self.ann = multiprocessing.Process(target=self.start_process, args=[queue_data,name,gpu_id,network_index,num_stimulus,num_electrodes,patterns])
        self.ann.start()
        
    def start_process(self,queue_data,name,gpu_id,network_index,num_stimulus,num_electrodes,patterns):
        setproctitle.setproctitle("IHLE:  CL " + str(network_index))
        cuda_environment["CUDA_VISIBLE_DEVICES"] = str(gpu_id)
        cuda_environment['TF_CPP_MIN_LOG_LEVEL'] = '2' # Do not print warning/info
        
        self.queue_data       = queue_data
        self.network_index    = network_index
        
        self.patterns         = patterns
        # [0:128]    pattern ids
        # [128:256]  rewards for these ids
        # [256]      1: asking for new patterns || 0: do not create new patterns
        
        self.config                                             = tf.ConfigProto()
        self.config.gpu_options.allow_growth                    = True
        self.config.gpu_options.per_process_gpu_memory_fraction = 1./4
        
        self.name             = name          # Should be name + subdirectories
        self.input_dim        = num_stimulus*num_electrodes
        self.num_stimulus     = num_stimulus
        self.num_electrodes   = num_electrodes
        
        self.graph            = tf.Graph()
        self.create_network()
        
        self.history_X        = []
        self.history_y        = []
        
        with self.graph.as_default():
            self.saver    = tf.train.Saver()
                
        self.run()
                        
    def run(self):
        while(True):
            self.train()
            if int(float(self.patterns[256])+0.5) == 1:
                self.get_new_pattern()
        
    def create_network(self):
        with self.graph.as_default():
            self.X        = tf.placeholder(tf.float32,[None,self.input_dim])
            self.y        = tf.placeholder(tf.float32,[None])
            
            self.h1       = tf.keras.layers.Dense(32,name="layer_1_"+str(self.network_index))(self.X)
            self.h1       = tf.nn.relu(self.h1)
            self.h2       = tf.keras.layers.Dense(32,name="layer_2_"+str(self.network_index))(self.h1)
            self.h2       = tf.nn.relu(self.h2)

            self.out      = tf.squeeze(tf.keras.layers.Dense(1,name="layer_2_"+str(self.network_index))(self.h2),-1)

            self.loss     = tf.reduce_mean(tf.square(self.y-self.out))
            
            optimizer_gen = tf.train.AdamOptimizer()
            self.opt_gen  = optimizer_gen.minimize(self.loss)
    
    def save(self,sess):
        self.saver.save(sess,self.name + "/Models/model_" + str(self.network_index) + ".ckpt")
            
    def init(self,sess):
        if os.path.exists(self.name + "/Models/model_" + str(self.network_index) + ".ckpt.meta"):
            self.load(sess)
        else:
            if not os.path.exists(self.name + "/Models/"):
                os.mkdir(self.name + "/Models/")
            sess.run(tf.global_variables_initializer())
    
    def load(self,sess):
        self.saver.restore(sess, self.name + "/Models/model_" + str(self.network_index) + ".ckpt")
    
    def train(self):
        X = []
        y = []
        while len(X) < 64:
            element = self.queue_data.get()
            X.append(element['X'])
            y.append(element['y'])
        X = np.stack(X)
        y = np.stack(y)
        if len(self.history_X) >= 64:
            p  = np.random.permutation(len(self.history_X))
            X1 = [self.history_X[i] for i in p[:64]]
            y1 = [self.history_y[i] for i in p[:64]]
            X1 = np.stack(X1)
            y1 = np.stack(y1)
            while len(self.history_X) > 1024:
                del self.history_X[0]
                del self.history_y[0]
            X  = np.concatenate([X,X1],0)
            y  = np.concatenate([y,y1],0)
            
        with tf.Session(config=self.config,graph=self.graph) as sess:
            self.init(sess)
            sess.run(self.opt_gen,feed_dict={self.X: X, self.y: y})
            self.save(sess)
  
    def id_to_pattern(self,stim_id):
        # Convert id to pattern
        # See also pattern_to_id(...)
        pattern = np.zeros(self.num_stimulus)
        for i in range(self.num_stimulus):
            pattern[self.num_stimulus-1-i] = stim_id % (self.num_electrodes+1)
            stim_id = stim_id // (self.num_electrodes+1)
        return pattern
    
    def id_to_input(self,pattern):
        pattern  = self.id_to_pattern(pattern)
        vec      = np.zeros(self.num_stimulus*self.num_electrodes)
        for i in range(self.num_stimulus):
            elec = pattern[i]
            if elec > 0:
                vec[i*self.num_electrodes+int(elec+0.5)-1] = 1
        return vec
    
    def get_new_pattern(self):
        # Old_patterns:  [N,1]
        potential_patterns = np.arange((self.num_electrodes+1)**self.num_stimulus).astype(np.int).tolist()
        i = 0
        while i < len(potential_patterns):
            flag = False
            for k in range(128):
                if potential_patterns[i] == int(float(self.patterns[k])+0.5):
                    flag = True
                    break
            if flag:
                del potential_patterns[i]
            else:
                i += 1
        X = np.array([self.id_to_input(i) for i in potential_patterns])
        
        # Do the predictions
        with tf.Session(config=self.config,graph=self.graph) as sess:
            self.init(sess)
            y = sess.run(self.out,feed_dict={self.X: X})
        
        # Sort the old/true rewards (descending)
        p             = np.argsort(np.array(self.patterns[128:256]))[::-1]
        new_patterns  = np.array(self.patterns[0:128]).tolist()
        new_patterns  = np.array([new_patterns[p[i]] for i in range(128)])
        new_rewards   = np.array(self.patterns[128:256]).tolist()
        new_rewards   = np.array([new_rewards[p[i]] for i in range(128)])
        
        # Sort the new/pred rewards (descending)
        p             = np.argsort(y)[::-1]
        pred_patterns = np.array([potential_patterns[i] for i in p])
        pred_rewards  = y[p]
        
        index = 0
        for i in range(64):
            if new_patterns[64+i]   < pred_patterns[index]:
                new_patterns[i+65:] = new_patterns[i+64:-1]
                new_rewards[i+65:]  = new_rewards[i+64:-1]
                new_patterns[i+64]  = pred_patterns[index]
                new_rewards[i+64]   = pred_rewards[index]
                index              += 1
        rand_8_patterns             = np.random.permutation(128-index)[:8]+index
        new_patterns[-8:]           = pred_patterns[rand_8_patterns]
        new_rewards[-8:]            = pred_rewards[rand_8_patterns]
                
        self.patterns[0:128]       = np.copy(new_patterns)
        self.patterns[128:256]     = np.copy(new_rewards)
        self.patterns[256]         = 0