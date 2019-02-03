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


#np.random.seed(7)


#Define Variables and Features:

gen_VARS = ["gen_met_pt", "Lept1Pt", "Lept2Pt", "EtaDil",
            "MTtot","genDzeta","dR" ,"Minv", "genMT", "MCTb", "genMT2lester",
            "gendMETPhiL1", "gendMETPhiL2"]

VARS = ["met_pt", "Lept1Pt", "Lept2Pt", "EtaDil",
        "MTtot","Dzeta","dR" ,"Minv", "MT", "MCTb",
        "MT2lester", "dMETPhiL1", "dMETPhiL2"]


LABELS = ["classID"]
WEIGHT = "weight"


#Run plotter:

def eval_model(DIR):
    plotter = Plotter(DIR)
    plotter.train_test_plot(_df_train, _df_test, bgd_train_sf, bgd_test_sf, signal_train_sf, signal_test_sf)
    plotter.significance_plot(_df_test, bgd_test_sf,signal_test_sf)
    plotter.roc_curve(_df_train, _df_test)

#Parse config:

config = ConfigParser.RawConfigParser()
config.read("config_150.ini")
dataloader = DataLoader(config)

#Load Data
train =dataloader._get_train()
from utils import label_correction
train = label_correction(train, labels=[1,0], class_names=["signal","background"], col_names=["classID", "className"])
#train["classID"] = train["classID"].apply(lambda x: 1-np.int32(x>0.5))
#Test train split
from preprocess import test_train_split
_train, _test = test_train_split(train, split=float(config.get("model","test_train_split" )))

#Extract the SF for signal and background:
from sf import *
Number_of_Background = float(config.get("physics", "Number_of_Background"))
Number_of_Signal = float(config.get("physics","Number_of_Signal"))
bgd_train_sf, bgd_test_sf = sf_bgd_train_test(test=_test,train=_train, Number_of_Background=Number_of_Background)
signal_train_sf, signal_test_sf = sf_signal_train_test(test=_test,train=_train, Number_of_Signal=Number_of_Signal)

def _overbalance(train):
    """
    Return Oversampled dataset
    """
    count_class_0, count_class_1 = train.classID.value_counts()
    # Divide by class
    df_class_0 = train[train["classID"] == 0]
    df_class_1 = train[train["classID"] == 1]
    df_class_1_over = df_class_1.sample(count_class_0, replace=True)
    df_over = pd.concat([df_class_0, df_class_1_over], axis=0)
    df_over = shuffle(df_over)
    return df_over

#Preprocess
_train, _validation = test_train_split(_train, split=0.1)
X_validation = _validation[VARS]
Y_validation = _validation[LABELS]
W_validation = _validation[WEIGHT]

from utils import _overbalance as ovbal
_train = ovbal(_train)

X_train = _train[VARS]
Y_train = _train[LABELS]
W_train = _train[WEIGHT]


X_test = _test[VARS]
Y_test = _test[LABELS]
W_test = _test[WEIGHT]

DIR = config.get("model", "dir")
from train import Training

x_train = _train[VARS]
x_gen_train = _train[gen_VARS]
y_train = _train[["classID"]]
w_train = _train["weight"]
x_test = _test[VARS]
x_gen_test = _test[gen_VARS]
y_test = _test[["classID"]]
w_test = _test["weight"]

"""GEN Pretraining"""
#Start training:

gen_met_trainin = Training(config)
model = gen_met_trainin._model()

gen_met_trainin.train(x_gen_train, y_train)
gen_met_trainin.epochs = 20

gen_met_trainin.train(x_train, y_train)
#gen_met_trainin.store_model()

#Get Result of training:
#from utils import get_results
#_df_train, _df_test = gen_met_trainin.get_results(model, X_train, Y_train, X_test, Y_test, W_train, W_test)
def get_results(model, _x_train, _y_train, x_test, y_test, _w_train, w_test):
        
    _df_train = pd.DataFrame()
    _df_test = pd.DataFrame()

    _df_train["train_labels"] =  [i[0] for i in _y_train.as_matrix()]
    _df_test["test_labels"] = [i[0] for i in y_test.as_matrix()]
    _df_train["index"] = _x_train.index
        
    _df_train["train_output"] = model.predict(_x_train, verbose=0)
    _df_test["test_output"] = model.predict(x_test, verbose=0)
        
    _df_train["train_weights"] = _w_train.as_matrix()
    _df_test["test_weights"] = w_test.as_matrix()
    
    _df_train["train_pred"] = _df_train["train_output"].apply(lambda x: 1 if x>0.5 else 0)
    _df_test["test_pred"] = _df_test["test_output"].apply(lambda x: 1 if x>0.5 else 0)
    _df_test["index"] = x_test.index
    return _df_train, _df_test



_df_train, _df_test  = get_results(model, x_train, y_train, x_test, y_test, w_train, w_test)


#Run plotter:
DIR = config.get("model", "dir")
MODEL_NAME = config.get("model", "model_name")

print(_df_train.train_pred.value_counts())
_df_train.to_csv(DIR+MODEL_NAME+"/train_results.csv")
_df_test.to_csv(DIR+MODEL_NAME+"/test_results.csv")




plotter = Plotter(DIR+MODEL_NAME)
plotter.train_test_plot(_df_train, _df_test, bgd_train_sf, bgd_test_sf, signal_train_sf, signal_test_sf)
plotter.significance_plot(_df_test, bgd_test_sf,signal_test_sf)
plotter.roc_curve(_df_train, _df_test)

#Store Config file

with open("{0}/config.ini".format(DIR+MODEL_NAME), "w") as f:
    config.write(f)

#plotter = Plotter(DIR)
#plotter.train_test_plot(_df_train, _df_test, bgd_train_sf, bgd_test_sf, signal_train_sf, #signal_test_sf)
#plotter.significance_plot(_df_test, bgd_test_sf,signal_test_sf)
#plotter.roc_curve(_df_train, _df_test)
    
#eval_model(DIR=DIR+"GenMETDNN")

"""Normal Training"""
#Training:
#gen_met_trainin.train(_X_train_ov, _Y_train_ov, gen_met_trainin.get_model())
#gen_met_trainin.store_model()

#Get Result of training:
#from utils import get_results
#_df_train, _df_test = get_results(gen_met_trainin.get_model(), _Y_train_ov, Y_train, X_test, Y_test, _W_train_ov, W_test)

#eval_model(DIR=DIR+"PretrainedGenMET")



#Test Evaluate




