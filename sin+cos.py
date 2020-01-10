import matplotlib.pyplot as plt
import tensorflow.compat.v1 as tf
tf.disable_v2_behavior()
import numpy as np
import math
 
def add_layer(inputs, in_size, out_size, activation_function=None):
    # add one more layer and return the output of this layer
    Weights = tf.Variable(tf.random_normal([in_size, out_size]))
    biases = tf.Variable(tf.zeros([1, out_size]))
    Wx_plus_b = tf.matmul(inputs, Weights) + biases
    if activation_function is None:
        outputs = Wx_plus_b
    else:
        outputs = activation_function(Wx_plus_b)
    return outputs

def NeuralNetwork(input_data):
    with tf.variable_scope('hidden1'):
        hidden1 = add_layer(input_data, 1, 16, activation_function=tf.nn.relu)

    with tf.variable_scope('hidden2'):
        hidden2 = add_layer(hidden1, 16, 16, activation_function=tf.nn.relu)

    with tf.variable_scope('output_layer'):
        output = add_layer(hidden2, 16, 1)
    
    return output

def main():
    # Make up some real data
    x_data = np.linspace(- math.pi, math.pi, 300)[:, np.newaxis]
    y_data = np.sin(x_data) + np.cos(x_data)
    
    # define the model
    xs = tf.placeholder(tf.float32, [None, 1])
    ys = tf.placeholder(tf.float32, [None, 1])
    prediction = NeuralNetwork(xs)
    loss = tf.reduce_mean(tf.square(ys - prediction))
    train_step = tf.train.GradientDescentOptimizer(0.01).minimize(loss)
    
    # initialization
    init = tf.initialize_all_variables()
    sess = tf.Session()
    sess.run(init)
    
    # plot the real data
    fig = plt.figure()
    ax = fig.add_subplot(1,1,1)
    ax.scatter(x_data, y_data)
    # Interactive mode on
    plt.ion()
    plt.show()
    
    for i in range(50000):
        # training
        sess.run(train_step, feed_dict={xs: x_data, ys: y_data})
        
        if i % 50 == 0:
            # to visualize the result and remove the previous line 
            try:
                ax.lines.remove(lines[0])
            except Exception:
                pass
            #print(sess.run(loss, feed_dict={xs: x_data, ys: y_data}))
            prediction_value = sess.run(prediction, feed_dict={xs: x_data})
            # plot the prediction
            lines = ax.plot(x_data, prediction_value, 'r-', lw=5)
            plt.pause(0.1)

if __name__ == "__main__":
    main()