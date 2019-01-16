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


def preporcess():
    """
    """
    
    
def store_model(model, name):
    """
    """
    #TODO: def save_model(model):
    # serialize model to JSON
    model_json = model.to_json()
    with open("{0}.json".format(name), "w") as json_file:
        json_file.write(model_json)
    # serialize weights to HDF5
    model.save_weights(name)
    return 


def load_model(model, name):
    """
    TODO:
    """
    return