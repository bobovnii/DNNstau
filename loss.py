import numpy as np
import keras.backend as K
import math
import tensorflow as tf
import pandas as pd


def asimov(true, predicted, weight):
    """
    Asimov Significance

    :param true:
    :param predicted:
    :param weight:
    :return:
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

#TODO add to config loss!
def significanceLoss(expectedSignal,expectedBkgd):
    '''Define a loss function that calculates the significance based on fixed
    expected signal and expected background yields for a given batch size'''
    "Loss From Adam https://github.com/aelwood/hepML/blob/master/MlFunctions/DnnFunctions.py"

    def sigLoss(y_true,y_pred):
        #Continuous version:

        signalWeight=expectedSignal/K.sum(y_true)
        bkgdWeight=expectedBkgd/K.sum(1-y_true)

        s = signalWeight*K.sum(y_pred*y_true)
        b = bkgdWeight*K.sum(y_pred*(1-y_true))

        return (s+b)/(s*s+K.epsilon()) #Add the epsilon to avoid dividing by 0

    return sigLoss


def significance(true, predicted, weigth):
    """

    :param true:
    :param false:
    :return:
    """
    return


def mywloss(y_true,y_pred):
    # https://www.kaggle.com/c/PLAsTiCC-2018/discussion/69795

    yc=tf.clip_by_value(y_pred,1e-15,1-1e-15)
    loss=-(tf.reduce_mean(tf.reduce_mean(y_true*tf.log(yc),axis=0)/wtable))
    return loss

