import sherpa
client = sherpa.Client()
trial = client.get_trial()

import os
from loader.dataloader import DataLoader
import configparser



_configparser = configparser.RawConfigParser()
config = _configparser.read(PATH)
_dataloader = DataLoader(config=config)

import keras.backend as K



def create_model():
    """

    :return:
    """
    return

def load_model():
    """

    :return:
    """
    return




if trial.parameters['load_from'] == '':
    model = create_model(trial.parameters)

else:
    model = load_model(os.path.join('./output', trial.parameters['load_from'] + '.h5'))
    K.set_value(model.optimizer.lr, trial.parameters['lr'])
    K.set_value(model.optimizer.momentum, trial.parameters['momentum'])