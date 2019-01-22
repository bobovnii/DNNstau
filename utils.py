"""
Utils:
"""
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import os
import math
import uproot
from tqdm import tqdm
import tensorflow as tf
import sklearn
from sklearn.model_selection import train_test_split
from sklearn import metrics
from sklearn.metrics import roc_auc_score
from sklearn.metrics import confusion_matrix
import os
import math

import logging
from sklearn.model_selection import train_test_split
from sklearn.metrics import accuracy_score
from sklearn.metrics import classification_report
from sklearn.utils import shuffle
from sklearn.metrics import roc_auc_score
from sklearn.metrics import roc_curve
import matplotlib.pyplot as plt
from sklearn.model_selection import KFold

def _overbalance(train):
    """
    Return Oversampled dataset
    """
    count_class_0, count_class_1 = train.classID.value_counts()
    # Divide by class
    df_class_0 = train[train['classID'] == 0]
    df_class_1 = train[train['classID'] == 1]
    df_class_1_over = df_class_1.sample(count_class_0, replace=True)
    df_over = pd.concat([df_class_0, df_class_1_over], axis=0)
    df_over = shuffle(df_over)
    return df_over


def label_correction(df, labels=[1,0], class_names=["signal","background"], col_names=["classID", "className"]):
    """
    """
    #test_row = df[col_names].loc[0].as_matrix()
    #if test_row[0] == labels[0] and test_row[1] == class_names[0]:
    #    pass
    #elif test_row[0] == labels[1] and test_row[1] == class_names[1]:
    #    pass
    #else:
    #    df[col_names[0]] = df[col_names[0]].apply(lambda x: 1 - np.int32(x > 0.5))
    df['classID'] = df['classID'].apply(lambda x: 1-np.int32(x>0.5))
    return df







def get_results(model, _x_train, _y_train, x_test, y_test, _w_train, w_test):
        
    _df_train = pd.DataFrame()
    _df_test = pd.DataFrame()

    _df_train['train_labels'] =  [i[0] for i in _y_train.as_matrix()]
    _df_test['test_labels'] = [i[0] for i in y_test.as_matrix()]
    _df_train['index'] = _x_train.index
        
    _df_train['train_output'] = model.predict(_x_train, verbose=0)
    _df_test['test_output'] = model.predict(x_test, verbose=0)
        
    _df_train['train_weights'] = _w_train.as_matrix()
    _df_test['test_weights'] = w_test.as_matrix()
    
    _df_train['train_pred'] = _df_train['train_output'].apply(lambda x: 1 if x>0.5 else 0)
    _df_test['test_pred'] = _df_test['test_output'].apply(lambda x: 1 if x>0.5 else 0)
    _df_test['index'] = x_test.index
    return _df_train, _df_test