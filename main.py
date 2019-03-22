from plotter.Plotter import Plotter
import ConfigParser
from sklearn.preprocessing import MinMaxScaler
from loader.dataloader import DataLoader
import argparse
import pandas as pd
from utils.utils import Histories
from lr.schedule import LossLearningRateScheduler
from metric.loss import significanceLoss

parser = argparse.ArgumentParser()
parser.add_argument('--config', default='config.ini',
                        help="Configuration file")
args = parser.parse_args()
configuration_name = args.config


###   Define Variables and Features:
gen_VARS = ["gen_met_pt", "Lept1Pt", "Lept2Pt", "EtaDil", "MTtot","genDzeta","dR",
            "Minv", "genMT", "MCTb", "genMT2lester","gendMETPhiL1", "gendMETPhiL2"]

VARS = ["met_pt", "Lept1Pt", "Lept2Pt", "EtaDil","MTtot","Dzeta","dR",
        "Minv", "MT", "MCTb", "MT2lester", "dMETPhiL1", "dMETPhiL2"]

LABELS = ["classID"]
WEIGHT = "weight"


######   Parse config:    #####

config = ConfigParser.RawConfigParser()
config.read(configuration_name)
dataloader = DataLoader(config)

#Load Data
from utils.utils import label_correction
train = dataloader._get_train()
train = label_correction(train)


###   Test train split  ###
from utils.preprocess import _train_test_split
_train, _test = _train_test_split(train, split=float(config.get("model","test_train_split" )))





###   Extract the SF for signal and background:  ###
from utils.sf import *
Number_of_Background = train[(train.classID==0)].weight.sum()# float(config.get("physics", "Number_of_Background"))
Number_of_Signal = train[(train.classID==1)].weight.sum()#float(config.get("physics","Number_of_Signal"))
bgd_train_sf, bgd_test_sf = sf_bgd_train_test(test=_test,train=_train, Number_of_Background=Number_of_Background)
signal_train_sf, signal_test_sf = sf_signal_train_test(test=_test,train=_train, Number_of_Signal=Number_of_Signal)

###    Preprocess     ###
from utils.utils import _overbalance as ovbal
scaler = MinMaxScaler()
ubalanced = _train
ubalanced_X_train = ubalanced[VARS]
ubalanced_Y_train = ubalanced[LABELS]
ubalanced_W_train = ubalanced[WEIGHT]

del ubalanced

__train = ovbal(_train)

X_train = __train[VARS]
gen_X_train = __train[gen_VARS]
X_train = scaler.fit_transform(__train[VARS])
gen_X_train = scaler.fit_transform(__train[gen_VARS])

Y_train = __train[LABELS]
W_train = __train[WEIGHT]

X_validation = _test[VARS]
gen_X_validation = _test[gen_VARS]
Y_validation = _test[LABELS]
W_validation = _test[WEIGHT]

X_test = _test[VARS]
gen_X_test = _test[gen_VARS]
Y_test = _test[LABELS]
W_test = _test[WEIGHT]

X_validation = scaler.fit_transform(_test[VARS])
gen_X_validation = scaler.fit_transform(_test[gen_VARS])


X_test = scaler.fit_transform(_test[VARS])
gen_X_test = scaler.fit_transform(_test[gen_VARS])


DIR = config.get("model", "dir")
from train import Training


###   Start training:   ####
training = Training(config)
expected_signal = _train[(_train.classID==1)].weight.sum()
expecred_background = _train[(_train.classID==0)].weight.sum()
print("Expected Signal: ", _train[(_train.classID==1)].weight.sum())
print("Expected Background: ", expecred_background)

loss =significanceLoss(expected_signal,expecred_background )
loss = "binary_crossentropy"
model = training._model(loss=loss)
histories = Histories()
histories.set_up_config(config)
histories.set_up_train_weight(weight=W_train)
histories.set_up_val_weight(weight=W_validation)

lr = LossLearningRateScheduler(base_lr=0.01, lookback_epochs=10, decay_multiple=0.9)


histories.set_mode(mode="pre_train")
training.pre_train(gen_X_train, Y_train,  gen_X_validation, Y_validation, callback=[histories],
              loss = loss)
histories.set_mode(mode="train")
training.train(X_train, Y_train,  X_validation, Y_validation, callback=[histories],
              loss = loss)

training.store_model()


###  Get Result of training:
_df_train, _df_test  = training.get_results(ubalanced_X_train, ubalanced_Y_train,
                                            X_test, Y_test, ubalanced_W_train, W_test,
                                           ubalanced_X_train.index,   _test.index
                                           )


###    Run plotter:    ###
DIR = config.get("model", "dir")
MODEL_NAME = config.get("model", "model_name")

## Save preliminary results:
_df_train.to_csv(DIR+MODEL_NAME+"/train_results.csv")
_df_test.to_csv(DIR+MODEL_NAME+"/test_results.csv")

plotter = Plotter(DIR+MODEL_NAME)

plotter.train_test_plot(pd.DataFrame(_df_train), pd.DataFrame(_df_test),
                        bgd_train_sf, bgd_test_sf,
                        signal_train_sf, signal_test_sf)

plotter.significance_plot(_df_test, bgd_test_sf, signal_test_sf)
plotter.roc_curve(_df_train, _df_test)

###    Store Config file   ###
with open("{0}/config.ini".format(DIR+MODEL_NAME), "w") as f:
    config.write(f)






