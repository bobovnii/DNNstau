import ConfigParser
from loader.dataloader import DataLoader
import argparse
from keras.layers import  Dense
from keras.layers.core import Dropout, Activation
from hyperopt import STATUS_OK
from hyperas.distributions import choice, uniform
from keras.models import Sequential
from hyperopt import Trials, tpe
from hyperas import optim
import  numpy as np
from keras.optimizers import Adam
from utils.utils import label_correction
from utils.preprocess import _train_test_split
from utils.utils import _overbalance as ovbal



#from logger import set_logger


######   Parse config:    #####
parser = argparse.ArgumentParser()
parser.add_argument('--config', default='config.ini',
                        help="Configuration file")
args = parser.parse_args()
configuration_name = args.config


config = ConfigParser.RawConfigParser()
config.read(configuration_name)
DIR = config.get("model", "dir")

### Set Logger ###:
#set_logger(file_name="{0}/hyper_optimisation.log".format(DIR), config=None)

####  Preprocess Data    #####

####   Get Data:   ####


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
    _train, _test = _train_test_split(train, split=0.3)

    _train, _test = _train_test_split(_train, split=0.2)

    ###    Preprocess     ###
    __train = ovbal(_train)
    X_train = __train[VARS]
    Y_train = __train[LABELS]

    X_validation = _test[VARS]
    Y_validation = _test[LABELS]

    return X_train, Y_train,  X_validation, Y_validation


### Create Model   ####
def create_model(X_train, Y_train,  X_validation, Y_validation):
    import datetime
    """
    Model providing function:

    Create Keras model with double curly brackets dropped-in as needed.
    Return value has to be a valid python dictionary with two customary keys:
        - loss: Specify a numeric evaluation metric to be minimized
        - status: Just use STATUS_OK and see hyperopt documentation if not feasible
    The last one is optional, though recommended, namely:
        - model: specify the model just created so that we can later use it again.
    """
    now = datetime.datetime.now()

    time_stamp = now.strftime("%s")
    # Train
    config = ConfigParser.RawConfigParser()
    configuration_name = "/Users/dydukhle/PycharmProjects/stau_learning/config/config_100.ini"
    config.read(configuration_name)
    {'Activation_1': 0, 'Dense_2': 3, 'Dense_1': 2, 'Activation_2': 1, 'Dense': 1, 'Activation': 1,
     'Dropout_1': 0.8366666847115819, 'Dropout_2': 0.9128294469805703, 'lr': 0, 'epochs': 2,
     'Dropout': 0.9770005173795487}


    model = Sequential()
    model.add(Dense( {{choice([32, 64, 128, 256])}}, input_shape=(13,)))
    model.add(Activation({{choice(['relu', 'sigmoid'])}}))
    model.add(Dropout({{uniform(0, 1)}}))

    model.add(Dense({{choice([32, 64, 128, 256])}}))
    model.add(Activation({{choice(['relu', 'sigmoid'])}}))
    model.add(Dropout({{uniform(0, 1)}}))

    model.add(Dense({{choice([32, 64, 128, 256])}}))
    model.add(Activation({{choice(['relu', 'sigmoid'])}}))
    model.add(Dropout({{uniform(0, 1)}}))

    model.add(Dense(1))
    model.add(Activation('sigmoid'))

    adam = Adam(lr={{choice([0.01, 0.001, 0.0001, 0.00001])}})

    model.compile(loss='binary_crossentropy', metrics=['accuracy'],
                  optimizer=adam)


    result = model.fit(X_train, Y_train,
                              batch_size= 4000,
                              epochs={{choice([20, 30, 50])}},
                              verbose=2,
                              validation_data=(X_validation, Y_validation),
                              )
    validation_acc = np.amax(result.history['val_acc'])
    print(model.summary())
    print('Best validation acc of epoch:', validation_acc)
    return {'loss': -validation_acc, 'status': STATUS_OK, 'model': model}


###   Start training:   ####
best_run, best_model = optim.minimize(model=create_model,
                                      data=get_data,
                                      algo=tpe.suggest,
                                      max_evals=10,
                                      trials=Trials())
### Evaluate best model   ###
print(best_run)
X_train, Y_train, X_test, Y_test = get_data()
print("Evalutation of best performing model:")
print(best_model.summary())
print(best_model.evaluate(X_test, Y_test))
print("Best performing model chosen hyper-parameters:")
print(best_run)



#from utils import LearningRateScheduler
#ls #lrate = LearningRateScheduler(step_decay)
#Prepare X and Y

#gen_met_trainin.epochs = 20
#gen_met_trainin.train(x_train, y_train, epochs=50)

#Get Result of training:
#from utils import get_results

#_df_train, _df_test  = get_results(ubalanced_X_train, ubalanced_Y_train,  X_test, Y_test, ubalanced_W_train, W_test)


###    Run plotter:    ###
#DIR = config.get("model", "dir")
#MODEL_NAME = config.ge#t("model", "model_name")

#_df_train.to_csv(DIR+MODEL_NAME+"/train_results.csv")
#_df_test.to_csv(DIR+MODEL_NAME+"/test_results.csv")


#plotter = Plotter(DIR+MODEL_NAME)
#plotter.train_test_plot(_df_train, _df_test, bgd_train_sf, bgd_test_sf, signal_train_sf, signal_test_sf)
#plotter.significance_plot(_df_test, bgd_test_sf,signal_test_sf)
#plotter.roc_curve(_df_train, _df_test)

###    Store Config file   ###

#with open("{0}/config.ini".format(DIR+MODEL_NAME), "w") as f:
#    config.write(f)






