from xgboost import XGBClassifier
from plotter.Plotter import Plotter

import ConfigParser
from loader.dataloader import DataLoader
import argparse

parser = argparse.ArgumentParser()
parser.add_argument('--config', default='config.ini',
                        help="Configuration file")

args = parser.parse_args()
configuration_name = args.config


#Define Variables and Features:

gen_VARS = ["gen_met_pt", "Lept1Pt", "Lept2Pt", "EtaDil",
            "MTtot","genDzeta","dR" ,"Minv", "genMT", "MCTb", "genMT2lester",
            "gendMETPhiL1", "gendMETPhiL2"]

VARS = ["met_pt", "Lept1Pt", "Lept2Pt", "EtaDil",
        "MTtot","Dzeta","dR" ,"Minv", "MT", "MCTb",
        "MT2lester", "dMETPhiL1", "dMETPhiL2"]


LABELS = ["classID"]
WEIGHT = "weight"


######   Parse config:    #####

config = ConfigParser.RawConfigParser()
config.read(configuration_name)
dataloader = DataLoader(config)

#Load Data
train =dataloader._get_train()
from utils import label_correction
train = label_correction(train)


###   Test train split  ###
from utils.preprocess import _train_test_split
_train, _test = _train_test_split(train, split=float(config.get("model","test_train_split" )))

params = {"learning_rate"    : [0.05, 0.10, 0.15, 0.20, 0.25, 0.30 ] ,
 "max_depth"        : [ 3, 4, 5, 6, 8, 10, 12, 15],
 "min_child_weight" : [ 1, 3, 5, 7 ],
 "gamma"            : [ 0.0, 0.1, 0.2 , 0.3, 0.4 ],
 "colsample_bytree" : [ 0.3, 0.4, 0.5 , 0.7 ] }


###   Extract the SF for signal and background:  ###
from utils.sf import *
Number_of_Background = train[(train.classID==0)].weight.sum()# float(config.get("physics", "Number_of_Background"))
Number_of_Signal = train[(train.classID==1)].weight.sum()#float(config.get("physics","Number_of_Signal"))
bgd_train_sf, bgd_test_sf = sf_bgd_train_test(test=_test,train=_train, Number_of_Background=Number_of_Background)
signal_train_sf, signal_test_sf = sf_signal_train_test(test=_test,train=_train, Number_of_Signal=Number_of_Signal)

X_train = _train[VARS]
Y_train = _train[LABELS]
W_train = _train[WEIGHT]

X_test = _test[VARS]
gen_X_test = _test[gen_VARS]
Y_test = _test[LABELS]
W_test = _test[WEIGHT]

DIR = config.get("model", "dir")

###   Start training:   ####

def model():
    ##Start training
    clf = XGBClassifier(objective='binary:logitraw', booster="gbtree",

                        eval_metric="auc",

                        nthread=4,

                        eta=0.01,

                        gamma=0,

                        max_depth=5,

                        subsample=0.6,

                        colsample_bytree=0.8,

                        colsample_bylevel=0.675,

                        min_child_weight=22,

                        alpha=0,

                        random_state=42,

                        nrounds=2000,

                        n_estimators=800

                        )
    return clf

clf = model()
clf.fit(X_train, Y_train, eval_set= [(X_train, Y_train), (X_test, Y_test)], verbose=10, early_stopping_rounds=200)

#Get Result of training:
from utils import get_results
_df_train, _df_test  = get_results(clf, X_train, Y_train,  X_test, Y_test, W_train, W_test)


###    Run plotter:    ###
DIR = config.get("model", "dir")
MODEL_NAME = config.get("model", "model_name")
#os.mkdir(DIR+MODEL_NAME)
_df_train.to_csv(DIR+MODEL_NAME+"/train_results.csv")
_df_test.to_csv(DIR+MODEL_NAME+"/test_results.csv")


plotter = Plotter(DIR+MODEL_NAME)
plotter.train_test_plot(_df_train, _df_test, bgd_train_sf, bgd_test_sf, signal_train_sf, signal_test_sf)
plotter.significance_plot(_df_test, bgd_test_sf,signal_test_sf)
plotter.roc_curve(_df_train, _df_test)

###    Store Config file   ###

with open("{0}/config.ini".format(DIR+MODEL_NAME), "w") as f:
    config.write(f)









