from plotter.Plotter import Plotter
import ConfigParser
from loader.dataloader import DataLoader
import argparse
from sklearn.preprocessing import MinMaxScaler

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


#####   Run plotter:     ######

def eval_model(DIR):
    plotter = Plotter(DIR)
    plotter.train_test_plot(_df_train, _df_test, bgd_train_sf, bgd_test_sf, signal_train_sf, signal_test_sf)
    plotter.significance_plot(_df_test, bgd_test_sf,signal_test_sf)
    plotter.roc_curve(_df_train, _df_test)

######   Parse config:    #####

config = ConfigParser.RawConfigParser()
config.read(configuration_name)
dataloader = DataLoader(config)

#Load Data
train =dataloader._get_train()
from utils import label_correction
train = label_correction(train)
#train["classID"] = train["classID"].apply(lambda x: 1-np.int32(x>0.5))


###   Test train split  ###
from utils.preprocess import _train_test_split
_train, _test = _train_test_split(train, split=float(config.get("model","test_train_split" )))


###    Preprocess     ###
from utils import _overbalance as ovbal


###   Extract the SF for signal and background:  ###
from utils.sf import *
Number_of_Background = float(config.get("physics", "Number_of_Background"))
Number_of_Signal = float(config.get("physics","Number_of_Signal"))

Number_of_Background = train[(train.classID==0)].weight.sum()# float(config.get("physics", "Number_of_Background"))
Number_of_Signal = train[(train.classID==1)].weight.sum()
print("Number_of_Background", Number_of_Background)
print("Number_of_Signal", Number_of_Signal)
bgd_train_sf, bgd_test_sf = sf_bgd_train_test(test=_test,train=_train, Number_of_Background=Number_of_Background)
signal_train_sf, signal_test_sf = sf_signal_train_test(test=_test,train=_train, Number_of_Signal=Number_of_Signal)

scaler = MinMaxScaler()

ubalanced = _train
ubalanced_X_train = scaler.fit_transform(ubalanced[VARS])
ubalanced_Y_train = ubalanced[LABELS]
ubalanced_W_train = ubalanced[WEIGHT]
del ubalanced
_train = ovbal(_train)

X_train = scaler.fit_transform(_train[VARS])
gen_X_train = scaler.fit_transform(_train[gen_VARS])

Y_train = _train[LABELS]
W_train = _train[WEIGHT]

X_validation = scaler.fit_transform(_test[VARS])
gen_X_validation = scaler.fit_transform(_test[gen_VARS])
Y_validation = _test[LABELS]
W_validation = _test[WEIGHT]


X_test = scaler.fit_transform(_test[VARS])
gen_X_test = scaler.fit_transform(_test[gen_VARS])
Y_test = _test[LABELS]
W_test = _test[WEIGHT]

DIR = config.get("model", "dir")
from train import Training


###   Start training:   ####

gen_met_trainin = Training(config)
model = gen_met_trainin._model()

from utils import Histories
histories = Histories()
histories.set_up_config(config)
histories.set_up_train_weight(weight=W_train)
histories.set_up_val_weight(weight=W_validation)

#lr = SGDRScheduler(min_lr=0.0001, max_lr=0.01, steps_per_epoch=np.ceil(X_train.shape[0]/4000),lr_decay=0.7, cycle_length=20, mult_factor=2)


#epochs = config.get("model", 'gen_lr')
#lr = config.get("model", 'gen_epoch')
#Gen pretrain
histories.set_mode(mode="pre_train")
gen_met_trainin.pre_train(gen_X_train, Y_train,  gen_X_validation, Y_validation, callback=[histories])
gen_met_trainin.store_model()

histories.set_mode(mode="train")
loss = "binary_crossentropy"

expected_signal = train[(train.classID==1)].weight.sum()
expecred_background = train[(train.classID==0)].weight.sum()
gen_met_trainin.train(X_train, Y_train,  X_validation, Y_validation, callback=[histories],
                      loss=loss)

##Store gen_pretraininf results:

#Normal training
# gen_met_trainin.train(X_train, Y_train,  X_validation, Y_validation, callback=[histories])



#gen_met_trainin.epochs = 20
#gen_met_trainin.train(x_train, y_train, epochs=50)

#Get Result of training:
#from utils import get_results

_df_train, _df_test  = gen_met_trainin.get_results(ubalanced_X_train, ubalanced_Y_train,  X_test,
                                                   Y_test, ubalanced_W_train, W_test)


###    Run plotter:    ###
DIR = config.get("model", "dir")
MODEL_NAME = config.get("model", "model_name")

_df_train.to_csv(DIR+MODEL_NAME+"/train_results.csv")
_df_test.to_csv(DIR+MODEL_NAME+"/test_results.csv")


plotter = Plotter(DIR+MODEL_NAME)
plotter.train_test_plot(_df_train, _df_test, bgd_train_sf, bgd_test_sf, signal_train_sf, signal_test_sf)
plotter.significance_plot(_df_test, bgd_test_sf,signal_test_sf)
plotter.roc_curve(_df_train, _df_test)

###    Store Config file   ###

with open("{0}/config.ini".format(DIR+MODEL_NAME), "w") as f:
    config.write(f)






