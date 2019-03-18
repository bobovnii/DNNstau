import ConfigParser
from loader.dataloader import DataLoader
import argparse
from keras.layers import  Dense
from keras.layers.core import Dropout, Activation
from keras.models import Sequential
import numpy as np
from keras.optimizers import Adam
import sherpa

#vu9B2SYu
#from logger import set_logger

parser = argparse.ArgumentParser()

parser.add_argument('--config', default='config.ini',
                        help="Configuration file")
args = parser.parse_args()
configuration_name = args.config


######   Parse config:    #####

config = ConfigParser.RawConfigParser()
config.read(configuration_name)
DIR = config.get("model", "dir")

### Set Logger ###:
#set_logger(file_name="{0}/hyper_optimisation.log".format(DIR), config=None)

####  Preprocess Data    #####
from utils import label_correction
from utils.preprocess import _train_test_split
from utils import _overbalance as ovbal

####   Get Data:   ####
from utils import Histories



def get_data():
    """
    Data providing function:

    This function is separated from create_model() so that hyperopt
    won't reload data for each evaluation run.
    """
    # Define Variables and Features:


    VARS = ["met_pt", "Lept1Pt", "Lept2Pt", "EtaDil",
            "MTtot", "Dzeta", "dR", "Minv", "MT", "MCTb",
            "MT2lester", "dMETPhiL1", "dMETPhiL2"]

    LABELS = ["classID"]

    config = ConfigParser.RawConfigParser()

    configuration_name = "/Users/dydukhle/PycharmProjects/stau_learning/config/config_100.ini"
    config.read(configuration_name)

    dataloader = DataLoader(config)

    # Load Data
    train = dataloader._get_train()
    train = label_correction(train)

    ###   Test train split  ###
    _train, _test = _train_test_split(train, split=float(config.get("model", "test_train_split")))

    ###    Preprocess     ###
    __train = ovbal(_train)
    X_train = __train[VARS]
    Y_train = __train[LABELS]

    X_validation = _test[VARS]
    Y_validation = _test[LABELS]

    return X_train, Y_train,  X_validation, Y_validation


### Create Model   ####
# Train
config = ConfigParser.RawConfigParser()
configuration_name = "/Users/dydukhle/PycharmProjects/stau_learning/config/config_100.ini"
config.read(configuration_name)


#Define parameters to search:

parameters = [sherpa.Discrete('num_units', [50, 200])]
alg = sherpa.algorithms.RandomSearch(max_num_trials=50)


model = Sequential()
model.add(Dense( 256, input_shape=(13,)))
model.add(Activation('sigmoid'))
model.add(Dropout(0.288662535902546))
model.add(Dense({{choice([256, 512, 1024])}}))
model.add(Activation({{choice(['relu', 'sigmoid'])}}))
model.add(Dropout(0.4844455237320119))

# If we choose 'four', add an additional fourth layer
if {{choice(['three', 'four'])}} == 'four':
    model.add(Dense(100))
    # We can also choose between complete sets of layers
    model.add(Activation('relu'))

model.add(Dense(1))
model.add(Activation('sigmoid'))

adam = Adam(lr={{choice([0.01, 0.001, 0.0001, 0.00001])}})

model.compile(loss='binary_crossentropy', metrics=['accuracy'],
              optimizer=adam)

histories = Histories()
histories.set_up_config(config)
histories.set_mode("fit")

result = model.fit(X_train, Y_train,
                          batch_size= 4000,
                          epochs=2,
                          verbose=2,
                          validation_data=(X_validation, Y_validation),
                          callbacks=[histories]
                          )

validation_acc = np.amax(result.history['val_acc'])
print(model.summary())
print('Best validation acc of epoch:', validation_acc)


###    Store Config file   ###

#with open("{0}/config.ini".format(DIR+MODEL_NAME), "w") as f:
#    config.write(f)






