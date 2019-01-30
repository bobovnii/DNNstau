import numpy as np
import keras.backend as K
from itertools import product
import math
import tensorflow as tf


def Asimov_Loss(y_true, y_pred, weights):
    """
    """
    # Get weighted true positive;
    # Get weighted false negative;
    # Calculate AMS;
    new_y_pred = y_pred
    new_y_true = y_true
    # new_y_pred = [i[0] for i in y_pred]
    # new_y_true = [i[0] for i in y_true.as_matrix()]#np.array
    new_weight = weights  # .as_matrix()
    nb_cl = len(weights)
    final_mask = K.zeros_like(y_pred[:, 0])
    y_pred_max = K.max(y_pred, axis=1)
    y_pred_max = K.reshape(y_pred_max, (K.shape(y_pred)[0], 1))
    y_pred_max_mat = K.equal(y_pred, y_pred_max)
    for c_p, c_t in product(range(nb_cl), range(nb_cl)):
        final_mask += (weights[c_t, c_p] * y_pred_max_mat[:, c_p] * y_true[:, c_t])

    TP = np.sum(np.logical_and(new_y_pred == 1, new_y_true == 1).astype(np.int32 ) *final_mask )
    FP = np.sum(np.logical_and(new_y_pred == 1, new_y_true == 0).astype(np.int32 ) *final_mask )



    if TP == 0:
        asimov = 0.0
    else:
        asimov = TP /np.sqrt(FP +TP)
    return asimov


w_table = [expectedSignal, expectedBkgd]

# https://www.kaggle.com/c/PLAsTiCC-2018/discussion/69795
def mywloss(y_true,y_pred):
    yc=tf.clip_by_value(y_pred,1e-15,1-1e-15)
    loss=-(tf.reduce_mean(tf.reduce_mean(y_true*tf.log(yc),axis=0)/wtable))
    return loss


def asimov_loss(y_true,y_pred):
    """

    :param expectedSignal:
    :param expectedBkgd:
    :return:
    """
    signalWeight = expectedSignal / K.sum(y_true)
    bkgdWeight = expectedBkgd / K.sum(1 - y_true)

    s = signalWeight * K.sum(y_pred * y_true)
    b = bkgdWeight * K.sum(y_pred * (1 - y_true))

    return s/math.sqrt(s+b)



def significanceLoss(expectedSignal,expectedBkgd):
    '''Define a loss function that calculates the significance based on fixed
    expected signal and expected background yields for a given batch size'''


    def sigLoss(y_true,y_pred):
        #Continuous version:

        signalWeight=expectedSignal/K.sum(y_true)
        bkgdWeight=expectedBkgd/K.sum(1-y_true)

        s = signalWeight*K.sum(y_pred*y_true)
        b = bkgdWeight*K.sum(y_pred*(1-y_true))

        return -(s*s)/(s+b+K.epsilon()) #Add the epsilon to avoid dividing by 0

    return sigLoss