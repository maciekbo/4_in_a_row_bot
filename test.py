"""
    This deep network uses Theano with GPU acceleration support.
    I am using Ubuntu 16.04 with CUDA 7.5.
    Tutorial:
    http://deeplearning.net/software/theano/install_ubuntu.html#install-ubuntu
    The following command will update only Theano:
        sudo pip install --upgrade --no-deps theano
    The following command will update Theano and Numpy/Scipy (warning bellow):
        sudo pip install --upgrade theano
"""

"""
    Below, there is a testing function to check whether your computations have been made on CPU or GPU.
    If the result is 'Used the cpu' and you want to have it in gpu,     do the following:
    1) install theano:
        sudo python3.5 -m pip install Theano
    2) download and install the latest cuda:
        https://developer.nvidia.com/cuda-downloads
        I had some issues with that, so I followed this idea (better option is to download the 1,1GB package as .run file):
        http://askubuntu.com/questions/760242/how-can-i-force-16-04-to-add-a-repository-even-if-it-isnt-considered-secure-eno
        You may also want to grab the proper NVidia driver, choose it form there:
        System Settings > Software & Updates > Additional Drivers.
    3) should work, run it with:
        THEANO_FLAGS=mode=FAST_RUN,device=gpu,floatX=float32 python3.5 test.py
        http://deeplearning.net/software/theano/tutorial/using_gpu.html
    4) Optionally, you can add cuDNN support from:
        https://developer.nvidia.com/cudnn
"""
def testTheano():
    from theano import function, config, shared, sandbox
    import theano.tensor as T
    import numpy
    import time
    print("Testing Theano library...")
    vlen = 10 * 30 * 768  # 10 x #cores x # threads per core
    iters = 1000

    rng = numpy.random.RandomState(22)
    x = shared(numpy.asarray(rng.rand(vlen), config.floatX))
    f = function([], T.exp(x))
    print(f.maker.fgraph.toposort())
    t0 = time.time()
    for i in range(iters):
        r = f()
    t1 = time.time()
    print("Looping %d times took %f seconds" % (iters, t1 - t0))
    print("Result is %s" % (r,))
    if numpy.any([isinstance(x.op, T.Elemwise) for x in f.maker.fgraph.toposort()]):
        print('Used the cpu')
    else:
        print('Used the gpu')
# Perform check:
#testTheano()


# ----------------------
# - network3.py example:
import network3
from network3 import Network, ConvPoolLayer, FullyConnectedLayer, SoftmaxLayer # softmax plus log-likelihood cost is more common in modern image classification networks.

# read data:
training_data, validation_data, test_data = network3.load_data_shared()
# mini-batch size:
mini_batch_size = 10

# chapter 6 - shallow architecture using just a single hidden layer, containing 100 hidden neurons.
'''
net = Network([
    FullyConnectedLayer(n_in=784, n_out=100),
    SoftmaxLayer(n_in=100, n_out=10)], mini_batch_size)
net.SGD(training_data, 60, mini_batch_size, 0.1, validation_data, test_data)
'''

# chapter 6 - 5x5 local receptive fields, 20 feature maps, max-pooling layer 2x2
'''
net = Network([
    ConvPoolLayer(image_shape=(mini_batch_size, 1, 28, 28),
                  filter_shape=(20, 1, 5, 5),
                  poolsize=(2, 2)),
    FullyConnectedLayer(n_in=20*12*12, n_out=100),
    SoftmaxLayer(n_in=100, n_out=10)], mini_batch_size)
net.SGD(training_data, 60, mini_batch_size, 0.1, validation_data, test_data)
'''

# chapter 6 - inserting a second convolutional-pooling layer to the previous example => better accuracy
'''
net = Network([
    ConvPoolLayer(image_shape=(mini_batch_size, 1, 28, 28),
                  filter_shape=(20, 1, 5, 5),
                  poolsize=(2, 2)),
    ConvPoolLayer(image_shape=(mini_batch_size, 20, 12, 12),
                  filter_shape=(40, 20, 5, 5),
                  poolsize=(2, 2)),
    FullyConnectedLayer(n_in=40*4*4, n_out=100),
    SoftmaxLayer(n_in=100, n_out=10)], mini_batch_size)
net.SGD(training_data, 60, mini_batch_size, 0.1, validation_data, test_data)
'''

# chapter 6 -  rectified linear units and some l2 regularization (lmbda=0.1) => even better accuracy
from network3 import ReLU
net = Network([
    ConvPoolLayer(image_shape=(mini_batch_size, 1, 28, 28),
                  filter_shape=(20, 1, 5, 5),
                  poolsize=(2, 2),
                  activation_fn=ReLU),
    ConvPoolLayer(image_shape=(mini_batch_size, 20, 12, 12),
                  filter_shape=(40, 20, 5, 5),
                  poolsize=(2, 2),
                  activation_fn=ReLU),
    FullyConnectedLayer(n_in=40*4*4, n_out=100, activation_fn=ReLU),
    SoftmaxLayer(n_in=100, n_out=10)], mini_batch_size)
net.SGD(training_data, 60, mini_batch_size, 0.03, validation_data, test_data, lmbda=0.1)