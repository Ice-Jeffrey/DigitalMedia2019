import numpy as np
#import tensorflow as tf
import tensorflow.compat.v1 as tf
tf.disable_v2_behavior()
from tensorflow.examples.tutorials.mnist import input_data

mnist_data=input_data.read_data_sets("MNIST_data",one_hot=True)
batch_size=100
batch_num=mnist_data.train.num_examples//batch_size

x = tf.placeholder(tf.float32,[None,784])
y = tf.placeholder(tf.float32,[None,10])

weights = {
    'hidden_1': tf.Variable(tf.random_normal([784, 256])),
    'out': tf.Variable(tf.random_normal([256, 10]))
}
biases = {
    'b1': tf.Variable(tf.random_normal([256])),
    'out': tf.Variable(tf.random_normal([10]))
}

def neural_network(x):
    hidden_layer_1 = tf.add(tf.matmul(x, weights['hidden_1']), biases['b1'])
    out_layer = tf.matmul(hidden_layer_1, weights['out']) + biases['out']
    return out_layer

result = neural_network(x)
prediction = tf.nn.softmax(result)
loss = tf.reduce_mean(tf.square(y-prediction))
train_step = tf.train.GradientDescentOptimizer(0.2).minimize(loss)
correct_pred = tf.equal(tf.argmax(y,1),tf.argmax(prediction,1))
accuracy = tf.reduce_mean(tf.cast(correct_pred,tf.float32))

init = tf.global_variables_initializer()

step_num=40000
with tf.Session() as sess:
    sess.run(init)
    for step in range(step_num+1):
        for batch in range(batch_num):
            batch_x,batch_y =  mnist_data.train.next_batch(batch_size)
        sess.run(train_step,feed_dict={x:batch_x,y:batch_y})
        acc = sess.run(accuracy,feed_dict={x:mnist_data.test.images,y:mnist_data.test.labels})
        if(step % 100 == 0):
            print("Step " + str(step) + ",Training Accuracy "+ "{:.3f}" + str(acc))
            print("Finished!")