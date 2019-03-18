import numpy as np
from metric.loss import asimov

#TODO implement relu and sigmoid

def pseudo_train(X, Y, W, _layers):
    """
    Propagate forward parameters:
    """
    # pass our inputs through our neural network
    a2 = __relu(np.dot(X, _layers['W'][0] - _layers['B'][0]))
    a3 = __relu(np.dot(a2, _layers['W'][1] - _layers['B'][1]))
    output = __sigmoid(np.dot(a3, _layers['W'][2] - _layers['B'][2]))
    output = [i[0] for i in output]
    pred = [1 if i > 0.5 else 0 for i in output]
    _Y = Y.as_matrix()
    _W = W.as_matrix()
    _pred = np.array(pred)
    del4 = asimov(_Y, _pred, _W)
    if del4 == np.nan:
        del4 = 0
    print("Loss", del4)
    return del4


def ES(alpha=0.001, population=5, sigma=0.1, X=None, Y=None, W=None, steps=2, layers=None):
    """
    Make Evolutional Strategy
    """
    # for step in range(steps):
    W_SHIFT = [  ]  # np.random.randn(population, 1)
    B_SHIFT = [  ]  # np.random.randn(population, 1)
    for i in range(population):
        for layer_id in range(len(layers['W'])):
            print("Layer ID", layer_id)
            w  = layers["W"][layer_id]
            b  = layers["B"][layer_id]
            w_shape = w.shape
            b_shape = b.shape
            R = np.zeros(population)
            w_shift = np.random.normal(loc=0.0, scale=1.0, size=w_shape)
            b_shift = np.random.normal(loc=0.0, scale=1.0, size=b_shape)
            W_SHIFT.append(w_shift)
            B_SHIFT.append(b_shift)
            layers['W'][layer_id] = layers['W'][layer_id ] +w_shift
            layers['B'][layer_id] = layers['B'][layer_id ] +b_shift
            # evaluate loss
            R[i] = pseudo_train(X, Y, W, layers)
        # layers['W'][layer_id] -= w_shift
        # layers['B'][layer_id]-= b_shift
        # Update hyperparamer
        # A = (R - np.mean(R)) / np.std(R)
        # layer[_i, _j] = w + alpha/(population*sigma) * np.dot(N.T, A)
        # loss = self.pseudo_train(training_set_inputs, training_set_outputs)
        # loss_history.append(np.mean(loss))
    return