import numpy as np
import keras.backend as K
from itertools import product
import math
import tensorflow as tf
import pandas as pd
import keras

def asimov(true, predicted, weight):
    """
    """
    result = pd.DataFrame()
    predicted = np.round(predicted)
    result['pred_label'] = [i[0] for i in predicted]#.as_matrix()
    result['true'] = [i[0] for i in true]
    result['weight'] = weight.as_matrix()
    sign = result[(result['true']==1)]
    bgd = result[(result['true']==0)]
    TP = sign[(sign['true'] == sign['pred_label'])].weight.sum()
    FP = bgd[(bgd['true'] != bgd['pred_label'])].weight.sum()
    asimov = TP/math.sqrt(TP+FP)
    return asimov

"""
def sigLoss(y_true,y_pred, weights):
    #Continuous version:

    signalWeight=weights/K.sum(y_true)
    bkgdWeight=weights/K.sum(1-y_true)

    s = signalWeight*K.sum(y_pred*y_true)
    b = bkgdWeight*K.sum(y_pred*(1-y_true))

    return -(s*s)/(s+b+K.epsilon()) #Add the epsilon to avoid dividing by 0
"""

def significance(true, predicted, weigth):
    """

    :param true:
    :param false:
    :return:
    """
    return

class Histories(keras.callbacks.Callback):
    def set_up_weight(self, weight):
        self.new_weight = weight
    def on_train_begin(self, logs={}):
        self.aucs = []
        self.losses = []
        self.asimov = []

    def on_train_end(self, logs={}):
        return

    def on_epoch_begin(self, epoch, logs={}):
        return

    def on_epoch_end(self, epoch, logs={}):
        self.losses.append(logs.get('loss'))
        y_pred = self.model.predict(self.validation_data[0])
        #self.aucs.append(roc_auc_score(self.validation_data[1], y_pred))
        #self.data = [self.validation_data[1],
        #                               y_pred,
        #                               self.new_weight]
        self.asimov.append(asimov(self.validation_data[1],
                                       y_pred,
                                       self.new_weight))
        return

	def on_batch_begin(self, batch, logs={}):
		return

	def on_batch_end(self, batch, logs={}):
		return


def mywloss(y_true,y_pred):
    # https://www.kaggle.com/c/PLAsTiCC-2018/discussion/69795

    yc=tf.clip_by_value(y_pred,1e-15,1-1e-15)
    loss=-(tf.reduce_mean(tf.reduce_mean(y_true*tf.log(yc),axis=0)/wtable))
    return loss

