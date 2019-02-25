import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import uproot
import os
from xgboost import XGBClassifier


DATA = "../DataSets/"
train = pd.read_hdf(DATA+"Train_overbalance_table.hdf")
test = pd.read_hdf(DATA+"Test_table.hdf")

param = list(train.keys())
VARS = ['Dzeta','EtaDil','Lept1Pt','Lept2Pt','MCTb','MT','MT2lester','MTtot','Minv','dMETPhiL1','dMETPhiL2','dR','met_pt']

from sklearn.model_selection import train_test_split
X_train, X_validation,y_train, y_validation = train_test_split(train[param], train[Target], train_size=0.9, random_state=42)


x_train = X_train[VARS]
y_train = y_train[Target]

x_val = X_validation[VARS]
y_val = y_validation[Target]

X_test = test[VARS]
Y_test = test[Target]


##Start training
clf = XGBClassifier(objective='binary:logitraw', booster="gbtree",

                    eval_metric="auc",

                    nthread=4,

                    eta=0.01,

                    gamma=0,

                    max_depth=2,

                    subsample=0.6,

                    colsample_bytree=0.8,

                    colsample_bylevel=0.675,

                    min_child_weight=22,

                    alpha=0,

                    random_state=42,

                    nrounds=2000,

                    n_estimators=3000

                    )
