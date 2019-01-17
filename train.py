"""
Script for the pretraining:
"""
# MLP for Pima Indians Dataset Serialize to JSON and HDF5
from keras.models import Sequential
from keras.layers import Dense, Dropout
from keras.models import model_from_json
import numpy as np
import os
import math
import uproot
import pandas as pd
import logging



np.random.seed(7)



class Training():
    """

    """

    def __init__(self, config):
        """

        :return:
        """
        self.config = config
        self.batch_size = 4000
        self.epoch = 10

        return

    def model(self):
        """
           :return:
        """

        model = Sequential()
        model.add(Dense(256, input_dim=13, kernel_initializer='uniform', activation='relu'))
        model.add(Dropout(0.4))
        model.add(Dense(256, kernel_initializer='uniform', activation='relu'))
        model.add(Dropout(0.4))
        model.add(Dense(256, kernel_initializer='uniform', activation='relu'))
        model.add(Dropout(0.4))
        model.add(Dense(1, kernel_initializer='uniform', activation='sigmoid'))
        # Compile model
        model.compile(loss='binary_crossentropy', metrics=['accuracy'], optimizer=Adam(lr=0.001))

        print(model.summary())
        self._model = model
        return model


    def train(self, X, Y , model=None):
        """


        :return:
        """
        if model == None:
            model = self.model()

        history = model.fit(X, Y, epochs=10, batch_size=4000)  # , verbose=0)
        if self.plot_history:
            """
            Plot the history
            """

        if self.store_history:
            """
            Store history of training
            """
        return



    def store_model(self, model, model_name):
        """

        :return:
        """
        if model is None:
            model = self._model

        model_json = model.to_json()
        with open("{0}.json".format(model_name), "w") as json_file:
            json_file.write(model_json)
        # serialize weights to HDF5
        model.save_weights(model_name)
        print("Model is saved on disk")
        return


    def load_model(self):
        """

        :return:
        """
        return

    def model_diagnostic(self):
        """

        :return:
        """
        #TODO
        return

    def evaluate_model(self, X, Y, model=None):
        """

        :param X:
        :param Y:
        :param model:
        :return:
        """
        if model in None:
            model = self._model

        # evaluate the model
        scores = model.evaluate(X, Y, verbose=0)
        print("Accuraccy %s: %.2f%%" % (model.metrics_names[1], scores[1] * 100))



    ## Extract parameter of mode:
    def extract_weights(self, model=None):
        """

        :param model:
        :return:
        """
        if model is None:
            model = self._model
        W = [layer.get_weights()[0] for layer in model.layers]
        B = [layer.get_weights()[1] for layer in model.layers]

        return {"W": W, "B": B}

    def get_results(self, model, _x_train, _y_train, x_test, y_test, _w_train, w_test):

        _df_train = pd.DataFrame()
        _df_test = pd.DataFrame()

        _df_train['train_labels'] = [i[0] for i in _y_train.as_matrix()]
        _df_test['test_labels'] = [i[0] for i in y_test.as_matrix()]
        _df_train['index'] = _x_train.index

        _df_train['train_output'] = model.predict(_x_train, verbose=0)
        _df_test['test_output'] = model.predict(x_test, verbose=0)

        _df_train['train_weights'] = _w_train.as_matrix()
        _df_test['test_weights'] = w_test.as_matrix()

        _df_test['train_pred'] = _df_train['train_output'].apply(lambda x: 1 if x > 0.5 else 0)
        _df_test['test_pred'] = _df_test['test_output'].apply(lambda x: 1 if x > 0.5 else 0)
        _df_test['index'] = x_test.index
        return _df_train, _df_test


    
    



    
def cv_train(train_x, train_y, train_w, num_folds):
    """
    """
    
    return
    
def evaluate_model():
    """
    """
    return
    
    
    