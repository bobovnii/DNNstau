import ConfigParser
from dataloader import DataLoader
import argparse
from keras.layers import  Dense
from keras.layers.core import Dropout, Activation
from hyperopt import Trials, STATUS_OK, tpe
from hyperas import optim
from hyperas.distributions import choice, uniform
from keras.models import Sequential
from hyperopt import Trials, tpe
from hyperas import optim
import  numpy as np



parser = argparse.ArgumentParser()

parser.add_argument('--config', default='config.ini',
                        help="Configuration file")
args = parser.parse_args()
configuration_name = args.config


######   Parse config:    #####

config = ConfigParser.RawConfigParser()
config.read(configuration_name)
DIR = config.get("model", "dir")

####  Preprocess Data    #####
from utils import label_correction
from preprocess import _train_test_split
from utils import _overbalance as ovbal

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
    train = label_correction(train, labels=[1, 0], class_names=["signal", "background"],
                             col_names=["classID", "className"])

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
def create_model(X_train, Y_train,  X_validation, Y_validation):
    """
    Model providing function:

    Create Keras model with double curly brackets dropped-in as needed.
    Return value has to be a valid python dictionary with two customary keys:
        - loss: Specify a numeric evaluation metric to be minimized
        - status: Just use STATUS_OK and see hyperopt documentation if not feasible
    The last one is optional, though recommended, namely:
        - model: specify the model just created so that we can later use it again.
    """
    # Train
    config = ConfigParser.RawConfigParser()
    configuration_name = "/Users/dydukhle/PycharmProjects/stau_learning/config/config_100.ini"
    config.read(configuration_name)


    model = Sequential()
    model.add(Dense(256, input_shape=(13,)))
    model.add(Activation({{choice(['relu', 'sigmoid'])}}))
    model.add(Dropout({{uniform(0, 1)}}))
    model.add(Dense({{choice([256, 512, 1024])}}))
    model.add(Activation({{choice(['relu', 'sigmoid'])}}))
    model.add(Dropout({{uniform(0, 1)}}))

    # If we choose 'four', add an additional fourth layer
    if {{choice(['three', 'four'])}} == 'four':
        model.add(Dense(100))
        # We can also choose between complete sets of layers
        model.add({{choice([Dropout(0.5), Activation({{choice(['relu', 'sigmoid'])}})])}})
        model.add(Activation('relu'))

    model.add(Dense(1))
    model.add(Activation('sigmoid'))

    model.compile(loss='binary_crossentropy', metrics=['accuracy'],
                  optimizer={{choice(['rmsprop', 'adam', 'sgd'])}})

    result = model.fit(X_train, Y_train,
                              batch_size={{choice([64, 128, 300, 500, 1000, 2000, 4000])}},
                              epochs={{choice([10, 20, 30, 50, 1000])}},
                              verbose=2,
                              validation_data=(X_validation, Y_validation),
                              )

    validation_acc = np.amax(result.history['val_acc'])
    print('Best validation acc of epoch:', validation_acc)
    return {'loss': validation_acc, 'status': STATUS_OK, 'model': model}


    return result

###   Start training:   ####
best_run, best_model = optim.minimize(model=create_model,
                                      data=get_data,
                                      algo=tpe.suggest,
                                      max_evals=5,
                                      trials=Trials())
### Evaluate best model   ###
X_train, Y_train, X_test, Y_test = get_data()
print("Evalutation of best performing model:")
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

#_df_train, _df_test  = trainin.get_results(ubalanced_X_train, ubalanced_Y_train,  X_test, Y_test, ubalanced_W_train, W_test)


###    Run plotter:    ###
#DIR = config.get("model", "dir")
#MODEL_NAME = config.get("model", "model_name")

#_df_train.to_csv(DIR+MODEL_NAME+"/train_results.csv")
#_df_test.to_csv(DIR+MODEL_NAME+"/test_results.csv")


#plotter = Plotter(DIR+MODEL_NAME)
#plotter.train_test_plot(_df_train, _df_test, bgd_train_sf, bgd_test_sf, signal_train_sf, signal_test_sf)
#plotter.significance_plot(_df_test, bgd_test_sf,signal_test_sf)
#plotter.roc_curve(_df_train, _df_test)

###    Store Config file   ###

#with open("{0}/config.ini".format(DIR+MODEL_NAME), "w") as f:
#    config.write(f)






