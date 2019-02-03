import pandas as pd
from sklearn.utils import shuffle
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



#Parse config:
config = ConfigParser.RawConfigParser()
config.read("config.ini")
dataloader = DataLoader(config)

#Load Data
train =dataloader._get_train()
from utils import label_correction
train = label_correction(train, labels=[1,0], class_names=["signal","background"], col_names=["classID", "className"])

#Test train split
from preprocess import test_train_split
_train, _test = test_train_split(train, split=float(config.get("model","test_train_split" )))

#Extract the SF for signal and background:
from sf import *
Number_of_Background = float(config.get("physics", "Number_of_Background"))
Number_of_Signal = float(config.get("physics","Number_of_Signal"))
bgd_train_sf, bgd_test_sf = sf_bgd_train_test(test=_test,train=_train, Number_of_Background=Number_of_Background)
signal_train_sf, signal_test_sf = sf_signal_train_test(test=_test,train=_train, Number_of_Signal=Number_of_Signal)


#Preprocess
from utils import _overbalance as ovbal
_train = ovbal(_train)

X_train = _train[VARS]
Y_train = _train[LABELS]
W_train = _train[WEIGHT]

gen_X_train = _train[VARS]
gen_Y_train = _train[LABELS]
gen_W_train = _train[WEIGHT]


X_test = _test[VARS]
Y_test = _test[LABELS]
W_test = _test[WEIGHT]

DIR = config.get("model", "dir")
from train import Training

#Start training:

gen_met_trainin = Training(config)
model = gen_met_trainin._model()

gen_met_trainin.train(gen_X_train, gen_Y_train)

gen_met_trainin.train(X_train, Y_train)
#gen_met_trainin.store_model()

#Get Result of training:
from utils import get_results

x_train = _train[VARS]
x_gen_train = _train[gen_VARS]
y_train = _train[["classID"]]
w_train = _train["weight"]
x_test = _test[VARS]
x_gen_test = _test[gen_VARS]
y_test = _test[["classID"]]
w_test = _test["weight"]

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






