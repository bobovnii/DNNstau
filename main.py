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
from Plotter import Plotter

np.random.seed(7)



gen_VARS = ["gen_met_pt", "Lept1Pt", "Lept2Pt", "EtaDil",
            "MTtot","genDzeta","dR" ,"Minv", "genMT", "MCTb", "genMT2lester",
            "gendMETPhiL1", "gendMETPhiL2"]

VARS = ["met_pt", "Lept1Pt", "Lept2Pt", "EtaDil",
        "MTtot","Dzeta","dR" ,"Minv", "MT", "MCTb",
        "MT2lester", "dMETPhiL1", "dMETPhiL2"]


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


plotter = Plotter("/Users/dydukhle/.kaggle/stau_2016/")
plotter.train_test_plot(_df_train, _df_test, bgd_train_sf, bgd_test_sf, signal_train_sf, signal_test_sf)
plotter.significance_plot(_df_train, _df_test, bgd_train_sf, bgd_test_sf, signal_train_sf, signal_test_sf)
plotter.roc_curve(_df_train, _df_test)