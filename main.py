from keras.optimizers import Adam
import uproot
import pandas as pd
from keras.models import Sequential
from keras.layers import Dense
from keras.models import model_from_json
import numpy as np
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
np.random.seed(7)


#Load data:
def load_data():
    """
    """
    return

#Prepare data:
def preprocess():
    """
    """
    return

#Train:
def train():
    """
    """
    return
    
#Test:
def test():
    """
    """
    return

#Save result: