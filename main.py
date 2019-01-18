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
import ConfigParser

from dataloader import DataLoader


np.random.seed(7)



gen_VARS = ["gen_met_pt", "Lept1Pt", "Lept2Pt", "EtaDil",
            "MTtot","genDzeta","dR" ,"Minv", "genMT", "MCTb", "genMT2lester",
            "gendMETPhiL1", "gendMETPhiL2"]

VARS = ["met_pt", "Lept1Pt", "Lept2Pt", "EtaDil",
        "MTtot","Dzeta","dR" ,"Minv", "MT", "MCTb",
        "MT2lester", "dMETPhiL1", "dMETPhiL2"]


LABELS = ["classID"]
WEIGHT = ["weight"]


#Save result:

#Parse config
config = ConfigParser.RawConfigParser()
config.read('config.ini')
dataloader = DataLoader(config)

#Load Data
train =dataloader._get_train()
from utils import label_correction
train = label_correction(train, labels=[1,0], class_names=["signal","background"], col_names=["classID", "className"])
#Test train split
from preprocess import test_train_split
_train, _test = test_train_split(train, split=0.4)

#Preprocess
from utils import _overbalance
ov_train = _overbalance(_train)

X_train = _train[VARS]
Y_train = _train[LABELS]
W_train = _train[WEIGHT]


X_test = _test[VARS]
Y_test = _test[LABELS]
W_test = _test[WEIGHT]

#Start training:
from train import Training

gen_met_trainin = Training(config)
model = gen_met_trainin._model()
#Training:
gen_met_trainin.train(X_train, Y_train, model)
gen_met_trainin.store_model()
gen_met_trainin.evaluate_model(X=X_test, Y=Y_test)
#Test Evaluate

#Get Result of training:
from utils import get_results
_df_train, _df_test = get_results(gen_met_trainin.get_model(), X_train, Y_train, X_test, Y_test, W_train, W_test)

#Extract the SF for signal and background:
from sf import *
Number_of_Background = float(config.get("physics", "Number_of_Background"))
Number_of_Signal = float(config.get("physics","Number_of_Signal"))

bgd_train_sf, bgd_test_sf = sf_bgd_train_test(test=_test,train=_train, Number_of_Background=Number_of_Background)
signal_train_sf, signal_test_sf = sf_signal_train_test(test=_test,train=_train, Number_of_Signal=Number_of_Signal)

#Run plotter:
DIR = config.get("model", "dir")

plotter = Plotter(DIR)
plotter.train_test_plot(_df_train, _df_test, bgd_train_sf, bgd_test_sf, signal_train_sf, signal_test_sf)
plotter.significance_plot(_df_test, bgd_test_sf,signal_test_sf)
plotter.roc_curve(_df_train, _df_test)