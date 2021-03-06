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

VARS = ["DiL", "Dr", "Dr2", "MT", "Mt2as","dEta", "dMETPhiL1", "dMETPhiL2", "Dzeta",
        "mcta",  "met_pt", "MTtot", "mu_pt",  "tau_pt", 'Mt2as']


gen_VARS = ["gen_DiL", "gen_Dr", "gen_Dr2", "gen_MT", "gen_Mt2as", "gen_dEta",
       "gen_dMETPhiL1", "gen_dMETPhiL2", "gen_Dzeta", "gen_mcta", "gen_met_pt",
       "gen_MTtot", "gen_muon_pt",  "gen_tau_pt", 'gen_Mt2as']
#gen_VARS = ["gen_met_pt", "Lept1Pt", "Lept2Pt", "EtaDil",
#            "MTtot","genDzeta","dR" ,"Minv", "genMT", "MCTb", "genMT2lester",
#            "gendMETPhiL1", "gendMETPhiL2"]

#VARS = ["met_pt", "Lept1Pt", "Lept2Pt", "EtaDil",
#        "MTtot","Dzeta","dR" ,"Minv", "MT", "MCTb",
#        "MT2lester", "dMETPhiL1", "dMETPhiL2"]

N_INPUTS = len(VARS)


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
train = label_correction(train, labels=[1,0], class_names=["signal","background"], col_names=["classID", "className"])
#train["classID"] = train["classID"].apply(lambda x: 1-np.int32(x>0.5))


###   Test train split  ###
from utils.preprocess import _train_test_split
_train, _test = _train_test_split(train, split=float(config.get("model","test_train_split" )))


###    Preprocess     ###
from utils import _overbalance as ovbal
_train = ovbal(_train)


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


X_train = _train[VARS]
gen_X_train = _train[gen_VARS]

Y_train = _train[LABELS]
W_train = _train[WEIGHT]

X_validation = _test[VARS]
gen_X_validation = _test[gen_VARS]
Y_validation = _test[LABELS]
W_validation = _test[WEIGHT]

X_test = _test[VARS]
gen_X_test = _test[gen_VARS]
Y_test = _test[LABELS]
W_test = _test[WEIGHT]

DIR = config.get("model", "dir")
from train import Training


###   Start training:   ####

gen_met_trainin = Training(config)
model = gen_met_trainin._model(input_dim = N_INPUTS)

from utils import Histories
histories = Histories()
histories.set_up_config(config)
histories.set_up_train_weight(weight=W_train)
histories.set_up_val_weight(weight=W_validation)


from keras.callbacks import LearningRateScheduler
from lr.schedule import *
lrate = LearningRateScheduler(step_decay)

#epochs = config.get("model", 'gen_lr')
#lr = config.get("model", 'gen_epoch')
#Gen pretrain
histories.set_mode(mode="pre_train")
gen_met_trainin.pre_train(gen_X_train, Y_train,  gen_X_validation, Y_validation, callback=[histories])
gen_met_trainin.pop_layers()
histories.set_mode(mode="train")
gen_met_trainin.train(X_train, Y_train,  X_validation, Y_validation, callback=[histories])

##Store gen_pretraininf results:

#Normal training
# gen_met_trainin.train(X_train, Y_train,  X_validation, Y_validation, callback=[histories])


gen_met_trainin.store_model()

#gen_met_trainin.epochs = 20
#gen_met_trainin.train(x_train, y_train, epochs=50)

#Get Result of training:
#from utils import get_results

_df_train, _df_test  = gen_met_trainin.get_results(X_train, Y_train,  X_test, Y_test, W_train, W_test)


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






