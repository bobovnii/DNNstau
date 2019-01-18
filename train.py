"""
Script for the pretraining:
"""
# MLP for Pima Indians Dataset Serialize to JSON and HDF5
from keras.models import Sequential
from keras.layers import Dense, Dropout
from keras.models import model_from_json
from keras.optimizers import Adam
import matplotlib.pyplot as plt
import os
import math
import uproot
import logging

import numpy as np
import pandas as pd
np.random.seed(7)


class Training():
    """

    """

    def __init__(self, config):
        """

        :return:
        """
        self.config = config
        self.dir = ""

        ##Model Properties
        try:
            self.batch_size = int(config.get("model", "batch_size"))
        except Exception:
            self.batch_size = 4000

        try:
            self.epochs = int(config.get("model", "epochs"))
        except Exception:
            self.epochs = 2


        ##Training Monitoring:
        try:
            self._plot_history = int(config.get("model", "plot_history"))
        except Exception:
            self._plot_history = False

        try:
            self._store_history = int(config.get("model", "store_history"))
        except Exception:
            self._store_history = False

        #Training store
        try:
            self.dir = config("model", 'dir')
            print("DIR", self.dir)
        except Exception:
            if self._store_history==False and self._plot_history==False:
                raise Exception

        #Training Name
        try:
            self.model_name = config("model", 'model_name')
        except Exception:
            self.model_name = "DNN"

        print(self.dir)
        #if not os.path.exists(self.dir):
        #    os.mkdir(self.dir + self.model_name)


        return

    def _model(self):
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
        self.__model = model
        return model


    def get_model(self):

        return self.__model


    def train(self, X, Y , model=None):
        """


        :return:
        """
        if model == None:
            model = self.__model

        history = model.fit(X, Y, epochs=self.epochs, batch_size=self.batch_size)  # , verbose=0)

        if self._plot_history:
            """
            Plot the history
            """
            self.plot_history(history, self.model_name)

        if self._store_history:
            """
            Store history of training
            """

        return



    def store_model(self, model=None, model_name=None):
        """

        :return:
        """
        if model is None:
            model = self.__model
        if model_name is None:
            model_name = self.model_name

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
        if model is None:
            model = self.__model

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
            model = self.__model
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


    def plot_history(self, history, training_name):
        """"

        """
        # summarize history for accuracy

        plt.plot(history.history['acc'])
        plt.title('model accuracy')
        plt.ylabel('accuracy')
        plt.xlabel('epoch')
        plt.legend(['train', 'test'], loc='upper left')
        plt.savefig(os.path.join(self.dir, 'Accuracy_{}.pdf'.format(training_name)))
        #plt.show()

        # summarize history for loss
        plt.plot(history.history['loss'])
        plt.title('model loss')
        plt.ylabel('loss')
        plt.xlabel('epoch')
        plt.legend(['train', 'test'], loc='upper left')
        plt.savefig(os.path.join(self.dir, 'Loss_{}.pdf'.format(training_name)))
        #plt.show()

        return
    



    
def cv_train(train_x, train_y, train_w, num_folds):
    """
    """
    
    return
    
def evaluate_model():
    """
    """
    return
    
    
    